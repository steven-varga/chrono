#  _____________________________________________________________________________
#
#  Copyright (c) <2018> <copyright Steven Istvan Varga, Toronto, On>
#
#  Contact: Steven Varga
#           steven@vargaconsulting.ca
#           2018 Toronto, On Canada
#  _____________________________________________________________________________
#

PREFIX = /usr/local

DIRS =  tests examples doxy

BUILDDIRS = $(DIRS:%=build-%)
CLEANDIRS = $(DIRS:%=clean-%)
TESTDIRS = $(DIRS:%=test-%)

all: $(BUILDDIRS)
$(DIRS): $(BUILDDIRS)
$(BUILDDIRS):
	$(MAKE) -C $(@:build-%=%)

install: 
	cp -rf clock.h $(PREFIX)/include
	cp clock.pc  $(PREFIX)/lib/pkgconfig

test: $(TESTDIRS) all
$(TESTDIRS): 
	$(MAKE) -C $(@:test-%=%) test

clean: $(CLEANDIRS)
$(CLEANDIRS): 
	$(MAKE) -C $(@:clean-%=%) clean

.PHONY: subdirs $(DIRS)
.PHONY: subdirs $(BUILDDIRS)
.PHONY: subdirs $(TESTDIRS)
.PHONY: subdirs $(CLEANDIRS)
.PHONY: all install clean tests


