EXE=test

CXXFLAGS=-Wall -Wextra -g -std=c++11 -O0

.PHONY: verify all

all: $(EXE)

$(EXE): test.cpp safelist.hpp
	$(CXX) -o $@ $(CXXFLAGS) $<

verify: reference.out actual.out
	diff $^

reference.out: $(EXE)
	valgrind --leak-check=full ./$< -r > $@ 2> /dev/null

actual.out: $(EXE)
	valgrind --error-exitcode=1 --leak-check=full ./$< > $@
