<!--

 Copyright (c) 2017 vargaconsulting, Toronto,ON Canada
 Author: Varga, Steven <steven@vargaconsulting.ca>

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this  software  and associated documentation files (the "Software"), to deal in
 the Software  without   restriction, including without limitation the rights to
 use, copy, modify, merge,  publish,  distribute, sublicense, and/or sell copies
 of the Software, and to  permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE  SOFTWARE IS  PROVIDED  "AS IS",  WITHOUT  WARRANTY  OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT  SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY  CLAIM,  DAMAGES OR OTHER LIABILITY, WHETHER
 IN  AN  ACTION  OF  CONTRACT, TORT OR  OTHERWISE, ARISING  FROM,  OUT  OF  OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-->

<!--
TODO:
	address leap second treatment
 -->

This header only library is build on top of Howard Hinnan's [date-time][19] templates, wraps
the underlying representation and provides  conversion between `std::chrono::system_clock` and R/.NET/Windows clocks.

Concepts:
----------
[Epoch][9] is the reference point time is counted from. The actual value  is the 
**representation**, and the resolution or the smallest tick of the clock is the [period][23]. Time is measured 
in [duration][22] and may or may not be tied to an **epoch**. The **epoch**, **duration**, **period** and **representation**
define a **clock**. Clock tells the current time: `SomeClock<std::chrono::milliseconds>::now()` and may provide means of
conversion to other clocks. Provided the right conversion one can tap into Howard Hinnan's calandar and time-zone parsing/printing
library: [date-time][19]. The conversion is either automatic, or manual: `date::clock_cast<other_clock>()`.

[R POSIXct][6]
---------------
internally uses  [IEEE 754][8] double precision float, storing fractional seconds in mantissa. This format allows missing values 
as [NAN][24]-s , and to define arbitrary **epoch** called [origin][4].  The [roundoff error][11] inherent in floats [discussed on stack overflow][10].

Here are the planned templates to represent POSIXct type with C++11 chrono compatible templates:

```cpp
using RClock = chrono::Clock< date::year{1970}/date::month{1}/date::day{1}, 	// epoch
				std::chrono::duration<double,std::ratio<1>> >;  				// representation and resolution
```

Use `<date/tz.h>` IO formatting function:

```cpp
	std::cout 
	<< RClock::now()            
	<< date::format("%a %b %d, %Y", RClock::now() );
```

[.NET tick][7]
--------------
Lifted from the website: A single tick represents one hundred nanoseconds or one ten-millionth of a second. There are 10,000
ticks in a millisecond, or 10 million ticks in a second.  The value of this property represents the
number of 100-nanosecond intervals that have elapsed since 12:00:00 midnight, January 1, 0001
(0:00:00 UTC on January 1, 0001, in the Gregorian calendar), which represents DateTime::MinValue.

[C/C++ time_t (legacy) posix time][13]
--------------------------------------
has been around on POSIX systems, conversion is supported between [chrono][14]
and [time_t][13]. The fact that there is no guaranteed underlying representation, and no sub-second
resolution makes it less suitable for [Real Time Bidding][15], [High Frequency Trading][16].

To find out the underlying type:
```bash
echo | gcc -E -xc -include 'time.h' - | grep time_t
```
On my system I get `typedef long int __time_t` same as [you find on  .NET][7] with the difference in
resolution, and epoch.

[BOOST Time][25]


[1]:  http://www.boost.org/doc/libs/master/doc/html/date_time/details.html#date_time.buildinfo
[4]:  https://stat.ethz.ch/R-manual/R-devel/library/base/html/DateTimeClasses.html
[5]:  http://gallery.rcpp.org/articles/parsing-datetimes
[6]:  https://github.com/eddelbuettel/rcppbdt/blob/master/demo/toPOSIXct.R
[7]:  https://msdn.microsoft.com/en-us/library/system.datetime.ticks(v=vs.110).aspx?cs-save-lang=1&cs-lang=cpp#code-snippet-1
[8]:  https://en.wikipedia.org/wiki/Double-precision_floating-point_format
[9]:  https://en.wikipedia.org/wiki/Unix_time
[10]: https://stackoverflow.com/questions/7726034/how-r-formats-posixct-with-fractional-seconds
[11]: https://en.wikibooks.org/wiki/Introduction_to_Numerical_Methods/Rounding_Off_Errors
[12]: http://gallery.rcpp.org/articles/parsing-datetimes/
[13]: http://en.cppreference.com/w/cpp/chrono/c/time
[14]: http://en.cppreference.com/w/cpp/chrono
[15]: https://en.wikipedia.org/wiki/Real-time_bidding
[16]: https://en.wikipedia.org/wiki/High-frequency_trading
[17]: https://en.wikipedia.org/wiki/ISO_8601
[18]: https://forum.openoffice.org/en/forum/viewtopic.php?f=9&t=79046
[19]: https://github.com/HowardHinnant/date
[20]: https://howardhinnant.github.io/date/date.html#to_stream_formatting
[21]: https://stackoverflow.com/questions/47962806/implicit-conversion-between-c11-clocks-time-points/47963216#47963216
[22]: http://naipc.uchicago.edu/2015/ref/cppreference/en/cpp/chrono/duration.html
[23]: http://naipc.uchicago.edu/2015/ref/cppreference/en/cpp/numeric/ratio/ratio.html
[24]: http://en.cppreference.com/w/cpp/types/numeric_limits/quiet_NaN
[25]: http://www.boost.org/doc/libs/1_62_0/doc/html/date_time.html
<!--
1. [boost::gregorian](http://www.boost.org/doc/libs/1_65_1/doc/html/date_time/gregorian.html), [boost::posix_time](http://www.boost.org/doc/libs/1_65_1/doc/html/date_time/posix_time.html) support [in progress]


-->
