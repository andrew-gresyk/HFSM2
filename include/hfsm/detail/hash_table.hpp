#pragma once

#include "utility.hpp"
#include "wrap.hpp"

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// Open-addressing, Robin Hood hashing associative container

template <typename TKey,
		  typename TValue,
		  unsigned TCapacity,
		  typename THasher = std::hash<TKey>>
class HashTable {
public:
	enum : unsigned {
		REQUESTED_CAPACITY = TCapacity,
		CAPACITY	= NextPowerOf2<REQUESTED_CAPACITY>::Value,
		INDEX_MASK	= CAPACITY - 1,
		USED_SHIFT	= 31u,
		USED_MASK	= 1u << USED_SHIFT,
		HASH_MASK	= ~USED_MASK,
		INVALID		= (unsigned)-1,
		DUMMY		= INVALID,
	};
	static_assert(CAPACITY <= HASH_MASK, "Capacity needs to be less than HASH_MASK 0x7fffffff");

	using Hash		= unsigned;
	using Key		= TKey;
	using Value		= TValue;
	using Hasher	= THasher;

	//----------------------------------------------------------------------

	class Item {
	public:
		inline Item() = default;

		inline Item(const Hash hash, const Key key);
		inline Item(const Hash hash, const Key key, const Value value);

		inline void swap(Item& other);
		inline bool operator == (const Item& other) const;

		inline unsigned hash() const		{ return _hash & HASH_MASK;			}

		inline bool vacant() const			{ return _hash >> USED_SHIFT == 0;	}
		inline bool occupied() const		{ return _hash >> USED_SHIFT != 0;	}
		inline void vacate()				{ _hash &= !USED_MASK;				}

		inline const Key& key() const		{ return *_keyWrap;					}

		inline		 Value* value()			{ return &_value;					}
		inline const Value* value() const	{ return &_value;					}

	private:
		Hash _hash = 0;
		Wrap<Key> _keyWrap;
		Value _value;
	};
	typedef Item Items[CAPACITY];

	//----------------------------------------------------------------------

	struct Stats {
		inline Stats(const float loadFactor_, const float averageProbeCount_)
			: loadFactor(loadFactor_)
			, averageProbeCount(averageProbeCount_)
		{}

		float loadFactor = 0.0f;
		float averageProbeCount = 0.0f;
	};

	//----------------------------------------------------------------------

public:
	HashTable() = default;

	bool insert(const Key key, const Value value);

		  Value* find(const Key key);
	const Value* find(const Key key) const;

	inline unsigned count() const						{ return _count;						}

private:
	unsigned locate(const Key key) const;

	inline unsigned probeCount(const unsigned i) const;

	inline unsigned index(const unsigned hash) const	{ return hash & INDEX_MASK;				}
	inline unsigned hash(const Key key) const			{ return Hasher()(key) & HASH_MASK;		}

	inline unsigned skipVacantForward(const unsigned i) const;

private:
	Items _items;
	unsigned _count = 0;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "hash_table.inl"
