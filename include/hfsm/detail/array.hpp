#pragma once

#include "array_view.hpp"
#include "iterator.hpp"

#include <cassert>
#include <limits>

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 4)

template <typename T, unsigned TCapacity>
class StaticArray {
public:
	enum {
		CAPACITY  = TCapacity,
		ALIGNMENT = alignof(T)
	};

	using Item  = T;
	using Index = unsigned char;
	static_assert(CAPACITY <= std::numeric_limits<Index>::max(), "");

public:
	inline StaticArray() = default;

	inline StaticArray(const char value)			{ hfsm::detail::fill(_items, value);		}

	inline void nullify()							{ hfsm::detail::nullify(_items);			}
	inline void fill(const char value)				{ hfsm::detail::fill(_items, value);		}

	// element access
	inline		 Item* storage()					{ return &_items[0];						}
	inline const Item* storage() const				{ return &_items[0];						}

	inline		 Item& operator[] (const unsigned i);
	inline const Item& operator[] (const unsigned i) const;

	inline const unsigned count() const				{ return CAPACITY;							}

private:
	Item _items[CAPACITY];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
struct StaticArray<T, 0> {};

//------------------------------------------------------------------------------

template <typename T, unsigned TCapacity>
class Array
	: public ArrayView<T>
{
public:
	enum : unsigned {
		CAPACITY = TCapacity,
		INVALID	 = (unsigned)-1,
		DUMMY	 = INVALID,
	};

	using View = ArrayView<T>;
	using Item = typename View::Item;

public:
	Array()
		: View(CAPACITY)
	{
		assert(&View::get(0) == _storage);
	}

	Array(unsigned& count)
		: View(CAPACITY, count)
	{
		assert(&View::get(0) == _storage);
	}

	inline void nullify()						{ hfsm::detail::nullify(_storage);					  }
	inline void fill(const char value)			{ hfsm::detail::fill(_storage, value);				  }

	inline Iterator<	  Array>  begin()		{ return Iterator<		Array>(*this, View::first()); }
	inline Iterator<const Array>  begin() const { return Iterator<const Array>(*this, View::first()); }
	inline Iterator<const Array> cbegin() const { return Iterator<const Array>(*this, View::first()); }

	inline Iterator<	  Array>	end()		{ return Iterator<		Array>(*this, DUMMY);		  }
	inline Iterator<const Array>	end() const { return Iterator<const Array>(*this, DUMMY);		  }
	inline Iterator<const Array>   cend() const { return Iterator<const Array>(*this, DUMMY);		  }

private:
	Item _storage[CAPACITY];
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}

#include "array.inl"
