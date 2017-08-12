#pragma once

#include <initializer_list>
#include <limits>
#include <memory>
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

		// Assignment operators
		safelist<value_type>& operator=(const safelist<value_type>& other);
		safelist<value_type>& operator=(safelist<value_type>&& other);


		~safelist();

		void push_front(const value_type& value);
		void push_back(const value_type& value);

		void pop_front();
		void pop_back();

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
};

template<class T>
class safelist<T>::const_iterator
{
	public:
		friend safelist<T>;

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
	std::weak_ptr<entry> prev;
	std::shared_ptr<entry> next;
	std::unique_ptr<value_type> value;

	entry()
	{
	}

	entry(const value_type& value) :
		value(std::make_unique<value_type>(value))
	{
	}

	entry (const value_type& value, const std::shared_ptr<entry>& next, const std::weak_ptr<entry>& prev) :
		prev(prev),
		next(next),
		value(std::make_unique<value_type>(value))
	{
	}
};


// Constructor definitions
template<class T>
safelist<T>::safelist(): entryPoint(std::make_shared<entry>())
{
	clear();
}

template<class T>
safelist<T>::safelist(size_type count): safelist(count, value_type())
{
}

template<class T>
safelist<T>::safelist(size_type count, const value_type& value): safelist()
{
	while (count--) {
		push_front(value);
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
	entryPoint = other.entryPoint;
	other.entryPoint = nullptr;
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
	entryPoint->next = nullptr; // Allow this list to auto-delete
	entryPoint = other.entryPoint; // Take other list entrypoint
	m_size = other.m_size;
	other.entryPoint = nullptr; // Invalidate other

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

// Iterator functions
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
