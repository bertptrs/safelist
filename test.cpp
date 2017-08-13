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
void test_access(T& sl)
{
	std::cout << sl.front() << std::endl;
	std::cout << sl.back() << std::endl;
}

template<class T>
void setup_list(T& l)
{
	l.push_front(2);
	l.push_front(1);
	l.push_back(3);
}

template<class T>
void test_emplace()
{
	T t = {1, 2, 3};
	t.emplace(t.end(), 4);
	print_list(t);

	t.emplace(++(++t.begin()), 0);
	print_list(t);

	t.emplace_front(-1);
	t.emplace_back(5);
	print_list(t);
}

template<class T>
void test_constructors()
{
	std::cout << "Testing different constructors" << std::endl;

	print_list(T(12));
	print_list(T({4, 8, 16, 2, 32}));
	print_list(T(5, 28));

	T t1 = {4, 8, 16, 2, 32};
	T t2 = t1;
	print_list(t2);
}

template<class T>
void test_pop()
{
	std::cout << "testing pop_back" << std::endl;

	T t = {1, 2, 3, 4};
	while (!t.empty())
	{
		print_list(t);
		t.pop_back();
	}

	std::cout << "testing pop_front" << std::endl;

	t = T({4, 3, 2, 1});
	while (!t.empty())
	{
		print_list(t);
		t.pop_front();
	}
}

template<class T>
void test_sizing()
{
	std::cout << "Testing sizing functions" << std::endl;
	T t;
	std::cout << t.size() << std::endl;

	t.resize(2);
	std::cout << t.size() << std::endl;
	print_list(t);

	t.resize(8, 4);
	std::cout << t.size() << std::endl;
	print_list(t);

	t.resize(3);
	std::cout << t.size() << std::endl;
	print_list(t);
}

template<class T>
void test()
{
	T t;
	setup_list(t);

	test_iters(t);
	test_iters((const T) t);

	test_constructors<T>();

	test_access(t);
	test_access((const T) t);

	test_pop<T>();
	test_sizing<T>();
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
