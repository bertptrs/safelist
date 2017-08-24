#include "safelist.hpp"
#include <cstdint>
#include <list>
#include <random>
#include <iostream>

using namespace std;

template<class T>
void stress(int count)
{
	T t;

	// Initialize with random data
	mt19937_64 r((random_device())());

	for (;count > 0; --count) {
		t.push_back(r());
	}

	t.sort();
}

int main(int argc, char** argv)
{

	assert(argc >= 2);
	if (argc == 2) {
		for (int i = 100; i > 0; --i) {
			stress<safelist<uint64_t>>(atoi(argv[1]));
		}
	} else {
		for (int i = 100; i > 0; --i) {
			stress<list<uint64_t>>(atoi(argv[2]));
		}
	}

	return 0;
}
