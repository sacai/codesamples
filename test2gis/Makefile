CC=g++
CFLAGS = -std=gnu++11 -fpack-struct -Wno-pragmas -Wpadded -I common -static
LFLAGS = -static
OFILES= \
sys_io.o \
sys_mem.o \
util.o \
appcmdl.o \
cmdline.o \
sarray.o \
textbase.o \
textfile.o \
findword.o \
checksum.o \
test2gis.o

all: bin/test2gis

bin/test2gis : $(OFILES) $(LDLIBS) 
	@echo ---------------------------------------------------------------------
	$(CC) $(LDFLAGS) $(LFLAGS) -o $@ $(OFILES) $(LDLIBS) 2>test2gis.link.err
	@if ! test -s test2gis.link.err; then rm test2gis.link.err; fi
	@echo ---------------------------------------------------------------------

clean:
	rm -f $(OFILES) bin/test2gis *.err

.cpp.o:
	$(CC) $(CCFLAGS) $(CFLAGS) -c $< 2>$<.err
	@if ! test -s $<.err; then rm $<.err; fi

.c.o:
	$(CC) $(CCFLAGS) $(CFLAGS) -c $< 2>$<.err
	@if ! test -s $<.err; then rm $<.err; fi

sys_io.o: common/sys_io.c common/sys_io.h
	$(CC) $(CCFLAGS) $(CFLAGS) -c $< 2>$<.err
	@if ! test -s $<.err; then rm $<.err; fi

sys_mem.o: common/sys_mem.c common/sys_mem.h
	$(CC) $(CCFLAGS) $(CFLAGS) -c $< 2>$<.err
	@if ! test -s $<.err; then rm $<.err; fi

util.o: common/util.c common/util.h
	$(CC) $(CCFLAGS) $(CFLAGS) -c $< 2>$<.err
	@if ! test -s $<.err; then rm $<.err; fi

appcmdl.o: common/appcmdl.cpp common/appcmdl.h
	$(CC) $(CCFLAGS) $(CFLAGS) -c $< 2>$<.err
	@if ! test -s $<.err; then rm $<.err; fi

cmdline.o: common/cmdline.cpp common/cmdline.h
	$(CC) $(CCFLAGS) $(CFLAGS) -c $< 2>$<.err
	@if ! test -s $<.err; then rm $<.err; fi

sarray.o: common/sarray.cpp common/sarray.h
	$(CC) $(CCFLAGS) $(CFLAGS) -c $< 2>$<.err
	@if ! test -s $<.err; then rm $<.err; fi

textbase.o: common/textbase.cpp common/textfile.h
	$(CC) $(CCFLAGS) $(CFLAGS) -c $< 2>$<.err
	@if ! test -s $<.err; then rm $<.err; fi

textfile.o: common/textfile.cpp common/textfile.h
	$(CC) $(CCFLAGS) $(CFLAGS) -c $< 2>$<.err
	@if ! test -s $<.err; then rm $<.err; fi

findword.o: findword.cpp findword.h
	$(CC) $(CCFLAGS) $(CFLAGS) -c $< 2>$<.err
	@if ! test -s $<.err; then rm $<.err; fi

checksum.o: checksum.cpp checksum.h
	$(CC) $(CCFLAGS) $(CFLAGS) -c $< 2>$<.err
	@if ! test -s $<.err; then rm $<.err; fi

test2gis.o: test2gis.cpp
	$(CC) $(CCFLAGS) $(CFLAGS) -c $< 2>$<.err
	@if ! test -s $<.err; then rm $<.err; fi

