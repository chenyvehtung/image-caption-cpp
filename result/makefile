CC=g++-4.8 -std=c++11
CFLAGS = -c -Wall

all: demo

demo: demo.o htmlgen.o
	$(CC) demo.o htmlgen.o -o demo

demo.o: demo.cpp
	$(CC) $(CFLAGS) demo.cpp

htmlgen.o: htmlgen.cpp
	$(CC) $(CFLAGS) htmlgen.cpp

clean:
	rm *.o demo

