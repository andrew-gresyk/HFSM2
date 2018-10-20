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
	HSFM_INLINE BitT(BitArray& array,
				const Index index)
		: _array(array)
		, _index(index)
	{}

public:
	HSFM_INLINE explicit operator bool() const		{ return _array.get(_index);	}
	HSFM_INLINE void operator = (const bool value)	{ _array.set(_index, value);	}

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
	HSFM_INLINE ConstBitT(const BitArray& array,
						  const Index index)
		: _array(array)
		, _index(index)
	{}

public:
	HSFM_INLINE explicit operator bool() const		{ return _array.get(_index);	}

private:
	const BitArray& _array;
	const Index _index;
};

//------------------------------------------------------------------------------

#pragma pack(push, 1)

template <typename TIndex>
class alignas(2) BitArrayT {
	using Index		= TIndex;

	using	   Bit	=	   BitT<Index>;
	using ConstBit	= ConstBitT<Index>;

	template <typename>
	friend class BitT;

	template <typename>
	friend class ConstBitT;

protected:
	using StorageUnit	= uint8_t;
	using Storage		= StorageUnit*;

	static constexpr Index STORAGE_UNIT_SIZE	= sizeof(StorageUnit) * 8;

	static constexpr Index ITEM_ALIGNMENT		= alignof(StorageUnit[2]);
	static constexpr Index VIEW_SIZE			= sizeof(Index);
	static constexpr Index OFFSET				= (VIEW_SIZE + ITEM_ALIGNMENT - 1) / ITEM_ALIGNMENT * ITEM_ALIGNMENT;

protected:
	HSFM_INLINE BitArrayT(const Index capacity_);

public:
	HSFM_INLINE void clear();

	HSFM_INLINE explicit operator bool() const;

	HSFM_INLINE		Bit operator[] (const Index i)			{ return	  Bit{*this, i}; }
	HSFM_INLINE ConstBit operator[] (const Index i) const	{ return ConstBit{*this, i}; }

protected:
	HSFM_INLINE bool get(const Index i) const;
	HSFM_INLINE void set(const Index i, const bool value);

	HSFM_INLINE		 StorageUnit* storage()			{ return reinterpret_cast<		StorageUnit*>(((uintptr_t)this) + OFFSET);	}
	HSFM_INLINE const StorageUnit* storage() const	{ return reinterpret_cast<const StorageUnit*>(((uintptr_t)this) + OFFSET);	}

	HSFM_INLINE Index storageUnitCount() const		{ return (capacity + STORAGE_UNIT_SIZE - 1) / STORAGE_UNIT_SIZE;			}

public:
	const Index capacity;
};

//------------------------------------------------------------------------------

template <typename TIndex, TIndex NCapacity>
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
	HSFM_INLINE BitArrayStorageT();

private:
	StorageUnit _storage[STORAGE_UNIT_COUNT];
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}

#include "lib_bit_array.inl"
