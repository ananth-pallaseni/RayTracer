CC = g++
RM = /bin/rm -f


all: main

main: Raytracer.o
	$(CC) RayTracer.0 -o as2

RayTracer.o: Raytracer.cpp
	$(CC) -c Raytracer.cpp -o Raytracer.o

clean: 
	rm -f *.o as2

