CC = g++
RM = /bin/rm -f
CFLAGS = -g


all: main

main: RayTracer.o Sampler.o 
	$(CC) $(CFLAGS) Sampler.o RayTracer.o test.o -o as2

RayTracer.o: RayTracer.cpp 
	$(CC) $(CFLAGS) -c RayTracer.cpp -o RayTracer.o

Sampler.o: Sampler.cpp
	$(CC) $(CFLAGS) -c Sampler.cpp -o Sampler.o

# test.o: test.cpp
# 	$(CC) $(CFLAGS) -c test.cpp -o test.o

# test: main 
# 	./as2 test

clean: 
	rm -f *.o as2

