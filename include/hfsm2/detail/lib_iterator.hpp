#pragma once

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContainer>
class Iterator {
public:
	using Container = TContainer;
	using Item = typename Container::Item;

	template <typename T, LongIndex NCapacity>
	friend class Array;

private:
	HFSM_INLINE Iterator(Container& container, const LongIndex cursor)
		: _container(container)
		, _cursor(cursor)
	{}

public:
	HFSM_INLINE bool operator != (const Iterator<Container>& dummy) const;

	HFSM_INLINE Iterator& operator ++();

	HFSM_INLINE		  Item& operator *()	   { return  _container[_cursor]; }
	HFSM_INLINE const Item& operator *() const { return  _container[_cursor]; }

	HFSM_INLINE		  Item* operator->()	   { return &_container[_cursor]; }
	HFSM_INLINE const Item* operator->() const { return &_container[_cursor]; }

private:
	Container& _container;

	LongIndex _cursor;
};

//------------------------------------------------------------------------------

template <typename TContainer>
class Iterator<const TContainer> {
public:
	using Container = TContainer;
	using Item = typename Container::Item;

	template <typename T, LongIndex NCapacity>
	friend class Array;

private:
	HFSM_INLINE Iterator(const Container& container, const LongIndex cursor)
		: _container(container)
		, _cursor(cursor)
	{}

public:
	HFSM_INLINE bool operator != (const Iterator<const Container>& dummy) const;

	HFSM_INLINE Iterator& operator ++();

	HFSM_INLINE const Item& operator *() const { return _container[_cursor]; }

	HFSM_INLINE const Item* operator->() const { return &operator *();		 }

private:
	const Container& _container;

	LongIndex _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "lib_iterator.inl"
