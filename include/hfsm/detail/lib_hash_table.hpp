#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// Open-addressing, Robin Hood hashing associative container

template <typename TKey,
		  typename TValue,
		  LongIndex NCapacity,
		  typename THasher = std::hash<TKey>>
class HashTable {
	using Key		= TKey;
	using Value		= TValue;
	using Hasher	= THasher;

	using Hash		= uint32_t;
	static_assert(sizeof(Hash) <= sizeof(decltype(&Hasher::operator())), "'Hash' not wide enough");

public:
	enum : LongIndex {
		CAPACITY	= NCapacity,
		INVALID		= INVALID_LONG_INDEX,
		DUMMY		= INVALID,
	};

	//----------------------------------------------------------------------

#pragma pack(push, 2)

	class Item {
	public:
		inline Item()
			: _hash(0)
		{
			assert((((uintptr_t) this) & (sizeof(void*) - 1)) == 0);
		}

		inline Item(const Hash hash, const Key key);

		inline Item(const Item& other)
			: _keyWrap(other._keyWrap)
			, _value  (other._value)
		{}

		inline Item(Item&& other)
			: _keyWrap(std::move(other._keyWrap))
			, _value  (std::move(other._value))
		{}

		void operator = (const Item& other) {
			_keyWrap = other._keyWrap;
			_value	 = other._value;
		}

		void operator = (Item&& other) {
			_keyWrap = std::move(other._keyWrap);
			_value	 = std::move(other._value);
		}

		inline void swap(Item& other);
		inline bool operator == (const Item& other) const;

		inline Hash hash() const			{ return _hash;			}

		inline bool vacant() const			{ return _hash == 0;	}
		inline bool occupied() const		{ return _hash != 0;	}

		inline auto key() const				{ return *_keyWrap;		}

		inline		 auto* value()			{ return &_value;		}
		inline const auto* value() const	{ return &_value;		}

	private:
		alignas(alignof(Key)) Wrap<Key> _keyWrap;
		union {
			Hash _hash;
			Value _value;
		};
	};
	typedef Item Items[CAPACITY];

#pragma pack(pop)

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

	bool insert(const Key key);

		  Value* find(const Key key);
	const Value* find(const Key key) const;

	inline auto count() const						{ return _count;						}

private:
	LongIndex locate(const Key key) const;

	inline LongIndex probeCount(const LongIndex i) const;

	inline LongIndex index(const Hash hash) const	{ return (LongIndex)(hash % CAPACITY);	}
	inline Hash hash(const Key key) const			{ return (Hash) Hasher()(key);			}

	inline LongIndex skipVacantForward(const LongIndex i) const;

private:
	LongIndex _count = 0;
	Items _items;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "lib_hash_table.inl"
