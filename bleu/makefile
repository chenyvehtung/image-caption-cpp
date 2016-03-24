CC=g++-4.8 -std=c++11
CFLAGS = -c -Wall

all: demo

demo: demo.o bleu.o
	$(CC) demo.o bleu.o -o demo

demo.o: demo.cpp
	$(CC) $(CFLAGS) demo.cpp

bleu.o: bleu.cpp
	$(CC) $(CFLAGS) bleu.cpp

clean:
	rm *.o demo
