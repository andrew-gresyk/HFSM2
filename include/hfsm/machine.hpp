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

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <typeindex>
#include <utility>

//------------------------------------------------------------------------------


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
	#define HSFM_UNLESS_DEBUG(...)
	#define HFSM_DEBUG_OR(y, n)		x
#else
	#define HSFM_IF_DEBUG(...)
	#define HSFM_UNLESS_DEBUG(...)	__VA_ARGS__
	#define HFSM_DEBUG_OR(y, n)		y
#endif

#ifndef NDEBUG
	#define HSFM_IF_ASSERT(...)		__VA_ARGS__
#else
	#define HSFM_IF_ASSERT(...)
#endif

namespace hfsm {

//------------------------------------------------------------------------------

using ShortIndex = uint8_t;
static constexpr ShortIndex	INVALID_SHORT_INDEX = UINT8_MAX;

using ForkID	 = ShortIndex;
static constexpr ForkID		INVALID_FORK_ID	= INVALID_SHORT_INDEX;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

using LongIndex  = uint16_t;
static constexpr LongIndex	INVALID_LONG_INDEX	= UINT16_MAX;

using StateID	 = LongIndex;
static constexpr StateID	INVALID_STATE_ID	= INVALID_LONG_INDEX;

//------------------------------------------------------------------------------

namespace detail {

HSFM_IF_DEBUG(struct None {});

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
constexpr
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
	static constexpr auto VALUE = N1 < N2 ? N1 : N2;
};

//------------------------------------------------------------------------------

template <int N1, int N2>
struct Max {
	static constexpr auto VALUE = N1 > N2 ? N1 : N2;
};

//------------------------------------------------------------------------------

template <unsigned NCapacity>
struct UnsignedIndex {
	static constexpr LongIndex CAPACITY = NCapacity;

	using Type = typename std::conditional<CAPACITY <= UINT8_MAX,  uint8_t,
				 typename std::conditional<CAPACITY <= UINT16_MAX, uint16_t,
				 typename std::conditional<CAPACITY <= UINT32_MAX, uint32_t,
																   uint64_t>::type>::type>::type;

	static_assert(CAPACITY <= UINT64_MAX, "STATIC ASSERT");
};

////////////////////////////////////////////////////////////////////////////////

template <unsigned V1, unsigned V2>
constexpr void
assertEquality() {
	static_assert(V1 == V2, "assertEquality() failed");
}

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

	inline		 Item& operator *()		  { return  _container[_cursor]; }
	inline const Item& operator *() const { return  _container[_cursor]; }

	inline		 Item* operator->()		  { return &_container[_cursor]; }
	inline const Item* operator->() const { return &_container[_cursor]; }

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

#pragma pack(push, 4)

template <typename T>
class ArrayView {
public:
	using Item = T;

	template <typename>
	friend class Iterator;

	static constexpr auto ITEM_ALIGNMENT = alignof(Item[2]);
	static constexpr auto VIEW_SIZE		 = 4;
	static constexpr auto OFFSET		 = (VIEW_SIZE + ITEM_ALIGNMENT - 1) / ITEM_ALIGNMENT * ITEM_ALIGNMENT;

protected:
	ArrayView(const LongIndex capacity);
	~ArrayView();

public:
	inline void clear()												{ _count = 0;			}

	inline LongIndex resize(const LongIndex count);

	template <typename TValue>
	inline LongIndex operator << (TValue&& value);

	inline		 Item& operator[] (const LongIndex i)				{ return get(i);		}
	inline const Item& operator[] (const LongIndex i) const			{ return get(i);		}

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
	inline		 Item* data()		{ return reinterpret_cast<		Item*>(((uintptr_t)this) + OFFSET);	}
	inline const Item* data() const	{ return reinterpret_cast<const Item*>(((uintptr_t)this) + OFFSET);	}

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
	static constexpr LongIndex CAPACITY = NCapacity;

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
	static constexpr LongIndex CAPACITY = NCapacity;
	static constexpr LongIndex INVALID	= INVALID_LONG_INDEX;
	static constexpr LongIndex DUMMY	= INVALID;

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

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

Bit::operator bool() const {
	return _array.get(_index);
}

//------------------------------------------------------------------------------

void
Bit::operator = (const bool value) {
	_array.set(_index, value);
}

//------------------------------------------------------------------------------

ConstBit::operator bool() const {
	return _array.get(_index);
}

////////////////////////////////////////////////////////////////////////////////

void
BitArray::clear() {
	StorageUnit* const s = storage();

	const ShortIndex count = storageUnitCount();
	for (ShortIndex i = 0; i < count; ++i)
		s[i] = 0;
}

//------------------------------------------------------------------------------

BitArray::operator bool() const {
	StorageUnit result = 0;

	const StorageUnit* const s = storage();

	const ShortIndex count = storageUnitCount();
	for (ShortIndex i = 0; i < count; ++i)
		result |= s[i];

	return result != 0;
}

//------------------------------------------------------------------------------

bool
BitArray::get(const ShortIndex i) const {
	assert(i < capacity);

	const auto d = std::div(i, sizeof(StorageUnit) * 8);

	const StorageUnit& unit = storage()[d.quot];
	const ShortIndex mask = 1 << d.rem;

	return (unit & mask) != 0;
}

//------------------------------------------------------------------------------

void
BitArray::set(const ShortIndex i, const bool value) {
	assert(i < capacity);

	const auto d = std::div(i, sizeof(StorageUnit) * 8);

	StorageUnit& unit = storage()[d.quot];

	if (value) {
		const ShortIndex mask = 1 << d.rem;
		unit |= mask;
	} else {
		const ShortIndex mask = ~(1 << d.rem);
		unit &= mask;
	}
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TItem, LongIndex NCapacity>
class List {
public:
	using Index = LongIndex;

	static constexpr Index CAPACITY = NCapacity;

	static constexpr Index INVALID = Index (-1);

private:
	using Item  = TItem;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Links {
		Index prev;
		Index next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	union Cell {
		Item item;
		Links links;

		inline Cell()
			: links{}
		{}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	template <typename... TArgs>
	Index emplace(TArgs... args);

	void remove(const Index i);

	inline		 Item& operator[] (const Index i);
	inline const Item& operator[] (const Index i) const;

	inline Index count() const { return _count; }

private:
	HSFM_IF_DEBUG(void verifyStructure(const Index occupied = INVALID) const);

private:
	Cell _cells[CAPACITY];
	Index _vacantHead = 0;
	Index _vacantTail = 0;
	Index _last = 0;
	Index _count = 0;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TItem, LongIndex NCapacity>
template <typename... TArgs>
LongIndex
List<TItem, NCapacity>::emplace(TArgs... args) {
	if (_count < CAPACITY) {
		assert(_vacantHead < CAPACITY);
		assert(_vacantTail < CAPACITY);

		const Index result = _vacantHead;
		auto& cell = _cells[result];
		++_count;

		if (_vacantHead != _vacantTail) {
			// recycle
			assert(cell.links.prev == INVALID);
			assert(cell.links.next != INVALID);

			_vacantHead = cell.links.next;

			auto& head = _cells[_vacantHead];
			assert(head.links.prev == result);
			head.links.prev = INVALID;
		} else if (_last < CAPACITY) {
			// grow
			++_last;
			_vacantHead = _last;
			_vacantTail = _last;

			auto& nextVacant = _cells[_vacantHead];
			nextVacant.links.prev = INVALID;
			nextVacant.links.next = INVALID;
		} else {
			assert(_count == CAPACITY);

			_vacantHead = INVALID;
			_vacantTail = INVALID;
		}

		HSFM_IF_DEBUG(verifyStructure());

		new (&cell.item) Item{std::forward<TArgs>(args)...};

		return result;
	} else {
		// full
		assert(_vacantHead == INVALID);
		assert(_vacantTail == INVALID);
		assert(_count == CAPACITY);
		assert(false);

		return INVALID;
	}
}

//------------------------------------------------------------------------------

template <typename TItem, LongIndex NCapacity>
void
List<TItem, NCapacity>::remove(const Index i) {
	assert(i < CAPACITY && _count);

	auto& fresh = _cells[i];

	if (_count < CAPACITY) {
		assert(_vacantHead < CAPACITY);
		assert(_vacantTail < CAPACITY);

		fresh.links.prev = INVALID;
		fresh.links.next = _vacantHead;

		auto& head = _cells[_vacantHead];
		head.links.prev = i;

		_vacantHead = i;
	} else {
		// 0 -> 1
		assert(_count == CAPACITY);
		assert(_vacantHead == INVALID);
		assert(_vacantTail == INVALID);

		fresh.links.prev = INVALID;
		fresh.links.next = INVALID;

		_vacantHead = i;
		_vacantTail = i;
	}

	--_count;

	HSFM_IF_DEBUG(verifyStructure());
}

//------------------------------------------------------------------------------

template <typename TItem, LongIndex NCapacity>
TItem&
List<TItem, NCapacity>::operator[] (const Index i) {
	HSFM_IF_DEBUG(verifyStructure());

	return _cells[i].item;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TItem, LongIndex NCapacity>
const TItem&
List<TItem, NCapacity>::operator[] (const Index i) const {
	HSFM_IF_DEBUG(verifyStructure());

	return _cells[i].item;
}

//------------------------------------------------------------------------------

#ifdef _DEBUG

template <typename TItem, LongIndex NCapacity>
void
List<TItem, NCapacity>::verifyStructure(const Index occupied) const {
	if (_count < CAPACITY) {
		assert(_vacantHead < CAPACITY);
		assert(_vacantTail < CAPACITY);

		assert(_cells[_vacantHead].links.prev == INVALID);
		assert(_cells[_vacantTail].links.next == INVALID);

		auto emptyCount = 1;

		for (auto c = _vacantHead; c != _vacantTail; ) {
			assert(occupied != c);

			const auto& current = _cells[c];

			const auto f = current.links.next;
			if (f != INVALID) {
				// next
				const auto& following = _cells[f];

				assert(following.links.prev == c);

				c = f;
				continue;
			} else {
				// end
				assert(_vacantTail == c);
				assert(_count == CAPACITY - emptyCount);

				break;
			}
		}
	} else {
		assert(_vacantHead == INVALID);
		assert(_vacantTail == INVALID);
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template<LongIndex N>
struct IndexConstant {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<LongIndex... Ns>
struct IndexSequence {
	typedef IndexSequence<Ns...> Type;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<typename, typename>
struct MakeIndexSequence_Impl {};

template<LongIndex N, LongIndex... Ns>
struct MakeIndexSequence_Impl<IndexConstant<N>,
							  IndexSequence<Ns...>>
	: MakeIndexSequence_Impl<IndexConstant<N - 1>,
							 IndexSequence<N - 1, Ns...>>
{};

template<LongIndex... Ns>
struct MakeIndexSequence_Impl<IndexConstant<0>,
							  IndexSequence<Ns...>>
	: IndexSequence<Ns...>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<LongIndex N>
using MakeIndexSequence = typename MakeIndexSequence_Impl<IndexConstant<N>,
														  IndexSequence<>>::Type;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<class... Ts>
using IndexSequenceFor = MakeIndexSequence<sizeof...(Ts)>;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct TypeList_EntryT {};

template <typename T, std::size_t N>
struct TypeList_EntryN {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
struct TypeList_Impl;

template <LongIndex... Ns, typename... Ts>
struct TypeList_Impl<IndexSequence<Ns...>, Ts...>
	: TypeList_EntryT<Ts>...
	, TypeList_EntryN<Ts, Ns>...
{
	template <typename T, std::size_t N>
	static constexpr LongIndex select(TypeList_EntryN<T, N>) { return (LongIndex) N; }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
class VariantT;

template <typename... Ts>
struct _TL
	: private TypeList_Impl<IndexSequenceFor<Ts...>, Ts...>
{
	using Base = TypeList_Impl<IndexSequenceFor<Ts...>, Ts...>;
	using Container = VariantT<Ts...>;

	static constexpr LongIndex SIZE = sizeof...(Ts);

	template <typename T>
	static constexpr bool contains() { return std::is_base_of<TypeList_EntryT<T>, _TL>::value; }

	template <typename T>
	static constexpr
	typename std::enable_if< std::is_base_of<TypeList_EntryT<T>, _TL>::value, LongIndex>::type
	index() {
		return Base::template select<T>(_TL{});
	}

	template <typename T>
	static constexpr
	typename std::enable_if<!std::is_base_of<TypeList_EntryT<T>, _TL>::value, LongIndex>::type
	index() {
		return INVALID_LONG_INDEX;
	}
};

template <>
struct _TL<>
	: _TL<void>
{};

//------------------------------------------------------------------------------

template <typename...>
struct MergeT;

template <typename... Ts1, typename... Ts2>
struct MergeT<_TL<Ts1...>, _TL<Ts2...>> {
	using TypeList = _TL<Ts1..., Ts2...>;
};

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

// TODO: add assignment op and templated type conversion op

template <typename... Ts>
class alignas(alignof(void*)) VariantT {
	using Types = _TL<Ts...>;

public:
	template <typename T>
	static constexpr LongIndex index() {
		return Types::template index<T>();
	}

	template <typename T>
	static constexpr bool contains() {
		return Types::template contains<T>();
	}

	inline VariantT() = default;

	template <typename T,
			  typename = typename std::enable_if<contains<T>()>::type>
	inline VariantT(T* const p)
		: _pointer(p)
		, _index(index<T>())
	{
		assert(_index != INVALID_LONG_INDEX);
	}

	inline explicit operator bool() const { return _index != INVALID_LONG_INDEX; }

	inline void reset() {
		_pointer = nullptr;
		_index = INVALID_LONG_INDEX;
	}

	template <typename T>
	inline typename std::enable_if<contains<T>(), T>::type*
	get() const {
		const auto INDEX = index<T>();

		return INDEX == _index ?
			reinterpret_cast<T*>(_pointer) : nullptr;
	}

private:
	void* _pointer = nullptr;
	LongIndex _index = INVALID_LONG_INDEX;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}


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

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG

namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

struct LoggerInterface {
	using Method	 = ::hfsm::Method;
	using Transition = ::hfsm::Transition;

	virtual
	void
	recordMethod(const StateID origin,
				 const Method method) = 0;

	virtual
	void
	recordTransition(const StateID origin,
					 const Transition transition,
					 const StateID target) = 0;
};

////////////////////////////////////////////////////////////////////////////////

}

#else

namespace hfsm {

using LoggerInterface = void;

}

#endif

//------------------------------------------------------------------------------

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	#define HFSM_IF_LOGGER(...)		__VA_ARGS__
	#define HFSM_LOGGER_OR(y, n)	y
#else
	#define HFSM_IF_LOGGER(...)
	#define HFSM_LOGGER_OR(y, n)	n
#endif

#if defined HFSM_FORCE_DEBUG_LOG
	#define HFSM_LOG_STATE_METHOD(METHOD, ID)									\
		if (auto* const logger = control.logger())								\
			logger->recordMethod(STATE_ID, ID);
	#define HFSM_LOG_PLANNER_METHOD(METHOD, ID)									\
		if (auto* const logger = control.logger())								\
			logger->recordMethod(STATE_ID, ID);
#elif defined HFSM_ENABLE_LOG_INTERFACE
	#define HFSM_LOG_STATE_METHOD(METHOD, ID)									\
		if (auto* const logger = control.logger())								\
			log<decltype(METHOD), ID>(*logger);
	#define HFSM_LOG_PLANNER_METHOD(METHOD, ID)									\
		if (auto* const logger = control.logger())								\
			State::template log<decltype(METHOD), ID>(*logger);
#else
	#define HFSM_LOG_STATE_METHOD(METHOD, ID)
	#define HFSM_LOG_PLANNER_METHOD(METHOD, ID)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	#define HFSM_IF_STRUCTURE(...)	__VA_ARGS__
#else
	#define HFSM_IF_STRUCTURE(...)
#endif

//------------------------------------------------------------------------------

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct Status {
	bool success = false;
	bool failure = false;
	bool innerTransition = false;
	bool outerTransition = false;

	inline Status(const bool success_ = false,
				  const bool failure_ = false,
				  const bool innerTransition_ = false,
				  const bool outerTransition_ = false);

	inline explicit operator bool() const { return success || failure || innerTransition || outerTransition; }

	inline void clear();
};

inline Status
combine(const Status lhs, const Status rhs) {
	return Status{lhs.success || rhs.success,
				  lhs.failure || rhs.failure,
				  lhs.innerTransition || rhs.innerTransition,
				  lhs.outerTransition || rhs.outerTransition};
}

#pragma pack(pop)

//------------------------------------------------------------------------------

#pragma pack(push, 2)

struct TaskIndices {
	LongIndex first = INVALID_LONG_INDEX;
	LongIndex last  = INVALID_LONG_INDEX;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct TaskTransition {
	//using PayloadList	= TPayloadList;
	//using Payload		= typename PayloadList::Container;

	inline TaskTransition(const StateID origin_,
						  const StateID destination_)
		: origin(origin_)
		, destination(destination_)
		, next(INVALID_LONG_INDEX)
	{}

	// TODO: add payload
	StateID origin		= INVALID_STATE_ID;
	StateID destination	= INVALID_STATE_ID;

	LongIndex prev		= INVALID_LONG_INDEX;
	LongIndex next		= INVALID_LONG_INDEX;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

template <typename, typename, LongIndex, LongIndex>
class PlanControlT;

template <typename, typename, LongIndex, typename, LongIndex>
class FullControlT;

template <typename TStateList, LongIndex NPlanCapacity>
class PlanT {
	template <typename, typename, LongIndex, LongIndex>
	friend class PlanControlT;

	template <typename, typename, LongIndex, typename, LongIndex>
	friend class FullControlT;

	using StateList			= TStateList;

	static constexpr LongIndex CAPACITY		= NPlanCapacity;
	static constexpr LongIndex STATE_COUNT	= StateList::SIZE;

	using Tasks				= List<TaskTransition, CAPACITY>;
	using StateTasks		= Array<TaskIndices,   STATE_COUNT>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	struct Iterator {
		inline Iterator(PlanT& plan);

		inline explicit operator bool() const;

		inline void operator ++();

		inline		 TaskTransition& operator  *()		 { return  _plan._tasks[_curr]; }
		inline const TaskTransition& operator  *() const { return  _plan._tasks[_curr]; }

		inline		 TaskTransition* operator ->()		 { return &_plan._tasks[_curr]; }
		inline const TaskTransition* operator ->() const { return &_plan._tasks[_curr]; }

		inline void remove();

		inline LongIndex next() const;

		PlanT& _plan;
		LongIndex _curr;
		LongIndex _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	inline PlanT(Tasks& tasks,
				 StateTasks& stateTasks,
				 const StateID plannerId);

	template <typename T>
	static constexpr LongIndex
	stateId()	{ return StateList::template index<T>();	}

public:
	inline explicit operator bool() const;

	inline void clear();

	void add(const StateID origin, const StateID destination);

	template <typename TOrigin, typename TDestination>
	inline void add();

	void remove(const LongIndex task);

	inline Iterator begin()	{ return Iterator{*this}; }

private:
	Tasks& _tasks;
	TaskIndices& _taskIndices;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

Status::Status(const bool success_,
			   const bool failure_,
			   const bool innerTransition_,
			   const bool outerTransition_)
		: success{success_}
		, failure{failure_}
		, innerTransition{innerTransition_}
		, outerTransition{outerTransition_}
	{}

//------------------------------------------------------------------------------

void
Status::clear() {
	success = false;
	failure = false;
	innerTransition = false;
	outerTransition = false;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TStateList, LongIndex NPlanCapacity>
PlanT<TStateList, NPlanCapacity>::Iterator::Iterator(PlanT& plan)
	: _plan{plan}
	, _curr{plan._taskIndices.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TStateList, LongIndex NPlanCapacity>
PlanT<TStateList, NPlanCapacity>::Iterator::operator bool() const {
	assert(_curr < PlanT::CAPACITY || _curr == INVALID_LONG_INDEX);

	return _curr < PlanT::CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TStateList, LongIndex NPlanCapacity>
void
PlanT<TStateList, NPlanCapacity>::Iterator::operator ++() {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TStateList, LongIndex NPlanCapacity>
void
PlanT<TStateList, NPlanCapacity>::Iterator::remove() {
	_plan.remove(_curr);
}

//------------------------------------------------------------------------------

template <typename TStateList, LongIndex NPlanCapacity>
LongIndex
PlanT<TStateList, NPlanCapacity>::Iterator::next() const {
	if (_curr < PlanT::CAPACITY) {
		const TaskTransition& task = _plan._tasks[_curr];

		return task.next;
	} else {
		assert(_curr == INVALID_LONG_INDEX);

		return INVALID_LONG_INDEX;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TStateList, LongIndex NPlanCapacity>
PlanT<TStateList, NPlanCapacity>::PlanT(Tasks& tasks,
										StateTasks& stateTasks,
										const StateID plannerId)
	: _tasks{tasks}
	, _taskIndices{stateTasks[plannerId]}
{}

//------------------------------------------------------------------------------

template <typename TStateList, LongIndex NPlanCapacity>
PlanT<TStateList, NPlanCapacity>::operator bool() const {
	if (_taskIndices.first < PlanT::CAPACITY) {
		assert(_taskIndices.last < PlanT::CAPACITY);
		return true;
	} else {
		assert(_taskIndices.last == INVALID_LONG_INDEX);
		return false;
	}
}

//------------------------------------------------------------------------------

template <typename TStateList, LongIndex NPlanCapacity>
void
PlanT<TStateList, NPlanCapacity>::clear() {
	if (_taskIndices.first < Tasks::CAPACITY) {
		assert(_taskIndices.last < Tasks::CAPACITY);

		for (LongIndex index = _taskIndices.first;
			 index != INVALID_LONG_INDEX;
			 )
		{
			assert(index < Tasks::CAPACITY);

			const auto& task = _tasks[index];
			assert(index == _taskIndices.first ?
				   task.prev == INVALID_LONG_INDEX :
				   task.prev <  Tasks::CAPACITY);

			const LongIndex next = task.next;

			_tasks.remove(index);

			index = next;
		}

		_taskIndices.first = INVALID_LONG_INDEX;
		_taskIndices.last  = INVALID_LONG_INDEX;
	} else
		assert(_taskIndices.first == INVALID_LONG_INDEX &&
			   _taskIndices.last  == INVALID_LONG_INDEX);
}

//------------------------------------------------------------------------------

template <typename TStateList, LongIndex NPlanCapacity>
void
PlanT<TStateList, NPlanCapacity>::add(const StateID origin,
									  const StateID destination)
{
	const auto index = _tasks.emplace(origin, destination);

	if (_taskIndices.first < Tasks::CAPACITY) {
		assert(_taskIndices.last < Tasks::CAPACITY);

		auto& last  = _tasks[_taskIndices.last];
		last.next = index;

		auto& next = _tasks[index];
		next.prev  = _taskIndices.last;

		_taskIndices.last = index;
	} else {
		assert(_taskIndices.first == INVALID_LONG_INDEX &&
			   _taskIndices.last  == INVALID_LONG_INDEX);

		_taskIndices.first = index;
		_taskIndices.last  = index;
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TStateList, LongIndex NPlanCapacity>
template <typename TOrigin, typename TDestination>
void
PlanT<TStateList, NPlanCapacity>::add() {
	using Origin = TOrigin;
	using Destination = TDestination;

	add(stateId<Origin>(), stateId<Destination>());
}

//------------------------------------------------------------------------------

template <typename TStateList, LongIndex NPlanCapacity>
void
PlanT<TStateList, NPlanCapacity>::remove(const LongIndex task) {
	assert(_taskIndices.first < Tasks::CAPACITY &&
		   _taskIndices.last  < Tasks::CAPACITY);

	assert(task < Tasks::CAPACITY);

	const TaskTransition& curr = _tasks[task];

	if (curr.prev < Tasks::CAPACITY) {
		TaskTransition& prev = _tasks[curr.prev];
		prev.next = curr.next;
	} else {
		assert(_taskIndices.first == task);
		_taskIndices.first = curr.next;
	}

	if (curr.next < Tasks::CAPACITY) {
		TaskTransition& next = _tasks[curr.next];
		next.prev = curr.prev;
	} else {
		assert(_taskIndices.last == task);
		_taskIndices.last = curr.prev;
	}

	_tasks.remove(task);
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct alignas(2 * sizeof(ShortIndex)) Parent {
	inline Parent() = default;

	inline Parent(const ShortIndex fork_,
				  const ShortIndex prong_)
		: fork(fork_)
		, prong(prong_)
	{}

	inline explicit operator bool() const {
		return fork  != INVALID_SHORT_INDEX &&
			   prong != INVALID_SHORT_INDEX;
	}

	ShortIndex fork  = INVALID_SHORT_INDEX;
	ShortIndex prong = INVALID_SHORT_INDEX;
};

#pragma pack(pop)

using Parents = ArrayView<Parent>;

////////////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG
#pragma pack(push, 1)
#endif

struct HSFM_UNLESS_DEBUG(alignas(4 * sizeof(ShortIndex))) Fork {
	inline Fork(const ShortIndex self_,
				const Parent parent,
				Parents& forkParents,
				const ShortIndex default_
				HSFM_IF_DEBUG(, std::type_index type))
		: self(self_)
		, active(default_)
		, resumable(default_)
		HSFM_IF_DEBUG(, TYPE{type})
	{
		forkParents[self_] = parent;
	}

	ShortIndex self;
	ShortIndex active;
	ShortIndex resumable;
	ShortIndex requested = INVALID_SHORT_INDEX;
	HSFM_IF_DEBUG(const std::type_index TYPE);
};

#ifndef _DEBUG
#pragma pack(pop)
#endif

using ForkPointers = ArrayView<Fork*>;

////////////////////////////////////////////////////////////////////////////////

template <LongIndex NStateCount>
using ParentsT = Array<Parent, NStateCount>;

template <LongIndex NForkCount>
using ForkPointersT = Array<Fork*, NForkCount>;

template <LongIndex NStateCount, LongIndex NForkCount>
struct RegistryT {
	static constexpr LongIndex STATE_COUNT = NStateCount;
	static constexpr LongIndex FORK_COUNT  = NForkCount;

	using StateParents = ParentsT<STATE_COUNT>;
	using ForkParents  = ParentsT<FORK_COUNT>;
	using ForkPointers = ForkPointersT<FORK_COUNT>;

	StateParents stateParents;
	ForkParents  forkParents;
	ForkPointers forkPointers;
};

//------------------------------------------------------------------------------

template <LongIndex NStateCount, LongIndex NForkCount>
bool isActive(const RegistryT<NStateCount, NForkCount>& registry,
			  const StateID stateId);

template <LongIndex NStateCount, LongIndex NForkCount>
bool isResumable(const RegistryT<NStateCount, NForkCount>& registry,
				 const StateID stateId);

template <LongIndex NStateCount, LongIndex NForkCount>
inline
bool isScheduled(const RegistryT<NStateCount, NForkCount>& registry,
				 const StateID stateId)
{
	return isResumable(registry, stateId);
}

////////////////////////////////////////////////////////////////////////////////

}
}


namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <LongIndex NStateCount, LongIndex NForkCount>
bool
isActive(const RegistryT<NStateCount, NForkCount>& registry,
		 const StateID stateId)
{
	constexpr LongIndex STATE_COUNT = NStateCount;
	assert(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (auto parent = registry.stateParents[stateId];
			 parent;
			 parent = registry.forkParents[parent.fork])
		{
			const auto& fork = *registry.forkPointers[parent.fork];

			if (fork.active != STATE_COUNT)
				return parent.prong == fork.active;
		}

	return false;
}

//------------------------------------------------------------------------------

template <LongIndex NStateCount, LongIndex NForkCount>
bool
isResumable(const RegistryT<NStateCount, NForkCount>& registry,
			const StateID stateId)
{
	constexpr LongIndex STATE_COUNT = NStateCount;
	assert(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (auto parent = registry.stateParents[stateId];
			 parent;
			 parent = registry.forkParents[parent.fork])
		{
			const auto& fork = *registry.forkPointers[parent.fork];

			if (fork.active != STATE_COUNT)
				return parent.prong == fork.resumable;
		}

	return false;
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename, typename, LongIndex>
struct ControlOriginT;

template <typename TContext, typename TStateList, LongIndex NForkCount>
class ControlT {
	template <StateID, typename, typename>
	friend struct _S;

	template <StateID, ForkID, ForkID, typename, typename, typename...>
	friend struct _C;

	template <StateID, ForkID, ForkID, typename, typename, typename...>
	friend struct _Q;

	template <StateID, ForkID, ForkID, typename, typename, typename...>
	friend struct _O;

	template <typename, typename, typename, typename>
	friend class _R;

	template <typename, typename, LongIndex>
	friend struct ControlOriginT;

	using Context		= TContext;
	using StateList		= TStateList;

protected:
	static constexpr LongIndex STATE_COUNT	= StateList::SIZE;
	static constexpr LongIndex FORK_COUNT	= NForkCount;

	using Registry		= RegistryT<STATE_COUNT, FORK_COUNT>;

	inline ControlT(Context& context,
					const Registry& registry,
					LoggerInterface* const HFSM_IF_LOGGER(logger))
		: _context(context)
		, _registry(registry)
		HFSM_IF_LOGGER(, _logger(logger))
	{}

	inline void setOrigin  (const StateID id);
	inline void resetOrigin(const StateID id);

	template <typename T>
	static constexpr LongIndex
	stateId()												{ return StateList::template index<T>();			}

public:
	inline Context& _()										{ return _context;									}
	inline Context& context()								{ return _context;									}

	inline bool isActive   (const StateID stateId) const	{ return ::hfsm::detail::isActive	(_registry, stateId);	}
	inline bool isResumable(const StateID stateId) const	{ return ::hfsm::detail::isResumable(_registry, stateId);	}

	inline bool isScheduled(const StateID stateId) const	{ return isResumable(stateId);						}

	template <typename TState>
	inline bool isActive() const							{ return isActive	(stateId<TState>());			}

	template <typename TState>
	inline bool isResumable() const							{ return isResumable(stateId<TState>());			}

	template <typename TState>
	inline bool isScheduled() const							{ return isResumable(stateId<TState>());			}

private:
#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	inline LoggerInterface* logger()						{ return _logger;									}
#endif

protected:
	Context& _context;
	const Registry& _registry;
	StateID _originId = INVALID_STATE_ID;
	HFSM_IF_LOGGER(LoggerInterface* _logger);
};

//------------------------------------------------------------------------------

template <typename TContext, typename TStateList, LongIndex NForkCount>
struct ControlOriginT {
	using Context		= TContext;
	using StateList		= TStateList;

	using Control		= ControlT<Context, StateList, NForkCount>;

	inline ControlOriginT(Control& control,
						  const StateID id);

	inline ~ControlOriginT();

	Control& control;
	const StateID prevId;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TContext, typename TStateList, LongIndex NForkCount, LongIndex NPlanCapacity>
class PlanControlT final
	: public ControlT<TContext, TStateList, NForkCount>
{
	using Context			= TContext;
	using StateList			= TStateList;

	static constexpr LongIndex FORK_COUNT	 = NForkCount;
	static constexpr LongIndex PLAN_CAPACITY = NPlanCapacity;

public:
	using Control			= ControlT<Context, StateList, FORK_COUNT>;
	using Registry			= typename Control::Registry;

	using Plan				= PlanT<StateList, PLAN_CAPACITY>;
	using Tasks				= typename Plan::Tasks;
	using StateTasks		= typename Plan::StateTasks;

	template <typename, typename, typename, typename>
	friend class _R;

private:
	inline PlanControlT(Context& context,
						const Registry& registry,
						Tasks& tasks,
						StateTasks& stateTasks,
						LoggerInterface* const logger)
		: Control(context, registry, logger)
		, _tasks(tasks)
		, _stateTasks(stateTasks)
	{}

	template <typename T>
	static constexpr LongIndex
	stateId()										{ return StateList::template index<T>();					}

public:
	using Control::isActive;
	using Control::isResumable;
	using Control::isScheduled;

	inline Plan plan()								{ return Plan(_tasks, _stateTasks, _originId);				}
	inline Plan plan() const						{ return Plan(_tasks, _stateTasks, _originId);				}

	inline Plan plan(const StateID stateId)			{ return Plan(_tasks, _stateTasks, stateId);				}
	inline Plan plan(const StateID stateId) const	{ return Plan(_tasks, _stateTasks, stateId);				}

	template <typename TPlanner>
	inline Plan plan()								{ return Plan(_tasks, _stateTasks, stateId<TPlanner>());	}

	template <typename TPlanner>
	inline Plan plan() const						{ return Plan(_tasks, _stateTasks, stateId<TPlanner>());	}

private:
	using Control::_originId;

	Tasks& _tasks;
	StateTasks& _stateTasks;
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

	template <typename T>
	static constexpr bool contains() {
		return PayloadList::template contains<T>();
	}

	inline TransitionT() = default;

	inline TransitionT(const Type type_,
					   const StateID stateId_)
		: type(type_)
		, stateId(stateId_)
	{
		assert(type_ < Type::COUNT);
	}

	template <typename T,
			  typename = typename std::enable_if<contains<T>(), T>::type>
	inline TransitionT(const Type type_,
					   const StateID stateId_,
					   T* const payload_)
		: type(type_)
		, stateId(stateId_)
		, payload(payload_)
	{
		assert(type_ < Type::COUNT);
	}

	Type type = RESTART;
	StateID stateId = INVALID_STATE_ID;
	Payload payload;
};

template <typename TPayloadList>
using TransitionQueueT = ArrayView<TransitionT<TPayloadList>>;

//------------------------------------------------------------------------------

template <typename, typename, LongIndex, typename>
struct ControlLockT;

template <typename, typename, LongIndex, typename>
struct ControlRegionT;

template <typename TContext, typename TStateList, LongIndex NForkCount, typename TPayloadList>
class TransitionControlT
	: public ControlT<TContext, TStateList, NForkCount>
{
protected:
	using Context			= TContext;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;

	static constexpr LongIndex FORK_COUNT	 = NForkCount;

	using Control			= ControlT<Context, StateList, FORK_COUNT>;
	using Registry			= typename Control::Registry;

	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using TransitionQueue	= TransitionQueueT<TPayloadList>;

	template <StateID, typename, typename>
	friend struct _S;

	template <typename, typename, typename, typename>
	friend class _R;

	template <typename, typename, LongIndex, typename>
	friend struct ControlLockT;

	template <typename, typename, LongIndex, typename>
	friend struct ControlRegionT;

protected:
	inline TransitionControlT(Context& context,
							  const Registry& registry,
							  TransitionQueue& requests,
							  LoggerInterface* const logger)
		: Control(context, registry, logger)
		, _requests(requests)
	{}

public:
	using Control::isActive;
	using Control::isResumable;
	using Control::isScheduled;

	template <typename T>
	static constexpr LongIndex
	stateId()						{ return StateList::template index<T>();	}

	inline void changeTo(const StateID stateId);
	inline void resume	(const StateID stateId);
	inline void schedule(const StateID stateId);

	template <typename TState>
	inline void changeTo()						{ changeTo(stateId<TState>());	}

	template <typename TState>
	inline void resume()						{ resume  (stateId<TState>());	}

	template <typename TState>
	inline void schedule()						{ schedule(stateId<TState>());	}

	inline void succeed()						{ _status.success = true;		}
	inline void fail()							{ _status.failure = true;		}

private:
	inline void setRegion  (const StateID id, const LongIndex size);
	inline void resetRegion(const StateID id, const LongIndex size);

protected:
	using Control::_originId;

	TransitionQueue& _requests;
	Status _status;
	bool _locked = false;
	StateID _regionId = INVALID_STATE_ID;
	LongIndex _regionSize = INVALID_LONG_INDEX;
};

//------------------------------------------------------------------------------

template <typename TContext, typename TStateList, LongIndex NForkCount, typename TPayloadList>
struct ControlLockT {
	using Context			= TContext;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;

	static constexpr LongIndex FORK_COUNT	 = NForkCount;

	using TransitionControl = TransitionControlT<Context, StateList, FORK_COUNT, PayloadList>;

	inline ControlLockT(TransitionControl& control);
	inline ~ControlLockT();

	TransitionControl* const control;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TContext, typename TStateList, LongIndex NForkCount, typename TPayloadList>
struct ControlRegionT {
	using Context			= TContext;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;

	static constexpr LongIndex FORK_COUNT	 = NForkCount;

	using TransitionControl = TransitionControlT<Context, StateList, FORK_COUNT, PayloadList>;

	inline ControlRegionT(TransitionControl& control,
						  const StateID id,
						  const LongIndex size);

	inline ~ControlRegionT();

	TransitionControl& control;
	const StateID prevId;
	const LongIndex prevSize;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TStateList,
		  LongIndex NForkCount,
		  typename TPayloadList,
		  LongIndex NPlanCapacity>
class FullControlT final
	: public TransitionControlT<TContext, TStateList, NForkCount, TPayloadList>
{
	using Context			= TContext;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;

	static constexpr LongIndex FORK_COUNT		= NForkCount;
	static constexpr LongIndex PLAN_CAPACITY	= NPlanCapacity;

	using Control			= ControlT<Context, StateList, FORK_COUNT>;
	using Registry			= typename Control::Registry;

	using TransitionControl	= TransitionControlT<Context, StateList, FORK_COUNT, PayloadList>;
	using TransitionQueue	= typename TransitionControl::TransitionQueue;
	using Plan				= PlanT<StateList, PLAN_CAPACITY>;
	using Tasks				= typename Plan::Tasks;
	using StateTasks		= typename Plan::StateTasks;

	template <typename, typename, typename, typename>
	friend class _R;

private:
	inline FullControlT(Context& context,
						const Registry& registry,
						TransitionQueue& requests,
						Tasks& tasks,
						StateTasks& firstTasks,
						LoggerInterface* const logger)
		: TransitionControl{context, registry, requests, logger}
		, _tasks{tasks}
		, _stateTasks{firstTasks}
	{}

	template <typename T>
	static constexpr LongIndex
	stateId()										{ return StateList::template index<T>();					}

public:
	using Control::isActive;
	using Control::isResumable;
	using Control::isScheduled;

	inline Plan plan()								{ return Plan(_tasks, _stateTasks, _originId);				}
	inline Plan plan() const						{ return Plan(_tasks, _stateTasks, _originId);				}

	inline Plan plan(const StateID stateId)			{ return Plan(_tasks, _stateTasks, stateId);				}
	inline Plan plan(const StateID stateId) const	{ return Plan(_tasks, _stateTasks, stateId);				}

	template <typename TPlanner>
	inline Plan plan()								{ return Plan(_tasks, _stateTasks, stateId<TPlanner>());	}

	template <typename TPlanner>
	inline Plan plan() const						{ return Plan(_tasks, _stateTasks, stateId<TPlanner>());	}

private:
	using Control::_originId;

	Tasks& _tasks;
	StateTasks& _stateTasks;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TSL, LongIndex NFC>
void
ControlT<TC, TSL, NFC>::setOrigin(const StateID id) {
	//assert(_regionId != INVALID_STATE_ID && _regionSize != INVALID_LONG_INDEX);
	//assert(_regionId < StateList::SIZE && _regionId + _regionSize <= StateList::SIZE);

	assert(id != INVALID_STATE_ID);

	_originId = id;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TSL, LongIndex NFC>
void
ControlT<TC, TSL, NFC>::resetOrigin(const StateID id) {
	//assert(_regionId != INVALID_STATE_ID && _regionSize != INVALID_LONG_INDEX);
	//assert(_regionId < StateList::SIZE && _regionId + _regionSize <= StateList::SIZE);

	assert(_originId != INVALID_STATE_ID);

	_originId = id;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TSL, LongIndex NFC>
ControlOriginT<TC, TSL, NFC>::ControlOriginT(Control& control_,
											 const StateID id)
	: control{control_}
	, prevId(control._originId)
{
	control.setOrigin(id);
}

//------------------------------------------------------------------------------

template <typename TC, typename TSL, LongIndex NFC>
ControlOriginT<TC, TSL, NFC>::~ControlOriginT() {
	control.resetOrigin(prevId);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TSL, LongIndex NFC, typename TPL>
void
TransitionControlT<TC, TSL, NFC, TPL>::changeTo(const StateID stateId) {
	if (!_locked) {
		const Transition transition{TransitionType::RESTART, stateId};
		_requests << transition;

		if (_regionId <= stateId && stateId < _regionId + _regionSize)
			_status.innerTransition = true;
		else
			_status.outerTransition = true;

	#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
		if (Control::_logger)
			Control::_logger->recordTransition(Control::_originId, ::hfsm::Transition::RESTART, stateId);
	#endif
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TSL, LongIndex NFC, typename TPL>
void
TransitionControlT<TC, TSL, NFC, TPL>::resume(const StateID stateId) {
	if (!_locked) {
		const Transition transition{TransitionType::RESUME, stateId};
		_requests << transition;

		if (_regionId <= stateId && stateId < _regionId + _regionSize)
			_status.innerTransition = true;
		else
			_status.outerTransition = true;

	#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
		if (Control::_logger)
			Control::_logger->recordTransition(Control::_originId, ::hfsm::Transition::RESUME, stateId);
	#endif
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TSL, LongIndex NFC, typename TPL>
void
TransitionControlT<TC, TSL, NFC, TPL>::schedule(const StateID stateId) {
	const Transition transition{TransitionType::SCHEDULE, stateId};
	_requests << transition;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	if (Control::_logger)
		Control::_logger->recordTransition(Control::_originId, ::hfsm::Transition::SCHEDULE, stateId);
#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TSL, LongIndex NFC, typename TPL>
void
TransitionControlT<TC, TSL, NFC, TPL>::setRegion(const StateID id,
												 const LongIndex size)
{
	if (_regionId == INVALID_STATE_ID) {
		assert(_regionSize == INVALID_LONG_INDEX);
		assert(id < StateList::SIZE && id + size <= StateList::SIZE);
	} else {
		assert(_regionSize != INVALID_LONG_INDEX);
		assert(_regionId < id && id + size <= _regionId + _regionSize);
	}

	assert(id != INVALID_STATE_ID && size != INVALID_LONG_INDEX);

	assert(Control::_originId == INVALID_STATE_ID);

	_regionId	= id;
	_regionSize	= size;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TSL, LongIndex NFC, typename TPL>
void
TransitionControlT<TC, TSL, NFC, TPL>::resetRegion(const StateID id,
												   const LongIndex size)
{
	assert(_regionId != INVALID_STATE_ID && _regionSize != INVALID_LONG_INDEX);

	if (id == INVALID_STATE_ID)
		assert(size == INVALID_LONG_INDEX);
	else
		assert(size != INVALID_LONG_INDEX);

	_regionId	= id;
	_regionSize	= size;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TSL, LongIndex NFC, typename TPL>
ControlLockT<TC, TSL, NFC, TPL>::ControlLockT(TransitionControl& control_)
	: control(!control_._locked ? &control_ : nullptr)
{
	if (control)
		control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TC, typename TSL, LongIndex NFC, typename TPL>
ControlLockT<TC, TSL, NFC, TPL>::~ControlLockT() {
	if (control)
		control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TSL, LongIndex NFC, typename TPL>
ControlRegionT<TC, TSL, NFC, TPL>::ControlRegionT(TransitionControl& control_,
												  const StateID id,
												  const LongIndex size)
	: control{control_}
	, prevId(control._regionId)
	, prevSize(control._regionSize)
{
	control.setRegion(id, size);
}

//------------------------------------------------------------------------------

template <typename TC, typename TSL, LongIndex NFC, typename TPL>
ControlRegionT<TC, TSL, NFC, TPL>::~ControlRegionT() {
	control.resetRegion(prevId, prevSize);

	control._status.clear();
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

struct alignas(alignof(void*)) StructureStateInfo {
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
	{}

	const char* name;
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

#pragma pack(push, 1)

template <typename TPayloadList>
struct alignas(4) TransitionInfoT {
	using TransitionPayloads = TPayloadList;
	using Transition = TransitionT<TPayloadList>;

	inline TransitionInfoT() = default;

	inline TransitionInfoT(const Transition transition_,
						   const ::hfsm::Method method_)
		: stateId{transition_.stateId}
		, method(method_)
		, transition(get<TransitionPayloads>(transition_.type))
	{
		assert(method_ < ::hfsm::Method::COUNT);
	}

	StateID stateId = INVALID_STATE_ID;
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

template <typename TContext,
		  typename TStateList,
		  LongIndex NForkCount,
		  typename TPayloadList,
		  LongIndex NPlanCapacity>
class Bare {
	template <typename...>
	friend struct _B;

protected:
	using Context			= TContext;
	using StateList			= TStateList;

	static constexpr LongIndex STATE_COUNT	 = StateList::SIZE;
	static constexpr LongIndex FORK_COUNT	 = NForkCount;
	static constexpr LongIndex PLAN_CAPACITY = NPlanCapacity;

	using Control			= ControlT			<Context, StateList, FORK_COUNT>;
	using PlanControl		= PlanControlT		<Context, StateList, FORK_COUNT, PLAN_CAPACITY>;
	using Plan				= typename PlanControl::Plan;
	using PayloadList		= TPayloadList;
	using TransitionControl = TransitionControlT<Context, StateList, FORK_COUNT, PayloadList>;
	using FullControl		= FullControlT		<Context, StateList, FORK_COUNT, PayloadList, PLAN_CAPACITY>;

public:
	inline void preGuard (Context&)												{}
	inline void preEnter (Context&)												{}
	inline void preUpdate(Context&)												{}
	template <typename TEvent>
	inline void preReact (const TEvent&,
						  Context&)												{}
	inline void postExit (Context&)												{}
};

//------------------------------------------------------------------------------

template <typename...>
struct _B;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TFirst>
struct _B<TFirst>
	: TFirst
{
	using First				= TFirst;

	using StateList			= typename First::StateList;

	inline void guard		 (typename First::TransitionControl&)				{}
	inline void enter		 (typename First::Control&)							{}
	inline void update		 (typename First::TransitionControl&)				{}
	template <typename TEvent>
	inline void react		 (const TEvent&,
							  typename First::TransitionControl&)				{}
	inline void exit		 (typename First::Control&)							{}

	inline void widePreGuard (typename First::Context& context);
	inline void widePreEnter (typename First::Context& context);
	inline void widePreUpdate(typename First::Context& context);
	template <typename TEvent>
	inline void widePreReact (const TEvent& event,
							  typename First::Context& context);
	inline void widePostExit (typename First::Context& context);

	template <typename T>
	static constexpr LongIndex
	stateId()						{ return StateList::template index<T>();	}
};

template <typename TContext, typename TStateList, LongIndex NForkCount, typename TPayloadList, LongIndex NPlanCapacity>
using State = _B<Bare<TContext, TStateList, NForkCount, TPayloadList, NPlanCapacity>>;

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

namespace hfsm {

//------------------------------------------------------------------------------

template <LongIndex NMaxPlanTasks = INVALID_LONG_INDEX,
		  LongIndex NMaxSubstitutions = 4>
struct Config {
	static constexpr LongIndex MAX_PLAN_TASKS	 = NMaxPlanTasks;
	static constexpr LongIndex MAX_SUBSTITUTIONS = NMaxSubstitutions;
};

namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename>
struct _SF;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
struct _CSF;

template <typename TInitial, typename... TRemaining>
struct _CSF<TInitial, TRemaining...>;

template <typename TInitial>
struct _CSF<TInitial>;

template <typename, typename...>
struct _CF;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
struct _OSF;

template <typename TInitial, typename... TRemaining>
struct _OSF<TInitial, TRemaining...>;

template <typename TInitial>
struct _OSF<TInitial>;

template <typename, typename...>
struct _OF;

//------------------------------------------------------------------------------

template <typename...>
struct WrapForward;

template <typename TH>
struct WrapForward<TH> {
	using Type = _SF<TH>;
};

template <typename TH, typename... TS>
struct WrapForward<_CF<TH, TS...>> {
	using Type = _CF<TH, TS...>;
};

template <typename TH, typename... TS>
struct WrapForward<_OF<TH, TS...>> {
	using Type = _OF<TH, TS...>;
};

//------------------------------------------------------------------------------

template <typename THead>
struct _SF final {
	using Head				= THead;
	using StateList			= _TL<Head>;

	static constexpr LongIndex REVERSE_DEPTH	= 1;
	static constexpr LongIndex DEEP_WIDTH		= 0;
	static constexpr LongIndex STATE_COUNT		= 1;
	static constexpr LongIndex COMPOSITE_COUNT	= 0;
	static constexpr LongIndex ORTHOGONAL_COUNT	= 0;
	static constexpr LongIndex FORK_COUNT		= 0;
	static constexpr LongIndex PRONG_COUNT		= 0;
	static constexpr LongIndex WIDTH			= 1;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct _CSF<TInitial, TRemaining...> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using Remaining			= _CSF<TRemaining...>;
	using StateList			= typename MergeT<typename Initial::StateList, typename Remaining::StateList>::TypeList;

	static constexpr LongIndex REVERSE_DEPTH	= Max<Initial::REVERSE_DEPTH, Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr LongIndex DEEP_WIDTH		= Max<Initial::DEEP_WIDTH,	  Remaining::DEEP_WIDTH>::VALUE;
	static constexpr LongIndex STATE_COUNT		= Initial::STATE_COUNT		+ Remaining::STATE_COUNT;
	static constexpr LongIndex COMPOSITE_COUNT	= Initial::COMPOSITE_COUNT	+ Remaining::COMPOSITE_COUNT;
	static constexpr LongIndex ORTHOGONAL_COUNT	= Initial::ORTHOGONAL_COUNT + Remaining::ORTHOGONAL_COUNT;
	static constexpr LongIndex FORK_COUNT		= Initial::FORK_COUNT		+ Remaining::FORK_COUNT;
	static constexpr LongIndex PRONG_COUNT		= Initial::PRONG_COUNT		+ Remaining::PRONG_COUNT;
};

template <typename TInitial>
struct _CSF<TInitial> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using StateList			= typename Initial::StateList;

	static constexpr LongIndex REVERSE_DEPTH	= Initial::REVERSE_DEPTH;
	static constexpr LongIndex DEEP_WIDTH		= Max<1, Initial::DEEP_WIDTH>::VALUE;
	static constexpr LongIndex STATE_COUNT		= Initial::STATE_COUNT;
	static constexpr LongIndex COMPOSITE_COUNT	= Initial::COMPOSITE_COUNT;
	static constexpr LongIndex ORTHOGONAL_COUNT	= Initial::ORTHOGONAL_COUNT;
	static constexpr LongIndex FORK_COUNT		= Initial::FORK_COUNT;
	static constexpr LongIndex PRONG_COUNT		= Initial::PRONG_COUNT;
};

template <typename THead, typename... TSubStates>
struct _CF final {
	using Head				= THead;
	using State				= _SF<Head>;
	using SubStates			= _CSF<TSubStates...>;
	using StateList			= typename MergeT<typename State::StateList, typename SubStates::StateList>::TypeList;

	static constexpr LongIndex REVERSE_DEPTH	= SubStates::REVERSE_DEPTH + 1;
	static constexpr LongIndex DEEP_WIDTH		= SubStates::DEEP_WIDTH;
	static constexpr LongIndex STATE_COUNT		= State::STATE_COUNT + SubStates::STATE_COUNT;
	static constexpr LongIndex COMPOSITE_COUNT	= SubStates::COMPOSITE_COUNT + 1;
	static constexpr LongIndex ORTHOGONAL_COUNT	= SubStates::ORTHOGONAL_COUNT;
	static constexpr LongIndex FORK_COUNT		= SubStates::FORK_COUNT + 1;
	static constexpr LongIndex PRONG_COUNT		= SubStates::PRONG_COUNT + sizeof...(TSubStates);
	static constexpr LongIndex WIDTH			= sizeof...(TSubStates);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct _OSF<TInitial, TRemaining...> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using Remaining			= _OSF<TRemaining...>;
	using StateList			= typename MergeT<typename Initial::StateList, typename Remaining::StateList>::TypeList;

	static constexpr LongIndex REVERSE_DEPTH	= Max<Initial::REVERSE_DEPTH, Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr LongIndex DEEP_WIDTH		= Initial::DEEP_WIDTH		+ Remaining::DEEP_WIDTH;
	static constexpr LongIndex STATE_COUNT		= Initial::STATE_COUNT		+ Remaining::STATE_COUNT;
	static constexpr LongIndex COMPOSITE_COUNT	= Initial::COMPOSITE_COUNT	+ Remaining::COMPOSITE_COUNT;
	static constexpr LongIndex ORTHOGONAL_COUNT	= Initial::ORTHOGONAL_COUNT + Remaining::ORTHOGONAL_COUNT;
	static constexpr LongIndex FORK_COUNT		= Initial::FORK_COUNT		+ Remaining::FORK_COUNT;
	static constexpr LongIndex PRONG_COUNT		= Initial::PRONG_COUNT		+ Remaining::PRONG_COUNT;
};

template <typename TInitial>
struct _OSF<TInitial> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using StateList			= typename Initial::StateList;

	static constexpr LongIndex REVERSE_DEPTH	= Initial::REVERSE_DEPTH;
	static constexpr LongIndex DEEP_WIDTH		= Initial::DEEP_WIDTH;
	static constexpr LongIndex STATE_COUNT		= Initial::STATE_COUNT;
	static constexpr LongIndex COMPOSITE_COUNT	= Initial::COMPOSITE_COUNT;
	static constexpr LongIndex ORTHOGONAL_COUNT	= Initial::ORTHOGONAL_COUNT;
 	static constexpr LongIndex FORK_COUNT		= Initial::FORK_COUNT;
	static constexpr LongIndex PRONG_COUNT		= Initial::PRONG_COUNT;
};

template <typename THead, typename... TSubStates>
struct _OF final {
	using Head				= THead;
	using State				= _SF<Head>;
	using SubStates			= _OSF<TSubStates...>;
	using StateList			= typename MergeT<typename State::StateList, typename SubStates::StateList>::TypeList;

	static constexpr LongIndex REVERSE_DEPTH	= SubStates::REVERSE_DEPTH + 1;
	static constexpr LongIndex DEEP_WIDTH		= SubStates::DEEP_WIDTH;
	static constexpr LongIndex STATE_COUNT		= State::STATE_COUNT + SubStates::STATE_COUNT;
	static constexpr LongIndex COMPOSITE_COUNT	= SubStates::COMPOSITE_COUNT;
	static constexpr LongIndex ORTHOGONAL_COUNT	= SubStates::ORTHOGONAL_COUNT + 1;
	static constexpr LongIndex FORK_COUNT		= SubStates::FORK_COUNT + 1;
	static constexpr LongIndex PRONG_COUNT		= SubStates::PRONG_COUNT;
	static constexpr LongIndex WIDTH			= sizeof...(TSubStates);
};

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  LongIndex NForkCount,
		  typename TPayloadList,
		  LongIndex NPlanCapacity>
struct ArgsT final {
	using Context			= TContext;
	using Config			= TConfig;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;

	static constexpr LongIndex FORK_COUNT	 = NForkCount;
	static constexpr LongIndex PLAN_CAPACITY = NPlanCapacity;

	using Empty = _B<Bare<Context, StateList, FORK_COUNT, PayloadList, PLAN_CAPACITY>>;
};

//------------------------------------------------------------------------------

template <StateID, typename, typename>
struct _S;

template <StateID, ForkID, ForkID, typename, typename, typename...>
struct _C;

template <StateID, ForkID, ForkID, typename, typename, typename...>
struct _Q;

template <StateID, ForkID, ForkID, typename, typename, typename...>
struct _O;

template <typename, typename, typename, typename>
class _R;

//------------------------------------------------------------------------------

template <StateID, ForkID, ForkID, typename...>
struct WrapMaterial;

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH>
struct WrapMaterial<NS, NC, NO, TA, TH> {
	using Type = _S<NS,			TA, TH>;
};

template <StateID NS, ForkID NC, ForkID NO, typename TA,			  typename... TS>
struct WrapMaterial<NS, NC, NO, TA, _CF<void, TS...>> {
	using Type = _C<NS, NC, NO, TA, typename TA::Empty, TS...>;
};

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
struct WrapMaterial<NS, NC, NO, TA, _CF<TH, TS...>> {
	using Type = _Q<NS, NC, NO, TA, TH,					TS...>;
};

template <StateID NS, ForkID NC, ForkID NO, typename TA,			  typename... TS>
struct WrapMaterial<NS, NC, NO, TA, _OF<void, TS...>> {
	using Type = _O<NS, NC, NO, TA, typename TA::Empty, TS...>;
};

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
struct WrapMaterial<NS, NC, NO, TA, _OF<TH, TS...>> {
	using Type = _O<NS, NC, NO, TA, TH,					TS...>;
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TPayloadList,
		  typename TApex>
struct _RF final {
	using Context			= TContext;
	using Config			= TConfig;
	using PayloadList		= TPayloadList;
	using Forward			= TApex;

	static constexpr LongIndex MAX_PLAN_TASKS	 = Config::MAX_PLAN_TASKS;
	static constexpr LongIndex MAX_SUBSTITUTIONS = Config::MAX_SUBSTITUTIONS;

	static constexpr LongIndex PLAN_CAPACITY	 = Config::MAX_PLAN_TASKS != INVALID_LONG_INDEX ?
													   Config::MAX_PLAN_TASKS : Forward::PRONG_COUNT * 2;

	using Instance			= _R<Context, Config, PayloadList, Forward>;

	static constexpr LongIndex FORK_COUNT		 = Forward::FORK_COUNT;

	using StateList			= typename Forward::StateList;
	using PlanControl		= PlanControlT		   <Context, StateList, FORK_COUNT, PLAN_CAPACITY>;
	using TransitionControl	= TransitionControlT   <Context, StateList, FORK_COUNT, PayloadList>;
	using FullControl		= FullControlT		   <Context, StateList, FORK_COUNT, PayloadList, PLAN_CAPACITY>;

	using Bare				= ::hfsm::detail::Bare <Context, StateList, FORK_COUNT, PayloadList, PLAN_CAPACITY>;
	using State				= ::hfsm::detail::State<Context, StateList, FORK_COUNT, PayloadList, PLAN_CAPACITY>;

	template <typename... TInjections>
	using BaseT = _B<TInjections...>;

	template <typename T>
	static constexpr LongIndex stateId() {
		return StateList::template index<T>();
	}

	template <typename T>
	static constexpr bool contains() {
		return StateList::template index<T>() != INVALID_LONG_INDEX;
	}
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TConfig = Config<>,
		  typename TPayloadList = _TL<>>
struct _M {
	using Context = TContext;
	using Config  = TConfig;
	//using Control = ControlT<Context>;

	using PayloadList = TPayloadList;

	//----------------------------------------------------------------------

	template <typename THead, typename... TSubStates>
	using Composite			 = _CF<THead, TSubStates...>;

	template <				  typename... TSubStates>
	using CompositePeers	 = _CF<void,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using Orthogonal		 = _OF<THead, TSubStates...>;

	template <				  typename... TSubStates>
	using OrthogonalPeers	 = _OF<void,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using Root				 = _RF<Context, Config, PayloadList, Composite <THead, TSubStates...>>;

	template <				  typename... TSubStates>
	using PeerRoot			 = _RF<Context, Config, PayloadList, CompositePeers	<  TSubStates...>>;

	template <typename THead, typename... TSubStates>
	using OrthogonalRoot	 = _RF<Context, Config, PayloadList, Orthogonal<THead, TSubStates...>>;

	template <				  typename... TSubStates>
	using OrthogonalPeerRoot = _RF<Context, Config, PayloadList, OrthogonalPeers<  TSubStates...>>;

	//----------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////

}

template <typename... Ts>
using TransitionPayloads = detail::_TL<Ts...>;

//------------------------------------------------------------------------------

template <typename...>
struct Machine;

template <typename TContext>
struct Machine<TContext>
	: detail::_M<TContext>
{};

template <typename TContext, LongIndex... NConstants>
struct Machine<TContext, Config<NConstants...>>
	: detail::_M<TContext, Config<NConstants...>, TransitionPayloads<>>
{};

template <typename TContext, typename... TPayloads>
struct Machine<TContext, TransitionPayloads<TPayloads...>>
	: detail::_M<TContext, Config<>, TransitionPayloads<TPayloads...>>
{};

template <typename TContext, LongIndex... NConstants, typename... TPayloads>
struct Machine<TContext, Config<NConstants...>, TransitionPayloads<TPayloads...>>
	: detail::_M<TContext, Config<NConstants...>, TransitionPayloads<TPayloads...>>
{};

////////////////////////////////////////////////////////////////////////////////

}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TStateID,
		  typename TArgs,
		  typename THead>
struct _S {
	static constexpr StateID STATE_ID = TStateID;

	using Args				= TArgs;
	using Head				= THead;

	using Context			= typename Args::Context;
	using Config			= typename Args::Config;
	using StateList			= typename Args::StateList;
	using PayloadList		= typename Args::PayloadList;

	using StateParents		= Array<Parent, StateList::SIZE>;
	using Control			= ControlT			   <Context, StateList, Args::FORK_COUNT>;
	using ControlOrigin		= ControlOriginT	   <Context, StateList, Args::FORK_COUNT>;
	using PlanControl		= PlanControlT		   <Context, StateList, Args::FORK_COUNT, Args::PLAN_CAPACITY>;
	using Transition		= TransitionT		   <PayloadList>;
	using TransitionControl	= TransitionControlT   <Context, StateList, Args::FORK_COUNT, PayloadList>;
	using FullControl		= FullControlT		   <Context, StateList, Args::FORK_COUNT, PayloadList, Args::PLAN_CAPACITY>;

	using StateBase			= ::hfsm::detail::State<Context, StateList, Args::FORK_COUNT, PayloadList, Args::PLAN_CAPACITY>;

	_S(StateParents& stateParents,
	   const Parent parent,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

	inline void	  deepForwardGuard		(FullControl&)									{}
	inline bool	  deepGuard				(FullControl& control);

	inline void	  deepEnterInitial		(PlanControl& control);
	inline void	  deepEnter				(PlanControl& control);

	inline Status deepUpdate			(FullControl& control);

	template <typename TEvent>
	inline void	  deepReact				(const TEvent& event,
										 FullControl& control);

	inline void	  deepExit				(PlanControl& control);

	inline void   deepForwardRequest	(const typename Transition::Type)				{}
	inline void   deepRequestRemain		()												{}
	inline void   deepRequestRestart	()												{}
	inline void   deepRequestResume		()												{}
	inline void   deepChangeToRequested	(Control&)										{}

#if defined _DEBUG || defined HFSM_ENABLE_STRUCTURE_REPORT || defined HFSM_ENABLE_LOG_INTERFACE
	static constexpr bool isBare()	 { return std::is_same<Head, StateBase>::value;		 }

	static constexpr LongIndex NAME_COUNT = isBare() ? 0 : 1;
#endif

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using RegionType		= typename StructureStateInfo::RegionType;

	static const char* name();

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

#ifdef HFSM_ENABLE_LOG_INTERFACE
	template <typename>
	struct MemberTraits;

	template <typename TReturn, typename TState, typename... Ts>
	struct MemberTraits<TReturn(TState::*)(Ts...)> {
		using State = TState;
	};

	template <typename TMethodType, LoggerInterface::Method>
	typename std::enable_if< std::is_same<typename MemberTraits<TMethodType>::State, StateBase>::value>::type
	log(LoggerInterface&) const {}

	template <typename TMethodType, LoggerInterface::Method TMethodId>
	typename std::enable_if<!std::is_same<typename MemberTraits<TMethodType>::State, StateBase>::value>::type
	log(LoggerInterface& logger) const {
		logger.recordMethod(STATE_ID, TMethodId);
	}
#endif

	Head _head;
	HSFM_IF_DEBUG(const std::type_index TYPE = isBare() ? typeid(None) : typeid(Head));
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

//------------------------------------------------------------------------------

namespace {

template <StateID NS, typename TC, typename TSL, LongIndex NFC, typename TPL, LongIndex NPC, typename TH>
struct RegisterT {
	using StateParents = Array<Parent, TSL::SIZE>;

	static inline void
	execute(StateParents& stateParents, const Parent parent) {
		static constexpr auto TYPE_ID = TSL::template index<TH>();
		assertEquality<NS, TYPE_ID>();

		stateParents[NS] = parent;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, typename TC, typename TSL, LongIndex NFC, typename TPL, LongIndex NPC>
struct RegisterT<NS, TC, TSL, NFC, TPL, NPC, State<TC, TSL, NFC, TPL, NPC>> {
	using StateParents = Array<Parent, TSL::SIZE>;

	static inline void
	execute(StateParents&, const Parent) {}
};

}

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, typename TA, typename TH>
_S<NS, TA, TH>::_S(StateParents& stateParents,
					const Parent parent,
					Parents& /*forkParents*/,
					ForkPointers& /*forkPointers*/)
{
	using Register = RegisterT<STATE_ID,
							   Context,
							   StateList,
							   Args::FORK_COUNT,
							   PayloadList,
							   Args::PLAN_CAPACITY,
							   Head>;
	Register::execute(stateParents, parent);
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
bool
_S<NS, TA, TH>::deepGuard(FullControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::guard, ::hfsm::Method::GUARD);

	ControlOrigin origin{control, STATE_ID};

	const auto requestCountBefore = control._requests.count();

	_head.widePreGuard(control.context());
	_head.guard(control);

	return requestCountBefore < control._requests.count();
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::deepEnterInitial(PlanControl& control) {
	deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::deepEnter(PlanControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::enter, ::hfsm::Method::ENTER);

	ControlOrigin origin{control, STATE_ID};

	_head.widePreEnter(control.context());
	_head.enter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
Status
_S<NS, TA, TH>::deepUpdate(FullControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::update, ::hfsm::Method::UPDATE);

	ControlOrigin origin{control, STATE_ID};

	_head.widePreUpdate(control.context());
	_head.update(control);
	// TODO: _head.widePostUpdate(control.context());

	return control._status;
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
template <typename TEvent>
void
_S<NS, TA, TH>::deepReact(const TEvent& event,
						   FullControl& control)
{
	auto reaction = static_cast<void(Head::*)(const TEvent&, TransitionControl&)>(&Head::react);
	HFSM_LOG_STATE_METHOD(reaction, ::hfsm::Method::REACT);

	ControlOrigin origin{control, STATE_ID};

	_head.widePreReact(event, control.context());

	//_head.react(event, control);
	(_head.*reaction)(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::deepExit(PlanControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::exit, ::hfsm::Method::EXIT);

	ControlOrigin origin{control, STATE_ID};

	_head.exit(control);
	_head.widePostExit(control.context());
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, typename TA, typename TH>
const char*
_S<NS, TA, TH>::name() {
	if (isBare())
		return "";
	else {
		const std::type_index type = typeid(Head);
		const char* const raw = type.name();

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

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::deepGetNames(const LongIndex parent,
							  const RegionType region,
							  const ShortIndex depth,
							  StructureStateInfos& _stateInfos) const
{
	_stateInfos << StructureStateInfo { parent, region, depth, name() };
}

#endif

///////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID, ForkID, ForkID, typename, ShortIndex, typename...>
struct _CS;

//------------------------------------------------------------------------------

template <StateID NInitialID,
		  ForkID NCompoID,
		  ForkID NOrthoID,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct _CS<NInitialID, NCompoID, NOrthoID, TArgs, NIndex, TInitial, TRemaining...> {
	static constexpr StateID   INITIAL_ID	= NInitialID;
	static constexpr StateID   COMPO_ID		= NCompoID;
	static constexpr StateID   ORTHO_ID		= NOrthoID;
	static constexpr LongIndex PRONG_INDEX	= NIndex;

	using Args				= TArgs;

	using Context			= typename Args::Context;
	using Config			= typename Args::Config;
	using StateList			= typename Args::StateList;
	using PayloadList		= typename Args::PayloadList;

	using StateParents		= Array<Parent, StateList::SIZE>;
	using PlanControl		= PlanControlT<Context, StateList, Args::FORK_COUNT, Args::PLAN_CAPACITY>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using FullControl		= FullControlT<Context, StateList, Args::FORK_COUNT, PayloadList, Args::PLAN_CAPACITY>;

	using Initial			= typename WrapMaterial<INITIAL_ID, COMPO_ID, ORTHO_ID, Args, TInitial>::Type;
	using InitialForward	= typename WrapForward<TInitial>::Type;
	using Remaining			= _CS<INITIAL_ID + InitialForward::STATE_COUNT,
								  COMPO_ID + InitialForward::COMPOSITE_COUNT,
								  ORTHO_ID + InitialForward::ORTHOGONAL_COUNT,
								  Args, NIndex + 1, TRemaining...>;
	using Forward			= _CSF<TInitial, TRemaining...>;

	_CS(StateParents& stateParents,
		const ShortIndex fork,
		Parents& forkParents,
		ForkPointers& forkPointers);

	inline void   wideForwardGuard		(const ShortIndex prong, FullControl& control);
	inline void   wideGuard				(const ShortIndex prong, FullControl& control);

	inline void   wideEnterInitial		(						 PlanControl& control);
	inline void   wideEnter				(const ShortIndex prong, PlanControl& control);

	inline Status wideUpdate			(const ShortIndex prong, FullControl& control);

	template <typename TEvent>
	inline void   wideReact				(const ShortIndex prong, const TEvent& event,
																 FullControl& control);

	inline void   wideExit				(const ShortIndex prong, PlanControl& control);

	inline void   wideForwardRequest	(const ShortIndex prong, const TransitionType transition);
	inline void   wideRequestRemain		();
	inline void   wideRequestRestart	();
	inline void   wideRequestResume		(const ShortIndex prong);
	inline void   wideChangeToRequested	(const ShortIndex prong, PlanControl& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT  + Remaining::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	Initial initial;
	Remaining remaining;
};

//------------------------------------------------------------------------------

template <StateID NInitialID,
		  ForkID NCompoID,
		  ForkID NOrthoID,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename TInitial>
struct _CS<NInitialID, NCompoID, NOrthoID, TArgs, NIndex, TInitial> {
	static constexpr StateID   INITIAL_ID	= NInitialID;
	static constexpr StateID   COMPO_ID		= NCompoID;
	static constexpr StateID   ORTHO_ID		= NOrthoID;
	static constexpr LongIndex PRONG_INDEX	= NIndex;

	using Args				= TArgs;

	using Context			= typename Args::Context;
	using Config			= typename Args::Config;
	using StateList			= typename Args::StateList;
	using PayloadList		= typename Args::PayloadList;

	using StateParents		= Array<Parent, StateList::SIZE>;
	using PlanControl		= PlanControlT<Context, StateList, Args::FORK_COUNT, Args::PLAN_CAPACITY>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using FullControl		= FullControlT<Context, StateList, Args::FORK_COUNT, PayloadList, Args::PLAN_CAPACITY>;

	using Initial			= typename WrapMaterial<INITIAL_ID, COMPO_ID, ORTHO_ID, Args, TInitial>::Type;
	using Forward			= _CSF<TInitial>;

	_CS(StateParents& stateParents,
		const ShortIndex fork,
		Parents& forkParents,
		ForkPointers& forkPointers);

	inline void   wideForwardGuard		(const ShortIndex prong, FullControl& control);
	inline void   wideGuard				(const ShortIndex prong, FullControl& control);

	inline void   wideEnterInitial		(						 PlanControl& control);
	inline void   wideEnter				(const ShortIndex prong, PlanControl& control);

	inline Status wideUpdate			(const ShortIndex prong, FullControl& control);

	template <typename TEvent>
	inline void   wideReact				(const ShortIndex prong, const TEvent& event,
				  												 FullControl& control);

	inline void   wideExit				(const ShortIndex prong, PlanControl& control);

	inline void   wideForwardRequest	(const ShortIndex prong, const TransitionType transition);
	inline void   wideRequestRemain		();
	inline void   wideRequestRestart	();
	inline void   wideRequestResume		(const ShortIndex prong);
	inline void   wideChangeToRequested	(const ShortIndex prong, PlanControl& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	Initial initial;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
_CS<NS, NC, NO, TA, NI, TI, TR...>::_CS(StateParents& stateParents,
								 const ShortIndex fork,
								 Parents& forkParents,
								 ForkPointers& forkPointers)
	: initial(stateParents,
			  Parent(fork, PRONG_INDEX),
			  forkParents,
			  forkPointers)
	, remaining(stateParents,
				fork,
				forkParents,
				forkPointers)
{}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardGuard(const ShortIndex prong,
											  FullControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepForwardGuard(control);
	else
		remaining.wideForwardGuard(prong, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideGuard(const ShortIndex prong,
									   FullControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepGuard(control);
	else
		remaining.wideGuard(prong, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideEnterInitial(PlanControl& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideEnter(const ShortIndex prong,
									   PlanControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepEnter(control);
	else
		remaining.wideEnter(prong, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
Status
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideUpdate(const ShortIndex prong,
										FullControl& control)
{
	return prong == PRONG_INDEX ?
		initial  .deepUpdate(control) :
		remaining.wideUpdate(prong, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
template <typename TEvent>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideReact(const ShortIndex prong,
									   const TEvent& event,
									   FullControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepReact(	   event, control);
	else
		remaining.wideReact(prong, event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideExit(const ShortIndex prong,
									  PlanControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepExit(control);
	else
		remaining.wideExit(prong, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardRequest(const ShortIndex prong,
												const TransitionType transition)
{
	if (prong == PRONG_INDEX)
		initial	 .deepForwardRequest(		transition);
	else
		remaining.wideForwardRequest(prong, transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestResume(const ShortIndex prong) {
	if (prong == PRONG_INDEX)
		initial	 .deepRequestResume();
	else
		remaining.wideRequestResume(prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideChangeToRequested(const ShortIndex prong,
												   PlanControl& control)
{
	if (prong == PRONG_INDEX)
		initial	 .deepChangeToRequested(control);
	else
		remaining.wideChangeToRequested(prong, control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideGetNames(const LongIndex parent,
										  const ShortIndex depth,
										  StructureStateInfos& _stateInfos) const
{
	initial	 .deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
	remaining.wideGetNames(parent,								  depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
_CS<NS, NC, NO, TA, NI, TI>::_CS(StateParents& stateParents,
								 const ShortIndex fork,
								 Parents& forkParents,
								 ForkPointers& forkPointers)
	: initial(stateParents,
			  Parent(fork, PRONG_INDEX),
			  forkParents,
			  forkPointers)
{}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideForwardGuard(const ShortIndex HSFM_IF_ASSERT(prong),
											  FullControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideGuard(const ShortIndex HSFM_IF_ASSERT(prong),
									   FullControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideEnterInitial(PlanControl& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideEnter(const ShortIndex HSFM_IF_ASSERT(prong),
									   PlanControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepEnter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
Status
_CS<NS, NC, NO, TA, NI, TI>::wideUpdate(const ShortIndex HSFM_IF_ASSERT(prong),
										FullControl& control)
{
	assert(prong == PRONG_INDEX);

	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
template <typename TEvent>
void
_CS<NS, NC, NO, TA, NI, TI>::wideReact(const ShortIndex HSFM_IF_ASSERT(prong),
									   const TEvent& event,
									   FullControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideExit(const ShortIndex HSFM_IF_ASSERT(prong),
									  PlanControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideForwardRequest(const ShortIndex HSFM_IF_ASSERT(prong),
												const TransitionType transition)
{
	assert(prong == PRONG_INDEX);

	initial.deepForwardRequest(transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideRequestResume(const ShortIndex HSFM_IF_ASSERT(prong)) {
	assert(prong == PRONG_INDEX);

	initial.deepRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideChangeToRequested(const ShortIndex HSFM_IF_ASSERT(prong),
												   PlanControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideGetNames(const LongIndex parent,
										  const ShortIndex depth,
										  StructureStateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NHeadID,
		  ForkID NCompoID,
		  ForkID NOrthoID,
		  typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct _C {
	static constexpr StateID HEAD_ID  = NHeadID;
	static constexpr StateID COMPO_ID = NCompoID;
	static constexpr StateID ORTHO_ID = NOrthoID;

	using Args				= TArgs;
	using Head				= THead;

	using Context			= typename Args::Context;
	using Config			= typename Args::Config;
	using StateList			= typename Args::StateList;
	using PayloadList		= typename Args::PayloadList;

	using StateParents		= Array<Parent, StateList::SIZE>;
	using PlanControl		= PlanControlT	<Context, StateList, Args::FORK_COUNT, Args::PLAN_CAPACITY>;
	using Transition		= TransitionT	<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using ControlLock		= ControlLockT	<Context, StateList, Args::FORK_COUNT, PayloadList>;
	using ControlRegion		= ControlRegionT<Context, StateList, Args::FORK_COUNT, PayloadList>;
	using FullControl		= FullControlT	<Context, StateList, Args::FORK_COUNT, PayloadList, Args::PLAN_CAPACITY>;

	using State				= _S <HEAD_ID, Args, Head>;
	using SubStates			= _CS<HEAD_ID + 1, COMPO_ID + 1, ORTHO_ID, Args, 0, TSubStates...>;
	using Forward			= _CF<Head, TSubStates...>;
	using SubStateList		= typename Forward::StateList;

	_C(StateParents& stateParents,
	   const Parent parent,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

	inline void   deepForwardGuard		(FullControl& control);
	inline void   deepGuard				(FullControl& control);

	inline void   deepEnterInitial		(PlanControl& control);
	inline void   deepEnter				(PlanControl& control);

	inline Status deepUpdate			(FullControl& control);

	template <typename TEvent>
	inline void   deepReact				(const TEvent& event,
										 FullControl& control);

	inline void   deepExit				(PlanControl& control);

	inline void   deepForwardRequest	(const TransitionType transition);
	inline void   deepRequestRemain		();
	inline void   deepRequestRestart	();
	inline void   deepRequestResume		();
	inline void   deepChangeToRequested	(PlanControl& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using RegionType		= typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT	 = State::NAME_COUNT + SubStates::NAME_COUNT;

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif
	Fork _fork;
	State _state;
	SubStates _subStates;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
_C<NS, NC, NO, TA, TH, TS...>::_C(StateParents& stateParents,
								  const Parent parent,
								  Parents& forkParents,
								  ForkPointers& forkPointers)
	: _fork(static_cast<ShortIndex>(forkPointers << &_fork),
			parent,
			forkParents,
			INVALID_SHORT_INDEX
			HSFM_IF_DEBUG(, _state.isBare() ? typeid(void) : typeid(Head)))
	, _state(stateParents,
			 parent,
			 forkParents,
			 forkPointers)
	, _subStates(stateParents,
				 _fork.self,
				 forkParents,
				 forkPointers)
{}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepForwardGuard(FullControl& control) {
	assert(_fork.requested != INVALID_SHORT_INDEX);

	ControlRegion region{control, HEAD_ID, SubStateList::SIZE};

	if (_fork.requested == _fork.active)
		_subStates.wideForwardGuard(_fork.requested, control);
	else
		_subStates.wideGuard	   (_fork.requested, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepGuard(FullControl& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.requested != INVALID_SHORT_INDEX);

	if (!_state	  .deepGuard(				  control))
		_subStates.wideGuard(_fork.requested, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepEnterInitial(PlanControl& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX &&
		   _fork.requested == INVALID_SHORT_INDEX);

	_fork.active = 0;

	_state	  .deepEnter	   (control);
	_subStates.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepEnter(PlanControl& control) {
	assert(_fork.active	   == INVALID_SHORT_INDEX &&
		   _fork.requested != INVALID_SHORT_INDEX);

	_fork.active	= _fork.requested;
	_fork.requested	= INVALID_SHORT_INDEX;

	_state	  .deepEnter(control);
	_subStates.wideEnter(_fork.active, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
Status
_C<NS, NC, NO, TA, TH, TS...>::deepUpdate(FullControl& control) {
	assert(_fork.active != INVALID_SHORT_INDEX);

	ControlRegion region{control, HEAD_ID, SubStateList::SIZE};

	if (const auto status = _state.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates.wideUpdate(_fork.active, control);

		return status;
	} else
		return _subStates.wideUpdate(_fork.active, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
template <typename TEvent>
void
_C<NS, NC, NO, TA, TH, TS...>::deepReact(const TEvent& event,
										 FullControl& control)
{
	assert(_fork.active != INVALID_SHORT_INDEX);

	ControlRegion region{control, HEAD_ID, SubStateList::SIZE};

	_state	  .deepReact(			   event, control);
	_subStates.wideReact(_fork.active, event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepExit(PlanControl& control) {
	assert(_fork.active != INVALID_SHORT_INDEX);

	_subStates.wideExit(_fork.active, control);
	_state	  .deepExit(control);

	_fork.resumable	= _fork.active;
	_fork.active	= INVALID_SHORT_INDEX;
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepForwardRequest(const TransitionType transition) {
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

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestRemain() {
	if (_fork.active == INVALID_SHORT_INDEX)
		_fork.requested = 0;

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestRestart() {
	_fork.requested = 0;

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestResume() {
	_fork.requested = _fork.resumable != INVALID_SHORT_INDEX ?
		_fork.resumable : 0;

	_subStates.wideRequestResume(_fork.requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepChangeToRequested(PlanControl& control) {
	assert(_fork.active != INVALID_SHORT_INDEX);

	if (_fork.requested == _fork.active)
		_subStates.wideChangeToRequested(_fork.requested, control);
	else if (_fork.requested != INVALID_SHORT_INDEX) {
		_subStates.wideExit(_fork.active, control);

		_fork.resumable	= _fork.active;
		_fork.active	= _fork.requested;
		_fork.requested	= INVALID_SHORT_INDEX;

		_subStates.wideEnter(_fork.active, control);
	}
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepGetNames(const LongIndex parent,
											const RegionType region,
											const ShortIndex depth,
											StructureStateInfos& _stateInfos) const
{
	_state	  .deepGetNames(parent, region,			 depth,		_stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, depth + 1, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NHeadID,
		  ForkID NCompoID,
		  ForkID NOrthoID,
		  typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct _Q
	: _C<NHeadID, NCompoID, NOrthoID, TArgs, THead, TSubStates...>
{
	static constexpr StateID HEAD_ID  = NHeadID;
	static constexpr StateID COMPO_ID = NCompoID;
	static constexpr StateID ORTHO_ID = NOrthoID;

	using Args				= TArgs;
	using Head				= THead;
	using Composite			= _C<HEAD_ID, COMPO_ID, ORTHO_ID, Args, Head, TSubStates...>;

	using Context			= typename Args::Context;
	using Config			= typename Args::Config;
	using StateList			= typename Args::StateList;
	using PayloadList		= typename Args::PayloadList;

	using PlanControl		= typename Composite::PlanControl;
	using Plan				= typename PlanControl::Plan;
	using ControlLock		= typename Composite::ControlLock;
	using ControlRegion		= typename Composite::ControlRegion;
	using FullControl		= typename Composite::FullControl;
	using HeadState			= typename Composite::State;

	using SubStateList		= typename Composite::SubStateList;

	using ControlOrigin		= ControlOriginT<Context, StateList, Args::FORK_COUNT>;

	using Composite::Composite;

	inline void	  deepGuard (FullControl& control);

	inline Status deepUpdate(FullControl& control);

	template <typename TEvent>
	inline void   deepReact	(const TEvent& event,
							 FullControl& control);

	inline void	  deepExit  (PlanControl& control);

	using Composite::_fork;
	using Composite::_state;
	using Composite::_subStates;

	bool _success = false;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_Q<NS, NC, NO, TA, TH, TS...>::deepGuard(FullControl& control) {
	Composite::deepGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
Status
_Q<NS, NC, NO, TA, TH, TS...>::deepUpdate(FullControl& control) {
	assert(_fork.active != INVALID_SHORT_INDEX);

	ControlRegion region{control, HEAD_ID, SubStateList::SIZE};

	if (const Status stateStatus = _state.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates.wideUpdate(_fork.active, control);

		return stateStatus;
	} else {
		const Status status = _subStates.wideUpdate(_fork.active, control);

		if (status.failure || status.outerTransition)
			return status;
		else if (status.success)
			_success = true;

		if (_success) {
			ControlOrigin origin{control, HEAD_ID};

			Plan plan = control.plan(HEAD_ID);

			for (auto it = plan.begin(); it; ++it)
				if (control.isActive(it->origin)) {
					control.changeTo(it->destination);
					it.remove();

					_success = false;
				}
		}

		return {_success,
				status.failure,
				status.innerTransition,
				status.outerTransition};
	}
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
template <typename TEvent>
void
_Q<NS, NC, NO, TA, TH, TS...>::deepReact(const TEvent& event,
										 FullControl& control)
{
	Composite::template deepReact<TEvent>(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_Q<NS, NC, NO, TA, TH, TS...>::deepExit(PlanControl& control) {
	Composite::deepExit(control);

	auto plan = control.plan(HEAD_ID);
	plan.clear();

	_success = false;
}

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID, ForkID, ForkID, typename, ShortIndex, typename...>
struct _OS;

//------------------------------------------------------------------------------

template <StateID NInitialID,
		  ForkID NCompoID,
		  ForkID NOrthoID,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct _OS<NInitialID, NCompoID, NOrthoID, TArgs, NIndex, TInitial, TRemaining...> {
	static constexpr StateID   INITIAL_ID	= NInitialID;
	static constexpr StateID   COMPO_ID		= NCompoID;
	static constexpr StateID   ORTHO_ID		= NOrthoID;
	static constexpr LongIndex PRONG_INDEX	= NIndex;

	using Args				= TArgs;

	using Context			= typename Args::Context;
	using Config			= typename Args::Config;
	using StateList			= typename Args::StateList;
	using PayloadList		= typename Args::PayloadList;

	using StateParents		= Array<Parent, StateList::SIZE>;
	using PlanControl		= PlanControlT<Context, StateList, Args::FORK_COUNT, Args::PLAN_CAPACITY>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using FullControl		= FullControlT<Context, StateList, Args::FORK_COUNT, PayloadList, Args::PLAN_CAPACITY>;

	using Initial			= typename WrapMaterial<INITIAL_ID, COMPO_ID, ORTHO_ID, Args, TInitial>::Type;
	using InitialForward	= typename WrapForward<TInitial>::Type;
	using Remaining			= _OS<INITIAL_ID + InitialForward::STATE_COUNT,
								  COMPO_ID + InitialForward::COMPOSITE_COUNT,
								  ORTHO_ID + InitialForward::ORTHOGONAL_COUNT,
								  Args, PRONG_INDEX + 1, TRemaining...>;
	using Forward			= _OSF<TInitial, TRemaining...>;

	_OS(StateParents& stateParents,
		const ShortIndex fork,
		Parents& forkParents,
		ForkPointers& forkPointers);

	inline void	  wideForwardGuard		(const ShortIndex prong,
										 FullControl& control);

	inline void	  wideForwardGuard		(FullControl& control);
	inline void	  wideGuard				(FullControl& control);

	inline void	  wideEnterInitial		(PlanControl& control);
	inline void	  wideEnter				(PlanControl& control);

	inline Status wideUpdate			(FullControl& control);

	template <typename TEvent>
	inline void   wideReact				(const TEvent& event,
										 FullControl& control);

	inline void   wideExit				(PlanControl& control);

	inline void   wideForwardRequest	(const ShortIndex prong, const TransitionType transition);
	inline void   wideRequestRemain		();
	inline void   wideRequestRestart	();
	inline void   wideRequestResume		();
	inline void   wideChangeToRequested	(PlanControl& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT  + Remaining::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	Initial initial;
	Remaining remaining;
};

//------------------------------------------------------------------------------

template <StateID NInitialID,
		  ForkID NCompoID,
		  ForkID NOrthoID,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename TInitial>
struct _OS<NInitialID, NCompoID, NOrthoID, TArgs, NIndex, TInitial> {
	static constexpr StateID   INITIAL_ID	= NInitialID;
	static constexpr StateID   COMPO_ID		= NCompoID;
	static constexpr StateID   ORTHO_ID		= NOrthoID;
	static constexpr LongIndex PRONG_INDEX	= NIndex;

	using Args				= TArgs;

	using Context			= typename Args::Context;
	using Config			= typename Args::Config;
	using StateList			= typename Args::StateList;
	using PayloadList		= typename Args::PayloadList;

	using StateParents		= Array<Parent, StateList::SIZE>;
	using PlanControl		= PlanControlT<Context, StateList, Args::FORK_COUNT, Args::PLAN_CAPACITY>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using FullControl		= FullControlT<Context, StateList, Args::FORK_COUNT, PayloadList, Args::PLAN_CAPACITY>;

	using Initial			= typename WrapMaterial<INITIAL_ID, COMPO_ID, ORTHO_ID, Args, TInitial>::Type;
	using Forward			= _OSF<TInitial>;

	_OS(StateParents& stateParents,
		const ShortIndex fork,
		Parents& forkParents,
		ForkPointers& forkPointers);

	inline void   wideForwardGuard		(const ShortIndex prong,
										 FullControl& control);

	inline void   wideForwardGuard		(FullControl& control);
	inline void   wideGuard				(FullControl& control);

	inline void   wideEnterInitial		(PlanControl& control);
	inline void   wideEnter				(PlanControl& control);

	inline Status wideUpdate			(FullControl& control);

	template <typename TEvent>
	inline void   wideReact				(const TEvent& event,
										 FullControl& control);

	inline void   wideExit				(PlanControl& control);

	inline void   wideForwardRequest	(const ShortIndex prong, const TransitionType transition);
	inline void   wideRequestRemain		();
	inline void   wideRequestRestart	();
	inline void   wideRequestResume		();
	inline void   wideChangeToRequested	(PlanControl& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	Initial initial;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
_OS<NS, NC, NO, TA, NI, TI, TR...>::_OS(StateParents& stateParents,
								 const ShortIndex fork,
								 Parents& forkParents,
								 ForkPointers& forkPointers)
	: initial(stateParents,
			  Parent(fork, PRONG_INDEX),
			  forkParents,
			  forkPointers)
	, remaining(stateParents,
				fork,
				forkParents,
				forkPointers)
{}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardGuard(const ShortIndex prong,
											  FullControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepForwardGuard(control);
	else
		remaining.wideForwardGuard(prong, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardGuard(FullControl& control) {
	initial	 .deepForwardGuard(control);
	remaining.wideForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideGuard(FullControl& control) {
	initial	 .deepGuard(control);
	remaining.wideGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideEnterInitial(PlanControl& control) {
	initial  .deepEnterInitial(control);
	remaining.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideEnter(PlanControl& control) {
	initial  .deepEnter(control);
	remaining.wideEnter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
Status
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideUpdate(FullControl& control) {
	const auto status =	   initial	.deepUpdate(control);
	return combine(status, remaining.wideUpdate(control));
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
template <typename TEvent>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideReact(const TEvent& event,
									   FullControl& control)
{
	initial  .deepReact(event, control);
	remaining.wideReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideExit(PlanControl& control) {
	initial	 .deepExit(control);
	remaining.wideExit(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardRequest(const ShortIndex prong,
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

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestRemain() {
	initial	 .deepRequestRemain();
	remaining.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestRestart() {
	initial	 .deepRequestRestart();
	remaining.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestResume() {
	initial	 .deepRequestResume();
	remaining.wideRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideChangeToRequested(PlanControl& control) {
	initial	 .deepChangeToRequested(control);
	remaining.wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideGetNames(const LongIndex parent,
										  const ShortIndex depth,
										  StructureStateInfos& _stateInfos) const
{
	initial	 .deepGetNames(parent, StructureStateInfo::ORTHOGONAL, depth, _stateInfos);
	remaining.wideGetNames(parent,								   depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
_OS<NS, NC, NO, TA, NI, TI>::_OS(StateParents& stateParents,
								 const ShortIndex fork,
								 Parents& forkParents,
								 ForkPointers& forkPointers)
	: initial(stateParents,
			  Parent(fork, PRONG_INDEX),
			  forkParents,
			  forkPointers)
{}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideForwardGuard(const ShortIndex HSFM_IF_ASSERT(prong),
											  FullControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideForwardGuard(FullControl& control) {
	initial.deepForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideGuard(FullControl& control) {
	initial.deepGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideEnterInitial(PlanControl& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideEnter(PlanControl& control) {
	initial.deepEnter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
Status
_OS<NS, NC, NO, TA, NI, TI>::wideUpdate(FullControl& control) {
	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
template <typename TEvent>
void
_OS<NS, NC, NO, TA, NI, TI>::wideReact(const TEvent& event,
									   FullControl& control)
{
	initial.deepReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideExit(PlanControl& control) {
	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideForwardRequest(const ShortIndex prong,
												const TransitionType transition)
{
	assert(prong <= PRONG_INDEX);

	if (prong == PRONG_INDEX)
		initial.deepForwardRequest(transition);
	else
		initial.deepForwardRequest(Transition::REMAIN);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideRequestResume() {
	initial.deepRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideChangeToRequested(PlanControl& control) {
	initial.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideGetNames(const LongIndex parent,
										  const ShortIndex depth,
										  StructureStateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StructureStateInfo::ORTHOGONAL, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NHeadID,
		  ForkID NCompoID,
		  ForkID NOrthoID,
		  typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct _O final {
	static constexpr StateID HEAD_ID  = NHeadID;
	static constexpr StateID COMPO_ID = NCompoID;
	static constexpr StateID ORTHO_ID = NOrthoID;

	using Args				= TArgs;
	using Head				= THead;

	using Context			= typename Args::Context;
	using Config			= typename Args::Config;
	using StateList			= typename Args::StateList;
	using PayloadList		= typename Args::PayloadList;

	static constexpr LongIndex STATE_COUNT = StateList::SIZE;

	using StateParents		= Array<Parent, STATE_COUNT>;
	using PlanControl		= PlanControlT	<Context, StateList, Args::FORK_COUNT, Args::PLAN_CAPACITY>;
	using Transition		= TransitionT	<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using ControlLock		= ControlLockT	<Context, StateList, Args::FORK_COUNT, PayloadList>;
	using ControlRegion		= ControlRegionT<Context, StateList, Args::FORK_COUNT, PayloadList>;
	using FullControl		= FullControlT	<Context, StateList, Args::FORK_COUNT, PayloadList, Args::PLAN_CAPACITY>;

	using State				= _S <HEAD_ID, Args, Head>;
	using SubStates			= _OS<HEAD_ID + 1, COMPO_ID, ORTHO_ID + 1, Args, 0, TSubStates...>;
	using Forward			= _OF<Head, TSubStates...>;
	using SubStateList		= typename Forward::StateList;

	_O(StateParents& stateParents,
	   const Parent parent,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

	inline void   deepForwardGuard		(FullControl& control);
	inline void   deepGuard				(FullControl& control);

	inline void   deepEnterInitial		(PlanControl& control);
	inline void   deepEnter				(PlanControl& control);

	inline Status deepUpdate			(FullControl& control);

	template <typename TEvent>
	inline void   deepReact				(const TEvent& event,
										 FullControl& control);

	inline void   deepExit				(PlanControl& control);

	inline void   deepForwardRequest	(const TransitionType transition);
	inline void   deepRequestRemain		();
	inline void   deepRequestRestart	();
	inline void   deepRequestResume		();
	inline void   deepChangeToRequested	(PlanControl& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using RegionType		= typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT	 = State::NAME_COUNT  + SubStates::NAME_COUNT;

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	Fork _fork;
	State _state;
	SubStates _subStates;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
_O<NS, NC, NO, TA, TH, TS...>::_O(StateParents& stateParents,
								  const Parent parent,
								  Parents& forkParents,
								  ForkPointers& forkPointers)
	: _fork(static_cast<ShortIndex>(forkPointers << &_fork),
			parent,
			forkParents,
			STATE_COUNT
			HSFM_IF_DEBUG(, _state.isBare() ? typeid(void) : typeid(Head)))
	, _state(stateParents,
			 parent,
			 forkParents,
			 forkPointers)
	, _subStates(stateParents,
				 _fork.self,
				 forkParents,
				 forkPointers)
{}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepForwardGuard(FullControl& control) {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	if (_fork.requested != INVALID_SHORT_INDEX)
		_subStates.wideForwardGuard(_fork.requested, control);
	else
		_subStates.wideForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepGuard(FullControl& control) {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	if (!_state	  .deepGuard(control))
		_subStates.wideGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepEnterInitial(PlanControl& control) {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT &&
		   _fork.requested == INVALID_SHORT_INDEX);

	_state	  .deepEnter	   (control);
	_subStates.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepEnter(PlanControl& control) {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	_state	  .deepEnter(control);
	_subStates.wideEnter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
Status
_O<NS, NC, NO, TA, TH, TS...>::deepUpdate(FullControl& control) {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	ControlRegion region{control, HEAD_ID, SubStateList::SIZE};

	if (const auto status = _state.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates.wideUpdate(control);

		return status;
	} else
		return _subStates.wideUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
template <typename TEvent>
void
_O<NS, NC, NO, TA, TH, TS...>::deepReact(const TEvent& event,
										 FullControl& control)
{
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	_state	  .deepReact(event, control);
	_subStates.wideReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepExit(PlanControl& control) {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	_subStates.wideExit(control);
	_state	  .deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepForwardRequest(const TransitionType transition) {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

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

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestRemain() {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestRestart() {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestResume() {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	_subStates.wideRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepChangeToRequested(PlanControl& control) {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	_subStates.wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepGetNames(const LongIndex parent,
											const RegionType region,
											const ShortIndex depth,
											StructureStateInfos& _stateInfos) const
{
	_state	  .deepGetNames(parent, region,			 depth,		_stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, depth + 1, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TConfig,
		  typename TPayloadList,
		  typename TApex>
class _R final {
	using Context			= TContext;
	using Config			= TConfig;
	using PayloadList		= TPayloadList;
	using Apex				= TApex;

	using ForwardApex		= typename WrapForward<Apex>::Type;
	using StateList			= typename ForwardApex::StateList;
	using Forward			= _RF<Context, Config, PayloadList, Apex>;

	static constexpr LongIndex MAX_PLAN_TASKS	 = Config::MAX_PLAN_TASKS;
	static constexpr LongIndex MAX_SUBSTITUTIONS = Config::MAX_SUBSTITUTIONS;

	static constexpr LongIndex PLAN_CAPACITY	 = Forward::PLAN_CAPACITY;

public:
	static constexpr LongIndex REVERSE_DEPTH	 = ForwardApex::REVERSE_DEPTH;
	static constexpr LongIndex DEEP_WIDTH		 = ForwardApex::DEEP_WIDTH;
	static constexpr LongIndex STATE_COUNT		 = ForwardApex::STATE_COUNT;
	static constexpr LongIndex COMPOSITE_COUNT	 = ForwardApex::COMPOSITE_COUNT;
	static constexpr LongIndex ORTHOGONAL_COUNT	 = ForwardApex::ORTHOGONAL_COUNT;
	static constexpr LongIndex FORK_COUNT		 = ForwardApex::FORK_COUNT;
	static constexpr LongIndex PRONG_COUNT		 = ForwardApex::PRONG_COUNT;
	static constexpr LongIndex WIDTH			 = ForwardApex::WIDTH;

private:
	using Args				= ArgsT<Context, Config, StateList, FORK_COUNT, PayloadList, PLAN_CAPACITY>;
	using PlanControl		= typename Forward::PlanControl;
	using Payload			= typename PayloadList::Container;
	using Transition		= TransitionT<PayloadList>;
	using TransitionControl	= typename Forward::TransitionControl;
	using FullControl		= typename Forward::FullControl;

	static_assert(STATE_COUNT <  (ShortIndex) -1, "Too many states in the hierarchy. Change 'ShortIndex' type.");
	static_assert(STATE_COUNT == (ShortIndex) StateList::SIZE, "STATE_COUNT != StateList::SIZE");

private:
	using Registry				 = RegistryT<STATE_COUNT, FORK_COUNT>;

	using Payloads				 = Array<Payload,		STATE_COUNT>;
	using TransitionQueueStorage = Array<Transition,	FORK_COUNT>;

	using MaterialApex			 = typename WrapMaterial<0, 0, 0, Args, Apex>::Type;
	using Tasks					 = typename FullControl::Tasks;
	using StateTasks			 = Array<TaskIndices,	STATE_COUNT>;			// TODO: change to ForkTasks

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static constexpr LongIndex NAME_COUNT	  = MaterialApex::NAME_COUNT;		// TODO: move to ForwardApex

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

	template <typename T>
	static constexpr LongIndex
	stateId()												{ return StateList::template index<T>();			}

	void update();

	template <typename TEvent>
	inline void react(const TEvent& event);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	inline void changeTo(const StateID stateId);
	inline void resume	(const StateID stateId);
	inline void schedule(const StateID stateId);

	template <typename TPayload>
	inline void changeTo(const StateID stateId, TPayload* const payload);

	template <typename TPayload>
	inline void resume  (const StateID stateId, TPayload* const payload);

	template <typename TPayload>
	inline void schedule(const StateID stateId, TPayload* const payload);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TState>
	inline void changeTo()									{ changeTo(stateId<TState>());						}

	template <typename TState>
	inline void resume	()									{ resume  (stateId<TState>());						}

	template <typename TState>
	inline void schedule()									{ schedule(stateId<TState>());						}

	template <typename TState, typename TPayload>
	inline void changeTo(TPayload* const payload)			{ changeTo(stateId<TState>(), payload);				}

	template <typename TState, typename TPayload>
	inline void resume	(TPayload* const payload)			{ resume  (stateId<TState>(), payload);				}

	template <typename TState, typename TPayload>
	inline void schedule(TPayload* const payload)			{ schedule(stateId<TState>(), payload);				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	inline void resetStateData(const StateID stateId);

	template <typename TPayload>
	inline void setStateData(const StateID stateId, TPayload* const payload);

	inline bool isStateDataSet(const StateID stateId) const;

	template <typename TPayload>
	inline TPayload* getStateData(const StateID stateId) const;

	template <typename TState>
	inline void resetStateData()							{ resetStateData(stateId<TState>());				}

	template <typename TState, typename TPayload>
	inline void setStateData(TPayload* const payload)		{ setStateData(stateId<TState>(), payload);			}

	template <typename TState>
	inline bool isStateDataSet() const						{ return isStateDataSet(stateId<TState>());			}

	template <typename TState, typename TPayload>
	inline TPayload* getStateData() const					{ return getStateData<TPayload>(stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	inline bool isActive   (const StateID stateId) const	{ return ::hfsm::detail::isActive	(_registry, stateId);	}
	inline bool isResumable(const StateID stateId) const	{ return ::hfsm::detail::isResumable(_registry, stateId);	}

	inline bool isScheduled(const StateID stateId) const	{ return isResumable(stateId);						}

	template <typename TState>
	inline bool isActive   () const							{ return isActive	(stateId<TState>());			}

	template <typename TState>
	inline bool isResumable() const							{ return isResumable(stateId<TState>());			}

	template <typename TState>
	inline bool isScheduled() const							{ return isResumable<TState>();						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	const MachineStructure& structure() const				{ return _structure;								}
	const MachineActivity&  activity()  const				{ return _activityHistory;							}
#endif

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	void attachLogger(LoggerInterface* const logger)		{ _logger = logger;									}
#endif

protected:
	void processTransitions();
	void requestImmediate(const Transition request);
	void requestScheduled(const Transition request);

#ifdef _DEBUG
	void verifyPlans() const;
	LongIndex verifyPlan(const StateID stateId) const;
#endif

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	void getStateNames();
	void udpateActivity();
#endif

private:
	Context& _context;

	Registry _registry;
	Tasks _tasks;
	StateTasks _stateTasks;

	Payloads _transitionPayloads;
	TransitionQueueStorage _requests;

	MaterialApex _apex;

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	Prefixes _prefixes;
	StateInfoStorage _stateInfos;

	StructureStorage _structure;
	ActivityHistoryStorage _activityHistory;

	TransitionInfoStorage _lastTransitions;
#endif

	HFSM_IF_LOGGER(LoggerInterface* _logger);
	HSFM_IF_DEBUG(StateList stateList);
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TPL, typename TA>
_R<TC, TG, TPL, TA>::_R(Context& context
						 HFSM_IF_LOGGER(, LoggerInterface* const logger))
	: _context{context}
	, _apex{_registry.stateParents, Parent{}, _registry.forkParents, _registry.forkPointers}
	HFSM_IF_LOGGER(, _logger{logger})
{
	HFSM_IF_STRUCTURE(getStateNames());

	for (auto& payload : _transitionPayloads)
		payload.reset();

	{
		PlanControl control{_context,
							_registry,
							_tasks,
							_stateTasks,
							HFSM_LOGGER_OR(_logger, nullptr)};
		_apex.deepEnterInitial(control);

		HSFM_IF_DEBUG(verifyPlans());
	}

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
_R<TC, TG, TPL, TA>::~_R() {
	PlanControl control{_context,
						_registry,
						_tasks,
						_stateTasks,
						HFSM_LOGGER_OR(_logger, nullptr)};
	_apex.deepExit(control);

	HSFM_IF_DEBUG(verifyPlans());
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::update() {
	FullControl control(_context,
						_registry,
						_requests,
						_tasks,
						_stateTasks,
						HFSM_LOGGER_OR(_logger, nullptr));
	_apex.deepUpdate(control);

	HSFM_IF_DEBUG(verifyPlans());

	if (_requests.count())
		processTransitions();

	_requests.clear();
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TEvent>
void
_R<TC, TG, TPL, TA>::react(const TEvent& event) {
	FullControl control(_context,
						_registry,
						_requests,
						_tasks,
						_stateTasks,
						HFSM_LOGGER_OR(_logger, nullptr));
	_apex.deepReact(event, control);

	HSFM_IF_DEBUG(verifyPlans());

	if (_requests.count())
		processTransitions();

	_requests.clear();
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::changeTo(const StateID stateId) {
	const Transition transition(Transition::Type::RESTART, stateId);
	_requests << transition;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, LoggerInterface::Transition::RESTART, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::resume(const StateID stateId) {
	const Transition transition(Transition::Type::RESUME, stateId);
	_requests << transition;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, LoggerInterface::Transition::RESUME, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::schedule(const StateID stateId) {
	const Transition transition(Transition::Type::SCHEDULE, stateId);
	_requests << transition;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, LoggerInterface::Transition::SCHEDULE, stateId);
#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
void
_R<TC, TG, TPL, TA>::changeTo(const StateID stateId,
							  TPayload* const payload)
{
	const Transition transition(Transition::Type::RESTART, stateId, payload);
	_requests << transition;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, LoggerInterface::Transition::RESTART, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
void
_R<TC, TG, TPL, TA>::resume(const StateID stateId,
							TPayload* const payload)
{
	const Transition transition(Transition::Type::RESUME, stateId, payload);
	_requests << transition;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, LoggerInterface::Transition::RESUME, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
void
_R<TC, TG, TPL, TA>::schedule(const StateID stateId,
							  TPayload* const payload)
{
	const Transition transition(Transition::Type::SCHEDULE, stateId, payload);
	_requests << transition;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, LoggerInterface::Transition::SCHEDULE, stateId);
#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::resetStateData(const StateID stateId) {
	assert(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY)
		_transitionPayloads[stateId].reset();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
void
_R<TC, TG, TPL, TA>::setStateData(const StateID stateId,
								  TPayload* const payload)
{
	assert(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY)
		_transitionPayloads[stateId] = payload;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
bool
_R<TC, TG, TPL, TA>::isStateDataSet(const StateID stateId) const {
	assert(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY)
		return !!_transitionPayloads[stateId];
	else
		return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
TPayload*
_R<TC, TG, TPL, TA>::getStateData(const StateID stateId) const {
	assert(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY) {
		const auto& payload = _transitionPayloads[stateId];

		return payload.template get<TPayload>();
	} else
		return nullptr;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::processTransitions() {
	assert(_requests.count());

	HFSM_IF_STRUCTURE(_lastTransitions.clear());

	for (LongIndex i = 0;
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
			FullControl substitutionControl(_context,
											_registry,
											_requests,
											_tasks,
											_stateTasks,
											HFSM_LOGGER_OR(_logger, nullptr));
			_apex.deepForwardGuard(substitutionControl);

			HSFM_IF_DEBUG(verifyPlans());

		#ifdef HFSM_ENABLE_STRUCTURE_REPORT
			for (const auto& request : _requests)
				_lastTransitions << TransitionInfo(request, Method::GUARD);
		#endif
		}
	}

	{
		PlanControl control{_context,
							_registry,
							_tasks,
							_stateTasks,
							HFSM_LOGGER_OR(_logger, nullptr)};
		_apex.deepChangeToRequested(control);

		HSFM_IF_DEBUG(verifyPlans());
	}

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::requestImmediate(const Transition request) {
	assert(STATE_COUNT > request.stateId);

	for (auto parent = _registry.stateParents[request.stateId]; parent; parent = _registry.forkParents[parent.fork]) {
		auto& fork = *_registry.forkPointers[parent.fork];

		fork.requested = parent.prong;
	}

	_apex.deepForwardRequest(request.type);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::requestScheduled(const Transition request) {
	assert(STATE_COUNT > request.stateId);

	const auto parent = _registry.stateParents[request.stateId];
	auto& fork = *_registry.forkPointers[parent.fork];

	fork.resumable = parent.prong;
}

//------------------------------------------------------------------------------

#ifdef _DEBUG

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::verifyPlans() const {
	LongIndex planCount = 0;
	for (StateID id = 0; id < STATE_COUNT; ++id)
		planCount += verifyPlan(id);

	assert(_tasks.count() == planCount);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
LongIndex
_R<TC, TG, TPL, TA>::verifyPlan(const StateID stateId) const {
	LongIndex length = 0;
	const TaskIndices& indices = _stateTasks[stateId];

	if (indices.first != INVALID_LONG_INDEX) {
		assert(indices.last != INVALID_LONG_INDEX);

		for (auto c = indices.first, d = c; ; ) {
			++length;
			const TaskTransition& task = _tasks[c];

			if (c != indices.last) {
				assert(task.next != INVALID_LONG_INDEX);
				c = task.next;

				// loop check
				if (d != INVALID_LONG_INDEX) {
					d = _tasks[d].next;

					if (d != INVALID_LONG_INDEX) {
						d = _tasks[d].next;
					}

					if (d != INVALID_LONG_INDEX)
						assert(c != d);
				}
			} else {
				assert(task.next == INVALID_LONG_INDEX);

				break;
			}
		};
	} else
		assert(indices.last == INVALID_LONG_INDEX);

	return length;
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::getStateNames() {
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

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::udpateActivity() {
	for (LongIndex i = 0; i < _structure.count(); ++i) {
		_structure[i].isActive = isActive(i);

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
#undef HFSM_LOG_STATE_METHOD
#undef HFSM_LOG_PLANNER_METHOD
#undef HFSM_IF_STRUCTURE

#ifdef _MSC_VER
#pragma warning(pop)
#endif
