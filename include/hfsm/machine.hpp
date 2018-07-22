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
#include <stdint.h>
#include <string.h>

#include <typeindex>


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

using ShortIndex = uint8_t;
enum : ShortIndex { INVALID_SHORT_INDEX = UINT8_MAX  };

using LongIndex  = uint16_t;
enum : LongIndex  { INVALID_LONG_INDEX	= UINT16_MAX };

//------------------------------------------------------------------------------

namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
inline
void
fill(T& a, const char value) {
	memset(&a, (int) value, sizeof(a));
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, unsigned NCount>
inline
unsigned
count(const T(&)[NCount]) {
	return NCount;
}

//------------------------------------------------------------------------------

template <typename T, unsigned NCapacity>
inline
const T*
end(const T(& a)[NCapacity]) {
	return &a[NCapacity];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TReturn, typename T, unsigned NCapacity>
inline
const TReturn*
end(const T(& a)[NCapacity]) {
	return reinterpret_cast<const TReturn*>(&a[NCapacity]);
}

////////////////////////////////////////////////////////////////////////////////

template <int N1, int N2>
struct Min {
	enum {
		VALUE = N1 < N2 ? N1 : N2
	};
};

//------------------------------------------------------------------------------

template <int N1, int N2>
struct Max {
	enum {
		VALUE = N1 > N2 ? N1 : N2
	};
};

//------------------------------------------------------------------------------

template <unsigned NCapacity>
struct UnsignedIndex {
	enum : LongIndex { CAPACITY = NCapacity };

	using Type = typename std::conditional<CAPACITY <= UINT8_MAX,  uint8_t,
				 typename std::conditional<CAPACITY <= UINT16_MAX, uint16_t,
				 typename std::conditional<CAPACITY <= UINT32_MAX, uint32_t,
																   uint64_t>::type>::type>::type;

	static_assert(CAPACITY <= UINT64_MAX, "STATIC ASSERT");
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

	template <typename T, LongIndex NCapacity>
	friend class Array;

private:
	inline Iterator(Container& container, const LongIndex cursor)
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

	LongIndex _cursor;
};

//------------------------------------------------------------------------------

template <typename TContainer>
class Iterator<const TContainer> {
public:
	using Container = TContainer;
	using Item = typename Container::Item;

	template <typename T, LongIndex NCapacity>
	friend class Array;

private:
	inline Iterator(const Container& container, const LongIndex cursor)
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

	LongIndex _cursor;
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

#pragma pack(push, 2)

template <typename T>
class ArrayView {
public:
	using Item = T;

	template <typename>
	friend class Iterator;

	enum {
		ITEM_ALIGNMENT	= alignof(Item[2]),
		VIEW_SIZE		= 4,
		OFFSET			= (VIEW_SIZE + ITEM_ALIGNMENT - 1) / ITEM_ALIGNMENT * ITEM_ALIGNMENT,
	};

protected:
	ArrayView(const LongIndex capacity);
	~ArrayView();

public:
	inline void clear()												{ _count = 0;			}

	inline LongIndex resize(const LongIndex count);

	template <typename TValue>
	inline LongIndex operator << (TValue&& value);

	inline		 auto& operator[] (const LongIndex i)				{ return get(i);		}
	inline const auto& operator[] (const LongIndex i) const			{ return get(i);		}

	inline LongIndex count() const									{ return _count;		}
	inline LongIndex capacity() const								{ return _capacity;		}

protected:
	inline LongIndex first() const									{ return 0;				}
	inline LongIndex limit() const									{ return _count;		}

	inline LongIndex prev(const LongIndex i) const					{ return i - 1;			}
	inline LongIndex next(const LongIndex i) const					{ return i + 1;			}

	inline		 Item& get(const LongIndex i);
	inline const Item& get(const LongIndex i) const;

private:
	// hacks
	inline		 auto* data()		{ return reinterpret_cast<		Item*>(((uintptr_t)this) + OFFSET);	}
	inline const auto* data() const	{ return reinterpret_cast<const Item*>(((uintptr_t)this) + OFFSET);	}

protected:
	LongIndex _count = 0;
	const LongIndex _capacity;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
ArrayView<T>::ArrayView(const LongIndex capacity)
	: _capacity(capacity)
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
ArrayView<T>::~ArrayView() {
	if (_count > 0)
		for (LongIndex i = _count - 1; i < _count; --i)
			get(i).~Item();
}

//------------------------------------------------------------------------------

template <typename T>
LongIndex
ArrayView<T>::resize(const LongIndex count) {
	const auto clampedCount = count < _capacity ?
		count : _capacity;

	if (clampedCount > _count) {
		for (LongIndex i = _count; i < clampedCount; ++i)
			get(i) = Item();
	}
	else if (clampedCount < _count) {
		for (LongIndex i = _count - 1; i >= clampedCount; --i)
			get(i).~Item();
	}

	return _count = clampedCount;
}

//------------------------------------------------------------------------------

template <typename T>
template <typename TValue>
LongIndex
ArrayView<T>::operator << (TValue&& value) {
	assert(_count < _capacity);

	new (&get(_count)) Item(std::move(value));

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T>
T&
ArrayView<T>::get(const LongIndex i) {
	assert(i < _capacity);

	return data()[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
const T&
ArrayView<T>::get(const LongIndex i) const {
	assert(i < _capacity);

	return data()[i];
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, LongIndex NCapacity>
class StaticArray {
public:
	enum : LongIndex {
		CAPACITY = NCapacity,
	};

	using Item  = T;
	using Index = typename UnsignedIndex<CAPACITY>::Type;

public:
	inline StaticArray() = default;

	inline		 Item& operator[] (const LongIndex i);
	inline const Item& operator[] (const LongIndex i) const;

	inline LongIndex count() const					{ return CAPACITY; }

private:
	Item _items[CAPACITY];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
struct StaticArray<T, 0> {};

//------------------------------------------------------------------------------

template <typename T, LongIndex NCapacity>
class Array
	: public ArrayView<T>
{
public:
	enum : LongIndex {
		CAPACITY = NCapacity,
		INVALID	 = INVALID_LONG_INDEX,
		DUMMY	 = INVALID,
	};

	using View = ArrayView<T>;
	using Item = typename View::Item;

public:
	Array()
		: View(CAPACITY)
	{
		HSFM_IF_ASSERT(const auto* const view = &View::get(0));
		assert(view == _storage);
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

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, LongIndex NCapacity>
T&
StaticArray<T, NCapacity>::operator[] (const LongIndex i) {
	assert(0 <= i && i < CAPACITY);

	return _items[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, LongIndex NCapacity>
const T&
StaticArray<T, NCapacity>::operator[] (const LongIndex i) const {
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
public:
	using Item = T;
	using Unit = uint32_t;
	enum { UNIT_COUNT = sizeof(Item) / sizeof(Unit) };

	using Storage = Unit[UNIT_COUNT];

public:
	inline Wrap() = default;

	template <typename... Ts>
	inline void create(Ts&&... ts)			{ new (&get()) Item(std::forward<Ts>(ts)...);	}

	inline Wrap(const Item& item)				{ get() = item;								}
	inline Wrap(Item&& item)					{ get() = std::move(item);					}

	inline Wrap& operator = (const Item& item)	{ get() = item;				return *this;	}
	inline Wrap& operator = (Item&& item)		{ get() = std::move(item);	return *this;	}

	inline void clear()										{ fill(_storage, 0);			}

	inline		 T& operator *()							{ return  get();				}
	inline const T& operator *() const						{ return  get();				}

	inline		 T* operator->()							{ return &get();				}
	inline const T* operator->() const						{ return &get();				}

	inline bool operator == (const Wrap other) const		{ return get() == other.get();	}

private:
	inline		 T& get()			{ return *reinterpret_cast<		 T* const>(&_storage);	}
	inline const T& get() const		{ return *reinterpret_cast<const T* const>(&_storage);	}

private:
	Storage _storage = { 0 };
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

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TK, typename TV, LongIndex TC, typename TH>
HashTable<TK, TV, TC, TH>::Item::Item(const Hash hash, const Key key)
	: _keyWrap(key)
	, _hash(hash)
{}

//------------------------------------------------------------------------------

template <typename TK, typename TV, LongIndex TC, typename TH>
void
HashTable<TK, TV, TC, TH>::Item::swap(Item& other) {
	std::swap(_hash,    other._hash);
	std::swap(_keyWrap, other._keyWrap);
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, LongIndex TC, typename TH>
bool
HashTable<TK, TV, TC, TH>::Item::operator == (const Item& other) const {
	return _hash == other._hash &&
		   *_keyWrap == *other._keyWrap;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TK, typename TV, LongIndex TC, typename TH>
bool
HashTable<TK, TV, TC, TH>::insert(const Key key) {
	assert(_count < CAPACITY);

	if (_count < CAPACITY) {
		Item newcomer(hash(key), key);

		for (LongIndex i = index(newcomer.hash()), newcomerDistance = 0;
			 ;
			 i = index(i + 1), ++newcomerDistance)
		{
			Item& resident = _items[i];

			if (resident.occupied()) {
				const auto residentDistance = probeCount(i);

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

template <typename TK, typename TV, LongIndex TC, typename TH>
typename HashTable<TK, TV, TC, TH>::Value*
HashTable<TK, TV, TC, TH>::find(const Key key) {
	const auto index = locate(key);

	return index != INVALID ?
		_items[index].value() : nullptr;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TK, typename TV, LongIndex TC, typename TH>
const typename HashTable<TK, TV, TC, TH>::Value*
HashTable<TK, TV, TC, TH>::find(const Key key) const {
	const auto index = locate(key);

	return index != INVALID ?
		_items[index].value() : nullptr;
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, LongIndex TC, typename TH>
LongIndex
HashTable<TK, TV, TC, TH>::locate(const Key key) const {
	const auto keyHash = hash(key);

	for (LongIndex i = index(keyHash), distance = 0;
		 ;
		 i = index(i + 1), ++distance)
	{
		if (_items[i].key() == key)
			return i;
		else if (distance > CAPACITY)
			return INVALID;
	}
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, LongIndex TC, typename TH>
LongIndex
HashTable<TK, TV, TC, TH>::probeCount(const LongIndex i) const {
	assert(i < CAPACITY);

	return index(CAPACITY + i - index(_items[i].hash()));
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, LongIndex TC, typename TH>
LongIndex
HashTable<TK, TV, TC, TH>::skipVacantForward(const LongIndex i) const {
	assert(0 <= i && i <= CAPACITY);

	if (i < CAPACITY) {
		LongIndex n = i;
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

// TODO: check where TypeInfo is passed instead of std::type_index

class alignas(sizeof(std::type_index)) TypeInfo
	: public Wrap<std::type_index>
{
	using Base = Wrap<std::type_index>;

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

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

using TypeListIndex = unsigned char;

enum : TypeListIndex { INVALID_TYPE_LIST_INDEX = (TypeListIndex) -1 };

//------------------------------------------------------------------------------

template <TypeListIndex, typename...>
struct TypeListBuilder;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <TypeListIndex NIndex, typename TFirst, typename... TRest>
struct TypeListBuilder<NIndex, TFirst, TRest...>
	: TypeListBuilder<NIndex + 1, TRest...>
{
	enum : TypeListIndex { INDEX = NIndex };

	static_assert(INDEX < (1 << 8 * sizeof(TypeListIndex)), "Too many types");

	using Type = TFirst;
	using Base = TypeListBuilder<INDEX + 1, TRest...>;

	template <typename T>
	static constexpr TypeListIndex index() {
		return std::is_same<T, Type>::value ? INDEX : Base::template index<T>();
	}

	template <typename TCheck>
	struct Index {
		using Check = TCheck;
		enum : TypeListIndex {
			VALUE = std::is_same<Check, Type>::value ? INDEX : Base::template Index<Check>::VALUE
		};
	};
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <TypeListIndex NIndex, typename TFirst>
struct TypeListBuilder<NIndex, TFirst> {
	enum : TypeListIndex { INDEX = NIndex };

	static_assert(NIndex < (1 << 8 * sizeof(TypeListIndex)), "Too many types");

	using Type = TFirst;

	template <typename T>
	static constexpr auto index() {
		return std::is_same<T, Type>::value ? INDEX : INVALID_TYPE_LIST_INDEX;
	}

	template <typename TCheck>
	struct Index {
		using Check = TCheck;
		enum : TypeListIndex {
			VALUE = std::is_same<Check, Type>::value ? INDEX : INVALID_TYPE_LIST_INDEX
		};
	};
};

////////////////////////////////////////////////////////////////////////////////

template <typename...>
class VariantT;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename... Ts>
struct TypeListT
	: TypeListBuilder<0, Ts...>
{
	using Base = TypeListBuilder<0, Ts...>;
	using Container = VariantT<Ts...>;

	using IndexType = typename UnsignedIndex<sizeof...(Ts)>::Type;

	enum : IndexType {
		SIZE = sizeof...(Ts),
		INVALID_INDEX = (IndexType) - 1,
	};

	template <typename TCheck>
	struct Index {
		using Check = TCheck;
		enum : IndexType {
			VALUE = Base::template Index<Check>::VALUE
		};
	};

	template <typename T>
	static constexpr bool contains() {
		return Base::template index<T>() != INVALID_TYPE_LIST_INDEX;
	}

	template <typename TCheck>
	struct Contains {
		using Check = TCheck;
		enum : bool {
			VALUE = Base::template Index<Check>::VALUE != INVALID_TYPE_LIST_INDEX
		};
	};
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <>
struct TypeListT<>
	: TypeListT<void>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


template <typename...>
struct MergeT;

template <typename... Ts1, typename... Ts2>
struct MergeT<TypeListT<Ts1...>, TypeListT<Ts2...>> {
	using TypeList = TypeListT<Ts1..., Ts2...>;
};

//------------------------------------------------------------------------------

#pragma pack(push, 1)

template <typename... Ts>
class VariantT {
	using Types = TypeListT<Ts...>;
	using IndexType = typename Types::IndexType;

	enum : IndexType { INVALID_INDEX = Types::INVALID_INDEX };

public:
	inline VariantT() = default;

	template <typename T, typename = typename std::enable_if<Types::template Contains<T>::VALUE>::type>
	inline VariantT(T* const p)
		: _pointer(p)
		, _index(Types::template Index<T>::VALUE)
	{
		assert((((uintptr_t) this) & 0x7) == 0);
		assert(_index != INVALID_INDEX);
	}

	inline explicit operator bool() const { return _index != INVALID_INDEX; }

	inline void reset() {
		_pointer = nullptr;
		_index = INVALID_INDEX;
	}

	template <typename T>
	inline typename std::enable_if<Types::template Contains<T>::VALUE, T>::type*
	get() {
		const auto INDEX = Types::template Index<T>::VALUE;

		assert(INDEX == _index);

		return INDEX == _index ?
			reinterpret_cast<T*>(_pointer) : nullptr;
	}

private:
	alignas(alignof(void*)) void* _pointer = nullptr;
	IndexType _index = INVALID_INDEX;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}


#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_ENABLE_STRUCTURE_REPORT

////////////////////////////////////////////////////////////////////////////////

namespace hfsm {

enum class Method : ShortIndex {
	GUARD,
	ENTER,
	UPDATE,
	REACT,
	EXIT,

	COUNT
};

enum class Transition : ShortIndex {
	RESTART,
	RESUME,
	SCHEDULE,

	COUNT
};

//------------------------------------------------------------------------------

static inline
const char*
stateName(const std::type_index stateType) {
	const char* const raw = stateType.name();

	#if defined(_MSC_VER)

		auto first =
			raw[0] == 's' ? 7 : // Struct
			raw[0] == 'c' ? 6 : // Class
							0;
		return raw + first;

	#elif defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)

		return raw;

	#else

		return raw;

	#endif
}

//------------------------------------------------------------------------------

static inline
const char*
methodName(const ::hfsm::Method method) {
	switch (method) {
		case Method::GUARD:		return "guard";
		case Method::ENTER:		return "enter";
		case Method::UPDATE:	return "update";
		case Method::REACT:		return "react";
		case Method::EXIT:		return "exit";

		default:
			HSFM_BREAK();
			return nullptr;
	}
}

//------------------------------------------------------------------------------

static inline
const char*
transitionName(const ::hfsm::Transition transition) {
	switch (transition) {
		case Transition::RESTART:		return "changeTo";
		case Transition::RESUME:		return "resume";
		case Transition::SCHEDULE:		return "schedule";

		default:
			HSFM_BREAK();
			return nullptr;
	}
}

////////////////////////////////////////////////////////////////////////////////

}

#endif


#ifdef HFSM_ENABLE_LOG_INTERFACE

namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

struct LoggerInterface {
	using Method	 = ::hfsm::Method;
	using Transition = ::hfsm::Transition;

	virtual
	void
	recordMethod(const std::type_index state,
				 const Method method) = 0;

	virtual
	void
	recordTransition(const Transition transition,
					 const std::type_index state) = 0;
};

////////////////////////////////////////////////////////////////////////////////

}

#else

namespace hfsm {

using LoggerInterface = void;

}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_LOG_INTERFACE
	#define HFSM_IF_LOGGER(...)		__VA_ARGS__
	#define HFSM_LOGGER_OR(y, n)	y
#else
	#define HFSM_IF_LOGGER(...)
	#define HFSM_LOGGER_OR(y, n)	n
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	#define HFSM_IF_STRUCTURE(...)	__VA_ARGS__
#else
	#define HFSM_IF_STRUCTURE(...)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_ALIGNMENT_CHEKS
	#define HFSM_IF_ALIGNMENT_CHEKS(...)	__VA_ARGS__
#else
	#define HFSM_IF_ALIGNMENT_CHEKS(...)
#endif

//------------------------------------------------------------------------------


namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename, typename, typename>
struct _S;

template <typename, typename, typename, typename...>
struct _C;

template <typename, typename, typename, typename...>
struct _O;

template <typename, typename, ShortIndex, typename>
class _R;

//------------------------------------------------------------------------------

#pragma pack(push, 1)

struct Parent {
	HSFM_IF_DEBUG(TypeInfo forkType);
	HSFM_IF_DEBUG(TypeInfo prongType);

	ShortIndex fork  = INVALID_SHORT_INDEX;
	ShortIndex prong = INVALID_SHORT_INDEX;

	inline Parent() = default;

	inline Parent(const ShortIndex fork_,
				  const ShortIndex prong_,
				  const TypeInfo HSFM_IF_DEBUG(forkType_),
				  const TypeInfo HSFM_IF_DEBUG(prongType_))
		: HSFM_IF_DEBUG(forkType(forkType_),)
		  HSFM_IF_DEBUG(prongType(prongType_),)
		  fork(fork_)
		, prong(prong_)
	{
		assert((((uintptr_t) this) & 0x1) == 0);
	}

	inline explicit operator bool() const {
		return fork  != INVALID_SHORT_INDEX &&
			   prong != INVALID_SHORT_INDEX;
	}
};

#pragma pack(pop)

using Parents = ArrayView<Parent>;

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 2)

template <typename TPayloadList>
struct StateInfoT {
	using PayloadList = TPayloadList;
	using Payload = typename PayloadList::Container;

	Payload payload;
	Parent parent;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

template <typename TPayloadList>
struct StateRegistryBaseT {
	using PayloadList = TPayloadList;
	using StateInfo = StateInfoT<PayloadList>;

public:
	virtual void insert(const TypeInfo type) = 0;
	virtual StateInfo& get(const TypeInfo type) = 0;
};

//------------------------------------------------------------------------------

template <typename TPayloadList, LongIndex NCapacity>
class StateRegistryT
	: public StateRegistryBaseT<TPayloadList>
{
	using PayloadList = TPayloadList;
	using StateInfo = StateInfoT<PayloadList>;

	enum : LongIndex { CAPACITY = NCapacity };

	using Table = HashTable<std::type_index, StateInfo, CAPACITY>;

public:
	inline		 StateInfo& operator[] (const TypeInfo type);
	inline const StateInfo& operator[] (const TypeInfo type) const;

	// StateRegistryBase
	virtual void insert(const TypeInfo type) override;
	virtual StateInfo& get(const TypeInfo type) override	{ return operator[](type);	}

private:
	Table _table;
};

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct Fork {
	HSFM_IF_DEBUG (const TypeInfo type);
	HSFM_IF_ASSERT(TypeInfo activeType);
	HSFM_IF_ASSERT(TypeInfo resumableType);
	HSFM_IF_ASSERT(TypeInfo requestedType);

	ShortIndex self		 = INVALID_SHORT_INDEX;
	ShortIndex active	 = INVALID_SHORT_INDEX;
	ShortIndex resumable = INVALID_SHORT_INDEX;
	ShortIndex requested = INVALID_SHORT_INDEX;

	inline Fork(const ShortIndex self_,
				const TypeInfo HSFM_IF_DEBUG(type_))
		: HSFM_IF_DEBUG(type(type_),)
		  self(self_)
	{
		assert((((uintptr_t) this) & 0x3) == 0);
	}
};

#pragma pack(pop)

using ForkPointers = ArrayView<Fork*>;

//------------------------------------------------------------------------------

template <typename T>
struct ForkT
	: Fork
{
	ForkT(const ShortIndex index,
		  const Parent parent,
		  Parents& forkParents)
		: Fork(index, TypeInfo::get<T>())
	{
		forkParents[index] = parent;
	}
};

////////////////////////////////////////////////////////////////////////////////

template <typename TPayloadList>
struct TransitionT {
	using PayloadList = TPayloadList;
	using Payload = typename PayloadList::Container;

	enum Type {
		REMAIN,
		RESTART,
		RESUME,
		SCHEDULE,

		COUNT
	};

	template <typename TCheck>
	struct Contains {
		using Check = TCheck;
		enum : bool {
			VALUE = PayloadList::template Contains<Check>::VALUE
		};
	};

	inline TransitionT() = default;

	inline TransitionT(const Type type_,
					   const TypeInfo stateType_)
		: stateType(stateType_)
		, type(type_)
	{
		assert(type_ < Type::COUNT);
	}

	template <typename T, typename = typename std::enable_if<Contains<T>::VALUE, T>::type>
	inline TransitionT(const Type type_,
					   const TypeInfo stateType_,
					   T* const payload_)
		: stateType(stateType_)
		, payload(payload_)
		, type(type_)
	{
		assert(type_ < Type::COUNT);
	}

	TypeInfo stateType;
	Payload payload;
	Type type = RESTART;
};

template <typename TPayloadList>
using TransitionQueueT = ArrayView<TransitionT<TPayloadList>>;

////////////////////////////////////////////////////////////////////////////////

template <typename TContext>
class ControlT {
	template <typename, typename, typename>
	friend struct _S;

	template <typename, typename, typename, typename...>
	friend struct _C;

	template <typename, typename, typename, typename...>
	friend struct _O;

	template <typename, typename, ShortIndex, typename>
	friend class _R;

protected:
	using Context = TContext;

	inline ControlT(Context& context,
					LoggerInterface* const HFSM_IF_LOGGER(logger))
		: _context(context)
		HFSM_IF_LOGGER(, _logger(logger))
	{}

public:
	inline auto& _()									{ return _context;		}
	inline auto& context()								{ return _context;		}

private:
#ifdef HFSM_ENABLE_LOG_INTERFACE
	inline auto& logger()								{ return _logger;		}
#endif

protected:
	Context& _context;
	HFSM_IF_LOGGER(LoggerInterface* _logger);
};

//------------------------------------------------------------------------------

template <typename TContext, typename TPayloadList>
class TransitionControlT final
	: public ControlT<TContext>
{
	using Context = TContext;
	using Base = ControlT<Context>;
	using PayloadList = TPayloadList;
	using Transition = TransitionT<PayloadList>;
	using TransitionType = enum Transition::Type;
	using TransitionQueue = TransitionQueueT<TPayloadList>;

	template <typename, typename, ShortIndex, typename>
	friend class _R;

	template <typename, typename>
	friend struct ControlLockT;

private:
	inline TransitionControlT(Context& context,
							  TransitionQueue& requests,
							  LoggerInterface* const logger)
		: Base(context, logger)
		, _requests(requests)
	{}

public:
	inline void changeTo(const std::type_index state);
	inline void resume	(const std::type_index state);
	inline void schedule(const std::type_index state);

	template <typename TState>
	inline void changeTo()					{ changeTo(typeid(TState));			}

	template <typename TState>
	inline void resume()					{ resume  (typeid(TState));			}

	template <typename TState>
	inline void schedule()					{ schedule(typeid(TState));			}

	inline auto requestCount() const		{ return _requests.count();			}

private:
	TransitionQueue& _requests;
	bool _locked = false;
};

//------------------------------------------------------------------------------

template <typename TContext, typename TPayloadList>
struct ControlLockT {
	using TransitionControl = TransitionControlT<TContext, TPayloadList>;

	inline ControlLockT(TransitionControl& control);
	inline ~ControlLockT();

	TransitionControl* const _control;
};

////////////////////////////////////////////////////////////////////////////////

template <typename...>
struct WrapState;

template <typename TContext, typename TPayloadList, typename THead>
struct WrapState<TContext, TPayloadList, THead> {
	using Type = _S<TContext, TPayloadList, THead>;
};

template <typename TContext, typename TPayloadList, typename THead, typename... TSubStates>
struct WrapState<TContext, TPayloadList, _C<TContext, TPayloadList, THead, TSubStates...>> {
	using Type = _C<TContext, TPayloadList, THead, TSubStates...>;
};

template <typename TContext, typename TPayloadList, typename THead, typename... TSubStates>
struct WrapState<TContext, TPayloadList, _O<TContext, TPayloadList, THead, TSubStates...>> {
	using Type = _O<TContext, TPayloadList, THead, TSubStates...>;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TPL, LongIndex NC>
StateInfoT<TPL>&
StateRegistryT<TPL, NC>::operator[] (const TypeInfo type) {
	auto* const result = _table.find(*type);
	HSFM_CHECKED(result);

	return *result;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TPL, LongIndex NC>
const StateInfoT<TPL>&
StateRegistryT<TPL, NC>::operator[] (const TypeInfo type) const {
	const auto* const result = _table.find(*type);
	HSFM_CHECKED(result);

	return *result;
}

//------------------------------------------------------------------------------

template <typename TPL, LongIndex NC>
void
StateRegistryT<TPL, NC>::insert(const TypeInfo type) {
	HSFM_CHECKED(_table.insert(*type));
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TPL>
void
TransitionControlT<TC, TPL>::changeTo(const std::type_index state) {
	if (!_locked) {
		const Transition transition{TransitionType::RESTART, state};
		_requests << transition;

	#ifdef HFSM_ENABLE_LOG_INTERFACE
		if (Base::_logger)
			Base::_logger->recordTransition(::hfsm::Transition::RESTART, state);
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL>
void
TransitionControlT<TC, TPL>::resume(const std::type_index state) {
	if (!_locked) {
		const Transition transition{TransitionType::RESUME, state};
		_requests << transition;

	#ifdef HFSM_ENABLE_LOG_INTERFACE
		if (Base::_logger)
			Base::_logger->recordTransition(::hfsm::Transition::RESUME, state);
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL>
void
TransitionControlT<TC, TPL>::schedule(const std::type_index state) {
	if (!_locked) {
		const Transition transition{TransitionType::SCHEDULE, state};
		_requests << transition;

	#ifdef HFSM_ENABLE_LOG_INTERFACE
		if (Base::_logger)
			Base::_logger->recordTransition(::hfsm::Transition::SCHEDULE, state);
	#endif
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TPL>
ControlLockT<TC, TPL>::ControlLockT(TransitionControl& control)
	: _control(!control._locked ? &control : nullptr)
{
	if (_control)
		_control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL>
ControlLockT<TC, TPL>::~ControlLockT() {
	if (_control)
		_control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

}
}

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

namespace hfsm {

//------------------------------------------------------------------------------

struct StructureEntry {
	bool isActive;
	const wchar_t* prefix;
	const char* name;
};
using MachineStructure = detail::ArrayView<StructureEntry>;
using MachineActivity  = detail::ArrayView<char>;

//------------------------------------------------------------------------------

namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct StructureStateInfo {
	enum RegionType : ShortIndex {
		COMPOSITE,
		ORTHOGONAL,
	};

	StructureStateInfo() = default;

	inline StructureStateInfo(const LongIndex parent_,
							  const RegionType region_,
							  const ShortIndex depth_,
							  const char* const name_)
		: name(name_)
		, parent(parent_)
		, region(region_)
		, depth(depth_)
	{
		assert((((uintptr_t) this) & (sizeof(void*) - 1)) == 0);
	}

	alignas(alignof(char*)) const char* name;
	LongIndex parent;
	RegionType region;
	ShortIndex depth;
};

#pragma pack(pop)

using StructureStateInfos = ArrayView<StructureStateInfo>;

//------------------------------------------------------------------------------

template <typename TPayloadList>
::hfsm::Transition
inline get(const typename TransitionT<TPayloadList>::Type type) {
	using Transition = TransitionT<TPayloadList>;

	switch (type) {
		case Transition::RESTART:
			return ::hfsm::Transition::RESTART;
		case Transition::RESUME:
			return ::hfsm::Transition::RESUME;
		case Transition::SCHEDULE:
			return ::hfsm::Transition::SCHEDULE;
		default:
			assert(false);
			return ::hfsm::Transition::RESTART;
	}
}

#pragma pack(push, 2)

template <typename TPayloadList>
struct alignas(alignof(TypeInfo)) TransitionInfoT {
	using PayloadList = TPayloadList;
	using Transition = TransitionT<TPayloadList>;

	inline TransitionInfoT() = default;

	inline TransitionInfoT(const Transition transition_,
						   const ::hfsm::Method method_)
		: state{transition_.stateType}
		, method(method_)
		, transition(get<PayloadList>(transition_.type))
	{
		assert((((uintptr_t) this) & 0x7) == 0);
		assert(method_ < ::hfsm::Method::COUNT);
	}

	TypeInfo state;
	::hfsm::Method method;
	::hfsm::Transition transition;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename...>
struct _B;

//------------------------------------------------------------------------------

template <typename TContext, typename TPayloadList>
class Bare {
	template <typename...>
	friend struct _B;

protected:
	using Context = TContext;
	using Control = ControlT<Context>;
	using PayloadList = TPayloadList;
	using TransitionControl = TransitionControlT<Context, PayloadList>;

public:
	inline void preGuard (Context&)									{}
	inline void preEnter (Context&)									{}
	inline void preUpdate(Context&)									{}
	template <typename TEvent>
	inline void preReact (const TEvent&,
						  Context&)									{}
	inline void postExit (Context&)									{}
};

//------------------------------------------------------------------------------

template <typename TFirst, typename... TRest>
struct _B<TFirst, TRest...>
	: TFirst
	, _B<TRest...>
{
	using First	  = TFirst;

	inline void widePreGuard (typename First::Context& context);
	inline void widePreEnter (typename First::Context& context);
	inline void widePreUpdate(typename First::Context& context);
	template <typename TEvent>
	inline void widePreReact (const TEvent& event,
							  typename First::Context& context);
	inline void widePostExit (typename First::Context& context);
};

//------------------------------------------------------------------------------

template <typename TFirst>
struct _B<TFirst>
	: TFirst
{
	using First	  = TFirst;

	inline void guard		 (typename First::TransitionControl&)			{}
	inline void enter		 (typename First::Control&)						{}
	inline void update		 (typename First::TransitionControl&)			{}
	template <typename TEvent>
	inline void react		 (const TEvent&,
							  typename First::TransitionControl&)			{}
	inline void exit		 (typename First::Control&)						{}

	inline void widePreGuard (typename First::Context& context);
	inline void widePreEnter (typename First::Context& context);
	inline void widePreUpdate(typename First::Context& context);
	template <typename TEvent>
	inline void widePreReact (const TEvent& event,
							  typename First::Context& context);
	inline void widePostExit (typename First::Context& context);
};

template <typename TContext, typename TPayloadList>
using Base = _B<Bare<TContext, TPayloadList>>;

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TF, typename... TR>
void
_B<TF, TR...>::widePreGuard(typename TF::Context& context) {
	TF::preGuard(context);
	_B<TR...>::widePreGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
void
_B<TF, TR...>::widePreEnter(typename TF::Context& context) {
	TF::preEnter(context);
	_B<TR...>::widePreEnter(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
void
_B<TF, TR...>::widePreUpdate(typename TF::Context& context) {
	TF::preUpdate(context);
	_B<TR...>::widePreUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
template <typename TEvent>
void
_B<TF, TR...>::widePreReact(const TEvent& event,
							typename TF::Context& context)
{
	TF::preReact(event, context);
	_B<TR...>::widePreReact(event, context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
void
_B<TF, TR...>::widePostExit(typename TF::Context& context) {
	TF::postExit(context);
	_B<TR...>::widePostExit(context);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TF>
void
_B<TF>::widePreGuard(typename TF::Context& context) {
	TF::preGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF>
void
_B<TF>::widePreEnter(typename TF::Context& context) {
	TF::preEnter(context);
}

//------------------------------------------------------------------------------

template <typename TF>
void
_B<TF>::widePreUpdate(typename TF::Context& context) {
	TF::preUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TF>
template <typename TEvent>
void
_B<TF>::widePreReact(const TEvent& event,
					 typename TF::Context& context)
{
	TF::preReact(event, context);
}

//------------------------------------------------------------------------------

template <typename TF>
void
_B<TF>::widePostExit(typename TF::Context& context) {
	TF::postExit(context);
}

////////////////////////////////////////////////////////////////////////////////

}
}

//------------------------------------------------------------------------------

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TPayloadList = TypeListT<>,
		  ShortIndex TMaxSubstitutions = 4>
struct _M {
	using Context = TContext;
	using Control = ControlT<Context>;

	enum : ShortIndex {
		MAX_SUBSTITUTIONS = TMaxSubstitutions
	};

	using PayloadList = TPayloadList;
	//using Transition = TransitionT<PayloadList>;

	using TransitionControl = TransitionControlT<Context, PayloadList>;

	using TypeInfo = TypeInfo;

	using Bare = Bare<Context, PayloadList>;
	using Base = Base<Context, PayloadList>;

	template <typename... TInjections>
	using BaseT = _B<TInjections...>;

	//----------------------------------------------------------------------

	template <typename THead, typename... TSubStates>
	using Composite			 = _C<Context, PayloadList, THead, TSubStates...>;

	template <				  typename... TSubStates>
	using CompositePeers	 = _C<Context, PayloadList, Base,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using Orthogonal		 = _O<Context, PayloadList, THead, TSubStates...>;

	template <				  typename... TSubStates>
	using OrthogonalPeers	 = _O<Context, PayloadList, Base,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using Root				 = _R<Context, PayloadList, MAX_SUBSTITUTIONS, Composite< THead, TSubStates...>>;

	template <				  typename... TSubStates>
	using PeerRoot			 = _R<Context, PayloadList, MAX_SUBSTITUTIONS, CompositePeers<	 TSubStates...>>;

	template <typename THead, typename... TSubStates>
	using OrthogonalRoot	 = _R<Context, PayloadList, MAX_SUBSTITUTIONS, Orthogonal<THead, TSubStates...>>;

	template <				  typename... TSubStates>
	using OrthogonalPeerRoot = _R<Context, PayloadList, MAX_SUBSTITUTIONS, OrthogonalPeers<	 TSubStates...>>;

	//----------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////

}

//------------------------------------------------------------------------------

template <ShortIndex TMaxSubstitutions = 4>
struct Config {
	enum : ShortIndex { MAX_SUBSTITUTIONS = TMaxSubstitutions };
};

//------------------------------------------------------------------------------

template <typename... Ts>
using PayloadList = detail::TypeListT<Ts...>;

//------------------------------------------------------------------------------

template <typename...>
struct Machine;

template <typename TContext>
struct Machine<TContext>
	: detail::_M<TContext>
{};

template <typename TContext, typename TConfig>
struct Machine<TContext, TConfig>
	: detail::_M<TContext, PayloadList<>, TConfig::MAX_SUBSTITUTIONS>
{};

template <typename TContext, typename... TPayloads>
struct Machine<TContext, PayloadList<TPayloads...>>
	: detail::_M<TContext, PayloadList<TPayloads...>>
{};

template <typename TContext, typename TConfig, typename... TPayloads>
struct Machine<TContext, PayloadList<TPayloads...>, TConfig>
	: detail::_M<TContext, PayloadList<TPayloads...>, TConfig::MAX_SUBSTITUTIONS>
{};

////////////////////////////////////////////////////////////////////////////////

}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TPayloadList,
		  typename THead>
struct _S final {
	using Context			= TContext;
	using Control			= ControlT<Context>;
	using PayloadList		= TPayloadList;
	using StateRegistryBase	= StateRegistryBaseT<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionControl	= TransitionControlT<Context, PayloadList>;
	using Head				= THead;
	using StateList			= TypeListT<Head>;
	HSFM_IF_DEBUG(StateList stateList);

	using Base				= Base<Context, PayloadList>;

	enum : LongIndex {
		REVERSE_DEPTH = 1,
		DEEP_WIDTH	  = 0,
		STATE_COUNT	  = 1,
		FORK_COUNT	  = 0,
		PRONG_COUNT	  = 0,
		WIDTH		  = 1,
	};

	_S(StateRegistryBase& stateRegistry,
	   const Parent parent,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

	inline void deepRegister		 (StateRegistryBase& stateRegistry, const Parent parent);

	inline void deepForwardGuard	 (TransitionControl&)							{}
	inline bool deepGuard			 (TransitionControl& control);

	inline void deepEnterInitial	 (Control& control);
	inline void deepEnter			 (Control& control);

	inline bool deepUpdate			 (TransitionControl& control);

	template <typename TEvent>
	inline void deepReact			 (const TEvent& event, TransitionControl& control);

	inline void deepExit			 (Control& control);

	inline void deepForwardRequest	 (const enum Transition::Type)					{}
	inline void deepRequestRemain	 ()												{}
	inline void deepRequestRestart	 ()												{}
	inline void deepRequestResume	 ()												{}
	inline void deepChangeToRequested(Control&)										{}

#if defined HFSM_ENABLE_STRUCTURE_REPORT || defined HFSM_ENABLE_LOG_INTERFACE
	static constexpr bool isBare()	 { return std::is_same<Head, Base>::value;		 }

	enum : LongIndex {
		NAME_COUNT	 = isBare() ? 0 : 1,
	};
#endif

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static const char* name();

	void deepGetNames(const LongIndex parent,
					  const enum StructureStateInfo::RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;

	void deepIsActive(const bool isActive,
					  LongIndex& index,
					  MachineStructure& structure) const;
#endif

#ifdef HFSM_ENABLE_LOG_INTERFACE
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
		logger.recordMethod(typeid(Head), TMethodId);
	}
#endif

	Head _head;

	HSFM_IF_DEBUG(const std::type_index _type = typeid(Head));
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TPL, typename TH>
_S<TC, TPL, TH>::_S(StateRegistryBase& stateRegistry,
					const Parent /*parent*/,
					Parents& /*forkParents*/,
					ForkPointers& /*forkPointers*/)
{
	stateRegistry.insert(TypeInfo::get<Head>());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH>
void
_S<TC, TPL, TH>::deepRegister(StateRegistryBase& stateRegistry,
							  const Parent parent)
{
	auto& stateInfo = stateRegistry.get(TypeInfo::get<Head>());
	stateInfo.parent = parent;
	stateInfo.payload.reset();
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH>
bool
_S<TC, TPL, TH>::deepGuard(TransitionControl& control) {
	const auto requestCountBefore = control.requestCount();

	_head.widePreGuard(control.context());
	_head.guard(control);

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		log<decltype(&Head::guard), LoggerInterface::Method::GUARD>(*logger);
#endif

	return requestCountBefore < control.requestCount();
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH>
void
_S<TC, TPL, TH>::deepEnterInitial(Control& control) {
	deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH>
void
_S<TC, TPL, TH>::deepEnter(Control& control) {
	_head.widePreEnter(control.context());
	_head.enter(control);

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		log<decltype(&Head::enter), LoggerInterface::Method::ENTER>(*logger);
#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH>
bool
_S<TC, TPL, TH>::deepUpdate(TransitionControl& control)
{
	const auto requestCountBefore = control.requestCount();

	_head.widePreUpdate(control.context());
	_head.update(control);
	// TODO: _head.widePostUpdate(control.context());

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		log<decltype(&Head::update), LoggerInterface::Method::UPDATE>(*logger);
#endif

	return requestCountBefore < control.requestCount();
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH>
template <typename TEvent>
void
_S<TC, TPL, TH>::deepReact(const TEvent& event,
						   TransitionControl& control)
{
	_head.widePreReact(event, control.context());

	//_head.react(event, control);
	auto reaction = static_cast<void(Head::*)(const TEvent&, TransitionControl&)>(&Head::react);
	(_head.*reaction)(event, control);

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		log<decltype(reaction), LoggerInterface::Method::REACT>(*logger);
#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH>
void
_S<TC, TPL, TH>::deepExit(Control& control) {
	_head.exit(control);
	_head.widePostExit(control.context());

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		log<decltype(&Head::exit), LoggerInterface::Method::EXIT>(*logger);
#endif
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, typename TPL, typename TH>
const char*
_S<TC, TPL, TH>::name() {
	if (isBare())
		return "";
	else {
		const char* const raw = TypeInfo::get<Head>()->name();

	#if defined(_MSC_VER)

		auto first =
			raw[0] == 's' ? 7 : // Struct
			raw[0] == 'c' ? 6 : // Class
							0;

		for (auto c = first; raw[c]; ++c)
			if (raw[c] == ':')
				first = c + 1;

		return raw + first;

	#elif defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)

		return raw;

	#else

		return raw;

	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH>
void
_S<TC, TPL, TH>::deepGetNames(const LongIndex parent,
							  const enum StructureStateInfo::RegionType region,
							  const ShortIndex depth,
							  StructureStateInfos& _stateInfos) const
{
	_stateInfos << StructureStateInfo { parent, region, depth, name() };
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH>
void
_S<TC, TPL, TH>::deepIsActive(const bool isActive,
							  LongIndex& index,
							  MachineStructure& structure) const
{
	if (!isBare())
		structure[index++].isActive = isActive;
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename, typename, ShortIndex, typename...>
struct _CS;

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TPayloadList,
		  ShortIndex NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct _CS<TContext, TPayloadList, NIndex, TInitial, TRemaining...> {
	using Context			= TContext;
	using Control			= ControlT<Context>;
	using PayloadList		= TPayloadList;
	using StateRegistryBase	= StateRegistryBaseT<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= enum Transition::Type;
	using TransitionControl	= TransitionControlT<Context, PayloadList>;
	using Initial			= typename WrapState<Context, PayloadList, TInitial>::Type;
	using Remaining			= _CS<Context, PayloadList, NIndex + 1, TRemaining...>;
	using StateList			= typename MergeT<typename Initial::StateList, typename Remaining::StateList>::TypeList;
	HSFM_IF_DEBUG(StateList stateList);

	enum : LongIndex {
		PRONG_INDEX	  = NIndex,
		REVERSE_DEPTH = Max<Initial::REVERSE_DEPTH, Remaining::REVERSE_DEPTH>::VALUE,
		DEEP_WIDTH	  = Max<Initial::DEEP_WIDTH, Remaining::DEEP_WIDTH>::VALUE,
		STATE_COUNT	  = Initial::STATE_COUNT + Remaining::STATE_COUNT,
		FORK_COUNT	  = Initial::FORK_COUNT  + Remaining::FORK_COUNT,
		PRONG_COUNT	  = Initial::PRONG_COUNT + Remaining::PRONG_COUNT,
	};

	_CS(StateRegistryBase& stateRegistry,
		const ShortIndex fork,
		const TypeInfo parentInfo,
		Parents& forkParents,
		ForkPointers& forkPointers);

	void wideRegister(StateRegistryBase& stateRegistry,
					  const ShortIndex fork,
					  const TypeInfo parentInfo);

	inline void wideForwardGuard	 (const ShortIndex prong, TransitionControl& control);
	inline void wideGuard			 (const ShortIndex prong, TransitionControl& control);

	inline void wideEnterInitial	 (						  Control& control);
	inline void wideEnter			 (const ShortIndex prong, Control& control);

	inline bool wideUpdate			 (const ShortIndex prong, TransitionControl& control);

	template <typename TEvent>
	inline void wideReact			 (const ShortIndex prong, const TEvent& event, TransitionControl& control);

	inline void wideExit			 (const ShortIndex prong, Control& control);

	inline void wideForwardRequest	 (const ShortIndex prong, const TransitionType transition);
	inline void wideRequestRemain	 ();
	inline void wideRequestRestart	 ();
	inline void wideRequestResume	 (const ShortIndex prong);
	inline void wideChangeToRequested(const ShortIndex prong, Control& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	enum : LongIndex {
		NAME_COUNT	 = Initial::NAME_COUNT  + Remaining::NAME_COUNT,
	};

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;

	void wideIsActive(const ShortIndex prong,
					  LongIndex& index,
					  MachineStructure& structure) const;
#endif

	Initial initial;
	Remaining remaining;
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TPayloadList,
		  ShortIndex NIndex,
		  typename TInitial>
struct _CS<TContext, TPayloadList, NIndex, TInitial> {
	using Context			= TContext;
	using Control			= ControlT<Context>;
	using PayloadList		= TPayloadList;
	using StateRegistryBase	= StateRegistryBaseT<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= enum Transition::Type;
	using TransitionControl	= TransitionControlT<Context, PayloadList>;
	using Initial			= typename WrapState<Context, PayloadList, TInitial>::Type;
	using StateList			= typename Initial::StateList;
	HSFM_IF_DEBUG(StateList stateList);

	enum : LongIndex {
		PRONG_INDEX	  = NIndex,
		REVERSE_DEPTH = Initial::REVERSE_DEPTH,
		DEEP_WIDTH	  = Max<1, Initial::DEEP_WIDTH>::VALUE,
		STATE_COUNT	  = Initial::STATE_COUNT,
		FORK_COUNT	  = Initial::FORK_COUNT,
		PRONG_COUNT	  = Initial::PRONG_COUNT,
	};

	_CS(StateRegistryBase& stateRegistry,
		const ShortIndex fork,
		const TypeInfo parentInfo,
		Parents& forkParents,
		ForkPointers& forkPointers);

	void wideRegister(StateRegistryBase& stateRegistry,
					  const ShortIndex fork,
					  const TypeInfo parentInfo);

	inline void wideForwardGuard	 (const ShortIndex prong, TransitionControl& control);
	inline void wideGuard			 (const ShortIndex prong, TransitionControl& control);

	inline void wideEnterInitial	 (						  Control& control);
	inline void wideEnter			 (const ShortIndex prong, Control& control);

	inline bool wideUpdate			 (const ShortIndex prong, TransitionControl& control);

	template <typename TEvent>
	inline void wideReact			 (const ShortIndex prong, const TEvent& event, TransitionControl& control);

	inline void wideExit			 (const ShortIndex prong, Control& control);

	inline void wideForwardRequest	 (const ShortIndex prong, const TransitionType transition);
	inline void wideRequestRemain	 ();
	inline void wideRequestRestart	 ();
	inline void wideRequestResume	 (const ShortIndex prong);
	inline void wideChangeToRequested(const ShortIndex prong, Control& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	enum : LongIndex {
		NAME_COUNT	 = Initial::NAME_COUNT,
	};

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;

	void wideIsActive(const ShortIndex prong,
					  LongIndex& index,
					  MachineStructure& structure) const;
#endif

	Initial initial;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
_CS<TC, TPL, NI, TI, TR...>::_CS(StateRegistryBase& stateRegistry,
								 const ShortIndex fork,
								 const TypeInfo parentInfo,
								 Parents& forkParents,
								 ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork,
					 PRONG_INDEX,
					 parentInfo,
					 TypeInfo::get<typename Initial::Head>()),
			  forkParents,
			  forkPointers)
	, remaining(stateRegistry,
				fork,
				parentInfo,
				forkParents,
				forkPointers)
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideRegister(StateRegistryBase& stateRegistry,
										  const ShortIndex fork,
										  const TypeInfo parentInfo)
{
	initial	 .deepRegister(stateRegistry,
						   Parent(fork,
								  PRONG_INDEX,
								  parentInfo,
								  TypeInfo::get<typename Initial::Head>()));
	remaining.wideRegister(stateRegistry,
						   fork,
						   parentInfo);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideForwardGuard(const ShortIndex prong,
											  TransitionControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepForwardGuard(control);
	else
		remaining.wideForwardGuard(prong, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideGuard(const ShortIndex prong,
									   TransitionControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepGuard(control);
	else
		remaining.wideGuard(prong, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideEnterInitial(Control& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideEnter(const ShortIndex prong,
									   Control& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepEnter(control);
	else
		remaining.wideEnter(prong, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
bool
_CS<TC, TPL, NI, TI, TR...>::wideUpdate(const ShortIndex prong,
										TransitionControl& control)
{
	return prong == PRONG_INDEX ?
		initial  .deepUpdate(control) :
		remaining.wideUpdate(prong, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
template <typename TEvent>
void
_CS<TC, TPL, NI, TI, TR...>::wideReact(const ShortIndex prong,
									   const TEvent& event,
									   TransitionControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepReact(	   event, control);
	else
		remaining.wideReact(prong, event, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideExit(const ShortIndex prong,
									  Control& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepExit(control);
	else
		remaining.wideExit(prong, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideForwardRequest(const ShortIndex prong,
												const TransitionType transition)
{
	if (prong == PRONG_INDEX)
		initial	 .deepForwardRequest(		transition);
	else
		remaining.wideForwardRequest(prong, transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideRequestResume(const ShortIndex prong) {
	if (prong == PRONG_INDEX)
		initial	 .deepRequestResume();
	else
		remaining.wideRequestResume(prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideChangeToRequested(const ShortIndex prong,
												   Control& control)
{
	if (prong == PRONG_INDEX)
		initial	 .deepChangeToRequested(control);
	else
		remaining.wideChangeToRequested(prong, control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideGetNames(const LongIndex parent,
										  const ShortIndex depth,
										  StructureStateInfos& _stateInfos) const
{
	initial	 .deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
	remaining.wideGetNames(parent,								  depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideIsActive(const ShortIndex prong,
										  LongIndex& index,
										  MachineStructure& structure) const
{
	initial	 .deepIsActive(prong == PRONG_INDEX, index, structure);
	remaining.wideIsActive(prong,				 index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TPL, ShortIndex NI, typename TI>
_CS<TC, TPL, NI, TI>::_CS(StateRegistryBase& stateRegistry,
						  const ShortIndex fork,
						  const TypeInfo parentInfo,
						  Parents& forkParents,
						  ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork,
					 PRONG_INDEX,
					 parentInfo,
					 TypeInfo::get<typename Initial::Head>()),
			  forkParents,
			  forkPointers)
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideRegister(StateRegistryBase& stateRegistry,
								   const ShortIndex fork,
								   const TypeInfo parentInfo)
{
	initial.deepRegister(stateRegistry,
						 Parent(fork,
								PRONG_INDEX,
								parentInfo,
								TypeInfo::get<typename Initial::Head>()));
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideForwardGuard(const ShortIndex HSFM_IF_ASSERT(prong),
									   TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideGuard(const ShortIndex HSFM_IF_ASSERT(prong),
								TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepGuard(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideEnterInitial(Control& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideEnter(const ShortIndex HSFM_IF_ASSERT(prong),
								Control& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepEnter(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
bool
_CS<TC, TPL, NI, TI>::wideUpdate(const ShortIndex HSFM_IF_ASSERT(prong),
								 TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
template <typename TEvent>
void
_CS<TC, TPL, NI, TI>::wideReact(const ShortIndex HSFM_IF_ASSERT(prong),
								const TEvent& event,
								TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepReact(event, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideExit(const ShortIndex HSFM_IF_ASSERT(prong),
							   Control& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideForwardRequest(const ShortIndex HSFM_IF_ASSERT(prong),
										 const TransitionType transition)
{
	assert(prong == PRONG_INDEX);

	initial.deepForwardRequest(transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideRequestResume(const ShortIndex HSFM_IF_ASSERT(prong)) {
	assert(prong == PRONG_INDEX);

	initial.deepRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideChangeToRequested(const ShortIndex HSFM_IF_ASSERT(prong),
											Control& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideGetNames(const LongIndex parent,
								   const ShortIndex depth,
								   StructureStateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideIsActive(const ShortIndex prong,
								   LongIndex& index,
								   MachineStructure& structure) const
{
	initial.deepIsActive(prong == PRONG_INDEX, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TPayloadList,
		  typename THead,
		  typename... TSubStates>
struct _C final {
	using Context			= TContext;
	using Control			= ControlT<Context>;
	using PayloadList		= TPayloadList;
	using StateRegistryBase	= StateRegistryBaseT<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= enum Transition::Type;
	using TransitionControl	= TransitionControlT<Context, PayloadList>;
	using ControlLock		= ControlLockT<Context, PayloadList>;
	using Head				= THead;
	using Fork				= ForkT<Head>;
	using State				= _S<Context, PayloadList, Head>;
	using SubStates			= _CS<Context, PayloadList, 0, TSubStates...>;
	using StateList			= typename MergeT<typename State::StateList, typename SubStates::StateList>::TypeList;
	HSFM_IF_DEBUG(StateList stateList);

	enum : LongIndex {
		REVERSE_DEPTH = SubStates::REVERSE_DEPTH + 1,
		DEEP_WIDTH	  = SubStates::DEEP_WIDTH,
		STATE_COUNT	  = State::STATE_COUNT + SubStates::STATE_COUNT,
		FORK_COUNT	  = SubStates::FORK_COUNT + 1,
		PRONG_COUNT	  = SubStates::PRONG_COUNT + sizeof...(TSubStates),
		WIDTH		  = sizeof...(TSubStates),
	};

	_C(StateRegistryBase& stateRegistry,
	   const Parent parent,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

	inline void deepRegister		 (StateRegistryBase& stateRegistry, const Parent parent);

	inline void deepForwardGuard	 (TransitionControl& control);
	inline void deepGuard			 (TransitionControl& control);

	inline void deepEnterInitial	 (Control& control);
	inline void deepEnter			 (Control& control);

	inline bool deepUpdate			 (TransitionControl& control);

	template <typename TEvent>
	inline void deepReact			 (const TEvent& event, TransitionControl& control);

	inline void deepExit			 (Control& control);

	inline void deepForwardRequest	 (const TransitionType transition);
	inline void deepRequestRemain	 ();
	inline void deepRequestRestart	 ();
	inline void deepRequestResume	 ();
	inline void deepChangeToRequested(Control& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	enum : LongIndex {
		NAME_COUNT	 = State::NAME_COUNT  + SubStates::NAME_COUNT,
	};

	void deepGetNames(const LongIndex parent,
					  const enum StructureStateInfo::RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;

	void deepIsActive(const bool isActive,
					  LongIndex& index,
					  MachineStructure& structure) const;
#endif
	Fork _fork;
	State _state;
	SubStates _subStates;

	HSFM_IF_DEBUG(const std::type_index _type = typeid(Head));
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TPL, typename TH, typename... TS>
_C<TC, TPL, TH, TS...>::_C(StateRegistryBase& stateRegistry,
						   const Parent parent,
						   Parents& forkParents,
						   ForkPointers& forkPointers)
	: _fork(static_cast<ShortIndex>(forkPointers << &_fork),
			parent,
			forkParents)
	, _state(stateRegistry,
			 parent,
			 forkParents,
			 forkPointers)
	, _subStates(stateRegistry,
				 _fork.self,
				 TypeInfo::get<Head>(),
				 forkParents,
				 forkPointers)
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepRegister(StateRegistryBase& stateRegistry,
									 const Parent parent)
{
	_state	  .deepRegister(stateRegistry, parent);
	_subStates.wideRegister(stateRegistry,
							_fork.self,
							TypeInfo::get<Head>());
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepForwardGuard(TransitionControl& control) {
	assert(_fork.requested != INVALID_SHORT_INDEX);

	if (_fork.requested == _fork.active)
		_subStates.wideForwardGuard(_fork.requested, control);
	else
		_subStates.wideGuard	   (_fork.requested, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepGuard(TransitionControl& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.requested != INVALID_SHORT_INDEX);

	if (!_state	  .deepGuard(				  control))
		_subStates.wideGuard(_fork.requested, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepEnterInitial(Control& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX &&
		   _fork.requested == INVALID_SHORT_INDEX);

	HSFM_IF_DEBUG(_fork.activeType = TypeInfo::get<typename SubStates::Initial::Head>());
	_fork.active = 0;

	_state	  .deepEnter	   (control);
	_subStates.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepEnter(Control& control) {
	assert(_fork.active	   == INVALID_SHORT_INDEX &&
		   _fork.requested != INVALID_SHORT_INDEX);

	HSFM_IF_DEBUG(_fork.activeType = _fork.requestedType);
	_fork.active = _fork.requested;

	HSFM_IF_DEBUG(_fork.requestedType.clear());
	_fork.requested = INVALID_SHORT_INDEX;

	_state	  .deepEnter(control);
	_subStates.wideEnter(_fork.active, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH, typename... TS>
bool
_C<TC, TPL, TH, TS...>::deepUpdate(TransitionControl& control) {
	assert(_fork.active != INVALID_SHORT_INDEX);

	if (_state	  .deepUpdate(control)) {
		ControlLock lock(control);
		_subStates.wideUpdate(_fork.active, control);

		return true;
	} else
		return _subStates.wideUpdate(_fork.active, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH, typename... TS>
template <typename TEvent>
void
_C<TC, TPL, TH, TS...>::deepReact(const TEvent& event,
								  TransitionControl& control)
{
	assert(_fork.active != INVALID_SHORT_INDEX);

	_state	  .deepReact(			   event, control);
	_subStates.wideReact(_fork.active, event, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepExit(Control& control) {
	assert(_fork.active != INVALID_SHORT_INDEX);

	_subStates.wideExit(_fork.active, control);
	_state	  .deepExit(control);

	HSFM_IF_DEBUG(_fork.resumableType = _fork.activeType);
	_fork.resumable = _fork.active;

	HSFM_IF_DEBUG(_fork.activeType.clear());
	_fork.active = INVALID_SHORT_INDEX;
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepForwardRequest(const TransitionType transition) {
	if (_fork.requested != INVALID_SHORT_INDEX)
		_subStates.wideForwardRequest(_fork.requested, transition);
	else
		switch (transition) {
		case Transition::REMAIN:
			deepRequestRemain();
			break;

		case Transition::RESTART:
			deepRequestRestart();
			break;

		case Transition::RESUME:
			deepRequestResume();
			break;

		default:
			assert(false);
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepRequestRemain() {
	if (_fork.active == INVALID_SHORT_INDEX) {
		HSFM_IF_DEBUG(_fork.requestedType = TypeInfo::get<typename SubStates::Initial::Head>());
		_fork.requested = 0;
	}

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepRequestRestart() {
	HSFM_IF_DEBUG(_fork.requestedType = TypeInfo::get<typename SubStates::Initial::Head>());
	_fork.requested = 0;

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepRequestResume() {
	if (_fork.resumable != INVALID_SHORT_INDEX) {
		HSFM_IF_DEBUG(_fork.requestedType = _fork.resumableType);
		_fork.requested = _fork.resumable;
	} else {
		HSFM_IF_DEBUG(_fork.requestedType = TypeInfo::get<typename SubStates::Initial::Head>());
		_fork.requested = 0;
	}

	_subStates.wideRequestResume(_fork.requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepChangeToRequested(Control& control) {
	assert(_fork.active != INVALID_SHORT_INDEX);

	if (_fork.requested == _fork.active)
		_subStates.wideChangeToRequested(_fork.requested, control);
	else if (_fork.requested != INVALID_SHORT_INDEX) {
		_subStates.wideExit(_fork.active, control);

		HSFM_IF_DEBUG(_fork.resumableType = _fork.activeType);
		_fork.resumable = _fork.active;

		HSFM_IF_DEBUG(_fork.activeType = _fork.requestedType);
		_fork.active = _fork.requested;

		HSFM_IF_DEBUG(_fork.requestedType.clear());
		_fork.requested = INVALID_SHORT_INDEX;

		_subStates.wideEnter(_fork.active, control);
	}
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepGetNames(const LongIndex parent,
									 const enum StructureStateInfo::RegionType region,
									 const ShortIndex depth,
									 StructureStateInfos& _stateInfos) const
{
	_state	  .deepGetNames(parent, region,			 depth,		_stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, depth + 1, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepIsActive(const bool isActive,
									 LongIndex& index,
									 MachineStructure& structure) const
{
	_state	  .deepIsActive(isActive,									   index, structure);
	_subStates.wideIsActive(isActive ? _fork.active : INVALID_SHORT_INDEX, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename, typename, ShortIndex, typename...>
struct _OS;

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TPayloadList,
		  ShortIndex NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct _OS<TContext, TPayloadList, NIndex, TInitial, TRemaining...> {
	using Context			= TContext;
	using Control			= ControlT<Context>;
	using PayloadList		= TPayloadList;
	using StateRegistryBase	= StateRegistryBaseT<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= enum Transition::Type;
	using TransitionControl	= TransitionControlT<Context, PayloadList>;
	using Initial			= typename WrapState<Context, PayloadList, TInitial>::Type;
	using Remaining			= _OS<Context, PayloadList, NIndex + 1, TRemaining...>;
	using StateList			= typename MergeT<typename Initial::StateList, typename Remaining::StateList>::TypeList;
	HSFM_IF_DEBUG(StateList stateList);

	enum : LongIndex {
		PRONG_INDEX	  = NIndex,
		REVERSE_DEPTH = Max<Initial::REVERSE_DEPTH, Remaining::REVERSE_DEPTH>::VALUE,
		DEEP_WIDTH	  = Initial::DEEP_WIDTH  + Remaining::DEEP_WIDTH,
		STATE_COUNT	  = Initial::STATE_COUNT + Remaining::STATE_COUNT,
		FORK_COUNT	  = Initial::FORK_COUNT  + Remaining::FORK_COUNT,
		PRONG_COUNT	  = Initial::PRONG_COUNT + Remaining::PRONG_COUNT,
	};

	_OS(StateRegistryBase& stateRegistry,
		const ShortIndex fork,
		const TypeInfo parentInfo,
		Parents& forkParents,
		ForkPointers& forkPointers);

	void wideRegister(StateRegistryBase& stateRegistry,
					  const ShortIndex fork,
					  const TypeInfo parentInfo);

	inline void wideForwardGuard	 (const ShortIndex prong, TransitionControl& control);

	inline void wideForwardGuard	 (TransitionControl& control);
	inline void wideGuard			 (TransitionControl& control);

	inline void wideEnterInitial	 (Control& control);
	inline void wideEnter			 (Control& control);

	inline bool wideUpdate			 (TransitionControl& control);

	template <typename TEvent>
	inline void wideReact			 (const TEvent& event, TransitionControl& control);

	inline void wideExit			 (Control& control);

	inline void wideForwardRequest	 (const ShortIndex prong, const TransitionType transition);
	inline void wideRequestRemain	 ();
	inline void wideRequestRestart	 ();
	inline void wideRequestResume	 ();
	inline void wideChangeToRequested(Control& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	enum : LongIndex {
		NAME_COUNT	 = Initial::NAME_COUNT  + Remaining::NAME_COUNT,
	};

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;

	void wideIsActive(const bool active,
					  LongIndex& index,
					  MachineStructure& structure) const;
#endif

	Initial initial;
	Remaining remaining;
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TPayloadList,
		  ShortIndex NIndex,
		  typename TInitial>
struct _OS<TContext, TPayloadList, NIndex, TInitial> {
	using Context			= TContext;
	using Control			= ControlT<Context>;
	using PayloadList		= TPayloadList;
	using StateRegistryBase	= StateRegistryBaseT<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= enum Transition::Type;
	using TransitionControl	= TransitionControlT<Context, PayloadList>;
	using Initial			= typename WrapState<Context, PayloadList, TInitial>::Type;
	using StateList			= typename Initial::StateList;
	HSFM_IF_DEBUG(StateList stateList);

	enum : LongIndex {
		PRONG_INDEX	  = NIndex,
		REVERSE_DEPTH = Initial::REVERSE_DEPTH,
		DEEP_WIDTH	  = Initial::DEEP_WIDTH,
		STATE_COUNT	  = Initial::STATE_COUNT,
		FORK_COUNT	  = Initial::FORK_COUNT,
		PRONG_COUNT	  = Initial::PRONG_COUNT,
	};

	_OS(StateRegistryBase& stateRegistry,
		const ShortIndex fork,
		const TypeInfo parentInfo,
		Parents& forkParents,
		ForkPointers& forkPointers);

	void wideRegister(StateRegistryBase& stateRegistry,
					  const ShortIndex fork,
					  const TypeInfo parentInfo);

	inline void wideForwardGuard	 (const ShortIndex prong, TransitionControl& control);

	inline void wideForwardGuard	 (TransitionControl& control);
	inline void wideGuard			 (TransitionControl& control);

	inline void wideEnterInitial	 (Control& control);
	inline void wideEnter			 (Control& control);

	inline bool wideUpdate			 (TransitionControl& control);

	template <typename TEvent>
	inline void wideReact			 (const TEvent& event, TransitionControl& control);

	inline void wideExit			 (Control& control);

	inline void wideForwardRequest	 (const ShortIndex prong, const TransitionType transition);
	inline void wideRequestRemain	 ();
	inline void wideRequestRestart	 ();
	inline void wideRequestResume	 ();
	inline void wideChangeToRequested(Control& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	enum : LongIndex {
		NAME_COUNT	 = Initial::NAME_COUNT,
	};

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;

	void wideIsActive(const bool active,
					  LongIndex& index,
					  MachineStructure& structure) const;
#endif

	Initial initial;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
_OS<TC, TPL, NI, TI, TR...>::_OS(StateRegistryBase& stateRegistry,
								 const ShortIndex fork,
								 const TypeInfo parentInfo,
								 Parents& forkParents,
								 ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork,
					 PRONG_INDEX,
					 parentInfo,
					 TypeInfo::get<typename Initial::Head>()),
			  forkParents,
			  forkPointers)
	, remaining(stateRegistry,
				fork,
				parentInfo,
				forkParents,
				forkPointers)
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideRegister(StateRegistryBase& stateRegistry,
										  const ShortIndex fork,
										  const TypeInfo parentInfo)
{
	initial	 .deepRegister(stateRegistry,
						   Parent(fork,
								  PRONG_INDEX,
								  parentInfo,
								  TypeInfo::get<typename Initial::Head>()));
	remaining.wideRegister(stateRegistry,
						   fork,
						   parentInfo);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideForwardGuard(const ShortIndex prong,
											  TransitionControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepForwardGuard(control);
	else
		remaining.wideForwardGuard(prong, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideForwardGuard(TransitionControl& control) {
	initial	 .deepForwardGuard(control);
	remaining.wideForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideGuard(TransitionControl& control) {
	initial	 .deepGuard(control);
	remaining.wideGuard(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideEnterInitial(Control& control) {
	initial  .deepEnterInitial(control);
	remaining.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideEnter(Control& control) {
	initial  .deepEnter(control);
	remaining.wideEnter(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
bool
_OS<TC, TPL, NI, TI, TR...>::wideUpdate(TransitionControl& control) {
	const bool r = initial.deepUpdate(control);

	return		 remaining.wideUpdate(control) || r;
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
template <typename TEvent>
void
_OS<TC, TPL, NI, TI, TR...>::wideReact(const TEvent& event,
									   TransitionControl& control)
{
	initial  .deepReact(event, control);
	remaining.wideReact(event, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideExit(Control& control) {
	initial	 .deepExit(control);
	remaining.wideExit(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideForwardRequest(const ShortIndex prong,
												const TransitionType transition)
{
	if (prong == PRONG_INDEX) {
		initial	 .deepForwardRequest(		transition);
		remaining.wideForwardRequest(prong, Transition::REMAIN);
	} else {
		initial	 .deepForwardRequest(		Transition::REMAIN);
		remaining.wideForwardRequest(prong, transition);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideRequestRemain() {
	initial	 .deepRequestRemain();
	remaining.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideRequestRestart() {
	initial	 .deepRequestRestart();
	remaining.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideRequestResume() {
	initial	 .deepRequestResume();
	remaining.wideRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideChangeToRequested(Control& control) {
	initial	 .deepChangeToRequested(control);
	remaining.wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideGetNames(const LongIndex parent,
										  const ShortIndex depth,
										  StructureStateInfos& _stateInfos) const
{
	initial	 .deepGetNames(parent, StructureStateInfo::ORTHOGONAL, depth, _stateInfos);
	remaining.wideGetNames(parent,								   depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideIsActive(const bool isActive,
										  LongIndex& index,
										  MachineStructure& structure) const
{
	initial	 .deepIsActive(isActive, index, structure);
	remaining.wideIsActive(isActive, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TPL, ShortIndex NI, typename TI>
_OS<TC, TPL, NI, TI>::_OS(StateRegistryBase& stateRegistry,
						  const ShortIndex fork,
						  const TypeInfo parentInfo,
						  Parents& forkParents,
						  ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork,
					 PRONG_INDEX,
					 parentInfo,
					 TypeInfo::get<typename Initial::Head>()),
			  forkParents,
			  forkPointers)
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideRegister(StateRegistryBase& stateRegistry,
								   const ShortIndex fork,
								   const TypeInfo parentInfo)
{
	initial.deepRegister(stateRegistry,
						 Parent(fork,
								PRONG_INDEX,
								parentInfo,
								TypeInfo::get<typename Initial::Head>()));
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideForwardGuard(const ShortIndex HSFM_IF_ASSERT(prong),
									   TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideForwardGuard(TransitionControl& control) {
	initial.deepForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideGuard(TransitionControl& control) {
	initial.deepGuard(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideEnterInitial(Control& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideEnter(Control& control) {
	initial.deepEnter(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
bool
_OS<TC, TPL, NI, TI>::wideUpdate(TransitionControl& control) {
	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
template <typename TEvent>
void
_OS<TC, TPL, NI, TI>::wideReact(const TEvent& event,
								TransitionControl& control)
{
	initial.deepReact(event, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideExit(Control& control) {
	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideForwardRequest(const ShortIndex prong,
										 const TransitionType transition)
{
	assert(prong <= PRONG_INDEX);

	if (prong == PRONG_INDEX)
		initial.deepForwardRequest(transition);
	else
		initial.deepForwardRequest(Transition::REMAIN);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideRequestResume() {
	initial.deepRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideChangeToRequested(Control& control) {
	initial.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideGetNames(const LongIndex parent,
								   const ShortIndex depth,
								   StructureStateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StructureStateInfo::ORTHOGONAL, depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideIsActive(const bool isActive,
								   LongIndex& index,
								   MachineStructure& structure) const
{
	initial.deepIsActive(isActive, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TPayloadList,
		  typename THead,
		  typename... TSubStates>
struct _O final {
	using Context			= TContext;
	using Control			= ControlT<Context>;
	using PayloadList		= TPayloadList;
	using StateRegistryBase	= StateRegistryBaseT<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= enum Transition::Type;
	using TransitionControl	= TransitionControlT<Context, PayloadList>;
	using ControlLock		= ControlLockT<Context, PayloadList>;
	using Head				= THead;
	using Fork				= ForkT<Head>;
	using State				= _S<Context, PayloadList, Head>;
	using SubStates			= _OS<Context, PayloadList, 0, TSubStates...>;
	using StateList			= typename MergeT<typename State::StateList, typename SubStates::StateList>::TypeList;
	HSFM_IF_DEBUG(StateList stateList);

	enum : LongIndex {
		REVERSE_DEPTH = SubStates::REVERSE_DEPTH + 1,
		DEEP_WIDTH	  = SubStates::DEEP_WIDTH,
		STATE_COUNT	  = State::STATE_COUNT + SubStates::STATE_COUNT,
		FORK_COUNT	  = SubStates::FORK_COUNT + 1,
		PRONG_COUNT	  = SubStates::PRONG_COUNT,
		WIDTH		  = sizeof...(TSubStates),
	};

	_O(StateRegistryBase& stateRegistry,
	   const Parent parent,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

	inline void deepRegister		 (StateRegistryBase& stateRegistry, const Parent parent);

	inline void deepForwardGuard	 (TransitionControl& control);
	inline void deepGuard			 (TransitionControl& control);

	inline void deepEnterInitial	 (Control& control);
	inline void deepEnter			 (Control& control);

	inline bool deepUpdate			 (TransitionControl& control);

	template <typename TEvent>
	inline void deepReact			 (const TEvent& event, TransitionControl& control);

	inline void deepExit			 (Control& control);

	inline void deepForwardRequest	 (const TransitionType transition);
	inline void deepRequestRemain	 ();
	inline void deepRequestRestart	 ();
	inline void deepRequestResume	 ();
	inline void deepChangeToRequested(Control& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	enum : LongIndex {
		NAME_COUNT	 = State::NAME_COUNT  + SubStates::NAME_COUNT,
	};

	void deepGetNames(const LongIndex parent,
					  const enum StructureStateInfo::RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;

	void deepIsActive(const bool isActive,
					  LongIndex& index,
					  MachineStructure& structure) const;
#endif

	Fork _fork;
	State _state;
	SubStates _subStates;

	HSFM_IF_DEBUG(const TypeInfo _type = TypeInfo::get<Head>());
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TPL, typename TH, typename... TS>
_O<TC, TPL, TH, TS...>::_O(StateRegistryBase& stateRegistry,
						   const Parent parent,
						   Parents& forkParents,
						   ForkPointers& forkPointers)
	: _fork(static_cast<ShortIndex>(forkPointers << &_fork),
			parent,
			forkParents)
	, _state(stateRegistry,
			 parent,
			 forkParents,
			 forkPointers)
	, _subStates(stateRegistry,
				 _fork.self,
				 TypeInfo::get<Head>(),
				 forkParents,
				 forkPointers)
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_O<TC, TPL, TH, TS...>::deepRegister(StateRegistryBase& stateRegistry,
									 const Parent parent)
{
	_state	  .deepRegister(stateRegistry, parent);
	_subStates.wideRegister(stateRegistry,
							_fork.self,
							TypeInfo::get<Head>());
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH, typename... TS>
void
_O<TC, TPL, TH, TS...>::deepForwardGuard(TransitionControl& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	if (_fork.requested != INVALID_SHORT_INDEX)
		_subStates.wideForwardGuard(_fork.requested, control);
	else
		_subStates.wideForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_O<TC, TPL, TH, TS...>::deepGuard(TransitionControl& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	if (!_state	  .deepGuard(control))
		_subStates.wideGuard(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH, typename... TS>
void
_O<TC, TPL, TH, TS...>::deepEnterInitial(Control& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX &&
		   _fork.requested == INVALID_SHORT_INDEX);

	_state	  .deepEnter	   (control);
	_subStates.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_O<TC, TPL, TH, TS...>::deepEnter(Control& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_state	  .deepEnter(control);
	_subStates.wideEnter(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH, typename... TS>
bool
_O<TC, TPL, TH, TS...>::deepUpdate(TransitionControl& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	if (_state	  .deepUpdate(control)) {
		ControlLock lock(control);
		_subStates.wideUpdate(control);

		return true;
	} else
		return _subStates.wideUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH, typename... TS>
template <typename TEvent>
void
_O<TC, TPL, TH, TS...>::deepReact(const TEvent& event,
							 TransitionControl& control)
{
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_state	  .deepReact(event, control);
	_subStates.wideReact(event, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH, typename... TS>
void
_O<TC, TPL, TH, TS...>::deepExit(Control& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_subStates.wideExit(control);
	_state	  .deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH, typename... TS>
void
_O<TC, TPL, TH, TS...>::deepForwardRequest(const TransitionType transition) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	if (_fork.requested != INVALID_SHORT_INDEX)
		_subStates.wideForwardRequest(_fork.requested, transition);
	else
		switch (transition) {
		case Transition::REMAIN:
			deepRequestRemain();
			break;

		case Transition::RESTART:
			deepRequestRestart();
			break;

		case Transition::RESUME:
			deepRequestResume();
			break;

		default:
			assert(false);
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_O<TC, TPL, TH, TS...>::deepRequestRemain() {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_O<TC, TPL, TH, TS...>::deepRequestRestart() {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_O<TC, TPL, TH, TS...>::deepRequestResume() {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_subStates.wideRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_O<TC, TPL, TH, TS...>::deepChangeToRequested(Control& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_subStates.wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, typename TPL, typename TH, typename... TS>
void
_O<TC, TPL, TH, TS...>::deepGetNames(const LongIndex parent,
									 const enum StructureStateInfo::RegionType region,
									 const ShortIndex depth,
									 StructureStateInfos& _stateInfos) const
{
	_state	  .deepGetNames(parent, region,			 depth,		_stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, depth + 1, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_O<TC, TPL, TH, TS...>::deepIsActive(const bool isActive,
									 LongIndex& index,
									 MachineStructure& structure) const
{
	_state	  .deepIsActive(isActive, index, structure);
	_subStates.wideIsActive(isActive, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TPayloadList,
		  ShortIndex TMaxSubstitutions,
		  typename TApex>
class _R final {
	enum : ShortIndex { MAX_SUBSTITUTIONS = TMaxSubstitutions };

	using Context			= TContext;
	using Control			= ControlT<Context>;
	using PayloadList		= TPayloadList;
	using StateRegistryBase	= StateRegistryBaseT<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionControl	= TransitionControlT<Context, PayloadList>;
	using Apex				= typename WrapState<Context, PayloadList, TApex>::Type;

public:
	using StateList			= typename Apex::StateList;
	HSFM_IF_DEBUG(StateList stateList);

public:
	enum : LongIndex {
		REVERSE_DEPTH			 = Apex::REVERSE_DEPTH,
		DEEP_WIDTH				 = Apex::DEEP_WIDTH,
		STATE_COUNT				 = Apex::STATE_COUNT,
		FORK_COUNT				 = Apex::FORK_COUNT,
		PRONG_COUNT				 = Apex::PRONG_COUNT,
		WIDTH					 = Apex::WIDTH,
	};
	static_assert(STATE_COUNT < (ShortIndex) -1, "Too many states in the hierarchy. Change 'ShortIndex' type.");
	static_assert(STATE_COUNT == StateList::SIZE, "STATE_COUNT != StateList::SIZE");

private:
	enum : LongIndex {
		STATE_CAPACITY = (LongIndex) 1.3 * Apex::STATE_COUNT,
	};

	using StateRegistryImpl		 = StateRegistryT<PayloadList, STATE_CAPACITY>;
	using ForkParentStorage		 = Array<Parent,	 FORK_COUNT>;
	using ForkPointerStorage	 = Array<Fork*,		 FORK_COUNT>;
	using TransitionQueueStorage = Array<Transition, FORK_COUNT>;

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	enum : LongIndex {
		NAME_COUNT	  = Apex::NAME_COUNT,
	};

	using Prefix				 = StaticArray<wchar_t,			REVERSE_DEPTH * 2 + 2>;
	using Prefixes				 = StaticArray<Prefix,			STATE_COUNT>;

	using StateInfoStorage		 = Array<StructureStateInfo,	STATE_COUNT>;

	using StructureStorage		 = Array<StructureEntry,		NAME_COUNT>;
	using ActivityHistoryStorage = Array<char,					NAME_COUNT>;

	using TransitionInfo		 = TransitionInfoT<PayloadList>;
	using TransitionInfoStorage	 = Array<TransitionInfo,		FORK_COUNT * 2>;
#endif

public:
	_R(Context& context
	   HFSM_IF_LOGGER(, LoggerInterface* const logger = nullptr));

	~_R();

	void update();

	template <typename TEvent>
	inline void react(const TEvent& event);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	inline void changeTo(const std::type_index state);
	inline void resume	(const std::type_index state);
	inline void schedule(const std::type_index state);

	template <typename TState>
	inline typename std::enable_if<StateList::template Contains<TState>::VALUE>::type
	changeTo()												{ changeTo(typeid(TState));		}

	template <typename TState>
	inline typename std::enable_if<StateList::template Contains<TState>::VALUE>::type
	resume	()												{ resume  (typeid(TState));		}

	template <typename TState>
	inline typename std::enable_if<StateList::template Contains<TState>::VALUE>::type
	schedule()												{ schedule(typeid(TState));		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TPayload>
	inline typename std::enable_if<TPayloadList::template Contains<TPayload>::VALUE>::type
	changeTo(const std::type_index state, TPayload* const payload);

	template <typename TPayload>
	inline typename std::enable_if<TPayloadList::template Contains<TPayload>::VALUE>::type
	resume(const std::type_index state, TPayload* const payload);

	template <typename TPayload>
	inline typename std::enable_if<TPayloadList::template Contains<TPayload>::VALUE>::type
	schedule(const std::type_index state, TPayload* const payload);

	template <typename TState, typename TPayload>
	inline typename std::enable_if<StateList::template Contains<TState>::VALUE &&
								 PayloadList::template Contains<TPayload>::VALUE>::type
	changeTo(TPayload* const payload)				{ changeTo(typeid(TState), payload);	}

	template <typename TState, typename TPayload>
	inline typename std::enable_if<StateList::template Contains<TState>::VALUE &&
								 PayloadList::template Contains<TPayload>::VALUE>::type
	resume	(TPayload* const payload)				{ resume  (typeid(TState), payload);	}

	template <typename TState, typename TPayload>
	inline typename std::enable_if<StateList::template Contains<TState>::VALUE &&
								 PayloadList::template Contains<TPayload>::VALUE>::type
	schedule(TPayload* const payload)				{ schedule(typeid(TState), payload);	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	inline void resetStateData(const std::type_index state);

	inline bool isStateDataSet(const std::type_index state);

	template <typename TPayload>
	inline typename std::enable_if<TPayloadList::template Contains<TPayload>::VALUE>::type
	setStateData(const std::type_index state, TPayload* const payload);

	template <typename TPayload>
	inline typename std::enable_if<TPayloadList::template Contains<TPayload>::VALUE, TPayload>::type*
	getStateData(const std::type_index state);

	template <typename TState>
	inline typename std::enable_if<StateList::template Contains<TState>::VALUE>::type
	resetStateData()							{ resetStateData(typeid(TState));			}

	template <typename TState>
	inline typename std::enable_if<StateList::template Contains<TState>::VALUE, bool>::type
	isStateDataSet()							{ return isStateDataSet(typeid(TState));	}

	template <typename TState, typename TPayload>
	inline typename std::enable_if<StateList::template Contains<TState>::VALUE &&
								 PayloadList::template Contains<TPayload>::VALUE>::type
	setStateData(TPayload* const payload)		{ setStateData(typeid(TState), payload);	}

	template <typename TState, typename TPayload>
	inline typename std::enable_if<StateList::template Contains<TState>::VALUE &&
								 PayloadList::template Contains<TPayload>::VALUE, TPayload>::type*
	getStateData()						{ return getStateData<TPayload>(typeid(TState));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	inline bool isActive   (const std::type_index state);
	inline bool isResumable(const std::type_index state);
	inline bool isScheduled(const std::type_index state)	{ return isResumable(state);	}

	template <typename TState>
	inline typename std::enable_if<StateList::template Contains<TState>::VALUE, bool>::type
	isActive()										{ return isActive	(typeid(TState));	}

	template <typename TState>
	inline typename std::enable_if<StateList::template Contains<TState>::VALUE, bool>::type
	isResumable()									{ return isResumable(typeid(TState));	}

	template <typename TState>
	inline typename std::enable_if<StateList::template Contains<TState>::VALUE, bool>::type
	isScheduled()									{ return isScheduled(typeid(TState));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	const MachineStructure& structure() const					{ return _structure;		}
	const MachineActivity&  activity()  const					{ return _activityHistory;	}
#endif

#ifdef HFSM_ENABLE_LOG_INTERFACE
	void attachLogger(LoggerInterface* const logger)					{ _logger = logger;	}
#endif

protected:
	void processTransitions();
	void requestImmediate(const Transition request);
	void requestScheduled(const Transition request);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	void getStateNames();
	void udpateActivity();
#endif

private:
	Context& _context;

	StateRegistryImpl _stateRegistry;

	ForkParentStorage  _forkParents;
	ForkPointerStorage _forkPointers;

	TransitionQueueStorage _requests;

	Apex _apex;

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	Prefixes _prefixes;
	StateInfoStorage _stateInfos;

	StructureStorage _structure;
	ActivityHistoryStorage _activityHistory;

	TransitionInfoStorage _lastTransitions;
#endif

	HFSM_IF_LOGGER(LoggerInterface* _logger);
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
_R<TC, TPL, TMS, TA>::_R(Context& context
						 HFSM_IF_LOGGER(, LoggerInterface* const logger))
	: _context{context}
	, _apex{_stateRegistry, Parent{}, _forkParents, _forkPointers}
	HFSM_IF_LOGGER(, _logger{logger})
{
	_apex.deepRegister(_stateRegistry, Parent{});

	HFSM_IF_STRUCTURE(getStateNames());

	{
		Control control{_context, HFSM_LOGGER_OR(_logger, nullptr)};
		_apex.deepEnterInitial(control);
	}

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
_R<TC, TPL, TMS, TA>::~_R() {
	Control control{_context, HFSM_LOGGER_OR(_logger, nullptr)};
	_apex.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::update() {
	TransitionControl control(_context, _requests, HFSM_LOGGER_OR(_logger, nullptr));

	if (_apex.deepUpdate(control))
		processTransitions();

	_requests.clear();
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename TEvent>
void
_R<TC, TPL, TMS, TA>::react(const TEvent& event) {
	TransitionControl control(_context, _requests, HFSM_LOGGER_OR(_logger, nullptr));
	_apex.deepReact(event, control);

	if (_requests.count())
		processTransitions();

	_requests.clear();
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::changeTo(const std::type_index state)	{
	const Transition transition{Transition::Type::RESTART, state};
	_requests << transition;

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (_logger)
		_logger->recordTransition(LoggerInterface::Transition::RESTART, state);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::resume(const std::type_index state) {
	const Transition transition{Transition::Type::RESUME, state};
	_requests << transition;

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (_logger)
		_logger->recordTransition(LoggerInterface::Transition::RESUME, state);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::schedule(const std::type_index state) {
	const Transition transition{Transition::Type::SCHEDULE, state};
	_requests << transition;

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (_logger)
		_logger->recordTransition(LoggerInterface::Transition::SCHEDULE, state);
#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename TPayload>
typename std::enable_if<TPL::template Contains<TPayload>::VALUE>::type
_R<TC, TPL, TMS, TA>::changeTo(const std::type_index state,
							   TPayload* const payload)
{
	const Transition transition{Transition::Type::RESTART, state, payload};
	_requests << transition;

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (_logger)
		_logger->recordTransition(LoggerInterface::Transition::RESTART, state);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename TPayload>
typename std::enable_if<TPL::template Contains<TPayload>::VALUE>::type
_R<TC, TPL, TMS, TA>::resume(const std::type_index state,
							 TPayload* const payload)
{
	const Transition transition{Transition::Type::RESUME, state, payload};
	_requests << transition;

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (_logger)
		_logger->recordTransition(LoggerInterface::Transition::RESUME, state);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename TPayload>
typename std::enable_if<TPL::template Contains<TPayload>::VALUE>::type
_R<TC, TPL, TMS, TA>::schedule(const std::type_index state,
							   TPayload* const payload)
{
	const Transition transition{Transition::Type::SCHEDULE, state, payload};
	_requests << transition;

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (_logger)
		_logger->recordTransition(LoggerInterface::Transition::SCHEDULE, state);
#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::resetStateData(const std::type_index state) {
	auto& stateInfo = _stateRegistry[state];

	stateInfo.payload.reset();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
bool
_R<TC, TPL, TMS, TA>::isStateDataSet(const std::type_index state) {
	auto& stateInfo = _stateRegistry[state];

	return !!stateInfo.payload;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename TPayload>
typename std::enable_if<TPL::template Contains<TPayload>::VALUE>::type
_R<TC, TPL, TMS, TA>::setStateData(const std::type_index state,
								   TPayload* const payload)
{
	auto& stateInfo = _stateRegistry[state];

	stateInfo.payload = payload;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename TPayload>
typename std::enable_if<TPL::template Contains<TPayload>::VALUE, TPayload>::type*
_R<TC, TPL, TMS, TA>::getStateData(const std::type_index state) {
	auto& stateInfo = _stateRegistry[state];
	auto& payload = stateInfo.payload;

	return payload.template get<TPayload>();
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
bool
_R<TC, TPL, TMS, TA>::isActive(const std::type_index state) {
	const auto& stateInfo = _stateRegistry[state];

	for (auto parent = stateInfo.parent; parent; parent = _forkParents[parent.fork]) {
		const auto& fork = *_forkPointers[parent.fork];

		if (fork.active != INVALID_SHORT_INDEX)
			return parent.prong == fork.active;
	}

	return false;
}

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
bool
_R<TC, TPL, TMS, TA>::isResumable(const std::type_index state) {
	const auto& stateInfo = _stateRegistry[state];

	for (auto parent = stateInfo.parent; parent; parent = _forkParents[parent.fork]) {
		const auto& fork = *_forkPointers[parent.fork];

		if (fork.active != INVALID_SHORT_INDEX)
			return parent.prong == fork.resumable;
	}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::processTransitions() {
	assert(_requests.count());

	HFSM_IF_STRUCTURE(_lastTransitions.clear());

	for (unsigned i = 0;
		i < MAX_SUBSTITUTIONS && _requests.count();
		++i)
	{
		unsigned changeCount = 0;

		for (const auto& request : _requests) {
			HFSM_IF_STRUCTURE(_lastTransitions << TransitionInfo(request, Method::UPDATE));

			switch (request.type) {
			case Transition::RESTART:
			case Transition::RESUME:
				requestImmediate(request);

				++changeCount;
				break;

			case Transition::SCHEDULE:
				requestScheduled(request);
				break;

			default:
				assert(false);
			}
		}
		_requests.clear();

		if (changeCount > 0) {
			TransitionControl substitutionControl(_context, _requests, HFSM_LOGGER_OR(_logger, nullptr));
			_apex.deepForwardGuard(substitutionControl);

		#ifdef HFSM_ENABLE_STRUCTURE_REPORT
			for (const auto& request : _requests)
				_lastTransitions << TransitionInfo(request, Method::GUARD);
		#endif
		}
	}

	{
		Control control{_context, HFSM_LOGGER_OR(_logger, nullptr)};
		_apex.deepChangeToRequested(control);
	}

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::requestImmediate(const Transition request) {
	const auto& stateInfo = _stateRegistry[*request.stateType];

	for (auto parent = stateInfo.parent; parent; parent = _forkParents[parent.fork]) {
		auto& fork = *_forkPointers[parent.fork];

		HSFM_IF_DEBUG(fork.requestedType = parent.prongType);
		fork.requested = parent.prong;
	}

	_apex.deepForwardRequest(request.type);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::requestScheduled(const Transition request) {
	const auto& stateInfo = _stateRegistry[*request.stateType];

	const auto parent = stateInfo.parent;
	auto& fork = *_forkPointers[parent.fork];

	HSFM_IF_DEBUG(fork.resumableType = parent.prongType);
	fork.resumable = parent.prong;
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::getStateNames() {
	_stateInfos.clear();
	_apex.deepGetNames((LongIndex) -1, StructureStateInfo::COMPOSITE, 0, _stateInfos);

	LongIndex margin = (LongIndex) -1;
	for (LongIndex s = 0; s < _stateInfos.count(); ++s) {
		const auto& state = _stateInfos[s];
		auto& prefix      = _prefixes[s];

		if (margin > state.depth && state.name[0] != '\0')
			margin = state.depth;

		if (state.depth == 0)
			prefix[0] = L'\0';
		else {
			const LongIndex mark = state.depth * 2 - 1;

			prefix[mark + 0] = state.region == StructureStateInfo::COMPOSITE ? L'└' : L'╙';
			prefix[mark + 1] = L' ';
			prefix[mark + 2] = L'\0';

			for (auto d = mark; d > 0; --d)
				prefix[d - 1] = L' ';

			for (auto r = s; r > state.parent; --r) {
				auto& prefixAbove = _prefixes[r - 1];

				switch (prefixAbove[mark]) {
				case L' ':
					prefixAbove[mark] = state.region == StructureStateInfo::COMPOSITE ? L'│' : L'║';
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
	for (LongIndex s = 0; s < _stateInfos.count(); ++s) {
		const auto& state = _stateInfos[s];
		auto& prefix = _prefixes[s];
		const LongIndex space = state.depth * 2;

		if (state.name[0] != L'\0') {
			_structure << StructureEntry { false, &prefix[margin * 2], state.name };
			_activityHistory << (char) 0;
		} else if (s + 1 < _stateInfos.count()) {
			auto& nextPrefix = _prefixes[s + 1];

			if (s > 0)
				for (LongIndex c = 0; c <= space; ++c)
					nextPrefix[c] = prefix[c];

			const LongIndex mark = space + 1;

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

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::udpateActivity() {
	for (auto& item : _structure)
		item.isActive = false;

	LongIndex index = 0;
	_apex.deepIsActive(true, index, _structure);

	for (LongIndex i = 0; i < _structure.count(); ++i) {
		auto& activity = _activityHistory[i];

		if (_structure[i].isActive) {
			if (activity > 0)
				activity = activity < INT8_MIN ? activity + 1 : activity;
			else
				activity = +1;
		} else {
			if (activity > 0)
				activity = -1;
			else
				activity = activity > INT8_MIN ? activity - 1 : activity;
		}
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#undef HFSM_IF_LOGGER
#undef HFSM_LOGGER_OR
#undef HFSM_IF_STRUCTURE
