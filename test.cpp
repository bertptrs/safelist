#include "safelist.hpp"
#include <list>
#include <iostream>
#include <typeinfo>

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

template<class T>
void setup_list(T& l)
{
	l.push_front(2);
	l.push_front(1);
	l.push_back(3);
}


template<class T>
void test()
{
	std::cerr << "Testing implementation " << typeid(T).name() << std::endl;
	T t;
	setup_list(t);

	test_iters(t);
	test_iters((const T) t);
}

int main()
{
	test<std::list<int>>();
	test<safelist<int>>();
	return 0;
}
