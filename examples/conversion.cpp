
#include <date/date.h>
#include <date/tz.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cstdint>
#include <ratio>
#include <limits>
#include <type_traits>

namespace convert { namespace clock {
	namespace ch=std::chrono;
	template <class T,class R> T data(const ch::duration<T,R>& du ){
		return du.count();
	}
	template <class C,class D, typename T= typename D::Rep> T data( const ch::time_point<C,D>& tp ){
		return data( tp.time_since_epoch() );
	}}

	namespace ch=std::chrono;
	template<class T, class F> T as( const F& ref ){
		static_assert(true, "default converter called, no conversion took place!!!");
		return T();
	}
/*	template<class T,class R, class Rep> T as(
			const ch::duration< typename std::enable_if< !std::is_same<T,Rep>::value,Rep>::type, R>& du ){
		return T( R::num * clock::data(du)) / R::den ;
	}
	*/
	template<class T,class R, class Rep> T as(
			const ch::duration<Rep,R>& du ){
		return T( R::num * clock::data(du)) / R::den ;
	}
}

namespace enable {
	template <typename T> using when_float = typename std::enable_if< std::is_floating_point<T>::value, T>::type;
	template <typename T> using when_int   = typename std::enable_if<!std::is_floating_point<T>::value, T>::type;
	template <typename A, typename B, typename C=A> using when_same  = typename std::enable_if<std::is_same<A,B>::value, C>::type;
	template <typename A, typename B, typename C=A> using when_different  = typename std::enable_if<!std::is_same<A,B>::value, C>::type;
}

template <typename T> std::string do_stuff( enable::when_int<T> t ){
	return " an implementation for integral types (int, char, unsigned, etc.)";
}

template <typename T> std::string do_stuff( enable::when_float<T> t ){
	return " an implementation for float types ";
}

template <typename A,typename B> std::string do_stuff( enable::when_same<A,B> a, enable::when_same<B,A> b  ){
	std::stringstream buf;
	buf << a << " + " << b <<" = " << a+b << "  [same]";
	return buf.str();
}

template <typename A, typename B> std::string do_stuff( enable::when_different<A,B> a, enable::when_different<B,A> b ){
	std::stringstream buf;
	buf << a << " + " << b <<" = " << a+b << "  [different]";
	return buf.str();
}




/**  Rdate-time
 *
 *	floating point representation with fractional seconds (mantissa):   R POSIXct clock represents date-time such
 *  NA/NaN/not-a-datetime: denoting not-yet-obtained values, should work for integers?
 *   		
*/
struct SomeClock {
	using period = std::ratio<1>;
	using rep = double;
    using duration = std::chrono::duration<rep,period>;
	using time_point = std::chrono::time_point<SomeClock>;
    static constexpr bool is_steady = false;

	static constexpr char NA[] = "N/A";
	static constexpr date::sys_days epoch = date::year{1970}/date::month{1}/date::day{3};

	template <typename Rep> static std::chrono::time_point<SomeClock,duration> nan(){
		namespace ch = std::chrono;
		return ch::time_point<SomeClock>( duration( std::numeric_limits<Rep>::quiet_NaN() ) );
	}

	template<typename T> static std::chrono::time_point<std::chrono::system_clock, T>
    to_sys(const std::chrono::time_point<SomeClock, SomeClock::duration>& from){
		using Type = typename T::rep;
		using ratio_e =  std::ratio_divide<date::sys_days::period,period>;
		using ratio_s =  std::ratio_divide<period, typename T::period>;

		constexpr rep e =  ratio_e::num / static_cast<rep>( ratio_e::den );
		constexpr Type s =  ratio_s::num / ratio_s::den;
		Type ticks = static_cast<Type>( s*from.time_since_epoch().count() +  e * s * epoch.time_since_epoch().count() );
		return date::sys_time<T>{ T( ticks ) };
    }

	template<typename F> static std::chrono::time_point<SomeClock, SomeClock::duration>
    from_sys(const date::sys_time<F>& from){
		using Type = rep;
		using ratio_e =  std::ratio_divide<date::sys_days::period,period>;
		using ratio_s =  std::ratio_divide<typename F::period,period>;

		constexpr Type e =  ratio_e::num / static_cast<Type>( ratio_e::den );
		constexpr Type s =  ratio_s::num / static_cast<Type>( ratio_s::den );

		return time_point{
			duration( s * from.time_since_epoch().count() - e * epoch.time_since_epoch().count()  ) };
    }

	static time_point now() noexcept {
		using F = std::chrono::system_clock::duration;
        return from_sys<F>(
				std::chrono::system_clock::now() );
    }
};
constexpr date::sys_days SomeClock::epoch;
constexpr char SomeClock::NA[];

template <class CharT, class Traits, class Duration> std::basic_ostream<CharT, Traits>& to_stream(
		std::basic_ostream<CharT, Traits>& os, const CharT* fmt, const std::chrono::time_point<SomeClock,Duration>& tp){
	using namespace date;
	using namespace std;
	if( !std::isnan(tp.time_since_epoch().count()) )

		// FIXME: this lines indicates that I didn't get the idea how <date/date.h> does conversion
		//auto aa =
		//	date::clock_cast<std::chrono::system_clock>(tp);

		return date::to_stream(os, fmt,
			SomeClock::to_sys<std::chrono::nanoseconds>(tp) );
	os << SomeClock::NA;
	return os;
}
template <class CharT, class Traits, class Duration> std::basic_ostream<CharT, Traits>&
		operator<<(std::basic_ostream<CharT, Traits>& os, const std::chrono::time_point<SomeClock,Duration>& tp) {

	const CharT fmt[] = {'%', 'F', ' ', '%', 'T', CharT{}};
    return to_stream(os, fmt,tp);
}

template <class Duration, class CharT, class Traits, class Alloc = std::allocator<CharT>> std::basic_istream<CharT, Traits>& 
	from_stream(std::basic_istream<CharT, Traits>& is, const CharT* fmt, std::chrono::time_point<SomeClock,Duration>& tp,
	   								std::basic_string<CharT, Traits, Alloc>* abbrev = nullptr, std::chrono::minutes* offset = nullptr) {
	using Clock = std::chrono::system_clock;
	using Type = SomeClock::rep;

	Clock::time_point tp_;
    date::from_stream(is, fmt, tp_, abbrev, offset);
	tp = SomeClock::from_sys<Clock::duration>(tp_);
    return is;
}



//template <typename T> std::duration<>


int main(){

	using namespace date;
	using namespace std;
	namespace ch = std::chrono;
	namespace dt = convert::clock;
	constexpr static int start = 0;

	std::cout <<"\n\n";
	auto tp_d = SomeClock::nan<double>(); std::cout << "double un-init: " << tp_d <<"\n";
	auto tp_f = SomeClock::nan<float>(); std::cout  << "float  un-init: " << tp_f <<"\n";
	auto tp_l = SomeClock::nan<long>(); std::cout   << "long   un-init: " << tp_l <<"\n";

	std::istringstream in{"2000-01-01 12:34:56.1234567891"};

	in >> date::parse("%F %T", tp_d ); std::cout <<"double    init: " << tp_d <<"\n"; in.seekg( start );
	in >> date::parse("%F %T", tp_f ); std::cout <<"float     init: " << tp_f <<"\n"; in.seekg( start );
	in >> date::parse("%F %T", tp_l ); std::cout <<"long      init: " << tp_l <<"\n"; in.seekg( start );

	std::cout<<"\n rounding of double: \n";
	std::cout << date::floor<days>( tp_d ) << "\n";
	std::cout << date::floor<ch::hours>( tp_d ) << "\n";
	std::cout << date::floor<ch::minutes>( tp_d ) << "\n";
	std::cout << date::floor<ch::seconds>( tp_d ) << "\n";
	std::cout << date::floor<ch::milliseconds>( tp_d ) << "\n";
	std::cout << date::floor<ch::microseconds>( tp_d ) << "\n";

	std::cout<<"\n rounding of float: \n";
	std::cout << date::floor<days>( tp_f ) << "\n";
	std::cout << date::floor<ch::hours>( tp_f ) << "\n";
	std::cout << date::floor<ch::minutes>( tp_f ) << "\n";
	std::cout << date::floor<ch::seconds>( tp_f ) << "\n";
	std::cout << date::floor<ch::milliseconds>( tp_f ) << "\n";
	std::cout << date::floor<ch::microseconds>( tp_f ) << "\n";

	std::cout<<"\n rounding of long: \n";
	std::cout << date::floor<days>( tp_l ) << "\n";
	std::cout << date::floor<ch::hours>( tp_l ) << "\n";
	std::cout << date::floor<ch::minutes>( tp_l ) << "\n";
	std::cout << date::floor<ch::seconds>( tp_l ) << "\n";
	std::cout << date::floor<ch::milliseconds>( tp_l ) << "\n";
	std::cout << date::floor<ch::microseconds>( tp_l ) << "\n";

	std::cout << weekday{jul/4/2001} << '\n';
    using namespace std::chrono;
	{
		std::cout.precision(14);
		auto du = ch::duration<double>( 1'000.123'456'789'123 );
		std::cout <<"auto du = ch::duration<double>( 1'000.123'456'789'123'00 );\n"
		<<"\tduration: " << du <<"\n"
		<<"\tdata:     " << std::fixed<< convert::clock::data( du ) <<"\n"
		<<"\tdouble:   " << convert::as<double>( du ) << "\n"
		<<"\tfloat:    " << convert::as<float>( du ) << "\n"
		<<"\tlong:     " << convert::as<long>( du )   << "\n"
		<<"\tshort:    " << convert::as<short>( du ) << "\n"
		<<"\n";
	}
	{
		std::cout.precision(14);
		auto du = ch::duration<long,std::pico>( 123'456'789'123 );
		std::cout <<"auto du = ch::duration<long,std::pico>( 123'456'789'123 );\n"
		<<"\tduration: " <<  du <<"\n"
		<<"\tdata:     " << std::fixed<< convert::clock::data( du ) <<"\n"
		<<"\tdouble:   " << convert::as<double>( du ) << "\n"
		<<"\tfloat:    " << convert::as<float>( du ) << "\n"
		<<"\tlong:     " << convert::as<long>( du ) << "\n"
		<<"\tshort:    " << convert::as<short>( du ) << "\n"
		;
	}


	std::cout << " \n"
	<< "integer:   " << do_stuff<int>( 123 ) << "\n"
	<< "double:    " << do_stuff<double>( 123.00 ) << "\n"
	<< "same:      " << do_stuff<int,int>( 123, 12 ) << "\n"
	<< "different: " << do_stuff<double,int>( 123.00, 2 ) << "\n"
	<< "same:      " << do_stuff<long,long>( 123, 12 ) << "\n"
	<< "different: " << do_stuff<double,int>( 123.00, 2 ) << "\n"
;

	//"as<double>( du_ ): " << convert::as<double>( date::floor<ch::milliseconds>( du ) ) <<"\n"
	//"as<double>(): " << convert::as<long>( ch::duration<long,std::deca>(123456789) ) <<"\n"
	//<<std::endl;
	
	typedef duration<int, ratio_multiply<hours::period, ratio<24>>> days;
	std::cout << "days: " << days(3) << " mins: " << ch::minutes(200) <<"\n\n" ;

	std::cout<<"\n\n------------------------------------------------------\n\n";
    using namespace date;
    using namespace std::chrono;
    auto zt = make_zoned(current_zone(), local_days{2018_y/jan/15} );
	auto today = local_days{2018_y/jan/15}  ;
	std::cout << zt << " " << today <<" "<< today + ch::hours(10)<<  "\n" ;



}
