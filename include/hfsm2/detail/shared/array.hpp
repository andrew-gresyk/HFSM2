namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NCapacity>
class StaticArray {
public:
	static constexpr Long CAPACITY = NCapacity;
	static constexpr Long DUMMY	   = INVALID_LONG;

	using Item  = T;
	using Index = UnsignedCapacity<CAPACITY>;

public:
	HFSM2_INLINE StaticArray() = default;
	HFSM2_INLINE StaticArray(const Item filler)					  noexcept;

	template <typename N>
	HFSM2_INLINE	   Item& operator[] (const N i)				  noexcept;

	template <typename N>
	HFSM2_INLINE const Item& operator[] (const N i)			const noexcept;

	HFSM2_INLINE Long count() const								  noexcept	{ return CAPACITY;									}

	HFSM2_INLINE void fill(const Item filler)					  noexcept;
	HFSM2_INLINE void clear()									  noexcept	{ fill(INVALID_SHORT);								}

	HFSM2_INLINE Iterator<      StaticArray>  begin()			  noexcept	{ return Iterator<      StaticArray>(*this,     0); }
	HFSM2_INLINE Iterator<const StaticArray>  begin()		const noexcept	{ return Iterator<const StaticArray>(*this,     0); }
	HFSM2_INLINE Iterator<const StaticArray> cbegin()		const noexcept	{ return Iterator<const StaticArray>(*this,     0); }

	HFSM2_INLINE Iterator<      StaticArray>    end()			  noexcept	{ return Iterator<      StaticArray>(*this, DUMMY);	}
	HFSM2_INLINE Iterator<const StaticArray>    end()		const noexcept	{ return Iterator<const StaticArray>(*this, DUMMY);	}
	HFSM2_INLINE Iterator<const StaticArray>   cend()		const noexcept	{ return Iterator<const StaticArray>(*this, DUMMY);	}

private:
	Item _items[CAPACITY];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
struct StaticArray<T, 0> {
	using Item  = T;

	HFSM2_INLINE StaticArray() = default;
	HFSM2_INLINE StaticArray(const Item) noexcept {}
};

//------------------------------------------------------------------------------

template <typename T, Long NCapacity>
class Array {
	template <typename>
	friend class Iterator;

public:
	static constexpr Long CAPACITY = NCapacity;
	static constexpr Long DUMMY	   = INVALID_LONG;

	using Item = T;
	using Index = UnsignedCapacity<CAPACITY>;

public:
	HFSM2_INLINE void clear()									  noexcept	{ _count = 0;		}

	// TODO: replace with 'emplace<>()'?
	template <typename TValue>
	HFSM2_INLINE Long append(const TValue& value)				  noexcept;

	template <typename TValue>
	HFSM2_INLINE Long append(	  TValue&& value)				  noexcept;

	template <typename N>
	HFSM2_INLINE	   Item& operator[] (const N i)				  noexcept;

	template <typename N>
	HFSM2_INLINE const Item& operator[] (const N i)			const noexcept;

	HFSM2_INLINE Long count()								const noexcept	{ return _count;	}

	template <Long N>
	HFSM2_INLINE Array& operator += (const Array<Item, N>& other) noexcept;

	HFSM2_INLINE Iterator<      Array>  begin()					  noexcept	{ return Iterator<		Array>(*this,     0);	}
	HFSM2_INLINE Iterator<const Array>  begin()				const noexcept	{ return Iterator<const Array>(*this,     0);	}
	HFSM2_INLINE Iterator<const Array> cbegin()				const noexcept	{ return Iterator<const Array>(*this,     0);	}

	HFSM2_INLINE Iterator<      Array>	  end()					  noexcept	{ return Iterator<		Array>(*this, DUMMY);	}
	HFSM2_INLINE Iterator<const Array>	  end()				const noexcept	{ return Iterator<const Array>(*this, DUMMY);	}
	HFSM2_INLINE Iterator<const Array>   cend()				const noexcept	{ return Iterator<const Array>(*this, DUMMY);	}

private:
	HFSM2_INLINE Long next(const Long i)					const noexcept	{ return i + 1;		}
	HFSM2_INLINE Long limit()								const noexcept	{ return _count;	}

private:
	Long _count = 0;

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
class Array<T, 0> {
public:
	static constexpr Long CAPACITY = 0;
	static constexpr Long DUMMY	   = INVALID_LONG;

	using Item = T;
	using Index = UnsignedCapacity<0>;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "array.inl"
