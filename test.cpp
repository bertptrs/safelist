#include "safelist.hpp"
#include <cassert>
#include <iostream>
#include <iterator>
#include <list>
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
void test_compare()
{
	std::cout << "Testing comparison operators" << std::endl;

	T t1 = {1, 2, 3, 4};
	T t2 = {1, 2, 4, 3};

	assert(t1 == t1);
	assert(t1 != t2);

	assert(t1 < t2);
	assert(t1 <= t2);
	assert(t2 > t1);
	assert(t2 >= t1);
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
void test_erase()
{
	std::cout << "Testing erase funciton" << std::endl;
	T t = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	t.erase(t.begin());
	print_list(t);

	auto lBegin = t.begin();
	std::advance(lBegin, 6);
	t.erase(++t.begin(), lBegin);
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
void test_sorting()
{
	std::cout << "Testing sorting functions" << std::endl;

	T t = {7,1,8,4,4,7,1,5,2};
	t.sort();

	print_list(t);

	t.sort(std::greater<typename T::value_type>());
	print_list(t);
}

template<class T>
void test_unique()
{
	std::cout << "Testing unique" << std::endl;

	T t = {8, 1, 5, 5, 2, 2, 3, 1, 7};
	t.unique();
	print_list(t);

	t = {8, 1, 5, 5, 2, 2, 3, 1, 7};
	t.unique(std::not_equal_to<typename T::value_type>());
	print_list(t);
}

template<class T>
void test_reverse()
{
	std::cout << "Testing list reversing" << std::endl;

	T t = {1, 2, 3, 4, 5};
	t.reverse();

	print_list(t);

	t = {5, 6, 7, 8, 9, 10};
	t.reverse();
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
	test_sorting<T>();
	test_erase<T>();
	test_unique<T>();
	test_reverse<T>();
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
