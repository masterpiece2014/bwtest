CC=g++
CXX_FLAGS=-std=c++11 -c -O2
all: libbwtest.a

libbwtest.a: testio.o  testbase.o nullostream.o  timeutil.o benchmark.o bwtest.o
	ar -r -c -s libbwtest.a benchmark.o bwtest.o nullostream.o testbase.o testio.o timeutil.o

timeutil.o: 
	$(CC) $(CXX_FLAGS) -lrt timeutil.h -o timeutil.o

testio.o:
	$(CC) $(CXX_FLAGS) testio.h -o testio.o

testbase.o:
	$(CC) $(CXX_FLAGS) testbase.h -o testbase.o

nullostream.o:
	$(CC) $(CXX_FLAGS) nullostream.h -o nullostream.o

benchmark.o:
	$(CC) $(CXX_FLAGS)  benchmark.h -o benchmark.o

bwtest.o:
	$(CC) $(CXX_FLAGS)  bwtest.h -o bwtest.o

clean:
	rm -rf *.o libbwtest.a