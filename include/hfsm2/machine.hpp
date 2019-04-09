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

#include <stdint.h>
#ifdef __GNUC__
#include <stdlib.h>		// @GCC-7: div()
#endif

#include <typeindex>
#ifdef __GNUC__
#include <utility>		// @GCC: std::conditional<>, move(), forward()
#endif

#define HFSM_INLINE														  //inline

//------------------------------------------------------------------------------


#if defined _DEBUG && _MSC_VER
	#include <intrin.h>		// __debugbreak()
#endif

//------------------------------------------------------------------------------

#if defined _DEBUG && defined _MSC_VER
	#define HFSM_BREAK()			__debugbreak()
	#define HFSM_CHECKED(x)			(!!(x) || (HFSM_BREAK(), 0))
#else
	#define HFSM_BREAK()			((void) 0)
	#define HFSM_CHECKED(x)			x
#endif

#ifdef _DEBUG
	#define HFSM_IF_DEBUG(...)		__VA_ARGS__
	#define HFSM_UNLESS_DEBUG(...)
	#define HFSM_DEBUG_OR(y, n)		y
#else
	#define HFSM_IF_DEBUG(...)
	#define HFSM_UNLESS_DEBUG(...)	__VA_ARGS__
	#define HFSM_DEBUG_OR(y, n)		n
#endif

#ifdef HFSM_ENABLE_ASSERT
	#define HFSM_IF_ASSERT(...)		__VA_ARGS__
	#define HFSM_ASSERT(x)			(!!(x) || (HFSM_BREAK(), 0))
	#define HFSM_ASSERT_OR(y, n)	y
#else
	#define HFSM_IF_ASSERT(...)
	#define HFSM_ASSERT(x)			((void) 0)
	#define HFSM_ASSERT_OR(y, n)	n
#endif

namespace hfsm2 {

//------------------------------------------------------------------------------

using ShortIndex = uint8_t;
static constexpr ShortIndex	INVALID_SHORT_INDEX = UINT8_MAX;

using RegionID	 = ShortIndex;
static constexpr RegionID	INVALID_REGION_ID	= INVALID_SHORT_INDEX;

using ForkID	 = int8_t;
static constexpr ForkID		INVALID_FORK_ID		= INT8_MIN;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

using LongIndex  = uint16_t;
static constexpr LongIndex	INVALID_LONG_INDEX	= UINT16_MAX;

using StateID	 = LongIndex;
static constexpr StateID	INVALID_STATE_ID	= INVALID_LONG_INDEX;

//------------------------------------------------------------------------------

namespace detail {

HFSM_IF_DEBUG(struct None {});

////////////////////////////////////////////////////////////////////////////////

template <typename T>
HFSM_INLINE
void
fill(T& a, const char value) {
	memset(&a, (int) value, sizeof(a));
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, unsigned NCount>
constexpr
unsigned
count(const T(&)[NCount]) {
	return NCount;
}

//------------------------------------------------------------------------------

template <typename T, unsigned NCapacity>
HFSM_INLINE
const T*
end(const T(& a)[NCapacity]) {
	return &a[NCapacity];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TReturn, typename T, unsigned NCapacity>
HFSM_INLINE
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

template <int>
struct StaticPrintConst;

template <typename>
struct StaticPrintType;

//------------------------------------------------------------------------------

template <unsigned V1, unsigned V2>
struct StaticAssertEquality;

template <unsigned V1>
struct StaticAssertEquality<V1, V1> {};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
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
	HFSM_INLINE Iterator(Container& container, const LongIndex cursor)
		: _container(container)
		, _cursor(cursor)
	{}

public:
	HFSM_INLINE bool operator != (const Iterator<Container>& dummy) const;

	HFSM_INLINE Iterator& operator ++();

	HFSM_INLINE		  Item& operator *()	   { return  _container[_cursor]; }
	HFSM_INLINE const Item& operator *() const { return  _container[_cursor]; }

	HFSM_INLINE		  Item* operator->()	   { return &_container[_cursor]; }
	HFSM_INLINE const Item* operator->() const { return &_container[_cursor]; }

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
	HFSM_INLINE Iterator(const Container& container, const LongIndex cursor)
		: _container(container)
		, _cursor(cursor)
	{}

public:
	HFSM_INLINE bool operator != (const Iterator<const Container>& dummy) const;

	HFSM_INLINE Iterator& operator ++();

	HFSM_INLINE const Item& operator *() const { return _container[_cursor]; }

	HFSM_INLINE const Item* operator->() const { return &operator *();		 }

private:
	const Container& _container;

	LongIndex _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContainer>
bool
Iterator<TContainer>::operator != (const Iterator<TContainer>& HFSM_IF_ASSERT(dummy)) const {
	HFSM_ASSERT(&_container == &dummy._container);

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
Iterator<const TContainer>::operator != (const Iterator<const TContainer>& HFSM_IF_ASSERT(dummy)) const {
	HFSM_ASSERT(&_container == &dummy._container);

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

namespace hfsm2 {
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
	HFSM_INLINE void clear()										{ _count = 0;		}

	HFSM_INLINE LongIndex resize(const LongIndex count);

	template <typename TValue>
	HFSM_INLINE LongIndex operator << (TValue&& value);

	HFSM_INLINE		  Item& operator[] (const LongIndex i)			{ return get(i);	}
	HFSM_INLINE const Item& operator[] (const LongIndex i) const	{ return get(i);	}

	HFSM_INLINE LongIndex count() const								{ return _count;	}
	HFSM_INLINE LongIndex capacity() const							{ return _capacity;	}

protected:
	HFSM_INLINE LongIndex first() const								{ return 0;			}
	HFSM_INLINE LongIndex limit() const								{ return _count;	}

	HFSM_INLINE LongIndex prev(const LongIndex i) const				{ return i - 1;		}
	HFSM_INLINE LongIndex next(const LongIndex i) const				{ return i + 1;		}

	HFSM_INLINE		  Item& get(const LongIndex i);
	HFSM_INLINE const Item& get(const LongIndex i) const;

protected:
	HFSM_INLINE		  Item* data()		 { return reinterpret_cast<		 Item*>(((uintptr_t)this) + OFFSET); }
	HFSM_INLINE const Item* data() const { return reinterpret_cast<const Item*>(((uintptr_t)this) + OFFSET); }

protected:
	LongIndex _count = 0;
	const LongIndex _capacity;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
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
	HFSM_ASSERT(_count < _capacity);

	new (&get(_count)) Item(std::move(value));

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T>
T&
ArrayView<T>::get(const LongIndex i) {
	HFSM_ASSERT(i < _capacity);

	return data()[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
const T&
ArrayView<T>::get(const LongIndex i) const {
	HFSM_ASSERT(i < _capacity);

	return data()[i];
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// TODO: support range-based for loop

template <typename T, LongIndex NCapacity>
class StaticArray {
public:
	static constexpr LongIndex CAPACITY = NCapacity;

	using Item  = T;
	using Index = typename UnsignedIndex<CAPACITY>::Type;

public:
	HFSM_INLINE StaticArray() = default;
	HFSM_INLINE StaticArray(const Item filler);

	HFSM_INLINE		  Item& operator[] (const LongIndex i);
	HFSM_INLINE const Item& operator[] (const LongIndex i) const;

	HFSM_INLINE LongIndex count() const						{ return CAPACITY;	}

	HFSM_INLINE void fill(const Item filler);

private:
	Item _items[CAPACITY];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
struct StaticArray<T, 0> {
	using Item  = T;

	HFSM_INLINE StaticArray() = default;
	HFSM_INLINE StaticArray(const Item)											{}
};

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
	HFSM_INLINE Array();
	HFSM_INLINE Array(const Array& other);

	HFSM_INLINE void operator = (const Array& other);

	HFSM_INLINE Iterator<	   Array>  begin()		 { return Iterator<		 Array>(*this, View::first());	}
	HFSM_INLINE Iterator<const Array>  begin() const { return Iterator<const Array>(*this, View::first());	}
	HFSM_INLINE Iterator<const Array> cbegin() const { return Iterator<const Array>(*this, View::first());	}

	HFSM_INLINE Iterator<	   Array>	 end()		 { return Iterator<		 Array>(*this, DUMMY);			}
	HFSM_INLINE Iterator<const Array>	 end() const { return Iterator<const Array>(*this, DUMMY);			}
	HFSM_INLINE Iterator<const Array>   cend() const { return Iterator<const Array>(*this, DUMMY);			}

private:
	Item _items[CAPACITY];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
class Array<T, 0> {};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, LongIndex NC>
StaticArray<T, NC>::StaticArray(const Item filler) {
	fill(filler);
}

//------------------------------------------------------------------------------

template <typename T, LongIndex NC>
T&
StaticArray<T, NC>::operator[] (const LongIndex i) {
	HFSM_ASSERT(0 <= i && i < CAPACITY);

	return _items[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, LongIndex NC>
const T&
StaticArray<T, NC>::operator[] (const LongIndex i) const {
	HFSM_ASSERT(0 <= i && i < CAPACITY);

	return _items[i];
}

//------------------------------------------------------------------------------

template <typename T, LongIndex NC>
void
StaticArray<T, NC>::fill(const Item filler) {
	for (LongIndex i = 0; i < CAPACITY; ++i)
		_items[i] = filler;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, LongIndex NC>
Array<T, NC>::Array()
	: View(CAPACITY)
{
	HFSM_ASSERT(View::data() == _items);
}

//------------------------------------------------------------------------------

template <typename T, LongIndex NC>
Array<T, NC>::Array(const Array& other)
	: View(CAPACITY)
{
	HFSM_ASSERT(View::data() == _items);

	for (unsigned i = 0; i < CAPACITY; ++i)
		_items[i] = other._items[i];
}

//------------------------------------------------------------------------------

template <typename T, LongIndex NC>
void
Array<T, NC>::operator = (const Array& other) {
	for (unsigned i = 0; i < CAPACITY; ++i)
		_items[i] = other._items[i];
}

////////////////////////////////////////////////////////////////////////////////

}
}

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
	HFSM_INLINE BitT(BitArray& array,
				const Index index)
		: _array(array)
		, _index(index)
	{}

public:
	HFSM_INLINE explicit operator bool() const		{ return _array.get(_index);	}
	HFSM_INLINE void operator = (const bool value)	{ _array.set(_index, value);	}

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
	HFSM_INLINE ConstBitT(const BitArray& array,
						  const Index index)
		: _array(array)
		, _index(index)
	{}

public:
	HFSM_INLINE explicit operator bool() const		{ return _array.get(_index);	}

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
	HFSM_INLINE BitArrayT(const Index capacity_);

public:
	HFSM_INLINE void clear();

	HFSM_INLINE explicit operator bool() const;

	HFSM_INLINE		 Bit operator[] (const Index i)			{ return	  Bit{*this, i}; }
	HFSM_INLINE ConstBit operator[] (const Index i) const	{ return ConstBit{*this, i}; }

protected:
	HFSM_INLINE bool get(const Index i) const;
	HFSM_INLINE void set(const Index i, const bool value);

	HFSM_INLINE		  StorageUnit* storage()		{ return reinterpret_cast<		StorageUnit*>(((uintptr_t)this) + OFFSET);	}
	HFSM_INLINE const StorageUnit* storage() const	{ return reinterpret_cast<const StorageUnit*>(((uintptr_t)this) + OFFSET);	}

	HFSM_INLINE Index storageUnitCount() const		{ return (capacity + STORAGE_UNIT_SIZE - 1) / STORAGE_UNIT_SIZE;			}

public:
	const Index capacity;
};

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
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
	HFSM_INLINE BitArrayStorageT();

private:
	StorageUnit _storage[STORAGE_UNIT_COUNT];
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndex>
BitArrayT<TIndex>::BitArrayT(const Index capacity_)
	: capacity{capacity_}
{
	clear();
}
//------------------------------------------------------------------------------

template <typename TIndex>
void
BitArrayT<TIndex>::clear() {
	StorageUnit* const s = storage();

	const LongIndex count = storageUnitCount();
	for (LongIndex i = 0; i < count; ++i)
		s[i] = 0;
}

//------------------------------------------------------------------------------

template <typename TIndex>
BitArrayT<TIndex>::operator bool() const {
	StorageUnit result = 0;

	const StorageUnit* const s = storage();

	const LongIndex count = storageUnitCount();
	for (LongIndex i = 0; i < count; ++i)
		result |= s[i];

	return result != 0;
}

//------------------------------------------------------------------------------

template <typename TIndex>
bool
BitArrayT<TIndex>::get(const Index i) const {
	HFSM_ASSERT(i < capacity);

	const auto d = div(i, sizeof(StorageUnit) * 8);

	const StorageUnit& unit = storage()[d.quot];
	const Index mask = 1 << d.rem;

	return (unit & mask) != 0;
}

//------------------------------------------------------------------------------

template <typename TIndex>
void
BitArrayT<TIndex>::set(const Index i, const bool value) {
	HFSM_ASSERT(i < capacity);

	const auto d = div(i, sizeof(StorageUnit) * 8);

	StorageUnit& unit = storage()[d.quot];

	if (value) {
		const Index mask = 1 << d.rem;
		unit |= mask;
	} else {
		const Index mask = ~(1 << d.rem);
		unit &= mask;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TIndex, ShortIndex NCapacity>
BitArrayStorageT<TIndex, NCapacity>::BitArrayStorageT()
	: BitArray{CAPACITY}
{
	HFSM_ASSERT(BitArray::storage() == _storage);
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
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

		HFSM_INLINE Cell()
			: links{}
		{}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	template <typename... TArgs>
	Index emplace(TArgs... args);

	void remove(const Index i);

	HFSM_INLINE		  Item& operator[] (const Index i);
	HFSM_INLINE const Item& operator[] (const Index i) const;

	HFSM_INLINE Index count() const							{ return _count;	}

private:
	HFSM_IF_ASSERT(void verifyStructure(const Index occupied = INVALID) const);

private:
	Cell _cells[CAPACITY];
	Index _vacantHead = 0;
	Index _vacantTail = 0;
	Index _last = 0;
	Index _count = 0;
};

//------------------------------------------------------------------------------

template <typename TItem>
class List<TItem, 0> {};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TItem, LongIndex NCapacity>
template <typename... TArgs>
LongIndex
List<TItem, NCapacity>::emplace(TArgs... args) {
	if (_count < CAPACITY) {
		HFSM_ASSERT(_vacantHead < CAPACITY);
		HFSM_ASSERT(_vacantTail < CAPACITY);

		const Index result = _vacantHead;
		auto& cell = _cells[result];
		++_count;

		if (_vacantHead != _vacantTail) {
			// recycle
			HFSM_ASSERT(cell.links.prev == INVALID);
			HFSM_ASSERT(cell.links.next != INVALID);

			_vacantHead = cell.links.next;

			auto& head = _cells[_vacantHead];
			HFSM_ASSERT(head.links.prev == result);
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
			HFSM_ASSERT(_count == CAPACITY);

			_vacantHead = INVALID;
			_vacantTail = INVALID;
		}

		HFSM_IF_ASSERT(verifyStructure());

		new (&cell.item) Item{std::forward<TArgs>(args)...};

		return result;
	} else {
		// full
		HFSM_ASSERT(_vacantHead == INVALID);
		HFSM_ASSERT(_vacantTail == INVALID);
		HFSM_ASSERT(_count == CAPACITY);
		HFSM_BREAK();

		return INVALID;
	}
}

//------------------------------------------------------------------------------

template <typename TItem, LongIndex NCapacity>
void
List<TItem, NCapacity>::remove(const Index i) {
	HFSM_ASSERT(i < CAPACITY && _count);

	auto& fresh = _cells[i];

	if (_count < CAPACITY) {
		HFSM_ASSERT(_vacantHead < CAPACITY);
		HFSM_ASSERT(_vacantTail < CAPACITY);

		fresh.links.prev = INVALID;
		fresh.links.next = _vacantHead;

		auto& head = _cells[_vacantHead];
		head.links.prev = i;

		_vacantHead = i;
	} else {
		// 0 -> 1
		HFSM_ASSERT(_count == CAPACITY);
		HFSM_ASSERT(_vacantHead == INVALID);
		HFSM_ASSERT(_vacantTail == INVALID);

		fresh.links.prev = INVALID;
		fresh.links.next = INVALID;

		_vacantHead = i;
		_vacantTail = i;
	}

	--_count;

	HFSM_IF_ASSERT(verifyStructure());
}

//------------------------------------------------------------------------------

template <typename TItem, LongIndex NCapacity>
TItem&
List<TItem, NCapacity>::operator[] (const Index i) {
	HFSM_IF_ASSERT(verifyStructure());

	return _cells[i].item;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TItem, LongIndex NCapacity>
const TItem&
List<TItem, NCapacity>::operator[] (const Index i) const {
	HFSM_IF_ASSERT(verifyStructure());

	return _cells[i].item;
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_ASSERT

template <typename TItem, LongIndex NCapacity>
void
List<TItem, NCapacity>::verifyStructure(const Index occupied) const {
	if (_count < CAPACITY) {
		HFSM_ASSERT(_vacantHead < CAPACITY);
		HFSM_ASSERT(_vacantTail < CAPACITY);

		HFSM_ASSERT(_cells[_vacantHead].links.prev == INVALID);
		HFSM_ASSERT(_cells[_vacantTail].links.next == INVALID);

		auto emptyCount = 1;

		for (auto c = _vacantHead; c != _vacantTail; ) {
			HFSM_ASSERT(occupied != c);

			const auto& current = _cells[c];

			const auto f = current.links.next;
			if (f != INVALID) {
				// next
				const auto& following = _cells[f];

				HFSM_ASSERT(following.links.prev == c);

				c = f;
				continue;
			} else {
				// end
				HFSM_ASSERT(_vacantTail == c);
				HFSM_ASSERT(_count == CAPACITY - emptyCount);

				break;
			}
		}
	} else {
		HFSM_ASSERT(_vacantHead == INVALID);
		HFSM_ASSERT(_vacantTail == INVALID);
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

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

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TI, ShortIndex NIC, LongIndex NSS>
ObjectPool<TI, NIC, NSS>::ObjectPool() {
	_offsets[0] = 0;
}

//------------------------------------------------------------------------------

template <typename TI, ShortIndex NIC, LongIndex NSS>
TI&
ObjectPool<TI, NIC, NSS>::operator[] (const ShortIndex i) {
	HFSM_ASSERT(i < _count);

	const ShortIndex offset = _offsets[i];
	HFSM_ASSERT(offset + sizeof(Interface) <= STORAGE_SIZE);

	return *reinterpret_cast<Interface*>(_storage + offset);
	//return *std::launder(reinterpret_cast<Interface*>(_storage + offset));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, ShortIndex NIC, LongIndex NSS>
const TI&
ObjectPool<TI, NIC, NSS>::operator[] (const ShortIndex i) const {
	HFSM_ASSERT(i < _count);

	const ShortIndex offset = _offsets[i];
	HFSM_ASSERT(offset + sizeof(Interface) <= STORAGE_SIZE);

	return *reinterpret_cast<const Interface*>(_storage + offset);
	//return *std::launder(reinterpret_cast<const Interface*>(_storage + offset));
}

//------------------------------------------------------------------------------

template <typename TI, ShortIndex NIC, LongIndex NSS>
template <typename TItem, typename... TArgs>
ShortIndex
ObjectPool<TI, NIC, NSS>::emplace(TArgs&&... args) {
	ShortIndex result = _count;

	using Item = TItem;
	static constexpr LongIndex SIZE = sizeof(Item);

	HFSM_ASSERT(_count < ITEM_COUNT);

	const ShortIndex offset = _offsets[_count];
	HFSM_ASSERT(offset + SIZE <= STORAGE_SIZE);

	new (_storage + offset) Item{std::forward<TArgs>(args)...};

	++_count;
	if (_count < ITEM_COUNT)
		_offsets[_count] = offset + SIZE;

	return result;
}

//------------------------------------------------------------------------------

template <typename TI, ShortIndex NIC, LongIndex NSS>
void
ObjectPool<TI, NIC, NSS>::clear() {
	for (const ShortIndex offset : _offsets) {
		HFSM_ASSERT(offset + sizeof(Interface) <= STORAGE_SIZE);

		Interface& item = *reinterpret_cast<Interface*>(_storage + offset);
		item.clear();
	}
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename... Ts>
struct _TL {
	static constexpr LongIndex SIZE = sizeof...(Ts);
};

//------------------------------------------------------------------------------

template <typename...>
struct PrependT;

template <typename T, typename... Ts>
struct PrependT<T, _TL<Ts...>> {
	using Type = _TL<T, Ts...>;
};

template <typename... Ts>
using Prepend = typename PrependT<Ts...>::Type;

//------------------------------------------------------------------------------

template <size_t, size_t, typename...>
struct LesserT;

template <size_t H, size_t I, typename TFirst, typename... TRest>
struct LesserT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<(I < H),
										   Prepend<TFirst, typename LesserT<H, I + 1, TRest...>::Type>,
										   typename LesserT<H, I + 1, TRest...>::Type>::type;
};

template <size_t H, size_t I>
struct LesserT<H, I> {
	using Type = _TL<>;
};

template <typename... Ts>
using SplitL = typename LesserT<sizeof...(Ts) / 2, 0, Ts...>::Type;

//------------------------------------------------------------------------------

template <size_t, size_t, typename...>
struct GreaterT;

template <size_t H, size_t I, typename TFirst, typename... TRest>
struct GreaterT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<(I < H),
										   typename GreaterT<H, I + 1, TRest...>::Type,
										   _TL<TFirst, TRest...>>::type;
};

template <size_t H, size_t I>
struct GreaterT<H, I> {
	using Type = _TL<>;
};

template <typename... Ts>
using SplitR = typename GreaterT<sizeof...(Ts) / 2, 0, Ts...>::Type;

////////////////////////////////////////////////////////////////////////////////

template<LongIndex N>
struct IndexConstant {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<LongIndex... Ns>
struct IndexSequence {
	using Type = IndexSequence<Ns...>;
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
struct IndexedTypeList_EntryT {};

template <typename T, std::size_t N>
struct IndexedTypeList_EntryN
	: IndexedTypeList_EntryT<T>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
struct IndexedTypeList_Impl;

template <LongIndex... Ns, typename... Ts>
struct IndexedTypeList_Impl<IndexSequence<Ns...>, Ts...>
	: IndexedTypeList_EntryN<Ts, Ns>...
{
	template <typename T, std::size_t N>
	static constexpr LongIndex select(IndexedTypeList_EntryN<T, N>) { return (LongIndex) N; }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
class VariantT;

template <typename... Ts>
struct _ITL
	: private IndexedTypeList_Impl<IndexSequenceFor<Ts...>, Ts...>
{
	using Base = IndexedTypeList_Impl<IndexSequenceFor<Ts...>, Ts...>;
	using Variant = VariantT<Ts...>;

	static constexpr LongIndex SIZE = sizeof...(Ts);

	template <typename T>
	static constexpr bool contains() { return std::is_base_of<IndexedTypeList_EntryT<T>, _ITL>::value; }

	template <typename T>
	static constexpr
	typename std::enable_if< std::is_base_of<IndexedTypeList_EntryT<T>, _ITL>::value, LongIndex>::type
	index() {
		return Base::template select<T>(_ITL{});
	}

	template <typename T>
	static constexpr
	typename std::enable_if<!std::is_base_of<IndexedTypeList_EntryT<T>, _ITL>::value, LongIndex>::type
	index() {
		return INVALID_LONG_INDEX;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
struct IndexedT;

template <typename... Ts>
struct IndexedT<_TL<Ts...>> {
	using Type = _ITL<Ts...>;
};

template <typename T>
using Indexed = typename IndexedT<T>::Type;

//------------------------------------------------------------------------------

template <typename...>
struct MergeT;

template <typename... Ts1, typename... Ts2>
struct MergeT<_TL<Ts1...>, _TL<Ts2...>> {
	using TypeList = _TL<Ts1..., Ts2...>;
};

template <typename... Ts>
using Merge = typename MergeT<Ts...>::TypeList;

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

template <typename... Ts>
class alignas(alignof(void*)) VariantT {
	using Types = _ITL<Ts...>;

public:
	template <typename T>
	static constexpr LongIndex index() {
		return Types::template index<T>();
	}

	template <typename T>
	static constexpr bool contains() {
		return Types::template contains<T>();
	}

	HFSM_INLINE VariantT() = default;

	template <typename T,
			  typename = typename std::enable_if<contains<T>()>::type>
	HFSM_INLINE VariantT(T* const p)
		: _pointer(p)
		, _index(index<T>())
	{
		HFSM_ASSERT(_index != INVALID_LONG_INDEX);
	}

	HFSM_INLINE explicit operator bool() const { return _index != INVALID_LONG_INDEX; }

	HFSM_INLINE void reset() {
		_pointer = nullptr;
		_index = INVALID_LONG_INDEX;
	}

	template <typename T>
	HFSM_INLINE typename std::enable_if<contains<T>(), T>::type*
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

namespace hfsm2 {

enum class Method : ShortIndex {
	ENTRY_GUARD,
	ENTER,
	UPDATE,
	REACT,
	EXIT_GUARD,
	EXIT,
	PLAN_SUCCEEDED,
	PLAN_FAILED,

	COUNT
};

enum class Transition : ShortIndex {
	RESTART,
	RESUME,
	SCHEDULE,

	COUNT
};

enum class StatusEvent : ShortIndex {
	SUCCEEDED,
	FAILED,

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
methodName(const Method method) {
	switch (method) {
		case Method::ENTRY_GUARD:		return "entryGuard";
		case Method::ENTER:				return "enter";
		case Method::UPDATE:			return "update";
		case Method::REACT:				return "react";
		case Method::EXIT_GUARD:		return "exitGuard";
		case Method::EXIT:				return "exit";
		case Method::PLAN_SUCCEEDED:	return "planSucceeded";
		case Method::PLAN_FAILED:		return "planFailed";

		default:
			HFSM_BREAK();
			return nullptr;
	}
}

//------------------------------------------------------------------------------

static inline
const char*
transitionName(const Transition transition) {
	switch (transition) {
		case Transition::RESTART:		return "changeTo";
		case Transition::RESUME:		return "resume";
		case Transition::SCHEDULE:		return "schedule";

		default:
			HFSM_BREAK();
			return nullptr;
	}
}

////////////////////////////////////////////////////////////////////////////////

}

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG

namespace hfsm2 {

////////////////////////////////////////////////////////////////////////////////

struct LoggerInterface {
	using Method	  = ::hfsm2::Method;
	using StateID	  = ::hfsm2::StateID;
	using RegionID	  = ::hfsm2::RegionID;
	using Transition  = ::hfsm2::Transition;
	using StatusEvent = ::hfsm2::StatusEvent;

	virtual void recordMethod(const StateID /*origin*/,
							  const Method /*method*/)
	{}

	virtual void recordTransition(const StateID /*origin*/,
								  const Transition /*transition*/,
								  const StateID /*target*/)
	{}

	virtual void recordTaskStatus(const RegionID /*region*/,
								  const StateID /*origin*/,
								  const StatusEvent /*event*/)
	{}

	virtual void recordPlanStatus(const RegionID /*region*/,
								  const StatusEvent /*event*/)
	{}

	virtual void recordCancelledPending(const StateID /*origin*/) {}
};

////////////////////////////////////////////////////////////////////////////////

}

#else

namespace hfsm2 {

using LoggerInterface = void;

}

#endif

//------------------------------------------------------------------------------

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	#define HFSM_IF_LOGGER(...)										  __VA_ARGS__
	#define HFSM_LOGGER_OR(Y, N)												Y
	#define HFSM_LOG_TASK_STATUS(REGION, ORIGIN, STATUS)						\
		if (_logger)															\
			_logger->recordTaskStatus(REGION, ORIGIN, STATUS);
	#define HFSM_LOG_PLAN_STATUS(REGION, STATUS)								\
		if (_logger)															\
			_logger->recordPlanStatus(REGION, STATUS);
	#define HFSM_LOG_CANCELLED_PENDING(ORIGIN)									\
		if (_logger)															\
			_logger->recordCancelledPending(ORIGIN);
#else
	#define HFSM_IF_LOGGER(...)
	#define HFSM_LOGGER_OR(Y, N)												N
	#define HFSM_LOG_TASK_STATUS(REGION, ORIGIN, STATUS)
	#define HFSM_LOG_PLAN_STATUS(REGION, STATUS)
	#define HFSM_LOG_CANCELLED_PENDING(ORIGIN)
#endif

#if defined HFSM_VERBOSE_DEBUG_LOG
	#define HFSM_LOG_STATE_METHOD(METHOD, ID)									\
		if (auto* const logger = control.logger())								\
			logger->recordMethod(STATE_ID, ID);
#elif defined HFSM_ENABLE_LOG_INTERFACE
	#define HFSM_LOG_STATE_METHOD(METHOD, ID)									\
		if (auto* const logger = control.logger())								\
			log<decltype(METHOD), ID>(*logger);
#else
	#define HFSM_LOG_STATE_METHOD(METHOD, ID)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	#define HFSM_IF_STRUCTURE(...)									  __VA_ARGS__
#else
	#define HFSM_IF_STRUCTURE(...)
#endif

//------------------------------------------------------------------------------

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// TODO: add TaskLink::payload

#pragma pack(push, 2)

//template <typename TPayloadList>
//struct TaskLinkT {

struct TaskLink {
	//using PayloadList	= TPayloadList;
	//using PayloadBox		= typename PayloadList::Variant;

	HFSM_INLINE TaskLink(const StateID origin_,
						 const StateID destination_)
		: origin(origin_)
		, destination(destination_)
		, next(INVALID_LONG_INDEX)
	{}

	//PayloadBox payload;

	StateID origin		= INVALID_STATE_ID;
	StateID destination	= INVALID_STATE_ID;

	LongIndex prev		= INVALID_LONG_INDEX;
	LongIndex next		= INVALID_LONG_INDEX;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Bounds {
	LongIndex first		= INVALID_LONG_INDEX;
	LongIndex last		= INVALID_LONG_INDEX;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

template <typename,
		  typename,
		  typename,
		  typename,
		  LongIndex,
		  LongIndex,
		  LongIndex,
		  typename,
		  LongIndex>
struct ArgsT;

template <typename>
struct PlanDataT;

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NCompoCount,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  typename TPayloadList,
		  LongIndex NTaskCapacity>
struct PlanDataT<ArgsT<TContext,
					   TConfig,
					   TStateList,
					   TRegionList,
					   NCompoCount,
					   NOrthoCount,
					   NOrthoUnits,
					   TPayloadList,
					   NTaskCapacity>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;

	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
	static constexpr ShortIndex TASK_CAPACITY = NTaskCapacity;

	using TaskLinks		= List<TaskLink, TASK_CAPACITY>;
	using TasksBounds	= Array<Bounds, RegionList::SIZE>;
	using TasksBits		= BitArrayStorageT<StateID, StateList::SIZE>;
	using RegionBit		= BitT<RegionID>;
	using RegionBits	= BitArrayStorageT<RegionID, RegionList::SIZE>;

	HFSM_INLINE void setSuccessful(const StateID stateId, const bool state) { tasksSuccesses[stateId]  = state;	}
	HFSM_INLINE void setFailed	  (const StateID stateId, const bool state) { tasksFailures [stateId]  = state;	}

	HFSM_INLINE bool hasSucceeded (const StateID stateId) const	   { return !!tasksSuccesses[stateId];			}
	HFSM_INLINE bool hasFailed	  (const StateID stateId) const	   { return !!tasksFailures [stateId];			}

	TaskLinks taskLinks;
	TasksBounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	RegionBits planExists;

#ifdef HFSM_ENABLE_ASSERT
	void verifyPlans() const;
	LongIndex verifyPlan(const RegionID stateId) const;
#endif
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  typename TPayloadList,
		  LongIndex NTaskCapacity>
struct PlanDataT<ArgsT<TContext,
					   TConfig,
					   TStateList,
					   TRegionList,
					   0,
					   NOrthoCount,
					   NOrthoUnits,
					   TPayloadList,
					   NTaskCapacity>>
{
	static /*constexpr*/ void setSuccessful(const StateID, const bool)			{}
	static /*constexpr*/ void setFailed	   (const StateID, const bool)			{}

	static constexpr bool hasSucceeded (const StateID)			{ return false;	}
	static constexpr bool hasFailed	   (const StateID)			{ return false;	}

#ifdef HFSM_ENABLE_ASSERT
	void verifyPlans() const													{}
	LongIndex verifyPlan(const RegionID) const					{ return 0;		}
#endif
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM_ENABLE_ASSERT

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::verifyPlans() const {
	LongIndex planCount = 0;
	for (RegionID id = 0; id < REGION_COUNT; ++id)
		planCount += verifyPlan(id);

	HFSM_ASSERT(taskLinks.count() == planCount);
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
LongIndex
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::verifyPlan(const RegionID regionId) const {
	LongIndex length = 0;
	const Bounds& bounds = tasksBounds[regionId];

	if (bounds.first != INVALID_LONG_INDEX) {
		HFSM_ASSERT(bounds.last != INVALID_LONG_INDEX);

		for (auto slow = bounds.first, fast = slow; ; ) {
			++length;
			const TaskLink& task = taskLinks[slow];

			if (slow != bounds.last) {
				HFSM_ASSERT(task.next != INVALID_LONG_INDEX);
				slow = task.next;

				// loop check
				if (fast != INVALID_LONG_INDEX) {
					fast = taskLinks[fast].next;

					if (fast != INVALID_LONG_INDEX) {
						fast = taskLinks[fast].next;
					}

					if (fast != INVALID_LONG_INDEX)
						HFSM_ASSERT(slow != fast);
				}
			} else {
				HFSM_ASSERT(task.next == INVALID_LONG_INDEX);

				break;
			}
		};
	} else
		HFSM_ASSERT(bounds.last == INVALID_LONG_INDEX);

	return length;
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct Status {
	bool success = false;
	bool failure = false;
	bool outerTransition = false;

	inline Status(const bool success_ = false,
				  const bool failure_ = false,
				  const bool outerTransition_ = false);

	inline explicit operator bool() const { return success || failure || outerTransition; }

	inline void clear();
};

#pragma pack(pop)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Status
combine(const Status lhs, const Status rhs) {
	return Status{lhs.success || rhs.success,
				  lhs.failure || rhs.failure,
				  lhs.outerTransition || rhs.outerTransition};
}

//------------------------------------------------------------------------------

template <typename>
class ControlT;

template <typename>
class FullControlT;

template <typename TArgs>
class PlanT {
	template <typename>
	friend class ControlT;

	template <typename>
	friend class FullControlT;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using PayloadList	= typename Args::PayloadList;

	static constexpr LongIndex  TASK_CAPACITY	= Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using TaskLinks		= typename PlanData::TaskLinks;
	using RegionBit		= typename PlanData::RegionBit;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Iterator {
		HFSM_INLINE Iterator(PlanT& plan);

		HFSM_INLINE explicit operator bool() const;

		HFSM_INLINE void operator ++();

		HFSM_INLINE		  TaskLink& operator  *()	    { return  _plan._planData.taskLinks[_curr];	}
		HFSM_INLINE const TaskLink& operator  *() const { return  _plan._planData.taskLinks[_curr];	}

		HFSM_INLINE		  TaskLink* operator ->()	    { return &_plan._planData.taskLinks[_curr];	}
		HFSM_INLINE const TaskLink* operator ->() const { return &_plan._planData.taskLinks[_curr];	}

		HFSM_INLINE void remove();

		HFSM_INLINE LongIndex next() const;

		PlanT& _plan;
		LongIndex _curr;
		LongIndex _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	HFSM_INLINE PlanT(PlanData& planData,
					  const RegionID regionId);

	template <typename T>
	static constexpr LongIndex stateId()	{ return StateList ::template index<T>();	}

	template <typename T>
	static constexpr LongIndex regionId()	{ return RegionList::template index<T>();	}

public:
	HFSM_INLINE explicit operator bool() const;

	HFSM_INLINE void clear();

	void add(const StateID origin,
			 const StateID destination);

	template <typename TOrigin, typename TDestination>
	HFSM_INLINE void add()			{ add(stateId<TOrigin>(), stateId<TDestination>());	}

	void remove(const LongIndex task);

	HFSM_INLINE Iterator begin()			{ return Iterator{*this};					}

private:
	PlanData& _planData;
	RegionBit _planExists;
	Bounds& _bounds;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

Status::Status(const bool success_,
			   const bool failure_,
			   const bool outerTransition_)
		: success{success_}
		, failure{failure_}
		, outerTransition{outerTransition_}
	{}

//------------------------------------------------------------------------------

void
Status::clear() {
	success = false;
	failure = false;
	outerTransition = false;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
PlanT<TArgs>::Iterator::Iterator(PlanT& plan)
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
PlanT<TArgs>::Iterator::operator bool() const {
	HFSM_ASSERT(_curr < PlanT::TASK_CAPACITY || _curr == INVALID_LONG_INDEX);

	return _curr < PlanT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanT<TArgs>::Iterator::operator ++() {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanT<TArgs>::Iterator::remove() {
	_plan.remove(_curr);
}

//------------------------------------------------------------------------------

template <typename TArgs>
LongIndex
PlanT<TArgs>::Iterator::next() const {
	if (_curr < PlanT::TASK_CAPACITY) {
		const TaskLink& task = _plan._planData.taskLinks[_curr];

		return task.next;
	} else {
		HFSM_ASSERT(_curr == INVALID_LONG_INDEX);

		return INVALID_LONG_INDEX;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
PlanT<TArgs>::PlanT(PlanData& planData,
					const RegionID regionId)

	: _planData{planData}
	, _planExists{planData.planExists[regionId]}
	, _bounds{planData.tasksBounds[regionId]}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
PlanT<TArgs>::operator bool() const {
	if (_bounds.first < PlanT::TASK_CAPACITY) {
		HFSM_ASSERT(_bounds.last < PlanT::TASK_CAPACITY);
		return true;
	} else {
		HFSM_ASSERT(_bounds.last == INVALID_LONG_INDEX);
		return false;
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanT<TArgs>::clear() {
	if (_bounds.first < TaskLinks::CAPACITY) {
		HFSM_ASSERT(_bounds.last < TaskLinks::CAPACITY);

		for (LongIndex index = _bounds.first;
			 index != INVALID_LONG_INDEX;
			 )
		{
			HFSM_ASSERT(index < TaskLinks::CAPACITY);

			const auto& task = _planData.taskLinks[index];
			HFSM_ASSERT(index == _bounds.first ?
				   task.prev == INVALID_LONG_INDEX :
				   task.prev <  TaskLinks::CAPACITY);

			const LongIndex next = task.next;

			_planData.taskLinks.remove(index);

			index = next;
		}

		_bounds.first = INVALID_LONG_INDEX;
		_bounds.last  = INVALID_LONG_INDEX;
	} else
		HFSM_ASSERT(_bounds.first == INVALID_LONG_INDEX &&
			   _bounds.last  == INVALID_LONG_INDEX);
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanT<TArgs>::add(const StateID origin,
				  const StateID destination)
{
	_planExists = true;
	const auto index = _planData.taskLinks.emplace(origin, destination);

	if (_bounds.first < TaskLinks::CAPACITY) {
		HFSM_ASSERT(_bounds.last < TaskLinks::CAPACITY);

		auto& last  = _planData.taskLinks[_bounds.last];
		last.next = index;

		auto& next = _planData.taskLinks[index];
		next.prev  = _bounds.last;

		_bounds.last = index;
	} else {
		HFSM_ASSERT(_bounds.first == INVALID_LONG_INDEX &&
					_bounds.last  == INVALID_LONG_INDEX);

		_bounds.first = index;
		_bounds.last  = index;
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanT<TArgs>::remove(const LongIndex task) {
	HFSM_ASSERT(_planExists &&
				_bounds.first < TaskLinks::CAPACITY &&
				_bounds.last  < TaskLinks::CAPACITY);

	HFSM_ASSERT(task < TaskLinks::CAPACITY);

	const TaskLink& curr = _planData.taskLinks[task];

	if (curr.prev < TaskLinks::CAPACITY) {
		TaskLink& prev = _planData.taskLinks[curr.prev];
		prev.next = curr.next;
	} else {
		HFSM_ASSERT(_bounds.first == task);
		_bounds.first = curr.next;
	}

	if (curr.next < TaskLinks::CAPACITY) {
		TaskLink& next = _planData.taskLinks[curr.next];
		next.prev = curr.prev;
	} else {
		HFSM_ASSERT(_bounds.last == task);
		_bounds.last = curr.prev;
	}

	_planData.taskLinks.remove(task);
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct alignas(2 * sizeof(ShortIndex)) Parent {
	HFSM_INLINE Parent() = default;

	HFSM_INLINE Parent(const ForkID forkId_)
		: forkId{forkId_}
	{}

	HFSM_INLINE Parent(const ForkID forkId_,
					   const ShortIndex prong_)
		: forkId{forkId_}
		, prong{prong_}
	{}

	HFSM_INLINE explicit operator bool() const {
		return forkId != INVALID_FORK_ID &&
			   prong  != INVALID_SHORT_INDEX;
	}

	ForkID	   forkId = INVALID_FORK_ID;
	ShortIndex prong  = INVALID_SHORT_INDEX;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

template <typename TPayloadList>
struct RequestT {
	using PayloadList = TPayloadList;
	using PayloadBox = typename PayloadList::Variant;

	enum Type {
		REMAIN,
		RESTART,
		RESUME,
		SCHEDULE,

		COUNT
	};

	template <typename T>
	static constexpr bool contains() { return PayloadList::template contains<T>();	}

	HFSM_INLINE RequestT() = default;

	HFSM_INLINE RequestT(const Type type_,
						 const StateID stateId_)
		: type{type_}
		, stateId{stateId_}
	{
		HFSM_ASSERT(type_ < Type::COUNT);
	}

	template <typename T,
			  typename = typename std::enable_if<contains<T>(), T>::type>
	HFSM_INLINE RequestT(const Type type_,
						 const StateID stateId_,
						 T* const payload_)
		: type{type_}
		, stateId{stateId_}
		, payload{payload_}
	{
		HFSM_ASSERT(type_ < Type::COUNT);
	}

	Type type = RESTART;
	StateID stateId = INVALID_STATE_ID;
	PayloadBox payload;
};

template <typename TPayloadList, ShortIndex NCount>
using RequestsT = Array<RequestT<TPayloadList>, NCount>;

////////////////////////////////////////////////////////////////////////////////

using OrthoFork = BitArrayT<ShortIndex>;

template <ShortIndex NProngCount>
using OrthoForkT = BitArrayStorageT<ShortIndex, NProngCount>;

template <ShortIndex NCapacity, LongIndex NStorageSize>
using OrthoForksT = ObjectPool<OrthoFork, NCapacity, NStorageSize>;

//------------------------------------------------------------------------------

template <LongIndex NCompoCount, LongIndex NOrthoCount, LongIndex NOrthoUnits>
struct AllForksT {
	static constexpr ShortIndex COMPO_COUNT	  = NCompoCount;
	static constexpr ShortIndex ORTHO_COUNT	  = NOrthoCount;
	static constexpr ShortIndex ORTHO_UNITS	  = NOrthoUnits;
	static constexpr LongIndex  ORTHO_STORAGE = ORTHO_COUNT * sizeof(OrthoFork) + ORTHO_UNITS;

	using Compo = StaticArray<ShortIndex,  COMPO_COUNT>;
	using Ortho = OrthoForksT<ORTHO_COUNT, ORTHO_STORAGE>;

	Compo compo{INVALID_SHORT_INDEX};
	Ortho ortho;

	HFSM_INLINE void clear();
};

////////////////////////////////////////////////////////////////////////////////

template <typename,
		  typename,
		  typename,
		  typename,
		  LongIndex,
		  LongIndex,
		  LongIndex,
		  typename,
		  LongIndex>
struct ArgsT;

template <typename>
struct StateRegistryT;

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NCompoCount,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  typename TPayloadList,
		  LongIndex NTaskCapacity>
struct StateRegistryT<ArgsT<TContext,
							TConfig,
							TStateList,
							TRegionList,
							NCompoCount,
							NOrthoCount,
							NOrthoUnits,
							TPayloadList,
							NTaskCapacity>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;
	using PayloadList	= TPayloadList;

	using Request		= RequestT<PayloadList>;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex COMPO_COUNT	  = NCompoCount;
	static constexpr ShortIndex ORTHO_COUNT	  = NOrthoCount;
	static constexpr ShortIndex ORTHO_UNITS	  = NOrthoUnits;

	using StateParents	= StaticArray<Parent, STATE_COUNT>;

	using CompoParents	= StaticArray<Parent, COMPO_COUNT>;
	using OrthoParents	= StaticArray<Parent, ORTHO_COUNT>;

	using CompoForks	= StaticArray<ShortIndex,  COMPO_COUNT>;
	using AllForks		= AllForksT<COMPO_COUNT, ORTHO_COUNT, ORTHO_UNITS>;

	bool isActive	(const StateID stateId) const;
	bool isResumable(const StateID stateId) const;

	bool isPendingChange(const StateID stateId) const;
	bool isPendingEnter	(const StateID stateId) const;
	bool isPendingExit	(const StateID stateId) const;

	HFSM_INLINE const Parent& forkParent(const ForkID forkId) const;
	HFSM_INLINE ForkID parentCompoForkId(const ForkID forkId) const;

	HFSM_INLINE ShortIndex    activeCompoProng(const ForkID forkId) const;
	HFSM_INLINE ShortIndex resumableCompoProng(const ForkID forkId) const;
	HFSM_INLINE ShortIndex requestedCompoProng(const ForkID forkId) const;

	HFSM_INLINE ShortIndex& resumableCompoFork(const ForkID forkId);
	HFSM_INLINE OrthoFork&  resumableOrthoFork(const ForkID forkId);

	HFSM_INLINE ShortIndex& requestedCompoFork(const ForkID forkId);
	HFSM_INLINE OrthoFork&  requestedOrthoFork(const ForkID forkId);

	void requestImmediate(const Request request);
	void requestScheduled(const StateID stateId);

	void clearOrthoRequested();

	StateParents stateParents;
	CompoParents compoParents;
	OrthoParents orthoParents;

	CompoForks compoActive{INVALID_SHORT_INDEX};

	AllForks resumable;
	AllForks requested;
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NCompoCount,
		  typename TPayloadList,
		  LongIndex NTaskCapacity>
struct StateRegistryT<ArgsT<TContext,
							TConfig,
							TStateList,
							TRegionList,
							NCompoCount,
							0,
							0,
							TPayloadList,
							NTaskCapacity>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;
	using PayloadList	= TPayloadList;

	using Request		= RequestT<PayloadList>;

	static constexpr LongIndex  STATE_COUNT = StateList::SIZE;
	static constexpr ShortIndex COMPO_COUNT = NCompoCount;

	using StateParents	= StaticArray<Parent, STATE_COUNT>;
	using CompoParents	= StaticArray<Parent, COMPO_COUNT>;

	using CompoForks	= StaticArray<ShortIndex, COMPO_COUNT>;

	using AllForks		= AllForksT<COMPO_COUNT, 0, 0>;

	bool isActive	(const StateID stateId) const;
	bool isResumable(const StateID stateId) const;

	bool isPendingChange(const StateID stateId) const;
	bool isPendingEnter	(const StateID stateId) const;
	bool isPendingExit	(const StateID stateId) const;

	HFSM_INLINE const Parent& forkParent(const ForkID forkId) const;

	HFSM_INLINE ShortIndex    activeCompoProng(const ForkID forkId) const;
	HFSM_INLINE ShortIndex resumableCompoProng(const ForkID forkId) const;
	HFSM_INLINE ShortIndex requestedCompoProng(const ForkID forkId) const;

	HFSM_INLINE ShortIndex& resumableCompoFork(const ForkID forkId);
	HFSM_INLINE ShortIndex& requestedCompoFork(const ForkID forkId);

	void requestImmediate(const Request request);
	void requestScheduled(const StateID stateId);

	HFSM_INLINE void clearOrthoRequested()										{}

	StateParents stateParents;
	CompoParents compoParents;

	CompoForks compoActive{INVALID_SHORT_INDEX};

	AllForks resumable;
	AllForks requested;
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  typename TPayloadList,
		  LongIndex NTaskCapacity>
struct StateRegistryT<ArgsT<TContext,
							TConfig,
							TStateList,
							TRegionList,
							0,
							NOrthoCount,
							NOrthoUnits,
							TPayloadList,
							NTaskCapacity>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;
	using PayloadList	= TPayloadList;

	using Request		= RequestT<PayloadList>;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex ORTHO_COUNT	  = NOrthoCount;
	static constexpr ShortIndex ORTHO_UNITS	  = NOrthoUnits;

	using StateParents	= StaticArray<Parent, STATE_COUNT>;
	using OrthoParents	= StaticArray<Parent, ORTHO_COUNT>;

	using AllForks		= AllForksT<0, ORTHO_COUNT, ORTHO_UNITS>;

	HFSM_INLINE bool isActive   (const StateID) const		{ return true;		}
	HFSM_INLINE bool isResumable(const StateID) const		{ return false;		}

	HFSM_INLINE bool isPendingChange(const StateID) const	{ return false;		}
	HFSM_INLINE bool isPendingEnter	(const StateID) const	{ return false;		}
	HFSM_INLINE bool isPendingExit	(const StateID) const	{ return false;		}

	HFSM_INLINE void requestImmediate(const Request)		{ HFSM_BREAK();		}
	HFSM_INLINE void requestScheduled(const Request)		{ HFSM_BREAK();		}

	HFSM_INLINE void clearOrthoRequested()										{}

	StateParents stateParents;
	OrthoParents orthoParents;

	AllForks resumable;
	AllForks requested;
};

////////////////////////////////////////////////////////////////////////////////

}
}


namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <LongIndex NCC, LongIndex NOC, LongIndex NOU>
void
AllForksT<NCC, NOC, NOU>::clear() {
	compo.fill(INVALID_SHORT_INDEX);
	ortho.clear();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isActive(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong == activeCompoProng(parent.forkId);
	}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isResumable(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong == resumableCompoProng(parent.forkId);
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isPendingChange(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return requestedCompoProng(parent.forkId) != activeCompoProng(parent.forkId);
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isPendingEnter(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong !=	  activeCompoProng(parent.forkId) &&
					   parent.prong == requestedCompoProng(parent.forkId);
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isPendingExit(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong ==	  activeCompoProng(parent.forkId) &&
					   parent.prong != requestedCompoProng(parent.forkId);
		}

	return true;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
const Parent&
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::forkParent(const ForkID forkId) const {
	HFSM_ASSERT(forkId != 0);

	return forkId > 0 ?
		compoParents[ forkId - 1] :
		orthoParents[-forkId - 1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
ForkID
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::parentCompoForkId(const ForkID forkId) const {
	HFSM_ASSERT(forkId != 0);

	for (Parent parent = forkParent(forkId);
		 parent;
		 parent = forkParent(parent.forkId))
	{
		if (parent.forkId > 0)
			return parent.forkId;
	}

	HFSM_BREAK();
	return 0;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
ShortIndex
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::activeCompoProng(const ForkID forkId) const {
	HFSM_ASSERT(forkId > 0);

	return compoActive[forkId - 1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
ShortIndex
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::resumableCompoProng(const ForkID forkId) const {
	HFSM_ASSERT(forkId > 0);

	return resumable.compo[forkId - 1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
ShortIndex
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestedCompoProng(const ForkID forkId) const {
	HFSM_ASSERT(forkId > 0);

	return requested.compo[forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
ShortIndex&
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::resumableCompoFork(const ForkID forkId) {
	HFSM_ASSERT(forkId > 0);

	return resumable.compo[forkId - 1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
OrthoFork&
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::resumableOrthoFork(const ForkID forkId) {
	HFSM_ASSERT(forkId < 0);

	return resumable.ortho[-forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
ShortIndex&
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestedCompoFork(const ForkID forkId) {
	HFSM_ASSERT(forkId > 0);

	return requested.compo[forkId - 1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
OrthoFork&
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestedOrthoFork(const ForkID forkId) {
	HFSM_ASSERT(forkId < 0);

	return requested.ortho[-forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestImmediate(const Request request) {
	if (HFSM_CHECKED(request.stateId < STATE_COUNT)) {
		Parent parent;

		for (parent = stateParents[request.stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			if (parent.forkId > 0) {
				requestedCompoFork(parent.forkId) = parent.prong;

				break;
			} else if (parent.forkId < 0)
				requestedOrthoFork(parent.forkId)[parent.prong] = true;
			else
				HFSM_BREAK();
		}

		for (parent = forkParent(parent.forkId);
			 parent;
			 parent = forkParent(parent.forkId))
		{
			if (parent.forkId > 0) {
				if (   activeCompoProng(parent.forkId) != parent.prong)
					requestedCompoFork (parent.forkId)  = parent.prong;
				else {
					parent = forkParent(parent.forkId);
					break;
				}
			} else if (parent.forkId < 0)
				requestedOrthoFork(parent.forkId)[parent.prong] = true;
			else
				HFSM_BREAK();
		}

		for (; parent; parent = forkParent(parent.forkId)) {
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId < 0)
				requestedOrthoFork(parent.forkId)[parent.prong] = true;
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestScheduled(const StateID stateId) {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT) {
		const Parent parent = stateParents[stateId];

		if (parent.forkId > 0)
			resumableCompoFork(parent.forkId) = parent.prong;
		else if (parent.forkId < 0)
			resumableOrthoFork(parent.forkId)[parent.prong] = true;
		else
			HFSM_BREAK();
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::clearOrthoRequested() {
	for (ForkID i = 0; i < requested.ortho.count(); ++i) {
		OrthoFork& orthoRequested = requested.ortho[i];

		orthoRequested.clear();
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isActive(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId > 0);

			if (parent.forkId > 0)
				return parent.prong == activeCompoProng(parent.forkId);
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isResumable(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong == resumableCompoProng(parent.forkId);
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isPendingChange(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return requestedCompoProng(parent.forkId) != activeCompoProng(parent.forkId);
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isPendingEnter(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong !=    activeCompoProng(parent.forkId) &&
					   parent.prong == requestedCompoProng(parent.forkId);
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isPendingExit(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong ==    activeCompoProng(parent.forkId) &&
					   parent.prong != requestedCompoProng(parent.forkId);
		}

	return true;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
const Parent&
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::forkParent(const ForkID forkId) const {
	HFSM_ASSERT(forkId > 0);

	return compoParents[forkId - 1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
ShortIndex
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::activeCompoProng(const ForkID forkId) const {
	HFSM_ASSERT(forkId > 0);

	return compoActive[forkId - 1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
ShortIndex
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::resumableCompoProng(const ForkID forkId) const {
	HFSM_ASSERT(forkId > 0);

	return resumable.compo[forkId - 1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
ShortIndex
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::requestedCompoProng(const ForkID forkId) const {
	HFSM_ASSERT(forkId > 0);

	return requested.compo[forkId - 1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
ShortIndex&
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::resumableCompoFork(const ForkID forkId) {
	HFSM_ASSERT(forkId > 0);

	return resumable.compo[forkId - 1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
ShortIndex&
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::requestedCompoFork(const ForkID forkId) {
	HFSM_ASSERT(forkId > 0);

	return requested.compo[forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
void
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::requestImmediate(const Request request) {
	if (HFSM_CHECKED(request.stateId < STATE_COUNT)) {
		if (Parent parent = stateParents[request.stateId]) {
			HFSM_ASSERT(parent.forkId > 0);

			requestedCompoFork(parent.forkId) = parent.prong;

			for (parent = forkParent(parent.forkId);
				 parent;
				 parent = forkParent(parent.forkId))
			{
				HFSM_ASSERT(parent.forkId > 0);

				if (   activeCompoProng(parent.forkId) != parent.prong)
					requestedCompoFork (parent.forkId)  = parent.prong;
				else
					break;
			}
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
void
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::requestScheduled(const StateID stateId) {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT) {
		const Parent parent = stateParents[stateId];

		if (HFSM_CHECKED(parent.forkId > 0))
			resumableCompoFork(parent.forkId) = parent.prong;
	}
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class ControlT {
	template <StateID, typename, typename>
	friend struct _S;

	template <StateID, ShortIndex, ShortIndex, typename, typename, typename...>
	friend struct _C;

	template <StateID, ShortIndex, ShortIndex, typename, typename, typename...>
	friend struct _O;

	template <typename, typename, typename, typename>
	friend class _R;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

public:
	using StateRegistry	= StateRegistryT<Args>;
	using PlanData		= PlanDataT <Args>;
	using Plan			= PlanT		<Args>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Origin {
		HFSM_INLINE Origin(ControlT& control_,
						   const StateID id);

		HFSM_INLINE ~Origin();

		ControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		HFSM_INLINE Region(ControlT& control,
						   const RegionID id,
						   const StateID index,
						   const LongIndex size);

		HFSM_INLINE ~Region();

		ControlT& control;
		const RegionID prevId;
		const LongIndex prevIndex;
		const LongIndex prevSize;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	HFSM_INLINE ControlT(Context& context,
						 StateRegistry& stateRegistry,
						 PlanData& planData,
						 LoggerInterface* const HFSM_IF_LOGGER(logger))
		: _context(context)
		, _stateRegistry(stateRegistry)
		, _planData(planData)
		HFSM_IF_LOGGER(, _logger(logger))
	{}

	HFSM_INLINE void setOrigin  (const StateID id);
	HFSM_INLINE void resetOrigin(const StateID id);

	HFSM_INLINE void setRegion  (const RegionID id, const StateID index, const LongIndex size);
	HFSM_INLINE void resetRegion(const RegionID id, const StateID index, const LongIndex size);

	template <typename T>
	static constexpr LongIndex stateId()						{ return StateList ::template index<T>();		}

	template <typename T>
	static constexpr RegionID regionId()						{ return RegionList::template index<T>();		}

public:
	HFSM_INLINE Context& _()									{ return _context;								}
	HFSM_INLINE Context& context()								{ return _context;								}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool isActive   (const StateID stateId) const	{ return _stateRegistry.isActive   (stateId);	}
	HFSM_INLINE bool isResumable(const StateID stateId) const	{ return _stateRegistry.isResumable(stateId);	}

	HFSM_INLINE bool isScheduled(const StateID stateId) const	{ return isResumable(stateId);					}

	template <typename TState>
	HFSM_INLINE bool isActive() const							{ return isActive	(stateId<TState>());		}

	template <typename TState>
	HFSM_INLINE bool isResumable() const						{ return isResumable(stateId<TState>());		}

	template <typename TState>
	HFSM_INLINE bool isScheduled() const						{ return isResumable(stateId<TState>());		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE Plan plan()										{ return Plan{_planData, _regionId};			}
	HFSM_INLINE Plan plan() const								{ return Plan{_planData, _regionId};			}

	HFSM_INLINE Plan plan(const RegionID regionId)				{ return Plan{_planData,  regionId};			}
	HFSM_INLINE Plan plan(const RegionID regionId) const		{ return Plan{_planData,  regionId};			}

	template <typename TRegion>
	HFSM_INLINE Plan plan()										{ return Plan{_planData, regionId<TRegion>()};	}

	template <typename TRegion>
	HFSM_INLINE Plan plan() const								{ return Plan{_planData, regionId<TRegion>()};	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	HFSM_INLINE		  StateRegistry&	stateRegistry()			{ return _stateRegistry;						}
	HFSM_INLINE const StateRegistry&	stateRegistry() const	{ return _stateRegistry;						}

	HFSM_INLINE		  PlanData&	planData()						{ return _planData;								}
	HFSM_INLINE const PlanData&	planData() const				{ return _planData;								}

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	HFSM_INLINE LoggerInterface* logger()						{ return _logger;								}
#endif

protected:
	Context& _context;
	StateRegistry& _stateRegistry;
	PlanData& _planData;
	StateID _originId = INVALID_STATE_ID;
	RegionID _regionId = INVALID_REGION_ID;
	StateID _regionIndex = INVALID_STATE_ID;
	LongIndex _regionSize = INVALID_LONG_INDEX;
	Status _status;
	HFSM_IF_LOGGER(LoggerInterface* _logger);
};

//------------------------------------------------------------------------------

template <typename TArgs>
class FullControlT
	: public ControlT<TArgs>
{
	template <StateID, typename, typename>
	friend struct _S;

	template <StateID, ShortIndex, ShortIndex, typename, typename, typename...>
	friend struct _C;

	template <StateID, ShortIndex, ShortIndex, typename, typename, typename...>
	friend struct _O;

	template <typename, typename, typename, typename>
	friend class _R;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using PayloadList	= typename Args::PayloadList;

	using Control		= ControlT<Args>;
	using Origin		= typename Control::Origin;

	using StateRegistry	= StateRegistryT<Args>;
	using PlanData		= PlanDataT		<Args>;
	using Plan			= PlanT			<Args>;

	using Request		= RequestT <PayloadList>;
	using Requests		= RequestsT<PayloadList, Args::COMPO_COUNT>;

protected:
	using Control::_planData;
	using Control::_originId;
	using Control::_regionId;
	using Control::_regionIndex;
	using Control::_regionSize;
	using Control::_status;
	HFSM_IF_LOGGER(using Control::_logger);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Lock {
		HFSM_INLINE Lock(FullControlT& control_);
		HFSM_INLINE ~Lock();

		FullControlT* const control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	HFSM_INLINE FullControlT(Context& context,
							 StateRegistry& stateRegistry,
							 PlanData& planData,
							 Requests& requests,
							 LoggerInterface* const logger)
		: Control{context, stateRegistry, planData, logger}
		, _requests(requests)
	{}

	template <typename T>
	static constexpr LongIndex stateId()	{ return StateList ::template index<T>();	}

	template <typename T>
	static constexpr LongIndex regionId()	{ return RegionList::template index<T>();	}

	template <typename TState>
	Status updatePlan(TState& headState, const Status subStatus);

	template <typename TState>
	Status buildPlanStatus(const bool outerTransition);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	using Control::isActive;
	using Control::isResumable;
	using Control::isScheduled;
	using Control::plan;

	HFSM_INLINE void changeTo(const StateID stateId);
	HFSM_INLINE void resume	 (const StateID stateId);
	HFSM_INLINE void schedule(const StateID stateId);

	template <typename TState>
	HFSM_INLINE void changeTo()					{ changeTo(stateId<TState>());	}

	template <typename TState>
	HFSM_INLINE void resume()					{ resume  (stateId<TState>());	}

	template <typename TState>
	HFSM_INLINE void schedule()					{ schedule(stateId<TState>());	}

	HFSM_INLINE void succeed();
	HFSM_INLINE void fail();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	Requests& _requests;
	bool _locked = false;
};

//------------------------------------------------------------------------------

template <typename TArgs>
class GuardControlT
	: public FullControlT<TArgs>
{
	template <StateID, typename, typename>
	friend struct _S;

	template <typename, typename, typename, typename>
	friend class _R;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using PayloadList	= typename Args::PayloadList;

	using Control		= ControlT	  <Args>;
	using FullControl	= FullControlT<Args>;

	using StateRegistry	= StateRegistryT<Args>;
	using PlanData		= PlanDataT		<Args>;

public:
	using Requests		= RequestsT<PayloadList, Args::COMPO_COUNT>;

private:
	using FullControl::_stateRegistry;
	using FullControl::_originId;
	HFSM_IF_LOGGER(using Control::_logger);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	HFSM_INLINE GuardControlT(Context& context,
							  StateRegistry& stateRegistry,
							  PlanData& planData,
							  Requests& requests,
							  const Requests& pendingChanges,
							  LoggerInterface* const logger)
		: FullControl{context, stateRegistry, planData, requests, logger}
		, _pending(pendingChanges)
	{}

	template <typename T>
	static constexpr LongIndex stateId()							{ return StateList::template index<T>();			}

	HFSM_INLINE bool cancelled() const								{ return _cancelled;								}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	using Control::isActive;
	using Control::isResumable;
	using Control::isScheduled;
	using Control::plan;

	HFSM_INLINE bool isPendingChange(const StateID stateId) const	{ return _stateRegistry.isPendingChange(stateId);	}
	HFSM_INLINE bool isPendingEnter	(const StateID stateId) const	{ return _stateRegistry.isPendingEnter (stateId);	}
	HFSM_INLINE bool isPendingExit	(const StateID stateId) const	{ return _stateRegistry.isPendingExit  (stateId);	}

	template <typename TState>
	HFSM_INLINE bool isPendingChange()								{ return isPendingChange(stateId<TState>());		}

	template <typename TState>
	HFSM_INLINE bool isPendingEnter()								{ return isPendingEnter (stateId<TState>());		}

	template <typename TState>
	HFSM_INLINE bool isPendingExit()								{ return isPendingExit  (stateId<TState>());		}

	HFSM_INLINE void cancelPendingChanges();

	HFSM_INLINE const Requests& pendingChanges() const				{ return _pending;									}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	bool _cancelled = false;
	const Requests& _pending;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
ControlT<TA>::Origin::Origin(ControlT& control_,
							 const StateID id)
	: control{control_}
	, prevId(control._originId)
{
	control.setOrigin(id);
}

//------------------------------------------------------------------------------

template <typename TA>
ControlT<TA>::Origin::~Origin() {
	control.resetOrigin(prevId);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
ControlT<TA>::Region::Region(ControlT& control_,
							 const RegionID id,
							 const StateID index,
							 const LongIndex size)
	: control{control_}
	, prevId(control._regionId)
	, prevIndex(control._regionIndex)
	, prevSize(control._regionSize)
{
	control.setRegion(id, index, size);
}

//------------------------------------------------------------------------------

template <typename TA>
ControlT<TA>::Region::~Region() {
	control.resetRegion(prevId, prevIndex, prevSize);

	control._status.clear();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
void
ControlT<TA>::setOrigin(const StateID id) {
	// TODO: see if this still can be used
	//HFSM_ASSERT(_regionId != INVALID_STATE_ID && _regionSize != INVALID_LONG_INDEX);
	//HFSM_ASSERT(_regionId < StateList::SIZE && _regionId + _regionSize <= StateList::SIZE);

	HFSM_ASSERT(id != INVALID_STATE_ID);

	_originId = id;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
ControlT<TA>::resetOrigin(const StateID id) {
	// TODO: see if this still can be used
	//HFSM_ASSERT(_regionId != INVALID_STATE_ID && _regionSize != INVALID_LONG_INDEX);
	//HFSM_ASSERT(_regionId < StateList::SIZE && _regionId + _regionSize <= StateList::SIZE);

	HFSM_ASSERT(_originId != INVALID_STATE_ID);

	_originId = id;
}

//------------------------------------------------------------------------------

template <typename TA>
void
ControlT<TA>::setRegion(const RegionID id,
						const StateID index,
						const LongIndex size)
{
	HFSM_ASSERT(index != INVALID_STATE_ID && size != INVALID_LONG_INDEX);

	if (_regionId == INVALID_REGION_ID) {
		HFSM_ASSERT(_regionIndex == INVALID_STATE_ID);
		HFSM_ASSERT(_regionSize  == INVALID_LONG_INDEX);
		HFSM_ASSERT(index < StateList::SIZE && index + size <= StateList::SIZE);
	} else {
		HFSM_ASSERT(_regionIndex != INVALID_STATE_ID);
		HFSM_ASSERT(_regionSize  != INVALID_LONG_INDEX);
		HFSM_ASSERT(_regionIndex <= index && index + size <= _regionIndex + _regionSize);
	}

	HFSM_ASSERT(_originId == INVALID_STATE_ID);

	_regionId	 = id;
	_regionIndex = index;
	_regionSize	 = size;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
ControlT<TA>::resetRegion(const RegionID id,
						  const StateID index,
						  const LongIndex size)
{
	HFSM_ASSERT(_regionId	 != INVALID_REGION_ID);
	HFSM_ASSERT(_regionIndex != INVALID_STATE_ID);
	HFSM_ASSERT(_regionSize  != INVALID_LONG_INDEX);

	if (index == INVALID_STATE_ID)
		HFSM_ASSERT(size == INVALID_LONG_INDEX);
	else
		HFSM_ASSERT(size != INVALID_LONG_INDEX);

	_regionId	 = id;
	_regionIndex = index;
	_regionSize	 = size;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
FullControlT<TA>::Lock::Lock(FullControlT& control_)
	: control(!control_._locked ? &control_ : nullptr)
{
	if (control)
		control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TA>
FullControlT<TA>::Lock::~Lock() {
	if (control)
		control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
template <typename TState>
Status
FullControlT<TA>::updatePlan(TState& headState,
							 const Status subStatus)
{
	using State = TState;
	static constexpr StateID STATE_ID = State::STATE_ID;

	HFSM_ASSERT(subStatus);

	if (subStatus.failure) {
		headState.wrapPlanFailed(*this);

		return buildPlanStatus<State>(subStatus.outerTransition);
	} else if (subStatus.success) {
		if (Plan p = plan(_regionId)) {
			for (auto it = p.begin(); it; ++it) {
				if (isActive(it->origin) &&
					_planData.tasksSuccesses[it->origin])
				{
					Origin origin{*this, STATE_ID};

					changeTo(it->destination);

					it.remove();
				} else
					break;
			}

			return {false, false, subStatus.outerTransition};
		} else {
			headState.wrapPlanSucceeded(*this);

			return buildPlanStatus<State>(subStatus.outerTransition);
		}
	} else
		return {false, false, subStatus.outerTransition};
}

//------------------------------------------------------------------------------

template <typename TA>
template <typename TState>
Status
FullControlT<TA>::buildPlanStatus(const bool outerTransition) {
	using State = TState;
	static constexpr StateID STATE_ID = State::STATE_ID;

	if (_status.failure) {
		_planData.tasksFailures[STATE_ID] = true;

		HFSM_LOG_PLAN_STATUS(_regionId, StatusEvent::FAILED);
		return {false, true,  outerTransition};
	} else if (_status.success) {
		_planData.tasksSuccesses[STATE_ID] = true;

		HFSM_LOG_PLAN_STATUS(_regionId, StatusEvent::SUCCEEDED);
		return {true,  false, outerTransition};
	} else
		return {false, false, outerTransition};
}

//------------------------------------------------------------------------------

template <typename TA>
void
FullControlT<TA>::changeTo(const StateID stateId) {
	if (!_locked) {
		const Request request{Request::Type::RESTART, stateId};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

	#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
		if (_logger)
			_logger->recordTransition(_originId, Transition::RESTART, stateId);
	#endif
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::resume(const StateID stateId) {
	if (!_locked) {
		const Request request{Request::Type::RESUME, stateId};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

	#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
		if (_logger)
			_logger->recordTransition(_originId, Transition::RESUME, stateId);
	#endif
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::schedule(const StateID stateId) {
	const Request transition{Request::Type::SCHEDULE, stateId};
	_requests << transition;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(_originId, Transition::SCHEDULE, stateId);
#endif
}

//------------------------------------------------------------------------------

template <typename TA>
void
FullControlT<TA>::succeed() {
	_status.success = true;

	_planData.tasksSuccesses[_originId] = true;

	HFSM_LOG_TASK_STATUS(_regionId, _originId, StatusEvent::SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::fail() {
	_status.failure = true;

	_planData.tasksFailures [_originId] = true;

	HFSM_LOG_TASK_STATUS(_regionId, _originId, StatusEvent::FAILED);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
void
GuardControlT<TA>::cancelPendingChanges() {
	_cancelled = true;

	HFSM_LOG_CANCELLED_PENDING(_originId);
}

////////////////////////////////////////////////////////////////////////////////

}
}

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

namespace hfsm2 {

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

	HFSM_INLINE StructureStateInfo(const LongIndex parent_,
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
Transition
HFSM_INLINE get(const typename RequestT<TPayloadList>::Type type) {
	using Request = RequestT<TPayloadList>;

	switch (type) {
		case Request::RESTART:
			return Transition::RESTART;
		case Request::RESUME:
			return Transition::RESUME;
		case Request::SCHEDULE:
			return Transition::SCHEDULE;
		default:
			HFSM_BREAK();
			return Transition::RESTART;
	}
}

#pragma pack(push, 1)

template <typename TPayloadList>
struct alignas(4) TransitionInfoT {
	using PayloadList = TPayloadList;
	using Request	  = RequestT<PayloadList>;

	HFSM_INLINE TransitionInfoT() = default;

	HFSM_INLINE TransitionInfoT(const Request transition_,
								const Method method_)
		: stateId{transition_.stateId}
		, method(method_)
		, transition(get<PayloadList>(transition_.type))
	{
		HFSM_ASSERT(method_ < Method::COUNT);
	}

	StateID stateId = INVALID_STATE_ID;
	Method method;
	Transition transition;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class BareT {
	template <typename...>
	friend struct _B;

protected:
	using Args				= TArgs;
	using Context			= typename Args::Context;
	using StateList			= typename Args::StateList;
	using RegionList		= typename Args::RegionList;

	using Control			= ControlT	   <Args>;
	using FullControl		= FullControlT <Args>;
	using GuardControl		= GuardControlT<Args>;

	using Plan				= typename Control::Plan;

public:
	HFSM_INLINE void preEntryGuard(Context&)									{}
	HFSM_INLINE void preEnter	  (Context&)									{}
	HFSM_INLINE void preUpdate	  (Context&)									{}
	template <typename TEvent>
	HFSM_INLINE void preReact	  (const TEvent&,
								   Context&)									{}
	HFSM_INLINE void preExitGuard (Context&)									{}
	HFSM_INLINE void postExit	  (Context&)									{}
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

	HFSM_INLINE void widePreEntryGuard(typename First::Context& context);
	HFSM_INLINE void widePreEnter	  (typename First::Context& context);
	HFSM_INLINE void widePreUpdate	  (typename First::Context& context);
	template <typename TEvent>
	HFSM_INLINE void widePreReact	  (const TEvent& event,
									   typename First::Context& context);
	HFSM_INLINE void widePreExitGuard (typename First::Context& context);
	HFSM_INLINE void widePostExit	  (typename First::Context& context);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TFirst>
struct _B<TFirst>
	: TFirst
{
	using First				= TFirst;

	using StateList			= typename First::StateList;
	using RegionList		= typename First::RegionList;

	HFSM_INLINE void entryGuard		  (typename First::GuardControl&)			{}
	HFSM_INLINE void enter			  (typename First::Control&)				{}
	HFSM_INLINE void update			  (typename First::FullControl&)			{}
	template <typename TEvent>
	HFSM_INLINE void react			  (const TEvent&,
									   typename First::FullControl&)			{}
	HFSM_INLINE void exitGuard		  (typename First::GuardControl&)			{}
	HFSM_INLINE void exit			  (typename First::Control&)				{}

	HFSM_INLINE void planSucceeded	  (typename First::FullControl& control) { control.succeed(); }
	HFSM_INLINE void planFailed		  (typename First::FullControl& control) { control.fail();	  }

	HFSM_INLINE void widePreEntryGuard(typename First::Context& context);
	HFSM_INLINE void widePreEnter	  (typename First::Context& context);
	HFSM_INLINE void widePreUpdate	  (typename First::Context& context);
	template <typename TEvent>
	HFSM_INLINE void widePreReact	  (const TEvent& event,
									   typename First::Context& context);
	HFSM_INLINE void widePreExitGuard (typename First::Context& context);
	HFSM_INLINE void widePostExit	  (typename First::Context& context);

	template <typename T>
	static constexpr LongIndex stateId()  { return StateList ::template index<T>();	}

	template <typename T>
	static constexpr LongIndex regionId() { return RegionList::template index<T>();	}
};

template <typename TArgs>
using Empty = _B<BareT<TArgs>>;

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TF, typename... TR>
void
_B<TF, TR...>::widePreEntryGuard(typename TF::Context& context) {
	TF::preEntryGuard(context);
	_B<TR...>::widePreEntryGuard(context);
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
_B<TF, TR...>::widePreExitGuard(typename TF::Context& context) {
	TF::preExitGuard(context);
	_B<TR...>::widePreExitGuard(context);
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
_B<TF>::widePreEntryGuard(typename TF::Context& context) {
	TF::preEntryGuard(context);
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
_B<TF>::widePreExitGuard(typename TF::Context& context) {
	TF::preExitGuard(context);
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

namespace hfsm2 {

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
struct WrapT;

template <typename TH>
struct WrapT<TH> {
	using Type = _SF<TH>;
};

template <typename... TS>
struct WrapT<_CF<TS...>> {
	using Type = _CF<TS...>;
};

template <typename... TS>
struct WrapT<_OF<TS...>> {
	using Type = _OF<TS...>;
};

template <typename... TS>
using Wrap = typename WrapT<TS...>::Type;

//------------------------------------------------------------------------------

template <typename THead>
struct _SF final {
	using Head				= THead;
	using StateList			= _TL<Head>;
	using RegionList		= _TL<>;

	static constexpr ShortIndex WIDTH		  = 1;
	static constexpr LongIndex  REVERSE_DEPTH = 1;
	static constexpr ShortIndex COMPO_COUNT	  = 0;
	static constexpr ShortIndex ORTHO_COUNT	  = 0;
	static constexpr ShortIndex ORTHO_UNITS	  = 0;
	static constexpr LongIndex  PRONG_COUNT	  = 0;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct _CSF<TInitial, TRemaining...> {
	using Initial			= Wrap<TInitial>;
	using Remaining			= _CSF<TRemaining...>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;
	using RegionList		= Merge<typename Initial::RegionList, typename Remaining::RegionList>;

	static constexpr LongIndex  REVERSE_DEPTH = Max<Initial::REVERSE_DEPTH, Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr ShortIndex COMPO_COUNT	  =		Initial::COMPO_COUNT +	Remaining::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT	  =		Initial::ORTHO_COUNT +	Remaining::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS	  =		Initial::ORTHO_UNITS +	Remaining::ORTHO_UNITS;
	static constexpr LongIndex  PRONG_COUNT	  =		Initial::PRONG_COUNT +	Remaining::PRONG_COUNT;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
};

template <typename TInitial>
struct _CSF<TInitial> {
	using Initial			= Wrap<TInitial>;
	using StateList			= typename Initial::StateList;
	using RegionList		= typename Initial::RegionList;

	static constexpr LongIndex  REVERSE_DEPTH = Initial::REVERSE_DEPTH;
	static constexpr ShortIndex COMPO_COUNT	  = Initial::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT	  = Initial::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS	  = Initial::ORTHO_UNITS;
	static constexpr LongIndex  PRONG_COUNT	  = Initial::PRONG_COUNT;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
};

template <typename THead, typename... TSubStates>
struct _CF final {
	using Head				= THead;
	using State				= _SF<Head>;
	using SubStates			= _CSF<TSubStates...>;
	using StateList			= Merge<typename State::StateList, typename SubStates::StateList>;
	using RegionList		= Merge<typename State::StateList, typename SubStates::RegionList>;

	static constexpr ShortIndex WIDTH		  = sizeof...(TSubStates);
	static constexpr LongIndex  REVERSE_DEPTH = SubStates::REVERSE_DEPTH + 1;
	static constexpr ShortIndex COMPO_COUNT	  = SubStates::COMPO_COUNT + 1;
	static constexpr ShortIndex ORTHO_COUNT	  = SubStates::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS	  = SubStates::ORTHO_UNITS;
	static constexpr LongIndex  PRONG_COUNT	  = WIDTH + SubStates::PRONG_COUNT;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct _OSF<TInitial, TRemaining...> {
	using Initial			= Wrap<TInitial>;
	using Remaining			= _OSF<TRemaining...>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;
	using RegionList		= Merge<typename Initial::RegionList, typename Remaining::RegionList>;

	static constexpr LongIndex  REVERSE_DEPTH = Max<Initial::REVERSE_DEPTH, Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr ShortIndex COMPO_COUNT	  =		Initial::COMPO_COUNT +	Remaining::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT	  =		Initial::ORTHO_COUNT +	Remaining::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS	  =		Initial::ORTHO_UNITS +	Remaining::ORTHO_UNITS;
	static constexpr LongIndex  PRONG_COUNT	  =		Initial::PRONG_COUNT +	Remaining::PRONG_COUNT;
};

template <typename TInitial>
struct _OSF<TInitial> {
	using Initial			= Wrap<TInitial>;
	using StateList			= typename Initial::StateList;
	using RegionList		= typename Initial::RegionList;

	static constexpr LongIndex  REVERSE_DEPTH = Initial::REVERSE_DEPTH;
	static constexpr ShortIndex COMPO_COUNT	  = Initial::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT	  = Initial::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS	  = Initial::ORTHO_UNITS;
	static constexpr LongIndex  PRONG_COUNT	  = Initial::PRONG_COUNT;
};

template <typename THead, typename... TSubStates>
struct _OF final {
	using Head				= THead;
	using State				= _SF<Head>;
	using SubStates			= _OSF<TSubStates...>;
	using StateList			= Merge<typename State::StateList, typename SubStates::StateList>;
	using RegionList		= Merge<typename State::StateList, typename SubStates::RegionList>;

	static constexpr ShortIndex WIDTH		  = sizeof...(TSubStates);
	static constexpr LongIndex  REVERSE_DEPTH = SubStates::REVERSE_DEPTH + 1;
	static constexpr ShortIndex COMPO_COUNT	  = SubStates::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT	  = SubStates::ORTHO_COUNT + 1;
	static constexpr ShortIndex ORTHO_UNITS	  = SubStates::ORTHO_UNITS + (WIDTH + 7) / 8;
	static constexpr LongIndex  PRONG_COUNT	  = SubStates::PRONG_COUNT;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NCompoCount,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  typename TPayloadList,
		  LongIndex NTaskCapacity>
struct ArgsT final {
	using Context			= TContext;
	using Config			= TConfig;
	using StateList			= TStateList;
	using RegionList		= TRegionList;
	using PayloadList		= TPayloadList;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex COMPO_COUNT	  = NCompoCount;
	static constexpr ShortIndex ORTHO_COUNT	  = NOrthoCount;
	static constexpr ShortIndex ORTHO_UNITS	  = NOrthoUnits;
	static constexpr LongIndex  TASK_CAPACITY = NTaskCapacity;
};

//------------------------------------------------------------------------------

template <StateID, typename, typename>
struct _S;

template <StateID, ShortIndex, ShortIndex, typename, typename, typename...>
struct _C;

template <StateID, ShortIndex, ShortIndex, typename, ShortIndex, typename...>
struct _CS;

template <StateID, ShortIndex, ShortIndex, typename, typename, typename...>
struct _O;

template <typename, typename, typename, typename>
class _R;

//------------------------------------------------------------------------------

template <StateID, ShortIndex, ShortIndex, typename...>
struct MaterialT;

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH>
struct MaterialT   <NS, NC, NO, TA, TH> {
	using Type = _S<NS,			TA, TH>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA,			  typename... TS>
struct MaterialT   <NS, NC, NO, TA, _CF<void, TS...>> {
	using Type = _C<NS, NC, NO, TA, Empty<TA>, TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
struct MaterialT   <NS, NC, NO, TA, _CF<TH,	 TS...>> {
	using Type = _C<NS, NC, NO, TA,		TH,	 TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA,			  typename... TS>
struct MaterialT   <NS, NC, NO, TA, _OF<void,  TS...>> {
	using Type = _O<NS, NC, NO, TA, Empty<TA>, TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
struct MaterialT   <NS, NC, NO, TA, _OF<TH,	 TS...>> {
	using Type = _O<NS, NC, NO, TA,		TH,	 TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename... TS>
using Material = typename MaterialT<NS, NC, NO, TS...>::Type;

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TPayloadList,
		  typename TApex>
struct _RF final {
	using Context			= TContext;
	using Config			= TConfig;
	using PayloadList		= TPayloadList;
	using Apex				= TApex;

	static constexpr LongIndex MAX_PLAN_TASKS	 = Config::MAX_PLAN_TASKS;
	static constexpr LongIndex MAX_SUBSTITUTIONS = Config::MAX_SUBSTITUTIONS;

	static constexpr LongIndex TASK_CAPACITY	 = Config::MAX_PLAN_TASKS != INVALID_LONG_INDEX ?
													   Config::MAX_PLAN_TASKS : Apex::PRONG_COUNT * 2;

	using Instance			= _R<Context, Config, PayloadList, Apex>;

	static constexpr ShortIndex COMPO_COUNT		 = Apex::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT		 = Apex::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS		 = Apex::ORTHO_UNITS;

	using StateList			= Indexed<typename Apex::StateList>;
	using RegionList		= Indexed<typename Apex::RegionList>;

	using Args				= ArgsT<Context,
									Config,
									StateList,
									RegionList,
									COMPO_COUNT,
									ORTHO_COUNT,
									ORTHO_UNITS,
									PayloadList,
									TASK_CAPACITY>;

	using Control			= ControlT	   <Args>;
	using FullControl		= FullControlT <Args>;
	using GuardControl		= GuardControlT<Args>;

	using Bare				= BareT<Args>;
	using State				= Empty<Args>;

	template <typename... TInjections>
	using BaseT				= _B<TInjections...>;

	template <typename T>
	static constexpr LongIndex stateId() {
		return StateList::template index<T>();
	}

	template <typename T>
	static constexpr bool contains() {
		return StateList::template index<T>() != INVALID_LONG_INDEX;
	}

	template <typename T>
	static constexpr LongIndex regionId() {
		return RegionList::template index<T>();
	}
};

////////////////////////////////////////////////////////////////////////////////

template <StateID, ShortIndex, ShortIndex, typename, ShortIndex, typename...>
struct CSubMaterialT;

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename T>
struct CSubMaterialT	 <NS, NC, NO, TA, NI, _TL<T>> {
	using Type = Material<NS, NC, NO, TA,		  T>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
struct CSubMaterialT<NS, NC, NO, TA, NI, _TL<TS...>> {
	using Type = _CS<NS, NC, NO, TA, NI,	 TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
using CSubMaterial = typename CSubMaterialT<NS, NC, NO, TA, NI, TS...>::Type;

//------------------------------------------------------------------------------

template <typename...>
struct ForwardT;

template <StateID NS, typename TA, typename TH>
struct ForwardT<  _S<NS, TA, TH>> {
	using Type = _SF<		 TH>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
struct ForwardT<_C<NS, NC, NO, TA, TH, TS...>> {
	using Type = _CF<TH, TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
struct ForwardT<_O<NS, NC, NO, TA, TH, TS...>> {
	using Type = _OF<TH, TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
struct ForwardT<_CS<NS, NC, NO, TA, NI, TS...>> {
	using Type = _CSF<TS...>;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TConfig = Config<>,
		  typename TPayloadList = _ITL<>>
struct _M {
	using Context = TContext;
	using Config  = TConfig;

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
using TransitionPayloads = detail::_ITL<Ts...>;

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

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TStateID,
		  typename TArgs,
		  typename THead>
struct _S {
	static constexpr StateID STATE_ID = TStateID;

	using Args			= TArgs;
	using Head			= THead;

	using PayloadList	= typename Args::PayloadList;

	using Request		= RequestT<PayloadList>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;

	using Control		= ControlT		<Args>;
	using ScopedOrigin	= typename Control::Origin;

	using FullControl	= FullControlT	<Args>;
	using GuardControl	= GuardControlT	<Args>;

	using Empty			= ::hfsm2::detail::Empty<Args>;

	HFSM_INLINE void   deepRegister			(StateRegistry& stateRegistry, const Parent parent);

	HFSM_INLINE bool   deepForwardEntryGuard(GuardControl&,								const ShortIndex = INVALID_SHORT_INDEX)	{ return false; }
	HFSM_INLINE bool   deepEntryGuard		(GuardControl& control,						const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepEnterInitial		(Control& control);
	HFSM_INLINE void   deepEnter			(Control& control,							const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE Status deepUpdate			(FullControl& control,						const ShortIndex = INVALID_SHORT_INDEX);

	template <typename TEvent>
	HFSM_INLINE void   deepReact			(FullControl& control, const TEvent& event, const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE bool   deepForwardExitGuard	(GuardControl&,								const ShortIndex = INVALID_SHORT_INDEX)	{ return false; }
	HFSM_INLINE bool   deepExitGuard		(GuardControl& control,						const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepExit				(Control& control,							const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   wrapPlanSucceeded	(FullControl& control);
	HFSM_INLINE void   wrapPlanFailed		(FullControl& control);

	HFSM_INLINE void   deepForwardActive	(StateRegistry&,	   const RequestType,	const ShortIndex = INVALID_SHORT_INDEX)	{}
	HFSM_INLINE void   deepForwardRequest	(StateRegistry&,	   const RequestType,	const ShortIndex = INVALID_SHORT_INDEX)	{}
	HFSM_INLINE void   deepRequestRemain	(StateRegistry&)															{}
	HFSM_INLINE void   deepRequestRestart	(StateRegistry&)															{}
	HFSM_INLINE void   deepRequestResume	(StateRegistry&,							const ShortIndex = INVALID_SHORT_INDEX)	{}
	HFSM_INLINE void   deepChangeToRequested(StateRegistry&, Control&,					const ShortIndex = INVALID_SHORT_INDEX)	{}

#if defined _DEBUG || defined HFSM_ENABLE_STRUCTURE_REPORT || defined HFSM_ENABLE_LOG_INTERFACE
	static constexpr bool isBare()	 { return std::is_same<Head, Empty>::value;	 }

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

	template <typename TMethodType, Method>
	typename std::enable_if< std::is_same<typename MemberTraits<TMethodType>::State, Empty>::value>::type
	log(LoggerInterface&) const {}

	template <typename TMethodType, Method TMethodId>
	typename std::enable_if<!std::is_same<typename MemberTraits<TMethodType>::State, Empty>::value>::type
	log(LoggerInterface& logger) const {
		logger.recordMethod(STATE_ID, TMethodId);
	}
#endif

	// VS	 - error C2079: 'hfsm2::detail::_S<BLAH>::_head' uses undefined struct 'Blah'
	// Clang - error : field has incomplete type 'hfsm2::detail::_S<BLAH>::Head' (aka 'Blah')
	//
	// State 'Blah' hasn't been defined
	Head _head;
	HFSM_IF_DEBUG(const std::type_index TYPE = isBare() ? typeid(None) : typeid(Head));
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

namespace {

template <StateID NS, typename TA, typename TH>
struct RegisterT {
	using StateParents	= StaticArray<Parent, TA::STATE_COUNT>;
	using StateList		= typename TA::StateList;

	static constexpr StateID STATE_ID = NS;

	static HFSM_INLINE
	void
	execute(StateParents& stateParents,
			const Parent parent)
	{
		static constexpr auto HEAD_ID = StateList::template index<TH>();
		StaticAssertEquality<STATE_ID, HEAD_ID>();

		stateParents[STATE_ID] = parent;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, typename TA>
struct RegisterT<NS, TA, Empty<TA>> {
	using StateParents	= StaticArray<Parent, TA::STATE_COUNT>;

	static HFSM_INLINE
	void
	execute(StateParents&, const Parent) {}
};

}

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::deepRegister(StateRegistry& stateRegistry,
							 const Parent parent)
{
	using Register = RegisterT<STATE_ID, TA, Head>;
	Register::execute(stateRegistry.stateParents, parent);
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
bool
_S<NS, TA, TH>::deepEntryGuard(GuardControl& control,
							   const ShortIndex /*prong*/)
{
	HFSM_LOG_STATE_METHOD(&Head::entryGuard, Method::ENTRY_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control.cancelled();

	_head.widePreEntryGuard(control.context());
	_head.entryGuard(control);

	return !cancelledBefore && control.cancelled();
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::deepEnterInitial(Control& control) {
	deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::deepEnter(Control& control,
						  const ShortIndex /*prong*/)
{
	HFSM_ASSERT(!control.planData().hasSucceeded(STATE_ID));
	HFSM_ASSERT(!control.planData().hasFailed   (STATE_ID));

	HFSM_LOG_STATE_METHOD(&Head::enter, Method::ENTER);

	ScopedOrigin origin{control, STATE_ID};

	_head.widePreEnter(control.context());
	_head.enter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
Status
_S<NS, TA, TH>::deepUpdate(FullControl& control,
						   const ShortIndex /*prong*/)
{
	HFSM_LOG_STATE_METHOD(&Head::update, Method::UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	_head.widePreUpdate(control.context());
	_head.update(control);

	return control._status;
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
template <typename TEvent>
void
_S<NS, TA, TH>::deepReact(FullControl& control,
						  const TEvent& event,
						  const ShortIndex /*prong*/)
{
	auto reaction = static_cast<void(Head::*)(const TEvent&, FullControl&)>(&Head::react);
	HFSM_LOG_STATE_METHOD(reaction, Method::REACT);

	ScopedOrigin origin{control, STATE_ID};

	_head.widePreReact(event, control.context());

	(_head.*reaction)(event, control);		//_head.react(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
bool
_S<NS, TA, TH>::deepExitGuard(GuardControl& control,
							  const ShortIndex /*prong*/)
{
	HFSM_LOG_STATE_METHOD(&Head::exitGuard, Method::EXIT_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control.cancelled();

	_head.widePreExitGuard(control.context());
	_head.exitGuard(control);

	return !cancelledBefore && control.cancelled();
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::deepExit(Control& control,
						 const ShortIndex /*prong*/)
{
	HFSM_LOG_STATE_METHOD(&Head::exit, Method::EXIT);

	ScopedOrigin origin{control, STATE_ID};

	_head.exit(control);
	_head.widePostExit(control.context());

	control.planData().setSuccessful(STATE_ID, false);
	control.planData().setFailed	(STATE_ID, false);
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::wrapPlanSucceeded(FullControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::planSucceeded, Method::PLAN_SUCCEEDED);

	ScopedOrigin origin{control, STATE_ID};

	_head.planSucceeded(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::wrapPlanFailed(FullControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::planFailed, Method::PLAN_FAILED);

	ScopedOrigin origin{control, STATE_ID};

	_head.planFailed(control);
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
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NInitialID,
		  ShortIndex NCompoIndex,
		  ShortIndex NOrthoIndex,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename... TStates>
struct _CS {
	static constexpr StateID	INITIAL_ID	= NInitialID;
	static constexpr ShortIndex COMPO_INDEX	= NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	= NOrthoIndex;
	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			= TArgs;

	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using PayloadList	= typename Args::PayloadList;

	using Request		= RequestT<PayloadList>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;

	using Control		= ControlT	   <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	static constexpr ShortIndex L_PRONG = PRONG_INDEX;

	using LStates		= SplitL<TStates...>;
	using LHalf			= CSubMaterial<INITIAL_ID,
									   COMPO_INDEX,
									   ORTHO_INDEX,
									   Args,
									   L_PRONG,
									   LStates>;
	using LHalfForward	= typename ForwardT<LHalf>::Type;

	static constexpr ShortIndex R_PRONG = PRONG_INDEX + LStates::SIZE;

	using RStates		= SplitR<TStates...>;
	using RHalf			= CSubMaterial<INITIAL_ID  + LHalfForward::STATE_COUNT,
									   COMPO_INDEX + LHalfForward::COMPO_COUNT,
									   ORTHO_INDEX + LHalfForward::ORTHO_COUNT,
									   Args,
									   R_PRONG,
									   RStates>;

	HFSM_INLINE void   deepRegister			(StateRegistry& stateRegistry, const Parent parent);

	HFSM_INLINE bool   deepForwardEntryGuard(GuardControl& control,									  const ShortIndex prong);
	HFSM_INLINE bool   deepEntryGuard		(GuardControl& control,									  const ShortIndex prong);

	HFSM_INLINE void   deepEnterInitial		(Control& control);
	HFSM_INLINE void   deepEnter			(Control& control,										  const ShortIndex prong);

	HFSM_INLINE Status deepUpdate			(FullControl& control,									  const ShortIndex prong);

	template <typename TEvent>
	HFSM_INLINE void   deepReact			(FullControl& control,		   const TEvent& event,		  const ShortIndex prong);

	HFSM_INLINE bool   deepForwardExitGuard	(GuardControl& control,									  const ShortIndex prong);
	HFSM_INLINE bool   deepExitGuard		(GuardControl& control,									  const ShortIndex prong);

	HFSM_INLINE void   deepExit				(Control& control,										  const ShortIndex prong);

	HFSM_INLINE void   deepForwardActive	(StateRegistry& stateRegistry, const RequestType request, const ShortIndex prong);
	HFSM_INLINE void   deepForwardRequest	(StateRegistry& stateRegistry, const RequestType request, const ShortIndex prong);
	HFSM_INLINE void   deepRequestRemain	(StateRegistry& stateRegistry);
	HFSM_INLINE void   deepRequestRestart	(StateRegistry& stateRegistry);
	HFSM_INLINE void   deepRequestResume	(StateRegistry& stateRegistry,							  const ShortIndex prong);

	HFSM_INLINE void   deepChangeToRequested(StateRegistry& stateRegistry, Control& control,		  const ShortIndex prong);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using RegionType		= typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT = LHalf::NAME_COUNT + RHalf::NAME_COUNT;

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	LHalf lHalf;
	RHalf rHalf;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepRegister(StateRegistry& stateRegistry,
											 const Parent parent)
{
	lHalf.deepRegister(stateRegistry, Parent{parent.forkId, L_PRONG});
	rHalf.deepRegister(stateRegistry, Parent{parent.forkId, R_PRONG});
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
bool
_CS<NS, NC, NO, TA, NI, TS...>::deepForwardEntryGuard(GuardControl& control,
													  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.deepForwardEntryGuard(control, prong);
	else
		return rHalf.deepForwardEntryGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
bool
_CS<NS, NC, NO, TA, NI, TS...>::deepEntryGuard(GuardControl& control,
											   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.deepEntryGuard(control, prong);
	else
		return rHalf.deepEntryGuard(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepEnterInitial(Control& control) {
	lHalf.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepEnter(Control& control,
										  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepEnter(control, prong);
	else
		rHalf.deepEnter(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
Status
_CS<NS, NC, NO, TA, NI, TS...>::deepUpdate(FullControl& control,
										   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	return prong < R_PRONG ?
		lHalf.deepUpdate(control, prong) :
		rHalf.deepUpdate(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
template <typename TEvent>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepReact(FullControl& control,
										  const TEvent& event,
										  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepReact(control, event, prong);
	else
		rHalf.deepReact(control, event, prong);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
bool
_CS<NS, NC, NO, TA, NI, TS...>::deepForwardExitGuard(GuardControl& control,
													 const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.deepForwardExitGuard(control, prong);
	else
		return rHalf.deepForwardExitGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
bool
_CS<NS, NC, NO, TA, NI, TS...>::deepExitGuard(GuardControl& control,
											  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.deepExitGuard(control, prong);
	else
		return rHalf.deepExitGuard(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepExit(Control& control,
										 const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepExit(control, prong);
	else
		rHalf.deepExit(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepForwardActive(StateRegistry& stateRegistry,
												  const RequestType request,
												  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepForwardActive(stateRegistry, request, prong);
	else
		rHalf.deepForwardActive(stateRegistry, request, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepForwardRequest(StateRegistry& stateRegistry,
												   const RequestType request,
												   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepForwardRequest(stateRegistry, request, prong);
	else
		rHalf.deepForwardRequest(stateRegistry, request, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepRequestRemain(StateRegistry& stateRegistry) {
	lHalf.deepRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepRequestRestart(StateRegistry& stateRegistry) {
	lHalf.deepRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepRequestResume(StateRegistry& stateRegistry,
												  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepRequestResume(stateRegistry, prong);
	else
		rHalf.deepRequestResume(stateRegistry, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepChangeToRequested(StateRegistry& stateRegistry,
													  Control& control,
													  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepChangeToRequested(stateRegistry, control, prong);
	else
		rHalf.deepChangeToRequested(stateRegistry, control, prong);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepGetNames(const LongIndex parent,
											 const RegionType /*region*/,
											 const ShortIndex depth,
											 StructureStateInfos& _stateInfos) const
{
	lHalf.deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
	rHalf.deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NHeadIndex,
		  ShortIndex NCompoIndex,
		  ShortIndex NOrthoIndex,
		  typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct _C {
	static constexpr StateID	HEAD_ID		= NHeadIndex;
	static constexpr ShortIndex COMPO_INDEX	= NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	= NOrthoIndex;
	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID		COMPO_ID	= COMPO_INDEX + 1;

	using Args			= TArgs;
	using Head			= THead;

	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using PayloadList	= typename Args::PayloadList;

	using Request		= RequestT<PayloadList>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;

	using Control		= ControlT<Args>;
	using ScopedOrigin	= typename Control::Origin;
	using ScopedRegion	= typename Control::Region;

	using FullControl	= FullControlT<Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;

	using Plan			= PlanT<Args>;

	using HeadState		= _S <HEAD_ID, Args, Head>;
	using SubStates		= _CS<HEAD_ID + 1, COMPO_INDEX + 1, ORTHO_INDEX, Args, 0, TSubStates...>;
	using AllForward	= _CF<Head, TSubStates...>;

	static constexpr ShortIndex REGION_SIZE	= AllForward::STATE_COUNT;

	ShortIndex& compoActive   (Control& control)	{ return control.stateRegistry().compoActive	[COMPO_INDEX];	}
	ShortIndex& compoResumable(Control& control)	{ return control.stateRegistry().resumable.compo[COMPO_INDEX];	}
	ShortIndex& compoRequested(Control& control)	{ return control.stateRegistry().requested.compo[COMPO_INDEX];	}

	HFSM_INLINE void   deepRegister			(StateRegistry& stateRegistry, const Parent parent);

	HFSM_INLINE bool   deepForwardEntryGuard(GuardControl& control,									  const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE bool   deepEntryGuard		(GuardControl& control,									  const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepEnterInitial		(Control& control);
	HFSM_INLINE void   deepEnter			(Control& control,										  const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE Status deepUpdate			(FullControl& control,									  const ShortIndex = INVALID_SHORT_INDEX);

	template <typename TEvent>
	HFSM_INLINE void   deepReact			(FullControl& control,		   const TEvent& event,		  const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE bool   deepForwardExitGuard	(GuardControl& control,									  const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE bool   deepExitGuard		(GuardControl& control,									  const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepExit				(Control& control,										  const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepForwardActive	(StateRegistry& stateRegistry, const RequestType request, const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE void   deepForwardRequest	(StateRegistry& stateRegistry, const RequestType request, const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE void   deepRequestRemain	(StateRegistry& stateRegistry);
	HFSM_INLINE void   deepRequestRestart	(StateRegistry& stateRegistry);
	HFSM_INLINE void   deepRequestResume	(StateRegistry& stateRegistry,							  const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepChangeToRequested(StateRegistry& stateRegistry, Control& control,		  const ShortIndex = INVALID_SHORT_INDEX);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using RegionType		= typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT = HeadState::NAME_COUNT + SubStates::NAME_COUNT;

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	HeadState _headState;
	SubStates _subStates;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRegister(StateRegistry& stateRegistry,
											const Parent parent)
{
	// TODO: add parent/forks type arrays to StateRegistry
	//HFSM_IF_DEBUG(CompoFork0& fork = compoFork(stateRegistry));
	//HFSM_IF_DEBUG(fork.TYPE = _headState.TYPE);

	stateRegistry.compoParents[COMPO_INDEX] = parent;

	_headState.deepRegister(stateRegistry, parent);
	_subStates.deepRegister(stateRegistry, Parent{COMPO_ID});
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
bool
_C<NS, NC, NO, TA, TH, TS...>::deepForwardEntryGuard(GuardControl& control,
													 const ShortIndex /*prong*/)
{
	const ShortIndex active	   = compoActive   (control);
	const ShortIndex requested = compoRequested(control);

	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested == INVALID_SHORT_INDEX)
		return _subStates.deepForwardEntryGuard(control, active);
	else
		return _subStates.deepEntryGuard	   (control, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
bool
_C<NS, NC, NO, TA, TH, TS...>::deepEntryGuard(GuardControl& control,
											  const ShortIndex /*prong*/)
{
	const ShortIndex requested = compoRequested(control);
	HFSM_ASSERT(requested != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepEntryGuard(control) ||
		   _subStates.deepEntryGuard(control, requested);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepEnterInitial(Control& control) {
	HFSM_ASSERT(compoResumable(control) == INVALID_SHORT_INDEX);
	HFSM_ASSERT(compoRequested(control) == INVALID_SHORT_INDEX);

	ShortIndex& active = compoActive(control);
	HFSM_ASSERT(active == INVALID_SHORT_INDEX);

	active = 0;

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter	   (control);
	_subStates.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepEnter(Control& control,
										 const ShortIndex /*prong*/)
{
	ShortIndex& active	  = compoActive   (control);
	ShortIndex& resumable = compoResumable(control);
	ShortIndex& requested = compoRequested(control);

	HFSM_ASSERT(active	  == INVALID_SHORT_INDEX &&
				requested != INVALID_SHORT_INDEX);

	active	  = requested;
	if (requested == resumable)
		resumable = INVALID_SHORT_INDEX;
	requested = INVALID_SHORT_INDEX;

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter(control);
	_subStates.deepEnter(control, active);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
Status
_C<NS, NC, NO, TA, TH, TS...>::deepUpdate(FullControl& control,
										  const ShortIndex /*prong*/)
{
	const ShortIndex active = compoActive(control);
	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion outer{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const Status headStatus = _headState.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates.deepUpdate(control, active);

		return headStatus;
	} else {
		const Status subStatus = _subStates.deepUpdate(control, active);

		if (subStatus.outerTransition)
			return subStatus;

		ScopedRegion inner{control, REGION_ID, HEAD_ID, REGION_SIZE};

		return subStatus && control.planData().planExists[REGION_ID] ?
			control.updatePlan(_headState, subStatus) : subStatus;
	}
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
template <typename TEvent>
void
_C<NS, NC, NO, TA, TH, TS...>::deepReact(FullControl& control,
										 const TEvent& event,
										 const ShortIndex /*prong*/)
{
	const ShortIndex active = compoActive(control);
	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepReact(control, event);
	_subStates.deepReact(control, event, active);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
bool
_C<NS, NC, NO, TA, TH, TS...>::deepForwardExitGuard(GuardControl& control,
													const ShortIndex /*prong*/)
{
	const ShortIndex active = compoActive(control);
	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (compoRequested(control) == INVALID_SHORT_INDEX)
		return _subStates.deepForwardExitGuard(control, active);
	else
		return _subStates.deepExitGuard		  (control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
bool
_C<NS, NC, NO, TA, TH, TS...>::deepExitGuard(GuardControl& control,
											 const ShortIndex /*prong*/)
{
	const ShortIndex active = compoActive(control);
	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepExitGuard(control) ||
		   _subStates.deepExitGuard(control, active);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepExit(Control& control,
										const ShortIndex /*prong*/)
{
	ShortIndex& active	  = compoActive   (control);
	ShortIndex& resumable = compoResumable(control);

	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	_subStates.deepExit(control, active);
	_headState.deepExit(control);

	resumable = active;
	active	  = INVALID_SHORT_INDEX;

	auto plan = control.plan(REGION_ID);
	plan.clear();
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepForwardActive(StateRegistry& stateRegistry,
												 const RequestType request,
												 const ShortIndex /*prong*/)
{
	HFSM_ASSERT(stateRegistry.isActive(HEAD_ID));

	const ShortIndex requested = stateRegistry.requested.compo[COMPO_INDEX];

	if (requested == INVALID_SHORT_INDEX) {
		const ShortIndex active = stateRegistry.compoActive[COMPO_INDEX];

		_subStates.deepForwardActive (stateRegistry, request, active);
	} else
		_subStates.deepForwardRequest(stateRegistry, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepForwardRequest(StateRegistry& stateRegistry,
												  const RequestType request,
												  const ShortIndex /*prong*/)
{
	const ShortIndex requested = stateRegistry.requested.compo[COMPO_INDEX];

	if (requested != INVALID_SHORT_INDEX)
		_subStates.deepForwardRequest(stateRegistry, request, requested);
	else
		switch (request) {
		case Request::REMAIN:
			deepRequestRemain (stateRegistry);
			break;

		case Request::RESTART:
			deepRequestRestart(stateRegistry);
			break;

		case Request::RESUME:
			deepRequestResume (stateRegistry, INVALID_SHORT_INDEX);
			break;

		default:
			HFSM_BREAK();
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestRemain(StateRegistry& stateRegistry) {
	const ShortIndex  active	= stateRegistry.compoActive	   [COMPO_INDEX];
		  ShortIndex& requested = stateRegistry.requested.compo[COMPO_INDEX];

	if (active == INVALID_SHORT_INDEX)
		requested = 0;

	_subStates.deepRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestRestart(StateRegistry& stateRegistry) {
	ShortIndex& requested = stateRegistry.requested.compo[COMPO_INDEX];

	requested = 0;

	_subStates.deepRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestResume(StateRegistry& stateRegistry,
												 const ShortIndex /*prong*/)
{
	const ShortIndex  resumable = stateRegistry.resumable.compo[COMPO_INDEX];
		  ShortIndex& requested = stateRegistry.requested.compo[COMPO_INDEX];

	requested = (resumable != INVALID_SHORT_INDEX) ?
		resumable : 0;

	_subStates.deepRequestResume(stateRegistry, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepChangeToRequested(StateRegistry& stateRegistry,
													 Control& control,
													 const ShortIndex /*prong*/)
{
	ShortIndex& active	  = stateRegistry.compoActive	 [COMPO_INDEX];
	ShortIndex& resumable = stateRegistry.resumable.compo[COMPO_INDEX];
	ShortIndex& requested = stateRegistry.requested.compo[COMPO_INDEX];

	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	if (requested == INVALID_SHORT_INDEX)
		_subStates.deepChangeToRequested(stateRegistry, control, active);
	else {
		_subStates.deepExit(control, active);

		if (requested != active) {
			resumable = active;
			active	  = requested;
		}
		requested = INVALID_SHORT_INDEX;

		_subStates.deepEnter(control, active);
	}
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepGetNames(const LongIndex parent,
											const RegionType region,
											const ShortIndex depth,
											StructureStateInfos& _stateInfos) const
{
	_headState.deepGetNames(parent,					 region,						depth,	   _stateInfos);
	_subStates.deepGetNames(_stateInfos.count() - 1, StructureStateInfo::COMPOSITE, depth + 1, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID, ShortIndex, ShortIndex, typename, ShortIndex, typename...>
struct _OS;

//------------------------------------------------------------------------------

template <StateID NInitialID,
		  ShortIndex NCompoIndex,
		  ShortIndex NOrthoIndex,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct _OS<NInitialID, NCompoIndex, NOrthoIndex, TArgs, NIndex, TInitial, TRemaining...> {
	static constexpr StateID	INITIAL_ID	= NInitialID;
	static constexpr ShortIndex COMPO_INDEX	= NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	= NOrthoIndex;
	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			 = TArgs;

	using PayloadList	 = typename Args::PayloadList;

	using Request		 = RequestT<PayloadList>;
	using RequestType	 = typename Request::Type;

	using StateRegistry	 = StateRegistryT<Args>;
	using StateParents	 = typename StateRegistry::StateParents;

	using Control		 = ControlT		 <Args>;
	using FullControl	 = FullControlT  <Args>;
	using GuardControl	 = GuardControlT <Args>;

	using Initial		 = Material<INITIAL_ID, COMPO_INDEX, ORTHO_INDEX, Args, TInitial>;
	using InitialForward = Wrap<TInitial>;

	using Remaining		 = _OS<INITIAL_ID  + InitialForward::STATE_COUNT,
						 	   COMPO_INDEX + InitialForward::COMPO_COUNT,
						 	   ORTHO_INDEX + InitialForward::ORTHO_COUNT,
						 	   Args,
							   PRONG_INDEX + 1,
							   TRemaining...>;

	using AllForward	 = _OSF<TInitial, TRemaining...>;

	HFSM_INLINE void   wideRegister			(StateRegistry& stateRegistry, const ForkID forkId);

	HFSM_INLINE bool   wideForwardEntryGuard(GuardControl& control,									 const OrthoFork& prongs);
	HFSM_INLINE bool   wideForwardEntryGuard(GuardControl& control);
	HFSM_INLINE bool   wideEntryGuard		(GuardControl& control);

	HFSM_INLINE void   wideEnterInitial		(Control& control);
	HFSM_INLINE void   wideEnter			(Control& control);

	HFSM_INLINE Status wideUpdate			(FullControl& control);

	template <typename TEvent>
	HFSM_INLINE void   wideReact			(const TEvent& event, FullControl& control);

	HFSM_INLINE bool   wideForwardExitGuard	(GuardControl& control,									  const OrthoFork& prongs);
	HFSM_INLINE bool   wideForwardExitGuard	(GuardControl& control);
	HFSM_INLINE bool   wideExitGuard		(GuardControl& control);

	HFSM_INLINE void   wideExit				(Control& control);

	HFSM_INLINE void   wideForwardActive	(StateRegistry& stateRegistry, const RequestType request, const OrthoFork& prongs);
	HFSM_INLINE void   wideForwardRequest	(StateRegistry& stateRegistry, const RequestType request, const OrthoFork& prongs);
	HFSM_INLINE void   wideRequestRemain	(StateRegistry& stateRegistry);
	HFSM_INLINE void   wideRequestRestart	(StateRegistry& stateRegistry);
	HFSM_INLINE void   wideRequestResume	(StateRegistry& stateRegistry);

	HFSM_INLINE void   wideChangeToRequested(StateRegistry& stateRegistry, Control& control);

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
		  ShortIndex NCompoIndex,
		  ShortIndex NOrthoIndex,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename TInitial>
struct _OS<NInitialID, NCompoIndex, NOrthoIndex, TArgs, NIndex, TInitial> {
	static constexpr StateID	INITIAL_ID	= NInitialID;
	static constexpr ShortIndex COMPO_INDEX	= NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	= NOrthoIndex;
	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			 = TArgs;

	using PayloadList	 = typename Args::PayloadList;

	using Request		 = RequestT<PayloadList>;
	using RequestType	 = typename Request::Type;

	using StateRegistry	 = StateRegistryT<Args>;
	using StateParents	 = typename StateRegistry::StateParents;

	using Control		 = ControlT		 <Args>;
	using FullControl	 = FullControlT  <Args>;
	using GuardControl	 = GuardControlT <Args>;

	using Initial		 = Material<INITIAL_ID, COMPO_INDEX, ORTHO_INDEX, Args, TInitial>;
	using AllForward	 = _OSF<TInitial>;

	HFSM_INLINE void   wideRegister			(StateRegistry& stateRegistry, const ForkID forkId);

	HFSM_INLINE bool   wideForwardEntryGuard(GuardControl& control,										 const OrthoFork& prongs);
	HFSM_INLINE bool   wideForwardEntryGuard(GuardControl& control);
	HFSM_INLINE bool   wideEntryGuard		(GuardControl& control);

	HFSM_INLINE void   wideEnterInitial		(Control& control);
	HFSM_INLINE void   wideEnter			(Control& control);

	HFSM_INLINE Status wideUpdate			(FullControl& control);

	template <typename TEvent>
	HFSM_INLINE void   wideReact			(const TEvent& event, FullControl& control);

	HFSM_INLINE bool   wideForwardExitGuard	(GuardControl& control,										 const OrthoFork& prongs);
	HFSM_INLINE bool   wideForwardExitGuard	(GuardControl& control);
	HFSM_INLINE bool   wideExitGuard		(GuardControl& control);

	HFSM_INLINE void   wideExit				(Control& control);

	HFSM_INLINE void   wideForwardActive	(StateRegistry& stateRegistry, const RequestType transition, const OrthoFork& prongs);
	HFSM_INLINE void   wideForwardRequest	(StateRegistry& stateRegistry, const RequestType transition, const OrthoFork& prongs);
	HFSM_INLINE void   wideRequestRemain	(StateRegistry& stateRegistry);
	HFSM_INLINE void   wideRequestRestart	(StateRegistry& stateRegistry);
	HFSM_INLINE void   wideRequestResume	(StateRegistry& stateRegistry);

	HFSM_INLINE void   wideChangeToRequested(StateRegistry& stateRegistry, Control& control);

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

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideRegister(StateRegistry& stateRegistry,
												 const ForkID forkId)
{
	initial  .deepRegister(stateRegistry, Parent{forkId, PRONG_INDEX});
	remaining.wideRegister(stateRegistry, forkId);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardEntryGuard(GuardControl& control,
														  const OrthoFork& prongs)
{
	const bool resultI = prongs[PRONG_INDEX] ?
						 initial  .deepForwardEntryGuard(control) : false;

	const bool resultR = remaining.wideForwardEntryGuard(control, prongs);

	return resultI || resultR;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardEntryGuard(GuardControl& control) {
	const bool resultI = initial  .deepForwardEntryGuard(control);
	const bool resultR = remaining.wideForwardEntryGuard(control);

	return resultI || resultR;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideEntryGuard(GuardControl& control) {
	const bool resultI = initial  .deepEntryGuard(control);
	const bool resultR = remaining.wideEntryGuard(control);

	return resultI || resultR;
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideEnterInitial(Control& control) {
	initial  .deepEnterInitial(control);
	remaining.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideEnter(Control& control) {
	initial  .deepEnter(control);
	remaining.wideEnter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
Status
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideUpdate(FullControl& control) {
	const auto status =	   initial	.deepUpdate(control);
	return combine(status, remaining.wideUpdate(control));
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
template <typename TEvent>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideReact(const TEvent& event,
											  FullControl& control)
{
	initial  .deepReact(control, event);
	remaining.wideReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardExitGuard(GuardControl& control,
														 const OrthoFork& prongs)
{
	const bool resultI = prongs[PRONG_INDEX] ?
						 initial  .deepForwardExitGuard(control) : false;

	const bool resultR = remaining.wideForwardExitGuard(control, prongs);

	return resultI || resultR;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardExitGuard(GuardControl& control) {
	const bool resultI = initial  .deepForwardExitGuard(control);
	const bool resultR = remaining.wideForwardExitGuard(control);

	return resultI || resultR;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideExitGuard(GuardControl& control) {
	const bool resultI = initial  .deepExitGuard(control);
	const bool resultR = remaining.wideExitGuard(control);

	return resultI || resultR;
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideExit(Control& control) {
	initial	 .deepExit(control);
	remaining.wideExit(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardActive(StateRegistry& stateRegistry,
													  const RequestType request,
													  const OrthoFork& prongs)
{
	const RequestType local = prongs[PRONG_INDEX] ? request : Request::REMAIN;

	initial	 .deepForwardActive(stateRegistry, local);
	remaining.wideForwardActive(stateRegistry, request, prongs);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardRequest(StateRegistry& stateRegistry,
													   const RequestType request,
													   const OrthoFork& prongs)
{
	const RequestType local = prongs[PRONG_INDEX] ? request : Request::REMAIN;

	initial	 .deepForwardRequest(stateRegistry, local);
	remaining.wideForwardRequest(stateRegistry, request, prongs);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestRemain(StateRegistry& stateRegistry) {
	initial	 .deepRequestRemain(stateRegistry);
	remaining.wideRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestRestart(StateRegistry& stateRegistry) {
	initial	 .deepRequestRestart(stateRegistry);
	remaining.wideRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestResume(StateRegistry& stateRegistry) {
	initial	 .deepRequestResume(stateRegistry, INVALID_SHORT_INDEX);
	remaining.wideRequestResume(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideChangeToRequested(StateRegistry& stateRegistry,
														  Control& control)
{
	initial	 .deepChangeToRequested(stateRegistry, control);
	remaining.wideChangeToRequested(stateRegistry, control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
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
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideRegister(StateRegistry& stateRegistry,
										  const ForkID forkId)
{
	initial.deepRegister(stateRegistry, Parent{forkId, PRONG_INDEX});
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
bool
_OS<NS, NC, NO, TA, NI, TI>::wideForwardEntryGuard(GuardControl& control,
												   const OrthoFork& prongs)
{
	return prongs[PRONG_INDEX] ?
		initial.deepForwardEntryGuard(control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
bool
_OS<NS, NC, NO, TA, NI, TI>::wideForwardEntryGuard(GuardControl& control) {
	return initial.deepForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
bool
_OS<NS, NC, NO, TA, NI, TI>::wideEntryGuard(GuardControl& control) {
	return initial.deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideEnterInitial(Control& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideEnter(Control& control) {
	initial.deepEnter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
Status
_OS<NS, NC, NO, TA, NI, TI>::wideUpdate(FullControl& control) {
	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
template <typename TEvent>
void
_OS<NS, NC, NO, TA, NI, TI>::wideReact(const TEvent& event,
									   FullControl& control)
{
	initial.deepReact(control, event);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
bool
_OS<NS, NC, NO, TA, NI, TI>::wideForwardExitGuard(GuardControl& control,
												  const OrthoFork& prongs)
{
	return prongs[PRONG_INDEX] ?
		initial.deepForwardExitGuard(control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
bool
_OS<NS, NC, NO, TA, NI, TI>::wideForwardExitGuard(GuardControl& control) {
	return initial.deepForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
bool
_OS<NS, NC, NO, TA, NI, TI>::wideExitGuard(GuardControl& control) {
	return initial.deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideExit(Control& control) {
	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideForwardActive(StateRegistry& stateRegistry,
											   const RequestType request,
											   const OrthoFork& prongs)
{
	const RequestType local = prongs[PRONG_INDEX] ? request : Request::REMAIN;

	initial.deepForwardActive(stateRegistry, local);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideForwardRequest(StateRegistry& stateRegistry,
												const RequestType request,
												const OrthoFork& prongs)
{
	const RequestType local = prongs[PRONG_INDEX] ? request : Request::REMAIN;

	initial.deepForwardRequest(stateRegistry, local);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideRequestRemain(StateRegistry& stateRegistry) {
	initial.deepRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideRequestRestart(StateRegistry& stateRegistry) {
	initial.deepRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideRequestResume(StateRegistry& stateRegistry) {
	initial.deepRequestResume(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideChangeToRequested(StateRegistry& stateRegistry,
												   Control& control)
{
	initial.deepChangeToRequested(stateRegistry, control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
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
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NHeadIndex,
		  ShortIndex NCompoIndex,
		  ShortIndex NOrthoIndex,
		  typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct _O final {
	static constexpr StateID	HEAD_ID		= NHeadIndex;
	static constexpr ShortIndex COMPO_INDEX	= NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	= NOrthoIndex;
	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID		ORTHO_ID	= (ForkID) -ORTHO_INDEX - 1;

	using Args			= TArgs;
	using Head			= THead;

	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using PayloadList	= typename Args::PayloadList;

	using Request		= RequestT<PayloadList>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;

	using Control		= ControlT<Args>;
	using ScopedOrigin	= typename Control::Origin;
	using ScopedRegion	= typename Control::Region;

	using FullControl	= FullControlT<Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;

	using HeadState		= _S <HEAD_ID, Args, Head>;
	using SubStates		= _OS<HEAD_ID + 1, COMPO_INDEX, ORTHO_INDEX + 1, Args, 0, TSubStates...>;
	using AllForward	= _OF<Head, TSubStates...>;

	static constexpr ShortIndex REGION_SIZE	= AllForward::STATE_COUNT;

	HFSM_INLINE		  OrthoFork& orthoRequested(	  StateRegistry& stateRegistry)			{ return stateRegistry.requested.ortho[ORTHO_INDEX];	}
	HFSM_INLINE const OrthoFork& orthoRequested(const StateRegistry& stateRegistry) const	{ return stateRegistry.requested.ortho[ORTHO_INDEX];	}

	HFSM_INLINE		  OrthoFork& orthoRequested(	  Control&   control)					{ return orthoRequested(control.stateRegistry());		}
	HFSM_INLINE const OrthoFork& orthoRequested(const Control&   control) const				{ return orthoRequested(control.stateRegistry());		}

	HFSM_INLINE void   deepRegister			(StateRegistry& stateRegistry, const Parent parent);

	HFSM_INLINE bool   deepForwardEntryGuard(GuardControl& control,									  const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE bool   deepEntryGuard		(GuardControl& control,									  const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepEnterInitial		(Control& control);
	HFSM_INLINE void   deepEnter			(Control& control,										  const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE Status deepUpdate			(FullControl& control,									  const ShortIndex = INVALID_SHORT_INDEX);

	template <typename TEvent>
	HFSM_INLINE void   deepReact			(FullControl& control,		   const TEvent& event,		  const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE bool   deepForwardExitGuard	(GuardControl& control,									  const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE bool   deepExitGuard		(GuardControl& control,									  const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepExit				(Control& control,										  const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepForwardActive	(StateRegistry& stateRegistry, const RequestType request, const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE void   deepForwardRequest	(StateRegistry& stateRegistry, const RequestType request, const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE void   deepRequestRemain	(StateRegistry& stateRegistry);
	HFSM_INLINE void   deepRequestRestart	(StateRegistry& stateRegistry);
	HFSM_INLINE void   deepRequestResume	(StateRegistry& stateRegistry,							  const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepChangeToRequested(StateRegistry& stateRegistry, Control& control,		  const ShortIndex = INVALID_SHORT_INDEX);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using RegionType		= typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT	 = HeadState::NAME_COUNT  + SubStates::NAME_COUNT;

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	HeadState _headState;
	SubStates _subStates;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRegister(StateRegistry& stateRegistry,
											const Parent parent)
{
	stateRegistry.orthoParents[ORTHO_INDEX] = parent;

	HFSM_IF_ASSERT(const ShortIndex requestedIndex =)
	stateRegistry.requested.ortho.template emplace<OrthoForkT<AllForward::WIDTH>>();
	HFSM_ASSERT(requestedIndex == ORTHO_INDEX);

	HFSM_IF_ASSERT(const ShortIndex resumableIndex =)
	stateRegistry.resumable.ortho.template emplace<OrthoForkT<AllForward::WIDTH>>();
	HFSM_ASSERT(resumableIndex == ORTHO_INDEX);

	_headState.deepRegister(stateRegistry, parent);
	_subStates.wideRegister(stateRegistry, ORTHO_ID);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
bool
_O<NS, NC, NO, TA, TH, TS...>::deepForwardEntryGuard(GuardControl& control,
													 const ShortIndex /*prong*/)
{
	const OrthoFork& requested = orthoRequested(control);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested)
		return _subStates.wideForwardEntryGuard(control, requested);
	else
		return _subStates.wideForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
bool
_O<NS, NC, NO, TA, TH, TS...>::deepEntryGuard(GuardControl& control,
											  const ShortIndex /*prong*/)
{
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepEntryGuard(control) ||
		   _subStates.wideEntryGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepEnterInitial(Control& control) {
	HFSM_IF_ASSERT(const OrthoFork& requested = orthoRequested(control));

	HFSM_ASSERT(!requested);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter	   (control);
	_subStates.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepEnter(Control& control,
										 const ShortIndex /*prong*/)
{
	OrthoFork& requested = orthoRequested(control);
	requested.clear();

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter(control);
	_subStates.wideEnter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
Status
_O<NS, NC, NO, TA, TH, TS...>::deepUpdate(FullControl& control,
										  const ShortIndex /*prong*/)
{
	ScopedRegion outer{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const auto headStatus = _headState.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates.wideUpdate(control);

		return headStatus;
	} else {
		const Status subStatus = _subStates.wideUpdate(control);

		if (subStatus.outerTransition)
			return subStatus;

		ScopedRegion inner{control, REGION_ID, HEAD_ID, REGION_SIZE};

		return subStatus && control.planData().planExists[REGION_ID] ?
			control.updatePlan(_headState, subStatus) : subStatus;
	}
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
template <typename TEvent>
void
_O<NS, NC, NO, TA, TH, TS...>::deepReact(FullControl& control,
										 const TEvent& event,
										 const ShortIndex /*prong*/)
{
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepReact(control, event);
	_subStates.wideReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
bool
_O<NS, NC, NO, TA, TH, TS...>::deepForwardExitGuard(GuardControl& control,
													const ShortIndex /*prong*/)
{
	const OrthoFork& requested = orthoRequested(control);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested)
		return _subStates.wideForwardExitGuard(control, requested);
	else
		return _subStates.wideForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
bool
_O<NS, NC, NO, TA, TH, TS...>::deepExitGuard(GuardControl& control,
											 const ShortIndex /*prong*/)
{
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepExitGuard(control) ||
		   _subStates.wideExitGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepExit(Control& control,
										const ShortIndex /*prong*/)
{
	_subStates.wideExit(control);
	_headState.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepForwardActive(StateRegistry& stateRegistry,
												 const RequestType request,
												 const ShortIndex /*prong*/)
{
	HFSM_ASSERT(stateRegistry.isActive(HEAD_ID));

	const OrthoFork& requested = orthoRequested(stateRegistry);
	HFSM_ASSERT(requested);

	_subStates.wideForwardActive(stateRegistry, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepForwardRequest(StateRegistry& stateRegistry,
												  const RequestType request,
												  const ShortIndex /*prong*/)
{
	const OrthoFork& requested = orthoRequested(stateRegistry);

	if (requested)
		_subStates.wideForwardRequest(stateRegistry, request, requested);
	else
		switch (request) {
		case Request::REMAIN:
			deepRequestRemain(stateRegistry);
			break;

		case Request::RESTART:
			deepRequestRestart(stateRegistry);
			break;

		case Request::RESUME:
			deepRequestResume(stateRegistry, INVALID_SHORT_INDEX);
			break;

		default:
			HFSM_BREAK();
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestRemain(StateRegistry& stateRegistry) {
	_subStates.wideRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestRestart(StateRegistry& stateRegistry) {
	_subStates.wideRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestResume(StateRegistry& stateRegistry,
												 const ShortIndex /*prong*/)
{
	_subStates.wideRequestResume(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepChangeToRequested(StateRegistry& stateRegistry,
													 Control& control,
													 const ShortIndex /*prong*/)
{
	// TODO: pass requested orthoFork
	_subStates.wideChangeToRequested(stateRegistry, control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepGetNames(const LongIndex parent,
											const RegionType region,
											const ShortIndex depth,
											StructureStateInfos& _stateInfos) const
{
	_headState.deepGetNames(parent, region,			 depth,		_stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, depth + 1, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TConfig,
		  typename TPayloadList,
		  typename TApex>
class _R final {
	using Context				 = TContext;
	using Config				 = TConfig;
	using PayloadList			 = TPayloadList;
	using Apex					 = TApex;

	using ForwardApex			 = Wrap<Apex>;
	using AllForward			 = _RF<Context, Config, PayloadList, Apex>;
	using StateList				 = typename AllForward::StateList;
	using RegionList			 = typename AllForward::RegionList;

	static constexpr LongIndex MAX_PLAN_TASKS	 = Config::MAX_PLAN_TASKS;
	static constexpr LongIndex MAX_SUBSTITUTIONS = Config::MAX_SUBSTITUTIONS;

	static constexpr LongIndex TASK_CAPACITY	 = AllForward::TASK_CAPACITY;

public:
	static constexpr LongIndex  REVERSE_DEPTH	 = ForwardApex::REVERSE_DEPTH;
	static constexpr ShortIndex COMPO_COUNT		 = ForwardApex::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT		 = ForwardApex::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS		 = ForwardApex::ORTHO_UNITS;
	static constexpr LongIndex  PRONG_COUNT		 = ForwardApex::PRONG_COUNT;

	static constexpr LongIndex  STATE_COUNT		 = ForwardApex::STATE_COUNT;
	static constexpr LongIndex  REGION_COUNT	 = ForwardApex::REGION_COUNT;

	static_assert(STATE_COUNT <  (ShortIndex) -1, "Too many states in the hierarchy. Change 'ShortIndex' type.");
	static_assert(STATE_COUNT == (ShortIndex) StateList::SIZE, "STATE_COUNT != StateList::SIZE");

private:
	using Args					 = typename AllForward::Args;

	using StateRegistry			 = StateRegistryT<Args>;
	using AllForks				 = typename StateRegistry::AllForks;
	using PlanData				 = PlanDataT	 <Args>;

	using Control				 = ControlT		 <Args>;
	using FullControl			 = FullControlT	 <Args>;
	using GuardControl			 = GuardControlT <Args>;

	using PayloadBox			 = typename PayloadList::Variant;
	using Payloads				 = Array<PayloadBox, STATE_COUNT>;
	using Request				 = typename FullControl::Request;
	using Requests				 = typename FullControl::Requests;

	using MaterialApex			 = Material<0, 0, 0, Args, Apex>;

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static constexpr LongIndex NAME_COUNT	  = MaterialApex::NAME_COUNT;

	using Prefix				 = StaticArray<wchar_t,			REVERSE_DEPTH * 2 + 2>;
	using Prefixes				 = StaticArray<Prefix,			STATE_COUNT>;

	using StateInfoStorage		 = Array<StructureStateInfo,	STATE_COUNT>;

	using StructureStorage		 = Array<StructureEntry,		NAME_COUNT>;
	using ActivityHistoryStorage = Array<char,					NAME_COUNT>;

	using TransitionInfo		 = TransitionInfoT<PayloadList>;
	using TransitionInfoStorage	 = Array<TransitionInfo,		COMPO_COUNT * 4>;
#endif

public:
	_R(Context& context
	   HFSM_IF_LOGGER(, LoggerInterface* const logger = nullptr));

	~_R();

	template <typename T>
	static constexpr LongIndex
	stateId()													{ return StateList::template index<T>();			}

	void update();

	template <typename TEvent>
	HFSM_INLINE void react(const TEvent& event);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool isActive   (const StateID stateId) const	{ return _stateRegistry.isActive   (stateId);			}
	HFSM_INLINE bool isResumable(const StateID stateId) const	{ return _stateRegistry.isResumable(stateId);			}

	HFSM_INLINE bool isScheduled(const StateID stateId) const	{ return isResumable(stateId);						}

	template <typename TState>
	HFSM_INLINE bool isActive   () const						{ return isActive	(stateId<TState>());			}

	template <typename TState>
	HFSM_INLINE bool isResumable() const						{ return isResumable(stateId<TState>());			}

	template <typename TState>
	HFSM_INLINE bool isScheduled() const						{ return isResumable<TState>();						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void changeTo(const StateID stateId);
	HFSM_INLINE void resume	 (const StateID stateId);
	HFSM_INLINE void schedule(const StateID stateId);

	template <typename TState>
	HFSM_INLINE void changeTo()									{ changeTo(stateId<TState>());						}

	template <typename TState>
	HFSM_INLINE void resume	()									{ resume  (stateId<TState>());						}

	template <typename TState>
	HFSM_INLINE void schedule()									{ schedule(stateId<TState>());						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TPayload>
	HFSM_INLINE void changeTo(const StateID stateId, TPayload* const payload);

	template <typename TPayload>
	HFSM_INLINE void resume  (const StateID stateId, TPayload* const payload);

	template <typename TPayload>
	HFSM_INLINE void schedule(const StateID stateId, TPayload* const payload);

	template <typename TState, typename TPayload>
	HFSM_INLINE void changeTo(TPayload* const payload)			{ changeTo(stateId<TState>(), payload);				}

	template <typename TState, typename TPayload>
	HFSM_INLINE void resume	(TPayload* const payload)			{ resume  (stateId<TState>(), payload);				}

	template <typename TState, typename TPayload>
	HFSM_INLINE void schedule(TPayload* const payload)			{ schedule(stateId<TState>(), payload);				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void resetStateData(const StateID stateId);

	template <typename TPayload>
	HFSM_INLINE void setStateData  (const StateID stateId, TPayload* const payload);

	HFSM_INLINE bool isStateDataSet(const StateID stateId) const;

	template <typename TPayload>
	HFSM_INLINE TPayload* getStateData(const StateID stateId) const;

	template <typename TState>
	HFSM_INLINE void resetStateData()							{ resetStateData(stateId<TState>());				}

	template <typename TState, typename TPayload>
	HFSM_INLINE void setStateData  (TPayload* const payload)	{ setStateData  (stateId<TState>(), payload);		}

	template <typename TState>
	HFSM_INLINE bool isStateDataSet() const						{ return isStateDataSet(stateId<TState>());			}

	template <typename TState, typename TPayload>
	HFSM_INLINE TPayload* getStateData() const					{ return getStateData<TPayload>(stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	const MachineStructure& structure() const					{ return _structure;								}
	const MachineActivity&  activity()  const					{ return _activityHistory;							}
#endif

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	void attachLogger(LoggerInterface* const logger)			{ _logger = logger;									}
#endif

protected:
	void processTransitions();
	bool cancelledByGuards(const Requests& pendingChanges);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	void getStateNames();
	void udpateActivity();
	void recordRequestsAs(const Method method);
#endif

private:
	Context& _context;

	StateRegistry _stateRegistry;
	PlanData _planData;

	Payloads _requestPayloads;
	Requests _requests;

	MaterialApex _apex;

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

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TPL, typename TA>
_R<TC, TG, TPL, TA>::_R(Context& context
						HFSM_IF_LOGGER(, LoggerInterface* const logger))
	: _context{context}
	HFSM_IF_LOGGER(, _logger{logger})
{
	_apex.deepRegister(_stateRegistry, Parent{});

	HFSM_IF_STRUCTURE(getStateNames());

	for (auto& payload : _requestPayloads)
		payload.reset();

	{
		Control control{_context,
						_stateRegistry,
						_planData,
						HFSM_LOGGER_OR(_logger, nullptr)};
		_apex.deepEnterInitial(control);

		HFSM_IF_ASSERT(_planData.verifyPlans());
	}

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
_R<TC, TG, TPL, TA>::~_R() {
	Control control{_context,
					_stateRegistry,
					_planData,
					HFSM_LOGGER_OR(_logger, nullptr)};
	_apex.deepExit(control);

	HFSM_IF_ASSERT(_planData.verifyPlans());
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::update() {
	FullControl control(_context,
						_stateRegistry,
						_planData,
						_requests,
						HFSM_LOGGER_OR(_logger, nullptr));
	_apex.deepUpdate(control);

	HFSM_IF_ASSERT(_planData.verifyPlans());

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
						_stateRegistry,
						_planData,
						_requests,
						HFSM_LOGGER_OR(_logger, nullptr));
	_apex.deepReact(control, event);

	HFSM_IF_ASSERT(_planData.verifyPlans());

	if (_requests.count())
		processTransitions();

	_requests.clear();
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::changeTo(const StateID stateId) {
	const Request request(Request::Type::RESTART, stateId);
	_requests << request;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, Transition::RESTART, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::resume(const StateID stateId) {
	const Request request(Request::Type::RESUME, stateId);
	_requests << request;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, Transition::RESUME, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::schedule(const StateID stateId) {
	const Request request(Request::Type::SCHEDULE, stateId);
	_requests << request;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, Transition::SCHEDULE, stateId);
#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
void
_R<TC, TG, TPL, TA>::changeTo(const StateID stateId,
							  TPayload* const payload)
{
	const Request request(Request::Type::RESTART, stateId, payload);
	_requests << request;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, Transition::RESTART, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
void
_R<TC, TG, TPL, TA>::resume(const StateID stateId,
							TPayload* const payload)
{
	const Request request(Request::Type::RESUME, stateId, payload);
	_requests << request;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, Transition::RESUME, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
void
_R<TC, TG, TPL, TA>::schedule(const StateID stateId,
							  TPayload* const payload)
{
	const Request request(Request::Type::SCHEDULE, stateId, payload);
	_requests << request;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, Transition::SCHEDULE, stateId);
#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::resetStateData(const StateID stateId) {
	HFSM_ASSERT(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY)
		_requestPayloads[stateId].reset();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
void
_R<TC, TG, TPL, TA>::setStateData(const StateID stateId,
								  TPayload* const payload)
{
	HFSM_ASSERT(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY)
		_requestPayloads[stateId] = payload;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
bool
_R<TC, TG, TPL, TA>::isStateDataSet(const StateID stateId) const {
	HFSM_ASSERT(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY)
		return !!_requestPayloads[stateId];
	else
		return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
TPayload*
_R<TC, TG, TPL, TA>::getStateData(const StateID stateId) const {
	using Payload = TPayload;

	HFSM_ASSERT(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY) {
		const auto& payload = _requestPayloads[stateId];

		return payload.template get<Payload>();
	} else
		return nullptr;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::processTransitions() {
	HFSM_ASSERT(_requests.count());

	HFSM_IF_STRUCTURE(_lastTransitions.clear());

	AllForks undoRequested;
	_stateRegistry.requested.clear();

	Requests lastRequests;

	for (LongIndex i = 0;
		i < MAX_SUBSTITUTIONS && _requests.count();
		++i)
	{
		unsigned changeCount = 0;
		undoRequested = _stateRegistry.requested;

		for (const Request& request : _requests) {
			HFSM_IF_STRUCTURE(_lastTransitions << TransitionInfo(request, Method::UPDATE));

			switch (request.type) {
			case Request::RESTART:
			case Request::RESUME:
				_stateRegistry.requestImmediate(request);
				_apex.deepForwardActive(_stateRegistry, request.type);

				++changeCount;
				break;

			case Request::SCHEDULE:
				_stateRegistry.requestScheduled(request.stateId);
				break;

			default:
				HFSM_BREAK();
			}
		}

		if (changeCount > 0) {
			lastRequests = _requests;
			_requests.clear();

			if (cancelledByGuards(lastRequests))
				_stateRegistry.requested = undoRequested;
		} else
			_requests.clear();
	}

	{
		Control control{_context,
						_stateRegistry,
						_planData,
						HFSM_LOGGER_OR(_logger, nullptr)};

		_apex.deepChangeToRequested(_stateRegistry, control);
		_stateRegistry.clearOrthoRequested();

		HFSM_IF_ASSERT(_planData.verifyPlans());
	}

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
bool
_R<TC, TG, TPL, TA>::cancelledByGuards(const Requests& pendingRequests) {
	GuardControl guardControl(_context,
							  _stateRegistry,
							  _planData,
							  _requests,
							  pendingRequests,
							  HFSM_LOGGER_OR(_logger, nullptr));

	if (_apex.deepForwardExitGuard(guardControl)) {
		HFSM_IF_STRUCTURE(recordRequestsAs(Method::EXIT_GUARD));

		return true;
	} else if (_apex.deepForwardEntryGuard(guardControl)) {
		HFSM_IF_STRUCTURE(recordRequestsAs(Method::ENTRY_GUARD));

		return true;
	} else
		return false;
}

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
	for (LongIndex s = 0, i = 0; s < _stateInfos.count(); ++s) {
		const auto& state = _stateInfos[s];

		if (state.name[0] != L'\0') {
			_structure[i].isActive = isActive(s);

			auto& activity = _activityHistory[i];

			if (_structure[i].isActive) {
				if (activity > 0)
					activity = activity < INT8_MAX ? activity + 1 : activity;
				else
					activity = +1;
			} else {
				if (activity > 0)
					activity = -1;
				else
					activity = activity > INT8_MIN ? activity - 1 : activity;
			}

			++i;
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::recordRequestsAs(const Method method) {
	for (const auto& request : _requests)
		_lastTransitions << TransitionInfo(request, method);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#undef HFSM_INLINE
#undef HFSM_IF_LOGGER
#undef HFSM_LOGGER_OR
#undef HFSM_LOG_STATE_METHOD
#undef HFSM_IF_STRUCTURE

#ifdef _MSC_VER
#pragma warning(pop)
#endif
