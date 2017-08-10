CXXFLAGS=-Wall -Wextra -g -std=c++11 -O0

all: test

test: test.cpp safelist.hpp
	$(CXX) -o $@ $(CXXFLAGS) $<

verify: reference.out actual.out
	diff $^

reference.out: test
	./test -r > $@ 2> /dev/null

actual.out: test
	valgrind --error-exitcode=1 --leak-check=full ./test > $@
