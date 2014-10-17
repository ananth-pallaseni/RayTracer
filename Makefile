CC = g++
RM = /bin/rm -f


all: main

main: RayTracer.o
	$(CC) RayTracer.o -o as2

RayTracer.o: RayTracer.cpp
	$(CC) -c RayTracer.cpp -o RayTracer.o

clean: 
	rm -f *.o as2

