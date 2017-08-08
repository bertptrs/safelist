CXXFLAGS=-Wall -Wextra -g -std=c++14 -O0

all: test

test: test.cpp safelist.hpp
	$(CXX) -o $@ $(CXXFLAGS) $<
