// HFSM2 (hierarchical state machine for games and interactive applications)
// 1.8.3 (2021-02-06)
//
// Created by Andrew Gresyk
//
// Licensed under the MIT License;
// you may not use this file except in compliance with the License.
//
//
// MIT License
//
// Copyright (c) 2021
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

#define HFSM2_VERSION_MAJOR 1
#define HFSM2_VERSION_MINOR 8
#define HFSM2_VERSION_PATCH 3

#define HFSM2_VERSION (10000 * HFSM2_VERSION_MAJOR + 100 * HFSM2_VERSION_MINOR + HFSM2_VERSION_PATCH)

#include <stdint.h>			// uint32_t, uint64_t
#include <string.h>			// memcpy_s()

#include <new>
#include <typeindex>
#include <utility>			// move(), forward()

#if defined _DEBUG && _MSC_VER
	#include <intrin.h>		// __debugbreak()
#endif

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#define HFSM2_INLINE													 //inline

////////////////////////////////////////////////////////////////////////////////

#if INTPTR_MAX == INT64_MAX
	#define HFSM2_64BIT_OR_32BIT(p64, p32)									  p64
#else
	#define HFSM2_64BIT_OR_32BIT(p64, p32)									  p32
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _DEBUG && defined _MSC_VER
	#define HFSM2_BREAK()										   __debugbreak()
#else
	#define HFSM2_BREAK()											   ((void) 0)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef _DEBUG
	#define HFSM2_IF_DEBUG(...)										  __VA_ARGS__
	#define HFSM2_UNLESS_DEBUG(...)
	#define HFSM2_DEBUG_OR(y, n)												y
#else
	#define HFSM2_IF_DEBUG(...)
	#define HFSM2_UNLESS_DEBUG(...)									  __VA_ARGS__
	#define HFSM2_DEBUG_OR(y, n)												n
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_ASSERT
	#define HFSM2_IF_ASSERT(...)									  __VA_ARGS__
	#define HFSM2_CHECKED(x)						(!!(x) || (HFSM2_BREAK(), 0))
	#define HFSM2_ASSERT(x)							(!!(x) || (HFSM2_BREAK(), 0))
	#define HFSM2_ASSERT_OR(y, n)												y
#else
	#define HFSM2_IF_ASSERT(...)
	#define HFSM2_CHECKED(x)													x
	#define HFSM2_ASSERT(x)											   ((void) 0)
	#define HFSM2_ASSERT_OR(y, n)												n
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _MSC_VER || defined __clang_major__ && __clang_major__ >= 7
	#define HFSM2_EXPLICIT_MEMBER_SPECIALIZATION
#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_ALL
	#define HFSM2_ENABLE_UTILITY_THEORY
	#define HFSM2_ENABLE_PLANS
	#define HFSM2_ENABLE_SERIALIZATION
	#define HFSM2_ENABLE_TRANSITION_HISTORY
	#define HFSM2_ENABLE_STRUCTURE_REPORT
#endif

//------------------------------------------------------------------------------

#ifndef HFSM2_DISABLE_TYPEINDEX
	#define HFSM2_IF_TYPEINDEX(...)									  __VA_ARGS__
	#define HFSM2_TYPEINDEX_MASK										 (1 << 0)
#else
	#define HFSM2_IF_TYPEINDEX(...)
	#define HFSM2_TYPEINDEX_MASK										 (0 << 0)
#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	#define HFSM2_IF_UTILITY_THEORY(...)							  __VA_ARGS__
	#define HFSM2_UTILITY_THEORY_MASK									 (1 << 1)
#else
	#define HFSM2_IF_UTILITY_THEORY(...)
	#define HFSM2_UTILITY_THEORY_MASK									 (0 << 1)
#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_PLANS
	#define HFSM2_IF_PLANS(...)										  __VA_ARGS__
	#define HFSM2_PLANS_MASK											 (1 << 2)
#else
	#define HFSM2_IF_PLANS(...)
	#define HFSM2_PLANS_MASK											 (0 << 2)
#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION
	#define HFSM2_IF_SERIALIZATION(...)								  __VA_ARGS__
	#define HFSM2_SERIALIZATION_MASK									 (1 << 3)
#else
	#define HFSM2_IF_SERIALIZATION(...)
	#define HFSM2_SERIALIZATION_MASK									 (0 << 3)
#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY
	#define HFSM2_IF_TRANSITION_HISTORY(...)						  __VA_ARGS__
	#define HFSM2_TRANSITION_HISTORY_MASK								 (1 << 4)
#else
	#define HFSM2_IF_TRANSITION_HISTORY(...)
	#define HFSM2_TRANSITION_HISTORY_MASK								 (0 << 4)
#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	#define HFSM2_IF_STRUCTURE_REPORT(...)							  __VA_ARGS__
	#define HFSM2_STRUCTURE_REPORT_MASK									 (1 << 5)
#else
	#define HFSM2_IF_STRUCTURE_REPORT(...)
	#define HFSM2_STRUCTURE_REPORT_MASK									 (0 << 5)
#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_VERBOSE_DEBUG_LOG
	#define HFSM2_ENABLE_LOG_INTERFACE
	#define HFSM2_VERBOSE_DEBUG_LOG_MASK								 (1 << 6)
#else
	#define HFSM2_VERBOSE_DEBUG_LOG_MASK								 (0 << 6)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_LOG_INTERFACE

	#define HFSM2_IF_LOG_INTERFACE(...)								  __VA_ARGS__
	#define HFSM2_LOG_INTERFACE_MASK									 (1 << 7)

	#define HFSM2_LOG_TRANSITION(CONTEXT, ORIGIN, TYPE, DESTINATION)			\
		if (_logger)															\
			_logger->recordTransition(CONTEXT, ORIGIN, TYPE, DESTINATION)

#ifdef HFSM2_ENABLE_PLANS

	#define HFSM2_LOG_TASK_STATUS(CONTEXT, REGION, ORIGIN, STATUS)				\
		if (_logger)															\
			_logger->recordTaskStatus(CONTEXT, REGION, ORIGIN, STATUS)

	#define HFSM2_LOG_PLAN_STATUS(CONTEXT, REGION, STATUS)						\
		if (_logger)															\
			_logger->recordPlanStatus(CONTEXT, REGION, STATUS)

#endif

	#define HFSM2_LOG_CANCELLED_PENDING(CONTEXT, ORIGIN)						\
		if (_logger)															\
			_logger->recordCancelledPending(CONTEXT, ORIGIN)

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	#define HFSM2_LOG_UTILITY_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)			\
		if (auto* const logger = control.logger())								\
			logger->recordUtilityResolution(CONTEXT, HEAD, PRONG, UTILITY)

	#define HFSM2_LOG_RANDOM_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)			\
		if (auto* const logger = control.logger())								\
			logger->recordRandomResolution(CONTEXT, HEAD, PRONG, UTILITY)

#endif

#else

	#define HFSM2_IF_LOG_INTERFACE(...)
	#define HFSM2_LOG_INTERFACE_MASK									 (0 << 7)

	#define HFSM2_LOG_TRANSITION(CONTEXT, ORIGIN, TYPE, DESTINATION)

#ifdef HFSM2_ENABLE_PLANS
	#define HFSM2_LOG_TASK_STATUS(CONTEXT, REGION, ORIGIN, STATUS)
	#define HFSM2_LOG_PLAN_STATUS(CONTEXT, REGION, STATUS)
#endif

	#define HFSM2_LOG_CANCELLED_PENDING(CONTEXT, ORIGIN)

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	#define HFSM2_LOG_UTILITY_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)
	#define HFSM2_LOG_RANDOM_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)
#endif

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_VERBOSE_DEBUG_LOG

	#define HFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)							\
		if (auto* const logger = control.logger())								\
			logger->recordMethod(control.context(), STATE_ID, METHOD_ID)

#elif defined HFSM2_ENABLE_LOG_INTERFACE

	#define HFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)							\
		if (auto* const logger = control.logger())								\
			log(METHOD, *logger, control.context(), METHOD_ID)

#else

	#define HFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)

#endif

////////////////////////////////////////////////////////////////////////////////

namespace hfsm2 {

using FeatureTag = uint8_t;

constexpr FeatureTag HFSM2_FEATURE_TAG = HFSM2_TYPEINDEX_MASK
									   | HFSM2_PLANS_MASK
									   | HFSM2_UTILITY_THEORY_MASK
									   | HFSM2_SERIALIZATION_MASK
									   | HFSM2_TRANSITION_HISTORY_MASK
									   | HFSM2_STRUCTURE_REPORT_MASK
									   | HFSM2_VERBOSE_DEBUG_LOG_MASK
									   | HFSM2_LOG_INTERFACE_MASK;

}

//------------------------------------------------------------------------------

#undef HFSM2_TYPEINDEX_MASK
#undef HFSM2_UTILITY_THEORY_MASK
#undef HFSM2_PLANS_MASK
#undef HFSM2_SERIALIZATION_MASK
#undef HFSM2_TRANSITION_HISTORY_MASK
#undef HFSM2_STRUCTURE_REPORT_MASK
#undef HFSM2_VERBOSE_DEBUG_LOG_MASK
#undef HFSM2_LOG_INTERFACE_MASK

////////////////////////////////////////////////////////////////////////////////

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wextra-semi" // error : extra ';' inside a class
#endif

#if defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic" // error : extra ';'
#endif

//------------------------------------------------------------------------------

namespace hfsm2 {

//------------------------------------------------------------------------------

struct EmptyContext {};
struct EmptyPayload {};

//------------------------------------------------------------------------------

using Short		 = uint8_t;
static constexpr Short		INVALID_SHORT		= UINT8_MAX;

using RegionID	 = Short;
static constexpr RegionID	INVALID_REGION_ID	= INVALID_SHORT;

using ForkID	 = int8_t;
static constexpr ForkID		INVALID_FORK_ID		= INT8_MIN;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

using Long		 = uint16_t;
static constexpr Long		INVALID_LONG		= UINT16_MAX;

using StateID	 = Long;
static constexpr StateID	INVALID_STATE_ID	= INVALID_LONG;

//------------------------------------------------------------------------------

namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <bool B, class TT, class TF>
struct ConditionalT {
	using Type = TT;
};

template <class TT, class TF>
struct ConditionalT<false, TT, TF> {
	using Type = TF;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <bool B, class TT, class TF>
using Conditional = typename ConditionalT<B, TT, TF>::Type;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
HFSM2_INLINE void
fill(T& a, const char value) noexcept {
	memset(&a, (int) value, sizeof(a));
}

//------------------------------------------------------------------------------

template <typename TIndex, typename TElement, TIndex NCount>
constexpr TIndex
count(const TElement(&)[NCount]) noexcept {
	return NCount;
}

////////////////////////////////////////////////////////////////////////////////

template <int N1, int N2>
struct Min {
	static constexpr auto VALUE = N1 < N2 ? N1 : N2;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <int N1, int N2>
struct Max {
	static constexpr auto VALUE = N1 > N2 ? N1 : N2;
};

//------------------------------------------------------------------------------

template <typename T>
constexpr T
min(const T t1, const T t2) noexcept {
	return t1 < t2 ?
		t1 : t2;
}


//------------------------------------------------------------------------------

template <uint64_t NCapacity>
struct UnsignedCapacityT {
	static constexpr uint64_t CAPACITY = NCapacity;

	using Type = Conditional<CAPACITY <= UINT8_MAX,  uint8_t,
				 Conditional<CAPACITY <= UINT16_MAX, uint16_t,
				 Conditional<CAPACITY <= UINT32_MAX, uint32_t,
													 uint64_t>>>;
};

template <uint64_t NCapacity>
using UnsignedCapacity = typename UnsignedCapacityT<NCapacity>::Type;

//------------------------------------------------------------------------------

template <uint64_t NBitWidth>
struct UnsignedBitWidthT {
	static constexpr Short BIT_WIDTH = NBitWidth;

	using Type = Conditional<BIT_WIDTH <= 8,  uint8_t,
				 Conditional<BIT_WIDTH <= 16, uint16_t,
				 Conditional<BIT_WIDTH <= 32, uint32_t,
											  uint64_t>>>;

	static_assert(BIT_WIDTH <= 64, "STATIC ASSERT");
};

template <uint64_t NCapacity>
using UnsignedBitWidth = typename UnsignedBitWidthT<NCapacity>::Type;

//------------------------------------------------------------------------------

template <typename T1, typename T2>
constexpr T1
contain(const T1 x, const T2 to) noexcept {
	return (x + (T1) to - 1) / (T1) to;
}

//------------------------------------------------------------------------------

constexpr Short
bitWidth(const Short x) noexcept {
	return x <=   2 ? 1 :
		   x <=   4 ? 2 :
		   x <=   8 ? 3 :
		   x <=  16 ? 4 :
		   x <=  32 ? 5 :
		   x <=  64 ? 6 :
		   x <= 128 ? 7 :
					  8 ;
}

//------------------------------------------------------------------------------

template <typename TTo, typename TFrom>
HFSM2_INLINE void
overwrite(TTo& to, const TFrom& from) noexcept {
	static_assert(sizeof(TTo) == sizeof(TFrom), "STATIC ASSERT");

#if defined(__GNUC__) || defined(__GNUG__)
	memcpy  (&to,			  &from, sizeof(from));
#else
	memcpy_s(&to, sizeof(to), &from, sizeof(from));
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TO, typename TI>
HFSM2_INLINE TO
convert(const TI& in) noexcept {
	TO out;

	overwrite(out, in);

	return out;
}

////////////////////////////////////////////////////////////////////////////////

template <int>
struct StaticPrintConst;

template <typename>
struct StaticPrintType;

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContainer>
class IteratorT {
public:
	using Container = TContainer;
	using Item		= typename Container::Item;
	using Index		= typename Container::Index;

	template <typename, Long>
	friend class ArrayT;

private:
	HFSM2_INLINE IteratorT(Container& container,
						   const Index cursor)	  noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	HFSM2_INLINE bool operator != (const IteratorT<Container>& other) const noexcept;

	HFSM2_INLINE IteratorT& operator ++()		  noexcept;

	HFSM2_INLINE	   Item& operator *()		  noexcept	{ return  _container[_cursor];	}
	HFSM2_INLINE const Item& operator *()	const noexcept	{ return  _container[_cursor];	}

	HFSM2_INLINE	   Item* operator->()		  noexcept	{ return &_container[_cursor];	}
	HFSM2_INLINE const Item* operator->()	const noexcept	{ return &_container[_cursor];	}

private:
	Container& _container;

	Index _cursor;
};

//------------------------------------------------------------------------------

template <typename TContainer>
class IteratorT<const TContainer> {
public:
	using Container = TContainer;
	using Item		= typename Container::Item;
	using Index		= typename Container::Index;

	template <typename, Long>
	friend class ArrayT;

private:
	HFSM2_INLINE IteratorT(const Container& container,
						   const Index cursor)	  noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	HFSM2_INLINE bool operator != (const IteratorT<const Container>& other) const noexcept;

	HFSM2_INLINE IteratorT& operator ++()		  noexcept;

	HFSM2_INLINE const Item& operator *()	const noexcept	{ return _container[_cursor];	}

	HFSM2_INLINE const Item* operator->()	const noexcept	{ return &operator *();			}

private:
	const Container& _container;

	Index _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool
IteratorT<T>::operator != (const IteratorT<T>& HFSM2_IF_ASSERT(other)) const noexcept {
	HFSM2_ASSERT(&_container == &other._container);

	return _cursor != _container.limit();
}

//------------------------------------------------------------------------------

template <typename T>
IteratorT<T>&
IteratorT<T>::operator ++() noexcept {
	_cursor = _container.next(_cursor);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool
IteratorT<const T>::operator != (const IteratorT<const T>& HFSM2_IF_ASSERT(other)) const noexcept {
	HFSM2_ASSERT(&_container == &other._container);

	return _cursor != _container.limit();
}

//------------------------------------------------------------------------------

template <typename T>
IteratorT<const T>&
IteratorT<const T>::operator ++() noexcept {
	_cursor = _container.next(_cursor);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NCapacity>
class StaticArrayT {
public:
	using Item  = T;
	using Index = UnsignedCapacity<NCapacity>;

	static constexpr Index CAPACITY	= NCapacity;
	static constexpr Index DUMMY	= (Index) -1;

public:
	HFSM2_INLINE StaticArrayT() = default;
	HFSM2_INLINE StaticArrayT(const Item filler)					  noexcept;

	template <typename N>
	HFSM2_INLINE	   Item& operator[] (const N i)					  noexcept;

	template <typename N>
	HFSM2_INLINE const Item& operator[] (const N i)				const noexcept;

	HFSM2_INLINE Index count()									const noexcept	{ return CAPACITY;										}

	HFSM2_INLINE void fill(const Item filler)						  noexcept;
	HFSM2_INLINE void clear()										  noexcept	{ fill(INVALID_SHORT);									}

	HFSM2_INLINE IteratorT<      StaticArrayT>  begin()				  noexcept	{ return IteratorT<      StaticArrayT>(*this,     0);	}
	HFSM2_INLINE IteratorT<const StaticArrayT>  begin()			const noexcept	{ return IteratorT<const StaticArrayT>(*this,     0);	}
	HFSM2_INLINE IteratorT<const StaticArrayT> cbegin()			const noexcept	{ return IteratorT<const StaticArrayT>(*this,     0);	}

	HFSM2_INLINE IteratorT<      StaticArrayT>    end()				  noexcept	{ return IteratorT<      StaticArrayT>(*this, DUMMY);	}
	HFSM2_INLINE IteratorT<const StaticArrayT>    end()			const noexcept	{ return IteratorT<const StaticArrayT>(*this, DUMMY);	}
	HFSM2_INLINE IteratorT<const StaticArrayT>   cend()			const noexcept	{ return IteratorT<const StaticArrayT>(*this, DUMMY);	}

private:
	Item _items[CAPACITY];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
struct StaticArrayT<T, 0> {
	using Item  = T;

	HFSM2_INLINE StaticArrayT() = default;
	HFSM2_INLINE StaticArrayT(const Item) noexcept {}
};

//------------------------------------------------------------------------------

template <typename T, Long NCapacity>
class ArrayT {
	template <typename>
	friend class IteratorT;

public:
	using Item	= T;
	using Index = UnsignedCapacity<NCapacity>;

	static constexpr Index CAPACITY	= NCapacity;
	static constexpr Index DUMMY	= (Index) -1;

public:
	HFSM2_INLINE void clear()										  noexcept	{ _count = 0;		}

	// TODO: replace with 'emplace<>()'?
	template <typename TValue>
	HFSM2_INLINE Index append(const TValue&  value)					  noexcept;

	template <typename TValue>
	HFSM2_INLINE Index append(		TValue&& value)					  noexcept;

	template <typename N>
	HFSM2_INLINE	   Item& operator[] (const N i)					  noexcept;

	template <typename N>
	HFSM2_INLINE const Item& operator[] (const N i)				const noexcept;

	HFSM2_INLINE Index count()									const noexcept	{ return _count;	}

	template <Long N>
	HFSM2_INLINE ArrayT& operator += (const ArrayT<Item, N>& other)	  noexcept;

	HFSM2_INLINE IteratorT<      ArrayT>  begin()					  noexcept	{ return IteratorT<		 ArrayT>(*this,     0);	}
	HFSM2_INLINE IteratorT<const ArrayT>  begin()				const noexcept	{ return IteratorT<const ArrayT>(*this,     0);	}
	HFSM2_INLINE IteratorT<const ArrayT> cbegin()				const noexcept	{ return IteratorT<const ArrayT>(*this,     0);	}

	HFSM2_INLINE IteratorT<      ArrayT>	end()					  noexcept	{ return IteratorT<		 ArrayT>(*this, DUMMY);	}
	HFSM2_INLINE IteratorT<const ArrayT>	end()				const noexcept	{ return IteratorT<const ArrayT>(*this, DUMMY);	}
	HFSM2_INLINE IteratorT<const ArrayT>   cend()				const noexcept	{ return IteratorT<const ArrayT>(*this, DUMMY);	}

private:
	HFSM2_INLINE Index next(const Index i)						const noexcept	{ return i + 1;		}
	HFSM2_INLINE Index limit()									const noexcept	{ return _count;	}

private:
	Index _count = 0;

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

	Item _items[CAPACITY]; // warning 4324 triggers for 'StructureStateInfo'

#ifdef _MSC_VER
	#pragma warning(pop)
#endif
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
class ArrayT<T, 0> {
public:
	using Item	= T;
	using Index = UnsignedCapacity<0>;

	static constexpr Index CAPACITY = 0;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC>
StaticArrayT<T, NC>::StaticArrayT(const Item filler) noexcept {
	fill(filler);
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
template <typename N>
T&
StaticArrayT<T, NC>::operator[] (const N i) noexcept {
	HFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
const T&
StaticArrayT<T, NC>::operator[] (const N i) const noexcept {
	HFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
void
StaticArrayT<T, NC>::fill(const Item filler) noexcept {
	for (Index i = 0; i < CAPACITY; ++i)
		_items[i] = filler;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC>
template <typename TValue>
typename ArrayT<T, NC>::Index
ArrayT<T, NC>::append(const TValue& value) noexcept {
	HFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{value};

	return _count++;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename TValue>
typename ArrayT<T, NC>::Index
ArrayT<T, NC>::append(TValue&& value) noexcept {
	HFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{std::move(value)};

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
template <typename N>
T&
ArrayT<T, NC>::operator[] (const N i) noexcept {
	HFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
const T&
ArrayT<T, NC>::operator[] (const N i) const noexcept {
	HFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

//------------------------------------------------------------------------------
// SPECIFIC
// SPECIFIC

template <typename T, Long NC>
template <Long N>
ArrayT<T, NC>&
ArrayT<T, NC>::operator += (const ArrayT<T, N>& other) noexcept {
	for (const auto& item : other)
		append(item);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

struct Units {
	inline Units(Short unit_  = INVALID_SHORT,
				 Short width_ = INVALID_SHORT) noexcept
		: unit {unit_ }
		, width{width_}
	{}

	Short unit;
	Short width;
};

//------------------------------------------------------------------------------

template <typename TIndex, Short NUnitCount>
class BitArrayT final {
public:
	using Index	= TIndex;
	using Unit	= unsigned char;

	static constexpr Index UNIT_COUNT = NUnitCount;
	static constexpr Index UNIT_WIDTH = sizeof(Unit) * 8;
	static constexpr Index BIT_COUNT  = UNIT_COUNT * UNIT_WIDTH;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class Bits {
		template <typename, Short>
		friend class BitArrayT;

	private:
		HFSM2_INLINE explicit Bits(Unit* const storage,
								   const Index width)		  noexcept
			: _storage{storage}
			, _width{width}
		{}

	public:
		HFSM2_INLINE explicit operator bool()			const noexcept;

		HFSM2_INLINE void clear()							  noexcept;

		template <Short NIndex>
		HFSM2_INLINE bool get()							const noexcept;

		template <Short NIndex>
		HFSM2_INLINE void set()								  noexcept;

		template <Short NIndex>
		HFSM2_INLINE void clear()							  noexcept;

		HFSM2_INLINE bool get  (const Index index)		const noexcept;
		HFSM2_INLINE void set  (const Index index)			  noexcept;
		HFSM2_INLINE void clear(const Index index)			  noexcept;

	private:
		Unit* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class CBits {
		template <typename, Short>
		friend class BitArrayT;

	private:
		HFSM2_INLINE explicit CBits(const Unit* const storage,
									const Index width)		  noexcept
			: _storage{storage}
			, _width{width}
		{}

	public:
		HFSM2_INLINE explicit operator bool()			const noexcept;

		template <Short NIndex>
		HFSM2_INLINE bool get()							const noexcept;

		HFSM2_INLINE bool get(const Index index)		const noexcept;

	private:
		const Unit* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	BitArrayT() noexcept {
		clear();
	}

	HFSM2_INLINE void clear()								  noexcept;

	template <Short NIndex>
	HFSM2_INLINE bool get()								const noexcept;

	template <Short NIndex>
	HFSM2_INLINE void set()									  noexcept;

	template <Short NIndex>
	HFSM2_INLINE void clear()								  noexcept;

	HFSM2_INLINE bool get  (const Index index)			const noexcept;
	HFSM2_INLINE void set  (const Index index)				  noexcept;
	HFSM2_INLINE void clear(const Index index)				  noexcept;

	template <Short NUnit, Short NWidth>
	HFSM2_INLINE  Bits bits()								  noexcept;

	template <Short NUnit, Short NWidth>
	HFSM2_INLINE CBits bits()							const noexcept;

	HFSM2_INLINE  Bits bits(const Units& units)				  noexcept;
	HFSM2_INLINE CBits bits(const Units& units)			const noexcept;

private:
	Unit _storage[UNIT_COUNT];
};

//------------------------------------------------------------------------------

template <typename TIndex>
class BitArrayT<TIndex, 0> final {
public:
	HFSM2_INLINE void clear() noexcept {}
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
BitArrayT<TI, NC>::Bits::operator bool() const noexcept {
	const Short fullUnits = _width / UNIT_WIDTH;

	// TODO: cover this case
	for (Index i = 0; i < fullUnits; ++i)
		if (_storage[i])
			return true;

	const Short bit = _width % UNIT_WIDTH;
	const Unit mask = (1 << bit) - 1;
	const Unit& u = _storage[fullUnits];

	return (u & mask) != 0;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
void
BitArrayT<TI, NC>::Bits::clear() noexcept {
	const Index unitCount = contain(_width, UNIT_WIDTH);

	for (Index i = 0; i < unitCount; ++i)
		_storage[i] = Unit{0};
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
template <Short NIndex>
bool
BitArrayT<TI, NC>::Bits::get() const noexcept {
	constexpr Index INDEX = NIndex;
	HFSM2_ASSERT(INDEX < _width);

	constexpr Index BIT_UNIT  = INDEX / UNIT_WIDTH;
	constexpr Index BIT_INDEX = INDEX % UNIT_WIDTH;
	constexpr Unit MASK = 1 << BIT_INDEX;

	return (_storage[BIT_UNIT] & MASK) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
template <Short NIndex>
void
BitArrayT<TI, NC>::Bits::set() noexcept {
	constexpr Index INDEX = NIndex;
	HFSM2_ASSERT(INDEX < _width);

	constexpr Index BIT_UNIT  = INDEX / UNIT_WIDTH;
	constexpr Index BIT_INDEX = INDEX % UNIT_WIDTH;
	constexpr Unit MASK = 1 << BIT_INDEX;

	_storage[BIT_UNIT] |= MASK;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
template <Short NIndex>
void
BitArrayT<TI, NC>::Bits::clear() noexcept {
	constexpr Index INDEX = NIndex;
	HFSM2_ASSERT(INDEX < _width);

	constexpr Index BIT_UNIT  = INDEX / UNIT_WIDTH;
	constexpr Index BIT_INDEX = INDEX % UNIT_WIDTH;
	constexpr Unit MASK = 1 << BIT_INDEX;

	_storage[BIT_UNIT] &= ~MASK;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
bool
BitArrayT<TI, NC>::Bits::get(const Index index) const noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
void
BitArrayT<TI, NC>::Bits::set(const Index index) noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
void
BitArrayT<TI, NC>::Bits::clear(const Index index) noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	_storage[unit] &= ~mask;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
BitArrayT<TI, NC>::CBits::operator bool() const noexcept {
	const Short fullUnits = _width / UNIT_WIDTH;

	for (Index i = 0; i < fullUnits; ++i)
		if (_storage[i])
			return true;

	const Short bit = _width % UNIT_WIDTH;
	const Unit mask = (1 << bit) - 1;
	const Unit& u = _storage[fullUnits];

	return (u & mask) != 0;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
template <Short NIndex>
bool
BitArrayT<TI, NC>::CBits::get() const noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < _width, "");

	constexpr Index BIT_UNIT  = INDEX / UNIT_WIDTH;
	constexpr Index BIT_INDEX = INDEX % UNIT_WIDTH;
	constexpr Unit MASK = 1 << BIT_INDEX;

	return (_storage[BIT_UNIT] & MASK) != 0;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
bool
BitArrayT<TI, NC>::CBits::get(const Index index) const noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
void
BitArrayT<TI, NC>::clear() noexcept {
	for (Unit& unit: _storage)
		unit = Unit{0};
}

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
template <Short NIndex>
bool
BitArrayT<TI, NC>::get() const noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < BIT_COUNT, "");

	constexpr Index BIT_UNIT  = INDEX / UNIT_WIDTH;
	constexpr Index BIT_INDEX = INDEX % UNIT_WIDTH;
	constexpr Unit MASK = 1 << BIT_INDEX;

	return (_storage[BIT_UNIT] & MASK) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
template <Short NIndex>
void
BitArrayT<TI, NC>::set() noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < BIT_COUNT, "");

	constexpr Index BIT_UNIT  = INDEX / UNIT_WIDTH;
	constexpr Index BIT_INDEX = INDEX % UNIT_WIDTH;
	constexpr Unit MASK = 1 << BIT_INDEX;

	_storage[BIT_UNIT] |= MASK;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
template <Short NIndex>
void
BitArrayT<TI, NC>::clear() noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < BIT_COUNT, "");

	constexpr Index BIT_UNIT  = INDEX / UNIT_WIDTH;
	constexpr Index BIT_INDEX = INDEX % UNIT_WIDTH;
	constexpr Unit MASK = 1 << BIT_INDEX;

	_storage[BIT_UNIT] &= ~MASK;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
bool
BitArrayT<TI, NC>::get(const Index index) const noexcept {
	HFSM2_ASSERT(index < BIT_COUNT);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
void
BitArrayT<TI, NC>::set(const Index index) noexcept {
	HFSM2_ASSERT(index < BIT_COUNT);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
void
BitArrayT<TI, NC>::clear(const Index index) noexcept {
	HFSM2_ASSERT(index < BIT_COUNT);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	_storage[unit] &= ~mask;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
template <Short NUnit, Short NWidth>
typename BitArrayT<TI, NC>::Bits
BitArrayT<TI, NC>::bits() noexcept {
	constexpr Short UNIT  = NUnit;
	constexpr Short WIDTH = NWidth;
	static_assert(UNIT + contain(WIDTH, UNIT_WIDTH) <= UNIT_COUNT, "");

	return Bits{_storage + UNIT, WIDTH};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
template <Short NUnit, Short NWidth>
typename BitArrayT<TI, NC>::CBits
BitArrayT<TI, NC>::bits() const noexcept {
	constexpr Short UNIT  = NUnit;
	constexpr Short WIDTH = NWidth;
	static_assert(UNIT + contain(WIDTH, UNIT_WIDTH) <= UNIT_COUNT, "");

	return CBits{_storage + UNIT, WIDTH};
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
typename BitArrayT<TI, NC>::Bits
BitArrayT<TI, NC>::bits(const Units& units) noexcept {
	HFSM2_ASSERT(units.unit + contain(units.width, UNIT_WIDTH) <= UNIT_COUNT);

	return Bits{_storage + units.unit, units.width};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
typename BitArrayT<TI, NC>::CBits
BitArrayT<TI, NC>::bits(const Units& units) const noexcept {
	HFSM2_ASSERT(units.unit + contain(units.width, UNIT_WIDTH) <= UNIT_COUNT);

	return CBits{_storage + units.unit, units.width};
}

////////////////////////////////////////////////////////////////////////////////

}
}
#ifdef HFSM2_ENABLE_SERIALIZATION

namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

template <Long>
class BitWriteStreamT;

template <Long>
class BitReadStreamT;

////////////////////////////////////////////////////////////////////////////////

template <Long NBitCapacity>
class StreamBufferT {
	template <Long>
	friend class BitWriteStreamT;

	template <Long>
	friend class BitReadStreamT;

public:
	static constexpr Long BIT_CAPACITY	= NBitCapacity;
	static constexpr Long BYTE_COUNT	= contain(BIT_CAPACITY, 8u);

	using Data = uint8_t[BYTE_COUNT];

	HFSM2_INLINE void clear()										  noexcept;

	HFSM2_INLINE	   Data& data()									  noexcept	{ return _data;		}
	HFSM2_INLINE const Data& data()								const noexcept	{ return _data;		}

private:
	Data _data;
};

////////////////////////////////////////////////////////////////////////////////

template <Long NBitCapacity>
class BitWriteStreamT final {
public:
	static constexpr Long BIT_CAPACITY = NBitCapacity;

	using Buffer = StreamBufferT<BIT_CAPACITY>;

public:
	HFSM2_INLINE explicit BitWriteStreamT(Buffer& buffer,
										  const Long cursor = 0)	  noexcept;

	template <Short NBitWidth>
	HFSM2_INLINE void write(const UnsignedBitWidth<NBitWidth> item)	  noexcept;

	HFSM2_INLINE Long cursor()									const noexcept	{ return _cursor;	}

private:
	Buffer& _buffer;

	Long _cursor = 0;
};

//------------------------------------------------------------------------------

template <Long NBitCapacity>
class BitReadStreamT final {
public:
	static constexpr Long BIT_CAPACITY = NBitCapacity;

	using Buffer = StreamBufferT<BIT_CAPACITY>;

public:
	HFSM2_INLINE explicit BitReadStreamT(const Buffer& buffer,
										 const Long cursor = 0)		  noexcept;

	template <Short NBitWidth>
	HFSM2_INLINE UnsignedBitWidth<NBitWidth> read()					  noexcept;

	HFSM2_INLINE Long cursor()									const noexcept	{ return _cursor;	}

private:
	const Buffer& _buffer;

	Long _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#ifdef HFSM2_ENABLE_SERIALIZATION

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <Long NBC>
void
StreamBufferT<NBC>::clear() noexcept {
	fill(_data, 0);
}

////////////////////////////////////////////////////////////////////////////////

template <Long NBC>
BitWriteStreamT<NBC>::BitWriteStreamT(Buffer& buffer,
									  const Long cursor) noexcept
	: _buffer{buffer}
	, _cursor{cursor}
{
	_buffer.clear();
}

//------------------------------------------------------------------------------

template <Long NBC>
template <Short NBitWidth>
void
BitWriteStreamT<NBC>::write(const UnsignedBitWidth<NBitWidth> item) noexcept {
	constexpr Short BIT_WIDTH = NBitWidth;
	static_assert(BIT_WIDTH > 0, "STATIC ASSERT");

	HFSM2_ASSERT(_cursor + BIT_WIDTH <= BIT_CAPACITY);

	using Item = UnsignedBitWidth<BIT_WIDTH>;

	Item itemBits = item;

	for (Short itemWidth = BIT_WIDTH; itemWidth; ) {
		const Long	byteIndex		= _cursor >> 3;
		uint8_t&	byte			= _buffer._data[byteIndex];

		const Short byteChunkStart	= _cursor & 0x7;
		const Short byteDataWidth	= 8 - byteChunkStart;
		const Short byteChunkWidth	= detail::min(byteDataWidth, itemWidth);
		const Item	byteChunk		= itemBits << byteChunkStart;

		byte		|= byteChunk;
		itemBits   >>= byteChunkWidth;
		itemWidth	-= byteChunkWidth;
		_cursor		+= byteChunkWidth;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <Long NBC>
BitReadStreamT<NBC>::BitReadStreamT(const Buffer& buffer,
									const Long cursor) noexcept
	: _buffer{buffer}
	, _cursor{cursor}
{}

//------------------------------------------------------------------------------

template <Long NBC>
template <Short NBitWidth>
UnsignedBitWidth<NBitWidth>
BitReadStreamT<NBC>::read() noexcept {
	constexpr Short BIT_WIDTH = NBitWidth;
	static_assert(BIT_WIDTH > 0, "STATIC ASSERT");

	HFSM2_ASSERT(_cursor <= BIT_CAPACITY);

	using Item = UnsignedBitWidth<BIT_WIDTH>;

	Item item = 0;
	Short itemCursor = 0;

	for (Short itemWidth = BIT_WIDTH; itemWidth; )
		if (HFSM2_CHECKED(_cursor + itemWidth <= BIT_CAPACITY)) {
			const Long	byteIndex		= _cursor >> 3;
			const uint8_t& byte			= _buffer._data[byteIndex];

			const Short byteChunkStart	= _cursor & 0x7;
			const Short byteDataWidth	= 8 - byteChunkStart;
			const Short byteChunkWidth	= detail::min(byteDataWidth, itemWidth);
			const Short byteChunkMask	= (1 << byteChunkWidth) - 1;
			const Item	byteChunk		= (byte >> byteChunkStart) & byteChunkMask;
			const Item	itemChunk		= byteChunk << itemCursor;

			item		|= itemChunk;
			itemCursor	+= byteChunkWidth;
			itemWidth	-= byteChunkWidth;
			_cursor		+= byteChunkWidth;
		}

	return item;
}

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#endif
#ifdef HFSM2_ENABLE_UTILITY_THEORY

namespace hfsm2 {

////////////////////////////////////////////////////////////////////////////////
// http://xoshiro.di.unimi.it/splitmix64.c

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 26495) // variable is uninitialized
#endif

class SplitMix64 {
public:
	inline SplitMix64()								  noexcept {}
	inline SplitMix64(const uint64_t seed)			  noexcept;

	inline uint64_t next()							  noexcept;

private:
	inline uint64_t raw()							  noexcept;

private:

	uint64_t _state;
};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

//------------------------------------------------------------------------------
// xoshiro256+ (http://xoshiro.di.unimi.it/xoshiro256plus.c)

class XoShiRo256Plus {
public:
	inline XoShiRo256Plus() noexcept;
	inline XoShiRo256Plus(const uint64_t s)			  noexcept;
	inline XoShiRo256Plus(const uint64_t(& s)[4])	  noexcept;

	inline void seed(const uint64_t s)				  noexcept;
	inline void seed(const uint64_t(& s)[4])		  noexcept;

	inline float next()								  noexcept;
	inline void jump()								  noexcept;

private:
	inline uint64_t raw()							  noexcept;

private:
	uint64_t _state[4];
};

////////////////////////////////////////////////////////////////////////////////
// https://groups.google.com/forum/#!topic/prng/VFjdFmbMgZI

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 26495) // variable is uninitialized
#endif

class SplitMix32 {
public:
	inline SplitMix32()								  noexcept {}
	inline SplitMix32(const uint32_t seed)			  noexcept;

	inline uint32_t next()							  noexcept;

private:
	inline uint32_t raw()							  noexcept;

private:
	uint32_t _state;
};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

//------------------------------------------------------------------------------
// http://xoshiro.di.unimi.it/xoshiro128plus.c

class XoShiRo128Plus {
public:
	inline XoShiRo128Plus() noexcept;
	inline XoShiRo128Plus(const uint32_t s)			  noexcept;
	inline XoShiRo128Plus(const uint32_t(& s)[4])	  noexcept;

	inline void seed(const uint32_t s)				  noexcept;
	inline void seed(const uint32_t(& s)[4])		  noexcept;

	inline float next()								  noexcept;
	inline void jump()								  noexcept;

private:
	inline uint32_t raw()							  noexcept;

private:
	uint32_t _state[4];
};

//------------------------------------------------------------------------------

template <typename T>
class RNGT;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <>
class RNGT<float>
	: public HFSM2_64BIT_OR_32BIT(XoShiRo256Plus, XoShiRo128Plus)
{
public:
	using HFSM2_64BIT_OR_32BIT(XoShiRo256Plus::XoShiRo256Plus,
							   XoShiRo128Plus::XoShiRo128Plus);
};

////////////////////////////////////////////////////////////////////////////////

}

#endif

#ifdef HFSM2_ENABLE_UTILITY_THEORY

namespace hfsm2 {

////////////////////////////////////////////////////////////////////////////////

namespace detail {

inline
float
uniformReal(const uint32_t uint) noexcept {
	const auto real = convert<float>(UINT32_C(0x7F) << 23 | uint >> 9);

	return real - 1.0f;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline
double
uniformReal(const uint64_t uint) noexcept {
	const auto real = convert<double>(UINT64_C(0x3FF) << 52 | uint >> 12);

	return real - 1.0;
}

//------------------------------------------------------------------------------

inline
uint32_t
rotl(const uint32_t x, const uint32_t k) noexcept {
	return (x << k) | (x >> (32 - k));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline
uint64_t
rotl(const uint64_t x, const uint64_t k) noexcept {
	return (x << k) | (x >> (64 - k));
}

}

////////////////////////////////////////////////////////////////////////////////

SplitMix64::SplitMix64(const uint64_t seed) noexcept
	: _state{seed}
{}

//------------------------------------------------------------------------------

uint64_t
SplitMix64::next() noexcept {
	for (;;)
		if (const uint64_t number = raw())
			return number;
}

//------------------------------------------------------------------------------

uint64_t
SplitMix64::raw() noexcept {
	uint64_t z = (_state += 0x9e3779b97f4a7c15);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;

	return z ^ (z >> 31);
}

////////////////////////////////////////////////////////////////////////////////

XoShiRo256Plus::XoShiRo256Plus() noexcept {
	SplitMix64 generator;

	_state[0] = generator.next();
	_state[1] = generator.next();
	_state[2] = generator.next();
	_state[3] = generator.next();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

XoShiRo256Plus::XoShiRo256Plus(const uint64_t s) noexcept {
	seed(s);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

XoShiRo256Plus::XoShiRo256Plus(const uint64_t(& s)[4]) noexcept {
	seed(s);
}

//------------------------------------------------------------------------------

void
XoShiRo256Plus::seed(const uint64_t s) noexcept {
	SplitMix64 generator{s};

	_state[0] = generator.next();
	_state[1] = generator.next();
	_state[2] = generator.next();
	_state[3] = generator.next();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
XoShiRo256Plus::seed(const uint64_t(& s)[4]) noexcept {
	_state[0] = s[0];
	_state[1] = s[1];
	_state[2] = s[2];
	_state[3] = s[3];
}

//------------------------------------------------------------------------------

float
XoShiRo256Plus::next() noexcept {
	return detail::uniformReal((uint32_t) raw());
}

//------------------------------------------------------------------------------

void
XoShiRo256Plus::jump() noexcept {
	static const uint64_t JUMP[] = { 0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c };

	uint64_t s0 = 0;
	uint64_t s1 = 0;
	uint64_t s2 = 0;
	uint64_t s3 = 0;

	for (unsigned i = 0; i < detail::count<unsigned>(JUMP); ++i)
		for (int b = 0; b < 64; ++b) {
			if (JUMP[i] & UINT64_C(1) << b) {
				s0 ^= _state[0];
				s1 ^= _state[1];
				s2 ^= _state[2];
				s3 ^= _state[3];
			}
			raw();
		}

	_state[0] = s0;
	_state[1] = s1;
	_state[2] = s2;
	_state[3] = s3;
}

//------------------------------------------------------------------------------

uint64_t
XoShiRo256Plus::raw() noexcept {
	const uint64_t result_plus = _state[0] + _state[3];

	const uint64_t t = _state[1] << 17;

	_state[2] ^= _state[0];
	_state[3] ^= _state[1];
	_state[1] ^= _state[2];
	_state[0] ^= _state[3];

	_state[2] ^= t;

	_state[3] = detail::rotl(_state[3], 45);

	return result_plus;
}

////////////////////////////////////////////////////////////////////////////////

SplitMix32::SplitMix32(const uint32_t seed) noexcept
	: _state{seed}
{}

//------------------------------------------------------------------------------

uint32_t
SplitMix32::next() noexcept {
	for (;;)
		if (const uint32_t number = raw())
			return number;
}

//------------------------------------------------------------------------------

uint32_t
SplitMix32::raw() noexcept {
	uint32_t z = (_state += 0x9E3779B9);
	z = (z ^ (z >> 16)) * 0x85ebca6b;
	z = (z ^ (z >> 13)) * 0xc2b2ae35;

	return z ^ (z >> 16);
}

////////////////////////////////////////////////////////////////////////////////

XoShiRo128Plus::XoShiRo128Plus() noexcept {
	SplitMix32 generator;

	_state[0] = generator.next();
	_state[1] = generator.next();
	_state[2] = generator.next();
	_state[3] = generator.next();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

XoShiRo128Plus::XoShiRo128Plus(const uint32_t s) noexcept {
	seed(s);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

XoShiRo128Plus::XoShiRo128Plus(const uint32_t(& s)[4]) noexcept {
	seed(s);
}

//------------------------------------------------------------------------------

void
XoShiRo128Plus::seed(const uint32_t s) noexcept {
	SplitMix32 generator{s};

	_state[0] = generator.next();
	_state[1] = generator.next();
	_state[2] = generator.next();
	_state[3] = generator.next();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
XoShiRo128Plus::seed(const uint32_t(& s)[4]) noexcept {
	_state[0] = s[0];
	_state[1] = s[1];
	_state[2] = s[2];
	_state[3] = s[3];
}

//------------------------------------------------------------------------------

float
XoShiRo128Plus::next() noexcept {
	return detail::uniformReal(raw());
}

//------------------------------------------------------------------------------

void
XoShiRo128Plus::jump() noexcept {
	static const uint32_t JUMP[] = { 0x8764000b, 0xf542d2d3, 0x6fa035c3, 0x77f2db5b };

	uint32_t s0 = 0;
	uint32_t s1 = 0;
	uint32_t s2 = 0;
	uint32_t s3 = 0;

	for (unsigned i = 0; i < detail::count<unsigned>(JUMP); ++i)
		for (unsigned b = 0; b < 32; ++b) {
			if (JUMP[i] & UINT32_C(1) << b) {
				s0 ^= _state[0];
				s1 ^= _state[1];
				s2 ^= _state[2];
				s3 ^= _state[3];
			}
			raw();
		}

	_state[0] = s0;
	_state[1] = s1;
	_state[2] = s2;
	_state[3] = s3;
}

//------------------------------------------------------------------------------

uint32_t
XoShiRo128Plus::raw() noexcept {
	const uint32_t result_plus = _state[0] + _state[3];

	const uint32_t t = _state[1] << 9;

	_state[2] ^= _state[0];
	_state[3] ^= _state[1];
	_state[1] ^= _state[2];
	_state[0] ^= _state[3];

	_state[2] ^= t;

	_state[3] = detail::rotl(_state[3], 11);

	return result_plus;
}

////////////////////////////////////////////////////////////////////////////////

}

#endif
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// SPECIFIC

template <typename... Ts>
struct TL_ {
	static constexpr Long SIZE = sizeof...(Ts);
};

// SPECIFIC
//------------------------------------------------------------------------------

template <Long N>
struct Const {
	static constexpr Long VALUE = N;
};

//------------------------------------------------------------------------------

template <typename, typename>
struct PrependT;

template <typename T, typename... Ts>
struct PrependT<T, TL_<Ts...>> {
	using Type = TL_<T, Ts...>;
};

template <typename... Ts>
using PrependTypes = typename PrependT<Ts...>::Type;

//------------------------------------------------------------------------------

template <typename, typename>
struct MergeT;

template <typename... Ts1, typename... Ts2>
struct MergeT<TL_<Ts1...>, TL_<Ts2...>> {
	using Type = TL_<Ts1..., Ts2...>;
};

template <typename... Ts>
using Merge = typename MergeT<Ts...>::Type;

//------------------------------------------------------------------------------

template <Long, Long, typename...>
struct LowerT;

template <Long NHalf, Long NIndex, typename... Ts>
using LowerTypes = typename LowerT<NHalf, NIndex, Ts...>::Type;

template <Long NHalf, Long NIndex, typename TFirst, typename... TRest>
struct LowerT<NHalf, NIndex, TFirst, TRest...> {
	using LTypeList = typename LowerT<NHalf, NIndex + 1, TRest...>::Type;

	using Type = Conditional<
					 (NIndex < NHalf),
					 PrependTypes<TFirst, LTypeList>,
					 LTypeList
				 >;
};

template <Long NHalf, Long NIndex>
struct LowerT<NHalf, NIndex> {
	using Type = TL_<>;
};

template <typename... Ts>
using LHalfTypes = LowerTypes<sizeof...(Ts) / 2, 0, Ts...>;

//------------------------------------------------------------------------------

template <Long, Long, typename...>
struct UpperT;

template <Long NHalf, Long NIndex, typename... Ts>
using UpperTypes = typename UpperT<NHalf, NIndex, Ts...>::Type;

template <Long NHalf, Long NIndex, typename TFirst, typename... TRest>
struct UpperT<NHalf, NIndex, TFirst, TRest...> {
	using Type = Conditional<
					 (NIndex < NHalf),
					 UpperTypes<NHalf, NIndex + 1, TRest...>,
					 TL_<TFirst, TRest...>
				 >;
};

template <Long NHalf, Long NIndex>
struct UpperT<NHalf, NIndex> {
	using Type = TL_<>;
};

template <typename... Ts>
using RHalfTypes = UpperTypes<sizeof...(Ts) / 2, 0, Ts...>;

//------------------------------------------------------------------------------

template<Long, typename...>
struct FindImpl
	: Const<INVALID_LONG>
{};

template<Long N, typename T, typename TFirst, typename... TRest>
struct FindImpl<N, T, TFirst, TRest...>
	: FindImpl<N + 1, T, TRest...>
{};

template<Long N, typename T, typename... Ts>
struct FindImpl<N, T, T, Ts...>
	: Const<N>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename, typename>
struct Find;

template <typename T, typename... Ts>
struct Find<TL_<Ts...>, T>
	: FindImpl<0, T, Ts...>
{};

////////////////////////////////////////////////////////////////////////////////
// SPECIFIC

}

template <typename TList, typename T>
constexpr Long index   () noexcept { return detail::Find<TList, T>::VALUE;		}

template <typename TList, typename T>
constexpr bool contains() noexcept { return index<TList, T>() != INVALID_LONG;	}

// SPECIFIC
//------------------------------------------------------------------------------

}

namespace hfsm2 {

////////////////////////////////////////////////////////////////////////////////

enum class Method : uint8_t {
	NONE,

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	RANK,
	UTILITY,
#endif

	ENTRY_GUARD,
	CONSTRUCT,
	ENTER,
	REENTER,
	UPDATE,
	REACT,
	EXIT_GUARD,
	EXIT,
	DESTRUCT,

#ifdef HFSM2_ENABLE_PLANS
	PLAN_SUCCEEDED,
	PLAN_FAILED,
#endif

	COUNT
};

//------------------------------------------------------------------------------

enum class TransitionType : uint8_t {
	CHANGE,
	RESTART,
	RESUME,

//#ifdef HFSM2_ENABLE_UTILITY_THEORY
	UTILIZE,
	RANDOMIZE,
//#endif

	SCHEDULE,

	COUNT
};

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_PLANS

enum class StatusEvent : uint8_t {
	SUCCEEDED,
	FAILED,

	COUNT
};

#endif

//------------------------------------------------------------------------------

static inline
const char*
stateName(const std::type_index stateType) noexcept {
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
methodName(const Method method) noexcept {
	switch (method) {

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	case Method::RANK:			 return "rank";
	case Method::UTILITY:		 return "utility";
#endif

	case Method::ENTRY_GUARD:	 return "entryGuard";
	case Method::ENTER:			 return "enter";
	case Method::CONSTRUCT:		 return "construct";
	case Method::REENTER:		 return "reenter";
	case Method::UPDATE:		 return "update";
	case Method::REACT:			 return "react";
	case Method::EXIT_GUARD:	 return "exitGuard";
	case Method::EXIT:			 return "exit";
	case Method::DESTRUCT:		 return "destruct";

#ifdef HFSM2_ENABLE_PLANS
	case Method::PLAN_SUCCEEDED: return "planSucceeded";
	case Method::PLAN_FAILED:	 return "planFailed";
#endif

	default:
		HFSM2_BREAK();
		return nullptr;
	}
}

//------------------------------------------------------------------------------

static inline
const char*
transitionName(const TransitionType type) noexcept {
	switch (type) {
	case TransitionType::CHANGE:	return "changeTo";
	case TransitionType::RESTART:	return "restart";
	case TransitionType::RESUME:	return "resume";

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	case TransitionType::UTILIZE:	return "utilize";
	case TransitionType::RANDOMIZE:	return "randomize";
#endif

	case TransitionType::SCHEDULE:	return "schedule";

	default:
		HFSM2_BREAK();
		return nullptr;
	}
}

//------------------------------------------------------------------------------

namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

struct alignas(4) TransitionBase {
	constexpr TransitionBase() noexcept = default;

	//----------------------------------------------------------------------

	HFSM2_INLINE TransitionBase(const StateID destination_,
								const TransitionType type_) noexcept
		: destination{destination_}
		, type{type_}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE TransitionBase(const StateID origin_,
								const StateID destination_,
								const TransitionType type_) noexcept
		: origin	 {origin_}
		, destination{destination_}
		, type		 {type_}
	{}

	//----------------------------------------------------------------------

	HFSM2_INLINE bool operator == (const TransitionBase& other) const noexcept {
		return origin	   == other.origin &&
			   destination == other.destination &&
			   method	   == other.method &&
			   type		   == other.type;
	}

	//----------------------------------------------------------------------

	StateID	origin		= INVALID_STATE_ID;
	StateID	destination	= INVALID_STATE_ID;
	Method	method		= Method::NONE;
	TransitionType type	= TransitionType::COUNT;
};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TPayload>
struct alignas(4) TransitionT
	: TransitionBase
{
	using Payload = TPayload;
	using Storage = typename std::aligned_storage<sizeof(Payload), 4>::type;

	using TransitionBase::TransitionBase;

	//----------------------------------------------------------------------

	HFSM2_INLINE TransitionT() noexcept {
		new (&storage) Payload{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE TransitionT(const StateID destination,
							 const TransitionType type,
							 const Payload& payload) noexcept
		: TransitionBase{destination, type}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE TransitionT(const StateID destination,
							 const TransitionType type,
							 Payload&& payload) noexcept
		: TransitionBase{destination, type}
		, payloadSet{true}
	{
		new (&storage) Payload{std::move(payload)};
	}

	//----------------------------------------------------------------------

	HFSM2_INLINE TransitionT(const StateID origin,
							 const StateID destination,
							 const TransitionType type,
							 const Payload& payload) noexcept
		: TransitionBase{origin, destination, type}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE TransitionT(const StateID origin,
							 const StateID destination,
							 const TransitionType type,
							 Payload&& payload) noexcept
		: TransitionBase{origin, destination, type}
		, payloadSet{true}
	{
		new (&storage) Payload{std::move(payload)};
	}

	//----------------------------------------------------------------------

	HFSM2_INLINE bool operator == (const TransitionT& other) const noexcept {
		return TransitionBase::operator == (other) &&
			   (payloadSet ==  other.payloadSet);
		//	  (!payloadSet && !other.payloadSet || payload ==  other.payload);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	bool payloadSet = false;
	Storage storage;
};

//------------------------------------------------------------------------------

template <>
struct alignas(4) TransitionT<void>
	: TransitionBase
{
	using TransitionBase::TransitionBase;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}

struct Request {
	TransitionType type;
	Short index;
};

////////////////////////////////////////////////////////////////////////////////

}
namespace hfsm2 {

#ifdef HFSM2_ENABLE_LOG_INTERFACE

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFeatureTag = HFSM2_FEATURE_TAG
		, typename TContext = EmptyContext
		HFSM2_IF_UTILITY_THEORY(, typename TUtilty = float)>
struct LoggerInterfaceT {
	using Context		 = TContext;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Utilty		 = TUtilty;
#endif

	using Method		 = ::hfsm2::Method;
	using StateID		 = ::hfsm2::StateID;
	using RegionID		 = ::hfsm2::RegionID;
	using TransitionType = ::hfsm2::TransitionType;

#ifdef HFSM2_ENABLE_PLANS
	using StatusEvent	 = ::hfsm2::StatusEvent;
#endif

	virtual void recordMethod(Context& /*context*/,
							  const StateID /*origin*/,
							  const Method /*method*/) noexcept
	{}

	virtual void recordTransition(Context& /*context*/,
								  const StateID /*origin*/,
								  const TransitionType /*transitionType*/,
								  const StateID /*target*/) noexcept
	{}

#ifdef HFSM2_ENABLE_PLANS

	virtual void recordTaskStatus(Context& /*context*/,
								  const RegionID /*region*/,
								  const StateID /*origin*/,
								  const StatusEvent /*event*/) noexcept
	{}

	virtual void recordPlanStatus(Context& /*context*/,
								  const RegionID /*region*/,
								  const StatusEvent /*event*/) noexcept
	{}

#endif

	virtual void recordCancelledPending(Context& /*context*/,
										const StateID /*origin*/) noexcept
	{}

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	virtual void recordUtilityResolution(Context& /*context*/,
										 const StateID /*head*/,
										 const StateID /*prong*/,
										 const Utilty /*utilty*/) noexcept
	{}

	virtual void recordRandomResolution(Context& /*context*/,
										const StateID /*head*/,
										const StateID /*prong*/,
										const Utilty /*utilty*/) noexcept
	{}

#endif
};

////////////////////////////////////////////////////////////////////////////////

#else

template <FeatureTag NFeatureTag = HFSM2_FEATURE_TAG
		, typename TContext = EmptyContext
		HFSM2_IF_UTILITY_THEORY(, typename TUtilty = float)>
using LoggerInterfaceT = void;

#endif

using LoggerInterface = LoggerInterfaceT<>;

}
#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

namespace hfsm2 {

//------------------------------------------------------------------------------

struct StructureEntry {
	bool isActive;
	const wchar_t* prefix;
	const char* name;
};

namespace detail {

////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

#pragma pack(push, 1)

struct alignas(alignof(void*)) StructureStateInfo {
	enum class RegionType : Short {
		COMPOSITE,
		ORTHOGONAL,

		COUNT
	};

	StructureStateInfo() noexcept = default;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE StructureStateInfo(const Long parent_,
									const RegionType regionType_,
									const Short depth_,
									const char* const name_) noexcept
		: name{name_}
		, parent{parent_}
		, regionType{regionType_ }
		, depth{depth_}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	const char* name	  = nullptr;
	Long parent			  = INVALID_LONG;
	RegionType regionType = RegionType::COUNT;
	Short depth			  = INVALID_SHORT;
};

#pragma pack(pop)

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#ifdef HFSM2_ENABLE_PLANS

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct TaskBase {
	HFSM2_INLINE TaskBase() noexcept {}

	HFSM2_INLINE TaskBase(const StateID origin_,
						  const StateID destination_,
						  const TransitionType type_) noexcept
		: origin{origin_}
		, destination{destination_}
		, type{type_}
	{}

	static_assert(sizeof(Long) == sizeof(StateID), "");

	union {
		StateID origin		= INVALID_STATE_ID;
		Long prev;
	};

	union {
		StateID destination	= INVALID_STATE_ID;
		Long next;
	};

	TransitionType type = TransitionType::COUNT;
};

inline bool operator == (const TaskBase& lhs, const TaskBase& rhs) noexcept {
	return lhs.origin	   == rhs.origin &&
		   lhs.destination == rhs.destination &&
		   lhs.type		   == rhs.type;
}

//------------------------------------------------------------------------------

template <typename TPayload>
struct TaskT
	: TaskBase
{
	using Payload = TPayload;
	using Storage = typename std::aligned_storage<sizeof(Payload), 2>::type;

	using TaskBase::TaskBase;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE TaskT() noexcept {
		new (&storage) Payload{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE TaskT(const StateID origin,
					   const StateID destination,
					   const TransitionType type,
					   const Payload& payload) noexcept
		: TaskBase{origin, destination, type}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE TaskT(const StateID origin,
					   const StateID destination,
					   const TransitionType type,
					   Payload&& payload) noexcept
		: TaskBase{origin, destination, type}
		, payloadSet{true}
	{
		new (&storage) Payload{std::move(payload)};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	Storage storage;
	bool payloadSet = false;
};

//------------------------------------------------------------------------------

template <>
struct TaskT<void>
	: TaskBase
{
	using TaskBase::TaskBase;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

template <typename TPayload, Long NCapacity>
class TaskListT {
public:
	using Index = Long;

	static constexpr Index CAPACITY = NCapacity;

	static constexpr Index INVALID = Index (-1);

private:
	using Payload	= TPayload;
	using Item		= TaskT<Payload>;

public:
	template <typename... TArgs>
	Index emplace(TArgs&&... args)												  noexcept;

	void remove(const Index i)													  noexcept;

	HFSM2_INLINE	   Item& operator[] (const Index i)							  noexcept;
	HFSM2_INLINE const Item& operator[] (const Index i)						const noexcept;

	HFSM2_INLINE Index count()												const noexcept	{ return _count;	}

private:
	HFSM2_IF_ASSERT(void verifyStructure(const Index occupied = INVALID)	const noexcept);

private:
	Item _items[CAPACITY];
	Index _vacantHead = 0;
	Index _vacantTail = 0;
	Index _last  = 0;
	Index _count = 0;
};

//------------------------------------------------------------------------------

template <typename TItem>
class TaskListT<TItem, 0> {};

////////////////////////////////////////////////////////////////////////////////

}
}

#ifdef HFSM2_ENABLE_PLANS

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TP, Long NC>
template <typename... TA>
Long
TaskListT<TP, NC>::emplace(TA&&... args) noexcept {
	if (_count < CAPACITY) {
		HFSM2_ASSERT(_vacantHead < CAPACITY);
		HFSM2_ASSERT(_vacantTail < CAPACITY);

		const Index index = _vacantHead;
		auto& cell = _items[index];
		++_count;

		if (_vacantHead != _vacantTail) {
			// recycle
			HFSM2_ASSERT(cell.prev == INVALID);
			HFSM2_ASSERT(cell.next != INVALID);

			_vacantHead = cell.next;

			auto& head = _items[_vacantHead];
			HFSM2_ASSERT(head.prev == index);
			head.prev = INVALID;
		} else if (_last < CAPACITY - 1) {
			// grow
			++_last;
			_vacantHead = _last;
			_vacantTail = _last;

			auto& vacant = _items[_vacantHead];
			vacant.prev = INVALID;
			vacant.next = INVALID;
		} else {
			HFSM2_ASSERT(_count == CAPACITY);

			_vacantHead = INVALID;
			_vacantTail = INVALID;
		}

		HFSM2_IF_ASSERT(verifyStructure());

		new (&cell) Item{std::forward<TA>(args)...};

		return index;
	} else {
		// full
		HFSM2_ASSERT(_vacantHead == INVALID);
		HFSM2_ASSERT(_vacantTail == INVALID);
		HFSM2_ASSERT(_count		 == CAPACITY);
		HFSM2_BREAK();

		return INVALID;
	}
}

//------------------------------------------------------------------------------

template <typename TP, Long NC>
void
TaskListT<TP, NC>::remove(const Index i) noexcept {
	HFSM2_ASSERT(i < CAPACITY && _count);

	auto& fresh = _items[i];

	if (_count < CAPACITY) {
		HFSM2_ASSERT(_vacantHead < CAPACITY);
		HFSM2_ASSERT(_vacantTail < CAPACITY);

		fresh.prev = INVALID;
		fresh.next = _vacantHead;

		auto& head = _items[_vacantHead];
		head.prev = i;

		_vacantHead = i;
	} else {
		// 0 -> 1
		HFSM2_ASSERT(_count		 == CAPACITY);
		HFSM2_ASSERT(_vacantHead == INVALID);
		HFSM2_ASSERT(_vacantTail == INVALID);

		fresh.prev = INVALID;
		fresh.next = INVALID;

		_vacantHead = i;
		_vacantTail = i;
	}

	--_count;

	HFSM2_IF_ASSERT(verifyStructure());
}

//------------------------------------------------------------------------------

template <typename TP, Long NC>
typename TaskListT<TP, NC>::Item&
TaskListT<TP, NC>::operator[] (const Index i) noexcept {
	HFSM2_IF_ASSERT(verifyStructure());

	return _items[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TP, Long NC>
const typename TaskListT<TP, NC>::Item&
TaskListT<TP, NC>::operator[] (const Index i) const noexcept {
	HFSM2_IF_ASSERT(verifyStructure());

	return _items[i];
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_ASSERT

template <typename TP, Long NC>
void
TaskListT<TP, NC>::verifyStructure(const Index occupied) const noexcept {
	if (_count < CAPACITY) {
		HFSM2_ASSERT(_vacantHead < CAPACITY);
		HFSM2_ASSERT(_vacantTail < CAPACITY);

		HFSM2_ASSERT(_items[_vacantHead].prev == INVALID);
		HFSM2_ASSERT(_items[_vacantTail].next == INVALID);

		auto emptyCount = 1;

		for (auto c = _vacantHead; c != _vacantTail; ) {
			HFSM2_ASSERT(occupied != c);

			const auto& current = _items[c];

			const auto f = current.next;
			if (f != INVALID) {
				// next
				const auto& following = _items[f];

				HFSM2_ASSERT(following.prev == c);

				c = f;
				continue;
			} else {
				// end
				HFSM2_ASSERT(_vacantTail == c);
				HFSM2_ASSERT(_count		 == CAPACITY - emptyCount);

				break;
			}
		}
	} else {
		HFSM2_ASSERT(_vacantHead == INVALID);
		HFSM2_ASSERT(_vacantTail == INVALID);
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#endif
#ifdef HFSM2_ENABLE_PLANS

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 2)

struct TaskLink {
	Long prev		= INVALID_LONG;
	Long next		= INVALID_LONG;
};

//------------------------------------------------------------------------------

struct Bounds {
	Long first		= INVALID_LONG;
	Long last		= INVALID_LONG;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

template <typename
		, typename
		, typename
		, typename
		, Long
		, Long
		, Long
		HFSM2_IF_SERIALIZATION(, Long)
		, Long
		, Long
		, typename>
struct ArgsT;

template <typename>
struct PlanDataT;

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NCompoCount
		, Long NOrthoCount
		, Long NOrthoUnits
		HFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		, Long NTaskCapacity
		, typename TPayload>
struct PlanDataT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 , TRegionList
					 , NCompoCount
					 , NOrthoCount
					 , NOrthoUnits
					 HFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , TPayload>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;
	using Payload		= TPayload;

	static constexpr Short REGION_COUNT  = RegionList::SIZE;
	static constexpr Short TASK_CAPACITY = NTaskCapacity;

	using Task			= TaskT<Payload>;
	using Tasks			= TaskListT	  <Payload,  TASK_CAPACITY>;
	using TaskLinks		= StaticArrayT<TaskLink, TASK_CAPACITY>;
	using Payloads		= StaticArrayT<Payload,  TASK_CAPACITY>;

	using TasksBounds	= ArrayT   <Bounds,   RegionList::SIZE>;
	using TasksBits		= BitArrayT<StateID,  StateList::SIZE>;
	using RegionBits	= BitArrayT<RegionID, RegionList::SIZE>;

	Tasks tasks;
	TaskLinks taskLinks;
	Payloads taskPayloads;
	TasksBits payloadExists;

	TasksBounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	RegionBits planExists;

	void clearTaskStatus  (const StateID stateId)		  noexcept;
	void verifyEmptyStatus(const StateID stateId)	const noexcept;

#ifdef HFSM2_ENABLE_ASSERT
	void verifyPlans() const noexcept;
	Long verifyPlan(const RegionID stateId)			const noexcept;
#endif
};

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NCompoCount
		, Long NOrthoCount
		, Long NOrthoUnits
		HFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		, Long NTaskCapacity>
struct PlanDataT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 , TRegionList
					 , NCompoCount
					 , NOrthoCount
					 , NOrthoUnits
					 HFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , void>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;

	static constexpr Short REGION_COUNT  = RegionList::SIZE;
	static constexpr Short TASK_CAPACITY = NTaskCapacity;

	using Task			= TaskT<void>;
	using Tasks			= TaskListT<void, TASK_CAPACITY>;
	using TaskLinks		= StaticArrayT<TaskLink, TASK_CAPACITY>;

	using TasksBounds	= ArrayT   <Bounds,   RegionList::SIZE>;
	using TasksBits		= BitArrayT<StateID,  StateList::SIZE>;
	using RegionBits	= BitArrayT<RegionID, RegionList::SIZE>;

	Tasks tasks;
	TaskLinks taskLinks;

	TasksBounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	RegionBits planExists;

	void clearTaskStatus  (const StateID stateId)		  noexcept;
	void verifyEmptyStatus(const StateID stateId)	const noexcept;

#ifdef HFSM2_ENABLE_ASSERT
	void verifyPlans()								const noexcept;
	Long verifyPlan(const RegionID stateId)			const noexcept;
#endif
};

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NOrthoCount
		, Long NOrthoUnits
		, Long NSubstitutionLimit
		, Long NTaskCapacity
		, typename TPayload>
struct PlanDataT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 , TRegionList
					 , 0
					 , NOrthoCount
					 , NOrthoUnits
					 HFSM2_IF_SERIALIZATION(, 0)
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , TPayload>>
{
	void clearTaskStatus  (const StateID)				  noexcept {}
	void verifyEmptyStatus(const StateID)			const noexcept {}

#ifdef HFSM2_ENABLE_ASSERT
	void verifyPlans()								const noexcept {}
#endif
};

////////////////////////////////////////////////////////////////////////////////

}
}

#ifdef HFSM2_ENABLE_PLANS

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::verifyEmptyStatus(const StateID HFSM2_IF_ASSERT(stateId)) const noexcept {
#ifdef HFSM2_ENABLE_ASSERT

	if (stateId != INVALID_STATE_ID) {
		HFSM2_ASSERT(!tasksSuccesses.get(stateId));
		HFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_ASSERT

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::verifyPlans() const noexcept {
	Long planCount = 0;

	for (RegionID regionId = 0; regionId < REGION_COUNT; ++regionId)
		planCount += verifyPlan(regionId);

	HFSM2_ASSERT(tasks.count() == planCount);
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
Long
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::verifyPlan(const RegionID regionId) const noexcept {
	Long length = 0;
	const Bounds& bounds = tasksBounds[regionId];

	if (bounds.first != INVALID_LONG) {
		HFSM2_ASSERT(bounds.last != INVALID_LONG);

		for (auto slow = bounds.first, fast = slow; ; ) {
			++length;
			const TaskLink& task = taskLinks[slow];

			if (slow != bounds.last) {
				HFSM2_ASSERT(task.next != INVALID_LONG);
				slow = task.next;

				// loop check
				if (fast != INVALID_LONG) {
					fast = taskLinks[fast].next;

					if (fast != INVALID_LONG)
						fast = taskLinks[fast].next;

					HFSM2_ASSERT(fast == INVALID_LONG || slow != fast);
				}
			} else {
				HFSM2_ASSERT(task.next == INVALID_LONG);

				break;
			}
		};
	} else
		HFSM2_ASSERT(bounds.last == INVALID_LONG);

	return length;
}

#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::verifyEmptyStatus(const StateID HFSM2_IF_ASSERT(stateId)) const noexcept {
#ifdef HFSM2_ENABLE_ASSERT

	if (stateId != INVALID_STATE_ID) {
		HFSM2_ASSERT(!tasksSuccesses.get(stateId));
		HFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_ASSERT

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::verifyPlans() const noexcept {
	Long planCount = 0;

	for (RegionID regionId = 0; regionId < REGION_COUNT; ++regionId)
		planCount += verifyPlan(regionId);

	HFSM2_ASSERT(tasks.count() == planCount);
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
Long
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::verifyPlan(const RegionID regionId) const noexcept {
	Long length = 0;
	const Bounds& bounds = tasksBounds[regionId];

	if (bounds.first != INVALID_LONG) {
		HFSM2_ASSERT(bounds.last != INVALID_LONG);

		for (auto slow = bounds.first, fast = slow; ; ) {
			++length;
			const TaskLink& task = taskLinks[slow];

			if (slow != bounds.last) {
				HFSM2_ASSERT(task.next != INVALID_LONG);
				slow = task.next;

				// loop check
				if (fast != INVALID_LONG) {
					fast = taskLinks[fast].next;

					if (fast != INVALID_LONG)
						fast = taskLinks[fast].next;

					HFSM2_ASSERT(fast == INVALID_LONG || slow != fast);
				}
			} else {
				HFSM2_ASSERT(task.next == INVALID_LONG);

				break;
			}
		};
	} else
		HFSM2_ASSERT(bounds.last == INVALID_LONG);

	return length;
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#endif
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct Status {
	enum class Result {
		NONE,
		SUCCESS,
		FAILURE
	};

	Result result = Result::NONE;
	bool outerTransition = false;

	inline Status(const Result result_ = Result::NONE,
				  const bool outerTransition_ = false) noexcept;

	inline explicit operator bool() const noexcept			{ return result != Result::NONE || outerTransition;	}

	inline void clear() noexcept;
};

#pragma pack(pop)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Status
combine(const Status lhs, const Status rhs) noexcept {
	const Status::Result result = lhs.result > rhs.result ?
		lhs.result : rhs.result;

	return {result, lhs.outerTransition || rhs.outerTransition};
}

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_PLANS

template <typename TArgs>
class CPlanT {
	template <typename>
	friend class ControlT;

	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	static constexpr Long TASK_CAPACITY = Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using Task			= typename PlanData::Task;
	using TaskLinks		= typename PlanData::TaskLinks;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct IteratorT {
		HFSM2_INLINE IteratorT(const CPlanT& plan)		  noexcept;

		HFSM2_INLINE explicit operator bool()		const noexcept;

		HFSM2_INLINE void operator ++()					  noexcept;

		HFSM2_INLINE const Task& operator  *()		const noexcept	{ return  _plan._planData.tasks[_curr];		}
		HFSM2_INLINE const Task* operator ->()		const noexcept	{ return &_plan._planData.tasks[_curr];		}

		HFSM2_INLINE Long next()					const noexcept;

		const CPlanT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	HFSM2_INLINE CPlanT(const PlanData& planData,
						const RegionID regionId)		  noexcept;

	template <typename T>
	static constexpr StateID  stateId()					  noexcept	{ return			index<StateList , T>();	}

	template <typename T>
	static constexpr RegionID regionId()				  noexcept	{ return (RegionID) index<RegionList, T>();	}

public:
	HFSM2_INLINE explicit operator bool()			const noexcept;

	HFSM2_INLINE IteratorT first()						  noexcept	{ return IteratorT{*this};					}

private:
	const PlanData& _planData;
	const Bounds& _bounds;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class PlanBaseT {
	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	static constexpr Long  TASK_CAPACITY	= Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using Task			= typename PlanData::Task;
	using Tasks			= typename PlanData::Tasks;
	using TaskLinks		= typename PlanData::TaskLinks;
	using TaskIndex		= typename TaskLinks::Index;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct IteratorT {
		HFSM2_INLINE IteratorT(PlanBaseT& plan)			  noexcept;

		HFSM2_INLINE explicit operator bool()		const noexcept;

		HFSM2_INLINE void operator ++()					  noexcept;

		HFSM2_INLINE	   Task& operator  *()			  noexcept	{ return  _plan._planData.tasks[_curr];		}
		HFSM2_INLINE const Task& operator  *()		const noexcept	{ return  _plan._planData.tasks[_curr];		}

		HFSM2_INLINE	   Task* operator ->()			  noexcept	{ return &_plan._planData.tasks[_curr];		}
		HFSM2_INLINE const Task* operator ->()		const noexcept	{ return &_plan._planData.tasks[_curr];		}

		HFSM2_INLINE void remove()						  noexcept;

		HFSM2_INLINE Long next()					const noexcept;

		PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct CIterator {
		HFSM2_INLINE CIterator(const PlanBaseT& plan)	  noexcept;

		HFSM2_INLINE explicit operator bool()		const noexcept;

		HFSM2_INLINE void operator ++()					  noexcept;

		HFSM2_INLINE	   Task& operator  *()			  noexcept	{ return  _plan._planData.tasks[_curr];		}
		HFSM2_INLINE const Task& operator  *()		const noexcept	{ return  _plan._planData.tasks[_curr];		}

		HFSM2_INLINE	   Task* operator ->()			  noexcept	{ return &_plan._planData.tasks[_curr];		}
		HFSM2_INLINE const Task* operator ->()		const noexcept	{ return &_plan._planData.tasks[_curr];		}

		HFSM2_INLINE Long next()					const noexcept;

		const PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	HFSM2_INLINE PlanBaseT(PlanData& planData,
						   const RegionID regionId)		  noexcept;

	template <typename T>
	static constexpr StateID  stateId()					  noexcept	{ return			index<StateList , T>();	}

	template <typename T>
	static constexpr RegionID regionId()				  noexcept	{ return (RegionID) index<RegionList, T>();	}

	bool append(const StateID origin,
				const StateID destination,
				const TransitionType transitionType)	  noexcept;

	bool linkTask(const Long index)						  noexcept;

public:
	HFSM2_INLINE explicit operator bool()			const noexcept;

	/// @brief Clear all tasks from the plan
	HFSM2_INLINE void clear()							  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, acts depending on the region type)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_INLINE bool change   (const StateID origin, const StateID destination) noexcept { return append(origin, destination, TransitionType::CHANGE);		}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, acts depending on the region type)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_INLINE bool change   (const StateID destination)						 noexcept { return change   (stateId<TOrigin>(), destination);				}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, acts depending on the region type)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool change   ()												 noexcept { return change   (stateId<TOrigin>(), stateId<TDestination>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the initial state)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_INLINE bool restart  (const StateID origin, const StateID destination) noexcept { return append(origin, destination, TransitionType::RESTART);	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the initial state)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_INLINE bool restart  (const StateID destination)						 noexcept { return restart  (stateId<TOrigin>(), destination);				}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the initial state)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool restart  ()												 noexcept { return restart  (stateId<TOrigin>(), stateId<TDestination>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state that was active previously)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_INLINE bool resume   (const StateID origin, const StateID destination) noexcept { return append(origin, destination, TransitionType::RESUME);		}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state that was active previously)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_INLINE bool resume   (const StateID destination)						 noexcept { return resume   (stateId<TOrigin>(), destination);				}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state that was active previously)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool resume   ()												 noexcept { return resume   (stateId<TOrigin>(), stateId<TDestination>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state with the highest 'utility()' among those with the highest 'rank()')
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE bool utilize  (const StateID origin, const StateID destination) noexcept { return append(origin, destination, TransitionType::UTILIZE);	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin>
	HFSM2_INLINE bool utilize  (const StateID destination)						 noexcept { return utilize  (stateId<TOrigin>(), destination);				}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool utilize  ()												 noexcept { return utilize  (stateId<TOrigin>(), stateId<TDestination>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, uses weighted random to activate the state proportional to 'utility()' among those with the highest 'rank()')
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE bool randomize(const StateID origin, const StateID destination)  noexcept { return append(origin, destination, TransitionType::RANDOMIZE);	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, uses weighted random to activate the state proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin>
	HFSM2_INLINE bool randomize(const StateID destination)						  noexcept { return randomize(stateId<TOrigin>(), destination);				}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, uses weighted random to activate the state proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool randomize()												  noexcept { return randomize(stateId<TOrigin>(), stateId<TDestination>());	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_INLINE bool schedule (const StateID origin, const StateID destination)  noexcept { return append(origin, destination, TransitionType::SCHEDULE);	}

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_INLINE bool schedule (const StateID destination)						  noexcept { return schedule (stateId<TOrigin>(), destination);				}

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool schedule ()												  noexcept { return schedule (stateId<TOrigin>(), stateId<TDestination>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Begin iteration over plan tasks for the current region
	/// @return IteratorT to the first task
	HFSM2_INLINE  IteratorT first()												  noexcept { return  IteratorT{*this};										}

	/// @brief Begin iteration over plan tasks
	/// @return CIterator to the first task
	HFSM2_INLINE CIterator first()											const noexcept { return CIterator{*this};										}

private:
	void remove(const Long task) noexcept;

protected:
	PlanData& _planData;
	const RegionID _regionId;
	Bounds& _bounds;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class PlanT;

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NCompoCount
		, Long NOrthoCount
		, Long NOrthoUnits
		HFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		, Long NTaskCapacity
		, typename TPayload>
class PlanT<ArgsT<TContext
				, TConfig
				, TStateList
				, TRegionList
				, NCompoCount
				, NOrthoCount
				, NOrthoUnits
				HFSM2_IF_SERIALIZATION(, NSerialBits)
				, NSubstitutionLimit
				, NTaskCapacity
				, TPayload>> final
	: public PlanBaseT<ArgsT<TContext
						   , TConfig
						   , TStateList
						   , TRegionList
						   , NCompoCount
						   , NOrthoCount
						   , NOrthoUnits
						   HFSM2_IF_SERIALIZATION(, NSerialBits)
						   , NSubstitutionLimit
						   , NTaskCapacity
						   , TPayload>>
{
	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	using Args = ArgsT<TContext
					 , TConfig
					 , TStateList
					 , TRegionList
					 , NCompoCount
					 , NOrthoCount
					 , NOrthoUnits
					 HFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , TPayload>;

	using Payload		= typename Args::Payload;

	using PlanBase		= PlanBaseT<Args>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using PlanBase::PlanBase;

	using PlanBase::linkTask;

	bool append(const StateID origin,
				const StateID destination,
				const TransitionType transitionType,
				const Payload& payload) noexcept;

	bool append(const StateID origin,
				const StateID destination,
				const TransitionType transitionType,
				Payload&& payload) noexcept;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, acts depending on the region type)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_INLINE bool changeWith   (const StateID origin, const StateID destination, const Payload& payload) noexcept { return append(origin							   , destination							   , TransitionType::CHANGE   ,			  payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, acts depending on the region type)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_INLINE bool changeWith   (const StateID origin, const StateID destination,	  Payload&& payload) noexcept { return append(origin							   , destination							   , TransitionType::CHANGE   , std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, acts depending on the region type)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_INLINE bool changeWith   (					  const StateID destination, const Payload& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), destination							   , TransitionType::CHANGE   ,			  payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, acts depending on the region type)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_INLINE bool changeWith   (					  const StateID destination,	  Payload&& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), destination							   , TransitionType::CHANGE   , std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, acts depending on the region type)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool changeWith   (												 const Payload& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::CHANGE   ,			  payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, acts depending on the region type)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool changeWith   (													  Payload&& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::CHANGE   , std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the initial state)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_INLINE bool restartWith  (const StateID origin, const StateID destination, const Payload& payload) noexcept { return append(origin							   , destination							   , TransitionType::RESTART  ,			  payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the initial state)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_INLINE bool restartWith  (const StateID origin, const StateID destination,	  Payload&& payload) noexcept { return append(origin							   , destination							   , TransitionType::RESTART  , std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the initial state)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_INLINE bool restartWith  (					  const StateID destination, const Payload& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), destination							   , TransitionType::RESTART  ,			  payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the initial state)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_INLINE bool restartWith  (					  const StateID destination,	  Payload&& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), destination							   , TransitionType::RESTART  , std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the initial state)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool restartWith  (												 const Payload& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::RESTART  ,			  payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the initial state)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool restartWith  (													  Payload&& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::RESTART  , std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state that was active previously)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_INLINE bool resumeWith   (const StateID origin, const StateID destination, const Payload& payload) noexcept { return append(origin							   , destination							   , TransitionType::RESUME   ,			  payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state that was active previously)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_INLINE bool resumeWith   (const StateID origin, const StateID destination,	  Payload&& payload) noexcept { return append(origin							   , destination							   , TransitionType::RESUME   , std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state that was active previously)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_INLINE bool resumeWith   (					  const StateID destination, const Payload& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), destination							   , TransitionType::RESUME   ,			  payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state that was active previously)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_INLINE bool resumeWith   (					  const StateID destination,	  Payload&& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), destination							   , TransitionType::RESUME   , std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state that was active previously)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool resumeWith   (												 const Payload& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::RESUME   ,			  payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state that was active previously)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool resumeWith   (													  Payload&& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::RESUME   , std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state with the highest 'utility()' among those with the highest 'rank()')
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE bool utilizeWith  (const StateID origin, const StateID destination, const Payload& payload) noexcept { return append(origin							   , destination							   , TransitionType::UTILIZE  ,			  payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state with the highest 'utility()' among those with the highest 'rank()')
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE bool utilizeWith  (const StateID origin, const StateID destination,	  Payload&& payload) noexcept { return append(origin							   , destination							   , TransitionType::UTILIZE  , std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin>
	HFSM2_INLINE bool utilizeWith  (					  const StateID destination, const Payload& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), destination							   , TransitionType::UTILIZE  ,			  payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin>
	HFSM2_INLINE bool utilizeWith  (					  const StateID destination,	  Payload&& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), destination							   , TransitionType::UTILIZE  , std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool utilizeWith  (												 const Payload& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::UTILIZE  ,			  payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool utilizeWith  (													  Payload&& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::UTILIZE  , std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, uses weighted random to activate the state proportional to 'utility()' among those with the highest 'rank()')
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE bool randomizeWith(const StateID origin, const StateID destination, const Payload& payload) noexcept { return append(origin							   , destination							   , TransitionType::RANDOMIZE,			  payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, uses weighted random to activate the state proportional to 'utility()' among those with the highest 'rank()')
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE bool randomizeWith(const StateID origin, const StateID destination,	  Payload&& payload) noexcept { return append(origin							   , destination							   , TransitionType::RANDOMIZE, std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, uses weighted random to activate the state proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin>
	HFSM2_INLINE bool randomizeWith(					  const StateID destination, const Payload& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), destination							   , TransitionType::RANDOMIZE,			  payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, uses weighted random to activate the state proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin>
	HFSM2_INLINE bool randomizeWith(					  const StateID destination,	  Payload&& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), destination							   , TransitionType::RANDOMIZE, std::move(payload));	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, uses weighted random to activate the state proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool randomizeWith(												 const Payload& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::RANDOMIZE,			  payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, uses weighted random to activate the state proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool randomizeWith(													  Payload&& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::RANDOMIZE, std::move(payload));	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_INLINE bool scheduleWith (const StateID origin, const StateID destination, const Payload& payload) noexcept { return append(origin							   , destination							   , TransitionType::SCHEDULE ,			  payload );	}

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_INLINE bool scheduleWith (const StateID origin, const StateID destination,	  Payload&& payload) noexcept { return append(origin							   , destination							   , TransitionType::SCHEDULE , std::move(payload));	}

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_INLINE bool scheduleWith (					  const StateID destination, const Payload& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), destination							   , TransitionType::SCHEDULE ,			  payload );	}

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_INLINE bool scheduleWith (					  const StateID destination,	  Payload&& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), destination							   , TransitionType::SCHEDULE , std::move(payload));	}

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool scheduleWith (												 const Payload& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::SCHEDULE ,			  payload );	}

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool scheduleWith (													  Payload&& payload) noexcept { return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::SCHEDULE , std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	using PlanBase::_planData;
	using PlanBase::_regionId;
};

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NCompoCount
		, Long NOrthoCount
		, Long NOrthoUnits
		HFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		, Long NTaskCapacity>
class PlanT<ArgsT<TContext
				, TConfig
				, TStateList
				, TRegionList
				, NCompoCount
				, NOrthoCount
				, NOrthoUnits
				HFSM2_IF_SERIALIZATION(, NSerialBits)
				, NSubstitutionLimit
				, NTaskCapacity
				, void>> final
	: public PlanBaseT<ArgsT<TContext
						   , TConfig
						   , TStateList
						   , TRegionList
						   , NCompoCount
						   , NOrthoCount
						   , NOrthoUnits
						   HFSM2_IF_SERIALIZATION(, NSerialBits)
						   , NSubstitutionLimit
						   , NTaskCapacity
						   , void>>
{
	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	using Args = ArgsT<TContext
					 , TConfig
					 , TStateList
					 , TRegionList
					 , NCompoCount
					 , NOrthoCount
					 , NOrthoUnits
					 HFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , void>;

	using PlanBase = PlanBaseT<Args>;

	using PlanBase::PlanBase;
};

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

Status::Status(const Result result_,
			   const bool outerTransition_) noexcept
	: result{result_}
	, outerTransition{outerTransition_}
{}

//------------------------------------------------------------------------------

void
Status::clear() noexcept {
	result = Result::NONE;
	outerTransition = false;
}

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_PLANS

template <typename TArgs>
CPlanT<TArgs>::IteratorT::IteratorT(const CPlanT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
CPlanT<TArgs>::IteratorT::operator bool() const noexcept {
	HFSM2_ASSERT(_curr < CPlanT::TASK_CAPACITY || _curr == INVALID_LONG);

	return _curr < CPlanT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
CPlanT<TArgs>::IteratorT::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
Long
CPlanT<TArgs>::IteratorT::next() const noexcept {
	if (_curr < CPlanT::TASK_CAPACITY) {
		const TaskLink& link = _plan._planData.taskLinks[_curr];

		return link.next;
	} else {
		HFSM2_ASSERT(_curr == INVALID_LONG);

		return INVALID_LONG;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
CPlanT<TArgs>::CPlanT(const PlanData& planData,
					  const RegionID regionId) noexcept
	: _planData{planData}
	, _bounds{planData.tasksBounds[regionId]}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
CPlanT<TArgs>::operator bool() const noexcept {
	if (_bounds.first < TASK_CAPACITY) {
		HFSM2_ASSERT(_bounds.last < TASK_CAPACITY);
		return true;
	} else {
		HFSM2_ASSERT(_bounds.last == INVALID_LONG);
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
PlanBaseT<TArgs>::IteratorT::IteratorT(PlanBaseT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
PlanBaseT<TArgs>::IteratorT::operator bool() const noexcept {
	HFSM2_ASSERT(_curr < PlanBaseT::TASK_CAPACITY || _curr == INVALID_LONG);

	return _curr < PlanBaseT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanBaseT<TArgs>::IteratorT::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanBaseT<TArgs>::IteratorT::remove() noexcept {
	_plan.remove(_curr);
}

//------------------------------------------------------------------------------

template <typename TArgs>
Long
PlanBaseT<TArgs>::IteratorT::next() const noexcept {
	if (_curr < PlanBaseT::TASK_CAPACITY) {
		const TaskLink& link = _plan._planData.taskLinks[_curr];

		return link.next;
	} else {
		HFSM2_ASSERT(_curr == INVALID_LONG);

		return INVALID_LONG;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
PlanBaseT<TArgs>::CIterator::CIterator(const PlanBaseT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
PlanBaseT<TArgs>::CIterator::operator bool() const noexcept {
	HFSM2_ASSERT(_curr < PlanBaseT::TASK_CAPACITY || _curr == INVALID_LONG);

	return _curr < PlanBaseT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanBaseT<TArgs>::CIterator::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
Long
PlanBaseT<TArgs>::CIterator::next() const noexcept {
	if (_curr < PlanBaseT::TASK_CAPACITY) {
		const TaskLink& link = _plan._planData.taskLinks[_curr];

		return link.next;
	} else {
		HFSM2_ASSERT(_curr == INVALID_LONG);

		return INVALID_LONG;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
PlanBaseT<TArgs>::PlanBaseT(PlanData& planData,
							const RegionID regionId) noexcept
	: _planData{planData}
	, _regionId{regionId}
	, _bounds{planData.tasksBounds[regionId]}
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
PlanBaseT<TArgs>::operator bool() const noexcept {
	if (_bounds.first < TASK_CAPACITY) {
		HFSM2_ASSERT(_bounds.last < TASK_CAPACITY);
		return true;
	} else {
		HFSM2_ASSERT(_bounds.last == INVALID_LONG);
		return false;
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
bool
PlanBaseT<TArgs>::append(const StateID origin,
						 const StateID destination,
						 const TransitionType transitionType) noexcept
{
	_planData.planExists.set(_regionId);

	return linkTask(_planData.tasks.emplace(origin, destination, transitionType));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
bool
PlanBaseT<TArgs>::linkTask(const Long index) noexcept {
	if (index != Tasks::INVALID) {
		if (_bounds.first == INVALID_LONG) {
			HFSM2_ASSERT(_bounds.last == INVALID_LONG);
			HFSM2_ASSERT(_planData.taskLinks[index].prev == INVALID_LONG);
			HFSM2_ASSERT(_planData.taskLinks[index].next == INVALID_LONG);

			_bounds.first = index;
			_bounds.last  = index;
		} else {
			HFSM2_ASSERT(_bounds.first < TaskLinks::CAPACITY);
			HFSM2_ASSERT(_bounds.last  < TaskLinks::CAPACITY);

			auto& lastLink = _planData.taskLinks[_bounds.last];
			HFSM2_ASSERT(lastLink.next == INVALID_LONG);

			lastLink.next  = index;

			auto& currLink = _planData.taskLinks[index];
			HFSM2_ASSERT(lastLink.prev == INVALID_LONG);

			currLink.prev  = _bounds.last;

			_bounds.last   = index;
		}

		return true;
	} else
		return false;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanBaseT<TArgs>::clear() noexcept {
	if (_bounds.first < TaskLinks::CAPACITY) {
		HFSM2_ASSERT(_bounds.last < TaskLinks::CAPACITY);

		for (Long index = _bounds.first;
			 index != INVALID_LONG;
			 )
		{
			HFSM2_ASSERT(index < TaskLinks::CAPACITY);

			const auto& taskLink = _planData.taskLinks[index];
			HFSM2_ASSERT(index == _bounds.first ?
							 taskLink.prev == INVALID_LONG :
							 taskLink.prev <  TaskLinks::CAPACITY);

			const Long next = taskLink.next;

			_planData.tasks.remove(index);

			index = next;
		}

		_bounds.first = INVALID_LONG;
		_bounds.last  = INVALID_LONG;
	} else {
		HFSM2_ASSERT(_bounds.first == INVALID_LONG);
		HFSM2_ASSERT(_bounds.last  == INVALID_LONG);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanBaseT<TArgs>::remove(const Long task) noexcept {
	HFSM2_ASSERT(_planData.planExists.get(_regionId));
	HFSM2_ASSERT(_bounds.first < TaskLinks::CAPACITY);
	HFSM2_ASSERT(_bounds.last  < TaskLinks::CAPACITY);

	HFSM2_ASSERT(task < TaskLinks::CAPACITY);

	TaskLink& curr = _planData.taskLinks[task];

	if (curr.prev < TaskLinks::CAPACITY) {
		TaskLink& prev = _planData.taskLinks[curr.prev];
		prev.next = curr.next;
	} else {
		HFSM2_ASSERT(_bounds.first == task);
		_bounds.first = curr.next;
	}

	if (curr.next < TaskLinks::CAPACITY) {
		TaskLink& next = _planData.taskLinks[curr.next];
		next.prev = curr.prev;
	} else {
		HFSM2_ASSERT(_bounds.last == task);
		_bounds.last = curr.prev;
	}

	curr.prev = INVALID_LONG;
	curr.next = INVALID_LONG;

	_planData.tasks.remove(task);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
bool
PlanT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::append(const StateID origin,
																								   const StateID destination,
																								   const TransitionType transitionType,
																								   const Payload& payload) noexcept
{
	_planData.planExists.set(_regionId);

	return linkTask(_planData.tasks.emplace(origin, destination, transitionType, payload));
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
bool
PlanT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::append(const StateID origin,
																								   const StateID destination,
																								   const TransitionType transitionType,
																								   Payload&& payload) noexcept
{
	_planData.planExists.set(_regionId);

	return linkTask(_planData.tasks.emplace(origin, destination, transitionType, std::move(payload)));
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

enum Strategy {
	Composite,
	Resumable,

//#ifdef HFSM2_ENABLE_UTILITY_THEORY
	Utilitarian,
	RandomUtil,
//#endif
};

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct alignas(2 * sizeof(Short)) Parent {
	HFSM2_INLINE Parent() = default;

	HFSM2_INLINE Parent(const ForkID forkId_) noexcept
		: forkId{forkId_}
	{}

	HFSM2_INLINE Parent(const ForkID forkId_,
						const Short prong_) noexcept
		: forkId{forkId_}
		, prong{prong_}
	{}

	HFSM2_INLINE explicit operator bool() const noexcept {
		return forkId != INVALID_FORK_ID &&
			   prong  != INVALID_SHORT;
	}

	ForkID forkId = INVALID_FORK_ID;
	Short  prong  = INVALID_SHORT;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

template <typename TRegistry>
struct BackUpT {
	using CompoForks = typename TRegistry::CompoForks;
	using OrthoForks = typename TRegistry::OrthoForks;

	CompoForks compoRequested;
	OrthoForks orthoRequested;
	CompoForks compoResumable;
};

////////////////////////////////////////////////////////////////////////////////

template <typename
		, typename
		, typename
		, typename
		, Long
		, Long
		, Long
		HFSM2_IF_SERIALIZATION(, Long)
		, Long
		HFSM2_IF_PLANS(, Long)
		, typename>
struct ArgsT;

template <typename>
struct RegistryT;

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NCompoCount
		, Long NOrthoCount
		, Long NOrthoUnits
		HFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct RegistryT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 , TRegionList
					 , NCompoCount
					 , NOrthoCount
					 , NOrthoUnits
					 HFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 HFSM2_IF_PLANS(, NTaskCapacity)
					 , TPayload>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;

	static constexpr Long  STATE_COUNT		= StateList::SIZE;
	static constexpr Short COMPO_REGIONS	= NCompoCount;
	static constexpr Short ORTHO_REGIONS	= NOrthoCount;
	static constexpr Short ORTHO_UNITS		= NOrthoUnits;

	using Payload		= TPayload;
	using Transition	= TransitionT<Payload>;

	using StateParents	= StaticArrayT<Parent, STATE_COUNT>;

	using CompoParents	= StaticArrayT<Parent, COMPO_REGIONS>;
	using OrthoParents	= StaticArrayT<Parent, ORTHO_REGIONS>;
	using OrthoUnits	= StaticArrayT<Units,  ORTHO_UNITS>;

	using CompoForks	= StaticArrayT<Short,  COMPO_REGIONS>;
	using OrthoForks	= BitArrayT	  <Short,  ORTHO_UNITS>;
	using OrthoBits		= typename OrthoForks::Bits;
	using CompoRemains	= BitArrayT	  <Short,  COMPO_REGIONS>;

	using BackUp		= BackUpT<RegistryT>;

	bool isActive		 (const StateID stateId) const noexcept;
	bool isResumable	 (const StateID stateId) const noexcept;

	bool isPendingChange (const StateID stateId) const noexcept;
	bool isPendingEnter	 (const StateID stateId) const noexcept;
	bool isPendingExit	 (const StateID stateId) const noexcept;

	HFSM2_INLINE const Parent&	   forkParent(const ForkID forkId) const noexcept;

	HFSM2_INLINE OrthoBits requestedOrthoFork(const ForkID forkId) noexcept;

	bool requestImmediate(const Transition& request) noexcept;
	void requestScheduled(const StateID stateId) noexcept;

	void clearRequests() noexcept;
	void clear() noexcept;

	StateParents stateParents;
	CompoParents compoParents;
	OrthoParents orthoParents;
	OrthoUnits orthoUnits;

	CompoForks compoRequested{INVALID_SHORT};
	OrthoForks orthoRequested;
	CompoForks compoActive	 {INVALID_SHORT};
	CompoForks compoResumable{INVALID_SHORT};

	CompoRemains compoRemains;
};

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NCompoCount
		HFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct RegistryT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 , TRegionList
					 , NCompoCount
					 , 0
					 , 0
					 HFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 HFSM2_IF_PLANS(, NTaskCapacity)
					 , TPayload>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;

	static constexpr Long  STATE_COUNT		= StateList::SIZE;
	static constexpr Short COMPO_REGIONS	= NCompoCount;

	using Payload		= TPayload;
	using Transition	= TransitionT<Payload>;

	using StateParents	= StaticArrayT<Parent, STATE_COUNT>;
	using CompoParents	= StaticArrayT<Parent, COMPO_REGIONS>;

	using CompoForks	= StaticArrayT<Short,  COMPO_REGIONS>;
	using OrthoForks	= BitArrayT	  <Short,  0>;
	using CompoRemains	= BitArrayT	  <Short,  COMPO_REGIONS>;

	using BackUp		= BackUpT<RegistryT>;

	bool isActive		 (const StateID stateId) const noexcept;
	bool isResumable	 (const StateID stateId) const noexcept;

	bool isPendingChange (const StateID stateId) const noexcept;
	bool isPendingEnter	 (const StateID stateId) const noexcept;
	bool isPendingExit	 (const StateID stateId) const noexcept;

	HFSM2_INLINE const Parent& forkParent(const ForkID forkId) const noexcept;

	bool requestImmediate(const Transition& request) noexcept;
	void requestScheduled(const StateID stateId) noexcept;

	void clearRequests() noexcept;
	void clear() noexcept;

	StateParents stateParents;
	CompoParents compoParents;

	CompoForks compoRequested{INVALID_SHORT};
	OrthoForks orthoRequested;
	CompoForks compoActive	 {INVALID_SHORT};
	CompoForks compoResumable{INVALID_SHORT};

	CompoRemains compoRemains;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TRegistry>
void
backup(const TRegistry& registry,
	   BackUpT<TRegistry>& copy) noexcept
{
	overwrite(copy.compoRequested, registry.compoRequested);
	overwrite(copy.orthoRequested, registry.orthoRequested);
	overwrite(copy.compoResumable, registry.compoResumable);
}

//------------------------------------------------------------------------------

template <typename TRegistry>
void
restore(TRegistry& registry,
		const BackUpT<TRegistry>& copy) noexcept
{
	overwrite(registry.compoRequested, copy.compoRequested);
	overwrite(registry.orthoRequested, copy.orthoRequested);
	overwrite(registry.compoResumable, copy.compoResumable);
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isActive(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM2_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong == compoActive[parent.forkId - 1];
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isResumable(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM2_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong == compoResumable[parent.forkId - 1];
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isPendingChange(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM2_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return compoRequested[parent.forkId - 1] !=
					   compoActive	 [parent.forkId - 1];
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isPendingEnter(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM2_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong != compoActive	 [parent.forkId - 1] &&
					   parent.prong == compoRequested[parent.forkId - 1];
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isPendingExit(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM2_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong == compoActive	 [parent.forkId - 1] &&
					   parent.prong != compoRequested[parent.forkId - 1];
		}

	return true;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
const Parent&
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::forkParent(const ForkID forkId) const noexcept {
	HFSM2_ASSERT(forkId != 0);

	return forkId > 0 ?
		compoParents[ forkId - 1] :
		orthoParents[-forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
typename RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::OrthoBits
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::requestedOrthoFork(const ForkID forkId) noexcept {
	HFSM2_ASSERT(forkId < 0);
	const Units& units = orthoUnits[-forkId - 1];

	return orthoRequested.bits(units);
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::requestImmediate(const Transition& request) noexcept {
	if (request.destination == 0)
		return false;
	else if (HFSM2_CHECKED(request.destination < STATE_COUNT)) {
		Parent parent;

		for (parent = stateParents[request.destination];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			if (parent.forkId > 0) {
				compoRequested[parent.forkId - 1] = parent.prong;
				parent = forkParent(parent.forkId);

				break;
			} else if (parent.forkId < 0)
				requestedOrthoFork(parent.forkId).set(parent.prong);
			else
				HFSM2_BREAK();
		}

		for (; parent; parent = forkParent(parent.forkId)) {
			if (parent.forkId > 0) {
				compoRemains.set(parent.forkId - 1);

				if (compoActive	  [parent.forkId - 1] != parent.prong)
					compoRequested[parent.forkId - 1]  = parent.prong;
				else {
					parent = forkParent(parent.forkId);
					break;
				}
			} else if (parent.forkId < 0)
				requestedOrthoFork(parent.forkId).set(parent.prong);
			else
				HFSM2_BREAK();
		}

		for (; parent; parent = forkParent(parent.forkId)) {
			if (parent.forkId > 0)
				compoRemains.set(parent.forkId - 1);
			else if (parent.forkId < 0)
				requestedOrthoFork(parent.forkId).set(parent.prong);
			else
				HFSM2_BREAK();
		}
	}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::requestScheduled(const StateID stateId) noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT)) {
		const Parent parent = stateParents[stateId];

		HFSM2_ASSERT(parent.forkId != 0);
		if (parent.forkId > 0)
			compoResumable[parent.forkId - 1] = parent.prong;
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::clearRequests() noexcept {
	compoRequested.clear();
	orthoRequested.clear();
	compoRemains  .clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::clear() noexcept {
	clearRequests();

	compoActive	  .clear();
	compoResumable.clear();
}

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isActive(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT)) {
		if (Parent parent = stateParents[stateId]) {
			HFSM2_ASSERT(parent.forkId > 0);

			return parent.prong == compoActive[parent.forkId - 1];
		} else
			return true;
	}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isResumable(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM2_ASSERT(parent.forkId > 0);

			return parent.prong == compoResumable[parent.forkId - 1];
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isPendingChange(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM2_ASSERT(parent.forkId > 0);

			return compoRequested[parent.forkId - 1] !=
				   compoActive	 [parent.forkId - 1];
		}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isPendingEnter(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM2_ASSERT(parent.forkId > 0);

			return parent.prong != compoActive	 [parent.forkId - 1] &&
				   parent.prong == compoRequested[parent.forkId - 1];
		}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isPendingExit(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM2_ASSERT(parent.forkId > 0);

			return parent.prong == compoActive	 [parent.forkId - 1] &&
				   parent.prong != compoRequested[parent.forkId - 1];
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
const Parent&
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::forkParent(const ForkID forkId) const noexcept {
	HFSM2_ASSERT(forkId > 0);

	return compoParents[forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::requestImmediate(const Transition& request) noexcept {
	// record request
	// promote it to all children

	if (request.destination == 0)
		return false;
	else if (HFSM2_CHECKED(request.destination < STATE_COUNT)) {
		Parent parent = stateParents[request.destination];

		if (HFSM2_CHECKED(parent)) {
			HFSM2_ASSERT(parent.forkId > 0);

			compoRequested[parent.forkId - 1] = parent.prong;

			for (parent = forkParent(parent.forkId);
				 parent;
				 parent = forkParent(parent.forkId))
			{
				HFSM2_ASSERT(parent.forkId > 0);
				compoRemains.set(parent.forkId - 1);

				if (compoActive	  [parent.forkId - 1] != parent.prong)
					compoRequested[parent.forkId - 1]  = parent.prong;
				else {
					parent = forkParent(parent.forkId);
					break;
				}
			}

			for (; parent; parent = forkParent(parent.forkId)) {
				HFSM2_ASSERT(parent.forkId > 0);
				compoRemains.set(parent.forkId - 1);
			}
		}
	}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::requestScheduled(const StateID stateId) noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT)) {
		const Parent parent = stateParents[stateId];

		HFSM2_ASSERT(parent.forkId > 0);
		compoResumable[parent.forkId - 1] = parent.prong;
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::clearRequests() noexcept {
	compoRequested.clear();
	orthoRequested.clear();
	compoRemains  .clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::clear() noexcept {
	clearRequests();

	compoActive	  .clear();
	compoResumable.clear();
}

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class ControlT {
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename, Strategy, typename, typename...>
	friend struct C_;

	template <typename, typename, typename, typename...>
	friend struct O_;

	template <typename, typename>
	friend class R_;

protected:
	using Context			= typename TArgs::Context;

	using StateList			= typename TArgs::StateList;
	using RegionList		= typename TArgs::RegionList;

	using Registry			= RegistryT<TArgs>;

	using Payload			= typename TArgs::Payload;
	using Transition		= TransitionT<Payload>;
	using TransitionSet		= ArrayT<Transition, TArgs::COMPO_REGIONS>;
	using TransitionSets	= ArrayT<Transition, TArgs::COMPO_REGIONS * TArgs::SUBSTITUTION_LIMIT>;

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY
	using TransitionTargets	= StaticArrayT<Short, TArgs::STATE_COUNT>;
#endif

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using RNG				= typename TArgs::RNG;
#endif

#ifdef HFSM2_ENABLE_PLANS
	using PlanData			= PlanDataT<TArgs>;
	using CPlan				= CPlanT<TArgs>;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger			= typename TArgs::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Origin {
		HFSM2_INLINE Origin(ControlT& control_,
							const StateID stateId)					  noexcept;

		HFSM2_INLINE ~Origin()										  noexcept;

		ControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		HFSM2_INLINE Region(ControlT& control,
							const RegionID regionId)				  noexcept;

		HFSM2_INLINE ~Region()										  noexcept;

		ControlT& control;
		const RegionID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE ControlT(Context& context
						, Registry& registry
						, TransitionSet& requests
						HFSM2_IF_UTILITY_THEORY(, RNG& rng)
						HFSM2_IF_PLANS(, PlanData& planData)
						HFSM2_IF_TRANSITION_HISTORY(, TransitionTargets& transitionTargets)
						HFSM2_IF_TRANSITION_HISTORY(, const TransitionSets& previousTransitions)
						HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
		: _context{context}
		, _registry{registry}
		, _requests{requests}
		HFSM2_IF_UTILITY_THEORY(, _rng{rng})
		HFSM2_IF_PLANS(, _planData{planData})
		HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets{transitionTargets})
		HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions{previousTransitions})
		HFSM2_IF_LOG_INTERFACE(, _logger{logger})
	{}

	HFSM2_INLINE void setOrigin  (const StateID	 stateId)			  noexcept;
	HFSM2_INLINE void resetOrigin(const StateID	 stateId)			  noexcept;

	HFSM2_INLINE void setRegion	 (const RegionID regionId)			  noexcept;
	HFSM2_INLINE void resetRegion(const RegionID regionId)			  noexcept;

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY
	HFSM2_INLINE void pinLastTransition(const StateID stateId, const Short index) noexcept;
#endif

public:

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()								  noexcept	{ return			index<StateList , TState>();	}

	/// @brief Get region identifier for a region type
	/// @tparam TState Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static constexpr RegionID regionId()							  noexcept	{ return (RegionID) index<RegionList, TState>();	}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	HFSM2_INLINE	   Context& _()									  noexcept	{ return _context;									}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	HFSM2_INLINE const Context& _()								const noexcept	{ return _context;									}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	HFSM2_INLINE	   Context& context()							  noexcept	{ return _context;									}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	HFSM2_INLINE const Context& context()						const noexcept	{ return _context;									}

	//----------------------------------------------------------------------

	/// @brief Inspect current transition requests
	/// @return Array of transition requests
	HFSM2_INLINE const TransitionSet& requests()				const noexcept	{ return _requests;									}

	//----------------------------------------------------------------------

	/// @brief Check if a state is active
	/// @param stateId State identifier
	/// @return State active status
	HFSM2_INLINE bool isActive   (const StateID stateId)		const noexcept	{ return _registry.isActive   (stateId);			}

	/// @brief Check if a state is active
	/// @tparam TState State type
	/// @return State active status
	template <typename TState>
	HFSM2_INLINE bool isActive	 ()								const noexcept	{ return isActive	(stateId<TState>());			}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @param stateId State identifier
	/// @return State resumable status
	HFSM2_INLINE bool isResumable(const StateID stateId)		const noexcept	{ return _registry.isResumable(stateId);			}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @tparam TState State type
	/// @return State resumable status
	template <typename TState>
	HFSM2_INLINE bool isResumable()								const noexcept	{ return isResumable(stateId<TState>());			}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @param stateId State identifier
	/// @return State scheduled status
	HFSM2_INLINE bool isScheduled(const StateID stateId)		const noexcept	{ return isResumable(stateId);						}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @tparam TState State type
	/// @return State scheduled status
	template <typename TState>
	HFSM2_INLINE bool isScheduled()								const noexcept	{ return isResumable(stateId<TState>());			}

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_PLANS

	/// @brief Access read-only plan for the current region
	/// @return Plan for the current region
	HFSM2_INLINE CPlan plan()									const noexcept	{ return CPlan{_planData, _regionId};				}

	/// @brief Access read-only plan for a region
	/// @param regionId Region identifier
	/// @return Read-only plan for the region
	HFSM2_INLINE CPlan plan(const RegionID regionId)			const noexcept	{ return CPlan{_planData, regionId};				}

	/// @brief Access read-only plan for a region
	/// @tparam TRegion Region head state type
	/// @return Read-only plan for the region
	template <typename TRegion>
	HFSM2_INLINE CPlan plan()										  noexcept	{ return CPlan{_planData, regionId<TRegion>()};		}

	/// @brief Access read-only plan for a region
	/// @tparam TRegion Region head state type
	/// @return Read-only Plan for the region
	template <typename TRegion>
	HFSM2_INLINE CPlan plan()									const noexcept	{ return CPlan{_planData, regionId<TRegion>()};		}

#endif

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY

	/// @brief Get transitions processed during last 'update()', 'react()' or 'replayTransitions()'
	/// @return Array of last transition requests
	HFSM2_INLINE const TransitionSets& previousTransitions()	const noexcept	{ return _previousTransitions;						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get the last transition that caused the state to be activated
	/// @param stateId State identifier
	/// @return Pointer to the last transition that activated the state
	const Transition* lastTransition(const StateID stateId)		const noexcept;

	/// @brief Get the last transition that caused the state to be activated
	/// @tparam TState State type
	/// @return Pointer to the last transition that activated the state
	template <typename TState>
	const Transition* lastTransition()							const noexcept	{ return lastTransition(stateId<TState>());			}

	/// @brief Get the last transition that caused the current state to be activated
	/// @return Pointer to the last transition that activated the current state
	const Transition* lastTransition()							const noexcept;

#endif

	//----------------------------------------------------------------------

protected:
#ifdef HFSM2_ENABLE_LOG_INTERFACE
	HFSM2_INLINE Logger* logger()									  noexcept	{ return _logger;									}
#endif

protected:
	Context& _context;
	Registry& _registry;
	TransitionSet& _requests;
	StateID _originId = INVALID_STATE_ID;
	RegionID _regionId = 0;
	HFSM2_IF_UTILITY_THEORY(RNG& _rng);
	HFSM2_IF_PLANS(PlanData& _planData);
	HFSM2_IF_TRANSITION_HISTORY(TransitionTargets& _transitionTargets);
	HFSM2_IF_TRANSITION_HISTORY(const TransitionSets& _previousTransitions);
	HFSM2_IF_LOG_INTERFACE(Logger* _logger);
};

//------------------------------------------------------------------------------

template <typename TArgs>
class PlanControlT
	: public ControlT<TArgs>
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename, Strategy, typename, typename...>
	friend struct C_;

	template <typename, typename, typename, typename...>
	friend struct O_;

	template <typename, typename>
	friend class R_;

protected:
	using Control		= ControlT<TArgs>;

	using typename Control::StateList;
	using typename Control::RegionList;

#ifdef HFSM2_ENABLE_PLANS
	using typename Control::PlanData;
	using typename Control::CPlan;

	using Plan			= PlanT<TArgs>;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		HFSM2_INLINE Region(PlanControlT& control,
							const RegionID regionId,
							const StateID index,
							const Long size) noexcept;

		HFSM2_INLINE ~Region() noexcept;

		PlanControlT& control;
		const RegionID prevId;
		const Long prevIndex;
		const Long prevSize;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Control::Control;

	HFSM2_INLINE void	setRegion(const RegionID regionId, const StateID index, const Long size) noexcept;
	HFSM2_INLINE void resetRegion(const RegionID regionId, const StateID index, const Long size) noexcept;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_PLANS
// COMMON

	/// @brief Access plan for the current region
	/// @return Plan for the current region
	HFSM2_INLINE	  Plan plan()							  noexcept	{ return  Plan{_planData, _regionId};								}

// COMMON

	/// @brief Access plan for a region
	/// @param regionId
	/// @return Plan for the region
	HFSM2_INLINE	  Plan plan(const RegionID regionId)	  noexcept	{ return  Plan{_planData,  regionId};								}

	/// @brief Access plan for a region
	/// @tparam TRegion Region head state type
	/// @return Plan for the region
	template <typename TRegion>
	HFSM2_INLINE	  Plan plan()							  noexcept	{ return  Plan{_planData, Control::template regionId<TRegion>()};	}

// COMMON

	/// @brief Access plan for the current region
	/// @return Plan for the current region
	HFSM2_INLINE CPlan plan()							const noexcept	{ return CPlan{_planData, _regionId};								}

// COMMON

	/// @brief Access plan for a region
	/// @param regionId
	/// @return Plan for the region
	HFSM2_INLINE CPlan plan(const RegionID regionId)	const noexcept	{ return CPlan{_planData,  regionId};								}

	/// @brief Access plan for a region
	/// @tparam TRegion Region head state type
	/// @return Plan for the region
	template <typename TRegion>
	HFSM2_INLINE CPlan plan()							const noexcept	{ return CPlan{_planData, Control::template regionId<TRegion>()};	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using Control::_regionId;
	HFSM2_IF_PLANS(using Control::_planData);
	HFSM2_IF_LOG_INTERFACE(using Control::_logger);

	StateID _regionStateId = 0;
	Long _regionSize = StateList::SIZE;
	Status _status;
};

//------------------------------------------------------------------------------

template <typename TArgs>
class FullControlBaseT
	: public PlanControlT<TArgs>
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename, Strategy, typename, typename...>
	friend struct C_;

	template <typename, typename, typename, typename...>
	friend struct O_;

	template <typename, typename>
	friend class R_;

protected:
	using PlanControl	= PlanControlT<TArgs>;

	using typename PlanControl::StateList;
	using typename PlanControl::RegionList;

	using typename PlanControl::Transition;

#ifdef HFSM2_ENABLE_PLANS
	using TasksBits		= BitArrayT<StateID, StateList::SIZE>;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Lock {
		HFSM2_INLINE Lock(FullControlBaseT& control_) noexcept;
		HFSM2_INLINE ~Lock() noexcept;

		FullControlBaseT* const control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using PlanControl::PlanControl;

#ifdef HFSM2_ENABLE_PLANS

	template <typename TState>
	Status buildPlanStatus() noexcept;

#endif

public:
	using PlanControl::context;

	//----------------------------------------------------------------------
	// COMMON

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId State identifier
	HFSM2_INLINE void changeTo (const StateID stateId)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_INLINE void changeTo ()						  noexcept	{ changeTo (PlanControl::template stateId<TState>());	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param stateId State identifier
	HFSM2_INLINE void restart  (const StateID stateId)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_INLINE void restart  ()						  noexcept	{ restart  (PlanControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @param stateId State identifier
	HFSM2_INLINE void resume   (const StateID stateId)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_INLINE void resume   ()						  noexcept	{ resume   (PlanControl::template stateId<TState>());	}

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @param stateId State identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void utilize  (const StateID stateId)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///   with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TState State type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void utilize  ()						  noexcept	{ utilize  (PlanControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @param stateId State identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void randomize(const StateID stateId)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///   proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TState State type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void randomize()						  noexcept	{ randomize(PlanControl::template stateId<TState>());	}

#endif

	//----------------------------------------------------------------------

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId State identifier
	HFSM2_INLINE void schedule (const StateID stateId)	  noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState State type
	template <typename TState>
	HFSM2_INLINE void schedule ()						  noexcept	{ schedule (PlanControl::template stateId<TState>());	}

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_PLANS

	/// @brief Succeed a plan task for the current state
	HFSM2_INLINE void succeed()							  noexcept;

	/// @brief Fail a plan task for the current state
	HFSM2_INLINE void fail()							  noexcept;

#endif

	//----------------------------------------------------------------------

protected:
	using PlanControl::_regionId;
	using PlanControl::_requests;
	HFSM2_IF_PLANS(using PlanControl::_planData);
	HFSM2_IF_LOG_INTERFACE(using PlanControl::_logger);

	using PlanControl::_originId;
	using PlanControl::_regionStateId;
	using PlanControl::_regionSize;
	using PlanControl::_status;

	bool _locked = false;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
class FullControlT;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NCompoCount
		, Long NOrthoCount
		, Long NOrthoUnits
		HFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
class FullControlT<ArgsT<TContext
					   , TConfig
					   , TStateList
					   , TRegionList
					   , NCompoCount
					   , NOrthoCount
					   , NOrthoUnits
					   HFSM2_IF_SERIALIZATION(, NSerialBits)
					   , NSubstitutionLimit
					   HFSM2_IF_PLANS(, NTaskCapacity)
					   , TPayload>>
	: public FullControlBaseT<ArgsT<TContext
								  , TConfig
								  , TStateList
								  , TRegionList
								  , NCompoCount
								  , NOrthoCount
								  , NOrthoUnits
								  HFSM2_IF_SERIALIZATION(, NSerialBits)
								  , NSubstitutionLimit
								  HFSM2_IF_PLANS(, NTaskCapacity)
								  , TPayload>>
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename, Strategy, typename, typename...>
	friend struct C_;

	template <typename, typename, typename, typename...>
	friend struct O_;

	template <typename, typename>
	friend class R_;

	using Args = ArgsT<TContext
					 , TConfig
					 , TStateList
					 , TRegionList
					 , NCompoCount
					 , NOrthoCount
					 , NOrthoUnits
					 HFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 HFSM2_IF_PLANS(, NTaskCapacity)
					 , TPayload>;

protected:
	using FullControlBase = FullControlBaseT<Args>;

	using typename FullControlBase::Payload;
	using typename FullControlBase::Transition;

	using typename FullControlBase::Origin;

#ifdef HFSM2_ENABLE_PLANS
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using FullControlBase::FullControlBase;

#ifdef HFSM2_ENABLE_PLANS

	template <typename TState>
	Status updatePlan(TState& headState, const Status subStatus) noexcept;

#endif

public:
	using FullControlBase::context;

	using FullControlBase::isActive;
	using FullControlBase::changeTo;

	HFSM2_IF_PLANS(using FullControlBase::plan);

	//------------------------------------------------------------------------------
	// COMMON

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void changeWith   (const StateID  stateId,
									const Payload& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void changeWith   (const StateID  stateId,
										 Payload&& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void changeWith   (const Payload& payload)	  noexcept { changeWith   (FullControlBase::template stateId<TState>(),			  payload );	}

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void changeWith   (	 Payload&& payload)	  noexcept { changeWith   (FullControlBase::template stateId<TState>(), std::move(payload));	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void restartWith  (const StateID  stateId,
									const Payload& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void restartWith  (const StateID  stateId,
										 Payload&& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void restartWith  (const Payload& payload)	  noexcept { restartWith  (FullControlBase::template stateId<TState>(),			  payload );	}

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void restartWith  (	 Payload&& payload)	  noexcept { restartWith  (FullControlBase::template stateId<TState>(), std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void resumeWith   (const StateID  stateId,
									const Payload& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void resumeWith   (const StateID  stateId,
										 Payload&& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void resumeWith   (const Payload& payload)	  noexcept { resumeWith   (FullControlBase::template stateId<TState>(),			  payload );	}

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void resumeWith   (	 Payload&& payload)	  noexcept { resumeWith   (FullControlBase::template stateId<TState>(), std::move(payload));	}

	//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void utilizeWith  (const StateID  stateId,
									const Payload& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void utilizeWith  (const StateID  stateId,
										 Payload&& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void utilizeWith  (const Payload& payload)	  noexcept { utilizeWith  (FullControlBase::template stateId<TState>(),			  payload );	}

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void utilizeWith  (	 Payload&& payload)	  noexcept { utilizeWith  (FullControlBase::template stateId<TState>(), std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void randomizeWith(const StateID  stateId,
									const Payload& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void randomizeWith(const StateID  stateId,
										 Payload&& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void randomizeWith(const Payload& payload)	  noexcept { randomizeWith(FullControlBase::template stateId<TState>(),			  payload );	}

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void randomizeWith(	 Payload&& payload)	  noexcept { randomizeWith(FullControlBase::template stateId<TState>(), std::move(payload));	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void scheduleWith (const StateID  stateId,
									const Payload& payload)	  noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void scheduleWith (const StateID  stateId,
										 Payload&& payload)	  noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void scheduleWith (const Payload& payload)	  noexcept { scheduleWith (FullControlBase::template stateId<TState>(),			  payload );	}

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void scheduleWith (	 Payload&& payload)	  noexcept { scheduleWith (FullControlBase::template stateId<TState>(), std::move(payload));	}

	//------------------------------------------------------------------------------

protected:
	using FullControlBase::_regionId;
	using FullControlBase::_requests;
	HFSM2_IF_PLANS(using FullControlBase::_planData);
	HFSM2_IF_LOG_INTERFACE(using FullControlBase::_logger);

	using FullControlBase::_originId;
	using FullControlBase::_regionStateId;
	using FullControlBase::_regionSize;
	using FullControlBase::_status;

	using FullControlBase::_locked;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NCompoCount
		, Long NOrthoCount
		, Long NOrthoUnits
		HFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)>
class FullControlT<ArgsT<TContext
					   , TConfig
					   , TStateList
					   , TRegionList
					   , NCompoCount
					   , NOrthoCount
					   , NOrthoUnits
					   HFSM2_IF_SERIALIZATION(, NSerialBits)
					   , NSubstitutionLimit
					   HFSM2_IF_PLANS(, NTaskCapacity)
					   , void>>
	: public FullControlBaseT<ArgsT<TContext
								  , TConfig
								  , TStateList
								  , TRegionList
								  , NCompoCount
								  , NOrthoCount
								  , NOrthoUnits
								  HFSM2_IF_SERIALIZATION(, NSerialBits)
								  , NSubstitutionLimit
								  HFSM2_IF_PLANS(, NTaskCapacity)
								  , void>>
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename, Strategy, typename, typename...>
	friend struct C_;

	template <typename, typename, typename, typename...>
	friend struct O_;

	template <typename, typename>
	friend class R_;

	using Args = ArgsT<TContext
					 , TConfig
					 , TStateList
					 , TRegionList
					 , NCompoCount
					 , NOrthoCount
					 , NOrthoUnits
					 HFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 HFSM2_IF_PLANS(, NTaskCapacity)
					 , void>;

protected:
	using FullControlBase = FullControlBaseT<Args>;

	using typename FullControlBase::Origin;

#ifdef HFSM2_ENABLE_PLANS
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using FullControlBase::FullControlBase;

#ifdef HFSM2_ENABLE_PLANS

	template <typename TState>
	Status updatePlan(TState& headState, const Status subStatus)	  noexcept;

#endif

public:
	using FullControlBase::isActive;
	using FullControlBase::changeTo;

	HFSM2_IF_PLANS(using FullControlBase::plan);

protected:
	using FullControlBase::_regionId;
	HFSM2_IF_PLANS(using FullControlBase::_planData);

	using FullControlBase::_status;
};

//------------------------------------------------------------------------------

template <typename TArgs>
class GuardControlT final
	: public FullControlT<TArgs>
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename>
	friend class R_;

	using FullControl	= FullControlT<TArgs>;

	using typename FullControl::Context;

	using typename FullControl::Registry;
	using typename FullControl::TransitionSet;
	using typename FullControl::TransitionSets;

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY
	using typename FullControl::TransitionTargets;
#endif

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using typename FullControl::RNG;
#endif

#ifdef HFSM2_ENABLE_PLANS
	using typename FullControl::PlanData;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using typename FullControl::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE GuardControlT(Context& context
							 , Registry& registry
							 , TransitionSet& requests
							 , const TransitionSets& currentTransitions
							 , const TransitionSet& pendingTransitions
							 HFSM2_IF_UTILITY_THEORY(, RNG& rng)
							 HFSM2_IF_PLANS(, PlanData& planData)
							 HFSM2_IF_TRANSITION_HISTORY(, TransitionTargets& transitionTargets)
							 HFSM2_IF_TRANSITION_HISTORY(, const TransitionSets& previousTransitions)
							 HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
		: FullControl{context
					, registry
					, requests
					HFSM2_IF_UTILITY_THEORY(, rng)
					HFSM2_IF_PLANS(, planData)
					HFSM2_IF_TRANSITION_HISTORY(, transitionTargets)
					HFSM2_IF_TRANSITION_HISTORY(, previousTransitions)
					HFSM2_IF_LOG_INTERFACE(, logger)}
		, _currentTransitions{currentTransitions}
		, _pendingTransitions{pendingTransitions}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	using FullControl::context;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be activated or deactivated
	/// @param stateId State identifier
	/// @return State pending activation/deactivation status
	HFSM2_INLINE bool isPendingChange(const StateID stateId)	const noexcept { return _registry.isPendingChange(stateId);							}

	/// @brief Check if a state is going to be activated or deactivated
	/// @tparam TState State type
	/// @return State pending activation/deactivation status
	template <typename TState>
	HFSM2_INLINE bool isPendingChange()							const noexcept { return isPendingChange(FullControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be activated
	/// @param stateId State identifier
	/// @return State pending activation status
	HFSM2_INLINE bool isPendingEnter (const StateID stateId)	const noexcept { return _registry.isPendingEnter (stateId);							}

	/// @brief Check if a state is going to be activated
	/// @tparam TState State type
	/// @return State pending activation status
	template <typename TState>
	HFSM2_INLINE bool isPendingEnter ()							const noexcept { return isPendingEnter (FullControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be deactivated
	/// @param stateId State identifier
	/// @return State pending deactivation status
	HFSM2_INLINE bool isPendingExit	 (const StateID stateId)	const noexcept { return _registry.isPendingExit  (stateId);							}

	/// @brief Check if a state is going to be deactivated
	/// @tparam TState State type
	/// @return State pending deactivation status
	template <typename TState>
	HFSM2_INLINE bool isPendingExit  ()							const noexcept { return isPendingExit  (FullControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	HFSM2_INLINE const TransitionSets& currentTransitions()		const noexcept { return _currentTransitions;										}

	/// @brief Get pending transition requests
	/// @return Array of pending transition requests
	HFSM2_INLINE const TransitionSet&  pendingTransitions()		const noexcept { return _pendingTransitions;										}

	/// @brief Cancel pending transition requests
	///		(can be used to substitute a transition into the current state with a different one)
	HFSM2_INLINE void cancelPendingTransitions()					  noexcept;

private:
	using FullControl::_registry;
	using FullControl::_originId;

	HFSM2_IF_LOG_INTERFACE(using FullControl::_logger);

	const TransitionSets& _currentTransitions;
	const TransitionSet&  _pendingTransitions;
	bool _cancelled = false;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <typename TArgs>
ControlT<TArgs>::Origin::Origin(ControlT& control_,
								const StateID stateId) noexcept
	: control{control_}
	, prevId{control._originId}
{
	control.setOrigin(stateId);
}

//------------------------------------------------------------------------------

template <typename TArgs>
ControlT<TArgs>::Origin::~Origin() noexcept {
	control.resetOrigin(prevId);
}

// COMMON
////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
ControlT<TArgs>::Region::Region(ControlT& control_,
								const RegionID regionId) noexcept
	: control{control_}
	, prevId{control._regionId}
{
	control.setRegion(regionId);
}

//------------------------------------------------------------------------------

template <typename TArgs>
ControlT<TArgs>::Region::~Region() noexcept {
	control.resetRegion(prevId);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
void
ControlT<TArgs>::setOrigin(const StateID stateId) noexcept {
	HFSM2_ASSERT(stateId < StateList::SIZE);

	_originId = stateId;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
void
ControlT<TArgs>::resetOrigin(const StateID stateId) noexcept { //-V524
	_originId = stateId;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
ControlT<TArgs>::setRegion(const RegionID regionId) noexcept {
	HFSM2_ASSERT(_regionId <= regionId && regionId < RegionList::SIZE);

	_regionId = regionId;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
void
ControlT<TArgs>::resetRegion(const RegionID regionId) noexcept { //-V524
	HFSM2_ASSERT(regionId <= _regionId && _regionId < RegionList::SIZE);

	_regionId = regionId;
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY

template <typename TArgs>
void
ControlT<TArgs>::pinLastTransition(const StateID stateId,
								   const Short index) noexcept
{
	if (index != INVALID_SHORT) {
		HFSM2_ASSERT(index < TransitionSets::CAPACITY);

		if (!_registry.isActive(stateId))
			_transitionTargets[stateId] = index;
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
const typename ControlT<TArgs>::Transition*
ControlT<TArgs>::lastTransition(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < StateList::SIZE)) {
		const Short index = _transitionTargets[stateId];

		if (index < _previousTransitions.count())
			return &_previousTransitions[index];
	}

	return nullptr;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
const typename ControlT<TArgs>::Transition*
ControlT<TArgs>::lastTransition() const noexcept {
	HFSM2_ASSERT(_originId < _transitionTargets.CAPACITY);

	return lastTransition(_originId);
}

#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
PlanControlT<TArgs>::Region::Region(PlanControlT& control_,
									const RegionID regionId,
									const StateID index,
									const Long size) noexcept
	: control  {control_}
	, prevId   {control._regionId}
	, prevIndex{control._regionStateId}
	, prevSize {control._regionSize}
{
	control.setRegion(regionId, index, size);
}

//------------------------------------------------------------------------------

template <typename TArgs>
PlanControlT<TArgs>::Region::~Region() noexcept {
	control.resetRegion(prevId, prevIndex, prevSize);

	control._status.clear();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
void
PlanControlT<TArgs>::setRegion(const RegionID regionId,
							   const StateID index,
							   const Long size) noexcept
{
	HFSM2_ASSERT(_regionId <= regionId && regionId <  RegionList::SIZE);
	HFSM2_ASSERT(_regionStateId <= index && index + size <= _regionStateId + _regionSize);

	_regionId	   = regionId;
	_regionStateId = index;
	_regionSize	   = size;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
void
PlanControlT<TArgs>::resetRegion(const RegionID regionId, //-V524
								 const StateID index,
								 const Long size) noexcept
{
	HFSM2_ASSERT(regionId <= _regionId && _regionId < RegionList::SIZE);
	HFSM2_ASSERT(index <= _regionStateId && _regionStateId + _regionSize <= index + size);

	_regionId	   = regionId;
	_regionStateId = index;
	_regionSize	   = size;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FullControlBaseT<TArgs>::Lock::Lock(FullControlBaseT& control_) noexcept
	: control{!control_._locked ? &control_ : nullptr}
{
	if (control)
		control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FullControlBaseT<TArgs>::Lock::~Lock() noexcept {
	if (control)
		control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_PLANS

template <typename TArgs>
template <typename TState>
Status
FullControlBaseT<TArgs>::buildPlanStatus() noexcept {
	using State = TState;
	static constexpr StateID STATE_ID = State::STATE_ID;

	switch (_status.result) {
	case Status::Result::NONE:
		HFSM2_BREAK();
		break;

	case Status::Result::SUCCESS:
		_planData.tasksSuccesses.template set<STATE_ID>();

		HFSM2_LOG_PLAN_STATUS(context(), _regionId, StatusEvent::SUCCEEDED);
		break;

	case Status::Result::FAILURE:
		_planData.tasksFailures .template set<STATE_ID>();

		HFSM2_LOG_PLAN_STATUS(context(), _regionId, StatusEvent::FAILED);
		break;

	default:
		HFSM2_BREAK();
	}

	return {_status.result};
}

#endif

//------------------------------------------------------------------------------
// COMMON

template <typename TArgs>
void
FullControlBaseT<TArgs>::changeTo(const StateID stateId) noexcept {
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::CHANGE});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::CHANGE, stateId);
	}
}

// COMMON
//------------------------------------------------------------------------------

template <typename TArgs>
void
FullControlBaseT<TArgs>::restart(const StateID stateId) noexcept {
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RESTART});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESTART, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
FullControlBaseT<TArgs>::resume(const StateID stateId) noexcept {
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RESUME});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESUME, stateId);
	}
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TArgs>
void
FullControlBaseT<TArgs>::utilize(const StateID stateId) noexcept {
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::UTILIZE});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::UTILIZE, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
FullControlBaseT<TArgs>::randomize(const StateID stateId) noexcept {
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RANDOMIZE});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RANDOMIZE, stateId);
	}
}

#endif

//------------------------------------------------------------------------------

template <typename TArgs>
void
FullControlBaseT<TArgs>::schedule(const StateID stateId) noexcept {
	_requests.append(Transition{_originId, stateId, TransitionType::SCHEDULE});

	HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SCHEDULE, stateId);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_PLANS

template <typename TArgs>
void
FullControlBaseT<TArgs>::succeed() noexcept {
	_status.result = Status::Result::SUCCESS;

	_planData.tasksSuccesses.set(_originId);

	// TODO: promote taskSuccess all the way up for all regions without plans
	if (_regionId < RegionList::SIZE && !_planData.planExists.get(_regionId)) {
		HFSM2_ASSERT(_regionStateId < StateList::SIZE);

		_planData.tasksSuccesses.set(_regionStateId);
	}

	HFSM2_LOG_TASK_STATUS(context(), _regionId, _originId, StatusEvent::SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
void
FullControlBaseT<TArgs>::fail() noexcept {
	_status.result = Status::Result::FAILURE;

	_planData.tasksFailures.set(_originId);

	// TODO: promote taskFailure all the way up for all regions without plans
	if (_regionId < RegionList::SIZE && !_planData.planExists.get(_regionId)) {
		HFSM2_ASSERT(_regionStateId < StateList::SIZE);

		_planData.tasksFailures.set(_regionStateId);
	}

	HFSM2_LOG_TASK_STATUS(context(), _regionId, _originId, StatusEvent::FAILED);
}

#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_PLANS

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
template <typename TState>
Status
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::updatePlan(TState& headState,
																											  const Status subStatus) noexcept
{
	using State = TState;
	static constexpr StateID STATE_ID = State::STATE_ID;

	HFSM2_ASSERT(subStatus);

	if (subStatus.result == Status::Result::FAILURE) {
		_status.result = Status::Result::FAILURE;
		headState.wrapPlanFailed(*this);

		if (Plan p = plan(_regionId))
			p.clear();

		return FullControlBase::template buildPlanStatus<State>();
	} else if (subStatus.result == Status::Result::SUCCESS) {
		if (Plan p = plan(_regionId)) {
			TasksBits processed;

			for (auto it = p.first(); it; ++it) {
				if (isActive(it->origin) &&
					_planData.tasksSuccesses.get(it->origin) &&
					!processed.get(it->origin))
				{
					Origin origin{*this, STATE_ID};
					changeTo(it->destination);

					_planData.tasksSuccesses.clear(it->origin);
					processed.set(it->origin);
					it.remove();
				}
			}

			return Status{};
		} else {
			_status.result = Status::Result::SUCCESS;
			headState.wrapPlanSucceeded(*this);

			return FullControlBase::template buildPlanStatus<State>();
		}
	} else
		return Status{};
}

#endif

//------------------------------------------------------------------------------
// COMMON

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::changeWith(const StateID stateId,
																															   const Payload& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::CHANGE, payload});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::CHANGE, stateId);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::changeWith(const StateID stateId,
																															   Payload&& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::CHANGE, std::move(payload)});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::CHANGE, stateId);
	}
}

// COMMON
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::restartWith(const StateID stateId,
																																const Payload& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RESTART, payload});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESTART, stateId);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::restartWith(const StateID stateId,
																																Payload&& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RESTART, std::move(payload)});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESTART, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::resumeWith(const StateID stateId,
																															   const Payload& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RESUME, payload});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESUME, stateId);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::resumeWith(const StateID stateId,
																															   Payload&& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RESUME, std::move(payload)});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESUME, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::utilizeWith(const StateID stateId,
																																const Payload& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::UTILIZE, payload});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::UTILIZE, stateId);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::utilizeWith(const StateID stateId,
																																Payload&& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::UTILIZE, std::move(payload)});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::UTILIZE, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::randomizeWith(const StateID stateId,
																																  const Payload& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RANDOMIZE, payload});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RANDOMIZE, stateId);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::randomizeWith(const StateID stateId,
																																  Payload&& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RANDOMIZE, std::move(payload)});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RANDOMIZE, stateId);
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::scheduleWith(const StateID stateId,
																																 const Payload& payload) noexcept
{
	_requests.append(Transition{_originId, stateId, TransitionType::SCHEDULE, payload});

	HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SCHEDULE, stateId);
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::scheduleWith(const StateID stateId,
																																 Payload&& payload) noexcept
{
	_requests.append(Transition{_originId, stateId, TransitionType::SCHEDULE, std::move(payload)});

	HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SCHEDULE, stateId);
}

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_PLANS

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
template <typename TState>
Status
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::updatePlan(TState& headState,
																											   const Status subStatus) noexcept
{
	using State = TState;
	static constexpr StateID STATE_ID = State::STATE_ID;

	HFSM2_ASSERT(subStatus);

	if (subStatus.result == Status::Result::FAILURE) {
		_status.result = Status::Result::FAILURE;
		headState.wrapPlanFailed(*this);

		if (Plan p = plan(_regionId))
			p.clear();

		return FullControlBase::template buildPlanStatus<State>();
	} else if (subStatus.result == Status::Result::SUCCESS) {
		if (Plan p = plan(_regionId)) {
			TasksBits processed;

			for (auto it = p.first(); it; ++it) {
				if (isActive(it->origin) &&
					_planData.tasksSuccesses.get(it->origin) &&
					!processed.get(it->origin))
				{
					Origin origin{*this, STATE_ID};
					changeTo(it->destination);

					_planData.tasksSuccesses.clear(it->origin);
					processed.set(it->origin);
					it.remove();
				}
			}

			return Status{};
		} else {
			_status.result = Status::Result::SUCCESS;
			headState.wrapPlanSucceeded(*this);

			return FullControlBase::template buildPlanStatus<State>();
		}
	} else
		return Status{};
}

#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
void
GuardControlT<TArgs>::cancelPendingTransitions() noexcept {
	_cancelled = true;

	HFSM2_LOG_CANCELLED_PENDING(context(), _originId);
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class InjectionT {
	template <typename...>
	friend struct B_;

protected:
	using Context		= typename TArgs::Context;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Rank			= typename TArgs::Rank;
	using Utility		= typename TArgs::Utility;
#endif

	using StateList		= typename TArgs::StateList;
	using RegionList	= typename TArgs::RegionList;

	using Control		= ControlT<TArgs>;
	using PlanControl	= PlanControlT<TArgs>;

#ifdef HFSM2_ENABLE_PLANS
	using Plan			= PlanT<TArgs>;
#endif

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

public:
	HFSM2_INLINE void preEntryGuard(Context&)		  noexcept {}

	HFSM2_INLINE void preEnter	   (Context&)		  noexcept {}
	HFSM2_INLINE void preReenter   (Context&)		  noexcept {}

	HFSM2_INLINE void preUpdate	   (Context&)		  noexcept {}

	template <typename TEvent>
	HFSM2_INLINE void preReact	   (const TEvent&,
									Context&)		  noexcept {}

	HFSM2_INLINE void preExitGuard (Context&)		  noexcept {}

	HFSM2_INLINE void postExit	   (Context&)		  noexcept {}

	template <typename T>
	static constexpr StateID  stateId()				  noexcept { return index<StateList, T>();				}

	template <typename T>
	static constexpr RegionID regionId()			  noexcept { return (RegionID) index<RegionList, T>();	}
};

//------------------------------------------------------------------------------

template <typename...>
struct B_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TFirst, typename... TRest>
struct B_<TFirst, TRest...>
	: TFirst
	, B_<TRest...>
{
	using typename TFirst::Context;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using typename TFirst::Rank;
	using typename TFirst::Utility;
#endif

	using typename TFirst::StateList;
	using typename TFirst::RegionList;

	using typename TFirst::Control;
	using typename TFirst::PlanControl;

#ifdef HFSM2_ENABLE_PLANS
	using typename TFirst::Plan;
#endif

	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;
	using TFirst::regionId;

	HFSM2_INLINE void widePreEntryGuard(Context& context)		 noexcept;

	HFSM2_INLINE void widePreEnter	   (Context& context)		 noexcept;
	HFSM2_INLINE void widePreReenter   (Context& context)		 noexcept;

	HFSM2_INLINE void widePreUpdate	   (Context& context)		 noexcept;

	template <typename TEvent>
	HFSM2_INLINE void widePreReact	   (const TEvent& event,
										Context& context)		 noexcept;

	HFSM2_INLINE void widePreExitGuard (Context& context)		 noexcept;

	HFSM2_INLINE void widePostExit	   (Context& context)		 noexcept;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TFirst>
struct B_<TFirst>
	: TFirst
{
	using typename TFirst::Context;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using typename TFirst::Rank;
	using typename TFirst::Utility;
#endif

	using typename TFirst::StateList;
	using typename TFirst::RegionList;

	using typename TFirst::Control;
	using typename TFirst::PlanControl;

#ifdef HFSM2_ENABLE_PLANS
	using typename TFirst::Plan;
#endif

	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;
	using TFirst::regionId;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE Rank	 rank			  (const Control&)			  noexcept { return Rank	{0};	}
	HFSM2_INLINE Utility utility		  (const Control&)			  noexcept { return Utility{1};		}
#endif

	HFSM2_INLINE void	 entryGuard		  (GuardControl&)			  noexcept {}

	HFSM2_INLINE void	 enter			  (PlanControl&)			  noexcept {}
	HFSM2_INLINE void	 reenter		  (PlanControl&)			  noexcept {}

	HFSM2_INLINE void	 update			  (FullControl&)			  noexcept {}

	template <typename TEvent>
	HFSM2_INLINE void	 react			  (const TEvent&,
						 				   FullControl&)			  noexcept {}

	HFSM2_INLINE void	 exitGuard		  (GuardControl&)			  noexcept {}

	HFSM2_INLINE void	 exit			  (PlanControl&)			  noexcept {}

#ifdef HFSM2_ENABLE_PLANS
	HFSM2_INLINE void	 planSucceeded	  (FullControl& control)	  noexcept { control.succeed();		}
	HFSM2_INLINE void	 planFailed		  (FullControl& control)	  noexcept { control.fail();		}
#endif

	HFSM2_INLINE void	 widePreEntryGuard(Context& context)		  noexcept;

	HFSM2_INLINE void	 widePreEnter	  (Context& context)		  noexcept;
	HFSM2_INLINE void	 widePreReenter   (Context& context)		  noexcept;

	HFSM2_INLINE void	 widePreUpdate	  (Context& context)		  noexcept;

	template <typename TEvent>
	HFSM2_INLINE void	 widePreReact	  (const TEvent& event,
					 	 				   Context& context)		  noexcept;

	HFSM2_INLINE void	 widePreExitGuard (Context& context)		  noexcept;

	HFSM2_INLINE void	 widePostExit	  (Context& context)		  noexcept;
};

//------------------------------------------------------------------------------

template <typename TArgs>
using EmptyT = B_<InjectionT<TArgs>>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Dynamic_ {};

template <typename... TI>
struct DB_
	: Dynamic_
	, B_<TI...>
{};

template <typename TArgs>
using DynamicEmptyT = DB_<InjectionT<TArgs>>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Static_ {};

template <typename... TI>
struct SB_
	: Static_
	, B_<TI...>
{};

template <typename TArgs>
using StaticEmptyT = SB_<InjectionT<TArgs>>;

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePreEntryGuard(Context& context) noexcept {
	TF::preEntryGuard(context);
	B_<TR...>::widePreEntryGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePreEnter(Context& context) noexcept {
	TF::preEnter(context);
	B_<TR...>::widePreEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePreReenter(Context& context) noexcept {
	TF::preReenter(context);
	B_<TR...>::widePreReenter(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePreUpdate(Context& context) noexcept {
	TF::preUpdate(context);
	B_<TR...>::widePreUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
template <typename TEvent>
void
B_<TF, TR...>::widePreReact(const TEvent& event,
							Context& context) noexcept
{
	TF::preReact(event, context);
	B_<TR...>::widePreReact(event, context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePreExitGuard(Context& context) noexcept {
	TF::preExitGuard(context);
	B_<TR...>::widePreExitGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePostExit(Context& context) noexcept {
	TF::postExit(context);
	B_<TR...>::widePostExit(context);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TF>
void
B_<TF>::widePreEntryGuard(Context& context) noexcept {
	TF::preEntryGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF>
void
B_<TF>::widePreEnter(Context& context) noexcept {
	TF::preEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF>
void
B_<TF>::widePreReenter(Context& context) noexcept {
	TF::preReenter(context);
}

//------------------------------------------------------------------------------

template <typename TF>
void
B_<TF>::widePreUpdate(Context& context) noexcept {
	TF::preUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TF>
template <typename TEvent>
void
B_<TF>::widePreReact(const TEvent& event,
					 Context& context) noexcept
{
	TF::preReact(event, context);
}

//------------------------------------------------------------------------------

template <typename TF>
void
B_<TF>::widePreExitGuard(Context& context) noexcept {
	TF::preExitGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF>
void
B_<TF>::widePostExit(Context& context) noexcept {
	TF::postExit(context);
}

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {

//------------------------------------------------------------------------------

template <typename>
struct Guard {
	template <typename TArgs>
	static HFSM2_INLINE void execute(hfsm2::detail::GuardControlT<TArgs>&) noexcept {}
};

namespace detail {

//------------------------------------------------------------------------------

HFSM2_IF_DEBUG(struct None {});

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct DynamicBox final {
	using Type = T;

	static constexpr bool isBare()							  noexcept	{ return false;								}

	union {
		Type t_;
	};

	HFSM2_INLINE  DynamicBox()								  noexcept	{}
	HFSM2_INLINE ~DynamicBox()								  noexcept	{}

	HFSM2_INLINE void guard(GuardControlT<TArgs>& control)	  noexcept	{ Guard<Type>::execute(control);			}

	HFSM2_INLINE void construct()							  noexcept;
	HFSM2_INLINE void  destruct()							  noexcept;

	HFSM2_INLINE	   Type& get()							  noexcept	{ HFSM2_ASSERT(initialized_); return t_;	}
	HFSM2_INLINE const Type& get()						const noexcept	{ HFSM2_ASSERT(initialized_); return t_;	}

	HFSM2_IF_ASSERT(bool initialized_ = false);

	HFSM2_IF_DEBUG(const std::type_index TYPE = typeid(Type));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct StaticBox final {
	using Type = T;

	static constexpr bool isBare() noexcept	{ return std::is_base_of<Type, StaticEmptyT<TArgs>>::value;	}

	Type t_;

	HFSM2_INLINE void guard(GuardControlT<TArgs>& control)	  noexcept;

	HFSM2_INLINE void construct()							  noexcept	{}
	HFSM2_INLINE void  destruct()							  noexcept	{}

	HFSM2_INLINE	   Type& get()							  noexcept	{ return t_;								}
	HFSM2_INLINE const Type& get()						const noexcept	{ return t_;								}

	HFSM2_IF_DEBUG(const std::type_index TYPE = isBare() ? typeid(None) : typeid(Type));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct BoxifyT final {
	using Type = Conditional<
					 std::is_base_of<Dynamic_, T>::value,
					 DynamicBox<T, TArgs>,
					 StaticBox <T, TArgs>
				 >;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, typename TArgs>
using Boxify = typename BoxifyT<T, TArgs>::Type;

//------------------------------------------------------------------------------

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TA>
void
DynamicBox<T, TA>::construct() noexcept {
	HFSM2_ASSERT(!initialized_);

	new(&t_) T{};

	HFSM2_IF_ASSERT(initialized_ = true);
}

//------------------------------------------------------------------------------

template <typename T, typename TA>
void
DynamicBox<T, TA>::destruct() noexcept {
	HFSM2_ASSERT(initialized_);

	t_.~T();

	HFSM2_IF_ASSERT(initialized_ = false);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TA>
void
StaticBox<T, TA>::guard(GuardControlT<TA>& control) noexcept {
	t_.widePreEntryGuard(control.context());
	t_.		  entryGuard(control);
}

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  typename THead>
struct S_ final {
	static constexpr auto STATE_ID	 = TIndices::STATE_ID;

	using Context		= typename TArgs::Context;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Rank			= typename TArgs::Rank;
	using Utility		= typename TArgs::Utility;
	using UP			= typename TArgs::UP;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger		= typename TArgs::Logger;
#endif

	using Control		= ControlT<TArgs>;
	using Registry		= RegistryT<TArgs>;
	using StateParents	= typename Registry::StateParents;

	using PlanControl	= PlanControlT<TArgs>;
	using ScopedOrigin	= typename PlanControl::Origin;

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

	using Head			= THead;
	using HeadBox		= Boxify<Head, TArgs>;

	//----------------------------------------------------------------------

#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION
#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wnull-dereference"
#endif

	template <typename T>
	struct Accessor {
		HFSM2_INLINE static		  T&	get(	  S_&  )			  noexcept	{ HFSM2_BREAK(); return *reinterpret_cast<T*>(0);	}
		HFSM2_INLINE static const T&	get(const S_&  )			  noexcept	{ HFSM2_BREAK(); return *reinterpret_cast<T*>(0);	}
	};

#ifdef __clang__
	#pragma clang diagnostic pop
#endif

	template <>
	struct Accessor<Head> {
		HFSM2_INLINE static		  Head& get(	  S_& s)			  noexcept	{ return s._headBox.get();							}
		HFSM2_INLINE static const Head& get(const S_& s)			  noexcept	{ return s._headBox.get();							}
	};

	template <typename T>
	HFSM2_INLINE	   T& access()									  noexcept	{ return Accessor<T>::get(*this);					}

	template <typename T>
	HFSM2_INLINE const T& access()								const noexcept	{ return Accessor<T>::get(*this);					}
#endif

	//----------------------------------------------------------------------

	HFSM2_INLINE Parent	 stateParent		  (Control&  control)	  noexcept	{ return control._registry.stateParents[STATE_ID];	}

	HFSM2_INLINE void	 deepRegister		  (Registry& registry,
											   const Parent parent)	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE Rank	 wrapRank			  (Control&  control)	  noexcept;
	HFSM2_INLINE Utility wrapUtility		  (Control&  control)	  noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE bool	 deepForwardEntryGuard(GuardControl&		) noexcept		{ return false;	}
	HFSM2_INLINE bool	 deepEntryGuard		  (GuardControl& control) noexcept;

	HFSM2_INLINE void	 deepConstruct		  (PlanControl&  control) noexcept;

	HFSM2_INLINE void	 deepEnter			  (PlanControl&  control) noexcept;
	HFSM2_INLINE void	 deepReenter		  (PlanControl&  control) noexcept;

	HFSM2_INLINE Status	 deepUpdate			  (FullControl&  control) noexcept;

	template <typename TEvent>
	HFSM2_INLINE Status	 deepReact			  (FullControl&	 control,
											   const TEvent& event)	  noexcept;

	HFSM2_INLINE bool	 deepForwardExitGuard (GuardControl&		) noexcept		{ return false; }
	HFSM2_INLINE bool	 deepExitGuard		  (GuardControl& control) noexcept;

	HFSM2_INLINE void	 deepExit			  (PlanControl&	 control) noexcept;

	HFSM2_INLINE void	 deepDestruct		  (PlanControl&  control) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_PLANS
	HFSM2_INLINE void	 wrapPlanSucceeded	  (FullControl&	 control) noexcept;
	HFSM2_INLINE void	 wrapPlanFailed		  (FullControl&	 control) noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 deepForwardActive	  (Control&,		 const Request		  ) noexcept	{}
	HFSM2_INLINE void	 deepForwardRequest	  (Control& control, const Request request) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 deepRequestChange	  (Control& control, const Request request) noexcept;
	HFSM2_INLINE void	 deepRequestRestart	  (Control& control, const Request request) noexcept;
	HFSM2_INLINE void	 deepRequestResume	  (Control& control, const Request request) noexcept;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void	 deepRequestUtilize	  (Control& control, const Request request) noexcept;
	HFSM2_INLINE void	 deepRequestRandomize (Control& control, const Request request) noexcept;

	HFSM2_INLINE UP		 deepReportChange	  (Control& control) noexcept;
	HFSM2_INLINE UP		 deepReportUtilize	  (Control& control) noexcept;
	HFSM2_INLINE Rank	 deepReportRank		  (Control& control) noexcept;
	HFSM2_INLINE Utility deepReportRandomize  (Control& control) noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	deepChangeToRequested(Control&) noexcept									{}

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION
	using WriteStream	= typename TArgs::WriteStream;
	using ReadStream	= typename TArgs::ReadStream;

	HFSM2_INLINE void	 deepSaveActive	  (const Registry&, WriteStream&) const noexcept			{}
	HFSM2_INLINE void	 deepSaveResumable(const Registry&, WriteStream&) const noexcept			{}

	HFSM2_INLINE void	 deepLoadRequested(		 Registry&, ReadStream& ) const noexcept			{}
	HFSM2_INLINE void	 deepLoadResumable(		 Registry&, ReadStream& ) const noexcept			{}
#endif

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename TArgs::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static const char* name() noexcept;

	void deepGetNames(const Long parent,
					  const RegionType region,
					  const Short depth,
					  StructureStateInfos& stateInfos) const noexcept;
#endif

	//----------------------------------------------------------------------

#if defined _DEBUG || defined HFSM2_ENABLE_STRUCTURE_REPORT || defined HFSM2_ENABLE_LOG_INTERFACE

	static constexpr Long NAME_COUNT = HeadBox::isBare() ? 0 : 1;

#endif

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_LOG_INTERFACE

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Empty			= EmptyT<TArgs>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TReturn, typename THost, typename... TParams>
	void log(TReturn(THost::*)(TParams...),
			 Logger& logger,
			 Context& context,
			 const Method method) const noexcept
	{
		logger.recordMethod(context, STATE_ID, method);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TReturn, typename... TParams>
	void log(TReturn(Empty::*)(TParams...),
			 Logger&,
			 Context&,
			 const Method) const noexcept
	{}

#endif

	//----------------------------------------------------------------------

	// TODO: account for boxing
	//
	// if you see..
	// VS	 - error C2079: 'hfsm2::detail::S_<...>::_head' uses undefined struct 'Blah'
	// Clang - error : field has incomplete type 'hfsm2::detail::S_<...>::Head' (aka 'Blah')
	//
	// .. add definition for the state 'Blah'
	HeadBox _headBox;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
struct RegisterT {
	using StateParents	= StaticArrayT<Parent, TA::STATE_COUNT>;
	using StateList		= typename TA::StateList;

	static constexpr StateID STATE_ID = NS;

	static HFSM2_INLINE
	void
	execute(StateParents& stateParents,
			const Parent parent) noexcept
	{
		static constexpr auto HEAD_ID = index<StateList, TH>();
		static_assert(STATE_ID == HEAD_ID, "");

		stateParents[STATE_ID] = parent;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, typename TA>
struct RegisterT<NS, TA, StaticEmptyT<TA>> {
	using StateParents = StaticArrayT<Parent, TA::STATE_COUNT>;

	static HFSM2_INLINE
	void
	execute(StateParents&, const Parent) noexcept {}
};

////////////////////////////////////////////////////////////////////////////////

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepRegister(Registry& registry,
							  const Parent parent) noexcept
{
	using Register = RegisterT<STATE_ID, TA, Head>;
	Register::execute(registry.stateParents, parent);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TN_, typename TA, typename TH>
typename S_<TN_, TA, TH>::Rank
S_<TN_, TA, TH>::wrapRank(Control& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::rank,
						   Method::RANK);

	return _headBox.get().rank(static_cast<const Control&>(control));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
typename S_<TN_, TA, TH>::Utility
S_<TN_, TA, TH>::wrapUtility(Control& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::utility,
						   Method::UTILITY);

	return _headBox.get().utility(static_cast<const Control&>(control));
}

#endif

//------------------------------------------------------------------------------
// COMMON

template <typename TN_, typename TA, typename TH>
bool
S_<TN_, TA, TH>::deepEntryGuard(GuardControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::entryGuard,
						   Method::ENTRY_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	_headBox.guard(control);

	return !cancelledBefore && control._cancelled;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepConstruct(PlanControl&
						   #if defined HFSM2_ENABLE_PLANS || defined HFSM2_ENABLE_LOG_INTERFACE
							   control
						   #endif
							   ) noexcept {
	HFSM2_IF_PLANS(control._planData.verifyEmptyStatus(STATE_ID));

	HFSM2_LOG_STATE_METHOD(&Head::enter,
						   Method::CONSTRUCT);

	_headBox.construct();
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepEnter(PlanControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::enter,
						   Method::ENTER);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().widePreEnter(control.context());
	_headBox.get().		  enter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepReenter(PlanControl& control) noexcept {
	HFSM2_IF_PLANS(control._planData.verifyEmptyStatus(STATE_ID));

	HFSM2_LOG_STATE_METHOD(&Head::reenter,
						   Method::REENTER);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.destruct ();
	_headBox.construct();

	_headBox.get().widePreReenter(control.context());
	_headBox.get().		  reenter(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
Status
S_<TN_, TA, TH>::deepUpdate(FullControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::update,
						   Method::UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().widePreUpdate(control.context());
	_headBox.get().		  update(control);

	return control._status;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
template <typename TEvent>
Status
S_<TN_, TA, TH>::deepReact(FullControl& control,
						   const TEvent& event) noexcept
{
	auto reaction = static_cast<void (Head::*)(const TEvent&, FullControl&)>(&Head::react);

	HFSM2_LOG_STATE_METHOD(reaction,
						   Method::REACT);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().widePreReact(event, control.context());
	(_headBox.get().*reaction) (event, control);				//_headBox.get().react(event, control);

	return control._status;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
bool
S_<TN_, TA, TH>::deepExitGuard(GuardControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::exitGuard,
						   Method::EXIT_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	_headBox.get().widePreExitGuard(control.context());
	_headBox.get().		  exitGuard(control);

	return !cancelledBefore && control._cancelled;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepExit(PlanControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::exit,
						   Method::EXIT);

	ScopedOrigin origin{control, STATE_ID};

	// if you see..
	// VS	 - error C2039:  'exit': is not a member of 'Blah'
	// Clang - error : no member named 'exit' in 'Blah'
	//
	// .. inherit state 'Blah' from hfsm2::Machine::Instance::State
	_headBox.get().		   exit(control);
	_headBox.get().widePostExit(control.context());
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepDestruct(PlanControl&
						  #if defined HFSM2_ENABLE_LOG_INTERFACE || defined HFSM2_ENABLE_PLANS
							  control
						  #endif
							  ) noexcept
{
	HFSM2_LOG_STATE_METHOD(&Head::exit,
						   Method::DESTRUCT);

	_headBox.destruct();

	HFSM2_IF_PLANS(control._planData.clearTaskStatus(STATE_ID));
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_PLANS

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::wrapPlanSucceeded(FullControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::planSucceeded,
						   Method::PLAN_SUCCEEDED);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().planSucceeded(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::wrapPlanFailed(FullControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::planFailed,
						   Method::PLAN_FAILED);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().planFailed(control);
}

#endif

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepForwardRequest(Control& HFSM2_IF_TRANSITION_HISTORY(control),
									const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepRequestChange(Control& HFSM2_IF_TRANSITION_HISTORY(control),
								   const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepRequestRestart(Control& HFSM2_IF_TRANSITION_HISTORY(control),
									const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepRequestResume(Control& HFSM2_IF_TRANSITION_HISTORY(control),
								   const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepRequestUtilize(Control& HFSM2_IF_TRANSITION_HISTORY(control),
									const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepRequestRandomize(Control& HFSM2_IF_TRANSITION_HISTORY(control),
									  const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
typename S_<TN_, TA, TH>::UP
S_<TN_, TA, TH>::deepReportChange(Control& control) noexcept {
	const Utility utility = wrapUtility(control);

	const Parent parent = stateParent(control);

	return {utility, parent.prong};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
typename S_<TN_, TA, TH>::UP
S_<TN_, TA, TH>::deepReportUtilize(Control& control) noexcept {
	const Utility utility = wrapUtility(control);
	const Parent  parent  = stateParent(control);

	return {utility, parent.prong};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
typename S_<TN_, TA, TH>::Rank
S_<TN_, TA, TH>::deepReportRank(Control& control) noexcept {
	return wrapRank(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
typename S_<TN_, TA, TH>::Utility
S_<TN_, TA, TH>::deepReportRandomize(Control& control) noexcept {
	return wrapUtility(control);
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

template <typename TN_, typename TA, typename TH>
const char*
S_<TN_, TA, TH>::name() noexcept {
	if (HeadBox::isBare())
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

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepGetNames(const Long parent,
							  const RegionType region,
							  const Short depth,
							  StructureStateInfos& _stateInfos) const noexcept
{
	_stateInfos.append(StructureStateInfo{parent, region, depth, name()});
}

#endif

///////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename>
struct SI_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <Strategy, typename, typename...>
struct CI_;

template <typename...>
struct CSI_;

template <typename TInitial, typename... TRemaining>
struct CSI_<TInitial, TRemaining...>;

template <typename TInitial>
struct CSI_<TInitial>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename, typename...>
struct OI_;

template <typename...>
struct OSI_;

template <typename TInitial, typename... TRemaining>
struct OSI_<TInitial, TRemaining...>;

template <typename TInitial>
struct OSI_<TInitial>;

//------------------------------------------------------------------------------

template <typename...>
struct WrapInfoT;

template <typename TH>
struct WrapInfoT<	 TH> {
	using Type = SI_<TH>;
};

template <Strategy SG, typename TH, typename... TS>
struct WrapInfoT<	 CI_<SG, TH, TS...>> {
	using Type =	 CI_<SG, TH, TS...>;
};

template <typename... TS>
struct WrapInfoT<	 OI_<TS...>> {
	using Type =	 OI_<TS...>;
};

template <typename... TS>
using WrapInfo = typename WrapInfoT<TS...>::Type;

//------------------------------------------------------------------------------
// COMMON

template <typename THead>
struct SI_ final {
	using Head				= THead;
	using StateList			= TL_<Head>;
	using RegionList		= TL_<>;

	static constexpr Short WIDTH		  = 1;
	static constexpr Long  REVERSE_DEPTH  = 1;
	static constexpr Short COMPO_REGIONS  = 0;
	static constexpr Long  COMPO_PRONGS	  = 0;
	static constexpr Short ORTHO_REGIONS  = 0;
	static constexpr Short ORTHO_UNITS	  = 0;

#ifdef HFSM2_ENABLE_SERIALIZATION
	static constexpr Long  ACTIVE_BITS	  = 0;
	static constexpr Long  RESUMABLE_BITS = 0;
#endif

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
	static constexpr Short REGION_COUNT	  = RegionList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct CSI_<TInitial, TRemaining...> {
	using Initial			= WrapInfo<TInitial>;
	using Remaining			= CSI_<TRemaining...>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;
	using RegionList		= Merge<typename Initial::RegionList, typename Remaining::RegionList>;

	static constexpr Long  REVERSE_DEPTH  = Max<Initial::REVERSE_DEPTH,	  Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr Short COMPO_REGIONS  =		Initial::COMPO_REGIONS  + Remaining::COMPO_REGIONS;
	static constexpr Long  COMPO_PRONGS	  =		Initial::COMPO_PRONGS   + Remaining::COMPO_PRONGS;
	static constexpr Short ORTHO_REGIONS  =		Initial::ORTHO_REGIONS  + Remaining::ORTHO_REGIONS;
	static constexpr Short ORTHO_UNITS	  =		Initial::ORTHO_UNITS    + Remaining::ORTHO_UNITS;

#ifdef HFSM2_ENABLE_SERIALIZATION
	static constexpr Long  ACTIVE_BITS	  = Max<Initial::ACTIVE_BITS,	  Remaining::ACTIVE_BITS>::VALUE;
	static constexpr Long  RESUMABLE_BITS =		Initial::RESUMABLE_BITS + Remaining::RESUMABLE_BITS;
#endif

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
	static constexpr Short REGION_COUNT	  = RegionList::SIZE;
};

template <typename TInitial>
struct CSI_<TInitial> {
	using Initial			= WrapInfo<TInitial>;
	using StateList			= typename Initial::StateList;
	using RegionList		= typename Initial::RegionList;

	static constexpr Long  REVERSE_DEPTH  = Initial::REVERSE_DEPTH;
	static constexpr Short COMPO_REGIONS  = Initial::COMPO_REGIONS;
	static constexpr Long  COMPO_PRONGS	  = Initial::COMPO_PRONGS;
	static constexpr Short ORTHO_REGIONS  = Initial::ORTHO_REGIONS;
	static constexpr Short ORTHO_UNITS	  = Initial::ORTHO_UNITS;

#ifdef HFSM2_ENABLE_SERIALIZATION
	static constexpr Long  ACTIVE_BITS	  = Initial::ACTIVE_BITS;
	static constexpr Long  RESUMABLE_BITS = Initial::RESUMABLE_BITS;
#endif

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
	static constexpr Short REGION_COUNT	  = RegionList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <Strategy TStrategy, typename THead, typename... TSubStates>
struct CI_ final {
	static constexpr Strategy STRATEGY = TStrategy;

	using Head				= THead;
	using HeadInfo			= SI_<Head>;
	using SubStates			= CSI_<TSubStates...>;
	using StateList			= Merge<typename HeadInfo::StateList, typename SubStates::StateList>;
	using RegionList		= Merge<typename HeadInfo::StateList, typename SubStates::RegionList>;

	static constexpr Short WIDTH		  = sizeof...(TSubStates);
	static constexpr Long  REVERSE_DEPTH  = SubStates::REVERSE_DEPTH + 1;
	static constexpr Short COMPO_REGIONS  = SubStates::COMPO_REGIONS + 1;
	static constexpr Long  COMPO_PRONGS	  = SubStates::COMPO_PRONGS + WIDTH;
	static constexpr Short ORTHO_REGIONS  = SubStates::ORTHO_REGIONS;
	static constexpr Short ORTHO_UNITS	  = SubStates::ORTHO_UNITS;

#ifdef HFSM2_ENABLE_SERIALIZATION
	static constexpr Long  WIDTH_BITS	  = bitWidth(WIDTH);
	static constexpr Long  ACTIVE_BITS	  = SubStates::ACTIVE_BITS	+ WIDTH_BITS;
	static constexpr Long  RESUMABLE_BITS = SubStates::RESUMABLE_BITS + WIDTH_BITS + 1;
#endif

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
	static constexpr Short REGION_COUNT	  = RegionList::SIZE;
};

// COMMON
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct OSI_<TInitial, TRemaining...> {
	using Initial			= WrapInfo<TInitial>;
	using Remaining			= OSI_<TRemaining...>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;
	using RegionList		= Merge<typename Initial::RegionList, typename Remaining::RegionList>;

	static constexpr Long  REVERSE_DEPTH  = Max<Initial::REVERSE_DEPTH,	  Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr Short COMPO_REGIONS  =		Initial::COMPO_REGIONS  + Remaining::COMPO_REGIONS;
	static constexpr Long  COMPO_PRONGS	  =		Initial::COMPO_PRONGS   + Remaining::COMPO_PRONGS;
	static constexpr Short ORTHO_REGIONS  =		Initial::ORTHO_REGIONS  + Remaining::ORTHO_REGIONS;
	static constexpr Short ORTHO_UNITS	  =		Initial::ORTHO_UNITS    + Remaining::ORTHO_UNITS;

#ifdef HFSM2_ENABLE_SERIALIZATION
	static constexpr Long  ACTIVE_BITS	  =		Initial::ACTIVE_BITS    + Remaining::ACTIVE_BITS;
	static constexpr Long  RESUMABLE_BITS =		Initial::RESUMABLE_BITS + Remaining::RESUMABLE_BITS;
#endif
};

template <typename TInitial>
struct OSI_<TInitial> {
	using Initial			= WrapInfo<TInitial>;
	using StateList			= typename Initial::StateList;
	using RegionList		= typename Initial::RegionList;

	static constexpr Long  REVERSE_DEPTH	= Initial::REVERSE_DEPTH;
	static constexpr Short COMPO_REGIONS	= Initial::COMPO_REGIONS;
	static constexpr Long  COMPO_PRONGS		= Initial::COMPO_PRONGS;
	static constexpr Short ORTHO_REGIONS	= Initial::ORTHO_REGIONS;
	static constexpr Short ORTHO_UNITS		= Initial::ORTHO_UNITS;

#ifdef HFSM2_ENABLE_SERIALIZATION
	static constexpr Long  ACTIVE_BITS		= Initial::ACTIVE_BITS;
	static constexpr Long  RESUMABLE_BITS	= Initial::RESUMABLE_BITS;
#endif
};

template <typename THead, typename... TSubStates>
struct OI_ final {
	using Head				= THead;
	using HeadInfo			= SI_<Head>;
	using SubStates			= OSI_<TSubStates...>;
	using StateList			= Merge<typename HeadInfo::StateList, typename SubStates::StateList>;
	using RegionList		= Merge<typename HeadInfo::StateList, typename SubStates::RegionList>;

	static constexpr Short WIDTH			= sizeof...(TSubStates);
	static constexpr Long  REVERSE_DEPTH	= SubStates::REVERSE_DEPTH + 1;
	static constexpr Short COMPO_REGIONS	= SubStates::COMPO_REGIONS;
	static constexpr Long  COMPO_PRONGS		= SubStates::COMPO_PRONGS;
	static constexpr Short ORTHO_REGIONS	= SubStates::ORTHO_REGIONS + 1;
	static constexpr Short ORTHO_UNITS		= SubStates::ORTHO_UNITS + (WIDTH + 7) / 8;

#ifdef HFSM2_ENABLE_SERIALIZATION
	static constexpr Long  ACTIVE_BITS		= SubStates::ACTIVE_BITS;
	static constexpr Long  RESUMABLE_BITS	= SubStates::RESUMABLE_BITS;
#endif

	static constexpr Long  STATE_COUNT		= StateList::SIZE;
	static constexpr Short REGION_COUNT		= RegionList::SIZE;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NCompoCount
		, Long NOrthoCount
		, Long NOrthoUnits
		HFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct ArgsT final {
	using Context		= TContext;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Rank			= typename TConfig::Rank;
	using Utility		= typename TConfig::Utility;
	using RNG			= typename TConfig::RNG;
	using UP			= typename TConfig::UP;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger		= typename TConfig::LoggerInterface;
#endif

	using StateList		= TStateList;
	using RegionList	= TRegionList;

	static constexpr Long  STATE_COUNT		  = StateList::SIZE;
	static constexpr Short COMPO_REGIONS	  = NCompoCount;
	static constexpr Short ORTHO_REGIONS	  = NOrthoCount;
	static constexpr Short ORTHO_UNITS		  = NOrthoUnits;

#ifdef HFSM2_ENABLE_SERIALIZATION
	static constexpr Short SERIAL_BITS		  = NSerialBits;
#endif

	static constexpr Short SUBSTITUTION_LIMIT = NSubstitutionLimit;

#ifdef HFSM2_ENABLE_PLANS
	static constexpr Long  TASK_CAPACITY	  = NTaskCapacity;
#endif

	using Payload		= TPayload;

#ifdef HFSM2_ENABLE_SERIALIZATION
	using SerialBuffer	= StreamBufferT  <SERIAL_BITS>;
	using WriteStream	= BitWriteStreamT<SERIAL_BITS>;
	using ReadStream	= BitReadStreamT <SERIAL_BITS>;
#endif

	HFSM2_IF_STRUCTURE_REPORT(using StructureStateInfos = ArrayT<StructureStateInfo, STATE_COUNT>);
};

//------------------------------------------------------------------------------

template <StateID NStateID,
		  Short NCompoIndex,
		  Short NOrthoIndex,
		  Short NOrthoUnit>
struct I_ {
	static constexpr StateID STATE_ID	 = NStateID;
	static constexpr Short	 COMPO_INDEX = NCompoIndex;
	static constexpr Short	 ORTHO_INDEX = NOrthoIndex;
	static constexpr Short	 ORTHO_UNIT	 = NOrthoUnit;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename, typename, typename>
struct S_;

template <typename, typename, Strategy, typename, typename...>
struct C_;

template <typename, typename, Strategy, Short, typename...>
struct CS_;

template <typename, typename, typename, typename...>
struct O_;

template <typename, typename, Short, typename...>
struct OS_;

template <typename, typename>
class RR_;

//------------------------------------------------------------------------------

template <typename, typename...>
struct MaterialT;

template <typename TN, typename TA, typename TH>
struct MaterialT   <TN, TA, TH> {
	using Type = S_<TN, TA, TH>;
};

template <typename TN, typename TA, Strategy SG, 			  typename... TS>
struct MaterialT   <TN, TA, CI_<SG, void,             TS...>> {
	using Type = C_<TN, TA,     SG, StaticEmptyT<TA>, TS...>;
};

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
struct MaterialT   <TN, TA, CI_<SG, TH,	TS...>> {
	using Type = C_<TN, TA,     SG, TH,	TS...>;
};

template <typename TN, typename TA,				 typename... TS>
struct MaterialT   <TN, TA, OI_<void,			  TS...>> {
	using Type = O_<TN, TA,     StaticEmptyT<TA>, TS...>;
};

template <typename TN, typename TA, typename TH, typename... TS>
struct MaterialT   <TN, TA, OI_<TH,				  TS...>> {
	using Type = O_<TN, TA,     TH,				  TS...>;
};

template <typename TN, typename... TS>
using Material = typename MaterialT<TN, TS...>::Type;

//------------------------------------------------------------------------------

template <typename TConfig,
		  typename TApex>
struct RF_ final {
	using Context		= typename TConfig::Context;
	using Apex			= TApex;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static constexpr Long  SUBSTITUTION_LIMIT	= TConfig::SUBSTITUTION_LIMIT;

#ifdef HFSM2_ENABLE_PLANS
	static constexpr Long  TASK_CAPACITY		= TConfig::TASK_CAPACITY != INVALID_LONG ?
													  TConfig::TASK_CAPACITY : Apex::COMPO_PRONGS * 2;
#endif

	using Payload	= typename TConfig::Payload;

	static constexpr Short COMPO_REGIONS		= Apex::COMPO_REGIONS;
	static constexpr Short ORTHO_REGIONS		= Apex::ORTHO_REGIONS;
	static constexpr Short ORTHO_UNITS			= Apex::ORTHO_UNITS;

#ifdef HFSM2_ENABLE_SERIALIZATION
	static constexpr Long  ACTIVE_BITS			= Apex::ACTIVE_BITS;
	static constexpr Long  RESUMABLE_BITS		= Apex::RESUMABLE_BITS;
#endif

	using StateList		= typename Apex::StateList;
	using RegionList	= typename Apex::RegionList;

	using Args			= ArgsT<Context
							  , TConfig
							  , StateList
							  , RegionList
							  , COMPO_REGIONS
							  , ORTHO_REGIONS
							  , ORTHO_UNITS
							  HFSM2_IF_SERIALIZATION(, ACTIVE_BITS + RESUMABLE_BITS)
							  , SUBSTITUTION_LIMIT
							  HFSM2_IF_PLANS(, TASK_CAPACITY)
							  , Payload>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Instance		= RR_<TConfig, Apex>;

	using Control		= ControlT	   <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Injection		= InjectionT<Args>;

	//----------------------------------------------------------------------

	using DynamicState	= DynamicEmptyT<Args>;

	template <typename... TInjections>
	using DynamicStateT	= DB_<TInjections...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using StaticState	= StaticEmptyT<Args>;

	template <typename... TInjections>
	using StaticStateT	= SB_<TInjections...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using State			= StaticState;

	template <typename... TInjections>
	using StateT		= StaticStateT<TInjections...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger		= typename TConfig::LoggerInterface;
#endif

	//----------------------------------------------------------------------

	template <typename T>
	static constexpr bool	  contains() noexcept	{ return contains<StateList, T>();			}

	template <typename T>
	static constexpr StateID  stateId()	 noexcept	{ return index	 <StateList, T>();			}

	template <typename T>
	static constexpr RegionID regionId() noexcept	{ return (RegionID) index<RegionList, T>();	}
};

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
struct CSubMaterialT;

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
struct CSubMaterialT<TN, TA, SG, NI, TL_<TS...>> {
	using Type = CS_<TN, TA, SG, NI,	  TS...>;
};

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
using CSubMaterial = typename CSubMaterialT<TN, TA, SG, NI, TS...>::Type;

//------------------------------------------------------------------------------

template <typename>
struct InfoT;

template <typename TN, typename TA, typename TH>
struct InfoT<S_<TN, TA, TH>> {
	using Type = SI_<	TH>;
};

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
struct InfoT<C_<TN, TA, SG, TH, TS...>> {
	using Type = CI_<	SG, TH, TS...>;
};

template <typename TN, typename TA, typename TH, typename... TS>
struct InfoT<O_<TN, TA, TH, TS...>> {
	using Type = OI_<	TH, TS...>;
};

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
struct InfoT<CS_<TN, TA, SG, NI, TS...>> {
	using Type = CSI_<			 TS...>;
};

template <typename T>
using Info = typename InfoT<T>::Type;

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  Strategy TStrategy,
		  Short NIndex,
		  typename... TStates>
struct CS_ final {
	static_assert(sizeof...(TStates) >= 2, "");

	using Indices		= TIndices;
	static constexpr StateID  INITIAL_ID  = Indices::STATE_ID;
	static constexpr Short	  COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	  ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	  ORTHO_UNIT  = Indices::ORTHO_UNIT;

	static constexpr Strategy STRATEGY	  = TStrategy;

	static constexpr Short	  REGION_ID	  = COMPO_INDEX + ORTHO_INDEX;
	static constexpr Short	  PRONG_INDEX = NIndex;

	using Args			= TArgs;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	static constexpr Short	  L_PRONG	  = PRONG_INDEX;

	using LStateList	= LHalfTypes<TStates...>;
	using LMaterial		= CSubMaterial<I_<INITIAL_ID,
										  COMPO_INDEX,
										  ORTHO_INDEX,
										  ORTHO_UNIT>,
									   Args,
									   STRATEGY,
									   L_PRONG,
									   LStateList>;

	using LHalfInfo		= Info<LMaterial>;

	static constexpr Short	  R_PRONG	  = PRONG_INDEX + LStateList::SIZE;

	using RStateList	= RHalfTypes<TStates...>;
	using RMaterial		= CSubMaterial<I_<INITIAL_ID  + LHalfInfo::STATE_COUNT,
										  COMPO_INDEX + LHalfInfo::COMPO_REGIONS,
										  ORTHO_INDEX + LHalfInfo::ORTHO_REGIONS,
										  ORTHO_UNIT  + LHalfInfo::ORTHO_UNITS>,
									   Args,
									   STRATEGY,
									   R_PRONG,
									   RStateList>;

	//----------------------------------------------------------------------

#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION
	template <typename T>
	HFSM2_INLINE	   T& access()		 noexcept;

	template <typename T>
	HFSM2_INLINE const T& access() const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideRegister				 (Registry& registry,  const Parent parent) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE bool	 wideForwardEntryGuard		 (GuardControl& control, const Short prong) noexcept;
	HFSM2_INLINE bool	 wideEntryGuard				 (GuardControl& control, const Short prong) noexcept;

	HFSM2_INLINE void	 wideConstruct				 (PlanControl&  control, const Short prong) noexcept;

	HFSM2_INLINE void	 wideEnter					 (PlanControl&  control, const Short prong) noexcept;
	HFSM2_INLINE void	 wideReenter				 (PlanControl&  control, const Short prong) noexcept;

	HFSM2_INLINE Status	 wideUpdate					 (FullControl&  control, const Short prong) noexcept;

	template <typename TEvent>
	HFSM2_INLINE Status	 wideReact					 (FullControl&  control,
													  const TEvent& event,	 const Short prong) noexcept;

	HFSM2_INLINE bool	 wideForwardExitGuard		 (GuardControl& control, const Short prong) noexcept;
	HFSM2_INLINE bool	 wideExitGuard				 (GuardControl& control, const Short prong) noexcept;

	HFSM2_INLINE void	 wideExit					 (PlanControl&  control, const Short prong) noexcept;

	HFSM2_INLINE void	 wideDestruct				 (PlanControl&  control, const Short prong) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideForwardActive			 (Control& control, const Request request, const Short prong) noexcept;
	HFSM2_INLINE void	 wideForwardRequest			 (Control& control, const Request request, const Short prong) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideRequestChangeComposite	 (Control& control, const Request request					) noexcept;
	HFSM2_INLINE void	 wideRequestChangeResumable	 (Control& control, const Request request, const Short prong) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideRequestRestart			 (Control& control, const Request request					) noexcept;
	HFSM2_INLINE void	 wideRequestResume			 (Control& control, const Request request, const Short prong) noexcept;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	/*
	HFSM2_INLINE void	 wideRequestUtilize			 (Control& control, const Request request) noexcept;
	HFSM2_INLINE void	 wideRequestRandomize		 (Control& control, const Request request) noexcept;
	*/

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE UP		 wideReportChangeComposite	 (Control& control																   ) noexcept;
	HFSM2_INLINE UP		 wideReportChangeResumable	 (Control& control, const Short prong											   ) noexcept;
	HFSM2_INLINE UP		 wideReportChangeUtilitarian (Control& control																   ) noexcept;
	HFSM2_INLINE Utility wideReportChangeRandom		 (Control& control, Utility* const options, const Rank* const ranks, const Rank top) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE UP		 wideReportUtilize			 (Control& control																   ) noexcept;
	HFSM2_INLINE Rank	 wideReportRank				 (Control& control,								  Rank* const ranks				   ) noexcept;
	HFSM2_INLINE Utility wideReportRandomize		 (Control& control, Utility* const options, const Rank* const ranks, const Rank top) noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideChangeToRequested		 (PlanControl& control, const Short prong) noexcept;

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_INLINE void	 wideSaveActive				 (const Registry& registry, WriteStream& stream, const Short prong) const noexcept;
	HFSM2_INLINE void	 wideSaveResumable			 (const Registry& registry, WriteStream& stream	 				  ) const noexcept;

	HFSM2_INLINE void	 wideLoadRequested			 (      Registry& registry, ReadStream&  stream, const Short prong) const noexcept;
	HFSM2_INLINE void	 wideLoadResumable			 (      Registry& registry, ReadStream&  stream	 				  ) const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr Long NAME_COUNT = LMaterial::NAME_COUNT + RMaterial::NAME_COUNT;

	void wideGetNames(const Long parent,
					  const RegionType region,
					  const Short depth,
					  StructureStateInfos& stateInfos) const noexcept;
#endif

	//----------------------------------------------------------------------

	LMaterial lHalf;
	RMaterial rHalf;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  Strategy TStrategy,
		  Short NIndex,
		  typename TState>
struct CS_<TIndices, TArgs, TStrategy, NIndex, TState> final {
	using Indices		= TIndices;
	static constexpr StateID INITIAL_ID	= Indices::STATE_ID;
	static constexpr Short COMPO_INDEX	= Indices::COMPO_INDEX;
	static constexpr Short ORTHO_INDEX	= Indices::ORTHO_INDEX;
	static constexpr Short ORTHO_UNIT	= Indices::ORTHO_UNIT;

	static constexpr Strategy	STRATEGY	= TStrategy;

	static constexpr Short REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr Short PRONG_INDEX	= NIndex;

	using Args			= TArgs;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using State			= Material<I_<INITIAL_ID,
									  COMPO_INDEX,
									  ORTHO_INDEX,
									  ORTHO_UNIT>,
								   Args,
								   TState>;

	//----------------------------------------------------------------------

#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION
	template <typename T>
	HFSM2_INLINE	   T& access()		 noexcept	 { return state.template access<T>();	}

	template <typename T>
	HFSM2_INLINE const T& access() const noexcept	 { return state.template access<T>();	}
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideRegister				 (Registry& registry,  const Parent parent) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE bool	 wideForwardEntryGuard		 (GuardControl& control, const Short prong) noexcept;
	HFSM2_INLINE bool	 wideEntryGuard				 (GuardControl& control, const Short prong) noexcept;

	HFSM2_INLINE void	 wideConstruct				 (PlanControl&  control, const Short prong) noexcept;

	HFSM2_INLINE void	 wideEnter					 (PlanControl&  control, const Short prong) noexcept;
	HFSM2_INLINE void	 wideReenter				 (PlanControl&  control, const Short prong) noexcept;

	HFSM2_INLINE Status	 wideUpdate					 (FullControl&  control, const Short prong) noexcept;

	template <typename TEvent>
	HFSM2_INLINE Status	 wideReact					 (FullControl&  control,
													  const TEvent& event,	 const Short prong) noexcept;

	HFSM2_INLINE bool	 wideForwardExitGuard		 (GuardControl& control, const Short prong) noexcept;
	HFSM2_INLINE bool	 wideExitGuard				 (GuardControl& control, const Short prong) noexcept;

	HFSM2_INLINE void	 wideExit					 (PlanControl&  control, const Short prong) noexcept;

	HFSM2_INLINE void	 wideDestruct				 (PlanControl&  control, const Short prong) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideForwardActive			 (Control& control, const Request request, const Short prong) noexcept;
	HFSM2_INLINE void	 wideForwardRequest			 (Control& control, const Request request, const Short prong) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideRequestChangeComposite	 (Control& control, const Request request					) noexcept;
	HFSM2_INLINE void	 wideRequestChangeResumable	 (Control& control, const Request request, const Short prong) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideRequestRestart			 (Control& control, const Request request					) noexcept;
	HFSM2_INLINE void	 wideRequestResume			 (Control& control, const Request request, const Short prong) noexcept;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	/*
	HFSM2_INLINE void	 wideRequestUtilize			 (Control& control, const Request request) noexcept;
	HFSM2_INLINE void	 wideRequestRandomize		 (Control& control, const Request request) noexcept;
	*/

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE UP		 wideReportChangeComposite	 (Control& control																   ) noexcept;
	HFSM2_INLINE UP		 wideReportChangeResumable	 (Control& control, const Short prong											   ) noexcept;
	HFSM2_INLINE UP		 wideReportChangeUtilitarian (Control& control																   ) noexcept;
	HFSM2_INLINE Utility wideReportChangeRandom		 (Control& control, Utility* const options, const Rank* const ranks, const Rank top) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - -

	HFSM2_INLINE UP		 wideReportUtilize			 (Control& control																   ) noexcept;
	HFSM2_INLINE Rank	 wideReportRank				 (Control& control,								  Rank* const ranks				   ) noexcept;
	HFSM2_INLINE Utility wideReportRandomize		 (Control& control, Utility* const options, const Rank* const ranks, const Rank top) noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideChangeToRequested		 (PlanControl& control, const Short prong) noexcept;

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_INLINE void	wideSaveActive				 (const Registry& registry, WriteStream& stream, const Short prong) const noexcept;
	HFSM2_INLINE void	wideSaveResumable			 (const Registry& registry, WriteStream& stream					  ) const noexcept;

	HFSM2_INLINE void	wideLoadRequested			 (		Registry& registry, ReadStream&  stream, const Short prong) const noexcept;
	HFSM2_INLINE void	wideLoadResumable			 (		Registry& registry, ReadStream&  stream					  ) const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr Long NAME_COUNT = State::NAME_COUNT;

	void wideGetNames(const Long parent,
					  const RegionType region,
					  const Short depth,
					  StructureStateInfos& stateInfos) const noexcept;
#endif

	//----------------------------------------------------------------------

	State state;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
template <typename T>
T&
CS_<TN, TA, SG, NI, TS...>::access() noexcept {
	return contains<typename LHalfInfo::StateList, T>() ?
		lHalf.template access<T>() :
		rHalf.template access<T>();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
template <typename T>
const T&
CS_<TN, TA, SG, NI, TS...>::access() const noexcept {
	return contains<typename LHalfInfo::StateList, T>() ?
		lHalf.template access<T>() :
		rHalf.template access<T>();
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideRegister(Registry& registry,
										 const Parent parent) noexcept
{
	lHalf.wideRegister(registry, Parent{parent.forkId, L_PRONG});
	rHalf.wideRegister(registry, Parent{parent.forkId, R_PRONG});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
bool
CS_<TN, TA, SG, NI, TS...>::wideForwardEntryGuard(GuardControl& control,
												  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return lHalf.wideForwardEntryGuard(control, prong);
	else
		return rHalf.wideForwardEntryGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
bool
CS_<TN, TA, SG, NI, TS...>::wideEntryGuard(GuardControl& control,
										   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return lHalf.wideEntryGuard(control, prong);
	else
		return rHalf.wideEntryGuard(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideConstruct(PlanControl& control,
										  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideConstruct(control, prong);
	else
		rHalf.wideConstruct(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideEnter(PlanControl& control,
									  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideEnter(control, prong);
	else
		rHalf.wideEnter(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideReenter(PlanControl& control,
										const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideReenter(control, prong);
	else
		rHalf.wideReenter(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
Status
CS_<TN, TA, SG, NI, TS...>::wideUpdate(FullControl& control,
									   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		lHalf.wideUpdate(control, prong) :
		rHalf.wideUpdate(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
template <typename TEvent>
Status
CS_<TN, TA, SG, NI, TS...>::wideReact(FullControl& control,
									  const TEvent& event,
									  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		lHalf.wideReact(control, event, prong) :
		rHalf.wideReact(control, event, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
bool
CS_<TN, TA, SG, NI, TS...>::wideForwardExitGuard(GuardControl& control,
												 const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return lHalf.wideForwardExitGuard(control, prong);
	else
		return rHalf.wideForwardExitGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
bool
CS_<TN, TA, SG, NI, TS...>::wideExitGuard(GuardControl& control,
										  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return lHalf.wideExitGuard(control, prong);
	else
		return rHalf.wideExitGuard(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideExit(PlanControl& control,
									 const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideExit(control, prong);
	else
		rHalf.wideExit(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideDestruct(PlanControl& control,
										 const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideDestruct(control, prong);
	else
		rHalf.wideDestruct(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideForwardActive(Control& control,
											  const Request request,
											  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideForwardActive(control, request, prong);
	else
		rHalf.wideForwardActive(control, request, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideForwardRequest(Control& control,
											   const Request request,
											   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideForwardRequest(control, request, prong);
	else
		rHalf.wideForwardRequest(control, request, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideRequestChangeComposite(Control& control,
													   const Request request) noexcept
{
	lHalf.wideRequestChangeComposite(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideRequestChangeResumable(Control& control,
													   const Request request,
													   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideRequestChangeResumable(control, request, prong);
	else
		rHalf.wideRequestChangeResumable(control, request, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideRequestRestart(Control& control,
											   const Request request) noexcept
{
	lHalf.wideRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideRequestResume(Control& control,
											  const Request request,
											  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideRequestResume(control, request, prong);
	else
		rHalf.wideRequestResume(control, request, prong);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
typename TA::UP
CS_<TN, TA, SG, NI, TS...>::wideReportUtilize(Control& control) noexcept {
	const UP l = lHalf.wideReportUtilize(control);
	const UP r = rHalf.wideReportUtilize(control);

	return l.utility >= r.utility ?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
typename TA::Rank
CS_<TN, TA, SG, NI, TS...>::wideReportRank(Control& control,
										   Rank* const ranks) noexcept
{
	HFSM2_ASSERT(ranks);

	const Rank l = lHalf.wideReportRank(control, ranks);
	const Rank r = rHalf.wideReportRank(control, ranks + LStateList::SIZE);

	return l > r?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
typename TA::Utility
CS_<TN, TA, SG, NI, TS...>::wideReportRandomize(Control& control,
												Utility* const options,
												const Rank* const ranks,
												const Rank top) noexcept
{
	HFSM2_ASSERT(options && ranks);

	const Utility l = lHalf.wideReportRandomize(control, options,					 ranks,					   top);
	const Utility r = rHalf.wideReportRandomize(control, options + LStateList::SIZE, ranks + LStateList::SIZE, top);

	return { l + r };
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
typename TA::UP
CS_<TN, TA, SG, NI, TS...>::wideReportChangeComposite(Control& control) noexcept {
	return lHalf.wideReportChangeComposite(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
typename TA::UP
CS_<TN, TA, SG, NI, TS...>::wideReportChangeResumable(Control& control,
													  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return lHalf.wideReportChangeResumable(control, prong);
	else
		return rHalf.wideReportChangeResumable(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
typename TA::UP
CS_<TN, TA, SG, NI, TS...>::wideReportChangeUtilitarian(Control& control) noexcept {
	const UP l = lHalf.wideReportChangeUtilitarian(control);
	const UP r = rHalf.wideReportChangeUtilitarian(control);

	return l.utility >= r.utility ?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
typename TA::Utility
CS_<TN, TA, SG, NI, TS...>::wideReportChangeRandom(Control& control,
												   Utility* const options,
												   const Rank* const ranks,
												   const Rank top) noexcept
{
	HFSM2_ASSERT(options && ranks);

	const Utility l = lHalf.wideReportChangeRandom(control, options,					ranks,					  top);
	const Utility r = rHalf.wideReportChangeRandom(control, options + LStateList::SIZE, ranks + LStateList::SIZE, top);

	return { l + r };
}

#endif

//------------------------------------------------------------------------------
// COMMON

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideChangeToRequested(PlanControl& control,
												  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideChangeToRequested(control, prong);
	else
		rHalf.wideChangeToRequested(control, prong);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideSaveActive(const Registry& registry,
										   WriteStream& stream,
										   const Short prong) const noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG) {
		lHalf.wideSaveActive   (registry, stream, prong);
		rHalf.wideSaveResumable(registry, stream);
	} else {
		lHalf.wideSaveResumable(registry, stream);
		rHalf.wideSaveActive   (registry, stream, prong);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideSaveResumable(const Registry& registry,
											  WriteStream& stream) const noexcept
{
	lHalf.wideSaveResumable(registry, stream);
	rHalf.wideSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideLoadRequested(Registry& registry,
											  ReadStream& stream,
											  const Short prong) const noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG) {
		lHalf.wideLoadRequested(registry, stream, prong);
		rHalf.wideLoadResumable(registry, stream);
	} else {
		lHalf.wideLoadResumable(registry, stream);
		rHalf.wideLoadRequested(registry, stream, prong);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideLoadResumable(Registry& registry,
											  ReadStream& stream) const noexcept
{
	lHalf.wideLoadResumable(registry, stream);
	rHalf.wideLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideGetNames(const Long parent,
										 const RegionType /*region*/,
										 const Short depth,
										 StructureStateInfos& _stateInfos) const noexcept
{
	lHalf.wideGetNames(parent, StructureStateInfo::RegionType::COMPOSITE, depth, _stateInfos);
	rHalf.wideGetNames(parent, StructureStateInfo::RegionType::COMPOSITE, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideRegister(Registry& registry,
									 const Parent parent) noexcept
{
	state.deepRegister(registry, Parent{parent.forkId, PRONG_INDEX});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
bool
CS_<TN, TA, SG, NI, T>::wideForwardEntryGuard(GuardControl& control,
											  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
bool
CS_<TN, TA, SG, NI, T>::wideEntryGuard(GuardControl& control,
									   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideConstruct(PlanControl& control,
									  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepConstruct(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideEnter(PlanControl& control,
								  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideReenter(PlanControl& control,
									const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
Status
CS_<TN, TA, SG, NI, T>::wideUpdate(FullControl& control,
								   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
template <typename TEvent>
Status
CS_<TN, TA, SG, NI, T>::wideReact(FullControl& control,
								  const TEvent& event,
								  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepReact(control, event);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
bool
CS_<TN, TA, SG, NI, T>::wideForwardExitGuard(GuardControl& control,
											 const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
bool
CS_<TN, TA, SG, NI, T>::wideExitGuard(GuardControl& control,
									  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideExit(PlanControl& control,
								 const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideDestruct(PlanControl& control,
									 const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepDestruct(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideForwardActive(Control& control,
										  const Request request,
										  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepForwardActive(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideForwardRequest(Control& control,
										   const Request request,
										   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepForwardRequest(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideRequestChangeComposite(Control& control,
												   const Request request) noexcept
{
	state.deepRequestChange(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideRequestChangeResumable(Control& control,
												   const Request request,
												   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepRequestChange(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideRequestRestart(Control& control,
										   const Request request) noexcept
{
	state.deepRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideRequestResume(Control& control,
										  const Request request,
										  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepRequestResume(control, request);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
typename TA::UP
CS_<TN, TA, SG, NI, T>::wideReportUtilize(Control& control) noexcept {
	return state.deepReportUtilize(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
typename TA::Rank
CS_<TN, TA, SG, NI, T>::wideReportRank(Control& control,
									   Rank* const ranks) noexcept
{
	HFSM2_ASSERT(ranks);

	*ranks = state.deepReportRank(control);

	return *ranks;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
typename TA::Utility
CS_<TN, TA, SG, NI, T>::wideReportRandomize(Control& control,
											Utility* const options,
											const Rank* const ranks,
											const Rank top) noexcept
{
	HFSM2_ASSERT(options && ranks);

	*options = (*ranks == top) ?
		state.deepReportRandomize(control) : Utility{0};

	return *options;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
typename TA::UP
CS_<TN, TA, SG, NI, T>::wideReportChangeComposite(Control& control) noexcept {
	return state.deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
typename TA::UP
CS_<TN, TA, SG, NI, T>::wideReportChangeResumable(Control& control,
												  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
typename TA::UP
CS_<TN, TA, SG, NI, T>::wideReportChangeUtilitarian(Control& control) noexcept {
	return state.deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
typename TA::Utility
CS_<TN, TA, SG, NI, T>::wideReportChangeRandom(Control& control,
											   Utility* const options,
											   const Rank* const ranks,
											   const Rank top) noexcept
{
	HFSM2_ASSERT(options && ranks);

	*options = (*ranks == top) ?
		state.deepReportChange(control).utility : Utility{0};

	return *options;
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideChangeToRequested(PlanControl& control,
											  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideSaveActive(const Registry& registry,
									   WriteStream& stream,
									   const Short HFSM2_IF_ASSERT(prong)) const noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideSaveResumable(const Registry& registry,
										  WriteStream& stream) const noexcept
{
	state.deepSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideLoadRequested(Registry& registry,
										  ReadStream& stream,
										  const Short HFSM2_IF_ASSERT(prong)) const noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideLoadResumable(Registry& registry,
										  ReadStream& stream) const noexcept
{
	state.deepLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideGetNames(const Long parent,
									 const RegionType /*region*/,
									 const Short depth,
									 StructureStateInfos& _stateInfos) const noexcept
{
	state.deepGetNames(parent, StructureStateInfo::RegionType::COMPOSITE, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  Strategy TStrategy,
		  typename THead,
		  typename... TSubStates>
struct C_ final {
	using Indices		= TIndices;
	static constexpr StateID  HEAD_ID	  = Indices::STATE_ID;
	static constexpr Short	  COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	  ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	  ORTHO_UNIT  = Indices::ORTHO_UNIT;

	static constexpr Strategy STRATEGY	  = TStrategy;

	static constexpr Short	  REGION_ID	  = COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID	  COMPO_ID	  = COMPO_INDEX + 1;

	using Args			= TArgs;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;

	using Control		= ControlT<Args>;
	using ScopedOrigin	= typename Control::Origin;

	using PlanControl	= PlanControlT<Args>;
	using ScopedRegion	= typename PlanControl::Region;

	using FullControl	= FullControlT<Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;

	using Head			= THead;

	using HeadState		= S_<Indices, Args, Head>;

	using SubStates		= CS_<I_<HEAD_ID + 1,
								 COMPO_INDEX + 1,
								 ORTHO_INDEX,
								 ORTHO_UNIT>,
							  Args,
							  STRATEGY,
							  0,
							  TSubStates...>;

	using Info			= CI_<STRATEGY, Head, TSubStates...>;

#ifdef HFSM2_ENABLE_SERIALIZATION
	static constexpr Short WIDTH		  = Info::WIDTH;
	static constexpr Short WIDTH_BITS	  = Info::WIDTH_BITS;
#endif

	static constexpr Short REGION_SIZE	  = Info::STATE_COUNT;

	//----------------------------------------------------------------------

#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION
	template <typename T>
	struct Accessor {
		HFSM2_INLINE static		  T&	get(	  C_& c) noexcept				{ return c._subStates.template access<T>();		}
		HFSM2_INLINE static const T&	get(const C_& c) noexcept				{ return c._subStates.template access<T>();		}
	};

	template <>
	struct Accessor<Head> {
		HFSM2_INLINE static		  Head& get(	  C_& c) noexcept				{ return c._headState._headBox.get();			}
		HFSM2_INLINE static const Head& get(const C_& c) noexcept				{ return c._headState._headBox.get();			}
	};

	template <typename T>
	HFSM2_INLINE	   T&	access()	   noexcept								{ return Accessor<T>::get(*this);				}

	template <typename T>
	HFSM2_INLINE const T&	access() const noexcept								{ return Accessor<T>::get(*this);				}
#endif

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION
	HFSM2_INLINE Short	 compoRequested				   (const Registry& registry) const noexcept	{ return registry.compoRequested[COMPO_INDEX];	}
	HFSM2_INLINE Short&	 compoRequested				   (	  Registry& registry) const noexcept	{ return registry.compoRequested[COMPO_INDEX];	}

	HFSM2_INLINE Short	 compoActive				   (const Registry& registry) const noexcept	{ return registry.compoActive	[COMPO_INDEX];	}
	HFSM2_INLINE Short&	 compoActive				   (	  Registry& registry) const noexcept	{ return registry.compoActive	[COMPO_INDEX];	}

	HFSM2_INLINE Short	 compoResumable				   (const Registry& registry) const noexcept	{ return registry.compoResumable[COMPO_INDEX];	}
	HFSM2_INLINE Short&	 compoResumable				   (	  Registry& registry) const noexcept	{ return registry.compoResumable[COMPO_INDEX];	}
#endif

	HFSM2_INLINE Short&	 compoRequested				   (Control& control) const noexcept	{ return control._registry.compoRequested[COMPO_INDEX];	}
	HFSM2_INLINE Short&	 compoActive				   (Control& control) const noexcept	{ return control._registry.compoActive	 [COMPO_INDEX];	}
	HFSM2_INLINE Short&	 compoResumable				   (Control& control) const noexcept	{ return control._registry.compoResumable[COMPO_INDEX];	}

	HFSM2_INLINE bool	 compoRemain				   (Control& control) const noexcept	{ return control._registry.compoRemains.template get<COMPO_INDEX>(); }

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE Short	 resolveRandom				   (Control& control,
													    const Utility(& options)[Info::WIDTH], const Utility sum,
													    const Rank	 (& ranks)  [Info::WIDTH], const Rank	 top) const noexcept;
#endif

	HFSM2_INLINE void	 deepRegister				   (Registry& registry, const Parent parent) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE bool	 deepForwardEntryGuard		   (GuardControl& control) noexcept;
	HFSM2_INLINE bool	 deepEntryGuard				   (GuardControl& control) noexcept;

	HFSM2_INLINE void	 deepConstruct				   (PlanControl&  control) noexcept;

	HFSM2_INLINE void	 deepEnter					   (PlanControl&  control) noexcept;
	HFSM2_INLINE void	 deepReenter				   (PlanControl&  control) noexcept;

	HFSM2_INLINE Status	 deepUpdate					   (FullControl&  control) noexcept;

	template <typename TEvent>
	HFSM2_INLINE Status	 deepReact					   (FullControl&  control,
														const TEvent& event)   noexcept;

	HFSM2_INLINE bool	 deepForwardExitGuard		   (GuardControl& control) noexcept;
	HFSM2_INLINE bool	 deepExitGuard				   (GuardControl& control) noexcept;

	HFSM2_INLINE void	 deepExit					   (PlanControl&  control) noexcept;

	HFSM2_INLINE void	 deepDestruct				   (PlanControl&  control) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 deepForwardActive			   (Control& control, const Request request) noexcept;
	HFSM2_INLINE void	 deepForwardRequest			   (Control& control, const Request request) noexcept;

	HFSM2_INLINE void	 deepRequest				   (Control& control, const Request request) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION

	template <Strategy = STRATEGY>
	HFSM2_INLINE void	 deepRequestChange			   (Control& control, const Request request, const Short = INVALID_SHORT) noexcept;

	template <>
	HFSM2_INLINE void	 deepRequestChange<Composite>  (Control& control, const Request request, const Short) noexcept	{ deepRequestChangeComposite  (control, request); }

	template <>
	HFSM2_INLINE void	 deepRequestChange<Resumable>  (Control& control, const Request request, const Short) noexcept	{ deepRequestChangeResumable  (control, request); }

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	template <>
	HFSM2_INLINE void	 deepRequestChange<Utilitarian>(Control& control, const Request request, const Short) noexcept	{ deepRequestChangeUtilitarian(control, request); }

	template <>
	HFSM2_INLINE void	 deepRequestChange<RandomUtil> (Control& control, const Request request, const Short) noexcept	{ deepRequestChangeRandom	  (control, request); }

#endif

#else

	HFSM2_INLINE void	 deepRequestChange			   (Control& control, const Request request) noexcept;

#endif

	HFSM2_INLINE void	 deepRequestChangeComposite	   (Control& control, const Request request) noexcept;
	HFSM2_INLINE void	 deepRequestChangeResumable	   (Control& control, const Request request) noexcept;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void	 deepRequestChangeUtilitarian  (Control& control, const Request request) noexcept;
	HFSM2_INLINE void	 deepRequestChangeRandom	   (Control& control, const Request request) noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 deepRequestRestart			   (Control& control, const Request request) noexcept;
	HFSM2_INLINE void	 deepRequestResume			   (Control& control, const Request request) noexcept;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void	 deepRequestUtilize			   (Control& control, const Request request) noexcept;
	HFSM2_INLINE void	 deepRequestRandomize		   (Control& control, const Request request) noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY
#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION

	template <Strategy = STRATEGY>
	HFSM2_INLINE UP		 deepReportChange			   (Control& control) noexcept;

	template <>
	HFSM2_INLINE UP		 deepReportChange<Composite>   (Control& control) noexcept	{ return deepReportChangeComposite  (control); }

	template <>
	HFSM2_INLINE UP		 deepReportChange<Resumable>   (Control& control) noexcept	{ return deepReportChangeResumable  (control); }

	template <>
	HFSM2_INLINE UP		 deepReportChange<Utilitarian> (Control& control) noexcept	{ return deepReportChangeUtilitarian(control); }

	template <>
	HFSM2_INLINE UP		 deepReportChange<RandomUtil>  (Control& control) noexcept	{ return deepReportChangeRandom		(control); }

#else

	HFSM2_INLINE UP		 deepReportChange			   (Control& control) noexcept;

#endif

	HFSM2_INLINE UP		 deepReportChangeComposite	   (Control& control) noexcept;
	HFSM2_INLINE UP		 deepReportChangeResumable	   (Control& control) noexcept;
	HFSM2_INLINE UP		 deepReportChangeUtilitarian   (Control& control) noexcept;
	HFSM2_INLINE UP		 deepReportChangeRandom		   (Control& control) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE UP		 deepReportUtilize			   (Control& control) noexcept;
	HFSM2_INLINE Rank	 deepReportRank				   (Control& control) noexcept;
	HFSM2_INLINE Utility deepReportRandomize		   (Control& control) noexcept;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 deepChangeToRequested		   (PlanControl& control) noexcept;

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_INLINE void	 deepSaveActive				   (const Registry& registry, WriteStream& stream) const noexcept;
	HFSM2_INLINE void	 deepSaveResumable			   (const Registry& registry, WriteStream& stream) const noexcept;

	HFSM2_INLINE void	 deepLoadRequested			   (	  Registry& registry, ReadStream&  stream) const noexcept;
	HFSM2_INLINE void	 deepLoadResumable			   (	  Registry& registry, ReadStream&  stream) const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr Long NAME_COUNT = HeadState::NAME_COUNT + SubStates::NAME_COUNT;

	void deepGetNames(const Long parent,
					  const RegionType region,
					  const Short depth,
					  StructureStateInfos& stateInfos) const noexcept;
#endif

	//----------------------------------------------------------------------

	HeadState _headState;
	SubStates _subStates;

	HFSM2_IF_DEBUG(static constexpr Info _info = Info{});
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
Short
C_<TN, TA, SG, TH, TS...>::resolveRandom(Control& control,
										 const Utility(& options)[Info::WIDTH],
										 const Utility sum,
										 const Rank(& ranks)[Info::WIDTH],
										 const Rank top) const noexcept
{
	const Utility random = control._rng.next();
	HFSM2_ASSERT(0.0f <= random && random < 1.0f);

	Utility cursor = random * sum;

	for (Short i = 0; i < count<Short>(ranks); ++i)
		if (ranks[i] == top) {
			HFSM2_ASSERT(options[i] >= 0.0f);

			if (cursor >= options[i])
				cursor -= options[i];
			else {
				HFSM2_LOG_RANDOM_RESOLUTION(control.context(), HEAD_ID, i, random);

				return i;
			}
		}

	HFSM2_BREAK();
	return INVALID_SHORT;
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRegister(Registry& registry,
										const Parent parent) noexcept
{
	registry.compoParents[COMPO_INDEX] = parent;

	_headState.deepRegister(registry, parent);
	_subStates.wideRegister(registry, Parent{COMPO_ID});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
bool
C_<TN, TA, SG, TH, TS...>::deepForwardEntryGuard(GuardControl& control) noexcept {
	const Short active	  = compoActive   (control);
	const Short requested = compoRequested(control);

	HFSM2_ASSERT(active != INVALID_SHORT);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested == INVALID_SHORT)
		return _subStates.wideForwardEntryGuard(control, active);
	else
		return _subStates.wideEntryGuard	   (control, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
bool
C_<TN, TA, SG, TH, TS...>::deepEntryGuard(GuardControl& control) noexcept {
	const Short requested = compoRequested(control);
	HFSM2_ASSERT(requested != INVALID_SHORT);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepEntryGuard(control) ||
		   _subStates.wideEntryGuard(control, requested);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepConstruct(PlanControl& control) noexcept {
	Short& active	 = compoActive   (control);
	Short& resumable = compoResumable(control);
	Short& requested = compoRequested(control);

	HFSM2_ASSERT(active	   == INVALID_SHORT);
	HFSM2_ASSERT(requested != INVALID_SHORT);

	active	  = requested;

	if (requested == resumable)
		resumable = INVALID_SHORT;

	requested = INVALID_SHORT;

	_headState.deepConstruct(control);
	_subStates.wideConstruct(control, active);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepEnter(PlanControl& control) noexcept {
	const Short& active = compoActive(control);
	HFSM2_ASSERT(active != INVALID_SHORT);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter(control);
	_subStates.wideEnter(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepReenter(PlanControl& control) noexcept {
	Short& active	 = compoActive   (control);
	Short& resumable = compoResumable(control);
	Short& requested = compoRequested(control);

	HFSM2_ASSERT(active	   != INVALID_SHORT &&
				 requested != INVALID_SHORT);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepReenter(control);

	if (active == requested)
		_subStates.wideReenter(control, active);
	else {
		_subStates.wideExit	  (control, active);

		active	  = requested;

		if (requested == resumable)
			resumable = INVALID_SHORT;

		_subStates.wideEnter  (control, active);
	}

	requested = INVALID_SHORT;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
Status
C_<TN, TA, SG, TH, TS...>::deepUpdate(FullControl& control) noexcept {
	const Short active = compoActive(control);
	HFSM2_ASSERT(active != INVALID_SHORT);

	ScopedRegion outer{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const Status headStatus = _headState.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates.wideUpdate(control, active);

		return headStatus;
	} else {
		const Status subStatus = _subStates.wideUpdate(control, active);

		if (subStatus.outerTransition)
			return Status{Status::Result::NONE, true};

		ScopedRegion inner{control, REGION_ID, HEAD_ID, REGION_SIZE};

	#ifdef HFSM2_ENABLE_PLANS
		return subStatus && control._planData.planExists.template get<REGION_ID>() ?
			control.updatePlan(_headState, subStatus) : subStatus;
	#else
		return subStatus;
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
template <typename TEvent>
Status
C_<TN, TA, SG, TH, TS...>::deepReact(FullControl& control,
									 const TEvent& event) noexcept
{
	const Short active = compoActive(control);
	HFSM2_ASSERT(active != INVALID_SHORT);

	ScopedRegion outer{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const Status headStatus = _headState.deepReact(control, event)) {
		ControlLock lock{control};

		_subStates.wideReact(control, event, active);

		return headStatus;
	} else {
		const Status subStatus = _subStates.wideReact(control, event, active);

		if (subStatus.outerTransition)
			return subStatus;

		ScopedRegion inner{control, REGION_ID, HEAD_ID, REGION_SIZE};

	#ifdef HFSM2_ENABLE_PLANS
		return subStatus && control._planData.planExists.template get<REGION_ID>() ?
			control.updatePlan(_headState, subStatus) : subStatus;
	#else
		return subStatus;
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
bool
C_<TN, TA, SG, TH, TS...>::deepForwardExitGuard(GuardControl& control) noexcept {
	const Short active = compoActive(control);
	HFSM2_ASSERT(active != INVALID_SHORT);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (compoRequested(control) == INVALID_SHORT)
		return _subStates.wideForwardExitGuard(control, active);
	else
		return _subStates.wideExitGuard		  (control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
bool
C_<TN, TA, SG, TH, TS...>::deepExitGuard(GuardControl& control) noexcept {
	const Short active = compoActive(control);
	HFSM2_ASSERT(active != INVALID_SHORT);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepExitGuard(control) ||
		   _subStates.wideExitGuard(control, active);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepExit(PlanControl& control) noexcept {
	Short& active = compoActive(control);
	HFSM2_ASSERT(active != INVALID_SHORT);

	_subStates.wideExit(control, active);
	_headState.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepDestruct(PlanControl& control) noexcept {
	Short& active	 = compoActive   (control);
	Short& resumable = compoResumable(control);

	HFSM2_ASSERT(active != INVALID_SHORT);

	_subStates.wideDestruct(control, active);
	_headState.deepDestruct(control);

	resumable = active;
	active	  = INVALID_SHORT;

#ifdef HFSM2_ENABLE_PLANS
	auto plan = control.plan(REGION_ID);
	plan.clear();
#endif
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepForwardActive(Control& control,
											 const Request request) noexcept
{
	HFSM2_ASSERT(control._registry.isActive(HEAD_ID));

	const Short requested = compoRequested(control);

	if (requested == INVALID_SHORT) {
		const Short active = compoActive(control);

		_subStates.wideForwardActive (control, request, active);
	} else
		_subStates.wideForwardRequest(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepForwardRequest(Control& control,
											  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const Short requested = compoRequested(control);

	if (requested != INVALID_SHORT)
		_subStates.wideForwardRequest(control, request, requested);
	else
		deepRequest					 (control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequest(Control& control,
									   const Request request) noexcept
{
	switch (request.type) {
	case TransitionType::CHANGE:
		deepRequestChange	(control, request);
		break;

	case TransitionType::RESTART:
		deepRequestRestart	(control, request);
		break;

	case TransitionType::RESUME:
		deepRequestResume	(control, request);
		break;

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	case TransitionType::UTILIZE:
		deepRequestUtilize	(control, request);
		break;

	case TransitionType::RANDOMIZE:
		deepRequestRandomize(control, request);
		break;

#endif

	default:
		HFSM2_BREAK();
	}
}

//------------------------------------------------------------------------------

#ifndef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequestChange(Control& control,
											 const Request request) noexcept
{
	switch (STRATEGY) {
	case Strategy::Composite:
		deepRequestChangeComposite  (control, request);
		break;

	case Strategy::Resumable:
		deepRequestChangeResumable  (control, request);
		break;

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	case Strategy::Utilitarian:
		deepRequestChangeUtilitarian(control, request);
		break;

	case Strategy::RandomUtil:
		deepRequestChangeRandom		(control, request);
		break;

#endif

	default:
		HFSM2_BREAK();
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequestChangeComposite(Control& control,
													  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Short& requested = compoRequested(control);

	requested = 0;

	_subStates.wideRequestChangeComposite(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequestChangeResumable(Control& control,
													  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const Short  resumable = compoResumable(control);
		  Short& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT) ?
		resumable : 0;

	_subStates.wideRequestChangeResumable(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequestChangeUtilitarian(Control& control,
														const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const UP s = _subStates.wideReportChangeUtilitarian(control);
	HFSM2_ASSERT(s.prong != INVALID_SHORT);

	Short& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequestChangeRandom(Control& control,
												   const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Rank ranks[Info::WIDTH];
	Rank top = _subStates.wideReportRank(control, ranks);

	Utility options[Info::WIDTH];
	const UP sum = _subStates.wideReportChangeRandom(control, options, ranks, top);

	Short& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum.utility, ranks, top);
	HFSM2_ASSERT(requested < Info::WIDTH);
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequestRestart(Control& control,
											  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Short& requested = compoRequested(control);

	requested = 0;

	_subStates.wideRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequestResume(Control& control,
											 const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const Short  resumable = compoResumable(control);
		  Short& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT) ?
		resumable : 0;

	_subStates.wideRequestResume(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequestUtilize(Control& control,
											  const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const UP s = _subStates.wideReportUtilize(control);

	Short& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequestRandomize(Control& control,
												const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Rank ranks[Info::WIDTH];
	Rank top = _subStates.wideReportRank(control, ranks);

	Utility options[Info::WIDTH];
	const Utility sum = _subStates.wideReportRandomize(control, options, ranks, top);

	Short& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum, ranks, top);
	HFSM2_ASSERT(requested < Info::WIDTH);
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_UTILITY_THEORY
#ifndef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChange(Control& control) noexcept {
	switch (STRATEGY) {
	case Strategy::Composite:
		return deepReportChangeComposite  (control);

	case Strategy::Resumable:
		return deepReportChangeResumable  (control);

	case Strategy::Utilitarian:
		return deepReportChangeUtilitarian(control);

	case Strategy::RandomUtil:
		return deepReportChangeRandom	  (control);

	default:
		HFSM2_BREAK();
		return {};
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChangeComposite(Control& control) noexcept {
	Short& requested = compoRequested(control);
	requested = 0;

	const UP h = _headState.deepReportChange(control);
	const UP s = _subStates.wideReportChangeComposite(control);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChangeResumable(Control& control) noexcept {
	const Short  resumable = compoResumable(control);
		  Short& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT) ?
		resumable : 0;

	const UP h = _headState.deepReportChange(control);
	const UP s = _subStates.wideReportChangeResumable(control, requested);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChangeUtilitarian(Control& control) noexcept {
	const UP h = _headState.deepReportChange(control);
	const UP s = _subStates.wideReportChangeUtilitarian(control);

	Short& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChangeRandom(Control& control) noexcept {
	const UP h = _headState.deepReportChange(control);

	Rank ranks[Info::WIDTH];
	Rank top = _subStates.wideReportRank(control, ranks);

	Utility options[Info::WIDTH];
	const UP sum = _subStates.wideReportChangeRandom(control, options, ranks, top);

	Short& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum.utility, ranks, top);
	HFSM2_ASSERT(requested < Info::WIDTH);

	return {
		h.utility * options[requested],
		h.prong
	};
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportUtilize(Control& control) noexcept {
	const UP h = _headState.deepReportUtilize(control);
	const UP s = _subStates.wideReportUtilize(control);

	Short& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
typename TA::Rank
C_<TN, TA, SG, TH, TS...>::deepReportRank(Control& control) noexcept {
	return _headState.wrapRank(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
typename TA::Utility
C_<TN, TA, SG, TH, TS...>::deepReportRandomize(Control& control) noexcept {
	const Utility h = _headState.wrapUtility(control);

	Rank ranks[Info::WIDTH];
	Rank top = _subStates.wideReportRank(control, ranks);

	Utility options[Info::WIDTH];
	const Utility sum = _subStates.wideReportRandomize(control, options, ranks, top);

	Short& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum, ranks, top);
	HFSM2_ASSERT(requested < Info::WIDTH);

	return h * options[requested];
}

#endif

//------------------------------------------------------------------------------
// COMMON

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepChangeToRequested(PlanControl& control) noexcept {
	Short& active	 = compoActive	 (control);
	Short& resumable = compoResumable(control);
	Short& requested = compoRequested(control);

	HFSM2_ASSERT(active != INVALID_SHORT);

	if (requested == INVALID_SHORT)
		_subStates.wideChangeToRequested(control, active);
	else if (requested != active) {
		_subStates.wideExit		(control, active);
		_subStates.wideDestruct	(control, active);

		resumable = active;
		active	  = requested;
		requested = INVALID_SHORT;

		_subStates.wideConstruct(control, active);
		_subStates.wideEnter	(control, active);
	} else if (compoRemain(control)) {
		_subStates.wideExit		(control, active);
		_subStates.wideDestruct	(control, active);

		requested = INVALID_SHORT;

		_subStates.wideConstruct(control, active);
		_subStates.wideEnter	(control, active);
	} else {
		requested = INVALID_SHORT;

		// reconstruction done in S_::reenter()
		_subStates.wideReenter	(control, active);
	}
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepSaveActive(const Registry& registry,
										  WriteStream& stream) const noexcept
{
	const Short active	  = compoActive   (registry);
	const Short resumable = compoResumable(registry);

	stream.template write<WIDTH_BITS>(active);

	if (resumable != INVALID_SHORT) {
		stream.template write<1>(1);
		stream.template write<WIDTH_BITS>(resumable);
	} else
		stream.template write<1>(0);

	_subStates.wideSaveActive(registry,stream, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepSaveResumable(const Registry& registry,
											 WriteStream& stream) const noexcept
{
	const Short resumable = compoResumable(registry);

	if (resumable != INVALID_SHORT) {
		stream.template write<1>(1);
		stream.template write<WIDTH_BITS>(resumable);
	} else
		stream.template write<1>(0);

	_subStates.wideSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepLoadRequested(Registry& registry,
											 ReadStream& stream) const noexcept
{
	Short& resumable = compoResumable(registry);
	Short& requested = compoRequested(registry);

	requested = stream.template read<WIDTH_BITS>();
	HFSM2_ASSERT(requested < WIDTH);

	if (stream.template read<1>()) {
		resumable = stream.template read<WIDTH_BITS>();
		HFSM2_ASSERT(resumable < WIDTH);
	} else
		resumable = INVALID_SHORT;

	_subStates.wideLoadRequested(registry, stream, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepLoadResumable(Registry& registry,
											 ReadStream& stream) const noexcept
{
	Short& resumable = compoResumable(registry);

	if (stream.template read<1>()) {
		resumable = stream.template read<WIDTH_BITS>();
		HFSM2_ASSERT(resumable < WIDTH);
	} else
		resumable = INVALID_SHORT;

	_subStates.wideLoadResumable(registry, stream);
}

#endif

// COMMON
//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepGetNames(const Long parent,
										const RegionType regionType,
										const Short depth,
										StructureStateInfos& _stateInfos) const noexcept
{
	_headState.deepGetNames(parent,					 regionType,								depth,	   _stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, StructureStateInfo::RegionType::COMPOSITE, depth + 1, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename, typename, Short, typename...>
struct OS_;

//------------------------------------------------------------------------------

template <typename TIndices,
		  typename TArgs,
		  Short NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct OS_<TIndices, TArgs, NIndex, TInitial, TRemaining...> final {
	using Indices		= TIndices;
	static constexpr StateID INITIAL_ID	 = Indices::STATE_ID;
	static constexpr Short	 COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	 ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	 ORTHO_UNIT	 = Indices::ORTHO_UNIT;

	static constexpr Short	 PRONG_INDEX = NIndex;

	using Args			= TArgs;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;
	using OrthoForks	= typename Registry::OrthoForks;
	using ProngBits		= typename OrthoForks::Bits;
	using ProngCBits= typename OrthoForks::CBits;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Initial		= Material<I_<INITIAL_ID,
									  COMPO_INDEX,
									  ORTHO_INDEX,
									  ORTHO_UNIT>,
								   Args,
								   TInitial>;

	using InitialInfo	= WrapInfo<TInitial>;
	using InitialStates	= typename InitialInfo::StateList;

	using Remaining		= OS_<I_<INITIAL_ID  + InitialInfo::STATE_COUNT,
								 COMPO_INDEX + InitialInfo::COMPO_REGIONS,
								 ORTHO_INDEX + InitialInfo::ORTHO_REGIONS,
								 ORTHO_UNIT  + InitialInfo::ORTHO_UNITS>,
							  Args,
							  PRONG_INDEX + 1,
							  TRemaining...>;

	using Info	= OSI_<TInitial, TRemaining...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION
	template <typename T>
	HFSM2_INLINE	   T& access();

	template <typename T>
	HFSM2_INLINE const T& access() const;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideRegister		  (Registry& registry, const ForkID forkId);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE bool	 wideForwardEntryGuard(GuardControl& control,					const ProngCBits prongs);
	HFSM2_INLINE bool	 wideForwardEntryGuard(GuardControl& control);
	HFSM2_INLINE bool	 wideEntryGuard		  (GuardControl& control);

	HFSM2_INLINE void	 wideConstruct		  (PlanControl&	 control);

	HFSM2_INLINE void	 wideEnter			  (PlanControl&	 control);
	HFSM2_INLINE void	 wideReenter		  (PlanControl&	 control);

	HFSM2_INLINE Status	 wideUpdate			  (FullControl&	 control);

	template <typename TEvent>
	HFSM2_INLINE Status	 wideReact			  (FullControl&	 control, const TEvent& event);

	HFSM2_INLINE bool	 wideForwardExitGuard (GuardControl& control,					const ProngCBits prongs);
	HFSM2_INLINE bool	 wideForwardExitGuard (GuardControl& control);
	HFSM2_INLINE bool	 wideExitGuard		  (GuardControl& control);

	HFSM2_INLINE void	 wideExit			  (PlanControl&	 control);

	HFSM2_INLINE void	 wideDestruct		  (PlanControl&  control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideForwardActive	  (Control& control, const Request request, const ProngCBits prongs);
	HFSM2_INLINE void	 wideForwardRequest	  (Control& control, const Request request);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideRequestChange	  (Control& control, const Request request);
	HFSM2_INLINE void	 wideRequestRestart	  (Control& control, const Request request);
	HFSM2_INLINE void	 wideRequestResume	  (Control& control, const Request request);

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void	 wideRequestUtilize	  (Control& control, const Request request);
	HFSM2_INLINE void	 wideRequestRandomize (Control& control, const Request request);

	HFSM2_INLINE Utility wideReportChange	  (Control& control);
	HFSM2_INLINE Utility wideReportUtilize	  (Control& control);
	HFSM2_INLINE Utility wideReportRandomize  (Control& control);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideChangeToRequested(PlanControl&  control);

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_INLINE void	 wideSaveActive		  (const Registry& registry, WriteStream& stream) const;
	HFSM2_INLINE void	 wideSaveResumable	  (const Registry& registry, WriteStream& stream) const;

	HFSM2_INLINE void	 wideLoadRequested	  (		 Registry& registry, ReadStream&  stream) const;
	HFSM2_INLINE void	 wideLoadResumable	  (		 Registry& registry, ReadStream&  stream) const;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;

	static constexpr Long NAME_COUNT	 = Initial::NAME_COUNT  + Remaining::NAME_COUNT;

	void wideGetNames(const Long parent,
					  const Short depth,
					  StructureStateInfos& stateInfos) const;
#endif

	//----------------------------------------------------------------------

	Initial initial;
	Remaining remaining;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  Short NIndex,
		  typename TInitial>
struct OS_<TIndices, TArgs, NIndex, TInitial> final {
	using Indices		= TIndices;
	static constexpr StateID INITIAL_ID	 = Indices::STATE_ID;
	static constexpr Short	 COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	 ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	 ORTHO_UNIT	 = Indices::ORTHO_UNIT;

	static constexpr Short	 PRONG_INDEX = NIndex;

	using Args			= TArgs;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;
	using OrthoForks	= typename Registry::OrthoForks;
	using ProngBits		= typename OrthoForks::Bits;
	using ProngCBits= typename OrthoForks::CBits;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Initial		= Material<I_<INITIAL_ID,
									  COMPO_INDEX,
									  ORTHO_INDEX,
									  ORTHO_UNIT>,
								   Args,
								   TInitial>;

	using Info	= OSI_<TInitial>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION
	template <typename T>
	HFSM2_INLINE	   T& access()			  { return initial.template access<T>();	}

	template <typename T>
	HFSM2_INLINE const T& access() const	  { return initial.template access<T>();	}
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideRegister		  (Registry& registry, const ForkID forkId);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE bool	 wideForwardEntryGuard(GuardControl& control,					const ProngCBits prongs);
	HFSM2_INLINE bool	 wideForwardEntryGuard(GuardControl& control);
	HFSM2_INLINE bool	 wideEntryGuard		  (GuardControl& control);

	HFSM2_INLINE void	 wideConstruct		  (PlanControl&  control);

	HFSM2_INLINE void	 wideEnter			  (PlanControl&  control);
	HFSM2_INLINE void	 wideReenter		  (PlanControl&  control);

	HFSM2_INLINE Status	 wideUpdate			  (FullControl&  control);

	template <typename TEvent>
	HFSM2_INLINE Status	 wideReact			  (FullControl&  control, const TEvent& event);

	HFSM2_INLINE bool	 wideForwardExitGuard (GuardControl& control,					const ProngCBits prongs);
	HFSM2_INLINE bool	 wideForwardExitGuard (GuardControl& control);
	HFSM2_INLINE bool	 wideExitGuard		  (GuardControl& control);

	HFSM2_INLINE void	 wideExit			  (PlanControl&  control);

	HFSM2_INLINE void	 wideDestruct		  (PlanControl&  control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideForwardActive	  (Control& control, const Request request, const ProngCBits prongs);
	HFSM2_INLINE void	 wideForwardRequest	  (Control& control, const Request request);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideRequestChange	  (Control& control, const Request request);
	HFSM2_INLINE void	 wideRequestRestart	  (Control& control, const Request request);
	HFSM2_INLINE void	 wideRequestResume	  (Control& control, const Request request);

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void	 wideRequestUtilize	  (Control& control, const Request request);
	HFSM2_INLINE void	 wideRequestRandomize (Control& control, const Request request);

	HFSM2_INLINE Utility wideReportChange	  (Control& control);
	HFSM2_INLINE Utility wideReportUtilize	  (Control& control);
	HFSM2_INLINE Utility wideReportRandomize  (Control& control);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideChangeToRequested(PlanControl&  control);

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_INLINE void	 wideSaveActive		  (const Registry& registry, WriteStream& stream) const;
	HFSM2_INLINE void	 wideSaveResumable	  (const Registry& registry, WriteStream& stream) const;

	HFSM2_INLINE void	 wideLoadRequested	  (		 Registry& registry, ReadStream&  stream) const;
	HFSM2_INLINE void	 wideLoadResumable	  (		 Registry& registry, ReadStream&  stream) const;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;

	static constexpr Long NAME_COUNT	 = Initial::NAME_COUNT;

	void wideGetNames(const Long parent,
					  const Short depth,
					  StructureStateInfos& stateInfos) const;
#endif

	//----------------------------------------------------------------------

	Initial initial;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
template <typename T>
T&
OS_<TN, TA, NI, TI, TR...>::access() {
	return contains<InitialStates, T>() ?
		initial  .template access<T>() :
		remaining.template access<T>();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
template <typename T>
const T&
OS_<TN, TA, NI, TI, TR...>::access() const {
	return contains<InitialStates, T>() ?
		initial  .template access<T>() :
		remaining.template access<T>();
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideRegister(Registry& registry,
										 const ForkID forkId)
{
	initial  .deepRegister(registry, Parent{forkId, PRONG_INDEX});
	remaining.wideRegister(registry, forkId);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardEntryGuard(GuardControl& control,
												  const ProngCBits prongs)
{
	const bool i = prongs.get(PRONG_INDEX) ?
				   initial  .deepForwardEntryGuard(control) : false;

	const bool r = remaining.wideForwardEntryGuard(control, prongs);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardEntryGuard(GuardControl& control) {
	const bool i = initial  .deepForwardEntryGuard(control);
	const bool r = remaining.wideForwardEntryGuard(control);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
bool
OS_<TN, TA, NI, TI, TR...>::wideEntryGuard(GuardControl& control) {
	const bool i = initial  .deepEntryGuard(control);
	const bool r = remaining.wideEntryGuard(control);

	return i || r;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideConstruct(PlanControl& control) {
	initial	 .deepConstruct(control);
	remaining.wideConstruct(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideEnter(PlanControl& control) {
	initial  .deepEnter(control);
	remaining.wideEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideReenter(PlanControl& control) {
	initial  .deepReenter(control);
	remaining.wideReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
Status
OS_<TN, TA, NI, TI, TR...>::wideUpdate(FullControl& control) {
	const auto status =	   initial	.deepUpdate(control);
	return combine(status, remaining.wideUpdate(control));
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
template <typename TEvent>
Status
OS_<TN, TA, NI, TI, TR...>::wideReact(FullControl& control,
									  const TEvent& event)
{
	const auto status =	   initial	.deepReact(control, event);
	return combine(status, remaining.wideReact(control, event));
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardExitGuard(GuardControl& control,
												 const ProngCBits prongs)
{
	const bool i = prongs.get(PRONG_INDEX) ?
				   initial  .deepForwardExitGuard(control) : false;

	const bool r = remaining.wideForwardExitGuard(control, prongs);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardExitGuard(GuardControl& control) {
	const bool i = initial  .deepForwardExitGuard(control);
	const bool r = remaining.wideForwardExitGuard(control);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
bool
OS_<TN, TA, NI, TI, TR...>::wideExitGuard(GuardControl& control) {
	const bool i = initial  .deepExitGuard(control);
	const bool r = remaining.wideExitGuard(control);

	return i || r;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideExit(PlanControl& control) {
	initial	 .deepExit(control);
	remaining.wideExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideDestruct(PlanControl& control) {
	initial	 .deepDestruct(control);
	remaining.wideDestruct(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideForwardActive(Control& control,
											  const Request request,
											  const ProngCBits prongs)
{
	if (prongs.get(PRONG_INDEX))
		initial.deepForwardActive(control, request);

	remaining  .wideForwardActive(control, request, prongs);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideForwardRequest(Control& control,
											   const Request request)
{
	initial	 .deepForwardRequest(control, request);
	remaining.wideForwardRequest(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideRequestChange(Control& control,
											  const Request request)
{
	initial  .deepRequestChange(control, request);
	remaining.wideRequestChange(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideRequestRestart(Control& control,
											   const Request request)
{
	initial	 .deepRequestRestart(control, request);
	remaining.wideRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideRequestResume(Control& control,
											  const Request request)
{
	initial	 .deepRequestResume(control, request);
	remaining.wideRequestResume(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideRequestUtilize(Control& control,
											   const Request request)
{
	initial  .deepRequestUtilize(control, request);
	remaining.wideRequestUtilize(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideRequestRandomize(Control& control,
												 const Request request)
{
	initial  .deepRequestRandomize(control, request);
	remaining.wideRequestRandomize(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
typename TA::Utility
OS_<TN, TA, NI, TI, TR...>::wideReportChange(Control& control) {
	const UP	  i = initial  .deepReportChange(control);
	const Utility r = remaining.wideReportChange(control);

	return i.utility + r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
typename TA::Utility
OS_<TN, TA, NI, TI, TR...>::wideReportUtilize(Control& control) {
	const UP	  i = initial  .deepReportUtilize(control);
	const Utility r = remaining.wideReportUtilize(control);

	return i.utility + r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
typename TA::Utility
OS_<TN, TA, NI, TI, TR...>::wideReportRandomize(Control& control) {
	const Utility i = initial  .deepReportRandomize(control);
	const Utility r = remaining.wideReportRandomize(control);

	return i + r;
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideChangeToRequested(PlanControl& control) {
	initial	 .deepChangeToRequested(control);
	remaining.wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideSaveActive(const Registry& registry,
										   WriteStream& stream) const
{
	initial	 .deepSaveActive(registry, stream);
	remaining.wideSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideSaveResumable(const Registry& registry,
											  WriteStream& stream) const
{
	initial	 .deepSaveResumable(registry, stream);
	remaining.wideSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideLoadRequested(Registry& registry,
											  ReadStream& stream) const
{
	initial	 .deepLoadRequested(registry, stream);
	remaining.wideLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideLoadResumable(Registry& registry,
											  ReadStream& stream) const
{
	initial	 .deepLoadResumable(registry, stream);
	remaining.wideLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideGetNames(const Long parent,
										 const Short depth,
										 StructureStateInfos& stateInfos) const
{
	initial	 .deepGetNames(parent, StructureStateInfo::RegionType::ORTHOGONAL, depth, stateInfos);
	remaining.wideGetNames(parent,											   depth, stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Short NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRegister(Registry& registry,
									  const ForkID forkId)
{
	initial.deepRegister(registry, Parent{forkId, PRONG_INDEX});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
bool
OS_<TN, TA, NI, TI>::wideForwardEntryGuard(GuardControl& control,
										   const ProngCBits prongs)
{
	return prongs.get(PRONG_INDEX) ?
		initial.deepForwardEntryGuard(control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
bool
OS_<TN, TA, NI, TI>::wideForwardEntryGuard(GuardControl& control) {
	return initial.deepForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
bool
OS_<TN, TA, NI, TI>::wideEntryGuard(GuardControl& control) {
	return initial.deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideConstruct(PlanControl& control) {
	initial.deepConstruct(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideEnter(PlanControl& control) {
	initial.deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideReenter(PlanControl& control) {
	initial.deepReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
Status
OS_<TN, TA, NI, TI>::wideUpdate(FullControl& control) {
	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
template <typename TEvent>
Status
OS_<TN, TA, NI, TI>::wideReact(FullControl& control,
							   const TEvent& event)
{
	return initial.deepReact(control, event);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
bool
OS_<TN, TA, NI, TI>::wideForwardExitGuard(GuardControl& control,
										  const ProngCBits prongs)
{
	return prongs.get(PRONG_INDEX) ?
		initial.deepForwardExitGuard(control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
bool
OS_<TN, TA, NI, TI>::wideForwardExitGuard(GuardControl& control) {
	return initial.deepForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
bool
OS_<TN, TA, NI, TI>::wideExitGuard(GuardControl& control) {
	return initial.deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideExit(PlanControl& control) {
	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideDestruct(PlanControl& control) {
	initial.deepDestruct(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideForwardActive(Control& control,
									   const Request request,
									   const ProngCBits prongs)
{
	if (prongs.get(PRONG_INDEX))
		initial.deepForwardActive(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideForwardRequest(Control& control,
										const Request request)
{
	initial.deepForwardRequest(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRequestChange(Control& control,
									   const Request request)
{
	initial.deepRequestChange(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRequestRestart(Control& control,
										const Request request)
{
	initial.deepRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRequestResume(Control& control,
									   const Request request)
{
	initial.deepRequestResume(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TN, typename TA, Short NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRequestUtilize(Control& control,
										const Request request)
{
	initial.deepRequestUtilize(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRequestRandomize(Control& control,
										  const Request request)
{
	initial.deepRequestRandomize(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
typename TA::Utility
OS_<TN, TA, NI, TI>::wideReportChange(Control& control) {
	const UP i = initial.deepReportChange(control);

	return i.utility;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
typename TA::Utility
OS_<TN, TA, NI, TI>::wideReportUtilize(Control& control) {
	const UP i = initial.deepReportUtilize(control);

	return i.utility;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
typename TA::Utility
OS_<TN, TA, NI, TI>::wideReportRandomize(Control& control) {
	return initial.deepReportRandomize(control);
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideChangeToRequested(PlanControl& control) {
	initial.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION

template <typename TN, typename TA, Short NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideSaveActive(const Registry& registry,
									WriteStream& stream) const
{
	initial.deepSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideSaveResumable(const Registry& registry,
									   WriteStream& stream) const
{
	initial.deepSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideLoadRequested(Registry& registry,
									   ReadStream& stream) const
{
	initial.deepLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideLoadResumable(Registry& registry,
									   ReadStream& stream) const
{
	initial.deepLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, Short NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideGetNames(const Long parent,
								  const Short depth,
								  StructureStateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StructureStateInfo::RegionType::ORTHOGONAL, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct O_ final {
	using Indices		= TIndices;
	static constexpr StateID HEAD_ID	 = Indices::STATE_ID;
	static constexpr Short	 COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	 ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	 ORTHO_UNIT	 = Indices::ORTHO_UNIT;

	static constexpr Short	 REGION_ID	 = COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID	 ORTHO_ID	 = (ForkID) -ORTHO_INDEX - 1;

	using Args			= TArgs;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using Head			= THead;

	using Info			= OI_<Head, TSubStates...>;
	static constexpr Short WIDTH		= Info::WIDTH;
	static constexpr Short REGION_SIZE	= Info::STATE_COUNT;
	static constexpr Short ORTHO_UNITS	= Info::ORTHO_UNITS;

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;
	using OrthoForks	= typename Registry::OrthoForks;
	using ProngBits		= typename OrthoForks::Bits;
	using ProngCBits	= typename OrthoForks::CBits;

	using Control		= ControlT<Args>;
	using ScopedOrigin	= typename Control::Origin;

	using PlanControl	= PlanControlT<Args>;
	using ScopedRegion	= typename PlanControl::Region;

	using FullControl	= FullControlT<Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;

	using HeadState		= S_<Indices, Args, Head>;

	using SubStates		= OS_<I_<HEAD_ID + 1,
								 COMPO_INDEX,
								 ORTHO_INDEX + 1,
								 ORTHO_UNIT + (WIDTH + 7) / 8>,
							  Args,
							  0,
							  TSubStates...>;

	//----------------------------------------------------------------------

#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION
	template <typename T>
	struct Accessor {
		HFSM2_INLINE static		  T&    get(	  O_& o)						{ return o._subStates.template access<T>();	}
		HFSM2_INLINE static const T&    get(const O_& o)						{ return o._subStates.template access<T>();	}
	};

	template <>
	struct Accessor<Head> {
		HFSM2_INLINE static		  Head& get(	  O_& o)						{ return o._headState._headBox.get();		}
		HFSM2_INLINE static const Head& get(const O_& o)						{ return o._headState._headBox.get();		}
	};

	template <typename T>
	HFSM2_INLINE	   T&	access()											{ return Accessor<T>::get(*this);			}

	template <typename T>
	HFSM2_INLINE const T&	access() const										{ return Accessor<T>::get(*this);			}
#endif

	//----------------------------------------------------------------------

	HFSM2_INLINE ProngBits		orthoRequested(		 Registry& registry)		{ return registry.orthoRequested.template bits<ORTHO_UNIT, WIDTH>();	}
	HFSM2_INLINE ProngCBits orthoRequested(const Registry& registry) const	{ return registry.orthoRequested.template bits<ORTHO_UNIT, WIDTH>();	}

	HFSM2_INLINE ProngBits		orthoRequested(		 Control&  control)			{ return orthoRequested(control._registry);								}
	HFSM2_INLINE ProngCBits orthoRequested(const Control&  control) const	{ return orthoRequested(control._registry);								}

	HFSM2_INLINE void	 deepRegister		  (Registry& registry, const Parent parent);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE bool	 deepForwardEntryGuard(GuardControl& control);
	HFSM2_INLINE bool	 deepEntryGuard		  (GuardControl& control);

	HFSM2_INLINE void	 deepConstruct		  (PlanControl&	 control);

	HFSM2_INLINE void	 deepEnter			  (PlanControl&	 control);
	HFSM2_INLINE void	 deepReenter		  (PlanControl&	 control);

	HFSM2_INLINE Status	 deepUpdate			  (FullControl&	 control);

	template <typename TEvent>
	HFSM2_INLINE Status	 deepReact			  (FullControl&	 control, const TEvent& event);

	HFSM2_INLINE bool	 deepForwardExitGuard (GuardControl& control);
	HFSM2_INLINE bool	 deepExitGuard		  (GuardControl& control);

	HFSM2_INLINE void	 deepExit			  (PlanControl&	 control);

	HFSM2_INLINE void	 deepDestruct		  (PlanControl&  control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 deepForwardActive	  (Control& control, const Request request);
	HFSM2_INLINE void	 deepForwardRequest	  (Control& control, const Request request);

	HFSM2_INLINE void	 deepRequest		  (Control& control, const Request request);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 deepRequestChange	  (Control& control, const Request request);
	HFSM2_INLINE void	 deepRequestRestart	  (Control& control, const Request request);
	HFSM2_INLINE void	 deepRequestResume	  (Control& control, const Request request);

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void	 deepRequestUtilize	  (Control& control, const Request request);
	HFSM2_INLINE void	 deepRequestRandomize (Control& control, const Request request);

	HFSM2_INLINE UP		 deepReportChange	  (Control& control);

	HFSM2_INLINE UP		 deepReportUtilize	  (Control& control);
	HFSM2_INLINE Rank	 deepReportRank		  (Control& control);
	HFSM2_INLINE Utility deepReportRandomize  (Control& control);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 deepChangeToRequested(PlanControl&  control);

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_INLINE void	 deepSaveActive		  (const Registry& registry, WriteStream& stream) const;
	HFSM2_INLINE void	 deepSaveResumable	  (const Registry& registry, WriteStream& stream) const;

	HFSM2_INLINE void	 deepLoadRequested	  (		 Registry& registry, ReadStream&  stream) const;
	HFSM2_INLINE void	 deepLoadResumable	  (		 Registry& registry, ReadStream&  stream) const;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr Long NAME_COUNT	 = HeadState::NAME_COUNT  + SubStates::NAME_COUNT;

	void deepGetNames(const Long parent,
					  const RegionType region,
					  const Short depth,
					  StructureStateInfos& stateInfos) const;
#endif

	//----------------------------------------------------------------------

	HeadState _headState;
	SubStates _subStates;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRegister(Registry& registry,
										const Parent parent)
{
	registry.orthoParents[ORTHO_INDEX] = parent;
	registry.orthoUnits[ORTHO_INDEX] = Units{ORTHO_UNIT, WIDTH};

	_headState.deepRegister(registry, parent);
	_subStates.wideRegister(registry, ORTHO_ID);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
bool
O_<TN, TA, TH, TS...>::deepForwardEntryGuard(GuardControl& control) {
	const ProngCBits requested = orthoRequested(static_cast<const GuardControl&>(control));

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested)
		return _subStates.wideForwardEntryGuard(control, requested);
	else
		return _subStates.wideForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
bool
O_<TN, TA, TH, TS...>::deepEntryGuard(GuardControl& control) {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepEntryGuard(control) ||
		   _subStates.wideEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepConstruct(PlanControl& control) {
	ProngBits requested = orthoRequested(control);
	requested.clear();

	_headState.deepConstruct(control);
	_subStates.wideConstruct(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepEnter(PlanControl& control) {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter(control);
	_subStates.wideEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepReenter(PlanControl& control) {
	ProngBits requested = orthoRequested(control);
	requested.clear();

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepReenter(control);
	_subStates.wideReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
Status
O_<TN, TA, TH, TS...>::deepUpdate(FullControl& control) {
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

	#ifdef HFSM2_ENABLE_PLANS
		return subStatus && control._planData.planExists.template get<REGION_ID>() ?
			control.updatePlan(_headState, subStatus) : subStatus;
	#else
		return subStatus;
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
template <typename TEvent>
Status
O_<TN, TA, TH, TS...>::deepReact(FullControl& control,
								 const TEvent& event)
{
	ScopedRegion outer{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const auto headStatus = _headState.deepReact(control, event)) {
		ControlLock lock{control};
		_subStates.wideReact(control, event);

		return headStatus;
	} else {
		const Status subStatus = _subStates.wideReact(control, event);

		if (subStatus.outerTransition)
			return subStatus;

		ScopedRegion inner{control, REGION_ID, HEAD_ID, REGION_SIZE};

	#ifdef HFSM2_ENABLE_PLANS
		return subStatus && control._planData.planExists.template get<REGION_ID>() ?
			control.updatePlan(_headState, subStatus) : subStatus;
	#else
		return subStatus;
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
bool
O_<TN, TA, TH, TS...>::deepForwardExitGuard(GuardControl& control) {
	const ProngCBits requested = orthoRequested(static_cast<const GuardControl&>(control));

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested)
		return _subStates.wideForwardExitGuard(control, requested);
	else
		return _subStates.wideForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
bool
O_<TN, TA, TH, TS...>::deepExitGuard(GuardControl& control) {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepExitGuard(control) ||
		   _subStates.wideExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepExit(PlanControl& control) {
	_subStates.wideExit(control);
	_headState.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepDestruct(PlanControl& control) {
	_subStates.wideDestruct(control);
	_headState.deepDestruct(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepForwardActive(Control& control,
										 const Request request)
{
	HFSM2_ASSERT(control._registry.isActive(HEAD_ID));

	const ProngCBits requested = orthoRequested(static_cast<const Control&>(control));
	HFSM2_ASSERT(!!requested);

	_subStates.wideForwardActive(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepForwardRequest(Control& control,
										  const Request request)
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const ProngCBits requested = orthoRequested(static_cast<const Control&>(control));

	if (requested)
		_subStates.wideForwardRequest(control, request);
	else
		deepRequest					 (control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRequest(Control& control,
								   const Request request)
{
	switch (request.type) {
	case TransitionType::CHANGE:
		deepRequestChange	(control, request);
		break;

	case TransitionType::RESTART:
		deepRequestRestart	(control, request);
		break;

	case TransitionType::RESUME:
		deepRequestResume	(control, request);
		break;

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	case TransitionType::UTILIZE:
		deepRequestUtilize	(control, request);
		break;

	case TransitionType::RANDOMIZE:
		deepRequestRandomize(control, request);
		break;

#endif

	default:
		HFSM2_BREAK();
	}
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRequestChange(Control& control,
										 const Request request)
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	_subStates.wideRequestChange(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRequestRestart(Control& control,
										  const Request request)
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	_subStates.wideRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRequestResume(Control& control,
										 const Request request)
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	_subStates.wideRequestResume(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRequestUtilize(Control& control,
										  const Request request)
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	_subStates.wideRequestUtilize(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRequestRandomize(Control& control,
											const Request request)
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	_subStates.wideRequestRandomize(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
typename TA::UP
O_<TN, TA, TH, TS...>::deepReportChange(Control& control) {
	const UP	  h = _headState.deepReportChange(control);
	const Utility s = _subStates.wideReportChange(control);

	const Utility sub = s / WIDTH;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, INVALID_STATE_ID, sub);

	return {
		h.utility * sub,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
typename TA::UP
O_<TN, TA, TH, TS...>::deepReportUtilize(Control& control) {
	const UP	  h = _headState.deepReportUtilize(control);
	const Utility s = _subStates.wideReportUtilize(control);

	const Utility sub = s / WIDTH;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, INVALID_STATE_ID, sub);

	return {
		h.utility * sub,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
typename TA::Rank
O_<TN, TA, TH, TS...>::deepReportRank(Control& control) {
	return _headState.wrapRank(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
typename TA::Utility
O_<TN, TA, TH, TS...>::deepReportRandomize(Control& control) {
	const Utility h = _headState.wrapUtility(control);
	const Utility s = _subStates.wideReportRandomize(control);

	const Utility sub = s / WIDTH;

	HFSM2_LOG_RANDOM_RESOLUTION(control.context(), HEAD_ID, INVALID_STATE_ID, sub);

	return h * sub;
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepChangeToRequested(PlanControl& control) {
	_subStates.wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepSaveActive(const Registry& registry,
									  WriteStream& stream) const
{
	_subStates.wideSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepSaveResumable(const Registry& registry,
										 WriteStream& stream) const
{
	_subStates.wideSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepLoadRequested(Registry& registry,
										 ReadStream& stream) const
{
	_subStates.wideLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepLoadResumable(Registry& registry,
										 ReadStream& stream) const
{
	_subStates.wideLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepGetNames(const Long parent,
									const RegionType region,
									const Short depth,
									StructureStateInfos& stateInfos) const
{
	_headState.deepGetNames(parent, region,			depth,	   stateInfos);
	_subStates.wideGetNames(stateInfos.count() - 1, depth + 1, stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

#ifndef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION

//------------------------------------------------------------------------------

template <typename...>
struct Accessor;

////////////////////////////////////////////////////////////////////////////////

template <typename T,
		  typename TN,
		  typename TA,
		  Strategy TG,
		  typename TH,
		  typename... TS>
struct Accessor<T,		 C_<TN, TA, TG, TH, TS...>> {
	using Host =		 C_<TN, TA, TG, TH, TS...>;

	HFSM2_INLINE	   T& get()		  noexcept { return Accessor<T,		  typename Host::SubStates>{host._subStates}.get();	}

	Host& host;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T,
		  typename TN,
		  typename TA,
		  Strategy TG,
		  typename TH,
		  typename... TS>
struct Accessor<T, const C_<TN, TA, TG, TH, TS...>> {
	using Host =   const C_<TN, TA, TG, TH, TS...>;

	HFSM2_INLINE const T& get() const noexcept { return Accessor<T, const typename Host::SubStates>{host._subStates}.get();	}

	Host& host;
};

//------------------------------------------------------------------------------

template <typename T,
		  typename TN,
		  typename TA,
		  Strategy TG,
		  typename... TS>
struct Accessor<T,		 C_<TN, TA, TG,  T, TS...>> {
	using Host =		 C_<TN, TA, TG,  T, TS...>;

	HFSM2_INLINE	   T& get()		  noexcept	{ return host._headState._headBox.get();	}

	Host& host;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T,
		  typename TN,
		  typename TA,
		  Strategy TG,
		  typename... TS>
struct Accessor<T, const C_<TN, TA, TG,  T, TS...>> {
	using Host =   const C_<TN, TA, TG,  T, TS...>;

	HFSM2_INLINE const T& get() const noexcept	{ return host._headState._headBox.get();	}

	Host& host;
};

////////////////////////////////////////////////////////////////////////////////

template <typename T,
		  typename TN,
		  typename TA,
		  Strategy TG,
		  Short NI,
		  typename... TS>
struct Accessor<T,		 CS_<TN, TA, TG, NI, TS...>> {
	using Host =		 CS_<TN, TA, TG, NI, TS...>;

	HFSM2_INLINE	   T& get()		  noexcept	{
		return contains<typename Host::LHalfInfo::StateList, T>() ?
			Accessor<T,		  typename Host::LMaterial>{host.lHalf}.get() :
			Accessor<T,		  typename Host::RMaterial>{host.rHalf}.get();
	}

	Host& host;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T,
		  typename TN,
		  typename TA,
		  Strategy TG,
		  Short NI,
		  typename... TS>
struct Accessor<T, const CS_<TN, TA, TG, NI, TS...>> {
	using Host =   const CS_<TN, TA, TG, NI, TS...>;

	HFSM2_INLINE const T& get() const noexcept	{
		return contains<typename Host::LHalfInfo::StateList, T>() ?
			Accessor<T, const typename Host::LMaterial>{host.lHalf}.get() :
			Accessor<T, const typename Host::RMaterial>{host.rHalf}.get();
	}

	Host& host;
};

//------------------------------------------------------------------------------

template <typename T,
		  typename TN,
		  typename TA,
		  Strategy TG,
		  Short NI,
		  typename TS>
struct Accessor<T,		 CS_<TN, TA, TG, NI, TS>> {
	using Host =		 CS_<TN, TA, TG, NI, TS>;

	HFSM2_INLINE	   T& get()		  noexcept	{ return Accessor<T,	   typename Host::State>{host.state}.get();				}

	Host& host;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T,
		  typename TN,
		  typename TA,
		  Strategy TG,
		  Short NI,
		  typename TS>
struct Accessor<T, const CS_<TN, TA, TG, NI, TS>> {
	using Host =   const CS_<TN, TA, TG, NI, TS>;

	HFSM2_INLINE const T& get() const noexcept	{ return Accessor<T, const typename Host::State>{host.state}.get();				}

	Host& host;
};

////////////////////////////////////////////////////////////////////////////////

template <typename T,
		  typename TN,
		  typename TA,
		  typename TH,
		  typename... TS>
struct Accessor<T,		 O_<TN, TA, TH, TS...>> {
	using Host =		 O_<TN, TA, TH, TS...>;

	HFSM2_INLINE	   T& get()		  noexcept	{ return Accessor<T,	   typename Host::SubStates>{host._subStates}.get();	}

	Host& host;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T,
		  typename TN,
		  typename TA,
		  typename TH,
		  typename... TS>
struct Accessor<T, const O_<TN, TA, TH, TS...>> {
	using Host =   const O_<TN, TA, TH, TS...>;

	HFSM2_INLINE const T& get() const noexcept	{ return Accessor<T, const typename Host::SubStates>{host._subStates}.get();	}

	Host& host;
};

//------------------------------------------------------------------------------

template <typename T,
		  typename TN,
		  typename TA,
		  typename... TS>
struct Accessor<T,		 O_<TN, TA,  T, TS...>> {
	using Host =		 O_<TN, TA,  T, TS...>;

	HFSM2_INLINE	   T& get()		  noexcept	{ return host._headState._headBox.get();	}

	Host& host;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T,
		  typename TN,
		  typename TA,
		  typename... TS>
struct Accessor<T, const O_<TN, TA,  T, TS...>> {
	using Host =   const O_<TN, TA,  T, TS...>;

	HFSM2_INLINE const T& get() const noexcept	{ return host._headState._headBox.get();	}

	Host& host;
};

////////////////////////////////////////////////////////////////////////////////

template <typename T,
		  typename TN,
		  typename TA,
		  Short NI,
		  typename... TS>
struct Accessor<T,		 OS_<TN, TA, NI, TS...>> {
	using Host =		 OS_<TN, TA, NI, TS...>;

	HFSM2_INLINE	   T& get()		  noexcept	{
		return contains<typename Host::InitialStates, T>() ?
			Accessor<T,		  typename Host::Initial  >{host.initial  }.get() :
			Accessor<T,		  typename Host::Remaining>{host.remaining}.get();
	}

	Host& host;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T,
		  typename TN,
		  typename TA,
		  Short NI,
		  typename... TS>
struct Accessor<T, const OS_<TN, TA, NI, TS...>> {
	using Host =   const OS_<TN, TA, NI, TS...>;

	HFSM2_INLINE const T& get() const noexcept	{
		return contains<typename Host::InitialStates, T>() ?
			Accessor<T, const typename Host::Initial  >{host.initial  }.get() :
			Accessor<T, const typename Host::Remaining>{host.remaining}.get();
	}

	Host& host;
};

//------------------------------------------------------------------------------

template <typename T,
		  typename TN,
		  typename TA,
		  Short NI,
		  typename TS>
struct Accessor<T,		 OS_<TN, TA, NI, TS>> {
	using Host =		 OS_<TN, TA, NI, TS>;

	HFSM2_INLINE	   T& get()		  noexcept	{ return Accessor<T,	   typename Host::Initial>{host.initial  }.get();	}

	Host& host;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T,
		  typename TN,
		  typename TA,
		  Short NI,
		  typename TS>
struct Accessor<T, const OS_<TN, TA, NI, TS>> {
	using Host =   const OS_<TN, TA, NI, TS>;

	HFSM2_INLINE const T& get() const noexcept	{ return Accessor<T, const typename Host::Initial>{host.initial  }.get();	}

	Host& host;
};


////////////////////////////////////////////////////////////////////////////////

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wnull-dereference"
#endif

template <typename T,
		  typename TN,
		  typename TA,
		  typename TH>
struct Accessor<T,		 S_<TN, TA, TH>> {
	using Host =		 S_<TN, TA, TH>;

	HFSM2_INLINE	   T& get()		  noexcept	{ HFSM2_BREAK(); return *reinterpret_cast<T*>(0);	}

	Host& host;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T,
		  typename TN,
		  typename TA,
		  typename TH>
struct Accessor<T, const S_<TN, TA, TH>> {
	using Host =   const S_<TN, TA, TH>;

	HFSM2_INLINE const T& get() const noexcept	{ HFSM2_BREAK(); return *reinterpret_cast<T*>(0);	}

	Host& host;
};

#ifdef __clang__
	#pragma clang diagnostic pop
#endif

//------------------------------------------------------------------------------

template <typename T,
		  typename TN,
		  typename TA>
struct Accessor<T,		 S_<TN, TA,  T>> {
	using Host =		 S_<TN, TA,  T>;

	HFSM2_INLINE	   T& get()		  noexcept	{ return host._headBox.get();	}

	Host& host;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T,
		  typename TN,
		  typename TA>
struct Accessor<T, const S_<TN, TA,  T>> {
	using Host =   const S_<TN, TA,  T>;

	HFSM2_INLINE const T& get() const noexcept	{ return host._headBox.get();	}

	Host& host;
};

////////////////////////////////////////////////////////////////////////////////

#endif

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFeatureTag
		, typename TContext

	#ifdef HFSM2_ENABLE_UTILITY_THEORY
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct G_ final {
	static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

	using Context			 = TContext;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Rank				 = TRank;
	using Utility			 = TUtility;
	using RNG				 = TRNG;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using LoggerInterface	 = LoggerInterfaceT<FEATURE_TAG, Context HFSM2_IF_UTILITY_THEORY(, Utility)>;
#endif

	static constexpr Long SUBSTITUTION_LIMIT = NSubstitutionLimit;

#ifdef HFSM2_ENABLE_PLANS
	static constexpr Long TASK_CAPACITY		 = NTaskCapacity;
#endif

	using Payload			 = TPayload;
	using Transition		 = TransitionT<Payload>;

#ifdef HFSM2_ENABLE_PLANS
	using Task				 = TaskT<Payload>;
#endif

	/// @brief Set Context type
	/// @tparam T Context type for data shared between states and/or data interface between FSM and external code
	template <typename T>
	using ContextT			 = G_<FEATURE_TAG, T	   HFSM2_IF_UTILITY_THEORY(, Rank, Utility, RNG), SUBSTITUTION_LIMIT HFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	/// @brief Set Rank type
	/// @tparam T Rank type for 'TRank State::rank() const' method
	template <typename T>
	using RankT				 = G_<FEATURE_TAG, Context                         , T   , Utility, RNG , SUBSTITUTION_LIMIT HFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

	/// @brief Set Utility type
	/// @tparam T Utility type for 'TUtility State::utility() const' method
	template <typename T>
	using UtilityT			 = G_<FEATURE_TAG, Context                         , Rank, T      , RNG , SUBSTITUTION_LIMIT HFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

	/// @brief Set RNG type
	/// @tparam T RNG type used in 'Random' regions
	template <typename T>
	using RandomT			 = G_<FEATURE_TAG, Context                         , Rank, Utility, T   , SUBSTITUTION_LIMIT HFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

#endif

	/// @brief Set Substitution limit
	/// @tparam N Maximum number times 'guard()' methods can substitute their states for others
	template <Long N>
	using SubstitutionLimitN = G_<FEATURE_TAG, Context HFSM2_IF_UTILITY_THEORY(, Rank, Utility, RNG), N                  HFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

#ifdef HFSM2_ENABLE_PLANS

	/// @brief Set Task capacity
	/// @tparam N Maximum number of tasks across all plans
	template <Long N>
	using TaskCapacityN		 = G_<FEATURE_TAG, Context HFSM2_IF_UTILITY_THEORY(, Rank, Utility, RNG), SUBSTITUTION_LIMIT                , N             , Payload>;

#endif

	/// @brief Set Transition Payload type
	/// @tparam T Utility type for 'TUtility State::utility() const' method
	template <typename T>
	using PayloadT			 = G_<FEATURE_TAG, Context HFSM2_IF_UTILITY_THEORY(, Rank, Utility, RNG), SUBSTITUTION_LIMIT HFSM2_IF_PLANS(, TASK_CAPACITY), T      >;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	struct UP {
		HFSM2_INLINE UP(const Utility utility_ = Utility{1},
						const Short prong_ = INVALID_SHORT) noexcept
			: utility{utility_}
			, prong{prong_}
		{}

		Utility utility;
		Short prong;
	};

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig>
struct M_;

template <FeatureTag NFeatureTag
		, typename TContext

	#ifdef HFSM2_ENABLE_UTILITY_THEORY
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct M_	   <G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>> {
	using Cfg = G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>;

	static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

	using Context			= TContext;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Utility			= TUtility;
#endif

	using Payload			= TPayload;
	using Transition		= TransitionT<Payload>;

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using LoggerInterface	= typename Cfg::LoggerInterface;
#endif

	//----------------------------------------------------------------------

	/// @brief Composite region ('changeTo<>()' into the region acts as 'restart<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Composite			 = CI_<Strategy::Composite,	   THead, TSubStates...>;

	/// @brief Headless composite region ('changeTo<>()' into the region acts as 'restart<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using CompositePeers	 = CI_<Strategy::Composite,    void,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Resumable region ('changeTo<>()' into the region acts as 'resume<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Resumable			  = CI_<Strategy::Resumable,   THead, TSubStates...>;

	/// @brief Headless resumable region ('changeTo<>()' into the region acts as 'resume<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using ResumablePeers	  = CI_<Strategy::Resumable,   void,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	/// @brief Utilitarian region ('changeTo<>()' into the region acts as 'utilize<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Utilitarian		  = CI_<Strategy::Utilitarian, THead, TSubStates...>;

	/// @brief Headless utilitarian region ('changeTo<>()' into the region acts as 'utilize<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using UtilitarianPeers	  = CI_<Strategy::Utilitarian, void,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Random region ('changeTo<>()' into the region acts as 'randomize<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Random			  = CI_<Strategy::RandomUtil,  THead, TSubStates...>;

	/// @brief Headless random region ('changeTo<>()' into the region acts as 'randomize<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using RandomPeers		  = CI_<Strategy::RandomUtil,  void,  TSubStates...>;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Orthogonal region (when activated, activates all sub-states)
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Orthogonal		  = OI_<THead, TSubStates...>;

	/// @brief Headless orthogonal region (when activated, activates all sub-states)
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using OrthogonalPeers	  = OI_<void,  TSubStates...>;

	//----------------------------------------------------------------------
	// COMMON

	/// @brief Root ('changeTo<>()' into the root region acts as 'restart<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Root				  = RF_<Cfg, Composite  <THead, TSubStates...>>;

	/// @brief Headless root ('changeTo<>()' into the root region acts as 'restart<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using PeerRoot			  = RF_<Cfg, CompositePeers  <  TSubStates...>>;

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Resumable root ('changeTo<>()' into the root region acts as 'resume<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using ResumableRoot		  = RF_<Cfg, Resumable  <THead, TSubStates...>>;

	/// @brief Headless resumable root ('changeTo<>()' into the root region acts as 'resume<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using ResumablePeerRoot	  = RF_<Cfg, ResumablePeers  <  TSubStates...>>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	/// @brief Utilitarian root ('changeTo<>()' into the root region acts as 'utilize<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using UtilitarianRoot	  = RF_<Cfg, Utilitarian<THead, TSubStates...>>;

	/// @brief Headless utilitarian root ('changeTo<>()' into the root region acts as 'utilize<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using UtilitarianPeerRoot = RF_<Cfg, UtilitarianPeers<  TSubStates...>>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Random root ('changeTo<>()' into the root region acts as 'randomize<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using RandomRoot		  = RF_<Cfg, Random		<THead, TSubStates...>>;

	/// @brief Headless random root ('changeTo<>()' into the root region acts as 'randomize<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using RandomPeerRoot	  = RF_<Cfg, RandomPeers	 <  TSubStates...>>;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Orthogonal root (all sub-states are active at all times)
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using OrthogonalRoot	  = RF_<Cfg, Orthogonal <THead, TSubStates...>>;

	/// @brief Headless orthogonal root (all sub-states are active at all times)
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using OrthogonalPeerRoot  = RF_<Cfg, OrthogonalPeers <  TSubStates...>>;

	//----------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////

}

/// @brief Type configuration for MachineT<>
using Config = detail::G_<HFSM2_FEATURE_TAG, EmptyContext HFSM2_IF_UTILITY_THEORY(, int8_t, float, RNGT<float>), 4 HFSM2_IF_PLANS(, INVALID_LONG), void>;

/// @brief 'Template namespace' for FSM classes
/// @tparam TConfig 'ConfigT<>' type configuration for MachineT<>
/// @see ConfigT<>
template <typename TConfig = Config>
using MachineT = detail::M_<TConfig>;

/// @brief 'Template namespace' for FSM classes parametrized with default types
using Machine = MachineT<>;

////////////////////////////////////////////////////////////////////////////////

}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename TConfig,
		  typename TApex>
class R_ {
public:
	static constexpr FeatureTag FEATURE_TAG = TConfig::FEATURE_TAG;

	using Context				= typename TConfig::Context;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Rank					= typename TConfig::Rank;
	using Utility				= typename TConfig::Utility;
	using RNG					= typename TConfig::RNG;
#endif

	using Payload				= typename TConfig::Payload;

private:
	using Apex					= TApex;

	using Forward				= RF_<TConfig, Apex>;
	using StateList				= typename Forward::StateList;
	using RegionList			= typename Forward::RegionList;
	using Args					= typename Forward::Args;

	static_assert(Args::STATE_COUNT <  (unsigned) -1, "Too many states in the FSM. Change 'Short' type.");
	static_assert(Args::STATE_COUNT == (unsigned) StateList::SIZE, "STATE_COUNT != StateList::SIZE");

	using MaterialApex			= Material<I_<0, 0, 0, 0>, Args, Apex>;

	using Registry				= RegistryT<Args>;
	using CompoForks			= typename Registry::CompoForks;
	using RegistryBackUp		= typename Registry::BackUp;

	using Control				= ControlT	   <Args>;
	using PlanControl			= PlanControlT <Args>;
	using FullControl			= FullControlT <Args>;
	using GuardControl			= GuardControlT<Args>;

	static constexpr Long	SUBSTITUTION_LIMIT	= Forward::SUBSTITUTION_LIMIT;

#ifdef HFSM2_ENABLE_PLANS
	using PlanData				= PlanDataT<Args>;
#endif

#ifdef HFSM2_ENABLE_SERIALIZATION
	using WriteStream			= typename Args::WriteStream;
	using ReadStream			= typename Args::ReadStream;
#endif

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	static constexpr Long NAME_COUNT = MaterialApex::NAME_COUNT;
#endif

#ifdef HFSM2_ENABLE_PLANS
	static constexpr Long TASK_CAPACITY = Forward::TASK_CAPACITY;
#endif

public:
	using Info					= WrapInfo<Apex>;

	/// @brief Transition
	using Transition			= typename Control::Transition;

	/// @brief Array of transitions
	using TransitionSet			= typename Control::TransitionSet;
	using TransitionSets		= typename Control::TransitionSets;

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY
	using TransitionTargets		= typename Control::TransitionTargets;
#endif

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	using Prefix				= StaticArrayT<wchar_t, Info::REVERSE_DEPTH * 2 + 2>;
	using Prefixes				= StaticArrayT<Prefix, Info::STATE_COUNT>;

	using StructureStateInfos	= typename Args::StructureStateInfos;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger				= typename TConfig::LoggerInterface;
#endif

public:

	//----------------------------------------------------------------------

	HFSM2_INLINE explicit R_(Context& context
						  HFSM2_IF_UTILITY_THEORY(, RNG& rng)
						  HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept;

	HFSM2_INLINE ~R_() noexcept;

	//----------------------------------------------------------------------

	/// @brief Access context
	/// @return context
	HFSM2_INLINE	   Context& context()								  noexcept	{ return _context;									}

	/// @brief Access context
	/// @return context
	HFSM2_INLINE const Context& context()							const noexcept	{ return _context;									}

	//----------------------------------------------------------------------

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()									  noexcept	{ return index<StateList, TState>();				}

	/// @brief Get region identifier for a region type
	/// @tparam TState Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static constexpr RegionID regionId()								  noexcept	{ return (RegionID) index<RegionList, TState>();	}

	//----------------------------------------------------------------------
#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION

private:

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4348) // redefinition of default parameter: parameter 2
#endif

	template <typename TState, bool = contains<StateList, TState>()>
	struct Accessor;

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

	template <typename TState>
	struct Accessor<TState, true> {
		HFSM2_INLINE static		  TState& get(		MaterialApex& apex)	  noexcept	{ return apex.template access<TState>();	}
		HFSM2_INLINE static const TState& get(const MaterialApex& apex)	  noexcept	{ return apex.template access<TState>();	}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
public:

	// if you see..
	// VS	 - error C2027: use of undefined type 'hfsm2::detail::R_<..>::Accessor<TState,false>'
	// Clang - error : implicit instantiation of undefined template 'hfsm2::detail::R_<..>::Accessor<*, false>'
	//
	// .. you're trying to access() a type that is not present in the state machine hierarchy

	/// @brief Access state instance
	/// @tparam TState State type
	/// @return State instance
	template <typename TState>
	HFSM2_INLINE	   TState& access()									  noexcept	{ return Accessor<TState				   >::get(_apex);	}

	/// @brief Access state instance
	/// @tparam TState State type
	/// @return State instance
	template <typename TState>
	HFSM2_INLINE const TState& access()								const noexcept	{ return Accessor<TState				   >::get(_apex);	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#else

public:

	/// @brief Access state instance
	/// @tparam TState State type
	/// @return State instance
	template <typename TState>
	HFSM2_INLINE	   TState& access()									  noexcept	{ return Accessor<TState,	   MaterialApex>{_apex}.get();	}

	/// @brief Access state instance
	/// @tparam TState State type
	/// @return State instance
	template <typename TState>
	HFSM2_INLINE const TState& access()								const noexcept	{ return Accessor<TState, const MaterialApex>{_apex}.get();	}

#endif

	//----------------------------------------------------------------------

	/// @brief Trigger FSM update cycle (recursively call 'update()' on all active states, then process requested transitions)
	void update() noexcept;

	/// @brief Have FSM react to an event (recursively call matching 'react<>()' on all active states, then process requested transitions)
	/// @tparam TEvent Event type
	/// @param event Event to react to
	template <typename TEvent>
	HFSM2_INLINE void react(const TEvent& event)						  noexcept;

	//----------------------------------------------------------------------

	/// @brief Check if a state is active
	/// @param stateId Destination state identifier
	/// @return State active status
	HFSM2_INLINE bool isActive		 (const StateID stateId)		const noexcept	{ return _registry.isActive   (stateId);		}

	/// @brief Check if a state is active
	/// @tparam TState Destination state type
	/// @return State active status
	template <typename TState>
	HFSM2_INLINE bool isActive		 ()								const noexcept	{ return isActive	(stateId<TState>());		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @param stateId Destination state identifier
	/// @return State resumable status
	HFSM2_INLINE bool isResumable	 (const StateID stateId)		const noexcept	{ return _registry.isResumable(stateId);		}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @tparam TState Destination state type
	/// @return State resumable status
	template <typename TState>
	HFSM2_INLINE bool isResumable	 ()								const noexcept	{ return isResumable(stateId<TState>());		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @param stateId Destination state identifier
	/// @return State scheduled status
	HFSM2_INLINE bool isScheduled	 (const StateID stateId)		const noexcept	{ return isResumable(stateId);					}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @tparam TState Destination state type
	/// @return State scheduled status
	template <typename TState>
	HFSM2_INLINE bool isScheduled	 ()								const noexcept	{ return isResumable<TState>();					}

	//------------------------------------------------------------------------------
	// COMMON

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	HFSM2_INLINE void changeTo		 (const StateID stateId)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_INLINE void changeTo		 ()									  noexcept	{ changeTo (stateId<TState>());					}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	HFSM2_INLINE void restart		 (const StateID stateId)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_INLINE void restart		 ()									  noexcept	{ restart  (stateId<TState>());					}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	HFSM2_INLINE void resume		 (const StateID stateId)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_INLINE void resume		 ()									  noexcept	{ resume   (stateId<TState>());					}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void utilize		 (const StateID stateId)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///   with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void utilize		 ()									  noexcept	{ utilize  (stateId<TState>());					}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void randomize		 (const StateID stateId)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///   proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void randomize		 ()									  noexcept	{ randomize(stateId<TState>());					}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId Destination state identifier
	HFSM2_INLINE void schedule		 (const StateID stateId)			  noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_INLINE void schedule		 ()									  noexcept	{ schedule (stateId<TState>());					}

	//------------------------------------------------------------------------------

	/// @brief Check if a state is going to be activated or deactivated
	/// @param stateId Destination state identifier
	/// @return State pending activation/deactivation status
	HFSM2_INLINE bool isPendingChange(const StateID stateId)		const noexcept	{ return _registry.isPendingChange(stateId);	}

	/// @brief Check if a state is going to be activated or deactivated
	/// @tparam TState Destination state type
	/// @return State pending activation/deactivation status
	template <typename TState>
	HFSM2_INLINE bool isPendingChange()									  noexcept	{ return isPendingChange(stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be activated
	/// @param stateId Destination state identifier
	/// @return State pending activation status
	HFSM2_INLINE bool isPendingEnter (const StateID stateId)		const noexcept	{ return _registry.isPendingEnter (stateId);	}

	/// @brief Check if a state is going to be activated
	/// @tparam TState Destination state type
	/// @return State pending activation status
	template <typename TState>
	HFSM2_INLINE bool isPendingEnter ()									  noexcept	{ return isPendingEnter (stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be deactivated
	/// @param stateId Destination state identifier
	/// @return State pending deactivation status
	HFSM2_INLINE bool isPendingExit	 (const StateID stateId)		const noexcept	{ return _registry.isPendingExit  (stateId);	}

	/// @brief Check if a state is going to be deactivated
	/// @tparam TState Destination state type
	/// @return State pending deactivation status
	template <typename TState>
	HFSM2_INLINE bool isPendingExit  ()									  noexcept	{ return isPendingExit  (stateId<TState>());	}

	//------------------------------------------------------------------------------

	/// @brief Reset FSM to initial state (recursively 'exit()' currently active states, 'enter()' initial states)
	void reset() noexcept;

	//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION

	/// @brief Buffer for serialization
	/// @see https://doc.hfsm.dev/user-guide/debugging-and-tools/serialization
	/// @see HFSM2_ENABLE_SERIALIZATION
	using SerialBuffer			= typename Args::SerialBuffer;

	/// @brief Serialize FSM into 'buffer'
	/// @param buffer 'SerialBuffer' to serialize to
	/// @see HFSM2_ENABLE_SERIALIZATION
	void save(		SerialBuffer& buffer)							const noexcept;

	/// @brief De-serialize FSM from 'buffer'
	/// @param buffer 'SerialBuffer' to de-serialize from
	/// @see HFSM2_ENABLE_SERIALIZATION
	void load(const SerialBuffer& buffer)								  noexcept;

#endif

	//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY

	/// @brief Get the list of transitions recorded during last 'update()'
	/// @return Array of last recorded transitions
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	const TransitionSets& previousTransitions()						const noexcept	{ return _previousTransitions;					}

	/// @brief Force process transitions (skips 'guard()' calls)
	///   Can be used to synchronize multiple FSMs
	/// @param transitions Array of 'Transition' to replay
	/// @param count Number of transitions
	/// @return Success status
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	bool replayTransitions(const Transition* const transitions,
						   const Short count)							  noexcept;

	/// @brief Force process transitions (skips 'guard()' calls)
	///   Can be used to synchronize multiple FSMs
	/// @param transitions 'TransitionHistory' to replay
	/// @return Success status
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	template <Long NCount>
	bool replayTransitions(const ArrayT<Transition, NCount>& transitions) noexcept;

	/// @brief Force process a transition (skips 'guard()' calls)
	///   Can be used to synchronize multiple FSMs
	/// @param transition 'Transition' to replay
	/// @return Success status
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	bool replayTransition (const Transition& transition)				  noexcept	{ return replayTransitions(&transition, 1);		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get the last transition that caused the state to be activated
	/// @param stateId State identifier
	/// @return Pointer to the last transition that activated the state
	const Transition* lastTransition(const StateID stateId)			const noexcept;

	/// @brief Get the last transition that caused the state to be activated
	/// @tparam TState State type
	/// @return Pointer to the last transition that activated the state
	template <typename TState>
	const Transition* lastTransition()								const noexcept	{ return lastTransition(stateId<TState>());		}

#endif

	//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

	/// @brief Array of 'StructureEntry' representing FSM structure
	/// @see HFSM2_ENABLE_STRUCTURE_REPORT
	using Structure				= ArrayT<StructureEntry, NAME_COUNT>;

	/// @brief Array of 'char' representing FSM activation history (negative - 'update()' cycles since deactivated, positive - 'update()' cycles since activated)
	/// @see HFSM2_ENABLE_STRUCTURE_REPORT
	using ActivityHistory		= ArrayT<char,			 NAME_COUNT>;

	/// @brief Get the array of 'StructureEntry' representing FSM structure
	/// @return FSM structure
	/// @see HFSM2_ENABLE_STRUCTURE_REPORT
	const Structure&	   structure()								const noexcept	{ return _structure;							}

	/// @brief Get the array of 'char' representing FSM activation history (negative - 'update()' cycles since deactivated, positive - 'update()' cycles since activated)
	/// @return FSM activation history
	/// @see HFSM2_ENABLE_STRUCTURE_REPORT
	const ActivityHistory& activityHistory()						const noexcept	{ return _activityHistory;						}

#endif

	//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_LOG_INTERFACE

	/// @brief Attach logger
	/// @param logger A logger implementing 'hfsm2::LoggerInterfaceT<>' interface
	/// @see HFSM2_ENABLE_LOG_INTERFACE
	HFSM2_INLINE void attachLogger(Logger* const logger)				  noexcept	{ _logger = logger;								}

#endif

	//----------------------------------------------------------------------

private:
	void initialEnter() noexcept;
	void processTransitions(TransitionSets& currentTransitions)			  noexcept;

	bool applyRequest (Control& control, const Transition& request, const Short index) noexcept;
	bool applyRequests(Control& control) noexcept;

	bool cancelledByEntryGuards(const TransitionSets& currentTransitions,
								const TransitionSet&  pendingTransitions) noexcept;

	bool cancelledByGuards(const TransitionSets& currentTransitions,
						   const TransitionSet&  pendingTransitions)	  noexcept;

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY
	bool applyRequests(Control& control,
					   const Transition* const transitions,
					   const Short count)								  noexcept;

	TransitionTargets _transitionTargets;
	TransitionSets _previousTransitions;
#endif

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	void getStateNames() noexcept;
	void udpateActivity() noexcept;

	Prefixes _prefixes;
	StructureStateInfos _stateInfos;

	Structure _structure;
	ActivityHistory _activityHistory;
#endif

protected:
	Context _context;
	HFSM2_IF_UTILITY_THEORY(RNG& _rng);

	Registry _registry;
	HFSM2_IF_PLANS(PlanData _planData);

	TransitionSet _requests;

	MaterialApex _apex;

	HFSM2_IF_LOG_INTERFACE(Logger* _logger);
};

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig,
		  typename TApex>
class RP_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFeatureTag
		, typename TContext

	#ifdef HFSM2_ENABLE_UTILITY_THEORY
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RP_		   <G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 R_<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = R_<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

	using Transition = TransitionT<TPayload>;

public:
	using Payload	 = typename Base::Payload;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using RNG		 = typename Base::RNG;
#endif

public:
	using Base::Base;

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID  stateId()								  noexcept	{ return Base::template stateId<TState>();		}

	/// @brief Get region identifier for a region type
	/// @tparam TState Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static constexpr RegionID regionId()							  noexcept	{ return Base::template regionId<TState>();		}

	//------------------------------------------------------------------------------
	// COMMON

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void changeWith   (const StateID  stateId,
									const Payload& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void changeWith   (const StateID  stateId,
										 Payload&& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void changeWith   (const Payload& payload)			  noexcept	{ changeWith   (stateId<TState>(),			 payload );	}

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void changeWith   (	 Payload&& payload)			  noexcept	{ changeWith   (stateId<TState>(), std::move(payload));	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void restartWith  (const StateID  stateId,
									const Payload& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void restartWith  (const StateID  stateId,
										 Payload&& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void restartWith  (const Payload& payload)			  noexcept	{ restartWith  (stateId<TState>(),			 payload );	}

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void restartWith  (	 Payload&& payload)			  noexcept	{ restartWith  (stateId<TState>(), std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void resumeWith   (const StateID  stateId,
									const Payload& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void resumeWith   (const StateID  stateId,
										 Payload&& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void resumeWith   (const Payload& payload)			  noexcept	{ resumeWith   (stateId<TState>(),			 payload );	}

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void resumeWith   (	 Payload&& payload)			  noexcept	{ resumeWith   (stateId<TState>(), std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void utilizeWith  (const StateID  stateId,
									const Payload& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void utilizeWith  (const StateID  stateId,
										 Payload&& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void utilizeWith  (const Payload& payload)			  noexcept	{ utilizeWith  (stateId<TState>(),			 payload );	}

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void utilizeWith  (	 Payload&& payload)			  noexcept	{ utilizeWith  (stateId<TState>(), std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void randomizeWith(const StateID  stateId,
									const Payload& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void randomizeWith(const StateID  stateId,
										 Payload&& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void randomizeWith(const Payload& payload)			  noexcept	{ randomizeWith(stateId<TState>(),			 payload );	}

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void randomizeWith(	 Payload&& payload)			  noexcept	{ randomizeWith(stateId<TState>(), std::move(payload));	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void scheduleWith (const StateID  stateId,
									const Payload& payload)			  noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void scheduleWith (const StateID  stateId,
										 Payload&& payload)			  noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void scheduleWith (const Payload& payload)			  noexcept	{ scheduleWith (stateId<TState>(),			 payload );	}

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void scheduleWith (	 Payload&& payload)			  noexcept	{ scheduleWith (stateId<TState>(), std::move(payload));	}

	//------------------------------------------------------------------------------

protected:
	using Base::_context;

private:
	using Base::_requests;

	HFSM2_IF_LOG_INTERFACE(using Base::_logger);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFeatureTag
		, typename TContext

	#ifdef HFSM2_ENABLE_UTILITY_THEORY
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TApex>
class RP_		   <G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>
	: public	 R_<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>
{
	using Base = R_<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>;

public:
#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using RNG		= typename Base::RNG;
#endif

public:
	using Base::Base;
};

////////////////////////////////////////////////////////////////////////////////

template <typename, typename>
class RC_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext

template <FeatureTag NFeatureTag
		, typename TContext

	#ifdef HFSM2_ENABLE_UTILITY_THEORY
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RC_			<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RP_<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RP_<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using Context	= TContext;
	using Payload	= typename Base::Payload;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using RNG		= typename Base::RNG;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Base::Logger;
#endif

public:
	HFSM2_INLINE explicit RC_(Context& context
							HFSM2_IF_UTILITY_THEORY(, RNG& rng)
							HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept
		: Base{context
			 HFSM2_IF_UTILITY_THEORY(, rng)
			 HFSM2_IF_LOG_INTERFACE(, logger)}
	{}

	HFSM2_INLINE void setContext(const Context&  context) noexcept { _context =			  context ; }
	HFSM2_INLINE void setContext(	   Context&& context) noexcept { _context = std::move(context); }

private:
	using Base::_context;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext&

template <FeatureTag NFeatureTag
		, typename TContext

	#ifdef HFSM2_ENABLE_UTILITY_THEORY
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RC_			<G_<NFeatureTag, TContext& HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RP_<G_<NFeatureTag, TContext& HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RP_<G_<NFeatureTag, TContext& HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using Context	= typename Base::Context;
	using Payload	= typename Base::Payload;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using RNG		= typename Base::RNG;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Base::Logger;
#endif

public:
	HFSM2_INLINE explicit RC_(Context context
							HFSM2_IF_UTILITY_THEORY(, RNG& rng)
							HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept
		: Base{context
			 HFSM2_IF_UTILITY_THEORY(, rng)
			 HFSM2_IF_LOG_INTERFACE(, logger)}
	{}

	HFSM2_INLINE void setContext(Context context) noexcept { _context = context; }

private:
	using Base::_context;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext*

template <FeatureTag NFeatureTag
		, typename TContext

	#ifdef HFSM2_ENABLE_UTILITY_THEORY
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RC_			<G_<NFeatureTag, TContext* HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RP_<G_<NFeatureTag, TContext* HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RP_<G_<NFeatureTag, TContext* HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using Context	= typename Base::Context;
	using Payload	= typename Base::Payload;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using RNG		= typename Base::RNG;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Base::Logger;
#endif

public:
#ifdef HFSM2_ENABLE_UTILITY_THEORY

	HFSM2_INLINE explicit RC_(Context context
							, RNG& rng
							HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept
		: Base{context
			 , rng
			 HFSM2_IF_LOG_INTERFACE(, logger)}
	{}

#else

	HFSM2_INLINE explicit RC_(Context context = nullptr
							HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept
		: Base{context
			 HFSM2_IF_LOG_INTERFACE(, logger)}
	{}

#endif

	HFSM2_INLINE void setContext(Context context) noexcept { _context = context; }

private:
	using Base::_context;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext == EmptyContext

template <FeatureTag NFeatureTag

	#ifdef HFSM2_ENABLE_UTILITY_THEORY
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RC_			<G_<NFeatureTag, EmptyContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RP_<G_<NFeatureTag, EmptyContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	, public EmptyContext
{
	using Base = RP_<G_<NFeatureTag, EmptyContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using Context	= typename Base::Context;
	using Payload	= typename Base::Payload;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using RNG		= typename Base::RNG;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Base::Logger;
#endif

public:

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	HFSM2_INLINE explicit RC_(RNG& rng
							HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept
		: Base{static_cast<Context&>(*this)
			 , rng
			 HFSM2_IF_LOG_INTERFACE(, logger)}
	{}

#else

	HFSM2_INLINE explicit RC_(HFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr)) noexcept
		: Base{static_cast<Context&>(*this)
			 HFSM2_IF_LOG_INTERFACE(, logger)}
	{}

#endif
};

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename TConfig,
		  typename TApex>
class RR_ final
	: public	 RC_<TConfig, TApex>
{
	using Base = RC_<TConfig, TApex>;

public:
	using Base::Base;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TRNG == RNGT<TUtility>

#ifdef HFSM2_ENABLE_UTILITY_THEORY

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TContext

#ifdef HFSM2_ENABLE_UTILITY_THEORY
		, typename TRank
		, typename TUtility
#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RR_			<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, RNGT<TUtility>), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex> final
	: public	 RC_<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, RNGT<TUtility>), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	HFSM2_IF_UTILITY_THEORY(, public RNGT<TUtility>)
{
	using Base = RC_<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, RNGT<TUtility>), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using Context	= typename Base::Context;
	using Payload	= typename Base::Payload;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using RNG		= typename Base::RNG;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Base::Logger;
#endif

public:
#ifdef HFSM2_ENABLE_UTILITY_THEORY

	HFSM2_INLINE explicit RR_(Context& context
							HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept
		: Base{context
			 , static_cast<RNG&>(*this)
			 HFSM2_IF_LOG_INTERFACE(, logger)}
		, RNG{0}
	{}

#else

	HFSM2_INLINE explicit RR_(Context& context
							HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept
		: Base{context
			 HFSM2_IF_LOG_INTERFACE(, logger)}
	{}

#endif
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext == EmptyContext
// TRNG == RNGT<TUtility>

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TRank
		, typename TUtility
		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RR_			<G_<NFeatureTag, EmptyContext, TRank, TUtility, RNGT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex> final
	: public	 RC_<G_<NFeatureTag, EmptyContext, TRank, TUtility, RNGT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	, public RNGT<TUtility>
{
	using Base = RC_<G_<NFeatureTag, EmptyContext, TRank, TUtility, RNGT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using Context	= typename Base::Context;
	using Payload	= typename Base::Payload;
	using RNG		= typename Base::RNG;

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Base::Logger;
#endif

public:
	HFSM2_INLINE explicit RR_(HFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr)) noexcept
		: Base{static_cast<RNG&>(*this)
			 HFSM2_IF_LOG_INTERFACE(, logger)}
		, RNG{0}
	{}
};

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TG, typename TA>
R_<TG, TA>::R_(Context& context
			   HFSM2_IF_UTILITY_THEORY(, RNG& rng)
			   HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: _context{context}
	HFSM2_IF_UTILITY_THEORY(, _rng{rng})
	HFSM2_IF_LOG_INTERFACE(, _logger{logger})
{
	_apex.deepRegister(_registry, Parent{});

	HFSM2_IF_STRUCTURE_REPORT(getStateNames());

	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
R_<TG, TA>::~R_() noexcept {
	PlanControl control{_context
					  , _registry
					  , _requests
					  HFSM2_IF_UTILITY_THEORY(, _rng)
					  HFSM2_IF_PLANS(, _planData)
					  HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets)
					  HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions)
					  HFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepExit	  (control);
	_apex.deepDestruct(control);

	HFSM2_IF_PLANS(HFSM2_IF_ASSERT(_planData.verifyPlans()));
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::update() noexcept {
	FullControl control{_context
					  , _registry
					  , _requests
					  HFSM2_IF_UTILITY_THEORY(, _rng)
					  HFSM2_IF_PLANS(, _planData)
					  HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets)
					  HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions)
					  HFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepUpdate(control);

	HFSM2_IF_PLANS(HFSM2_IF_ASSERT(_planData.verifyPlans()));

	TransitionSets currentTransitions;
	HFSM2_IF_TRANSITION_HISTORY(_transitionTargets.clear());

	if (_requests.count())
		processTransitions(currentTransitions);

	HFSM2_IF_TRANSITION_HISTORY(_previousTransitions = currentTransitions);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
template <typename TEvent>
void
R_<TG, TA>::react(const TEvent& event) noexcept {
	FullControl control{_context
					  , _registry
					  , _requests
					  HFSM2_IF_UTILITY_THEORY(, _rng)
					  HFSM2_IF_PLANS(, _planData)
					  HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets)
					  HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions)
					  HFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepReact(control, event);

	HFSM2_IF_PLANS(HFSM2_IF_ASSERT(_planData.verifyPlans()));

	TransitionSets currentTransitions;
	HFSM2_IF_TRANSITION_HISTORY(_transitionTargets.clear());

	if (_requests.count())
		processTransitions(currentTransitions);

	HFSM2_IF_TRANSITION_HISTORY(_previousTransitions = currentTransitions);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::changeTo(const StateID stateId) noexcept {
	_requests.append(Transition{stateId, TransitionType::CHANGE});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::CHANGE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::restart(const StateID stateId) noexcept {
	_requests.append(Transition{stateId, TransitionType::RESTART});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RESTART, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::resume(const StateID stateId) noexcept {
	_requests.append(Transition{stateId, TransitionType::RESUME});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RESUME, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TG, typename TA>
void
R_<TG, TA>::utilize(const StateID stateId) noexcept {
	_requests.append(Transition{stateId, TransitionType::UTILIZE});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::UTILIZE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::randomize(const StateID stateId) noexcept {
	_requests.append(Transition{stateId, TransitionType::RANDOMIZE});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RANDOMIZE, stateId);
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::schedule(const StateID stateId) noexcept {
	_requests.append(Transition{stateId, TransitionType::SCHEDULE});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::SCHEDULE, stateId);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::reset() noexcept {
	PlanControl control{_context
					  , _registry
					  , _requests
					  HFSM2_IF_UTILITY_THEORY(, _rng)
					  HFSM2_IF_PLANS(, _planData)
					  HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets)
					  HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions)
					  HFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepExit	  (control);
	_apex.deepDestruct(control);

	HFSM2_IF_TRANSITION_HISTORY(_transitionTargets.clear());
	HFSM2_IF_TRANSITION_HISTORY(_previousTransitions.clear());

	_registry.clear();
	// TODO: clear _requests
	// TODO: clear _rng					// HFSM2_IF_UTILITY_THEORY()
	// TODO: clear _planData			// HFSM2_IF_PLANS()
	// TODO: clear _activityHistory		// HFSM2_IF_STRUCTURE_REPORT()

	_apex.deepRequestChange(control, {TransitionType::RESTART, INVALID_SHORT});
	_apex.deepConstruct	   (control);
	_apex.deepEnter		   (control);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION

template <typename TG, typename TA>
void
R_<TG, TA>::save(SerialBuffer& _buffer) const noexcept {
	WriteStream stream{_buffer};

	// TODO: save _registry
	// TODO: save _requests
	// TODO: save _rng						// HFSM2_IF_UTILITY_THEORY()
	// TODO: save _planData					// HFSM2_IF_PLANS()
	// TODO: save _previousTransitions		// HFSM2_IF_TRANSITION_HISTORY()
	// TODO: save _activityHistory			// HFSM2_IF_STRUCTURE_REPORT()

	_apex.deepSaveActive(_registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::load(const SerialBuffer& buffer) noexcept {
	_requests.clear();

	PlanControl control{_context
					  , _registry
					  , _requests
					  HFSM2_IF_UTILITY_THEORY(, _rng)
					  HFSM2_IF_PLANS(, _planData)
					  HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets)
					  HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions)
					  HFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepExit	  (control);
	_apex.deepDestruct(control);

	HFSM2_IF_TRANSITION_HISTORY(_transitionTargets.clear());
	HFSM2_IF_TRANSITION_HISTORY(_previousTransitions.clear());

	_registry.clear();
	_requests.clear();

	ReadStream stream{buffer};

	// TODO: load _registry
	// TODO: load _requests
	// TODO: load _rng				// HFSM2_IF_UTILITY_THEORY()
	// TODO: load _planData			// HFSM2_IF_PLANS()
	// TODO: load _previousTransitions	// HFSM2_IF_TRANSITION_HISTORY()
	// TODO: load _activityHistory	// HFSM2_IF_STRUCTURE_REPORT()

	_apex.deepLoadRequested(_registry, stream);

	_apex.deepConstruct(control);
	_apex.deepEnter	   (control);
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY

template <typename TG, typename TA>
bool
R_<TG, TA>::replayTransitions(const Transition* const transitions,
							  const Short count) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(_transitionTargets.clear());
	HFSM2_IF_TRANSITION_HISTORY(_previousTransitions.clear());

	if (HFSM2_CHECKED(transitions && count)) {
		PlanControl control{_context
						  , _registry
						  , _requests
						  HFSM2_IF_UTILITY_THEORY(, _rng)
						  HFSM2_IF_PLANS(, _planData)
						  HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets)
						  HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions)
						  HFSM2_IF_LOG_INTERFACE(, _logger)};

		if (HFSM2_CHECKED(applyRequests(control, transitions, count))) {
		#ifdef HFSM2_ENABLE_TRANSITION_HISTORY
			for (Short i = 0; i < count; ++i)
				_previousTransitions.append(transitions[i]);
		#endif

			_apex.deepChangeToRequested(control);

			_registry.clearRequests();

			HFSM2_IF_PLANS(HFSM2_IF_ASSERT(_planData.verifyPlans()));
			HFSM2_IF_STRUCTURE_REPORT(udpateActivity());

			return true;
		}
	}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
template <Long NCount>
bool
R_<TG, TA>::replayTransitions(const ArrayT<Transition, NCount>& transitions) noexcept {
	if (transitions.count())
		return replayTransitions(&transitions[0],
								 transitions.count());
	else
		return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
const typename R_<TG, TA>::Transition*
R_<TG, TA>::lastTransition(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < StateList::SIZE)) {
		const Short index = _transitionTargets[stateId];

		if (index < _previousTransitions.count())
			return &_previousTransitions[index];
	}

	return nullptr;
}

#endif

//------------------------------------------------------------------------------
// COMMON

template <typename TG, typename TA>
void
R_<TG, TA>::initialEnter() noexcept {
	// TODO:
	//HFSM2_ASSERT(_registry.empty() == 0);
	HFSM2_ASSERT(_requests.count() == 0);
	HFSM2_IF_TRANSITION_HISTORY(_transitionTargets.clear());
	HFSM2_IF_TRANSITION_HISTORY(HFSM2_ASSERT(_previousTransitions.count() == 0));

	RegistryBackUp undo;

	PlanControl control{_context
					  , _registry
					  , _requests
					  HFSM2_IF_UTILITY_THEORY(, _rng)
					  HFSM2_IF_PLANS(, _planData)
					  HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets)
					  HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions)
					  HFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepRequestChange(control, {TransitionType::RESTART, INVALID_SHORT});

	TransitionSets currentTransitions;
	TransitionSet  pendingTransitions;

	cancelledByEntryGuards(currentTransitions,
						   pendingTransitions);

	for (Long i = 0;
		 i < SUBSTITUTION_LIMIT && _requests.count();
		 ++i)
	{
		backup(_registry, undo);

		if (HFSM2_CHECKED(applyRequests(control))) {
			pendingTransitions = _requests;
			_requests.clear();

			if (cancelledByEntryGuards(currentTransitions,
									   pendingTransitions))
			{
				HFSM2_BREAK();

				restore(_registry, undo);
			} else
				currentTransitions += pendingTransitions;

			pendingTransitions.clear();
		} else
			_requests.clear();
	}
	HFSM2_ASSERT(_requests.count() == 0);
	HFSM2_IF_TRANSITION_HISTORY(_previousTransitions = currentTransitions);

	_apex.deepConstruct(control);
	_apex.deepEnter	   (control);

	_registry.clearRequests();

	HFSM2_IF_PLANS(HFSM2_IF_ASSERT(_planData.verifyPlans()));
	HFSM2_IF_STRUCTURE_REPORT(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::processTransitions(TransitionSets& currentTransitions) noexcept {
	HFSM2_ASSERT(_requests.count());

	RegistryBackUp registryUndo;

	PlanControl control{_context
					  , _registry
					  , _requests
					  HFSM2_IF_UTILITY_THEORY(, _rng)
					  HFSM2_IF_PLANS(, _planData)
					  HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets)
					  HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions)
					  HFSM2_IF_LOG_INTERFACE(, _logger)};

	TransitionSet pendingTransitions;

	for (Long i = 0;
		i < SUBSTITUTION_LIMIT && _requests.count();
		++i)
	{
		backup(_registry, registryUndo);

		if (applyRequests(control)) {
			pendingTransitions = _requests;
			_requests.clear();

			if (cancelledByGuards(currentTransitions, pendingTransitions)) {
				HFSM2_IF_TRANSITION_HISTORY(_transitionTargets.clear());
				restore(_registry, registryUndo);
			} else
				currentTransitions += pendingTransitions;

			pendingTransitions.clear();
		} else
			_requests.clear();
	}
	HFSM2_ASSERT(_requests.count() == 0);

	if (currentTransitions.count())
		_apex.deepChangeToRequested(control);

	_registry.clearRequests();

	HFSM2_IF_PLANS(HFSM2_IF_ASSERT(_planData.verifyPlans()));
	HFSM2_IF_STRUCTURE_REPORT(udpateActivity());
}

// COMMON
//------------------------------------------------------------------------------

template <typename TG, typename TA>
bool
R_<TG, TA>::applyRequest(Control& control,
						 const Transition& request,
						 const Short index) noexcept
{
	switch (request.type) {
	case TransitionType::CHANGE:
	case TransitionType::RESTART:
	case TransitionType::RESUME:

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	case TransitionType::UTILIZE:
	case TransitionType::RANDOMIZE:
#endif

		if (_registry.requestImmediate(request))
			_apex.deepForwardActive(control, {request.type, index});
		else
			_apex.deepRequest	   (control, {request.type, index});

		return true;

	case TransitionType::SCHEDULE:
		_registry.requestScheduled(request.destination);

		return false;

	default:
		HFSM2_BREAK();

		return false;
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
bool
R_<TG, TA>::applyRequests(Control& control) noexcept {
	bool changesMade = false;

	for (Short i = 0; i < _requests.count(); ++i)
		changesMade |= applyRequest(control, _requests[i], i);

	return changesMade;
}

//------------------------------------------------------------------------------
// COMMON

template <typename TG, typename TA>
bool
R_<TG, TA>::cancelledByEntryGuards(const TransitionSets& currentTransitions,
								   const TransitionSet&  pendingTransitions) noexcept
{
	GuardControl guardControl{_context
							, _registry
							, _requests
							, currentTransitions
							, pendingTransitions
							HFSM2_IF_UTILITY_THEORY(, _rng)
							HFSM2_IF_PLANS(, _planData)
							HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets)
							HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions)
							HFSM2_IF_LOG_INTERFACE(, _logger)};

	return _apex.deepEntryGuard(guardControl);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
bool
R_<TG, TA>::cancelledByGuards(const TransitionSets& currentTransitions,
							  const TransitionSet&  pendingTransitions) noexcept
{
	GuardControl guardControl{_context
							, _registry
							, _requests
							, currentTransitions
							, pendingTransitions
							HFSM2_IF_UTILITY_THEORY(, _rng)
							HFSM2_IF_PLANS(, _planData)
							HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets)
							HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions)
							HFSM2_IF_LOG_INTERFACE(, _logger)};

	return _apex.deepForwardExitGuard(guardControl) ||
		   _apex.deepForwardEntryGuard(guardControl);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY

template <typename TG, typename TA>
bool
R_<TG, TA>::applyRequests(Control& control,
						  const Transition* const transitions,
						  const Short count) noexcept
{
	if (HFSM2_CHECKED(transitions && count)) {
		bool changesMade = false;

		for (Short i = 0; i < count; ++i)
			changesMade |= applyRequest(control, transitions[i], i);

		return changesMade;
	} else
		return false;
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

template <typename TG, typename TA>
void
R_<TG, TA>::getStateNames() noexcept {
	_stateInfos.clear();
	_apex.deepGetNames((Long) -1, StructureStateInfo::RegionType::COMPOSITE, 0, _stateInfos);

	Long margin = (Long) -1;
	for (Long s = 0; s < _stateInfos.count(); ++s) {
		const auto& state = _stateInfos[s];
		auto& prefix      = _prefixes[s];

		if (margin > state.depth && state.name[0] != '\0')
			margin = state.depth;

		if (state.depth == 0)
			prefix[0] = L'\0';
		else {
			const Long mark = state.depth * 2 - 1;

			prefix[mark + 0] = state.regionType == StructureStateInfo::RegionType::COMPOSITE ? L'└' : L'╙';
			prefix[mark + 1] = L' ';
			prefix[mark + 2] = L'\0';

			for (auto d = mark; d > 0; --d)
				prefix[d - 1] = L' ';

			for (auto r = s; r > state.parent; --r) {
				auto& prefixAbove = _prefixes[r - 1];

				switch (prefixAbove[mark]) {
				case L' ':
					prefixAbove[mark] = state.regionType == StructureStateInfo::RegionType::COMPOSITE ? L'│' : L'║';
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
	for (Long s = 0; s < _stateInfos.count(); ++s) {
		const auto& state = _stateInfos[s];
		auto& prefix = _prefixes[s];
		const Long space = state.depth * 2;

		if (state.name[0] != L'\0') {
			_structure.append(StructureEntry{false, &prefix[margin * 2], state.name});
			_activityHistory.append((char) 0);
		} else if (s + 1 < _stateInfos.count()) {
			auto& nextPrefix = _prefixes[s + 1];

			if (s > 0)
				for (Long c = 0; c <= space; ++c)
					nextPrefix[c] = prefix[c];

			const Long mark = space + 1;

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

template <typename TG, typename TA>
void
R_<TG, TA>::udpateActivity() noexcept {
	for (Long s = 0, i = 0; s < _stateInfos.count(); ++s)
		if (_stateInfos[s].name[0] != L'\0') {
			_structure[i].isActive = isActive(s);

			auto& activity = _activityHistory[i];

			if (_structure[i].isActive) {
				if (activity < 0)
					activity = +1;
				else
					activity = activity < INT8_MAX ? activity + 1 : activity;
			} else {
				if (activity > 0)
					activity = -1;
				else
					activity = activity > INT8_MIN ? activity - 1 : activity;
			}

			++i;
		}
}

#endif

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <FeatureTag NFT, typename TC HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
void
RP_<G_<NFT, TC HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::changeWith(const StateID  stateId,
																									  const Payload& payload) noexcept
{
	_requests.append(Transition{stateId, TransitionType::CHANGE, payload});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::CHANGE, stateId);
}

template <FeatureTag NFT, typename TC HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
void
RP_<G_<NFT, TC HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::changeWith(const StateID  stateId,
																										   Payload&& payload) noexcept
{
	_requests.append(Transition{stateId, TransitionType::CHANGE, std::move(payload)});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::CHANGE, stateId);
}

// COMMON
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
void
RP_<G_<NFT, TC HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::restartWith(const StateID  stateId,
																									   const Payload& payload) noexcept
{
	_requests.append(Transition{stateId, TransitionType::RESTART, payload});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RESTART, stateId);
}

template <FeatureTag NFT, typename TC HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
void
RP_<G_<NFT, TC HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::restartWith(const StateID  stateId,
																											Payload&& payload) noexcept
{
	_requests.append(Transition{stateId, TransitionType::RESTART, std::move(payload)});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RESTART, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
void
RP_<G_<NFT, TC HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::resumeWith(const StateID  stateId,
																									  const Payload& payload) noexcept
{
	_requests.append(Transition{stateId, TransitionType::RESUME, payload});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RESUME, stateId);
}

template <FeatureTag NFT, typename TC HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
void
RP_<G_<NFT, TC HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::resumeWith(const StateID  stateId,
																										   Payload&& payload) noexcept
{
	_requests.append(Transition{stateId, TransitionType::RESUME, std::move(payload)});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RESUME, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <FeatureTag NFT, typename TC HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
void
RP_<G_<NFT, TC HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::utilizeWith(const StateID  stateId,
																									   const Payload& payload) noexcept
{
	_requests.append(Transition{stateId, TransitionType::UTILIZE, payload});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::UTILIZE, stateId);
}

template <FeatureTag NFT, typename TC HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
void
RP_<G_<NFT, TC HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::utilizeWith(const StateID  stateId,
																											Payload&& payload) noexcept
{
	_requests.append(Transition{stateId, TransitionType::UTILIZE, std::move(payload)});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::UTILIZE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
void
RP_<G_<NFT, TC HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::randomizeWith(const StateID  stateId,
																										 const Payload& payload) noexcept
{
	_requests.append(Transition{stateId, TransitionType::RANDOMIZE, payload});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RANDOMIZE, stateId);
}

template <FeatureTag NFT, typename TC HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
void
RP_<G_<NFT, TC HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::randomizeWith(const StateID  stateId,
																											  Payload&& payload) noexcept
{
	_requests.append(Transition{stateId, TransitionType::RANDOMIZE, std::move(payload)});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RANDOMIZE, stateId);
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
void
RP_<G_<NFT, TC HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::scheduleWith(const StateID  stateId,
																										const Payload& payload) noexcept
{
	_requests.append(Transition{stateId, TransitionType::SCHEDULE, payload});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::SCHEDULE, stateId);
}

template <FeatureTag NFT, typename TC HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
void
RP_<G_<NFT, TC HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::scheduleWith(const StateID  stateId,
																											 Payload&& payload) noexcept
{
	_requests.append(Transition{stateId, TransitionType::SCHEDULE, std::move(payload)});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::SCHEDULE, stateId);
}

////////////////////////////////////////////////////////////////////////////////

}
}

//------------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
#endif

#ifdef __clang__
	#pragma clang diagnostic pop
#endif

////////////////////////////////////////////////////////////////////////////////

#undef HFSM2_INLINE

#undef HFSM2_64BIT_OR_32BIT

//#undef HFSM2_BREAK

#undef HFSM2_IF_DEBUG
#undef HFSM2_UNLESS_DEBUG
#undef HFSM2_DEBUG_OR

#undef HFSM2_IF_ASSERT
#undef HFSM2_CHECKED
#undef HFSM2_ASSERT
#undef HFSM2_ASSERT_OR

#undef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION

#undef HFSM2_IF_TYPEINDEX
#undef HFSM2_IF_UTILITY_THEORY
#undef HFSM2_IF_PLANS
#undef HFSM2_IF_SERIALIZATION
#undef HFSM2_IF_TRANSITION_HISTORY
#undef HFSM2_IF_STRUCTURE_REPORT
#undef HFSM2_IF_LOG_INTERFACE

#undef HFSM2_LOG_TRANSITION
#undef HFSM2_LOG_TASK_STATUS
#undef HFSM2_LOG_PLAN_STATUS
#undef HFSM2_LOG_CANCELLED_PENDING

#undef HFSM2_LOG_STATE_METHOD

////////////////////////////////////////////////////////////////////////////////
