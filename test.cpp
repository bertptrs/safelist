#include "safelist.hpp"
#include <list>
#include <iostream>
#include <typeinfo>

template<class T>
void print_list(const T& l)
{
	for (auto x : l) {
		std::cout << x << std::endl;
	}
}

template<class T>
void test_iters(T& sl)
{
	std::cout << "Testing iteration methods" << std::endl;
	// Test prefix increment
	print_list(sl);

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
void test_constructors()
{
	std::cout << "Testing different constructors" << std::endl;

	print_list(T(12));
	print_list(T({4, 8, 16, 2, 32}));
	print_list(T(5, 28));

	T t1 = T({4, 8, 16, 2, 32});
	T t2 = t1;
	print_list(t2);
}

template<class T>
void test()
{

	T t;
	setup_list(t);

	test_iters(t);
	test_iters((const T) t);

	test_constructors<T>();
}

int main(int argc, char**)
{
	if (argc == 2) {
		test<std::list<int>>();
	} else {
		test<safelist<int>>();
	}

	return 0;
}
