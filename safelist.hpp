#pragma once

#include <algorithm>
#include <initializer_list>
#include <limits>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

#if __cplusplus < 201300
namespace std {
	// If C++14 is not available, use the following to still have make_unique
	template<typename T, typename... Args>
		static unique_ptr<T> make_unique(Args&&... args)
		{
			return unique_ptr<T>(new T(std::forward<Args>(args)...));
		}
}
#endif


template<class T>
class safelist
{
	public:
		typedef T value_type;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef T& reference;
		typedef const T& const_reference;

		class iterator;
		class const_iterator;

		// Template definitions
		template<typename Iterator>
			using iterator_value_type = decltype(*(std::declval<Iterator>()));

		template<typename Iterator>
			using is_compatible_iterator = std::is_assignable<value_type&, iterator_value_type<Iterator>>;

		template<typename Iterator>
			using if_is_compatible_iterator = std::enable_if<is_compatible_iterator<Iterator>::value>;

		// Constructors
		safelist();
		safelist(size_type count);
		safelist(size_type count, const value_type& v);
		safelist(const safelist<value_type>& other);
		safelist(safelist<value_type>&&);
		safelist(std::initializer_list<value_type> l);
		template<class InputIt,
			typename = typename if_is_compatible_iterator<InputIt>::type>
				safelist(InputIt first, InputIt last);

		void swap(safelist& other);

		// Assignment operators
		safelist<value_type>& operator=(const safelist<value_type>& other);
		safelist<value_type>& operator=(safelist<value_type>&& other);


		~safelist();

		void push_front(const value_type& value);
		void push_back(const value_type& value);

		// Emplacement
		template<class... Args>
			iterator emplace(const_iterator pos, Args&&... args);
		template<class... Args>
			void emplace_back(Args&&... args);
		template<class... Args>
			void emplace_front(Args&&... args);

		void pop_front();
		void pop_back();

		iterator erase(const_iterator iter);
		iterator erase(const_iterator first, const_iterator last);

		// (re)sizing
		size_type size();
		size_type max_size();
		void resize(size_type count);
		void resize(size_type count, const value_type& value);

		// Element accesss
		value_type& front();
		value_type& back();
		const value_type& front() const;
		const value_type& back() const;

		void clear();

		bool empty() const;

		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;
		const_iterator cbegin() const { return begin();};
		const_iterator cend() const { return end();};


		// Algorithms
		template<class Compare = std::less<value_type>>
		void sort(Compare compare = Compare());

		// Comparisons
		bool operator<(const safelist& other) const;
		bool operator<=(const safelist& other) const;
		bool operator>=(const safelist& other) const;
		bool operator>(const safelist& other) const;

		bool operator==(const safelist& other) const;
		bool operator!=(const safelist& other) const;

	private:
		struct entry;
		size_type m_size;

		std::shared_ptr<entry> entryPoint;
};

template<class T>
class safelist<T>::iterator
{
	public:
		friend safelist<T>;
		friend safelist<T>::const_iterator;

		typedef std::ptrdiff_t difference_type;
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef std::bidirectional_iterator_tag iterator_category;

		iterator() = default;
		iterator(const iterator&) = default;
		iterator(iterator&&) = default;

		iterator& operator++();
		iterator operator++(int);
		iterator& operator--();
		iterator operator--(int);

		reference operator*() const;
		bool operator==(const iterator&) const;
		bool operator!=(const iterator&) const;

	private:
		std::weak_ptr<entry> item;

		iterator(const std::weak_ptr<entry>& item): item(item) {};
		iterator(const_iterator);
};

template<class T>
class safelist<T>::const_iterator
{
	public:
		friend safelist<T>;
		friend safelist<T>::iterator;

		typedef std::ptrdiff_t difference_type;
		typedef const T value_type;
		typedef const T* pointer;
		typedef const T& reference;
		typedef std::bidirectional_iterator_tag iterator_category;

		const_iterator() = default;
		const_iterator(const const_iterator&) = default;
		const_iterator(const_iterator&&) = default;
		const_iterator(const iterator&);

		const_iterator& operator++();
		const_iterator operator++(int);
		const_iterator& operator--();
		const_iterator operator--(int);

		const_reference operator*() const;
		bool operator==(const const_iterator&) const;
		bool operator!=(const const_iterator&) const;

	private:
		std::weak_ptr<const entry> item;

		const_iterator(const std::weak_ptr<entry>& item): item(item) {};
};

template<class T>
struct safelist<T>::entry
{
	typedef std::weak_ptr<entry> prev_ptr_t;
	typedef std::shared_ptr<entry> next_ptr_t;
	typedef std::unique_ptr<value_type> value_ptr_t;

	prev_ptr_t prev;
	next_ptr_t next;
	value_ptr_t value;

	entry()
	{
	}

	entry(const value_type& value) :
		value(std::make_unique<value_type>(value))
	{
	}

	entry (const value_type& value, const next_ptr_t& next, const prev_ptr_t& prev) :
		prev(prev),
		next(next),
		value(std::make_unique<value_type>(value))
	{
	}

	entry(value_ptr_t&& value, const next_ptr_t& next, const prev_ptr_t& prev) :
		prev(prev),
		next(next),
		value(std::move(value))
	{
	}

	void swap(entry& other)
	{
		if (value && other.value) {
			std::swap(value, other.value);
		}
	}
};


// Constructor definitions
template<class T>
safelist<T>::safelist(): entryPoint(std::make_shared<entry>())
{
	clear();
}

template<class T>
safelist<T>::safelist(size_type count): safelist()
{
	while (count--) {
		emplace_back();
	}
}

template<class T>
safelist<T>::safelist(size_type count, const value_type& value): safelist()
{
	while (count--) {
		push_back(value);
	}
}

template<class T>
template<class InputIt, typename>
safelist<T>::safelist(InputIt first, InputIt last): safelist()
{
	for (; first != last; ++first) {
		push_back(*first);
	}
}

template<class T>
safelist<T>::safelist(const safelist<T>& other): safelist(other.begin(), other.end())
{
}

template<class T>
safelist<T>::safelist(safelist<T>&& other)
{
	m_size = other.size();
	entryPoint = std::move(other.entryPoint);
}

template<class T>
safelist<T>::safelist(std::initializer_list<value_type> l): safelist(l.begin(), l.end())
{
}

template<class T>
safelist<T>::~safelist()
{
	if (entryPoint) {
		entryPoint->next = nullptr;
	}
}

template<class T>
void safelist<T>::swap(safelist& other)
{
	std::swap(entryPoint, other.entryPoint);
	std::swap(m_size, other.m_size);
}

template<class T>
void std::swap(safelist<T>& a, safelist<T>& b)
{
	a.swap(b);
}

// Assignment operators
template<class T>
safelist<T>& safelist<T>::operator=(const safelist<T>& other)
{
	clear();
	for (auto &entry : other) {
		push_back(entry);
	}

	return *this;
}

template<class T>
safelist<T>& safelist<T>::operator=(safelist<T>&& other)
{
	entryPoint->next = nullptr; // Need to break the ring of references
	entryPoint = std::move(other.entryPoint); // Take other list entrypoint
	m_size = other.m_size;

	return *this;
}

// Sizing functions

template<class T>
void safelist<T>::clear()
{
	m_size = 0;
	entryPoint->next = entryPoint;
	entryPoint->prev = entryPoint;
}

template<class T>
typename safelist<T>::size_type safelist<T>::size()
{
	return m_size;
}


template<class T>
typename safelist<T>::size_type safelist<T>::max_size()
{
	return std::numeric_limits<value_type>::max();
}

template<class T>
void safelist<T>::resize(size_type count)
{
	resize(count, T());
}

template<class T>
void safelist<T>::resize(size_type count, const value_type& value)
{
	while (m_size > count) {
		pop_back();
	}

	while (m_size < count) {
		push_back(value);
	}
}

template<class T>
bool safelist<T>::empty() const
{
	return m_size == 0;
}

// Element access
template<class T>
T& safelist<T>::front()
{
	return *entryPoint->next->value;
}

template<class T>
const T& safelist<T>::front() const
{
	return *entryPoint->next->value;
}

template<class T>
T& safelist<T>::back()
{
	return *entryPoint->prev.lock()->value;
}

template<class T>
const T& safelist<T>::back() const
{
	return *entryPoint->prev.lock()->value;
}

// Element creation
template<class T>
void safelist<T>::push_front(const T& value)
{
	entryPoint->next = std::make_shared<entry>(value, entryPoint->next, entryPoint);
	entryPoint->next->next->prev = entryPoint->next;
	++m_size;
}

template<class T>
void safelist<T>::push_back(const T& value)
{
	auto tmpShared = entryPoint->prev.lock();

	entryPoint->prev = tmpShared->next = std::make_shared<entry>(value, entryPoint, entryPoint->prev);
	++m_size;
}

template<class T>
typename safelist<T>::iterator safelist<T>::erase(const_iterator pos)
{
	auto e = pos.item.lock();
	if (!e->value) {
		throw std::range_error("Unable to erase end()");
	}

	auto p = std::const_pointer_cast<entry>(e);

	p->prev.lock()->next = p->next;
	p->next->prev = p->prev;

	return iterator(p->next);
}

template<class T>
typename safelist<T>::iterator safelist<T>::erase(const_iterator first, const_iterator last)
{
	for (; first != last; erase(first++));

	return last;
}

// Element deletion
template<class T>
void safelist<T>::pop_front()
{
	if (m_size) {
		entryPoint->next = entryPoint->next->next;
		entryPoint->next->prev = entryPoint;
		--m_size;
	}
}

template<class T>
void safelist<T>::pop_back()
{
	if (m_size) {
		auto tempShared = entryPoint->prev.lock()->prev.lock();
		entryPoint->prev = tempShared;
		tempShared->next = entryPoint;
		--m_size;
	}
}

// Emplacement functions
template<class T>
template<class... Args>
typename safelist<T>::iterator safelist<T>::emplace(const_iterator pos, Args&&... args)
{
	auto realPos = std::const_pointer_cast<entry>((--pos).item.lock());
	realPos->next->next->prev = realPos->next = std::make_shared<entry>(std::make_unique<value_type>(args...), realPos->next, realPos);

	++m_size;

	return iterator(realPos->next);
}


template<class T>
template<class... Args>
void safelist<T>::emplace_back(Args&&... args)
{
	emplace(end(), args...);
}


template<class T>
template<class... Args>
void safelist<T>::emplace_front(Args&&... args)
{
	emplace(++begin(), args...);
}

// Iterator creation
template<class T>
typename safelist<T>::iterator safelist<T>::begin()
{
	return iterator(entryPoint->next);
}

template<class T>
typename safelist<T>::iterator safelist<T>::end()
{
	return iterator(entryPoint);
}

template<class T>
typename safelist<T>::const_iterator safelist<T>::begin() const
{
	return const_iterator(entryPoint->next);
}

template<class T>
typename safelist<T>::const_iterator safelist<T>::end() const
{
	return const_iterator(entryPoint);
}

// Comparison functions
template<class T>
bool safelist<T>::operator<(const safelist& other) const
{
	return std::lexicographical_compare(begin(), end(), other.begin(), other.end(), std::less<value_type>());
}

template<class T>
bool safelist<T>::operator<=(const safelist& other) const
{
	return std::lexicographical_compare(begin(), end(), other.begin(), other.end(), std::less_equal<value_type>());
}

template<class T>
bool safelist<T>::operator>=(const safelist& other) const
{
	return std::lexicographical_compare(begin(), end(), other.begin(), other.end(), std::greater_equal<value_type>());
}

template<class T>
bool safelist<T>::operator>(const safelist& other) const
{
	return std::lexicographical_compare(begin(), end(), other.begin(), other.end(), std::greater<value_type>());
}

template<class T>
bool safelist<T>::operator==(const safelist& other) const
{
	if (m_size != other.m_size) {
		return false;
	}

	for (auto it1 = begin(), it2 = begin(); it1 != end(); ++it1, ++it2) {
		if (*it1 != *it2) {
			return false;
		}
	}

	return true;
}

template<class T>
bool safelist<T>::operator!=(const safelist& other) const
{
	return !(*this == other);
}

template<class T>
template<class Compare>
void safelist<T>::sort(Compare compare)
{
	// Sorry for the lack of n log n, I'll just implement bubblesort
	for (auto endPtr = end(); endPtr != begin(); --endPtr) {
		auto it = begin();
		auto prev = it++;
		for (; it != endPtr; ++prev, ++it) {
			if (!compare(*prev, *it)) {
				it.item.lock()->swap(*prev.item.lock());
			}
		}
	}
}

// Iterator functions
template<class T>
safelist<T>::iterator::iterator(const_iterator it) : item(std::const_pointer_cast<entry>(it.item.lock()))
{
}

template<class T>
typename safelist<T>::iterator& safelist<T>::iterator::operator++()
{
	item = item.lock()->next;
	return *this;
}

template<class T>
typename safelist<T>::iterator safelist<T>::iterator::operator++(int)
{
	iterator copy = *this;
	++(*this);

	return copy;
}

template<class T>
typename safelist<T>::iterator& safelist<T>::iterator::operator--()
{
	item = item.lock()->prev;

	return *this;
}

template<class T>
typename safelist<T>::iterator safelist<T>::iterator::operator--(int)
{
	iterator copy = *this;
	--(*this);

	return copy;
}

template<class T>
T& safelist<T>::iterator::operator*() const
{
	return *item.lock()->value;
}

template<class T>
bool safelist<T>::iterator::operator==(const iterator& other) const
{
	return item.lock() == other.item.lock();
}

template<class T>
bool safelist<T>::iterator::operator!=(const iterator& other) const
{
	return !(*this == other);
}

// Const iterator functions. Mostly repeated from above
template<class T>
safelist<T>::const_iterator::const_iterator(const safelist<T>::iterator& it): item(it.item)
{
}

template<class T>
typename safelist<T>::const_iterator& safelist<T>::const_iterator::operator++()
{
	item = item.lock()->next;
	return *this;
}

template<class T>
typename safelist<T>::const_iterator safelist<T>::const_iterator::operator++(int)
{
	const_iterator copy = *this;
	++(*this);

	return copy;
}

template<class T>
typename safelist<T>::const_iterator& safelist<T>::const_iterator::operator--()
{
	item = item.lock()->prev;

	return *this;
}

template<class T>
typename safelist<T>::const_iterator safelist<T>::const_iterator::operator--(int)
{
	const_iterator copy = *this;
	--(*this);

	return copy;
}

template<class T>
const T& safelist<T>::const_iterator::operator*() const
{
	return *item.lock()->value;
}

template<class T>
bool safelist<T>::const_iterator::operator==(const const_iterator& other) const
{
	return item.lock() == other.item.lock();
}

template<class T>
bool safelist<T>::const_iterator::operator!=(const const_iterator& other) const
{
	return !(*this == other);
}
