FLAGS=-std=c++11 -Wall -Wextra -O2

all: makedir pmt clean

pmt: pmt.o pmt_backend.o
	g++ $(FLAGS) pmt.o pmt_backend.o -o ../bin/pmt

pmt.o: pmt.cpp
	g++ $(FLAGS) -c pmt.cpp

pmt_backend.o: pmt_backend.cpp
	g++ $(FLAGS) -c pmt_backend.cpp

clean:
	rm -rf *o pmt

makedir:
	mkdir -p ../bin/