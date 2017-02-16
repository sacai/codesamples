CC=g++
CFLAGS  = -std=gnu++11 -march=native -Wno-pragmas -Wpadded -Wwrite-strings
OFILES= test.o
DEP=minicurs.o


BUILD_NUMBER_FILE=build.txt
BUILD_NUMBER_FLAGS  = -D__BUILD_DATE=$$(date +'%Y%m%d')
BUILD_NUMBER_FLAGS += -D__BUILD_NUMBER=$$(cat $(BUILD_NUMBER_FILE))

all: libminicurs.a ./test $(BUILD_NUMBER_FILE)

./test : $(OFILES) libminicurs.a 
	@echo ---------------------------------------------------------------------
	$(CC) $(LDFLAGS) $(LFLAGS) -o $@ $(OFILES) libminicurs.a 2>test.link.err
	@if ! test -s test.link.err; then rm test.link.err; fi
	@echo -- build number $$(cat $(BUILD_NUMBER_FILE)) from $$(date +'%Y%m%d') --
	@echo ---------------------------------------------------------------------

libminicurs.a: $(DEP)
	@echo ---------------------------------------------------------------------
	ar -q libminicurs.a $(DEP)
	@echo -- build number $$(cat $(BUILD_NUMBER_FILE)) from $$(date +'%Y%m%d') --
	@echo ---------------------------------------------------------------------

clean:
	rm -f $(OFILES) $(DEP) ./libminicurs.a ./test *.err

.c.o:
	$(CC) $(BUILD_NUMBER_FLAGS) $(CCFLAGS) $(CFLAGS) -c $< 2>$<.err
	@if ! test -s $<.err; then rm $<.err; fi

test.o: test.c
minicurs.o: minicurs.c

$(BUILD_NUMBER_FILE): $(OFILES)
	@if ! test -f $(BUILD_NUMBER_FILE); then echo 0 > $(BUILD_NUMBER_FILE); fi
	@echo $$(($$(cat $(BUILD_NUMBER_FILE)) + 1)) > $(BUILD_NUMBER_FILE)
