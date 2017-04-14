#pragma once

#include "array_view.hpp"
#include "iterator.hpp"

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 4)

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
		assert(&get(0) == _storage);
	}

	Array(unsigned& count)
		: View(CAPACITY, count)
	{
		assert(&get(0) == _storage);
	}

	inline void nullify()						{ hfsm::detail::nullify(_storage);				}
	inline void fill(const char value)			{ hfsm::detail::fill(_storage, value);			}

	inline Iterator<	  Array>  begin()		{ return Iterator<		Array>(*this, first()); }
	inline Iterator<const Array>  begin() const { return Iterator<const Array>(*this, first()); }
	inline Iterator<const Array> cbegin() const { return Iterator<const Array>(*this, first()); }

	inline Iterator<	  Array>	end()		{ return Iterator<		Array>(*this, DUMMY);	}
	inline Iterator<const Array>	end() const { return Iterator<const Array>(*this, DUMMY);	}
	inline Iterator<const Array>   cend() const { return Iterator<const Array>(*this, DUMMY);	}

private:
	Item _storage[CAPACITY];
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}
