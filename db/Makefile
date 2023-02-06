SIMPLEKV_DIR=/home/lj/util/app/simpleKV
UTIL_DIR=/home/lj/util/cpp-util
CC=g++
CFLAGS=-std=c++11 -c -g -Wall -fgnu-tm  -I$(SIMPLEKV_DIR) -I$(UTIL_DIR)
INCLUDES=-I../
SOURCES=$(wildcard *.cc)
OBJECTS=$(SOURCES:.cc=.o)

all: $(SOURCES) $(OBJECTS)

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	$(RM) $(OBJECTS)

