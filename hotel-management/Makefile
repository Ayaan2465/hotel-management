CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2

hotel: main.cpp $(wildcard *.h)
	$(CXX) $(CXXFLAGS) main.cpp -o hotel

run: hotel
	./hotel

clean:
	rm -f hotel hotel.exe

.PHONY: run clean
