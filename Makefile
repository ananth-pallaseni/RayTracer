CC = g++
RM = /bin/rm -f
CFLAGS = -g -o2


all: main

main: Scene.o RayTracer.o Sampler.o Canvas.o lodepng.o
	$(CC) $(CFLAGS) Scene.o Sampler.o RayTracer.o Canvas.o lodepng.o -o as2

Scene.o: Scene.cpp
	$(CC) $(CFLAGS) -c Scene.cpp -o Scene.o

RayTracer.o: RayTracer.cpp 
	$(CC) $(CFLAGS) -c RayTracer.cpp -o RayTracer.o

Sampler.o: Sampler.cpp
	$(CC) $(CFLAGS) -c Sampler.cpp -o Sampler.o

Canvas.o: Canvas.cpp 
	$(CC) $(CFLAGS) -c Canvas.cpp -o Canvas.o

lodepng.o: lodepng.cpp
	$(CC) $(CFLAGS) -c lodepng.cpp -o lodepng.o

# test.o: test.cpp
# 	$(CC) $(CFLAGS) -c test.cpp -o test.o

# test: main 
# 	./as2 test

clean: 
	rm -f *.o as2 *.png

