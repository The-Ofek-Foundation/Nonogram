SOURCE_CPP = src/*.cpp

all: main.cpp src/*.cpp src/*.hpp
	mkdir -p bin/
	g++ -o bin/main main.cpp $(SOURCE_CPP) -O3 -Wall -funroll-loops -std=c++2a

clean:
	rm -rfv bin/