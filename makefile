#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'screensaver'
# 'make clean'  removes all .o and executable files
##

#Compiler in use
NUM_THREADS=5
CC=g++

#Compile time Flags
CFLAGS= -std=c++11

#Libraries to include during Cmpilation 
LIBRARIES= -lglut -lGL -lGLU -lm -lpthread

#The C++ source files
SOURCES= assignment.cpp imageloader.cpp imageloader.h

#The C++ object files
objects= $(SOURCES:.c=.o)

execute: a.out
	./g $(NUM_THREADS)
#Main executable file screensaver
a.out: $(objects)
	g++ $(CFLAGS) $(objects) $(LIBRARIES) -o g

assignment.o: assignment.cpp imageloader.h imageloader.cpp
	g++ assignment.cpp $(CFLAGS) $(LIBRARIES)

imageloader.o: imageloader.cpp 
	g++ imageloader.cpp $(CFLAGS) $(LIBRARIES)

clean :
	rm a.out


##.PHONY : clean
##clean:
##	rm screensaverApp	

##.PHONY : print


