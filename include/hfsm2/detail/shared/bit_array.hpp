#pragma once

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename>
class BitArrayT;

template <typename TIndex>
class BitT {
	using Index		= TIndex;

	using BitArray	= BitArrayT<Index>;

	template <typename>
	friend class BitArrayT;

private:
	HFSM_INLINE BitT(BitArray& array,
				const Index index)
		: _array(array)
		, _index(index)
	{}

public:
	HFSM_INLINE explicit operator bool() const		{ return _array.get(_index);	}
	HFSM_INLINE void operator = (const bool value)	{ _array.set(_index, value);	}

private:
	BitArray& _array;
	const Index _index;
};

//------------------------------------------------------------------------------

template <typename TIndex>
class ConstBitT {
	using Index		= TIndex;

	using BitArray	= BitArrayT<Index>;

	template <typename>
	friend class BitArrayT;

private:
	HFSM_INLINE ConstBitT(const BitArray& array,
						  const Index index)
		: _array(array)
		, _index(index)
	{}

public:
	HFSM_INLINE explicit operator bool() const		{ return _array.get(_index);	}

private:
	const BitArray& _array;
	const Index _index;
};

//------------------------------------------------------------------------------

#pragma pack(push, 1)

template <typename TIndex>
class alignas(2) BitArrayT {
	template <typename>
	friend class BitT;

	template <typename>
	friend class ConstBitT;

public:
	using Index		= TIndex;

	using	   Bit	=	   BitT<Index>;
	using ConstBit	= ConstBitT<Index>;

protected:
	using StorageUnit	= uint8_t;
	using Storage		= StorageUnit*;

	static constexpr Index STORAGE_UNIT_SIZE	= sizeof(StorageUnit) * 8;

	static constexpr Index ITEM_ALIGNMENT		= alignof(StorageUnit[2]);
	static constexpr Index VIEW_SIZE			= sizeof(Index);
	static constexpr Index OFFSET				= (VIEW_SIZE + ITEM_ALIGNMENT - 1) / ITEM_ALIGNMENT * ITEM_ALIGNMENT;

protected:
	HFSM_INLINE BitArrayT(const Index capacity_);

public:
	HFSM_INLINE void clear();

	HFSM_INLINE explicit operator bool() const;

	HFSM_INLINE		 Bit operator[] (const Index i)			{ return	  Bit{*this, i}; }
	HFSM_INLINE ConstBit operator[] (const Index i) const	{ return ConstBit{*this, i}; }

protected:
	HFSM_INLINE bool get(const Index i) const;
	HFSM_INLINE void set(const Index i, const bool value);

	HFSM_INLINE		  StorageUnit* storage()		{ return reinterpret_cast<		StorageUnit*>(((uintptr_t)this) + OFFSET);	}
	HFSM_INLINE const StorageUnit* storage() const	{ return reinterpret_cast<const StorageUnit*>(((uintptr_t)this) + OFFSET);	}

	HFSM_INLINE Index storageUnitCount() const		{ return (capacity + STORAGE_UNIT_SIZE - 1) / STORAGE_UNIT_SIZE;			}

public:
	const Index capacity;
};

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
class BitArrayStorageT final
	: public BitArrayT<TIndex>
{
	using Index			= TIndex;

	using BitArray		= BitArrayT<Index>;

	using StorageUnit	= typename BitArray::StorageUnit;

	using BitArray::STORAGE_UNIT_SIZE;

	static constexpr Index CAPACITY				= NCapacity;
	static constexpr Index STORAGE_UNIT_COUNT	= (CAPACITY + STORAGE_UNIT_SIZE - 1) / STORAGE_UNIT_SIZE;

public:
	HFSM_INLINE BitArrayStorageT();

private:
	StorageUnit _storage[STORAGE_UNIT_COUNT];
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}

#include "bit_array.inl"
