CC=g++
CFLAGS  =  -std=gnu++11 -Wno-pragmas -Wpadded -D_MK_PARALLEL_ -O2
#LDLIBS=-lrt -lpthread
OFILES= mkcalcpool.o mkcalctest.o

BUILD_NUMBER_FILE=build.txt
BUILD_NUMBER_FLAGS  = -D__BUILD_DATE=$$(date +'%Y%m%d')
BUILD_NUMBER_FLAGS += -D__BUILD_NUMBER=$$(cat $(BUILD_NUMBER_FILE))

all: ../bin/mkcalctest $(BUILD_NUMBER_FILE)

../bin/mkcalctest : $(OFILES) $(LDLIBS) 
	@echo ---------------------------------------------------------------------
	$(CC) $(LDFLAGS) $(LFLAGS) -o $@ $(OFILES) $(LDLIBS) 2>mkcalctest.link.err
	@if ! test -s mkcalctest.link.err; then rm mkcalctest.link.err; fi
	@echo -- build number $$(cat $(BUILD_NUMBER_FILE)) from $$(date +'%Y%m%d') --
	@echo ---------------------------------------------------------------------

clean:
	rm -f $(OFILES) ../bin/mkcalctest *.err

.cpp.o:
	$(CC) $(BUILD_NUMBER_FLAGS) $(CCFLAGS) $(CFLAGS) -c $< 2>$<.err
	@if ! test -s $<.err; then rm $<.err; fi

mkcalcpool.o: mkcalcpool.cpp mkcalcpool.hpp
mkcalctest.o: mkcalctest.cpp

$(BUILD_NUMBER_FILE): $(OFILES)
	@if ! test -f $(BUILD_NUMBER_FILE); then echo 0 > $(BUILD_NUMBER_FILE); fi
	@echo $$(($$(cat $(BUILD_NUMBER_FILE)) + 1)) > $(BUILD_NUMBER_FILE)
