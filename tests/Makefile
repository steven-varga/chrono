# Copyright (c) 2018 vargaconsulting, Toronto,ON Canada
# Author: Varga, Steven <steven@vargaconsulting.ca>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this  software  and associated documentation files (the "Software"), to deal in
# the Software  without   restriction, including without limitation the rights to
# use, copy, modify, merge,  publish,  distribute, sublicense, and/or sell copies
# of the Software, and to  permit persons to whom the Software is furnished to do
# so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE  SOFTWARE IS  PROVIDED  "AS IS",  WITHOUT  WARRANTY  OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT  SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY  CLAIM,  DAMAGES OR OTHER LIABILITY, WHETHER
# IN  AN  ACTION  OF  CONTRACT, TORT OR  OTHERWISE, ARISING  FROM,  OUT  OF  OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

### 
# SEE README to install GTEST 

CXX = g++
CXXFLAGS =  -I../ -std=c++14
LIBS = -lprofiler -lglog -lgtest -lgtest_main -ltz -lcurl
flags =  --gtest_print_time=0 --gtest_color=auto

apps =
all: $(apps)

%.o : $(SRC_DIR)/%.cpp 
	$(CXX) -$(INCLUDES) -o $@  $(CPPFLAGS) $(CXXFLAGS) -c $^

test: 

clean:
	@$(RM) *.o  $(apps)

.PHONY: test

