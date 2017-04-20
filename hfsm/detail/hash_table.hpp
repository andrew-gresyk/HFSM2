#pragma once

#include "utility.hpp"
#include "wrap.hpp"

#include <cassert>
#include <tuple>

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// Open-addressing, Robin Hood hashing associative container

template <typename TKey, typename TValue, unsigned TCapacity, typename THasher = std::hash<TKey>>
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
	using KeyValue	= std::tuple<Key, Value>;

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

	HashTable(const std::initializer_list<KeyValue> pairs) {
		for (const auto& pair : pairs)
			HSFM_CHECKED(insert(std::get<0>(pair), std::get<1>(pair)));
	}

	void clear();

	bool insert(const Key key, const Value value);
	bool remove(const Key key);

		  Value* find(const Key key);
	const Value* find(const Key key) const;

	inline unsigned count() const						{ return _count;						}

	Stats measure() const;

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

template <typename TK, typename TV, unsigned TC, typename TH>
HashTable<TK, TV, TC, TH>::Item::Item(const Hash hash, const Key key)
	: _hash(hash | USED_MASK)
	, _keyWrap(key)
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TK, typename TV, unsigned TC, typename TH>
HashTable<TK, TV, TC, TH>::Item::Item(const Hash hash, const Key key, const Value value)
	: _hash(hash | USED_MASK)
	, _keyWrap(key)
	, _value(value)
{}

//------------------------------------------------------------------------------

template <typename TK, typename TV, unsigned TC, typename TH>
void
HashTable<TK, TV, TC, TH>::Item::swap(Item& other) {
	std::swap(_hash, other._hash);
	std::swap(_keyWrap, other._keyWrap);
	std::swap(_value, other._value);
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, unsigned TC, typename TH>
bool
HashTable<TK, TV, TC, TH>::Item::operator == (const Item& other) const {
	return _hash == other._hash && *_keyWrap == *other._keyWrap;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TK, typename TV, unsigned TC, typename TH>
void
HashTable<TK, TV, TC, TH>::clear() {
	nullify(_items);
	_count = 0;
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, unsigned TC, typename TH>
bool
HashTable<TK, TV, TC, TH>::insert(const Key key, const Value value) {
	assert(_count < CAPACITY);

	if (_count < CAPACITY) {
		Item newcomer(hash(key), key, value);

		for (unsigned i = index(newcomer.hash()), newcomerDistance = 0;
			 ;
			 i = index(i + 1), ++newcomerDistance)
		{
			Item& resident = _items[i];

			if (resident.occupied()) {
				const unsigned residentDistance = probeCount(i);

				if (newcomerDistance > residentDistance) {
					newcomerDistance = residentDistance;

					std::swap(newcomer, resident);
				}
			}
			else {
				resident = newcomer;
				++_count;

				return true;
			}

			assert(newcomerDistance < CAPACITY);
		}
	}
	else
		return false;
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, unsigned TC, typename TH>
bool
HashTable<TK, TV, TC, TH>::remove(const Key key) {
	const unsigned index = locate(key);

	if (index != INVALID) {
		_items[index].vacate();

		assert(_count > 0);
		--_count;

		return true;
	}
	else
		return false;
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, unsigned TC, typename TH>
typename HashTable<TK, TV, TC, TH>::Value*
HashTable<TK, TV, TC, TH>::find(const Key key) {
	const unsigned index = locate(key);

	return index != INVALID ?
		_items[index].value() : nullptr;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TK, typename TV, unsigned TC, typename TH>
const typename HashTable<TK, TV, TC, TH>::Value*
HashTable<TK, TV, TC, TH>::find(const Key key) const {
	const unsigned index = locate(key);

	return index != INVALID ?
		_items[index].value() : nullptr;
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, unsigned TC, typename TH>
typename HashTable<TK, TV, TC, TH>::Stats
HashTable<TK, TV, TC, TH>::measure() const {
	unsigned itemCount = 0;
	unsigned totalProbeCount = 0;

	for (unsigned i = 0; i < CAPACITY; ++i)
		if (_items[i].occupied()) {
			++itemCount;
			assert(itemCount <= _count); // superfluous, but break at the first offending item

			totalProbeCount += probeCount(i);
		}

	assert(itemCount == _count);

	return Stats((float)itemCount / CAPACITY,
		(float)totalProbeCount / itemCount);
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, unsigned TC, typename TH>
unsigned
HashTable<TK, TV, TC, TH>::locate(const Key key) const {
	const Item item(hash(key), key);

	for (unsigned i = index(item.hash()), distance = 0;
		 ;
		 i = index(i + 1), ++distance)
	{
		const Item& resident = _items[i];

		if (item == resident)
			return i;
		else if (distance > probeCount(i))
			return INVALID;

		assert(distance < CAPACITY);
	}
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, unsigned TC, typename TH>
unsigned
HashTable<TK, TV, TC, TH>::probeCount(const unsigned i) const {
	assert(i < CAPACITY);

	return index(CAPACITY + i - index(_items[i].hash()));
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, unsigned TC, typename TH>
unsigned
HashTable<TK, TV, TC, TH>::skipVacantForward(const unsigned i) const {
	assert(0 <= i && i <= CAPACITY);

	if (i < CAPACITY) {
		unsigned n = i;
		for (; _items[n].vacant(); ++n)
			if (n < CAPACITY)
				continue;
			else
				return CAPACITY;

		return n;
	}
	else
		return i;
}

////////////////////////////////////////////////////////////////////////////////

}
}
