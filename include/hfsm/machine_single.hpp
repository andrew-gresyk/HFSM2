// HFSM (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk
//
// Licensed under the MIT License;
// you may not use this file except in compliance with the License.
//
//
// MIT License
//
// Copyright (c) 2017
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <assert.h>
#include <string.h>

#include <limits>
#include <typeindex>
#include <utility>



namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 4)

template <typename T>
class ArrayView {
public:
	using Item = T;

	template <typename>
	friend class Iterator;

protected:
	ArrayView(const unsigned capacity);
	~ArrayView();

public:
	inline void clear()										{ _count = 0;				}

	inline unsigned resize(const unsigned count);

	template <typename TValue>
	inline unsigned operator << (TValue&& value);

	inline		 Item& operator[] (const unsigned i)		{ return get(i);			}
	inline const Item& operator[] (const unsigned i) const	{ return get(i);			}

	inline unsigned capacity() const						{ return _capacity;			}
	inline unsigned count() const							{ return _count;			}

protected:
	inline unsigned first() const							{ return 0;					}
	inline unsigned limit() const							{ return _count;			}

	inline unsigned prev(const unsigned i) const			{ return i - 1;				}
	inline unsigned next(const unsigned i) const			{ return i + 1;				}

	inline		 Item& get(const unsigned i);
	inline const Item& get(const unsigned i) const;

private:
	// hacks
	inline		 Item* data()		{ return reinterpret_cast<		Item*>(&_count + 1);	}
	inline const Item* data() const	{ return reinterpret_cast<const Item*>(&_count + 1);	}

protected:
	const unsigned _capacity;
	unsigned _count = 0;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
ArrayView<T>::ArrayView(const unsigned capacity)
	: _capacity(capacity)
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
ArrayView<T>::~ArrayView() {
	if (_count > 0)
		for (int i = _count - 1; i >= 0; --i)
			get(i).~Item();
}

//------------------------------------------------------------------------------

template <typename T>
unsigned
ArrayView<T>::resize(const unsigned count) {
	const unsigned clampedCount = count < _capacity ?
		count : _capacity;

	if (clampedCount > _count) {
		for (unsigned i = _count; i < clampedCount; ++i)
			get(i) = Item();
	}
	else if (clampedCount < _count) {
		for (unsigned i = _count - 1; i >= clampedCount; --i)
			get(i).~Item();
	}

	return _count = clampedCount;
}

//------------------------------------------------------------------------------

template <typename T>
template <typename TValue>
unsigned
ArrayView<T>::operator << (TValue&& value) {
	assert(_count < _capacity);

	new (&get(_count)) Item(std::move(value));

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T>
T&
ArrayView<T>::get(const unsigned i) {
	assert(0 <= i && i < _capacity);

	return data()[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
const T&
ArrayView<T>::get(const unsigned i) const {
	assert(0 <= i && i < _capacity);

	return data()[i];
}

////////////////////////////////////////////////////////////////////////////////

}
}


#if defined _DEBUG && _MSC_VER
	#include <intrin.h>		// __debugbreak()
#endif

//------------------------------------------------------------------------------

#if defined _DEBUG && _MSC_VER
	#define HSFM_BREAK()			__debugbreak()
	#define HSFM_CHECKED(x)			(!!(x) || (HSFM_BREAK(), 0))
#else
	#define HSFM_BREAK()			((void) 0)
	#define HSFM_CHECKED(x)			x
#endif

#ifdef _DEBUG
	#define HSFM_IF_DEBUG(...)		__VA_ARGS__
#else
	#define HSFM_IF_DEBUG(...)
#endif

#ifndef NDEBUG
	#define HSFM_IF_ASSERT(...)		__VA_ARGS__
#else
	#define HSFM_IF_ASSERT(...)
#endif

//------------------------------------------------------------------------------

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
inline
void
fill(T& a, const char value) {
	memset(&a, (int) value, sizeof(a));
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, unsigned TCount>
inline
unsigned
count(const T(&)[TCount]) {
	return TCount;
}

//------------------------------------------------------------------------------

template <typename T, unsigned TCapacity>
inline
const T*
end(const T(& a)[TCapacity]) {
	return &a[TCapacity];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TReturn, typename T, unsigned TCapacity>
inline
const TReturn*
end(const T(& a)[TCapacity]) {
	return reinterpret_cast<const TReturn*>(&a[TCapacity]);
}

////////////////////////////////////////////////////////////////////////////////

template <int t1, int t2>
struct Min {
	enum {
		Value = t1 < t2 ? t1 : t2
	};
};

//------------------------------------------------------------------------------

template <int t1, int t2>
struct Max {
	enum {
		Value = t1 > t2 ? t1 : t2
	};
};

////////////////////////////////////////////////////////////////////////////////

template <unsigned t>
struct PowerOf2 {
	enum {
		Value = !(t & (t - 1))
	};
};

//------------------------------------------------------------------------------

template <unsigned t>
struct BitCount {
	enum {
		Value =
		t		== 0 ?  0 :
		t >>  1 == 0 ?  1 :
		t >>  2 == 0 ?  2 :
		t >>  3 == 0 ?  3 :
		t >>  4 == 0 ?  4 :
		t >>  5 == 0 ?  5 :
		t >>  6 == 0 ?  6 :
		t >>  7 == 0 ?  7 :

		t >>  8 == 0 ?  8 :
		t >>  9 == 0 ?  9 :
		t >> 10 == 0 ? 10 :
		t >> 11 == 0 ? 11 :
		t >> 12 == 0 ? 12 :
		t >> 13 == 0 ? 13 :
		t >> 14 == 0 ? 14 :
		t >> 15 == 0 ? 15 :

		t >> 16 == 0 ? 16 :
		t >> 17 == 0 ? 17 :
		t >> 18 == 0 ? 18 :
		t >> 19 == 0 ? 19 :
		t >> 20 == 0 ? 20 :
		t >> 21 == 0 ? 21 :
		t >> 22 == 0 ? 22 :
		t >> 23 == 0 ? 23 :

		t >> 24 == 0 ? 24 :
		t >> 25 == 0 ? 25 :
		t >> 26 == 0 ? 26 :
		t >> 27 == 0 ? 27 :
		t >> 28 == 0 ? 28 :
		t >> 29 == 0 ? 29 :
		t >> 30 == 0 ? 30 :
		t >> 31 == 0 ? 31 :
					   32
	};
};

//------------------------------------------------------------------------------

template <unsigned t>
struct NextPowerOf2 {
	enum {
		Value = PowerOf2<t>::Value ?
			t : 1 << BitCount<t>::Value
	};
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContainer>
class Iterator {
public:
	using Container = TContainer;
	using Item = typename Container::Item;

	template <typename T, unsigned TCapacity>
	friend class Array;

private:
	inline Iterator(Container& container, const unsigned cursor)
		: _container(container)
		, _cursor(cursor)
	{}

public:
	inline bool operator != (const Iterator<Container>& dummy) const;

	inline Iterator& operator ++();

	inline		 Item& operator *()		  { return _container[_cursor]; }
	inline const Item& operator *() const { return _container[_cursor]; }

	inline		 Item* operator->()		  { return &operator *();		}
	inline const Item* operator->() const { return &operator *();		}

private:
	Container& _container;

	unsigned _cursor;
};

//------------------------------------------------------------------------------

template <typename TContainer>
class Iterator<const TContainer> {
public:
	using Container = TContainer;
	using Item = typename Container::Item;

	template <typename T, unsigned TCapacity>
	friend class Array;

private:
	inline Iterator(const Container& container, const unsigned cursor)
		: _container(container)
		, _cursor(cursor)
	{}

public:
	inline bool operator != (const Iterator<const Container>& dummy) const;

	inline Iterator& operator ++();

	inline const Item& operator *() const { return _container[_cursor]; }

	inline const Item* operator->() const { return &operator *();		}

private:
	const Container& _container;

	unsigned _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContainer>
bool
Iterator<TContainer>::operator != (const Iterator<TContainer>& HSFM_IF_ASSERT(dummy)) const {
	assert(&_container == &dummy._container);

	return _cursor != _container.limit();
}

//------------------------------------------------------------------------------

template <typename TContainer>
Iterator<TContainer>&
Iterator<TContainer>::operator ++() {
	_cursor = _container.next(_cursor);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TContainer>
bool
Iterator<const TContainer>::operator != (const Iterator<const TContainer>& HSFM_IF_ASSERT(dummy)) const {
	assert(&_container == &dummy._container);

	return _cursor != _container.limit();
}

//------------------------------------------------------------------------------

template <typename TContainer>
Iterator<const TContainer>&
Iterator<const TContainer>::operator ++() {
	_cursor = _container.next(_cursor);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 4)

template <typename T, unsigned TCapacity>
class StaticArray {
public:
	enum {
		CAPACITY  = TCapacity,
	};

	using Item  = T;
	using Index = unsigned char; // TODO: adjust to CAPACITY
	static_assert(CAPACITY <= std::numeric_limits<Index>::max(), "");

public:
	inline StaticArray() = default;

	inline		 Item& operator[] (const unsigned i);
	inline const Item& operator[] (const unsigned i) const;

	inline const unsigned count() const						{ return CAPACITY; }

private:
	Item _items[CAPACITY];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
struct StaticArray<T, 0> {};

//------------------------------------------------------------------------------

template <typename T, unsigned TCapacity>
class Array
	: public ArrayView<T>
{
public:
	enum : unsigned {
		CAPACITY = TCapacity,
		INVALID	 = (unsigned)-1,
		DUMMY	 = INVALID,
	};

	using View = ArrayView<T>;
	using Item = typename View::Item;

public:
	Array()
		: View(CAPACITY)
	{
		assert(&View::get(0) == _storage);
	}

	inline Iterator<	  Array>  begin()		{ return Iterator<		Array>(*this, View::first()); }
	inline Iterator<const Array>  begin() const { return Iterator<const Array>(*this, View::first()); }
	inline Iterator<const Array> cbegin() const { return Iterator<const Array>(*this, View::first()); }

	inline Iterator<	  Array>	end()		{ return Iterator<		Array>(*this, DUMMY);		  }
	inline Iterator<const Array>	end() const { return Iterator<const Array>(*this, DUMMY);		  }
	inline Iterator<const Array>   cend() const { return Iterator<const Array>(*this, DUMMY);		  }

private:
	Item _storage[CAPACITY];
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, unsigned TCapacity>
T&
StaticArray<T, TCapacity>::operator[] (const unsigned i) {
	assert(0 <= i && i < CAPACITY);

	return _items[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, unsigned TCapacity>
const T&
StaticArray<T, TCapacity>::operator[] (const unsigned i) const {
	assert(0 <= i && i < CAPACITY);

	return _items[i];
}

////////////////////////////////////////////////////////////////////////////////

}
}


namespace hfsm {
namespace detail {

//------------------------------------------------------------------------------

template <typename T>
class Wrap;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
class Wrap {
	using Item = T;
	using Storage = typename std::aligned_storage<sizeof(Item), sizeof(Item)>::type;

public:
	inline Wrap() = default;

	template <typename... Ts>
	inline void create(Ts&&... ts)			{ new (&get()) Item(std::forward<Ts>(ts)...);	}

	inline Wrap(const Item& item)							{ get() = item;					}
	inline Wrap(Item&& item)								{ get() = std::move(item);		}

	inline Wrap& operator = (const Item& item)	{ get() = item;				return *this;	}
	inline Wrap& operator = (Item&& item)		{ get() = std::move(item);	return *this;	}

	inline void clear()										{ fill(_storage, 0);			}

	inline		 T& operator *()							{ return  get();				}
	inline const T& operator *() const						{ return  get();				}

	inline		 T* operator->()							{ return &get();				}
	inline const T* operator->() const						{ return &get();				}

	inline explicit operator bool() const					{ return _storage != 0;			}

	inline bool operator == (const Wrap other) const		{ return get() == other.get();	}

private:
	inline		 T& get()			{ return *reinterpret_cast<		 T* const>(&_storage);	}
	inline const T& get() const		{ return *reinterpret_cast<const T* const>(&_storage);	}

private:
	Storage _storage;
};

////////////////////////////////////////////////////////////////////////////////

}
}

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

namespace hfsm {
namespace detail {

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


namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

class TypeInfo
	: public Wrap<std::type_index>
{
	using Base = Wrap<std::type_index>;

public:
	typedef std::type_index Native;

public:
	inline TypeInfo() = default;

	inline TypeInfo(const std::type_index type)
		: Base(type)
	{}

	template <typename T>
	static inline TypeInfo get() { return TypeInfo(typeid(T)); }
};

////////////////////////////////////////////////////////////////////////////////

}
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	#define HFSM_IF_STRUCTURE(...)	__VA_ARGS__
#else
	#define HFSM_IF_STRUCTURE(...)
#endif

#ifdef HFSM_ENABLE_LOG_INTERFACE
	#define HFSM_IF_LOGGER(...)		__VA_ARGS__
	#define HFSM_LOGGER_OR(y, n)	y
#else
	#define HFSM_IF_LOGGER(...)
	#define HFSM_LOGGER_OR(y, n)	n
#endif

namespace hfsm {

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
struct StructureEntry {
	bool isActive;
	const wchar_t* prefix;
	const char* name;
};
using MachineStructure = detail::ArrayView<StructureEntry>;
using MachineActivity  = detail::ArrayView<char>;
#endif

#ifdef HFSM_ENABLE_LOG_INTERFACE
struct LoggerInterface {
	enum class Method {
		Substitute,
		Enter,
		Update,
		Transition,
		React,
		Leave,
	};

	virtual void record(const std::type_index& state,
						const char* const stateName,
						const Method method,
						const char* const methodName) = 0;
};

static const char* methodName(const LoggerInterface::Method method) {
	switch (method) {
		case LoggerInterface::Method::Substitute:	return "substitute"	"()";
		case LoggerInterface::Method::Enter:		return "enter"		"()";
		case LoggerInterface::Method::Update:		return "update"		"()";
		case LoggerInterface::Method::Transition:	return "transition"	"()";
		case LoggerInterface::Method::React:		return "react"		"()";
		case LoggerInterface::Method::Leave:		return "leave"		"()";

		default:
			HSFM_BREAK();
			return nullptr;
	}
}
#else
using LoggerInterface = void;
#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TContext, unsigned TMaxSubstitutions = 4>
class M {
	using TypeInfo = detail::TypeInfo;

	template <typename T, unsigned TCapacity>
	using Array = detail::Array<T, TCapacity>;

	template <typename T>
	using ArrayView = detail::ArrayView<T>;

	template <typename TKey, typename TValue, unsigned TCapacity, typename THasher = std::hash<TKey>>
	using HashTable = detail::HashTable<TKey, TValue, TCapacity, THasher>;

	//----------------------------------------------------------------------


public:
	using Context = TContext;
	class Control;

private:
	using Index = unsigned char;
	enum : Index { INVALID_INDEX = std::numeric_limits<Index>::max() };

	enum : unsigned { MaxSubstitutions = TMaxSubstitutions };

	//----------------------------------------------------------------------

	struct Parent {
		#pragma pack(push, 1)
			Index fork  = INVALID_INDEX;
			Index prong = INVALID_INDEX;
		#pragma pack(pop)

		HSFM_IF_DEBUG(TypeInfo forkType);
		HSFM_IF_DEBUG(TypeInfo prongType);

		inline Parent() = default;

		inline Parent(const Index fork_,
					  const Index prong_
					  HSFM_IF_DEBUG(, const TypeInfo forkType_)
					  HSFM_IF_DEBUG(, const TypeInfo prongType_))
			: fork(fork_)
			, prong(prong_)
			HSFM_IF_DEBUG(, forkType(forkType_))
			HSFM_IF_DEBUG(, prongType(prongType_))
		{}

		inline explicit operator bool() const { return fork != INVALID_INDEX && prong != INVALID_INDEX; }
	};

	using Parents = ArrayView<Parent>;

	//----------------------------------------------------------------------

	struct StateRegistry {
	public:
		virtual unsigned add(const TypeInfo stateType) = 0;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <unsigned TCapacity>
	class StateRegistryT
		: public StateRegistry
	{
		enum : unsigned { Capacity = TCapacity };

		using TypeToIndex = HashTable<TypeInfo::Native, unsigned, Capacity>;

	public:
		inline unsigned operator[] (const TypeInfo stateType) const { return *_typeToIndex.find(*stateType); }

		virtual unsigned add(const TypeInfo stateType) override;

	private:
		TypeToIndex _typeToIndex;
	};

	//----------------------------------------------------------------------

	struct Fork {
		#pragma pack(push, 1)
			Index self		= INVALID_INDEX;
			Index active	= INVALID_INDEX;
			Index resumable = INVALID_INDEX;
			Index requested = INVALID_INDEX;
		#pragma pack(pop)

		HSFM_IF_DEBUG(const TypeInfo type);
		HSFM_IF_ASSERT(TypeInfo activeType);
		HSFM_IF_ASSERT(TypeInfo resumableType);
		HSFM_IF_ASSERT(TypeInfo requestedType);

		Fork(const Index index, const TypeInfo type_);
	};
	using ForkPointers = ArrayView<Fork*>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename T>
	struct ForkT
		: Fork
	{
		ForkT(const Index index,
			  const Parent parent,
			  Parents& forkParents)
			: Fork(index, TypeInfo::get<T>())
		{
			forkParents[index] = parent;
		}
	};

	//----------------------------------------------------------------------

	struct Transition {
		enum Type {
			Remain,
			Restart,
			Resume,
			Schedule,

			COUNT
		};

		Type type = Restart;
		TypeInfo stateType;

		inline Transition() = default;

		inline Transition(const Type type_, const TypeInfo stateType_)
			: type(type_)
			, stateType(stateType_)
		{
			assert(type_ < Type::COUNT);
		}
	};
	using TransitionQueue = ArrayView<Transition>;

	//----------------------------------------------------------------------

	template <typename>
	struct _S;

	template <typename, typename...>
	struct _C;

	template <typename, typename...>
	struct _O;

	template <typename>
	class _R;

	//----------------------------------------------------------------------

	template <typename... TS>
	struct WrapState;

	template <typename T>
	struct WrapState<T> {
		using Type = _S<T>;
	};

	template <typename T>
	struct WrapState<_S<T>> {
		using Type = _S<T>;
	};

	template <typename T, typename... TS>
	struct WrapState<_C<T, TS...>> {
		using Type = _C<T, TS...>;
	};

	template <typename T, typename... TS>
	struct WrapState<_O<T, TS...>> {
		using Type = _O<T, TS...>;
	};

	//----------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	struct StateInfo {
		enum RegionType {
			Composite,
			Orthogonal,
		};

		StateInfo() {}

		StateInfo(const unsigned parent_,
				  const RegionType region_,
				  const unsigned depth_,
				  const char* const name_)
			: parent(parent_)
			, region(region_)
			, depth(depth_)
			, name(name_)
		{}

		unsigned parent;
		RegionType region;
		unsigned depth;
		const char* name;
	};
	using StateInfos = detail::ArrayView<StateInfo>;

	using StateStructure = detail::ArrayView<StructureEntry>;
#endif


	//----------------------------------------------------------------------


public:

	class Bare {
		template <typename...>
		friend struct _B;

	protected:
		using Control	 = typename M::Control;
		using Context	 = typename M::Context;
		using Transition = typename M::Transition;

	public:
		inline void preSubstitute(Context&)				{}
		inline void preEnter(Context&)					{}
		inline void preUpdate(Context&)					{}
		inline void preTransition(Context&)				{}
		template <typename TEvent>
		inline void preReact(const TEvent&, Context&)	{}
		inline void postLeave(Context&)					{}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
private:

	template <typename...>
	struct _B;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TInjection, typename... TRest>
	struct _B<TInjection, TRest...>
		: public TInjection
		, public _B<TRest...>
	{
		inline void widePreSubstitute(Context& context);
		inline void widePreEnter(Context& context);
		inline void widePreUpdate(Context& context);
		inline void widePreTransition(Context& context);
		template <typename TEvent>
		inline void widePreReact(const TEvent& event, Context& context);
		inline void widePostLeave(Context& context);
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TInjection>
	struct _B<TInjection>
		: public TInjection
	{
		inline void substitute(Control&, Context&)				{}
		inline void enter(Context&)								{}
		inline void update(Context&)							{}
		inline void transition(Control&, Context&)				{}
		template <typename TEvent>
		inline void react(const TEvent&, Control&, Context&)	{}
		inline void leave(Context&)								{}

		inline void widePreSubstitute(Context& context);
		inline void widePreEnter(Context& context);
		inline void widePreUpdate(Context& context);
		inline void widePreTransition(Context& context);
		template <typename TEvent>
		inline void widePreReact(const TEvent& event, Context& context);
		inline void widePostLeave(Context& context);
	};


	//----------------------------------------------------------------------


	template <typename TApex>
	class _R final {
		using Apex = typename WrapState<TApex>::Type;

	public:
		enum : unsigned {
			ReverseDepth  = Apex::ReverseDepth,
			DeepWidth	  = Apex::DeepWidth,
			StateCount	  = Apex::StateCount,
			ForkCount	  = Apex::ForkCount,
			ProngCount	  = Apex::ProngCount,
			Width		  = Apex::Width,
		};
		static_assert(StateCount < std::numeric_limits<Index>::max(), "Too many states in the hierarchy. Change 'Index' type.");

	private:
		enum : unsigned {
			StateCapacity = (unsigned) 1.3 * Apex::StateCount,
		};

		using StateRegistryImpl		 = StateRegistryT<StateCapacity>;
		using StateParentStorage	 = Array<Parent, StateCount>;
		using ForkParentStorage		 = Array<Parent, ForkCount>;
		using ForkPointerStorage	 = Array<Fork*, ForkCount>;
		using TransitionQueueStorage = Array<Transition, ForkCount>;

	#ifdef HFSM_ENABLE_STRUCTURE_REPORT
		enum : unsigned {
			NameCount	  = Apex::NameCount,
		};

		using Prefix				 = detail::StaticArray<wchar_t, ReverseDepth * 2 + 2>;
		using Prefixes				 = detail::StaticArray<Prefix, StateCount>;

		using StateInfoStorage		 = detail::Array<StateInfo, StateCount>;

		using StructureStorage		 = detail::Array<StructureEntry, NameCount>;
		using ActivityHistoryStorage = detail::Array<char, NameCount>;
	#endif

	public:
		_R(Context& context
		   HFSM_IF_LOGGER(, LoggerInterface* const logger = nullptr));

		~_R();

		void update();

		template <typename TEvent>
		inline void react(const TEvent& event);

		template <typename T>
		inline void changeTo()	{ _requests << Transition(Transition::Type::Restart,  TypeInfo::get<T>());	}

		template <typename T>
		inline void resume()	{ _requests << Transition(Transition::Type::Resume,   TypeInfo::get<T>());	}

		template <typename T>
		inline void schedule()	{ _requests << Transition(Transition::Type::Schedule, TypeInfo::get<T>());	}

		template <typename T>
		inline bool isActive();

		template <typename T>
		inline bool isResumable();

	#ifdef HFSM_ENABLE_STRUCTURE_REPORT
		const MachineStructure& structure() const								{ return _structure;		};
		const MachineActivity&  activity()  const								{ return _activityHistory;	};
	#endif

	#ifdef HFSM_ENABLE_LOG_INTERFACE
		void attachLogger(LoggerInterface* const logger)						{ _logger = logger;			}
	#endif

	protected:
		void processTransitions();
		void requestImmediate(const Transition request);
		void requestScheduled(const Transition request);

		inline unsigned id(const Transition request) const	{ return _stateRegistry[*request.stateType];	}

	#ifdef HFSM_ENABLE_STRUCTURE_REPORT
		void getStateNames();
		void udpateActivity();
	#endif

	private:
		Context& _context;

		StateRegistryImpl _stateRegistry;

		StateParentStorage _stateParents;
		ForkParentStorage  _forkParents;
		ForkPointerStorage _forkPointers;

		TransitionQueueStorage _requests;

		Apex _apex;

	#ifdef HFSM_ENABLE_STRUCTURE_REPORT
		Prefixes _prefixes;
		StateInfoStorage _stateInfos;

		StructureStorage _structure;
		ActivityHistoryStorage _activityHistory;

		struct DebugTransitionInfo {
			typename Transition::Type type;
			TypeInfo state;

			enum Source {
				Update,
				Substitute,
				Linger,

				COUNT
			};
			Source source;

			inline DebugTransitionInfo() = default;

			inline DebugTransitionInfo(const Transition transition,
									   const Source source_)
				: type(transition.type)
				, state(transition.stateType)
				, source(source_)
			{
				assert(source_ < Source::COUNT);
			}
		};
		using DebugTransitionInfos = Array<DebugTransitionInfo, 2 * ForkCount>;
		DebugTransitionInfos _lastTransitions;
	#endif

		HFSM_IF_LOGGER(LoggerInterface* _logger);
	};

	//----------------------------------------------------------------------

public:

	class Control final {
		template <typename>
		friend class _R;

	private:
		Control(TransitionQueue& requests)
			: _requests(requests)
		{}

	public:
		template <typename T>
		inline void changeTo()	{ _requests << Transition(Transition::Type::Restart,  TypeInfo::get<T>());	}

		template <typename T>
		inline void resume()	{ _requests << Transition(Transition::Type::Resume,	  TypeInfo::get<T>());	}

		template <typename T>
		inline void schedule()	{ _requests << Transition(Transition::Type::Schedule, TypeInfo::get<T>());	}

		inline unsigned requestCount() const									{ return _requests.count();	}

	private:
		TransitionQueue& _requests;
	};


	//----------------------------------------------------------------------


	using Base = _B<Bare>;

	template <typename... TInjections>
	using BaseT = _B<TInjections...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TState, typename... TSubStates>
	using Composite	= _C<TState, TSubStates...>;

	template <typename... TSubStates>
	using CompositePeers = _C<Base, TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TState, typename... TSubStates>
	using Orthogonal = _O<TState, TSubStates...>;

	template <typename... TSubStates>
	using OrthogonalPeers = _O<Base, TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TState, typename... TSubStates>
	using Root = _R<Composite<TState, TSubStates...>>;

	template <typename... TSubStates>
	using PeerRoot = _R<CompositePeers<TSubStates...>>;

	template <typename TState, typename... TSubStates>
	using OrthogonalRoot = _R<Orthogonal<TState, TSubStates...>>;

	template <typename... TSubStates>
	using OrthogonalPeerRoot = _R<OrthogonalPeers<TSubStates...>>;

	//----------------------------------------------------------------------

};

template <typename TContext, unsigned TMaxSubstitutions = 4>
using Machine = M<TContext, TMaxSubstitutions>;

////////////////////////////////////////////////////////////////////////////////

}

namespace hfsm {

////////////////////////////////////////////////////////////////////////////////


template <typename TC, unsigned TMS>
M<TC, TMS>::Fork::Fork(const Index index,
					   const TypeInfo HSFM_IF_DEBUG(type_))
	: self(index)
	HSFM_IF_DEBUG(, type(type_))
{}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <unsigned TCapacity>
unsigned
M<TC, TMS>::StateRegistryT<TCapacity>::add(const TypeInfo stateType) {
	const unsigned index = _typeToIndex.count();

	HSFM_CHECKED(_typeToIndex.insert(*stateType, index));

	return index;
}


////////////////////////////////////////////////////////////////////////////////


template <typename TC, unsigned TMS>
template <typename TI, typename... TR>
void
M<TC, TMS>::_B<TI, TR...>::widePreSubstitute(Context& context) {
	TI::preSubstitute(context);
	_B<TR...>::widePreSubstitute(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI, typename... TR>
void
M<TC, TMS>::_B<TI, TR...>::widePreEnter(Context& context) {
	TI::preEnter(context);
	_B<TR...>::widePreEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI, typename... TR>
void
M<TC, TMS>::_B<TI, TR...>::widePreUpdate(Context& context) {
	TI::preUpdate(context);
	_B<TR...>::widePreUpdate(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI, typename... TR>
void
M<TC, TMS>::_B<TI, TR...>::widePreTransition(Context& context) {
	TI::preTransition(context);
	_B<TR...>::widePreTransition(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI, typename... TR>
template <typename TEvent>
void
M<TC, TMS>::_B<TI, TR...>::widePreReact(const TEvent& event,
										Context& context)
{
	TI::preReact(event, context);
	_B<TR...>::widePreReact(event, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI, typename... TR>
void
M<TC, TMS>::_B<TI, TR...>::widePostLeave(Context& context) {
	TI::postLeave(context);
	_B<TR...>::widePostLeave(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TI>
void
M<TC, TMS>::_B<TI>::widePreSubstitute(Context& context) {
	TI::preSubstitute(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI>
void
M<TC, TMS>::_B<TI>::widePreEnter(Context& context) {
	TI::preEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI>
void
M<TC, TMS>::_B<TI>::widePreUpdate(Context& context) {
	TI::preUpdate(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI>
void
M<TC, TMS>::_B<TI>::widePreTransition(Context& context) {
	TI::preTransition(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI>
template <typename TEvent>
void
M<TC, TMS>::_B<TI>::widePreReact(const TEvent& event,
								 Context& context)
{
	TI::preReact(event, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI>
void
M<TC, TMS>::_B<TI>::widePostLeave(Context& context) {
	TI::postLeave(context);
}


////////////////////////////////////////////////////////////////////////////////


template <typename TC, unsigned TMS>
template <typename TA>
M<TC, TMS>::_R<TA>::_R(Context& context
					   HFSM_IF_LOGGER(, LoggerInterface* const logger = nullptr))
	: _context(context)
	, _apex(_stateRegistry, Parent(), _stateParents, _forkParents, _forkPointers)
	HFSM_IF_LOGGER(, _logger(logger))
{
	HFSM_IF_STRUCTURE(getStateNames());

	_apex.deepEnterInitial(_context, HFSM_LOGGER_OR(_logger, nullptr));

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TA>
M<TC, TMS>::_R<TA>::~_R() {
	_apex.deepLeave(_context, HFSM_LOGGER_OR(_logger, nullptr));
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TA>
void
M<TC, TMS>::_R<TA>::update() {
	Control control(_requests);
	_apex.deepUpdateAndTransition(control, _context, HFSM_LOGGER_OR(_logger, nullptr));

	if (_requests.count())
		processTransitions();
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TA>
template <typename TEvent>
void
M<TC, TMS>::_R<TA>::react(const TEvent& event) {
	Control control(_requests);
	_apex.deepReact(event, control, _context, HFSM_LOGGER_OR(_logger, nullptr));

	if (_requests.count())
		processTransitions();
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TA>
template <typename T>
bool
M<TC, TMS>::_R<TA>::isActive() {
	using Type = T;

	const auto stateType = TypeInfo::get<Type>();
	const auto state = _stateRegistry[*stateType];

	for (auto parent = _stateParents[state]; parent; parent = _forkParents[parent.fork]) {
		const auto& fork = *_forkPointers[parent.fork];

		if (fork.active != INVALID_INDEX)
			return parent.prong == fork.active;
	}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TA>
template <typename T>
bool
M<TC, TMS>::_R<TA>::isResumable() {
	using Type = T;

	const auto stateType = TypeInfo::get<Type>();
	const auto state = _stateRegistry[*stateType];

	for (auto parent = _stateParents[state]; parent; parent = _forkParents[parent.fork]) {
		const auto& fork = *_forkPointers[parent.fork];

		if (fork.active != INVALID_INDEX)
			return parent.prong == fork.resumable;
	}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TA>
void
M<TC, TMS>::_R<TA>::processTransitions() {
	HFSM_IF_STRUCTURE(_lastTransitions.clear());

	for (unsigned i = 0;
		i < MaxSubstitutions && _requests.count();
		++i)
	{
		unsigned changeCount = 0;

		for (const auto& request : _requests) {
			HFSM_IF_STRUCTURE(_lastTransitions << DebugTransitionInfo(request, DebugTransitionInfo::Update));

			switch (request.type) {
			case Transition::Restart:
			case Transition::Resume:
				requestImmediate(request);

				++changeCount;
				break;

			case Transition::Schedule:
				requestScheduled(request);
				break;

			default:
				assert(false);
			}
		}
		_requests.clear();

		if (changeCount > 0) {
			Control substitutionControl(_requests);
			_apex.deepForwardSubstitute(substitutionControl, _context, HFSM_LOGGER_OR(_logger, nullptr));

		#ifdef HFSM_ENABLE_STRUCTURE_REPORT
			for (const auto& request : _requests)
				_lastTransitions << DebugTransitionInfo(request, DebugTransitionInfo::Substitute);
		#endif
		}
	}

	_apex.deepChangeToRequested(_context, HFSM_LOGGER_OR(_logger, nullptr));

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TA>
void
M<TC, TMS>::_R<TA>::requestImmediate(const Transition request) {
	const unsigned state = id(request);

	for (auto parent = _stateParents[state]; parent; parent = _forkParents[parent.fork]) {
		auto& fork = *_forkPointers[parent.fork];

		HSFM_IF_DEBUG(fork.requestedType = parent.prongType);
		fork.requested = parent.prong;
	}

	_apex.deepForwardRequest(request.type);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TA>
void
M<TC, TMS>::_R<TA>::requestScheduled(const Transition request) {
	const unsigned state = id(request);

	const auto parent = _stateParents[state];
	auto& fork = *_forkPointers[parent.fork];

	HSFM_IF_ASSERT(const auto forksParent = _stateParents[fork.self]);
	HSFM_IF_ASSERT(const auto& forksFork = *_forkPointers[forksParent.fork]);
	assert(forksFork.active == INVALID_INDEX);

	HSFM_IF_DEBUG(fork.resumableType = parent.prongType);
	fork.resumable = parent.prong;
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, unsigned TMS>
template <typename TA>
void
M<TC, TMS>::_R<TA>::getStateNames() {
	_stateInfos.clear();
	_apex.deepGetNames((unsigned) -1, StateInfo::Composite, 0, _stateInfos);

	unsigned margin = (unsigned) -1;
	for (unsigned s = 0; s < _stateInfos.count(); ++s) {
		const auto& state = _stateInfos[s];
		auto& prefix      = _prefixes[s];

		if (margin > state.depth && state.name[0] != '\0')
			margin = state.depth;

		if (state.depth == 0)
			prefix[0] = L'\0';
		else {
			const auto mark = state.depth * 2 - 1;

			prefix[mark + 0] = state.region == StateInfo::Composite ? L'└' : L'╙';
			prefix[mark + 1] = L' ';
			prefix[mark + 2] = L'\0';

			for (unsigned d = mark; d > 0; --d)
				prefix[d - 1] = L' ';

			for (unsigned r = s; r > state.parent; --r) {
				auto& prefixAbove = _prefixes[r - 1];

				switch (prefixAbove[mark]) {
				case L' ':
					prefixAbove[mark] = state.region == StateInfo::Composite ? L'│' : L'║';
					break;
				case L'└':
					prefixAbove[mark] = L'├';
					break;
				case L'╙':
					prefixAbove[mark] = L'╟';
					break;
				}
			}
		}
	}
	if (margin > 0)
		margin -= 1;

	_structure.clear();
	for (unsigned s = 0; s < _stateInfos.count(); ++s) {
		const auto& state = _stateInfos[s];
		auto& prefix = _prefixes[s];
		const auto space = state.depth * 2;

		if (state.name[0] != L'\0') {
			_structure << StructureEntry { false, &prefix[margin * 2], state.name };
			_activityHistory << (char) 0;
		} else if (s + 1 < _stateInfos.count()) {
			auto& nextPrefix = _prefixes[s + 1];

			if (s > 0)
				for (unsigned c = 0; c <= space; ++c)
					nextPrefix[c] = prefix[c];

			const auto mark = space + 1;

			switch (nextPrefix[mark]) {
			case L'├':
				nextPrefix[mark] = state.depth == margin ? L'┌' : L'┬';
				break;
			case L'╟':
				nextPrefix[mark] = state.depth == margin ? L'╓' : L'╥';
				break;
			}
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TA>
void
M<TC, TMS>::_R<TA>::udpateActivity() {
	for (auto& item : _structure)
		item.isActive = false;

	unsigned index = 0;
	_apex.deepIsActive(true, index, _structure);

	for (unsigned i = 0; i < _structure.count(); ++i) {
		auto& activity = _activityHistory[i];

		if (_structure[i].isActive) {
			if (activity > 0)
				activity = activity < std::numeric_limits<char>::max() ? activity + 1 : activity;
			else
				activity = +1;
		} else {
			if (activity > 0)
				activity = -1;
			else
				activity = activity > std::numeric_limits<char>::min() ? activity - 1 : activity;
		}
	}
}

#endif


////////////////////////////////////////////////////////////////////////////////

}

namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext, unsigned TMaxSubstitutions>
template <typename TH>
struct M<TContext, TMaxSubstitutions>::_S {
	using Head = TH;

	enum : unsigned {
		ReverseDepth = 1,
		DeepWidth	 = 0,
		StateCount	 = 1,
		ForkCount	 = 0,
		ProngCount	 = 0,
		Width		 = 1,
	};

	_S(StateRegistry& stateRegistry,
	   const Parent parent,
	   Parents& stateParents,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

	inline void deepForwardSubstitute	(Control&,		   Context&,		 LoggerInterface* const)		{}
	inline bool deepSubstitute			(Control& control, Context& context, LoggerInterface* const logger);

	inline void deepEnterInitial		(				   Context& context, LoggerInterface* const logger);
	inline void deepEnter				(				   Context& context, LoggerInterface* const logger);

	inline bool deepUpdateAndTransition	(Control& control, Context& context, LoggerInterface* const logger);
	inline void deepUpdate				(				   Context& context, LoggerInterface* const logger);

	template <typename TEvent>
	inline void deepReact				(const TEvent& event,
										 Control& control, Context& context, LoggerInterface* const logger);

	inline void deepLeave				(				   Context& context, LoggerInterface* const logger);

	inline void deepForwardRequest(const enum Transition::Type)												{}
	inline void deepRequestRemain()																			{}
	inline void deepRequestRestart()																		{}
	inline void deepRequestResume()																			{}
	inline void deepChangeToRequested	(				   Context&,		 LoggerInterface* const)		{}

#if defined HFSM_ENABLE_STRUCTURE_REPORT || defined HFSM_ENABLE_LOG_INTERFACE
	static constexpr bool isBare()		{ return std::is_same<Head, Base>::value; }

	enum : unsigned {
		NameCount	 = isBare() ? 0 : 1,
	};
#endif

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static const char* name();

	void deepGetNames(const unsigned parent,
					  const enum StateInfo::RegionType region,
					  const unsigned depth,
					  StateInfos& stateInfos) const;

	void deepIsActive(const bool isActive,
					  unsigned& index,
					  MachineStructure& structure) const;
#endif

#ifdef HFSM_ENABLE_LOG_INTERFACE
	static const char* fullName();

	template <typename>
	struct MemberTraits;

	template <typename TReturn, typename TState, typename... TArgs>
	struct MemberTraits<TReturn(TState::*)(TArgs...)> {
		using State = TState;
	};

	template <typename TMethodType, LoggerInterface::Method>
	typename std::enable_if< std::is_same<typename MemberTraits<TMethodType>::State, Base>::value>::type
	log(LoggerInterface&) const {}

	template <typename TMethodType, LoggerInterface::Method TMethodId>
	typename std::enable_if<!std::is_same<typename MemberTraits<TMethodType>::State, Base>::value>::type
	log(LoggerInterface& logger) const {
		logger.record(typeid(Head), fullName(), TMethodId, methodName(TMethodId));
	}
#endif

	Head _head;

	HSFM_IF_DEBUG(const TypeInfo _type = TypeInfo::get<Head>());
};

////////////////////////////////////////////////////////////////////////////////

}

namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, unsigned TMS>
template <typename TH>
M<TC, TMS>::_S<TH>::_S(StateRegistry& stateRegistry,
					   const Parent parent,
					   Parents& stateParents,
					   Parents& /*forkParents*/,
					   ForkPointers& /*forkPointers*/)
{
	const auto id = stateRegistry.add(TypeInfo::get<Head>());
	stateParents[id] = parent;
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH>
bool
M<TC, TMS>::_S<TH>::deepSubstitute(Control& control,
								   Context& context,
								   LoggerInterface* const HFSM_IF_LOGGER(logger))
{
	HFSM_IF_LOGGER(if (logger) log<decltype(&Head::substitute), LoggerInterface::Method::Substitute>(*logger));

	const unsigned requestCountBefore = control.requestCount();

	_head.widePreSubstitute(context);
	_head.substitute(control, context);

	return requestCountBefore < control.requestCount();
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH>
void
M<TC, TMS>::_S<TH>::deepEnterInitial(Context& context,
									 LoggerInterface* const logger)
{
	deepEnter(context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH>
void
M<TC, TMS>::_S<TH>::deepEnter(Context& context,
							  LoggerInterface* const HFSM_IF_LOGGER(logger))
{
	HFSM_IF_LOGGER(if (logger) log<decltype(&Head::enter), LoggerInterface::Method::Enter>(*logger));

	_head.widePreEnter(context);
	_head.enter(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH>
bool
M<TC, TMS>::_S<TH>::deepUpdateAndTransition(Control& control,
											Context& context,
											LoggerInterface* const HFSM_IF_LOGGER(logger))
{
	HFSM_IF_LOGGER(if (logger) log<decltype(&Head::update), LoggerInterface::Method::Update>(*logger));

	_head.widePreUpdate(context);
	_head.update(context);

	HFSM_IF_LOGGER(if (logger) log<decltype(&Head::transition), LoggerInterface::Method::Transition>(*logger));

	const unsigned requestCountBefore = control.requestCount();

	_head.widePreTransition(context);
	_head.transition(control, context);

	return requestCountBefore < control.requestCount();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH>
void
M<TC, TMS>::_S<TH>::deepUpdate(Context& context,
							   LoggerInterface* const HFSM_IF_LOGGER(logger))
{
	HFSM_IF_LOGGER(if (logger) log<decltype(&Head::update), LoggerInterface::Method::Update>(*logger));

	_head.widePreUpdate(context);
	_head.update(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH>
template <typename TEvent>
void
M<TC, TMS>::_S<TH>::deepReact(const TEvent& event,
							  Control& control,
							  Context& context,
							  LoggerInterface* const HFSM_IF_LOGGER(logger))
{
	HFSM_IF_LOGGER(if (logger) log<decltype(&Head::template react<TEvent>), LoggerInterface::Method::React>(*logger));

	_head.widePreReact(event, context);
	_head.react(event, control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH>
void
M<TC, TMS>::_S<TH>::deepLeave(Context& context,
							  LoggerInterface* const HFSM_IF_LOGGER(logger))
{
	HFSM_IF_LOGGER(if (logger) log<decltype(&Head::leave), LoggerInterface::Method::Leave>(*logger));

	_head.leave(context);
	_head.widePostLeave(context);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, unsigned TMS>
template <typename TH>
const char*
M<TC, TMS>::_S<TH>::name() {
	if (isBare())
		return "";
	else {
		const char* const raw = TypeInfo::get<Head>()->name();

		unsigned first =

		#if defined(_MSC_VER)
			raw[0] == 's' ? 7 : // Struct
			raw[0] == 'c' ? 6 : // Class
		#elif defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
			raw[0] ? 1 :
		#endif
			0;

		for (auto c = first; raw[c]; ++c)
			if (raw[c] == ':')
				first = c + 1;

		return raw + first;
	}
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH>
void
M<TC, TMS>::_S<TH>::deepGetNames(const unsigned parent,
								 const enum StateInfo::RegionType region,
								 const unsigned depth,
								 StateInfos& _stateInfos) const
{
	_stateInfos << StateInfo { parent, region, depth, name() };
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH>
void
M<TC, TMS>::_S<TH>::deepIsActive(const bool isActive,
								 unsigned& index,
								 MachineStructure& structure) const
{
	if (!isBare())
		structure[index++].isActive = isActive;
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_LOG_INTERFACE

template <typename TC, unsigned TMS>
template <typename TH>
const char*
M<TC, TMS>::_S<TH>::fullName() {
	if (isBare())
		return "";
	else {
		const char* const raw = TypeInfo::get<Head>()->name();

		unsigned first =

		#if defined(_MSC_VER)
			raw[0] == 's' ? 7 : // Struct
			raw[0] == 'c' ? 6 : // Class
		#elif defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
			raw[0] ? 1 :
		#endif
			0;

		return raw + first;
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext, unsigned TMaxSubstitutions>
template <typename TH, typename... TS>
struct M<TContext, TMaxSubstitutions>::_C final {
	using Head	= TH;
	using Fork	= ForkT<Head>;
	using State	= _S<Head>;

	//----------------------------------------------------------------------

	template <unsigned TN, typename...>
	struct Sub;

	//----------------------------------------------------------------------

	template <unsigned TN, typename TI, typename... TR>
	struct Sub<TN, TI, TR...> {
		using Initial = typename WrapState<TI>::Type;
		using Remaining = Sub<TN + 1, TR...>;

		enum : unsigned {
			ProngIndex	 = TN,
			ReverseDepth = detail::Max<Initial::ReverseDepth, Remaining::ReverseDepth>::Value,
			DeepWidth	 = detail::Max<Initial::DeepWidth, Remaining::DeepWidth>::Value,
			StateCount	 = Initial::StateCount + Remaining::StateCount,
			ForkCount	 = Initial::ForkCount  + Remaining::ForkCount,
			ProngCount	 = Initial::ProngCount + Remaining::ProngCount,
		};

		Sub(StateRegistry& stateRegistry,
			const Index fork,
			Parents& stateParents,
			Parents& forkParents,
			ForkPointers& forkPointers);

		inline void wideForwardSubstitute	(const unsigned prong, Control& control, Context& context, LoggerInterface* const logger);
		inline void wideSubstitute			(const unsigned prong, Control& control, Context& context, LoggerInterface* const logger);

		inline void wideEnterInitial		(										 Context& context, LoggerInterface* const logger);
		inline void wideEnter				(const unsigned prong,					 Context& context, LoggerInterface* const logger);

		inline bool wideUpdateAndTransition	(const unsigned prong, Control& control, Context& context, LoggerInterface* const logger);
		inline void wideUpdate				(const unsigned prong,					 Context& context, LoggerInterface* const logger);

		template <typename TEvent>
		inline void wideReact				(const unsigned prong,
											 const TEvent& event,  Control& control, Context& context, LoggerInterface* const logger);

		inline void wideLeave				(const unsigned prong,					 Context& context, LoggerInterface* const logger);

		inline void wideForwardRequest(const unsigned prong, const enum Transition::Type transition);
		inline void wideRequestRemain();
		inline void wideRequestRestart();
		inline void wideRequestResume(const unsigned prong);
		inline void wideChangeToRequested	(const unsigned prong,					 Context& context, LoggerInterface* const logger);

	#ifdef HFSM_ENABLE_STRUCTURE_REPORT
		enum : unsigned {
			NameCount	 = Initial::NameCount  + Remaining::NameCount,
		};

		void wideGetNames(const unsigned parent,
						  const unsigned depth,
						  StateInfos& stateInfos) const;

		void wideIsActive(const unsigned prong,
						  unsigned& index,
						  MachineStructure& structure) const;
	#endif

		Initial initial;
		Remaining remaining;
	};

	//----------------------------------------------------------------------

	template <unsigned TN, typename TI>
	struct Sub<TN, TI> {
		using Initial = typename WrapState<TI>::Type;

		enum : unsigned {
			ProngIndex	 = TN,
			ReverseDepth = Initial::ReverseDepth,
			DeepWidth	 = detail::Max<1, Initial::DeepWidth>::Value,
			StateCount	 = Initial::StateCount,
			ForkCount	 = Initial::ForkCount,
			ProngCount	 = Initial::ProngCount,
		};

		Sub(StateRegistry& stateRegistry,
			const Index fork,
			Parents& stateParents,
			Parents& forkParents,
			ForkPointers& forkPointers);

		inline void wideForwardSubstitute	(const unsigned prong, Control& control, Context& context, LoggerInterface* const logger);
		inline void wideSubstitute			(const unsigned prong, Control& control, Context& context, LoggerInterface* const logger);

		inline void wideEnterInitial		(										 Context& context, LoggerInterface* const logger);
		inline void wideEnter				(const unsigned prong,					 Context& context, LoggerInterface* const logger);

		inline bool wideUpdateAndTransition	(const unsigned prong, Control& control, Context& context, LoggerInterface* const logger);
		inline void wideUpdate				(const unsigned prong,					 Context& context, LoggerInterface* const logger);

		template <typename TEvent>
		inline void wideReact				(const unsigned prong, const TEvent& event,
																   Control& control, Context& context, LoggerInterface* const logger);

		inline void wideLeave				(const unsigned prong,					 Context& context, LoggerInterface* const logger);

		inline void wideForwardRequest(const unsigned prong, const enum Transition::Type transition);
		inline void wideRequestRemain();
		inline void wideRequestRestart();
		inline void wideRequestResume(const unsigned prong);
		inline void wideChangeToRequested	(const unsigned prong,					 Context& context, LoggerInterface* const logger);

	#ifdef HFSM_ENABLE_STRUCTURE_REPORT
		enum : unsigned {
			NameCount	 = Initial::NameCount,
		};

		void wideGetNames(const unsigned parent,
						  const unsigned depth,
						  StateInfos& stateInfos) const;

		void wideIsActive(const unsigned prong,
						  unsigned& index,
						  MachineStructure& structure) const;
	#endif

		Initial initial;
	};

	using SubStates = Sub<0, TS...>;

	//----------------------------------------------------------------------

	enum : unsigned {
		ReverseDepth = SubStates::ReverseDepth + 1,
		DeepWidth	 = SubStates::DeepWidth,
		StateCount	 = State::StateCount + SubStates::StateCount,
		ForkCount	 = SubStates::ForkCount + 1,
		ProngCount	 = SubStates::ProngCount + sizeof...(TS),
		Width		 = sizeof...(TS),
	};

	_C(StateRegistry& stateRegistry,
	   const Parent parent,
	   Parents& stateParents,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

	inline void deepForwardSubstitute	(Control& control, Context& context, LoggerInterface* const logger);
	inline void deepSubstitute			(Control& control, Context& context, LoggerInterface* const logger);

	inline void deepEnterInitial		(				   Context& context, LoggerInterface* const logger);
	inline void deepEnter				(				   Context& context, LoggerInterface* const logger);

	inline bool deepUpdateAndTransition	(Control& control, Context& context, LoggerInterface* const logger);
	inline void deepUpdate				(				   Context& context, LoggerInterface* const logger);

	template <typename TEvent>
	inline void deepReact				(const TEvent& event,
										 Control& control, Context& context, LoggerInterface* const logger);

	inline void deepLeave				(				   Context& context, LoggerInterface* const logger);

	inline void deepForwardRequest(const enum Transition::Type transition);
	inline void deepRequestRemain();
	inline void deepRequestRestart();
	inline void deepRequestResume();
	inline void deepChangeToRequested	(				   Context& context, LoggerInterface* const logger);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	enum : unsigned {
		NameCount	 = State::NameCount  + SubStates::NameCount,
	};

	void deepGetNames(const unsigned parent,
					  const enum StateInfo::RegionType region,
					  const unsigned depth,
					  StateInfos& stateInfos) const;

	void deepIsActive(const bool isActive,
					  unsigned& index,
					  MachineStructure& structure) const;
#endif
	Fork _fork;
	State _state;
	SubStates _subStates;

	HSFM_IF_DEBUG(const TypeInfo _type = TypeInfo::get<Head>());
};

////////////////////////////////////////////////////////////////////////////////

}

namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
M<TC, TMS>::_C<TH, TS...>::_C(StateRegistry& stateRegistry,
							  const Parent parent,
							  Parents& stateParents,
							  Parents& forkParents,
							  ForkPointers& forkPointers)
	: _fork(static_cast<Index>(forkPointers << &_fork), parent, forkParents)
	, _state(stateRegistry, parent, stateParents, forkParents, forkPointers)
	, _subStates(stateRegistry, _fork.self, stateParents, forkParents, forkPointers)
{}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepForwardSubstitute(Control& control,
												 Context& context,
												 LoggerInterface* const logger)
{
	assert(_fork.requested != INVALID_INDEX);

	if (_fork.requested == _fork.active)
		_subStates.wideForwardSubstitute(_fork.requested, control, context, logger);
	else
		_subStates.wideSubstitute		(_fork.requested, control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepSubstitute(Control& control,
										  Context& context,
										  LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.requested != INVALID_INDEX);

	if (!_state	  .deepSubstitute(				   control, context, logger))
		_subStates.wideSubstitute(_fork.requested, control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepEnterInitial(Context& context,
											LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX &&
		   _fork.requested == INVALID_INDEX);

	HSFM_IF_DEBUG(_fork.activeType = TypeInfo::get<typename SubStates::Initial::Head>());
	_fork.active = 0;

	_state	  .deepEnter	   (context, logger);
	_subStates.wideEnterInitial(context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepEnter(Context& context,
									 LoggerInterface* const logger)
{
	assert(_fork.active	   == INVALID_INDEX &&
		   _fork.requested != INVALID_INDEX);

	HSFM_IF_DEBUG(_fork.activeType = _fork.requestedType);
	_fork.active = _fork.requested;

	HSFM_IF_DEBUG(_fork.requestedType.clear());
	_fork.requested = INVALID_INDEX;

	_state	  .deepEnter(			   context, logger);
	_subStates.wideEnter(_fork.active, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
bool
M<TC, TMS>::_C<TH, TS...>::deepUpdateAndTransition(Control& control,
												   Context& context,
												   LoggerInterface* const logger)
{
	assert(_fork.active != INVALID_INDEX);

	if (_state.deepUpdateAndTransition(control, context, logger)) {
		_subStates.wideUpdate(_fork.active, context, logger);

		return true;
	} else
		return _subStates.wideUpdateAndTransition(_fork.active, control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepUpdate(Context& context,
									  LoggerInterface* const logger)
{
	assert(_fork.active != INVALID_INDEX);

	_state	  .deepUpdate(				context, logger);
	_subStates.wideUpdate(_fork.active, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
template <typename TEvent>
void
M<TC, TMS>::_C<TH, TS...>::deepReact(const TEvent& event,
									 Control& control,
									 Context& context,
									 LoggerInterface* const logger)
{
	assert(_fork.active != INVALID_INDEX);

	_state	  .deepReact(			   event, control, context, logger);
	_subStates.wideReact(_fork.active, event, control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepLeave(Context& context,
									 LoggerInterface* const logger)
{
	assert(_fork.active != INVALID_INDEX);

	_subStates.wideLeave(_fork.active, context, logger);
	_state	  .deepLeave(			   context, logger);

	HSFM_IF_DEBUG(_fork.resumableType = _fork.activeType);
	_fork.resumable = _fork.active;

	HSFM_IF_DEBUG(_fork.activeType.clear());
	_fork.active = INVALID_INDEX;
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepForwardRequest(const enum Transition::Type transition) {
	if (_fork.requested != INVALID_INDEX)
		_subStates.wideForwardRequest(_fork.requested, transition);
	else
		switch (transition) {
		case Transition::Remain:
			deepRequestRemain();
			break;

		case Transition::Restart:
			deepRequestRestart();
			break;

		case Transition::Resume:
			deepRequestResume();
			break;

		default:
			assert(false);
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepRequestRemain() {
	if (_fork.active == INVALID_INDEX) {
		HSFM_IF_DEBUG(_fork.requestedType = TypeInfo::get<typename SubStates::Initial::Head>());
		_fork.requested = 0;
	}

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepRequestRestart() {
	HSFM_IF_DEBUG(_fork.requestedType = TypeInfo::get<typename SubStates::Initial::Head>());
	_fork.requested = 0;

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepRequestResume() {
	if (_fork.resumable != INVALID_INDEX) {
		HSFM_IF_DEBUG(_fork.requestedType = _fork.resumableType);
		_fork.requested = _fork.resumable;
	} else {
		HSFM_IF_DEBUG(_fork.requestedType = TypeInfo::get<typename SubStates::Initial::Head>());
		_fork.requested = 0;
	}

	_subStates.wideRequestResume(_fork.requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepChangeToRequested(Context& context,
												 LoggerInterface* const logger)
{
	assert(_fork.active != INVALID_INDEX);

	if (_fork.requested == _fork.active)
		_subStates.wideChangeToRequested(_fork.requested, context, logger);
	else if (_fork.requested != INVALID_INDEX) {
		_subStates.wideLeave(_fork.active, context, logger);

		HSFM_IF_DEBUG(_fork.resumableType = _fork.activeType);
		_fork.resumable = _fork.active;

		HSFM_IF_DEBUG(_fork.activeType = _fork.requestedType);
		_fork.active = _fork.requested;

		HSFM_IF_DEBUG(_fork.requestedType.clear());
		_fork.requested = INVALID_INDEX;

		_subStates.wideEnter(_fork.active, context, logger);
	}
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepGetNames(const unsigned parent,
										const enum StateInfo::RegionType region,
										const unsigned depth,
										StateInfos& _stateInfos) const
{
	_state.deepGetNames(parent, region, depth, _stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, depth + 1, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepIsActive(const bool isActive,
										unsigned& index,
										MachineStructure& structure) const
{
	_state.deepIsActive(isActive, index, structure);
	_subStates.wideIsActive(isActive ? _fork.active : INVALID_INDEX, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::Sub(StateRegistry& stateRegistry,
												  const Index fork,
												  Parents& stateParents,
												  Parents& forkParents,
												  ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork,
					 ProngIndex
					 HSFM_IF_DEBUG(, TypeInfo::get<T>())
					 HSFM_IF_DEBUG(, TypeInfo::get<typename Initial::Head>())),
			  stateParents,
			  forkParents,
			  forkPointers)
	, remaining(stateRegistry, fork, stateParents, forkParents, forkPointers)
{}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideForwardSubstitute(const unsigned prong,
																	Control& control,
																	Context& context,
																	LoggerInterface* const logger)
{
	if (prong == ProngIndex)
		initial  .deepForwardSubstitute(	   control, context, logger);
	else
		remaining.wideForwardSubstitute(prong, control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideSubstitute(const unsigned prong,
															 Control& control,
															 Context& context,
															 LoggerInterface* const logger)
{
	if (prong == ProngIndex)
		initial  .deepSubstitute(		control, context, logger);
	else
		remaining.wideSubstitute(prong, control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideEnterInitial(Context& context,
															   LoggerInterface* const logger)
{
	initial.deepEnterInitial(context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideEnter(const unsigned prong,
														Context& context,
														LoggerInterface* const logger)
{
	if (prong == ProngIndex)
		initial  .deepEnter(	   context, logger);
	else
		remaining.wideEnter(prong, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
bool
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideUpdateAndTransition(const unsigned prong,
																	  Control& control,
																	  Context& context,
																	  LoggerInterface* const logger)
{
	return prong == ProngIndex ?
		initial  .deepUpdateAndTransition(		 control, context, logger) :
		remaining.wideUpdateAndTransition(prong, control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideUpdate(const unsigned prong,
														 Context& context,
														 LoggerInterface* const logger)
{
	if (prong == ProngIndex)
		initial  .deepUpdate(		context, logger);
	else
		remaining.wideUpdate(prong, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
template <typename TEvent>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideReact(const unsigned prong,
														const TEvent& event,
														Control& control,
														Context& context,
														LoggerInterface* const logger)
{
	if (prong == ProngIndex)
		initial  .deepReact(	   event, control, context, logger);
	else
		remaining.wideReact(prong, event, control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideLeave(const unsigned prong,
														Context& context,
														LoggerInterface* const logger)
{
	if (prong == ProngIndex)
		initial  .deepLeave(	   context, logger);
	else
		remaining.wideLeave(prong, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideForwardRequest(const unsigned prong,
																 const enum Transition::Type transition)
{
	if (prong == ProngIndex)
		initial	 .deepForwardRequest(		transition);
	else
		remaining.wideForwardRequest(prong, transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideRequestResume(const unsigned prong) {
	if (prong == ProngIndex)
		initial.deepRequestResume();
	else
		remaining.wideRequestResume(prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideChangeToRequested(const unsigned prong,
																	Context& context,
																	LoggerInterface* const logger)
{
	if (prong == ProngIndex)
		initial	 .deepChangeToRequested(	   context, logger);
	else
		remaining.wideChangeToRequested(prong, context, logger);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideGetNames(const unsigned parent,
														   const unsigned depth,
														   StateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StateInfo::Composite, depth, _stateInfos);
	remaining.wideGetNames(parent, depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideIsActive(const unsigned prong,
														   unsigned& index,
														   MachineStructure& structure) const
{
	initial.deepIsActive(prong == ProngIndex, index, structure);
	remaining.wideIsActive(prong, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::Sub(StateRegistry& stateRegistry,
										   const Index fork,
										   Parents& stateParents,
										   Parents& forkParents,
										   ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork,
					 ProngIndex
					 HSFM_IF_DEBUG(, TypeInfo::get<T>())
					 HSFM_IF_DEBUG(, TypeInfo::get<typename Initial::Head>())),
			  stateParents,
			  forkParents,
			  forkPointers)
{}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideForwardSubstitute(const unsigned HSFM_IF_ASSERT(prong),
															 Control& control,
															 Context& context,
															 LoggerInterface* const logger)
{
	assert(prong == ProngIndex);

	initial.deepForwardSubstitute(control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideSubstitute(const unsigned HSFM_IF_ASSERT(prong),
													  Control& control,
													  Context& context,
													  LoggerInterface* const logger)
{
	assert(prong == ProngIndex);

	initial.deepSubstitute(control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideEnterInitial(Context& context,
														LoggerInterface* const logger)
{
	initial.deepEnterInitial(context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideEnter(const unsigned HSFM_IF_ASSERT(prong),
												 Context& context,
												 LoggerInterface* const logger)
{
	assert(prong == ProngIndex);

	initial.deepEnter(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
bool
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideUpdateAndTransition(const unsigned HSFM_IF_ASSERT(prong),
															   Control& control,
															   Context& context,
															   LoggerInterface* const logger)
{
	assert(prong == ProngIndex);

	return initial.deepUpdateAndTransition(control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideUpdate(const unsigned HSFM_IF_ASSERT(prong),
												  Context& context,
												  LoggerInterface* const logger)
{
	assert(prong == ProngIndex);

	initial.deepUpdate(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
template <typename TEvent>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideReact(const unsigned HSFM_IF_ASSERT(prong),
												 const TEvent& event,
												 Control& control,
												 Context& context,
												 LoggerInterface* const logger)
{
	assert(prong == ProngIndex);

	initial.deepReact(event, control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideLeave(const unsigned HSFM_IF_ASSERT(prong),
												 Context& context,
												 LoggerInterface* const logger)
{
	assert(prong == ProngIndex);

	initial.deepLeave(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideForwardRequest(const unsigned HSFM_IF_ASSERT(prong),
														  const enum Transition::Type transition)
{
	assert(prong == ProngIndex);

	initial.deepForwardRequest(transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideRequestResume(const unsigned HSFM_IF_ASSERT(prong)) {
	assert(prong == ProngIndex);

	initial.deepRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideChangeToRequested(const unsigned HSFM_IF_ASSERT(prong),
															 Context& context,
															 LoggerInterface* const logger)
{
	assert(prong == ProngIndex);

	initial.deepChangeToRequested(context, logger);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideGetNames(const unsigned parent,
													const unsigned depth,
													StateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StateInfo::Composite, depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideIsActive(const unsigned prong,
													unsigned& index,
													MachineStructure& structure) const
{
	initial.deepIsActive(prong == ProngIndex, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext, unsigned TMaxSubstitutions>
template <typename TH, typename... TS>
struct M<TContext, TMaxSubstitutions>::_O final {
	using Head	= TH;
	using Fork	= ForkT<Head>;
	using State	= _S<Head>;

	//----------------------------------------------------------------------

	template <unsigned TN, typename...>
	struct Sub;

	//----------------------------------------------------------------------

	template <unsigned TN, typename TI, typename... TR>
	struct Sub<TN, TI, TR...> {
		using Initial = typename WrapState<TI>::Type;
		using Remaining = Sub<TN + 1, TR...>;

		enum : unsigned {
			ProngIndex	 = TN,
			ReverseDepth = detail::Max<Initial::ReverseDepth, Remaining::ReverseDepth>::Value,
			DeepWidth	 = Initial::DeepWidth  + Remaining::DeepWidth,
			StateCount	 = Initial::StateCount + Remaining::StateCount,
			ForkCount	 = Initial::ForkCount  + Remaining::ForkCount,
			ProngCount	 = Initial::ProngCount + Remaining::ProngCount,
		};

		Sub(StateRegistry& stateRegistry,
			const Index fork,
			Parents& stateParents,
			Parents& forkParents,
			ForkPointers& forkPointers);

		inline void wideForwardSubstitute	(const unsigned prong,
											 Control& control, Context& context, LoggerInterface* const logger);

		inline void wideForwardSubstitute	(Control& control, Context& context, LoggerInterface* const logger);
		inline void wideSubstitute			(Control& control, Context& context, LoggerInterface* const logger);

		inline void wideEnterInitial		(				   Context& context, LoggerInterface* const logger);
		inline void wideEnter				(				   Context& context, LoggerInterface* const logger);

		inline bool wideUpdateAndTransition	(Control& control, Context& context, LoggerInterface* const logger);
		inline void wideUpdate				(				   Context& context, LoggerInterface* const logger);

		template <typename TEvent>
		inline void wideReact				(const TEvent& event,
											 Control& control, Context& context, LoggerInterface* const logger);

		inline void wideLeave				(				   Context& context, LoggerInterface* const logger);

		inline void wideForwardRequest(const unsigned prong, const enum Transition::Type transition);
		inline void wideRequestRemain();
		inline void wideRequestRestart();
		inline void wideRequestResume();
		inline void wideChangeToRequested	(				   Context& context, LoggerInterface* const logger);

	#ifdef HFSM_ENABLE_STRUCTURE_REPORT
		enum : unsigned {
			NameCount	 = Initial::NameCount  + Remaining::NameCount,
		};

		void wideGetNames(const unsigned parent,
						  const unsigned depth,
						  StateInfos& stateInfos) const;

		void wideIsActive(const bool active,
						  unsigned& index,
						  MachineStructure& structure) const;
	#endif

		Initial initial;
		Remaining remaining;
	};

	//----------------------------------------------------------------------

	template <unsigned TN, typename TI>
	struct Sub<TN, TI> {
		using Initial = typename WrapState<TI>::Type;

		enum : unsigned {
			ProngIndex	 = TN,
			ReverseDepth = Initial::ReverseDepth,
			DeepWidth	 = Initial::DeepWidth,
			StateCount	 = Initial::StateCount,
			ForkCount	 = Initial::ForkCount,
			ProngCount	 = Initial::ProngCount,
		};

		Sub(StateRegistry& stateRegistry,
			const Index fork,
			Parents& stateParents,
			Parents& forkParents,
			ForkPointers& forkPointers);

		inline void wideForwardSubstitute	(const unsigned prong,
											 Control& control, Context& context, LoggerInterface* const logger);

		inline void wideForwardSubstitute	(Control& control, Context& context, LoggerInterface* const logger);
		inline void wideSubstitute			(Control& control, Context& context, LoggerInterface* const logger);

		inline void wideEnterInitial		(				   Context& context, LoggerInterface* const logger);
		inline void wideEnter				(				   Context& context, LoggerInterface* const logger);

		inline bool wideUpdateAndTransition	(Control& control, Context& context, LoggerInterface* const logger);
		inline void wideUpdate				(				   Context& context, LoggerInterface* const logger);

		template <typename TEvent>
		inline void wideReact				(const TEvent& event,
											 Control& control, Context& context, LoggerInterface* const logger);

		inline void wideLeave				(				   Context& context, LoggerInterface* const logger);

		inline void wideForwardRequest(const unsigned prong, const enum Transition::Type transition);
		inline void wideRequestRemain();
		inline void wideRequestRestart();
		inline void wideRequestResume();
		inline void wideChangeToRequested	(				   Context& context, LoggerInterface* const logger);

	#ifdef HFSM_ENABLE_STRUCTURE_REPORT
		enum : unsigned {
			NameCount	 = Initial::NameCount,
		};

		void wideGetNames(const unsigned parent,
						  const unsigned depth,
						  StateInfos& stateInfos) const;

		void wideIsActive(const bool active,
						  unsigned& index,
						  MachineStructure& structure) const;
	#endif

		Initial initial;
	};

	using SubStates = Sub<0, TS...>;

	//----------------------------------------------------------------------

	enum : unsigned {
		ReverseDepth = SubStates::ReverseDepth + 1,
		DeepWidth	 = SubStates::DeepWidth,
		StateCount	 = State::StateCount + SubStates::StateCount,
		ForkCount	 = SubStates::ForkCount + 1,
		ProngCount	 = SubStates::ProngCount,
		Width		 = sizeof...(TS),
	};

	_O(StateRegistry& stateRegistry,
	   const Parent parent,
	   Parents& stateParents,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

	inline void deepForwardSubstitute	(Control& control, Context& context, LoggerInterface* const logger);
	inline void deepSubstitute			(Control& control, Context& context, LoggerInterface* const logger);

	inline void deepEnterInitial		(				   Context& context, LoggerInterface* const logger);
	inline void deepEnter				(				   Context& context, LoggerInterface* const logger);

	inline bool deepUpdateAndTransition	(Control& control, Context& context, LoggerInterface* const logger);
	inline void deepUpdate				(				   Context& context, LoggerInterface* const logger);

	template <typename TEvent>
	inline void deepReact				(const TEvent& event,
										 Control& control, Context& context, LoggerInterface* const logger);

	inline void deepLeave				(				   Context& context, LoggerInterface* const logger);

	inline void deepForwardRequest(const enum Transition::Type transition);
	inline void deepRequestRemain();
	inline void deepRequestRestart();
	inline void deepRequestResume();
	inline void deepChangeToRequested	(				   Context& context, LoggerInterface* const logger);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	enum : unsigned {
		NameCount	 = State::NameCount  + SubStates::NameCount,
	};

	void deepGetNames(const unsigned parent,
					  const enum StateInfo::RegionType region,
					  const unsigned depth,
					  StateInfos& stateInfos) const;

	void deepIsActive(const bool isActive,
					  unsigned& index,
					  MachineStructure& structure) const;
#endif

	Fork _fork;
	State _state;
	SubStates _subStates;

	HSFM_IF_DEBUG(const TypeInfo _type = TypeInfo::get<Head>());
};

////////////////////////////////////////////////////////////////////////////////

}

namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
M<TC, TMS>::_O<TH, TS...>::_O(StateRegistry& stateRegistry,
							  const Parent parent,
							  Parents& stateParents,
							  Parents& forkParents,
							  ForkPointers& forkPointers)
	: _fork(static_cast<Index>(forkPointers << &_fork), parent, forkParents)
	, _state(stateRegistry, parent, stateParents, forkParents, forkPointers)
	, _subStates(stateRegistry, _fork.self, stateParents, forkParents, forkPointers)
{}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepForwardSubstitute(Control& control,
												 Context& context,
												 LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	if (_fork.requested != INVALID_INDEX)
		_subStates.wideForwardSubstitute(_fork.requested, control, context, logger);
	else
		_subStates.wideForwardSubstitute(				  control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepSubstitute(Control& control,
										  Context& context,
										  LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	if (!_state	  .deepSubstitute(control, context, logger))
		_subStates.wideSubstitute(control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepEnterInitial(Context& context,
											LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX &&
		   _fork.requested == INVALID_INDEX);

	_state	  .deepEnter	   (context, logger);
	_subStates.wideEnterInitial(context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepEnter(Context& context,
									 LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_state	  .deepEnter(context, logger);
	_subStates.wideEnter(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
bool
M<TC, TMS>::_O<TH, TS...>::deepUpdateAndTransition(Control& control,
												   Context& context,
												   LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	if (_state.deepUpdateAndTransition(control, context, logger)) {
		_subStates.wideUpdate(context, logger);

		return true;
	} else
		return _subStates.wideUpdateAndTransition(control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepUpdate(Context& context,
									  LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_state	  .deepUpdate(context, logger);
	_subStates.wideUpdate(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
template <typename TEvent>
void
M<TC, TMS>::_O<TH, TS...>::deepReact(const TEvent& event,
									 Control& control,
									 Context& context,
									 LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_state	  .deepReact(event, control, context, logger);
	_subStates.wideReact(event, control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepLeave(Context& context,
									 LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_subStates.wideLeave(context, logger);
	_state	  .deepLeave(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepForwardRequest(const enum Transition::Type transition) {
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	if (_fork.requested != INVALID_INDEX)
		_subStates.wideForwardRequest(_fork.requested, transition);
	else
		switch (transition) {
		case Transition::Remain:
			deepRequestRemain();
			break;

		case Transition::Restart:
			deepRequestRestart();
			break;

		case Transition::Resume:
			deepRequestResume();
			break;

		default:
			assert(false);
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepRequestRemain() {
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepRequestRestart() {
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepRequestResume() {
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_subStates.wideRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepChangeToRequested(Context& context,
												 LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_subStates.wideChangeToRequested(context, logger);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepGetNames(const unsigned parent,
										const enum StateInfo::RegionType region,
										const unsigned depth,
										StateInfos& _stateInfos) const
{
	_state.deepGetNames(parent, region, depth, _stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, depth + 1, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepIsActive(const bool isActive,
										unsigned& index,
										MachineStructure& structure) const
{
	_state.deepIsActive(isActive, index, structure);
	_subStates.wideIsActive(isActive, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::Sub(StateRegistry& stateRegistry,
											  const Index fork,
											  Parents& stateParents,
											  Parents& forkParents,
											  ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork,
					 ProngIndex
					 HSFM_IF_DEBUG(, TypeInfo::get<T>())
					 HSFM_IF_DEBUG(, TypeInfo::get<typename Initial::Head>())),
			  stateParents,
			  forkParents,
			  forkPointers)
	, remaining(stateRegistry, fork, stateParents, forkParents, forkPointers)
{}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideForwardSubstitute(const unsigned prong,
																	Control& control,
																	Context& context,
																	LoggerInterface* const logger)
{
	if (prong == ProngIndex)
		initial  .deepForwardSubstitute(	   control, context, logger);
	else
		remaining.wideForwardSubstitute(prong, control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideForwardSubstitute(Control& control,
																	Context& context,
																	LoggerInterface* const logger)
{
	initial	 .deepForwardSubstitute(control, context, logger);
	remaining.wideForwardSubstitute(control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideSubstitute(Control& control,
															 Context& context,
															 LoggerInterface* const logger)
{
	initial	 .deepSubstitute(control, context, logger);
	remaining.wideSubstitute(control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideEnterInitial(Context& context,
															   LoggerInterface* const logger)
{
	initial  .deepEnterInitial(context, logger);
	remaining.wideEnterInitial(context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideEnter(Context& context,
														LoggerInterface* const logger)
{
	initial  .deepEnter(context, logger);
	remaining.wideEnter(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
bool
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideUpdateAndTransition(Control& control,
																	  Context& context,
																	  LoggerInterface* const logger)
{
	return initial  .deepUpdateAndTransition(control, context, logger)
		|| remaining.wideUpdateAndTransition(control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideUpdate(Context& context,
														 LoggerInterface* const logger)
{
	initial  .deepUpdate(context, logger);
	remaining.wideUpdate(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
template <typename TEvent>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideReact(const TEvent& event,
														Control& control,
														Context& context,
														LoggerInterface* const logger)
{
	initial  .deepReact(event, control, context, logger);
	remaining.wideReact(event, control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideLeave(Context& context,
														LoggerInterface* const logger)
{
	initial	 .deepLeave(context, logger);
	remaining.wideLeave(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideForwardRequest(const unsigned prong,
																 const enum Transition::Type transition)
{
	if (prong == ProngIndex) {
		initial.deepForwardRequest(transition);
		remaining.wideForwardRequest(prong, Transition::Remain);
	} else {
		initial.deepForwardRequest(Transition::Remain);
		remaining.wideForwardRequest(prong, transition);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideRequestRemain() {
	initial.deepRequestRemain();
	remaining.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideRequestRestart() {
	initial.deepRequestRestart();
	remaining.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideRequestResume() {
	initial.deepRequestResume();
	remaining.wideRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideChangeToRequested(Context& context,
																	LoggerInterface* const logger)
{
	initial	 .deepChangeToRequested(context, logger);
	remaining.wideChangeToRequested(context, logger);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideGetNames(const unsigned parent,
														   const unsigned depth,
														   StateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StateInfo::Orthogonal, depth, _stateInfos);
	remaining.wideGetNames(parent, depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideIsActive(const bool isActive,
														   unsigned& index,
														   MachineStructure& structure) const
{
	initial.deepIsActive(isActive, index, structure);
	remaining.wideIsActive(isActive, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::Sub(StateRegistry& stateRegistry,
										   const Index fork,
										   Parents& stateParents,
										   Parents& forkParents,
										   ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork,
					 ProngIndex
					 HSFM_IF_DEBUG(, TypeInfo::get<T>())
					 HSFM_IF_DEBUG(, TypeInfo::get<typename Initial::Head>())),
			  stateParents,
			  forkParents,
			  forkPointers)
{}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideForwardSubstitute(const unsigned HSFM_IF_ASSERT(prong),
															 Control& control,
															 Context& context,
															 LoggerInterface* const logger)
{
	assert(prong == ProngIndex);

	initial.deepForwardSubstitute(control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideForwardSubstitute(Control& control,
															 Context& context,
															 LoggerInterface* const logger)
{
	initial.deepForwardSubstitute(control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideSubstitute(Control& control,
													  Context& context,
													  LoggerInterface* const logger)
{
	initial.deepSubstitute(control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideEnterInitial(Context& context,
														LoggerInterface* const logger)
{
	initial.deepEnterInitial(context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideEnter(Context& context,
												 LoggerInterface* const logger)
{
	initial.deepEnter(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
bool
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideUpdateAndTransition(Control& control,
															   Context& context,
															   LoggerInterface* const logger)
{
	return initial.deepUpdateAndTransition(control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideUpdate(Context& context,
												  LoggerInterface* const logger)
{
	initial.deepUpdate(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
template <typename TEvent>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideReact(const TEvent& event,
												 Control& control,
												 Context& context,
												 LoggerInterface* const logger)
{
	initial.deepReact(event, control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideLeave(Context& context,
												 LoggerInterface* const logger)
{
	initial.deepLeave(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideForwardRequest(const unsigned prong,
														  const enum Transition::Type transition)
{
	assert(prong <= ProngIndex);

	if (prong == ProngIndex)
		initial.deepForwardRequest(transition);
	else
		initial.deepForwardRequest(Transition::Remain);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideRequestResume() {
	initial.deepRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideChangeToRequested(Context& context,
															 LoggerInterface* const logger)
{
	initial.deepChangeToRequested(context, logger);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideGetNames(const unsigned parent,
													const unsigned depth,
													StateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StateInfo::Orthogonal, depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideIsActive(const bool isActive,
													unsigned& index,
													MachineStructure& structure) const
{
	initial.deepIsActive(isActive, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}

#undef HFSM_IF_STRUCTURE
#undef HFSM_IF_LOGGER
#undef HFSM_LOGGER_OR
