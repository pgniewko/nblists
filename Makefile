RM          := rm -f
MKDIR	    := mkdir -p
# C compiler
CC	     	:= g++-7

STATIC		:=libnblists.a
EXE			:=run_nblists

SRC			:=./src
PREFIX      := /usr/local
INCLUDE     :=$(PREFIX)/include

CFLAGS 		:= -O3 -I$(INCLUDE)
CXXFLAGS	:= -O3 -std=gnu++11 -I$(INCLUDE)
LDLIBS   	:= -lnblists 

SOURCES	     := $(shell find $(SRC) -type f -name "*.cpp" -or -name "*.c")

HEADERS	     := $(shell find $(SRC) -type f -name "*.h" -not -name "fastmath.h" )

OBJECTS      := $(patsubst %.cpp,%.o,$(patsubst %.c,%.o, $(SOURCES) ))


$(STATIC): $(OBJECTS)
	@echo "[Link (Static)]"
	ar -rcs $@ $^
	ranlib $@

$(EXE): main.cpp
	@echo [Building exe]
	$(CC) -o $@ $^ $(CXXFLAGS) $(LDLIBS)

%.o: %.cpp %.h
	@echo [Compile C++ files] $<
	$(CC) -c $(CXXFLAGS) $< -o $@

%.o: %.c %.h
	@echo [Compile C files] $<
	$(CC) -c $(CFLAGS) $< -o $@

PHONY: exe build clean install

exe: $(EXE)

build: $(STATIC)
	@echo [Building]

clean:
	@echo [Cleaning]
	$(RM) $(STATIC) $(EXE) $(OBJECTS) 

install: $(STATIC)
	@echo [Installing] $<
	sudo install -m 755 $(STATIC) $(PREFIX)/lib
	sudo install -m 755 $(HEADERS) $(PREFIX)/include
