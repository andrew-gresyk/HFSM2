#pragma once

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, LongIndex NCapacity>
class StaticArray {
public:
	static constexpr LongIndex CAPACITY = NCapacity;
	static constexpr LongIndex DUMMY	= INVALID_LONG_INDEX;

	using Item  = T;
	using Index = typename UnsignedIndex<CAPACITY>::Type;

public:
	HFSM_INLINE StaticArray() = default;
	HFSM_INLINE StaticArray(const Item filler);

	HFSM_INLINE		  Item& operator[] (const LongIndex i);
	HFSM_INLINE const Item& operator[] (const LongIndex i) const;

	HFSM_INLINE LongIndex count() const						{ return CAPACITY;									}

	HFSM_INLINE void fill(const Item filler);

	HFSM_INLINE Iterator<	   StaticArray>  begin()		{ return Iterator<		StaticArray>(*this, 0);		}
	HFSM_INLINE Iterator<const StaticArray>  begin() const	{ return Iterator<const StaticArray>(*this, 0);		}
	HFSM_INLINE Iterator<const StaticArray> cbegin() const	{ return Iterator<const StaticArray>(*this, 0);		}

	HFSM_INLINE Iterator<	   StaticArray>	 end()			{ return Iterator<		StaticArray>(*this, DUMMY);	}
	HFSM_INLINE Iterator<const StaticArray>	 end() const	{ return Iterator<const StaticArray>(*this, DUMMY);	}
	HFSM_INLINE Iterator<const StaticArray> cend() const	{ return Iterator<const StaticArray>(*this, DUMMY);	}

private:
	Item _items[CAPACITY];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
struct StaticArray<T, 0> {
	using Item  = T;

	HFSM_INLINE StaticArray() = default;
	HFSM_INLINE StaticArray(const Item)											{}
};

//------------------------------------------------------------------------------

template <typename T, LongIndex NCapacity>
class Array
	: public ArrayView<T>
{
public:
	static constexpr LongIndex CAPACITY = NCapacity;
	static constexpr LongIndex INVALID	= INVALID_LONG_INDEX;
	static constexpr LongIndex DUMMY	= INVALID;

	using Item = T;
	using View = ArrayView<Item>;

public:
	HFSM_INLINE Array();
	HFSM_INLINE Array(const Array& other);

	HFSM_INLINE void operator = (const Array& other);

	HFSM_INLINE Iterator<	   Array>  begin()			{ return Iterator<		Array>(*this, 0);		}
	HFSM_INLINE Iterator<const Array>  begin() const	{ return Iterator<const Array>(*this, 0);		}
	HFSM_INLINE Iterator<const Array> cbegin() const	{ return Iterator<const Array>(*this, 0);		}

	HFSM_INLINE Iterator<	   Array>	 end()			{ return Iterator<		Array>(*this, DUMMY);	}
	HFSM_INLINE Iterator<const Array>	 end() const	{ return Iterator<const Array>(*this, DUMMY);	}
	HFSM_INLINE Iterator<const Array>   cend() const	{ return Iterator<const Array>(*this, DUMMY);	}

private:
	Item _items[CAPACITY];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
class Array<T, 0> {};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "array.inl"
