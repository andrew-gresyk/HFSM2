namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NCapacity>
class StaticArrayT final {
	template <typename>
	friend class IteratorT;

public:
	using  Iterator	= IteratorT<      StaticArrayT>;
	using CIterator	= IteratorT<const StaticArrayT>;

	using Item		= T;
	using Index		= UCapacity<NCapacity>;

	static constexpr Index CAPACITY	= NCapacity;

public:
	HFSM2_CONSTEXPR(14)	StaticArrayT() = default;
	HFSM2_CONSTEXPR(14)	StaticArrayT(const Item filler)						  noexcept	{ fill(filler);						}

	template <typename N>
	HFSM2_CONSTEXPR(14)		  Item& operator[] (const N index)				  noexcept;

	template <typename N>
	HFSM2_CONSTEXPR(14)	const Item& operator[] (const N index)			const noexcept;

	HFSM2_CONSTEXPR(11)	Index count()									const noexcept	{ return CAPACITY;					}

	HFSM2_CONSTEXPR(14)	void fill(const Item filler)						  noexcept;
	HFSM2_CONSTEXPR(14)	void clear()										  noexcept	{ fill(INVALID_SHORT);				}

	HFSM2_CONSTEXPR(14)	 Iterator  begin()									  noexcept	{ return  Iterator(*this, first());	}
	HFSM2_CONSTEXPR(11)	CIterator  begin()								const noexcept	{ return CIterator(*this, first());	}
	HFSM2_CONSTEXPR(11)	CIterator cbegin()								const noexcept	{ return CIterator(*this, first());	}

	HFSM2_CONSTEXPR(14)	 Iterator	 end()									  noexcept	{ return  Iterator(*this, limit());	}
	HFSM2_CONSTEXPR(11)	CIterator	 end()								const noexcept	{ return CIterator(*this, limit());	}
	HFSM2_CONSTEXPR(11)	CIterator	cend()								const noexcept	{ return CIterator(*this, limit());	}

private:
	HFSM2_CONSTEXPR(11)	Index first()									const noexcept	{ return 0;							}
	HFSM2_CONSTEXPR(11)	Index  next(const Index index)					const noexcept	{ return index + 1;					}
	HFSM2_CONSTEXPR(11)	Index limit()									const noexcept	{ return CAPACITY;					}

private:
	Item _items[CAPACITY] {};
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
struct StaticArrayT<T, 0> final {
	using Item		= T;

	HFSM2_CONSTEXPR(11)	StaticArrayT() = default;
	HFSM2_CONSTEXPR(11)	StaticArrayT(const Item) noexcept {}
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NCapacity>
class ArrayT final {
	template <typename>
	friend class IteratorT;

public:
	using  Iterator	= IteratorT<      ArrayT>;
	using CIterator	= IteratorT<const ArrayT>;

	using Item		= T;
	using Index		= UCapacity<NCapacity>;

	static constexpr Index CAPACITY	= NCapacity;

public:
	HFSM2_CONSTEXPR(14)	 void clear()										  noexcept	{ _count = 0;						}

	template <typename... TArgs>
	HFSM2_CONSTEXPR(14)	Index emplace(const TArgs &... args)				  noexcept;

	template <typename... TArgs>
	HFSM2_CONSTEXPR(14)	Index emplace(		TArgs&&... args)				  noexcept;

	template <typename N>
	HFSM2_CONSTEXPR(14)		  Item& operator[] (const N index)				  noexcept;

	template <typename N>
	HFSM2_CONSTEXPR(14)	const Item& operator[] (const N index)			const noexcept;

	HFSM2_CONSTEXPR(11)	Index  count()									const noexcept	{ return _count;					}

	template <Long N>
	HFSM2_CONSTEXPR(14)	ArrayT& operator += (const ArrayT<Item, N>& other)	  noexcept;

	HFSM2_CONSTEXPR(14)	 Iterator  begin()									  noexcept	{ return  Iterator(*this, first());	}
	HFSM2_CONSTEXPR(11)	CIterator  begin()								const noexcept	{ return CIterator(*this, first());	}
	HFSM2_CONSTEXPR(11)	CIterator cbegin()								const noexcept	{ return CIterator(*this, first());	}

	HFSM2_CONSTEXPR(14)	 Iterator	 end()									  noexcept	{ return  Iterator(*this, limit());	}
	HFSM2_CONSTEXPR(11)	CIterator	 end()								const noexcept	{ return CIterator(*this, limit());	}
	HFSM2_CONSTEXPR(11)	CIterator	cend()								const noexcept	{ return CIterator(*this, limit());	}

private:
	HFSM2_CONSTEXPR(11)	Index first()									const noexcept	{ return 0;							}
	HFSM2_CONSTEXPR(11)	Index next(const Index index)					const noexcept	{ return index + 1;					}
	HFSM2_CONSTEXPR(11)	Index limit()									const noexcept	{ return _count;					}

private:
	Index _count = 0;

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

	Item _items[CAPACITY]; // warning 4324 triggers for 'StructureStateInfo'

#ifdef _MSC_VER
	#pragma warning(pop)
#endif
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
class ArrayT<T, 0> final {
public:
	using Item	= T;
	using Index	= UCapacity<0>;

	static constexpr Index CAPACITY = 0;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "array.inl"
