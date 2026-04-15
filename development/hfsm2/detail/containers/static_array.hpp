namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TItem>
HFSM2_CONSTEXPR(11)
TItem
filler()																noexcept	{
	return TItem{};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <>
HFSM2_CONSTEXPR(11)
Short
filler<Short>()															noexcept	{
	return INVALID_SHORT;
}

//------------------------------------------------------------------------------

template <typename TItem, Long NCapacity>
class StaticArrayT final {
	template <typename, typename, typename>
	friend class IteratorT;

public:
	using Item		= TItem;
	using Index		= UCapacity<NCapacity>;

	using  Iterator	= IteratorT<      StaticArrayT, Item, Index>;
	using CIterator	= IteratorT<const StaticArrayT, Item, Index>;

	static constexpr Index CAPACITY	= NCapacity;

	using This		= StaticArrayT<Item, CAPACITY>;

public:
	HFSM2_CONSTEXPR(11)	StaticArrayT()								   = default;
	HFSM2_CONSTEXPR(14)	StaticArrayT(const Item filler)					noexcept	{ fill(filler);						}

	template <typename N>
	HFSM2_CONSTEXPR(14)		  Item& operator[] (const N index)			noexcept;

	template <typename N>
	HFSM2_CONSTEXPR(14)	const Item& operator[] (const N index)	  const noexcept;

	HFSM2_CONSTEXPR(11)	Index count()							  const noexcept	{ return CAPACITY;					}

	HFSM2_CONSTEXPR(14)	bool operator == (const This& other)	  const noexcept;
	HFSM2_CONSTEXPR(14)	bool operator != (const This& other)	  const noexcept;

	HFSM2_CONSTEXPR(14)	void fill(const Item filler)					noexcept;
	HFSM2_CONSTEXPR(14)	void clear()									noexcept	{ fill(filler<Item>());				}
	HFSM2_CONSTEXPR(14)	bool empty()							  const noexcept;

	HFSM2_CONSTEXPR(14)	 Iterator  begin()								noexcept	{ return  Iterator(*this, first());	}
	HFSM2_CONSTEXPR(11)	CIterator  begin()						  const noexcept	{ return CIterator(*this, first());	}
	HFSM2_CONSTEXPR(11)	CIterator cbegin()						  const noexcept	{ return CIterator(*this, first());	}

	HFSM2_CONSTEXPR(14)	 Iterator	 end()								noexcept	{ return  Iterator(*this, limit());	}
	HFSM2_CONSTEXPR(11)	CIterator	 end()						  const noexcept	{ return CIterator(*this, limit());	}
	HFSM2_CONSTEXPR(11)	CIterator	cend()						  const noexcept	{ return CIterator(*this, limit());	}

private:
	HFSM2_CONSTEXPR(11)	Index first()							  const noexcept	{ return 0;							}
	HFSM2_CONSTEXPR(11)	Index  next(const Index index)			  const noexcept	{ return index + 1;					}
	HFSM2_CONSTEXPR(11)	Index limit()							  const noexcept	{ return CAPACITY;					}

private:
	Item _items[CAPACITY] {};
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TItem>
class StaticArrayT<TItem, 0> final {
public:
	using Item		= TItem;

public:
	HFSM2_CONSTEXPR(11)	StaticArrayT()								   = default;
	HFSM2_CONSTEXPR(11)	StaticArrayT(const Item)						noexcept	{}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "static_array.inl"
