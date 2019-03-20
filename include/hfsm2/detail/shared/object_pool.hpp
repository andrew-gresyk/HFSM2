#pragma once

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TInterface, ShortIndex NItemCapacity, LongIndex NStorageSize>
class ObjectPool {
public:
	using Interface = TInterface;

public:
	static constexpr ShortIndex ITEM_COUNT	 = NItemCapacity;
	static constexpr LongIndex  STORAGE_SIZE = NStorageSize;

public:
	HFSM_INLINE ObjectPool();

	HFSM_INLINE ShortIndex count() const					{ return _count;	}

	HFSM_INLINE		  Interface& operator[] (const ShortIndex i);
	HFSM_INLINE const Interface& operator[] (const ShortIndex i) const;

	template <typename TItem, typename... TArgs>
	ShortIndex emplace(TArgs&&... args);

	HFSM_INLINE void clear();

private:
	ShortIndex _offsets[ITEM_COUNT];
	uint8_t _storage[STORAGE_SIZE];
	ShortIndex _count = 0;
};

//------------------------------------------------------------------------------

template <typename TInterface, LongIndex NStorageSize>
class ObjectPool<TInterface, 0, NStorageSize> {
public:
	HFSM_INLINE void clear()													{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInterface, ShortIndex NItemCapacity>
class ObjectPool<TInterface, NItemCapacity, 0> {
public:
	HFSM_INLINE void clear()													{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInterface>
class ObjectPool<TInterface, 0, 0> {
public:
	HFSM_INLINE void clear()													{}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "object_pool.inl"
