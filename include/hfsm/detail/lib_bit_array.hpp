#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

class BitArray;

class Bit {
	friend class BitArray;

private:
	inline Bit(BitArray& array,
			   const ShortIndex index)
		: _array(array)
		, _index(index)
	{}

public:
	inline explicit operator bool() const;
	inline void operator = (const bool value);

private:
	BitArray& _array;
	const ShortIndex _index;
};

//------------------------------------------------------------------------------

class ConstBit {
	friend class BitArray;

private:
	inline ConstBit(const BitArray& array,
					const ShortIndex index)
		: _array(array)
		, _index(index)
	{}

public:
	inline explicit operator bool() const;

private:
	const BitArray& _array;
	const ShortIndex _index;
};

//------------------------------------------------------------------------------

#pragma pack(push, 1)

class BitArray {
	friend class Bit;
	friend class ConstBit;

protected:
	using StorageUnit	= uint8_t;
	using Storage		= StorageUnit*;

	static constexpr ShortIndex STORAGE_UNIT_SIZE	= sizeof(StorageUnit) * 8;

	static constexpr ShortIndex ITEM_ALIGNMENT		= alignof(StorageUnit[2]);
	static constexpr ShortIndex VIEW_SIZE			= 1;
	static constexpr ShortIndex OFFSET				= (VIEW_SIZE + ITEM_ALIGNMENT - 1) / ITEM_ALIGNMENT * ITEM_ALIGNMENT;

protected:
	inline BitArray(const ShortIndex capacity_)
		: capacity{capacity_}
	{
		clear();
	}

public:
	inline void clear();

	inline explicit operator bool() const;

	inline		Bit operator[] (const ShortIndex i)			{ return	  Bit{*this, i}; }
	inline ConstBit operator[] (const ShortIndex i) const	{ return ConstBit{*this, i}; }

private:
	inline bool get(const ShortIndex i) const;
	inline void set(const ShortIndex i, const bool value);

	inline		 StorageUnit* storage()			{ return reinterpret_cast<		StorageUnit*>(((uintptr_t)this) + OFFSET);	}
	inline const StorageUnit* storage() const	{ return reinterpret_cast<const StorageUnit*>(((uintptr_t)this) + OFFSET);	}

	inline ShortIndex storageUnitCount() const	{ return (capacity + STORAGE_UNIT_SIZE - 1) / STORAGE_UNIT_SIZE;			}

public:
	const ShortIndex capacity;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

template <ShortIndex NCapacity>
class BitArrayT final
	: public BitArray
{
	static constexpr ShortIndex CAPACITY			= NCapacity;
	static constexpr ShortIndex STORAGE_UNIT_COUNT	= (CAPACITY + STORAGE_UNIT_SIZE - 1) / STORAGE_UNIT_SIZE;

public:
	inline BitArrayT()
		: BitArray{CAPACITY}
	{}

private:
	StorageUnit _storage[STORAGE_UNIT_COUNT];
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "lib_bit_array.inl"
