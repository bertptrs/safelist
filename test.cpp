#include "safelist.hpp"
#include <iostream>

template<class T>
void test_iters(T& sl)
{
	// Test prefix increment
	for (auto i : sl) {
		std::cout << i << std::endl;
	}

	// Test postfix increment
	for (auto it = sl.begin(); it != sl.end(); it++) {
		std::cout << *it << std::endl;
	}

	// Test reverse iteration
	auto it = sl.end();
	do {
		std::cout << *(--it) << std::endl;
	} while (it != sl.begin());
}

int main()
{
	safelist<int> sl;

	// Test push front/back
	sl.push_front(2);
	sl.push_front(1);
	sl.push_back(3);

	test_iters(sl);
	test_iters((const safelist<int>) sl);

	return 0;
}
