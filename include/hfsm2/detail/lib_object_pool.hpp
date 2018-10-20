#pragma once

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TInterface, ShortIndex NItemCapacity, LongIndex NStorageSize>
class ObjectPool {
public:
	using Interface		= TInterface;

public:
	static constexpr ShortIndex ITEM_COUNT	 = NItemCapacity;
	static constexpr LongIndex  STORAGE_SIZE = NStorageSize;

	static_assert(ITEM_COUNT   > 0, "Item count must be positive");
	static_assert(STORAGE_SIZE > 0, "Storage size must be positive");

public:
	HSFM_INLINE ObjectPool();

	HSFM_INLINE ShortIndex count() const					{ return _count;	}

	HSFM_INLINE		  Interface& operator[] (const ShortIndex i);
	HSFM_INLINE const Interface& operator[] (const ShortIndex i) const;

	template <typename TItem, typename... TArgs>
	ShortIndex emplace(TArgs&&... args);

private:
	ShortIndex _offsets[ITEM_COUNT];
	uint8_t _storage[STORAGE_SIZE];
	ShortIndex _count = 0;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "lib_object_pool.inl"
