#pragma once

#include <memory>

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

		void push_front(const value_type& value);
		void push_back(const value_type& value);
		bool empty() const;

		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;


	private:
		struct entry;

		std::shared_ptr<entry> head;
		std::weak_ptr<entry> tail;

		inline void singleton_list(const T& value);
};

template<class T>
class safelist<T>::iterator
{
	public:
		friend safelist<T>;

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
		std::weak_ptr<entry> prev; // Needed for past-the-end iterators.

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

		const_iterator& operator++();
		const_iterator operator++(int);
		const_iterator& operator--();
		const_iterator operator--(int);

		const_reference operator*() const;
		bool operator==(const const_iterator&) const;
		bool operator!=(const const_iterator&) const;

	private:
		std::weak_ptr<const entry> item;
		std::weak_ptr<const entry> prev; // Needed for past-the-end iterators.

		const_iterator(const std::weak_ptr<entry>& item): item(item) {};
};

template<class T>
struct safelist<T>::entry
{
	typedef std::shared_ptr<entry> fwd_ptr;
	typedef std::weak_ptr<entry> back_ptr;

	std::weak_ptr<entry> prev;
	std::shared_ptr<entry> next;
	value_type value;

	entry(const value_type& value) :
		value(value)
	{
	}

	entry (const value_type& value, const fwd_ptr next) :
		next(next), value(value)
	{
	}
};

template<class T>
bool safelist<T>::empty() const
{
	return head == nullptr;
}

	template<class T>
void safelist<T>::push_front(const T& value)
{
	if (empty()) {
		singleton_list(value);
	} else {
		auto to_add = std::make_shared<entry>(value, head);
		to_add->next = head;
		head->prev = to_add;
		head = to_add;
	}
}

	template<class T>
void safelist<T>::push_back(const T& value)
{
	if (empty()) {
		singleton_list(value);
	} else {
		auto to_add = std::make_shared<entry>(value);
		to_add->prev = tail;
		tail.lock()->next = to_add;
		tail = std::weak_ptr<entry>(to_add);
	}
}

	template<class T>
void safelist<T>::singleton_list(const T& value)
{
	head = std::make_shared<entry>(value);
	tail = std::weak_ptr<entry>(head);
}

	template<class T>
typename safelist<T>::iterator safelist<T>::begin()
{
	return iterator(std::weak_ptr<entry>(head));
}

	template<class T>
typename safelist<T>::iterator safelist<T>::end()
{
	iterator it;
	it.prev = tail;

	return it;
}

	template<class T>
typename safelist<T>::const_iterator safelist<T>::begin() const
{
	return const_iterator(std::weak_ptr<entry>(head));
}

	template<class T>
typename safelist<T>::const_iterator safelist<T>::end() const
{
	const_iterator it;
	it.prev = tail;

	return it;
}

// Iterator functions
template<class T>
typename safelist<T>::iterator& safelist<T>::iterator::operator++()
{
	prev = item;
	item = std::weak_ptr<entry>(item.lock()->next);
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
	item = prev;
	prev = item.lock()->prev;

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
	return item.lock()->value;
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
typename safelist<T>::const_iterator& safelist<T>::const_iterator::operator++()
{
	prev = item;
	item = std::weak_ptr<entry>(item.lock()->next);
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
	item = prev;
	prev = item.lock()->prev;

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
	return item.lock()->value;
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
