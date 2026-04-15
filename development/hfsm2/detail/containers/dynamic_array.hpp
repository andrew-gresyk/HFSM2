namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TItem, Long NCapacity>
class DynamicArrayT final {
	template <typename, typename, typename>
	friend class IteratorT;

public:
	using Item		= TItem;
	using Index		= UCapacity<NCapacity>;

	using  Iterator	= IteratorT<      DynamicArrayT, Item, Index>;
	using CIterator	= IteratorT<const DynamicArrayT, Item, Index>;

	static constexpr Index CAPACITY	= NCapacity;
	using Storage	= uint8_t[sizeof(Item) * CAPACITY];

	using This		= DynamicArrayT<Item, CAPACITY>;

	static_assert(sizeof(Item) % alignof(Item) == 0, "");

public:
						 DynamicArrayT()								noexcept	= default;
	HFSM2_CONSTEXPR(14)	 DynamicArrayT(const This & other)				noexcept;
	HFSM2_CONSTEXPR(14)	 DynamicArrayT(      This&& other)				noexcept;
	HFSM2_CONSTEXPR(20)	~DynamicArrayT()								noexcept;

	HFSM2_CONSTEXPR(14)	DynamicArrayT& operator = (const This & other)	noexcept;
	HFSM2_CONSTEXPR(14)	DynamicArrayT& operator = (      This&& other)	noexcept;

	template <typename... TArgs>
	HFSM2_CONSTEXPR(14)	Index emplace(TArgs&&... args)			  HFSM2_NOEXCEPT_17(noexcept(Item{::hfsm2::forward<TArgs>(args)...}));

	template <typename N>
	HFSM2_CONSTEXPR(14)		  Item& operator[] (const N index)			noexcept;

	template <typename N>
	HFSM2_CONSTEXPR(14)	const Item& operator[] (const N index)	  const noexcept;

	HFSM2_CONSTEXPR(11)	Index count()							  const noexcept	{ return _count;					}

	HFSM2_CONSTEXPR(14)	 void clear()									noexcept;
	HFSM2_CONSTEXPR(11)	 bool empty()							  const noexcept	{ return _count == 0;				}

	template <Long N>
	HFSM2_CONSTEXPR(14)	DynamicArrayT& operator += (const DynamicArrayT<Item, N>& other)	noexcept;

	HFSM2_CONSTEXPR(14)	 Iterator  begin()								noexcept	{ return  Iterator(*this, first());	}
	HFSM2_CONSTEXPR(11)	CIterator  begin()						  const noexcept	{ return CIterator(*this, first());	}
	HFSM2_CONSTEXPR(11)	CIterator cbegin()						  const noexcept	{ return CIterator(*this, first());	}

	HFSM2_CONSTEXPR(14)	 Iterator    end()								noexcept	{ return  Iterator(*this, limit());	}
	HFSM2_CONSTEXPR(11)	CIterator    end()						  const noexcept	{ return CIterator(*this, limit());	}
	HFSM2_CONSTEXPR(11)	CIterator   cend()						  const noexcept	{ return CIterator(*this, limit());	}

private:
	HFSM2_CONSTEXPR(11)	Index first()							  const noexcept	{ return 0;							}
	HFSM2_CONSTEXPR(11)	Index next(const Index index)			  const noexcept	{ return index + 1;					}
	HFSM2_CONSTEXPR(11)	Index limit()							  const noexcept	{ return _count;					}

	HFSM2_CONSTEXPR(14)		  Item   & item   (const Index index)		noexcept;
	HFSM2_CONSTEXPR(11)	const Item   & item   (const Index index) const noexcept;

	HFSM2_CONSTEXPR(14)		  uint8_t* storage(const Index index)		noexcept;
	HFSM2_CONSTEXPR(11)	const uint8_t* storage(const Index index) const noexcept;

private:
	Index _count = 0;

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

	alignas(Item) Storage _storage {};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif
};

//------------------------------------------------------------------------------

template <typename TItem>
class DynamicArrayT<TItem, 0> final {
public:
	using Item	= TItem;
	using Index	= UCapacity<0>;

	static constexpr Index CAPACITY = 0;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "dynamic_array.inl"
