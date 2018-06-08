#pragma once

#include "utility.hpp"

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContainer>
class Iterator {
public:
	using Container = TContainer;
	using Item = typename Container::Item;

	template <typename T, unsigned TCapacity>
	friend class Array;

private:
	inline Iterator(Container& container, const unsigned cursor)
		: _container(container)
		, _cursor(cursor)
	{}

public:
	inline bool operator != (const Iterator<Container>& dummy) const;

	inline Iterator& operator ++();

	inline		 Item& operator *()		  { return _container[_cursor]; }
	inline const Item& operator *() const { return _container[_cursor]; }

	inline		 Item* operator->()		  { return &operator *();		}
	inline const Item* operator->() const { return &operator *();		}

private:
	Container& _container;

	unsigned _cursor;
};

//------------------------------------------------------------------------------

template <typename TContainer>
class Iterator<const TContainer> {
public:
	using Container = TContainer;
	using Item = typename Container::Item;

	template <typename T, unsigned TCapacity>
	friend class Array;

private:
	inline Iterator(const Container& container, const unsigned cursor)
		: _container(container)
		, _cursor(cursor)
	{}

public:
	inline bool operator != (const Iterator<const Container>& dummy) const;

	inline Iterator& operator ++();

	inline const Item& operator *() const { return _container[_cursor]; }

	inline const Item* operator->() const { return &operator *();		}

private:
	const Container& _container;

	unsigned _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "iterator.inl"
