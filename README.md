<!---

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
--->

<!--
TODO:
	address leap second treatment
 -->

C++11 Chrono abstraction layer for Popular Date Time representations    {#link-chrono}
======================================================================================

Howard Hinnan [date-time][19] header library extends [C++11 std::chrono][13] with date support and
extensive conversion capability between [custom clocks][21] and system clocks of numeric underlying
datatype.  The library provides the familiar [formatting][20] parameters, and parses the entire time
zone database without error.  This header only library is build on [date-time][19] templates, wraps
the respective data-formats and provide easy conversions between commonly used time formats.

List of Clocks: RClock, NETClock, IntraDayClock

[R POSIXct][6]
--------------
internally uses  [IEEE 754][8] double precision float. This format allows
representing missing values as `NA`-s as well provide sub-second resolution and direction
from epoch using the sign bit. The format is convenient  as you can store date-time
in a matrix next to other numeric features and call your  BLAS/LAPACK operation.  The
downside is only 52 bit is used to represent time,  the [roundoff error][11] inherent in floats
[cause arguments][10] as well the epoch which maybe tied to [unix epoch][9]. Dirk Eddelbuettel's [article][12] and
implementation shows insight how R represents date-time in posix format.

Here are the planned templates to represent POSIXct type with C++11 chrono compatible templates:
``` cpp
	using RClock = chrono::Clock< date::year{1980}/date::month{1}/date::day{1},      // epoch
									std::chrono::duration<double,std::ratio<1,1>> >; // representation and resolution
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


[1]:  http://www.boost.org/doc/libs/master/doc/html/date_time/details.html#date_time.buildinfo)
[2]:  \ref link-chrono
[3]:  \ref link-examples-chrono
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

