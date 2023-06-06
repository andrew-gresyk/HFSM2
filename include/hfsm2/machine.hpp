// HFSM2 (hierarchical state machine for games and interactive applications)
// 2.2.3 (2023-02-07)
//
// Created by Andrew Gresyk
//
// Licensed under the MIT License;
// you may not use this file except in compliance with the License.
//
//
// MIT License
//
// Copyright (c) 2023
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

#define HFSM2_VERSION_MAJOR 2
#define HFSM2_VERSION_MINOR 2
#define HFSM2_VERSION_PATCH 3

#define HFSM2_VERSION (10000 * HFSM2_VERSION_MAJOR + 100 * HFSM2_VERSION_MINOR + HFSM2_VERSION_PATCH)

#include <stdint.h>			// uint32_t, uint64_t
#include <string.h>			// memcpy_s()

#include <new>
#ifndef HFSM2_DISABLE_TYPEINDEX
	#include <typeindex>
#endif
#include <type_traits>		// std::aligned_storage<>

#if defined _DEBUG && _MSC_VER
	#include <intrin.h>		// __debugbreak()
#endif


#define HFSM2_UNUSED(x)

#define HFSM2_ATTRIBUTE(A)								   HFSM2_ATTRIBUTE_##A()

#ifndef __has_cpp_attribute
	#define __has_cpp_attribute(x)											   0
#endif

#if __has_cpp_attribute(fallthrough)
	#define HFSM2_ATTRIBUTE_FALLTHROUGH()						 [[fallthrough]]
#elif __has_cpp_attribute(clang::fallthrough)
	#define HFSM2_ATTRIBUTE_FALLTHROUGH()				  [[clang::fallthrough]]
#elif __has_cpp_attribute(gnu::fallthrough)
	#define HFSM2_ATTRIBUTE_FALLTHROUGH()					[[gnu::fallthrough]]
#else
	#define HFSM2_ATTRIBUTE_FALLTHROUGH()
#endif

#if __has_cpp_attribute(no_unique_address)
	#define HFSM2_ATTRIBUTE_NO_UNIQUE_ADDRESS()			   [[no_unique_address]]
#else
	#define HFSM2_ATTRIBUTE_NO_UNIQUE_ADDRESS()
#endif

#define HFSM2_CONSTEXPR(A)								   HFSM2_CONSTEXPR_##A()

#define HFSM2_CONSTEXPR_NO()

#define HFSM2_CONSTEXPR_11()										   constexpr

#if __cplusplus >= 201402L
	#define HFSM2_CONSTEXPR_14()									   constexpr
#else
	#define HFSM2_CONSTEXPR_14()										  inline
#endif

#if __cplusplus >= 201703L
	#define HFSM2_CONSTEXPR_17()									   constexpr
#else
	#define HFSM2_CONSTEXPR_17()										  inline
#endif

#if __cplusplus >= 202002L
	#define HFSM2_CONSTEXPR_20()									   constexpr
#else
	#define HFSM2_CONSTEXPR_20()										  inline
#endif

#if defined _MSC_VER
	#define HFSM2_EMPTY_BASES							 __declspec(empty_bases)
#else
	#define HFSM2_EMPTY_BASES
#endif

#define HFSM2_ARCHITECTURE(A)							HFSM2_ARCHITECTURE_##A()

#if INTPTR_MAX == INT64_MAX
	#define HFSM2_ARCHITECTURE_64()											true
	#define HFSM2_ARCHITECTURE_32()										   false

	#define HFSM2_64BIT_OR_32BIT(p64, p32)									 p64
#elif INTPTR_MAX == INT32_MAX
	#define HFSM2_ARCHITECTURE_64()										   false
	#define HFSM2_ARCHITECTURE_32()											true

	#define HFSM2_64BIT_OR_32BIT(p64, p32)									 p32
#else
	#error
#endif

#if defined _DEBUG && defined _MSC_VER
	#define HFSM2_BREAK()										  __debugbreak()
	#define HFSM2_BREAK_AVAILABLE()											true
#else
	#define HFSM2_BREAK()											  ((void) 0)
	#define HFSM2_BREAK_AVAILABLE()										   false
#endif

#ifdef _DEBUG
	#define HFSM2_IF_DEBUG(...)										 __VA_ARGS__
	#define HFSM2_UNLESS_DEBUG(...)
	#define HFSM2_DEBUG_OR(y, n)											   y
#else
	#define HFSM2_IF_DEBUG(...)
	#define HFSM2_UNLESS_DEBUG(...)									 __VA_ARGS__
	#define HFSM2_DEBUG_OR(y, n)											   n
#endif

#if HFSM2_BREAK_AVAILABLE() && defined HFSM2_ENABLE_ASSERT
	#define HFSM2_ASSERT_AVAILABLE()										true
	#define HFSM2_IF_ASSERT(...)									 __VA_ARGS__
	#define HFSM2_CHECKED(x)					   (!!(x) || (HFSM2_BREAK(), 0))
	#define HFSM2_ASSERT(x)						   (!!(x) || (HFSM2_BREAK(), 0))
	#define HFSM2_ASSERT_OR(y, n)											   y
#else
	#define HFSM2_ASSERT_AVAILABLE()									   false
	#define HFSM2_IF_ASSERT(...)
	#define HFSM2_CHECKED(x)												   x
	#define HFSM2_ASSERT(x)											  ((void) 0)
	#define HFSM2_ASSERT_OR(y, n)											   n
#endif

#if defined _MSC_VER || defined __clang_major__ && __clang_major__ >= 7
	#define HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()				true
#else
	#define HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()			   false
#endif

#ifdef HFSM2_ENABLE_ALL
	#define HFSM2_ENABLE_DEBUG_STATE_TYPE
	#define HFSM2_ENABLE_PLANS
	#define HFSM2_ENABLE_SERIALIZATION
	#define HFSM2_ENABLE_STRUCTURE_REPORT
	#define HFSM2_ENABLE_TRANSITION_HISTORY
	#define HFSM2_ENABLE_UTILITY_THEORY
#endif

#ifndef HFSM2_DISABLE_TYPEINDEX
	#define HFSM2_TYPEINDEX_AVAILABLE()										true
	#define HFSM2_IF_TYPEINDEX(...)									 __VA_ARGS__
	#define HFSM2_TYPEINDEX_MASK										(1 << 0)
#else
	#define HFSM2_TYPEINDEX_AVAILABLE()									   false
	#define HFSM2_IF_TYPEINDEX(...)
	#define HFSM2_TYPEINDEX_MASK										(0 << 0)
#endif

#ifdef HFSM2_ENABLE_DEBUG_STATE_TYPE
	#define HFSM2_DEBUG_STATE_TYPE_AVAILABLE()								true
	#define HFSM2_DEBUG_STATE_TYPE_MASK									(1 << 1)
#else
	#define HFSM2_DEBUG_STATE_TYPE_AVAILABLE()							   false
	#define HFSM2_DEBUG_STATE_TYPE_MASK									(0 << 1)
#endif

#ifdef HFSM2_ENABLE_PLANS
	#define HFSM2_PLANS_AVAILABLE()											true
	#define HFSM2_IF_PLANS(...)										 __VA_ARGS__
	#define HFSM2_PLANS_MASK											(1 << 2)
#else
	#define HFSM2_PLANS_AVAILABLE()										   false
	#define HFSM2_IF_PLANS(...)
	#define HFSM2_PLANS_MASK											(0 << 2)
#endif

#ifdef HFSM2_ENABLE_SERIALIZATION
	#define HFSM2_SERIALIZATION_AVAILABLE()									true
	#define HFSM2_IF_SERIALIZATION(...)								 __VA_ARGS__
	#define HFSM2_SERIALIZATION_MASK									(1 << 3)
#else
	#define HFSM2_SERIALIZATION_AVAILABLE()								   false
	#define HFSM2_IF_SERIALIZATION(...)
	#define HFSM2_SERIALIZATION_MASK									(0 << 3)
#endif

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	#define HFSM2_STRUCTURE_REPORT_AVAILABLE()								true
	#define HFSM2_IF_STRUCTURE_REPORT(...)							 __VA_ARGS__
	#define HFSM2_STRUCTURE_REPORT_MASK									(1 << 4)
#else
	#define HFSM2_STRUCTURE_REPORT_AVAILABLE()							   false
	#define HFSM2_IF_STRUCTURE_REPORT(...)
	#define HFSM2_STRUCTURE_REPORT_MASK									(0 << 4)
#endif

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY
	#define HFSM2_TRANSITION_HISTORY_AVAILABLE()							true
	#define HFSM2_IF_TRANSITION_HISTORY(...)						 __VA_ARGS__
	#define HFSM2_TRANSITION_HISTORY_MASK								(1 << 5)
#else
	#define HFSM2_TRANSITION_HISTORY_AVAILABLE()						   false
	#define HFSM2_IF_TRANSITION_HISTORY(...)
	#define HFSM2_TRANSITION_HISTORY_MASK								(0 << 5)
#endif

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	#define HFSM2_UTILITY_THEORY_AVAILABLE()								true
	#define HFSM2_IF_UTILITY_THEORY(...)							 __VA_ARGS__
	#define HFSM2_UTILITY_THEORY_MASK									(1 << 6)
#else
	#define HFSM2_UTILITY_THEORY_AVAILABLE()							   false
	#define HFSM2_IF_UTILITY_THEORY(...)
	#define HFSM2_UTILITY_THEORY_MASK									(0 << 6)
#endif

#ifdef HFSM2_ENABLE_VERBOSE_DEBUG_LOG
	#define HFSM2_ENABLE_LOG_INTERFACE

	#define HFSM2_VERBOSE_DEBUG_LOG_AVAILABLE()								true
	#define HFSM2_VERBOSE_DEBUG_LOG_MASK								(1 << 7)
#else
	#define HFSM2_VERBOSE_DEBUG_LOG_AVAILABLE()							   false
	#define HFSM2_VERBOSE_DEBUG_LOG_MASK								(0 << 7)
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	#define HFSM2_LOG_INTERFACE_AVAILABLE()									true
	#define HFSM2_IF_LOG_INTERFACE(...)								 __VA_ARGS__
	#define HFSM2_LOG_INTERFACE_MASK									(1 << 8)

	#define HFSM2_LOG_TRANSITION(CONTEXT, ORIGIN, TYPE, DESTINATION)		   \
		if (_core.logger)													   \
			_core.logger->recordTransition(CONTEXT, ORIGIN, TYPE, DESTINATION)

#if HFSM2_PLANS_AVAILABLE()
	#define HFSM2_LOG_TASK_STATUS(CONTEXT, REGION, ORIGIN, STATUS)			   \
		if (_core.logger)													   \
			_core.logger->recordTaskStatus(CONTEXT, REGION, ORIGIN, STATUS)

	#define HFSM2_LOG_PLAN_STATUS(CONTEXT, REGION, STATUS)					   \
		if (_core.logger)													   \
			_core.logger->recordPlanStatus(CONTEXT, REGION, STATUS)
#endif

	#define HFSM2_LOG_CANCELLED_PENDING(CONTEXT, ORIGIN)					   \
		if (_core.logger)													   \
			_core.logger->recordCancelledPending(CONTEXT, ORIGIN)

	#define HFSM2_LOG_SELECT_RESOLUTION(CONTEXT, HEAD, PRONG)				   \
		if (auto* const logger = control._core.logger)						   \
			logger->recordSelectResolution(CONTEXT, HEAD, PRONG)

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	#define HFSM2_LOG_UTILITY_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)		   \
		if (auto* const logger = control._core.logger)						   \
			logger->recordUtilityResolution(CONTEXT, HEAD, PRONG, UTILITY)

	#define HFSM2_LOG_RANDOM_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)		   \
		if (auto* const logger = control._core.logger)						   \
			logger->recordRandomResolution(CONTEXT, HEAD, PRONG, UTILITY)
#endif

#else

	#define HFSM2_LOG_INTERFACE_AVAILABLE()								   false
	#define HFSM2_IF_LOG_INTERFACE(...)
	#define HFSM2_LOG_INTERFACE_MASK									(0 << 8)

	#define HFSM2_LOG_TRANSITION(CONTEXT, ORIGIN, TYPE, DESTINATION)

#if HFSM2_PLANS_AVAILABLE()
	#define HFSM2_LOG_TASK_STATUS(CONTEXT, REGION, ORIGIN, STATUS)
	#define HFSM2_LOG_PLAN_STATUS(CONTEXT, REGION, STATUS)
#endif

	#define HFSM2_LOG_CANCELLED_PENDING(CONTEXT, ORIGIN)
	#define HFSM2_LOG_SELECT_RESOLUTION(CONTEXT, HEAD, PRONG)

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	#define HFSM2_LOG_UTILITY_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)
	#define HFSM2_LOG_RANDOM_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)
#endif

#endif

#if HFSM2_VERBOSE_DEBUG_LOG_AVAILABLE()

	#define HFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)						   \
		if (auto* const logger = control._core.logger)						   \
			logger->recordMethod(control.context(), STATE_ID, METHOD_ID)

#elif HFSM2_LOG_INTERFACE_AVAILABLE()

	#define HFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)						   \
		if (auto* const logger = control._core.logger)						   \
			log(METHOD, *logger, control.context(), METHOD_ID)

#else

	#define HFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)

#endif

namespace hfsm2 {

using FeatureTag = uint16_t;

constexpr FeatureTag HFSM2_FEATURE_TAG = HFSM2_TYPEINDEX_MASK
									   | HFSM2_DEBUG_STATE_TYPE_MASK
									   | HFSM2_PLANS_MASK
									   | HFSM2_SERIALIZATION_MASK
									   | HFSM2_STRUCTURE_REPORT_MASK
									   | HFSM2_TRANSITION_HISTORY_MASK
									   | HFSM2_UTILITY_THEORY_MASK
									   | HFSM2_VERBOSE_DEBUG_LOG_MASK
									   | HFSM2_LOG_INTERFACE_MASK;

}

#undef HFSM2_TYPEINDEX_MASK
#undef HFSM2_DEBUG_STATE_TYPE_MASK
#undef HFSM2_PLANS_MASK
#undef HFSM2_SERIALIZATION_MASK
#undef HFSM2_STRUCTURE_REPORT_MASK
#undef HFSM2_TRANSITION_HISTORY_MASK
#undef HFSM2_UTILITY_THEORY_MASK
#undef HFSM2_VERBOSE_DEBUG_LOG_MASK
#undef HFSM2_LOG_INTERFACE_MASK

#if _MSC_VER == 1900
	#pragma warning(push)
	#pragma warning(disable: 4814) // in C++14 'constexpr' will not imply 'const'; consider explicitly specifying 'const'
#endif

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wextra-semi" // error : extra ';' inside a class
	#pragma clang diagnostic ignored "-Wconstexpr-not-const" // error: 'constexpr' non-static member function will not be implicitly 'const' in C++14; add 'const' to avoid a change in behavior
#endif

#if defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic" // error : extra ';'
#endif


namespace hfsm2 {

struct EmptyContext {};
struct EmptyPayload final {};

struct Automatic;
struct Manual;

using Short		 = uint8_t;
static constexpr Short		INVALID_SHORT		= UINT8_MAX;

using RegionID	 = Short;
static constexpr RegionID	INVALID_REGION_ID	= INVALID_SHORT;

using ForkID	 = int8_t;
static constexpr ForkID		INVALID_FORK_ID		= INT8_MIN;

using Long		 = uint16_t;
static constexpr Long		INVALID_LONG		= UINT16_MAX;

using StateID	 = Long;
static constexpr StateID	INVALID_STATE_ID	= INVALID_LONG;

template <bool B,
		  typename TT,
		  typename TF>
struct ConditionalT final {
	using Type = TT;
};

template <typename TT,
		  typename TF>
struct ConditionalT<false, TT, TF> final {
	using Type = TF;
};

template <bool B,
		  typename TT,
		  typename TF>
using Conditional = typename ConditionalT<B, TT, TF>::Type;

template <typename T>
struct RemoveConstT final {
	using Type = T;
};

template <typename T>
struct RemoveConstT<const T> final {
	using Type = T;
};

template <typename T>
using RemoveConst = typename RemoveConstT<T>::Type;

template <typename T>
struct RemoveReferenceT final {
	using Type = T;
};

template <typename T>
struct RemoveReferenceT<T&> final {
	using Type = T;
};

template <typename T>
struct RemoveReferenceT<T&&> final {
	using Type = T;
};

template <typename T>
using RemoveReference = typename RemoveReferenceT<T>::Type;

template <typename T>
using Undecorate = RemoveConst<RemoveReference<T>>;

template <typename T>
struct IsValueReferenceT final {
	static const bool VALUE = false;
};

template <typename T>
struct IsValueReferenceT<T&> final {
	static const bool VALUE = true;
};

template <uint64_t N>
using UIndex = Conditional<N <= (1ull <<  8),  uint8_t,
			   Conditional<N <= (1ull << 16), uint16_t,
			   Conditional<N <= (1ull << 32), uint32_t,
												  void>>>;

template <uint64_t N>
using UCapacity = Conditional<N < (1ull <<  8),  uint8_t,
				  Conditional<N < (1ull << 16), uint16_t,
				  Conditional<N < (1ull << 32), uint32_t,
													void>>>;

HFSM2_CONSTEXPR(11)
Short
bitContain(const Short v)												noexcept	{
	return v <= 1 << 0 ? 0 :
		   v <= 1 << 1 ? 1 :
		   v <= 1 << 2 ? 2 :
		   v <= 1 << 3 ? 3 :
		   v <= 1 << 4 ? 4 :
		   v <= 1 << 5 ? 5 :
		   v <= 1 << 6 ? 6 :
		   v <= 1 << 7 ? 7 :
						 8 ;
}

template <uint64_t N>
using UBitWidth = Conditional<N <=  8,  uint8_t,
				  Conditional<N <= 16, uint16_t,
				  Conditional<N <= 32, uint32_t,
										   void>>>;

template <typename T>
HFSM2_CONSTEXPR(11)
T&&
forward(RemoveReference<T>& t)											noexcept	{
	return static_cast<T&&>(t);
}

template <typename T>
HFSM2_CONSTEXPR(11)
T&&
forward(RemoveReference<T>&& t)											noexcept	{
	static_assert(!IsValueReferenceT<T>::VALUE, "");

	return static_cast<T&&>(t);
}

template <typename T>
HFSM2_CONSTEXPR(11)
RemoveReference<T>&&
move(T&& t)																noexcept	{
	return static_cast<RemoveReference<T>&&>(t);
}

template <typename T0,
		  typename T1>
HFSM2_CONSTEXPR(11)
T0
min(const T0 t0,
	const T1 t1)														noexcept
{
	return t0 < static_cast<T0>(t1) ?
		   t0 : static_cast<T0>(t1);
}

template <typename T0,
		  typename T1>
HFSM2_CONSTEXPR(11)
T0
max(const T0 t0,
	const T1 t1)														noexcept
{
	return t0 > static_cast<T0>(t1) ?
		   t0 : static_cast<T0>(t1);
}

template <typename TIndex,
		  typename TElement,
		  TIndex NCount>
HFSM2_CONSTEXPR(11)
TIndex
count(const TElement(&)[NCount])										noexcept	{
	return NCount;
}

template <typename T,
		  typename TT>
HFSM2_CONSTEXPR(11)
T
contain(const T x,
		const TT to)													noexcept	{ return (x + static_cast<T>(to) - 1) / static_cast<T>(to);	}

HFSM2_CONSTEXPR(11)
uint64_t
widen(const uint32_t x, const uint32_t y)								noexcept	{ return static_cast<uint64_t>(x) << 32 | y;	}

template <typename T>
HFSM2_CONSTEXPR(14)
void
fill(T& a, const char value)											noexcept	{ memset(&a, static_cast<int>(value), sizeof(a));	}

template <typename T>
HFSM2_CONSTEXPR(14)
void
swap(T& l, T& r)														noexcept	{
	T t = move(l);
	l = move(r);
	r = move(t);
}

template <typename TTo,
		  typename TFrom>
HFSM2_CONSTEXPR(14)
void
overwrite(TTo& to, const TFrom& from)									noexcept	{
	static_assert(sizeof(TTo) == sizeof(TFrom), "");

#if defined(__GNUC__) || defined(__GNUG__)
	memcpy  (&to,			  &from, sizeof(from));
#else
	memcpy_s(&to, sizeof(to), &from, sizeof(from));
#endif
}

template <typename TO,
		  typename TI>
HFSM2_CONSTEXPR(14)
TO
reinterpret(const TI& in)												noexcept	{
	TO out{};

	overwrite(out, in);

	return out;
}

template <int>
struct StaticPrintConstT;

template <typename>
struct StaticPrintTypeT;

}

namespace hfsm2 {
namespace detail {

template <typename TContainer>
class IteratorT {
public:
	using Container = TContainer;
	using Item		= typename Container::Item;
	using Index		= typename Container::Index;

	template <typename, Long>
	friend class ArrayT;

private:
	HFSM2_CONSTEXPR(11)	IteratorT(Container& container,
								  const Index cursor)										noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	HFSM2_CONSTEXPR(14)	bool operator != (const IteratorT& HFSM2_IF_ASSERT(other))	  const noexcept	{
		HFSM2_ASSERT(&_container == &other._container);

		return _cursor != _container.limit();
	}

	HFSM2_CONSTEXPR(14)	IteratorT& operator ++()											noexcept	{
		_cursor = _container.next(_cursor);

		return *this;
	}

	HFSM2_CONSTEXPR(14)		  Item& operator *()			noexcept	{ return  _container[_cursor];	}
	HFSM2_CONSTEXPR(11)	const Item& operator *()	  const noexcept	{ return  _container[_cursor];	}

	HFSM2_CONSTEXPR(14)		  Item* operator->()			noexcept	{ return &_container[_cursor];	}
	HFSM2_CONSTEXPR(11)	const Item* operator->()	  const noexcept	{ return &_container[_cursor];	}

private:
	Container& _container;

	Index _cursor;
};

template <typename TContainer>
class IteratorT<const TContainer> {
public:
	using Container = TContainer;
	using Item		= typename Container::Item;
	using Index		= typename Container::Index;

	template <typename, Long>
	friend class ArrayT;

private:
	HFSM2_CONSTEXPR(11)	IteratorT(const Container& container,
								  const Index cursor)										noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	HFSM2_CONSTEXPR(14)	bool operator != (const IteratorT& HFSM2_IF_ASSERT(other))	  const noexcept	{
		HFSM2_ASSERT(&_container == &other._container);

		return _cursor != _container.limit();
	}

	HFSM2_CONSTEXPR(14)	IteratorT& operator ++()											noexcept	{
		_cursor = _container.next(_cursor);

		return *this;
	}

	HFSM2_CONSTEXPR(11)	const Item& operator *()	  const noexcept	{ return _container[_cursor];	}

	HFSM2_CONSTEXPR(11)	const Item* operator->()	  const noexcept	{ return &operator *();			}

private:
	const Container& _container;

	Index _cursor;
};

}
}

#if HFSM2_SERIALIZATION_AVAILABLE()

namespace hfsm2 {
namespace detail {

template <Long>
class BitWriteStreamT;

template <Long>
class BitReadStreamT;

template <Long NBitCapacity>
class StreamBufferT {
	template <Long>
	friend class BitWriteStreamT;

	template <Long>
	friend class BitReadStreamT;

public:
	static constexpr Long BIT_CAPACITY	= NBitCapacity;
	static constexpr Long BYTE_COUNT	= contain(BIT_CAPACITY, 8u);

	using StreamBuffer	= StreamBufferT<BIT_CAPACITY>;
	using Data			= uint8_t	   [BYTE_COUNT	];

	HFSM2_CONSTEXPR(14)	void clear()									noexcept	{ fill(_data, 0);	}

	HFSM2_CONSTEXPR(14)		  Data& data()								noexcept	{ return _data;		}
	HFSM2_CONSTEXPR(11)	const Data& data()						  const noexcept	{ return _data;		}

	HFSM2_CONSTEXPR(14) bool operator == (const StreamBuffer& s)  const noexcept;
	HFSM2_CONSTEXPR(14) bool operator != (const StreamBuffer& s)  const noexcept;

private:
	Data _data {};
};

template <Long NBitCapacity>
class BitWriteStreamT final {
public:
	static constexpr Long BIT_CAPACITY = NBitCapacity;

	using Buffer = StreamBufferT<BIT_CAPACITY>;

public:
	HFSM2_CONSTEXPR(14)	explicit BitWriteStreamT(Buffer& buffer,
												 const Long cursor = 0)	noexcept
		: _buffer{buffer}
		, _cursor{cursor}
	{
		_buffer.clear();
	}

	template <Short NBitWidth>
	HFSM2_CONSTEXPR(14)	void write(const UBitWidth<NBitWidth> item)		noexcept;

	HFSM2_CONSTEXPR(11)	Long cursor()							  const noexcept	{ return _cursor;	}

private:
	Buffer& _buffer;

	Long _cursor = 0;
};

template <Long NBitCapacity>
class BitReadStreamT final {
public:
	static constexpr Long BIT_CAPACITY = NBitCapacity;

	using Buffer = StreamBufferT<BIT_CAPACITY>;

public:
	HFSM2_CONSTEXPR(11)	explicit BitReadStreamT(const Buffer& buffer,
												const Long cursor = 0)	noexcept
		: _buffer{buffer}
		, _cursor{cursor}
	{}

	template <Short NBitWidth>
	HFSM2_CONSTEXPR(14)	UBitWidth<NBitWidth> read()						noexcept;

	HFSM2_CONSTEXPR(11)	Long cursor()							  const noexcept	{ return _cursor;	}

private:
	const Buffer& _buffer;

	Long _cursor;
};

}
}


#if HFSM2_SERIALIZATION_AVAILABLE()

namespace hfsm2 {
namespace detail {

template <Long NBitCapacity>
HFSM2_CONSTEXPR(14)
bool
StreamBufferT<NBitCapacity>::operator == (const StreamBuffer& buffer) const noexcept {
	for (hfsm2::Long i = 0; i < BYTE_COUNT; ++i)
		if (_data[i] != buffer._data[i])
			return false;

	return true;
}

template <Long NBitCapacity>
HFSM2_CONSTEXPR(14)
bool
StreamBufferT<NBitCapacity>::operator != (const StreamBuffer& buffer) const noexcept {
	for (hfsm2::Long i = 0; i < BYTE_COUNT; ++i)
		if (_data[i] != buffer._data[i])
			return true;

	return false;
}

template <Long NBC>
template <Short NBitWidth>
HFSM2_CONSTEXPR(14)
void
BitWriteStreamT<NBC>::write(const UBitWidth<NBitWidth> item) noexcept {
	constexpr Short BIT_WIDTH = NBitWidth;
	static_assert(BIT_WIDTH > 0, "STATIC ASSERT");

	HFSM2_ASSERT(_cursor + BIT_WIDTH <= BIT_CAPACITY);

	using Item = UBitWidth<BIT_WIDTH>;

	Item itemBits = item;

	for (Short itemWidth = BIT_WIDTH; itemWidth; ) {
		const Long	byteIndex		= _cursor >> 3;
		uint8_t&	byte			= _buffer._data[byteIndex];

		const Short byteChunkStart	= _cursor & 0x7;
		const Short byteDataWidth	= 8 - byteChunkStart;
		const Short byteChunkWidth	= min(byteDataWidth, itemWidth);
		const Item	byteChunk		= itemBits << byteChunkStart;

		byte		|= byteChunk;
		itemBits   >>= byteChunkWidth;
		itemWidth	-= byteChunkWidth;
		_cursor		+= byteChunkWidth;
	}
}

template <Long NBC>
template <Short NBitWidth>
HFSM2_CONSTEXPR(14)
UBitWidth<NBitWidth>
BitReadStreamT<NBC>::read() noexcept {
	constexpr Short BIT_WIDTH = NBitWidth;
	static_assert(BIT_WIDTH > 0, "STATIC ASSERT");

	HFSM2_ASSERT(_cursor + BIT_WIDTH <= BIT_CAPACITY);

	using Item = UBitWidth<BIT_WIDTH>;

	Item item = 0;

	for (Short itemCursor = 0, itemWidth = BIT_WIDTH; itemWidth; ) {
		const Long	byteIndex		= _cursor >> 3;
		const uint8_t& byte			= _buffer._data[byteIndex];

		const Short byteChunkStart	= _cursor & 0x7;
		const Short byteDataWidth	= 8 - byteChunkStart;
		const Short byteChunkWidth	= min(byteDataWidth, itemWidth);
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

}
}

#endif
#endif

#if HFSM2_UTILITY_THEORY_AVAILABLE()

namespace hfsm2 {
namespace detail {

HFSM2_CONSTEXPR(14)	float  uniform(const uint32_t uint)					noexcept;
HFSM2_CONSTEXPR(14)	double uniform(const uint64_t uint)					noexcept;

template <unsigned>
class SimpleRandomT;

// SplitMix64 (http://xoshiro.di.unimi.it/splitmix64.c)

template <>
class SimpleRandomT<8> {
public:
	constexpr SimpleRandomT()											noexcept	{}

	HFSM2_CONSTEXPR(14)	SimpleRandomT(const uint64_t seed)				noexcept;

	HFSM2_CONSTEXPR(14)	uint64_t uint64()								noexcept;

private:
	HFSM2_CONSTEXPR(14)	uint64_t raw64()								noexcept;

private:

	uint64_t _state = 0;
};

// SplitMix32 (https://groups.google.com/forum/#!topic/prng/VFjdFmbMgZI)

template <>
class SimpleRandomT<4> {
public:
	HFSM2_CONSTEXPR(11)	SimpleRandomT()									noexcept	{}

	HFSM2_CONSTEXPR(14)	SimpleRandomT(const uint32_t seed)				noexcept;

	HFSM2_CONSTEXPR(14)	uint32_t uint32()								noexcept;

private:
	HFSM2_CONSTEXPR(14)	uint32_t raw32()								noexcept;

private:
	uint32_t _state = 0;
};

template <unsigned>
class BaseRandomT;

template <>
class BaseRandomT<8> {
protected:
	using SimpleRandom = SimpleRandomT<8>;

	HFSM2_CONSTEXPR(14)	BaseRandomT(SimpleRandom&& simple)				noexcept;

	HFSM2_CONSTEXPR(14)	void seed(SimpleRandom&& simple)				noexcept;

public:
	HFSM2_CONSTEXPR(14)	BaseRandomT()									noexcept;

	HFSM2_CONSTEXPR(14)	BaseRandomT(const uint64_t s)					noexcept;
	HFSM2_CONSTEXPR(14)	BaseRandomT(const uint64_t(& s)[4])				noexcept;

	HFSM2_CONSTEXPR(14)	void seed(const uint64_t s)						noexcept;
	HFSM2_CONSTEXPR(14)	void seed(const uint64_t(& s)[4])				noexcept;

protected:
	uint64_t _state[4];
};

template <>
class BaseRandomT<4> {
protected:
	using SimpleRandom = SimpleRandomT<4>;

	HFSM2_CONSTEXPR(14)	BaseRandomT(SimpleRandom&& simple)				noexcept;

	HFSM2_CONSTEXPR(14)	void seed(SimpleRandom&& simple)				noexcept;

public:
	HFSM2_CONSTEXPR(14)	BaseRandomT()									noexcept;

	HFSM2_CONSTEXPR(14)	BaseRandomT(const uint32_t s)					noexcept;
	HFSM2_CONSTEXPR(14)	BaseRandomT(const uint32_t(& s)[4])				noexcept;

	HFSM2_CONSTEXPR(14)	void seed(const uint32_t s)						noexcept;
	HFSM2_CONSTEXPR(14)	void seed(const uint32_t(& s)[4])				noexcept;

protected:
	uint32_t _state[4];
};

template <unsigned>
class FloatRandomT;

// xoshiro256+ (http://xoshiro.di.unimi.it/xoshiro256plus.c)

template <>
class FloatRandomT<8>
	: BaseRandomT<8>
{
	using Base = BaseRandomT<8>;

public:
	using Base::BaseRandomT;

	HFSM2_CONSTEXPR(14)	double	float64()								noexcept	{ return uniform(uint64());					}
	HFSM2_CONSTEXPR(14)	float	float32()								noexcept	{ return uniform(uint32());					}

	HFSM2_CONSTEXPR(14)	uint64_t uint64()								noexcept;
	HFSM2_CONSTEXPR(14)	uint32_t uint32()								noexcept	{ return static_cast<uint32_t>(uint64());	}

	HFSM2_CONSTEXPR(14)	float next()									noexcept	{ return float32();							}
	HFSM2_CONSTEXPR(14)	void  jump()									noexcept;
};

// xoshiro128+ (http://xoshiro.di.unimi.it/xoshiro128plus.c)

template <>
class FloatRandomT<4>
	: BaseRandomT<4>
{
	using Base = BaseRandomT<4>;

public:
	using Base::BaseRandomT;

	HFSM2_CONSTEXPR(14)	double	float64()								noexcept	{ return uniform(uint64());					}
	HFSM2_CONSTEXPR(14)	float	float32()								noexcept	{ return uniform(uint32());					}

	HFSM2_CONSTEXPR(14)	uint64_t uint64()								noexcept	{ return widen(uint32(), uint32());			}
	HFSM2_CONSTEXPR(14)	uint32_t uint32()								noexcept;

	HFSM2_CONSTEXPR(14)	float next()									noexcept	{ return float32();							}
	HFSM2_CONSTEXPR(14)	void  jump()									noexcept;
};

template <unsigned>
class IntRandomT;

// xoshiro256** (https://prng.di.unimi.it/xoshiro256starstar.c)

template <>
class IntRandomT<8>
	: BaseRandomT<8>
{
	using Base = BaseRandomT<8>;

public:
	using Base::BaseRandomT;

	HFSM2_CONSTEXPR(14)	double	float64()								noexcept	{ return uniform(uint64());					}
	HFSM2_CONSTEXPR(14)	float	float32()								noexcept	{ return uniform(uint32());					}

	HFSM2_CONSTEXPR(14)	uint64_t uint64()								noexcept;
	HFSM2_CONSTEXPR(14)	uint32_t uint32()								noexcept	{ return static_cast<uint32_t>(uint64());	}

	HFSM2_CONSTEXPR(14)	void jump()										noexcept;
};

// xoshiro128** (https://prng.di.unimi.it/xoshiro128starstar.c)

template <>
class IntRandomT<4>
	: BaseRandomT<4>
{
	using Base = BaseRandomT<4>;

public:
	using Base::BaseRandomT;

	HFSM2_CONSTEXPR(14)	double	float64()								noexcept	{ return uniform(uint64());			}
	HFSM2_CONSTEXPR(14)	float	float32()								noexcept	{ return uniform(uint32());			}

	HFSM2_CONSTEXPR(14)	uint64_t uint64()								noexcept	{ return widen(uint32(), uint32());	}
	HFSM2_CONSTEXPR(14)	uint32_t uint32()								noexcept;

	HFSM2_CONSTEXPR(14)	void jump()										noexcept;
};

}

using SimpleRandom = detail::SimpleRandomT<sizeof(void*)>;
using FloatRandom  = detail::FloatRandomT <sizeof(void*)>;
using IntRandom	   = detail::IntRandomT	  <sizeof(void*)>;

template <typename T>
class RNGT;

template <>
class RNGT<float>
	: public FloatRandom
{
public:
	using Base = FloatRandom;

	using Base::Base;
};

template <>
class RNGT<uintptr_t>
	: public IntRandom
{
public:
	using Base = IntRandom;

	using Base::Base;
};

}

#endif


#if HFSM2_UTILITY_THEORY_AVAILABLE()

namespace hfsm2 {
namespace detail {

HFSM2_CONSTEXPR(14)
float
uniform(const uint32_t uint) noexcept {
	return reinterpret<float>(UINT32_C(0x7F) << 23 | uint >> 9) - 1.0f;
}

HFSM2_CONSTEXPR(14)
double
uniform(const uint64_t uint) noexcept {
	return reinterpret<double>(UINT64_C(0x3FF) << 52 | uint >> 12) - 1.0;
}

HFSM2_CONSTEXPR(11)
uint32_t
rotl(const uint32_t x, const uint32_t k) noexcept {
	return (x << k) | (x >> (32 - k));
}

HFSM2_CONSTEXPR(11)
uint64_t
rotl(const uint64_t x, const uint64_t k) noexcept {
	return (x << k) | (x >> (64 - k));
}

HFSM2_CONSTEXPR(14)
SimpleRandomT<8>::SimpleRandomT(const uint64_t seed) noexcept
	: _state{seed}
{}

HFSM2_CONSTEXPR(14)
uint64_t
SimpleRandomT<8>::uint64() noexcept {
	for (;;)
		if (const uint64_t number = raw64())
			return number;
}

HFSM2_CONSTEXPR(14)
uint64_t
SimpleRandomT<8>::raw64() noexcept {
	uint64_t z = (_state += 0x9e3779b97f4a7c15);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;

	return z ^ (z >> 31);
}

HFSM2_CONSTEXPR(14)
SimpleRandomT<4>::SimpleRandomT(const uint32_t seed) noexcept
	: _state{seed}
{}

HFSM2_CONSTEXPR(14)
uint32_t
SimpleRandomT<4>::uint32() noexcept {
	for (;;)
		if (const uint32_t number = raw32())
			return number;
}

HFSM2_CONSTEXPR(14)
uint32_t
SimpleRandomT<4>::raw32() noexcept {
	uint32_t z = (_state += 0x9E3779B9);
	z = (z ^ (z >> 16)) * 0x85ebca6b;
	z = (z ^ (z >> 13)) * 0xc2b2ae35;

	return z ^ (z >> 16);
}

HFSM2_CONSTEXPR(14)
BaseRandomT<8>::BaseRandomT(SimpleRandom&& simple) noexcept
	: _state{simple.uint64(), simple.uint64(), simple.uint64(), simple.uint64()}
{}

HFSM2_CONSTEXPR(14)
void
BaseRandomT<8>::seed(SimpleRandom&& simple) noexcept {
	_state[0] = simple.uint64();
	_state[1] = simple.uint64();
	_state[2] = simple.uint64();
	_state[3] = simple.uint64();
}

HFSM2_CONSTEXPR(14)
BaseRandomT<8>::BaseRandomT() noexcept
	: BaseRandomT{SimpleRandom{0}}
{}

HFSM2_CONSTEXPR(14)
BaseRandomT<8>::BaseRandomT(const uint64_t s) noexcept
	: BaseRandomT{SimpleRandom{s}}
{}

HFSM2_CONSTEXPR(14)
BaseRandomT<8>::BaseRandomT(const uint64_t(& s)[4]) noexcept
	: _state{s[0], s[1], s[2], s[3]}
{}

HFSM2_CONSTEXPR(14)
void
BaseRandomT<8>::seed(const uint64_t s) noexcept {
	seed(SimpleRandom{s});
}

HFSM2_CONSTEXPR(14)
void
BaseRandomT<8>::seed(const uint64_t(& s)[4]) noexcept {
	_state[0] = s[0];
	_state[1] = s[1];
	_state[2] = s[2];
	_state[3] = s[3];
}

HFSM2_CONSTEXPR(14)
BaseRandomT<4>::BaseRandomT(SimpleRandom&& simple) noexcept
	: _state{simple.uint32(), simple.uint32(), simple.uint32(), simple.uint32()}
{}

HFSM2_CONSTEXPR(14)
void
BaseRandomT<4>::seed(SimpleRandom&& simple) noexcept {
	_state[0] = simple.uint32();
	_state[1] = simple.uint32();
	_state[2] = simple.uint32();
	_state[3] = simple.uint32();
}

HFSM2_CONSTEXPR(14)
BaseRandomT<4>::BaseRandomT() noexcept
	: BaseRandomT{SimpleRandom{0}}
{}

HFSM2_CONSTEXPR(14)
BaseRandomT<4>::BaseRandomT(const uint32_t s) noexcept
	: BaseRandomT{SimpleRandom{s}}
{}

HFSM2_CONSTEXPR(14)
BaseRandomT<4>::BaseRandomT(const uint32_t(& s)[4]) noexcept
	: _state{s[0], s[1], s[2], s[3]}
{}

HFSM2_CONSTEXPR(14)
void
BaseRandomT<4>::seed(const uint32_t s) noexcept {
	seed(SimpleRandom{s});
}

HFSM2_CONSTEXPR(14)
void
BaseRandomT<4>::seed(const uint32_t(& s)[4]) noexcept {
	_state[0] = s[0];
	_state[1] = s[1];
	_state[2] = s[2];
	_state[3] = s[3];
}

HFSM2_CONSTEXPR(14)
uint64_t
FloatRandomT<8>::uint64() noexcept {
	const uint64_t result = _state[0] + _state[3];

	const uint64_t t = _state[1] << 17;

	_state[2] ^= _state[0];
	_state[3] ^= _state[1];
	_state[1] ^= _state[2];
	_state[0] ^= _state[3];

	_state[2] ^= t;

	_state[3] = rotl(_state[3], 45);

	return result;
}

HFSM2_CONSTEXPR(14)
void
FloatRandomT<8>::jump() noexcept {
	constexpr uint64_t JUMP[] = {
		0x180ec6d33cfd0aba,
		0xd5a61266f0c9392c,
		0xa9582618e03fc9aa,
		0x39abdc4529b1661c
	};

	uint64_t s0 = 0;
	uint64_t s1 = 0;
	uint64_t s2 = 0;
	uint64_t s3 = 0;

	for (unsigned i = 0; i < count<unsigned>(JUMP); ++i)
		for (int b = 0; b < 64; ++b) {
			if (JUMP[i] & UINT64_C(1) << b) {
				s0 ^= _state[0];
				s1 ^= _state[1];
				s2 ^= _state[2];
				s3 ^= _state[3];
			}
			uint64();
		}

	_state[0] = s0;
	_state[1] = s1;
	_state[2] = s2;
	_state[3] = s3;
}

HFSM2_CONSTEXPR(14)
uint32_t
FloatRandomT<4>::uint32() noexcept {
	const uint32_t result = _state[0] + _state[3];

	const uint32_t t = _state[1] << 9;

	_state[2] ^= _state[0];
	_state[3] ^= _state[1];
	_state[1] ^= _state[2];
	_state[0] ^= _state[3];

	_state[2] ^= t;

	_state[3] = rotl(_state[3], 11);

	return result;
}

HFSM2_CONSTEXPR(14)
void
FloatRandomT<4>::jump() noexcept {
	constexpr uint32_t JUMP[] = {
		0x8764000b,
		0xf542d2d3,
		0x6fa035c3,
		0x77f2db5b
	};

	uint32_t s0 = 0;
	uint32_t s1 = 0;
	uint32_t s2 = 0;
	uint32_t s3 = 0;

	for (unsigned i = 0; i < count<unsigned>(JUMP); ++i)
		for (unsigned b = 0; b < 32; ++b) {
			if (JUMP[i] & UINT32_C(1) << b) {
				s0 ^= _state[0];
				s1 ^= _state[1];
				s2 ^= _state[2];
				s3 ^= _state[3];
			}
			uint32();
		}

	_state[0] = s0;
	_state[1] = s1;
	_state[2] = s2;
	_state[3] = s3;
}

HFSM2_CONSTEXPR(14)
uint64_t
IntRandomT<8>::uint64() noexcept {
	const uint64_t result = rotl(_state[1] * 5, 7) * 9;

	const uint64_t t = _state[1] << 17;

	_state[2] ^= _state[0];
	_state[3] ^= _state[1];
	_state[1] ^= _state[2];
	_state[0] ^= _state[3];

	_state[2] ^= t;

	_state[3] = rotl(_state[3], 45);

	return result;
}

HFSM2_CONSTEXPR(14)
void
IntRandomT<8>::jump() noexcept {
	constexpr uint64_t JUMP[] = {
		0x180ec6d33cfd0aba,
		0xd5a61266f0c9392c,
		0xa9582618e03fc9aa,
		0x39abdc4529b1661c
	};

	uint64_t s0 = 0;
	uint64_t s1 = 0;
	uint64_t s2 = 0;
	uint64_t s3 = 0;

	for (unsigned i = 0; i < count<unsigned>(JUMP); ++i)
		for (int b = 0; b < 64; ++b) {
			if (JUMP[i] & UINT64_C(1) << b) {
				s0 ^= _state[0];
				s1 ^= _state[1];
				s2 ^= _state[2];
				s3 ^= _state[3];
			}
			uint64();
		}

	_state[0] = s0;
	_state[1] = s1;
	_state[2] = s2;
	_state[3] = s3;
}

HFSM2_CONSTEXPR(14)
uint32_t
IntRandomT<4>::uint32() noexcept {
	const uint32_t result = rotl(_state[1] * 5, 7) * 9;

	const uint32_t t = _state[1] << 9;

	_state[2] ^= _state[0];
	_state[3] ^= _state[1];
	_state[1] ^= _state[2];
	_state[0] ^= _state[3];

	_state[2] ^= t;

	_state[3] = rotl(_state[3], 11);

	return result;
}

HFSM2_CONSTEXPR(14)
void
IntRandomT<4>::jump() noexcept {
	constexpr uint32_t JUMP[] = {
		0x8764000b,
		0xf542d2d3,
		0x6fa035c3,
		0x77f2db5b
	};

	uint32_t s0 = 0;
	uint32_t s1 = 0;
	uint32_t s2 = 0;
	uint32_t s3 = 0;

	for (unsigned i = 0; i < count<unsigned>(JUMP); ++i)
		for (unsigned b = 0; b < 32; ++b) {
			if (JUMP[i] & UINT32_C(1) << b) {
				s0 ^= _state[0];
				s1 ^= _state[1];
				s2 ^= _state[2];
				s3 ^= _state[3];
			}
			uint32();
		}

	_state[0] = s0;
	_state[1] = s1;
	_state[2] = s2;
	_state[3] = s3;
}

}
}

#endif

namespace hfsm2 {
namespace detail {

template <typename... Ts>
struct TL_ final {
	static constexpr Long SIZE = sizeof...(Ts);
};

template <Long N>
struct Const {
	static constexpr Long VALUE = N;
};

template <typename, typename>
struct PrependT;

template <typename T, typename... Ts>
struct PrependT<T, TL_<Ts...>> final {
	using Type = TL_<T, Ts...>;
};

template <typename... Ts>
using PrependTypes = typename PrependT<Ts...>::Type;

template <typename, typename>
struct MergeT;

template <typename... Ts1, typename... Ts2>
struct MergeT<TL_<Ts1...>, TL_<Ts2...>> final {
	using Type = TL_<Ts1..., Ts2...>;
};

template <typename... Ts>
using Merge = typename MergeT<Ts...>::Type;

template <Long, Long, typename...>
struct LowerT;

template <Long NHalf, Long NIndex, typename... Ts>
using LowerTypes = typename LowerT<NHalf, NIndex, Ts...>::Type;

template <Long NHalf, Long NIndex, typename TFirst, typename... TRest>
struct LowerT<NHalf, NIndex, TFirst, TRest...> final {
	using LTypeList = typename LowerT<NHalf, NIndex + 1, TRest...>::Type;

	using Type = Conditional<
					 (NIndex < NHalf),
					 PrependTypes<TFirst, LTypeList>,
					 LTypeList
				 >;
};

template <Long NHalf, Long NIndex>
struct LowerT<NHalf, NIndex> final {
	using Type = TL_<>;
};

template <typename... Ts>
using LHalfTypes = LowerTypes<sizeof...(Ts) / 2, 0, Ts...>;

template <Long, Long, typename...>
struct UpperT;

template <Long NHalf, Long NIndex, typename... Ts>
using UpperTypes = typename UpperT<NHalf, NIndex, Ts...>::Type;

template <Long NHalf, Long NIndex, typename TFirst, typename... TRest>
struct UpperT<NHalf, NIndex, TFirst, TRest...> final {
	using Type = Conditional<
					 (NIndex < NHalf),
					 UpperTypes<NHalf, NIndex + 1, TRest...>,
					 TL_<TFirst, TRest...>
				 >;
};

template <Long NHalf, Long NIndex>
struct UpperT<NHalf, NIndex> final {
	using Type = TL_<>;
};

template <typename... Ts>
using RHalfTypes = UpperTypes<sizeof...(Ts) / 2, 0, Ts...>;

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

template <typename, typename>
struct Find;

template <typename T, typename... Ts>
struct Find<TL_<Ts...>, T> final
	: FindImpl<0, T, Ts...>
{};

}

template <typename TList, typename T>
constexpr Long index   () noexcept	{ return detail::Find<TList, T>::VALUE;		}

template <typename TList, typename T>
constexpr bool contains() noexcept	{ return index<TList, T>() != INVALID_LONG;	}

}

namespace hfsm2 {

enum class Method : uint8_t {
	NONE,

	SELECT,

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	RANK,
	UTILITY,
#endif

	ENTRY_GUARD,
	ENTER,
	REENTER,
	PRE_UPDATE,
	UPDATE,
	POST_UPDATE,
	PRE_REACT,
	REACT,
	QUERY,
	POST_REACT,
	EXIT_GUARD,
	EXIT,

#if HFSM2_PLANS_AVAILABLE()
	PLAN_SUCCEEDED,
	PLAN_FAILED,
#endif

	COUNT
};

enum class TransitionType : uint8_t {
	CHANGE,
	RESTART,
	RESUME,
	SELECT,

//#if HFSM2_UTILITY_THEORY_AVAILABLE()
	UTILIZE,
	RANDOMIZE,
//#endif

	SCHEDULE,

	COUNT
};

#if HFSM2_PLANS_AVAILABLE()

enum class StatusEvent : uint8_t {
	SUCCEEDED,
	FAILED,

	COUNT
};

#endif

#if HFSM2_TYPEINDEX_AVAILABLE()

static
inline
const char*
stateName(const std::type_index stateType)								noexcept	{
	const char* const raw = stateType.name();

	#if defined(_MSC_VER)

		const uint8_t first =
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

#endif

static
HFSM2_CONSTEXPR(14)
const char*
methodName(const Method method)											noexcept	{
	switch (method) {

	case Method::SELECT:		 return "select";

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	case Method::RANK:			 return "rank";
	case Method::UTILITY:		 return "utility";
#endif

	case Method::ENTRY_GUARD:	 return "entryGuard";
	case Method::ENTER:			 return "enter";
	case Method::REENTER:		 return "reenter";
	case Method::PRE_UPDATE:	 return "preUpdate";
	case Method::UPDATE:		 return "update";
	case Method::POST_UPDATE:	 return "postUpdate";
	case Method::PRE_REACT:		 return "preReact";
	case Method::REACT:			 return "react";
	case Method::POST_REACT:	 return "postReact";
	case Method::QUERY:			 return "query";
	case Method::EXIT_GUARD:	 return "exitGuard";
	case Method::EXIT:			 return "exit";

#if HFSM2_PLANS_AVAILABLE()
	case Method::PLAN_SUCCEEDED: return "planSucceeded";
	case Method::PLAN_FAILED:	 return "planFailed";
#endif

	default:
		HFSM2_BREAK();
		return nullptr;
	}
}

static
HFSM2_CONSTEXPR(14)
const char*
transitionName(const TransitionType type)								noexcept	{
	switch (type) {
	case TransitionType::CHANGE:	return "changeTo";
	case TransitionType::RESTART:	return "restart";
	case TransitionType::RESUME:	return "resume";
	case TransitionType::SELECT:	return "select";

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	case TransitionType::UTILIZE:	return "utilize";
	case TransitionType::RANDOMIZE:	return "randomize";
#endif

	case TransitionType::SCHEDULE:	return "schedule";

	default:
		HFSM2_BREAK();
		return nullptr;
	}
}

namespace detail {

#pragma pack(push, 1)

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

struct TransitionBase {
	HFSM2_CONSTEXPR(11)
	TransitionBase()													noexcept = default;

	HFSM2_CONSTEXPR(11)
	TransitionBase(const StateID destination_,
				   const TransitionType type_)							noexcept
		: destination{destination_}
		, type		 {type_}
	{}

	HFSM2_CONSTEXPR(11)
	TransitionBase(const StateID origin_,
				   const StateID destination_,
				   const TransitionType type_)							noexcept
		: origin	 {origin_}
		, destination{destination_}
		, type		 {type_}
	{}

	HFSM2_CONSTEXPR(11)
	bool
	operator == (const TransitionBase& other)					  const noexcept	{
		return origin	   == other.origin &&
			   destination == other.destination &&
			   method	   == other.method &&
			   type		   == other.type;
	}

	HFSM2_CONSTEXPR(11)
	bool
	operator != (const TransitionBase& other)					  const noexcept	{
		return origin	   != other.origin ||
			   destination != other.destination ||
			   method	   != other.method ||
			   type		   != other.type;
	}

	StateID	origin		= INVALID_STATE_ID;
	StateID	destination	= INVALID_STATE_ID;
	Method	method		= Method::NONE;
	TransitionType type	= TransitionType::COUNT;
};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

template <typename TPayload>
struct TransitionT final
	: TransitionBase
{
	using Payload = TPayload;
	using Storage = uint8_t[sizeof(Payload)];

	using TransitionBase::TransitionBase;

	HFSM2_CONSTEXPR(14)
	TransitionT()														noexcept	{
		new (&storage) Payload{};
	}

	HFSM2_CONSTEXPR(14)
	TransitionT(const StateID destination_,
				const TransitionType type_,
				const Payload& payload)									noexcept
		: TransitionBase{destination_, type_}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	HFSM2_CONSTEXPR(14)
	TransitionT(const StateID origin_,
				const StateID destination_,
				const TransitionType type_,
				const Payload& payload)									noexcept
		: TransitionBase{origin_, destination_, type_}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	HFSM2_CONSTEXPR(11)
	bool
	operator == (const TransitionT& other)						  const noexcept	{
		return TransitionBase::operator == (other) &&
			   (payloadSet ==  other.payloadSet);
		//	  (!payloadSet && !other.payloadSet || payload ==  other.payload);
	}

	HFSM2_CONSTEXPR(11)
	bool
	operator != (const TransitionT& other)						  const noexcept	{
		return TransitionBase::operator != (other) ||
			   (payloadSet != other.payloadSet);
		//	   (payloadSet |= other.payloadSet || payload != other.payload);
	}

	HFSM2_CONSTEXPR(11)
	const Payload*
	payload()													  const noexcept	{
		return payloadSet ?
			reinterpret_cast<const Payload*>(&storage) : nullptr;
	}

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

	alignas(Payload) Storage storage {};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

	bool payloadSet = false;
};

template <>
struct TransitionT<void> final
	: TransitionBase
{
	using TransitionBase::TransitionBase;
};

#pragma pack(pop)

}

struct Request final {
	TransitionType type;
	Short index;
};

}

namespace hfsm2 {

#if HFSM2_LOG_INTERFACE_AVAILABLE()

template <FeatureTag NFeatureTag = HFSM2_FEATURE_TAG
		, typename TContext = EmptyContext
		HFSM2_IF_UTILITY_THEORY(, typename TUtilty = float)>
struct LoggerInterfaceT {
	using Context		 = TContext;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Utilty		 = TUtilty;
#endif

	using Method		 = ::hfsm2::Method;
	using StateID		 = ::hfsm2::StateID;
	using RegionID		 = ::hfsm2::RegionID;
	using TransitionType = ::hfsm2::TransitionType;

#if HFSM2_PLANS_AVAILABLE()
	using StatusEvent	 = ::hfsm2::StatusEvent;
#endif

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordMethod(const Context& HFSM2_UNUSED(context),
				 const StateID HFSM2_UNUSED(origin),
				 const Method HFSM2_UNUSED(method))						noexcept
	{}

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordTransition(const Context& HFSM2_UNUSED(context),
					 const StateID HFSM2_UNUSED(origin),
					 const TransitionType HFSM2_UNUSED(transitionType),
					 const StateID HFSM2_UNUSED(target))				noexcept
	{}

#if HFSM2_PLANS_AVAILABLE()

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordTaskStatus(const Context& HFSM2_UNUSED(context),
					 const RegionID HFSM2_UNUSED(region),
					 const StateID HFSM2_UNUSED(origin),
					 const StatusEvent HFSM2_UNUSED(event))				noexcept
	{}

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordPlanStatus(const Context& HFSM2_UNUSED(context),
					 const RegionID HFSM2_UNUSED(region),
					 const StatusEvent HFSM2_UNUSED(event))				noexcept
	{}

#endif

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordCancelledPending(const Context& HFSM2_UNUSED(context),
						   const StateID HFSM2_UNUSED(origin))			noexcept
	{}

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordSelectResolution(const Context& HFSM2_UNUSED(context),
						   const StateID HFSM2_UNUSED(head),
						   const StateID HFSM2_UNUSED(prong))			noexcept
	{}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordUtilityResolution(const Context& HFSM2_UNUSED(context),
							const StateID HFSM2_UNUSED(head),
							const StateID HFSM2_UNUSED(prong),
							const Utilty HFSM2_UNUSED(utilty))			noexcept
	{}

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordRandomResolution(const Context& HFSM2_UNUSED(context),
						   const StateID HFSM2_UNUSED(head),
						   const StateID HFSM2_UNUSED(prong),
						   const Utilty HFSM2_UNUSED(utilty))			noexcept
	{}

#endif
};

#else

template <FeatureTag NFeatureTag = HFSM2_FEATURE_TAG
		, typename TContext = EmptyContext
		HFSM2_IF_UTILITY_THEORY(, typename TUtilty = float)>
using LoggerInterfaceT = void;

#endif

using LoggerInterface = LoggerInterfaceT<>;

}

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

namespace hfsm2 {

struct StructureEntry final {
	bool isActive;
	const wchar_t* prefix;
	const char* name;
};

namespace detail {

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

#pragma pack(push, 1)

struct alignas(alignof(void*)) StructureStateInfo final {
	enum class RegionType : Short {
		COMPOSITE,
		ORTHOGONAL,

		COUNT
	};

	StructureStateInfo() noexcept = default;

	HFSM2_CONSTEXPR(11)
	StructureStateInfo(const Long parent_,
					   const RegionType regionType_,
					   const Short depth_,
					   const char* const name_)							noexcept
		: name{name_}
		, parent{parent_}
		, regionType{regionType_ }
		, depth{depth_}
	{}

	const char* name	  = nullptr;
	Long parent			  = INVALID_LONG;
	RegionType regionType = RegionType::COUNT;
	Short depth			  = INVALID_SHORT;
};

#pragma pack(pop)

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

}
}

#endif

namespace hfsm2 {
namespace detail {

template <typename T>
HFSM2_CONSTEXPR(11)
T
filler()																noexcept	{
	return T{INVALID_SHORT};
}

template <typename T, Long NCapacity>
class StaticArrayT final {
	template <typename>
	friend class IteratorT;

public:
	using  Iterator	= IteratorT<      StaticArrayT>;
	using CIterator	= IteratorT<const StaticArrayT>;

	using Item		= T;
	using Index		= UCapacity<NCapacity>;

	static constexpr Index CAPACITY	= NCapacity;

public:
	HFSM2_CONSTEXPR(14)	StaticArrayT() = default;
	HFSM2_CONSTEXPR(14)	StaticArrayT(const Item filler)					noexcept	{ fill(filler);						}

	template <typename N>
	HFSM2_CONSTEXPR(14)		  Item& operator[] (const N index)			noexcept;

	template <typename N>
	HFSM2_CONSTEXPR(14)	const Item& operator[] (const N index)	  const noexcept;

	HFSM2_CONSTEXPR(11)	Index count()							  const noexcept	{ return CAPACITY;					}

	HFSM2_CONSTEXPR(14)	void fill(const Item filler)					noexcept;
	HFSM2_CONSTEXPR(14)	void clear()									noexcept	{ fill(filler<Item>());				}
	HFSM2_CONSTEXPR(14)	bool empty()							  const noexcept;

	HFSM2_CONSTEXPR(14)	 Iterator  begin()								noexcept	{ return  Iterator(*this, first());	}
	HFSM2_CONSTEXPR(11)	CIterator  begin()						  const noexcept	{ return CIterator(*this, first());	}
	HFSM2_CONSTEXPR(11)	CIterator cbegin()						  const noexcept	{ return CIterator(*this, first());	}

	HFSM2_CONSTEXPR(14)	 Iterator	 end()								noexcept	{ return  Iterator(*this, limit());	}
	HFSM2_CONSTEXPR(11)	CIterator	 end()						  const noexcept	{ return CIterator(*this, limit());	}
	HFSM2_CONSTEXPR(11)	CIterator	cend()						  const noexcept	{ return CIterator(*this, limit());	}

private:
	HFSM2_CONSTEXPR(11)	Index first()							  const noexcept	{ return 0;							}
	HFSM2_CONSTEXPR(11)	Index  next(const Index index)			  const noexcept	{ return index + 1;					}
	HFSM2_CONSTEXPR(11)	Index limit()							  const noexcept	{ return CAPACITY;					}

private:
	Item _items[CAPACITY] {};
};

template <typename T>
struct StaticArrayT<T, 0> final {
	using Item		= T;

	HFSM2_CONSTEXPR(11)	StaticArrayT() = default;
	HFSM2_CONSTEXPR(11)	StaticArrayT(const Item)						noexcept	{}
};

template <typename T, Long NCapacity>
class ArrayT final {
	template <typename>
	friend class IteratorT;

public:
	using  Iterator	= IteratorT<      ArrayT>;
	using CIterator	= IteratorT<const ArrayT>;

	using Item		= T;
	using Index		= UCapacity<NCapacity>;

	static constexpr Index CAPACITY	= NCapacity;

public:
	template <typename... TArgs>
	HFSM2_CONSTEXPR(14)	Index emplace(const TArgs &... args)				noexcept;

	template <typename... TArgs>
	HFSM2_CONSTEXPR(14)	Index emplace(		TArgs&&... args)				noexcept;

	template <typename N>
	HFSM2_CONSTEXPR(14)		  Item& operator[] (const N index)				noexcept;

	template <typename N>
	HFSM2_CONSTEXPR(14)	const Item& operator[] (const N index)		  const noexcept;

	HFSM2_CONSTEXPR(11)	Index count()								  const noexcept	{ return _count;					}

	HFSM2_CONSTEXPR(14)	 void clear()										noexcept	{ _count = 0;						}
	HFSM2_CONSTEXPR(11)	 bool empty()								  const noexcept	{ return _count == 0;				}

	template <Long N>
	HFSM2_CONSTEXPR(14)	ArrayT& operator += (const ArrayT<Item, N>& other)	noexcept;

	HFSM2_CONSTEXPR(14)	 Iterator  begin()									noexcept	{ return  Iterator(*this, first());	}
	HFSM2_CONSTEXPR(11)	CIterator  begin()							  const noexcept	{ return CIterator(*this, first());	}
	HFSM2_CONSTEXPR(11)	CIterator cbegin()							  const noexcept	{ return CIterator(*this, first());	}

	HFSM2_CONSTEXPR(14)	 Iterator	 end()									noexcept	{ return  Iterator(*this, limit());	}
	HFSM2_CONSTEXPR(11)	CIterator	 end()							  const noexcept	{ return CIterator(*this, limit());	}
	HFSM2_CONSTEXPR(11)	CIterator	cend()							  const noexcept	{ return CIterator(*this, limit());	}

private:
	HFSM2_CONSTEXPR(11)	Index first()								  const noexcept	{ return 0;							}
	HFSM2_CONSTEXPR(11)	Index next(const Index index)				  const noexcept	{ return index + 1;					}
	HFSM2_CONSTEXPR(11)	Index limit()								  const noexcept	{ return _count;					}

private:
	Index _count = 0;

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

	Item _items[CAPACITY] {};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif
};

template <typename T>
class ArrayT<T, 0> final {
public:
	using Item	= T;
	using Index	= UCapacity<0>;

	static constexpr Index CAPACITY = 0;
};

}
}


namespace hfsm2 {
namespace detail {

template <typename T, Long NC>
template <typename N>
HFSM2_CONSTEXPR(14)
T&
StaticArrayT<T, NC>::operator[] (const N index) noexcept	{
	HFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[static_cast<Index>(index)];
}

template <typename T, Long NC>
template <typename N>
HFSM2_CONSTEXPR(14)
const T&
StaticArrayT<T, NC>::operator[] (const N index) const noexcept	{
	HFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[static_cast<Index>(index)];
}

template <typename T, Long NC>
HFSM2_CONSTEXPR(14)
void
StaticArrayT<T, NC>::fill(const Item filler) noexcept {
	for (Item& item : _items)
		item = filler;
}

template <typename T, Long NC>
HFSM2_CONSTEXPR(14)
bool
StaticArrayT<T, NC>::empty() const noexcept {
	for (const Item& item : _items)
		if (item != filler<Item>())
			return false;

	return true;
}

template <typename T, Long NC>
template <typename... TArgs>
HFSM2_CONSTEXPR(14)
typename ArrayT<T, NC>::Index
ArrayT<T, NC>::emplace(const TArgs&... args) noexcept {
	HFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{args...};

	return _count++;
}

template <typename T, Long NC>
template <typename... TArgs>
HFSM2_CONSTEXPR(14)
typename ArrayT<T, NC>::Index
ArrayT<T, NC>::emplace(TArgs&&... args) noexcept {
	HFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{forward<TArgs>(args)...};

	return _count++;
}

template <typename T, Long NC>
template <typename N>
HFSM2_CONSTEXPR(14)
typename ArrayT<T, NC>::Item&
ArrayT<T, NC>::operator[] (const N index) noexcept {
	HFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[static_cast<Index>(index)];
}

template <typename T, Long NC>
template <typename N>
HFSM2_CONSTEXPR(14)
const typename ArrayT<T, NC>::Item&
ArrayT<T, NC>::operator[] (const N index) const noexcept {
	HFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[static_cast<Index>(index)];
}

template <typename T, Long NC>
template <Long N>
HFSM2_CONSTEXPR(14)
ArrayT<T, NC>&
ArrayT<T, NC>::operator += (const ArrayT<T, N>& other) noexcept {
	for (const auto& item : other)
		emplace(item);

	return *this;
}

}
}

namespace hfsm2 {
namespace detail {

struct Units final {
	HFSM2_CONSTEXPR(11)	Units(Short unit_  = INVALID_SHORT,
							  Short width_ = INVALID_SHORT)				noexcept
		: unit {unit_ }
		, width{width_}
	{}

	Short unit;
	Short width;
};

template <unsigned NCapacity>
class BitArrayT final {
public:
	using Index		= UCapacity<NCapacity>;

	static constexpr Index CAPACITY   = NCapacity;
	static constexpr Index UNIT_COUNT = contain(CAPACITY, 8);

	class Bits {
		template <unsigned>
		friend class BitArrayT;

	private:
		HFSM2_CONSTEXPR(11)	explicit Bits(uint8_t* const storage,
										  const Index width)			noexcept
			: _storage{storage}
			, _width{width}
		{}

	public:
		HFSM2_CONSTEXPR(14)	explicit operator bool()			  const noexcept;

		HFSM2_CONSTEXPR(14)	void clear()								noexcept;

		template <Short NIndex>
		HFSM2_CONSTEXPR(14)	bool get()							  const noexcept;

		template <Short NIndex>
		HFSM2_CONSTEXPR(14)	void set()									noexcept;

		template <Short NIndex>
		HFSM2_CONSTEXPR(14)	void clear()								noexcept;

		HFSM2_CONSTEXPR(14)	bool get  (const Index index)		  const noexcept;
		HFSM2_CONSTEXPR(14)	void set  (const Index index)				noexcept;
		HFSM2_CONSTEXPR(14)	void clear(const Index index)				noexcept;

	private:
		uint8_t* const _storage;
		const Index _width;
	};

	class CBits {
		template <unsigned>
		friend class BitArrayT;

	private:
		HFSM2_CONSTEXPR(11)	explicit CBits(const uint8_t* const storage,
										   const Index width)			noexcept
			: _storage{storage}
			, _width{width}
		{}

	public:
		HFSM2_CONSTEXPR(14)	explicit operator bool()			  const noexcept;

		template <Short NIndex>
		HFSM2_CONSTEXPR(14)	bool get()							  const noexcept;

		HFSM2_CONSTEXPR(14)	bool get(const Index index)			  const noexcept;

	private:
		const uint8_t* const _storage;
		const Index _width;
	};

public:
	HFSM2_CONSTEXPR(14)	BitArrayT()										noexcept	{ clear();	}

	HFSM2_CONSTEXPR(14)	void clear()									noexcept;

	template <Short NIndex>
	HFSM2_CONSTEXPR(14)	bool get()								  const noexcept;

	template <Short NIndex>
	HFSM2_CONSTEXPR(14)	void set()										noexcept;

	template <Short NIndex>
	HFSM2_CONSTEXPR(14)	void clear()									noexcept;

	HFSM2_CONSTEXPR(14)	bool empty()							  const noexcept;

	template <typename TIndex>
	HFSM2_CONSTEXPR(14)	bool get  (const TIndex index)			  const noexcept;

	template <typename TIndex>
	HFSM2_CONSTEXPR(14)	void set  (const TIndex index)					noexcept;

	template <typename TIndex>
	HFSM2_CONSTEXPR(14)	void clear(const TIndex index)					noexcept;

	template <Short NUnit, Short NWidth>
	HFSM2_CONSTEXPR(14)	 Bits  bits()									noexcept;

	template <Short NUnit, Short NWidth>
	HFSM2_CONSTEXPR(14)	CBits cbits()							  const noexcept;

	HFSM2_CONSTEXPR(14)	 Bits  bits(const Units& units)					noexcept;
	HFSM2_CONSTEXPR(14)	CBits cbits(const Units& units)			  const noexcept;

private:
	uint8_t _storage[UNIT_COUNT] {};
};

template <>
class BitArrayT<0> final {
public:
	HFSM2_CONSTEXPR(14)	void clear()									noexcept	{}

	HFSM2_CONSTEXPR(11)	bool empty()							  const noexcept	{ return true;	}
};

}
}


namespace hfsm2 {
namespace detail {

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
BitArrayT<NCapacity>::Bits::operator bool() const noexcept {
	const Short fullUnits = _width / 8;

	// TODO: cover this case
	for (Index i = 0; i < fullUnits; ++i)
		if (_storage[i])
			return true;

	const Short bit = _width % 8;
	const uint8_t mask = (1 << bit) - 1;
	const uint8_t& unit = _storage[fullUnits];

	return (unit & mask) != 0;
}

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::Bits::clear() noexcept {
	const Index unitCount = contain(_width, 8);

	for (Index i = 0; i < unitCount; ++i)
		_storage[i] = uint8_t{0};
}

template <unsigned NCapacity>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::Bits::get() const noexcept {
	constexpr Index INDEX = NIndex;
	HFSM2_ASSERT(INDEX < _width);

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	return (_storage[unitIndex] & mask) != 0;
}

template <unsigned NCapacity>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::Bits::set() noexcept {
	constexpr Index INDEX = NIndex;
	HFSM2_ASSERT(INDEX < _width);

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	_storage[unitIndex] |= mask;
}

template <unsigned NCapacity>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::Bits::clear() noexcept {
	constexpr Index INDEX = NIndex;
	HFSM2_ASSERT(INDEX < _width);

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	_storage[unitIndex] &= ~mask;
}

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::Bits::get(const Index index) const noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / 8;
	const Index bit  = index % 8;
	const uint8_t mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::Bits::set(const Index index) noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / 8;
	const Index bit  = index % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] |= mask;
}

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::Bits::clear(const Index index) noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / 8;
	const Index bit  = index % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] &= ~mask;
}

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
BitArrayT<NCapacity>::CBits::operator bool() const noexcept {
	const Short fullUnits = _width / 8;

	for (Index i = 0; i < fullUnits; ++i)
		if (_storage[i])
			return true;

	const Short bit = _width % 8;
	const uint8_t mask = (1 << bit) - 1;
	const uint8_t& unit = _storage[fullUnits];

	return (unit & mask) != 0;
}

template <unsigned NCapacity>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::CBits::get() const noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < _width, "");

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	return (_storage[unitIndex] & mask) != 0;
}

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::CBits::get(const Index index) const noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / 8;
	const Index bit  = index % 8;
	const uint8_t mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::clear() noexcept {
	for (uint8_t& unit : _storage)
		unit = uint8_t{0};
}

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::empty() const noexcept {
	for (const uint8_t& unit : _storage)
		if (unit != uint8_t{0})
			return false;

	return true;
}

template <unsigned NCapacity>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::get() const noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < CAPACITY, "");

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	return (_storage[unitIndex] & mask) != 0;
}

template <unsigned NCapacity>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::set() noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < CAPACITY, "");

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	_storage[unitIndex] |= mask;
}

template <unsigned NCapacity>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::clear() noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < CAPACITY, "");

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	_storage[unitIndex] &= ~mask;
}

template <unsigned NCapacity>
template <typename TIndex>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::get(const TIndex index) const noexcept {
	HFSM2_ASSERT(index < CAPACITY);

	const Index unit = static_cast<Index>(index) / 8;
	const Index bit  = static_cast<Index>(index) % 8;
	const uint8_t mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

template <unsigned NCapacity>
template <typename TIndex>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::set(const TIndex index) noexcept {
	HFSM2_ASSERT(index < CAPACITY);

	const Index unit = static_cast<Index>(index) / 8;
	const Index bit  = static_cast<Index>(index) % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] |= mask;
}

template <unsigned NCapacity>
template <typename TIndex>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::clear(const TIndex index) noexcept {
	HFSM2_ASSERT(index < CAPACITY);

	const Index unit = static_cast<Index>(index) / 8;
	const Index bit  = static_cast<Index>(index) % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] &= ~mask;
}

template <unsigned NCapacity>
template <Short NUnit, Short NWidth>
HFSM2_CONSTEXPR(14)
typename BitArrayT<NCapacity>::Bits
BitArrayT<NCapacity>::bits() noexcept {
	constexpr Short UNIT  = NUnit;
	constexpr Short WIDTH = NWidth;
	static_assert(UNIT + contain(WIDTH, 8) <= UNIT_COUNT, "");

	return Bits{_storage + UNIT, WIDTH};
}

template <unsigned NCapacity>
template <Short NUnit, Short NWidth>
HFSM2_CONSTEXPR(14)
typename BitArrayT<NCapacity>::CBits
BitArrayT<NCapacity>::cbits() const noexcept {
	constexpr Short UNIT  = NUnit;
	constexpr Short WIDTH = NWidth;
	static_assert(UNIT + contain(WIDTH, 8) <= UNIT_COUNT, "");

	return CBits{_storage + UNIT, WIDTH};
}

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
typename BitArrayT<NCapacity>::Bits
BitArrayT<NCapacity>::bits(const Units& units) noexcept {
	HFSM2_ASSERT(units.unit + contain(units.width, 8) <= UNIT_COUNT);

	return Bits{_storage + units.unit, units.width};
}

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
typename BitArrayT<NCapacity>::CBits
BitArrayT<NCapacity>::cbits(const Units& units) const noexcept {
	HFSM2_ASSERT(units.unit + contain(units.width, 8) <= UNIT_COUNT);

	return CBits{_storage + units.unit, units.width};
}

}
}

#if HFSM2_PLANS_AVAILABLE()

namespace hfsm2 {
namespace detail {

#pragma pack(push, 1)

struct TaskBase {
	HFSM2_CONSTEXPR(11)	TaskBase()										noexcept	{}

	HFSM2_CONSTEXPR(11)	TaskBase(const StateID origin_,
								 const StateID destination_,
								 const TransitionType type_)			noexcept
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

HFSM2_CONSTEXPR(11)
bool
operator == (const TaskBase& lhs,
			 const TaskBase& rhs)										noexcept
{
	return lhs.origin	   == rhs.origin &&
		   lhs.destination == rhs.destination &&
		   lhs.type		   == rhs.type;
}

template <typename TPayload>
struct TaskT final
	: TaskBase
{
	using Payload = TPayload;
	using Storage = uint8_t[sizeof(Payload)];

	using TaskBase::TaskBase;

	HFSM2_CONSTEXPR(14)	TaskT()											noexcept	{
		new (&storage) Payload{};
	}

	HFSM2_CONSTEXPR(14)	TaskT(const StateID origin_,
							  const StateID destination_,
							  const TransitionType type_,
							  const Payload& payload)					noexcept
		: TaskBase{origin_, destination_, type_}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	HFSM2_CONSTEXPR(11)
	const Payload*
	payload()													  const noexcept	{
		return payloadSet ?
			reinterpret_cast<const Payload*>(&storage) : nullptr;
	}

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

	alignas(Payload) Storage storage {};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

	bool payloadSet = false;
};

template <>
struct TaskT<void> final
	: TaskBase
{
	using TaskBase::TaskBase;
};

#pragma pack(pop)

template <typename TPayload, Long NCapacity>
class TaskListT {
public:
	using Index		= Long;

	static constexpr Index CAPACITY	= NCapacity;
	static constexpr Index INVALID	= Index (-1);

private:
	using Payload	= TPayload;
	using Item		= TaskT<Payload>;

public:
	HFSM2_CONSTEXPR(14)	 void clear()											noexcept;

	template <typename... TArgs>
	HFSM2_CONSTEXPR(14)	Index emplace(TArgs&&... args)							noexcept;

	HFSM2_CONSTEXPR(14)	void remove(const Index i)								noexcept;

	HFSM2_CONSTEXPR(14)		  Item& operator[] (const Index i)					noexcept;
	HFSM2_CONSTEXPR(11)	const Item& operator[] (const Index i)			  const noexcept;

	HFSM2_CONSTEXPR(11)	Index count()									  const noexcept	{ return _count;		}
	HFSM2_CONSTEXPR(11)	bool  empty()									  const noexcept	{ return _count == 0;	}

private:
	HFSM2_IF_ASSERT(void verifyStructure(const Index occupied = INVALID)  const noexcept);

private:
	Index _vacantHead = 0;
	Index _vacantTail = 0;
	Index _last		  = 0;
	Index _count	  = 0;
	Item _items[CAPACITY] {};
};

template <typename TItem>
class TaskListT<TItem, 0> {};

}
}


#if HFSM2_PLANS_AVAILABLE()

namespace hfsm2 {
namespace detail {

template <typename TP, Long NC>
HFSM2_CONSTEXPR(14)
void
TaskListT<TP, NC>::clear() noexcept {
	_vacantHead	= 0;
	_vacantTail	= 0;
	_last		= 0;
	_count		= 0;
}

template <typename TP, Long NC>
template <typename... TA>
HFSM2_CONSTEXPR(14)
Long
TaskListT<TP, NC>::emplace(TA&&... args) noexcept {
	HFSM2_ASSERT(_last  <= CAPACITY);

	if (_count < CAPACITY) {
		HFSM2_ASSERT(_vacantHead < CAPACITY);
		HFSM2_ASSERT(_vacantTail < CAPACITY);

		const Index index = _vacantHead;
		Item& item = _items[index];

		if (_vacantHead != _vacantTail) {
			// recycle
			HFSM2_ASSERT(item.prev == INVALID);
			HFSM2_ASSERT(item.next != INVALID);

			_vacantHead = item.next;

			Item& head = _items[_vacantHead];
			HFSM2_ASSERT(head.prev == index);
			head.prev = INVALID;
		} else if (_last < CAPACITY - 1) {
			// grow
			++_last;
			_vacantHead = _last;
			_vacantTail = _last;

			Item& vacant = _items[_vacantHead];
			vacant.prev = INVALID;
			vacant.next = INVALID;
		} else {
			// last
			HFSM2_ASSERT(_count == CAPACITY - 1);

			_last = CAPACITY;
			_vacantHead = INVALID;
			_vacantTail = INVALID;
		}

		new (&item) Item{forward<TA>(args)...};
		++_count;

		HFSM2_IF_ASSERT(verifyStructure());

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

template <typename TP, Long NC>
HFSM2_CONSTEXPR(14)
void
TaskListT<TP, NC>::remove(const Index i) noexcept {
	HFSM2_ASSERT(i < CAPACITY && _count);

	Item& item = _items[i];

	if (_count < CAPACITY) {
		HFSM2_ASSERT(_vacantHead < CAPACITY);
		HFSM2_ASSERT(_vacantTail < CAPACITY);

		item.prev = INVALID;
		item.next = _vacantHead;

		Item& head = _items[_vacantHead];
		head.prev = i;

		_vacantHead = i;
	} else {
		// 0 -> 1
		HFSM2_ASSERT(_count		 == CAPACITY);
		HFSM2_ASSERT(_vacantHead == INVALID);
		HFSM2_ASSERT(_vacantTail == INVALID);

		item.prev = INVALID;
		item.next = INVALID;

		_vacantHead = i;
		_vacantTail = i;
	}

	--_count;

	HFSM2_IF_ASSERT(verifyStructure());
}

template <typename TP, Long NC>
HFSM2_CONSTEXPR(14)
typename TaskListT<TP, NC>::Item&
TaskListT<TP, NC>::operator[] (const Index i) noexcept {
	HFSM2_IF_ASSERT(verifyStructure());

	return _items[i];
}

template <typename TP, Long NC>
HFSM2_CONSTEXPR(11)
const typename TaskListT<TP, NC>::Item&
TaskListT<TP, NC>::operator[] (const Index i) const noexcept {
	HFSM2_IF_ASSERT(verifyStructure());

	return _items[i];
}

#if HFSM2_ASSERT_AVAILABLE()

template <typename TP, Long NC>
void
TaskListT<TP, NC>::verifyStructure(const Index occupied) const noexcept {
	if (_count < CAPACITY) {
		HFSM2_ASSERT(_vacantHead < CAPACITY);
		HFSM2_ASSERT(_vacantTail < CAPACITY);

		HFSM2_ASSERT(_items[_vacantHead].prev == INVALID);
		HFSM2_ASSERT(_items[_vacantTail].next == INVALID);

		Index emptyCount = 1;

		for (Index c = _vacantHead; c != _vacantTail; ) {
			HFSM2_ASSERT(occupied != c);

			const Item& current = _items[c];

			const Long f = current.next;
			if (f != INVALID) {
				// next
				const Item& following = _items[f];

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

}
}

#endif
#endif

namespace hfsm2 {
namespace detail {

enum Strategy {
	Composite,
	Resumable,
	Selectable,

//#if HFSM2_UTILITY_THEORY_AVAILABLE()
	Utilitarian,
	RandomUtil,
//#endif
};

#pragma pack(push, 1)

struct alignas(2 * sizeof(Short)) Parent final {
	HFSM2_CONSTEXPR(11)
	Parent() = default;

	HFSM2_CONSTEXPR(11)
	Parent(const ForkID forkId_)										noexcept
		: forkId{forkId_}
	{}

	HFSM2_CONSTEXPR(11)
	Parent(const ForkID forkId_,
		   const Short prong_)											noexcept
		: forkId{forkId_}
		, prong{prong_}
	{}

	HFSM2_CONSTEXPR(11)
	explicit operator bool()									  const noexcept	{
		return forkId != INVALID_FORK_ID &&
			   prong  != INVALID_SHORT;
	}

	ForkID forkId = INVALID_FORK_ID;
	Short  prong  = INVALID_SHORT;
};

#pragma pack(pop)

template <typename TRegistry>
struct BackUpT final {
	using CompoForks = typename TRegistry::CompoForks;
	using OrthoForks = typename TRegistry::OrthoForks;

	CompoForks compoRequested;
	OrthoForks orthoRequested;
	CompoForks compoResumable;
};

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
					 , TPayload>> final
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;

	static constexpr Long  STATE_COUNT		= StateList::SIZE;
	static constexpr Long  REGION_COUNT		= RegionList::SIZE;
	static constexpr Short COMPO_REGIONS	= NCompoCount;
	static constexpr Short ORTHO_REGIONS	= NOrthoCount;
	static constexpr Short ORTHO_UNITS		= NOrthoUnits;

	using Payload		= TPayload;
	using Transition	= TransitionT<Payload>;

	using StateParents	= StaticArrayT<Parent, STATE_COUNT>;

	using CompoParents	= StaticArrayT<Parent, COMPO_REGIONS>;
	using OrthoParents	= StaticArrayT<Parent, ORTHO_REGIONS>;
	using OrthoUnits	= StaticArrayT<Units,  ORTHO_UNITS>;
	using RegionSizes	= StaticArrayT<Short,  REGION_COUNT>;

	using CompoForks	= StaticArrayT<Short,  COMPO_REGIONS>;
	using OrthoForks	= BitArrayT	  <ORTHO_UNITS * 8>;
	using OrthoBits		= typename OrthoForks::Bits;
	using CompoRemains	= BitArrayT	  <COMPO_REGIONS>;

#if HFSM2_PLANS_AVAILABLE()
	using CompoStatuses	= BitArrayT	  <COMPO_REGIONS>;
#endif

	using BackUp		= BackUpT<RegistryT>;

	HFSM2_CONSTEXPR(14)	Short activeSubState (const StateID stateId)	  const noexcept;

	HFSM2_CONSTEXPR(11)	bool isActive		 ()							  const noexcept;
	HFSM2_CONSTEXPR(14)	bool isActive		 (const StateID stateId)	  const noexcept;
	HFSM2_CONSTEXPR(14)	bool isResumable	 (const StateID stateId)	  const noexcept;

	HFSM2_CONSTEXPR(14)	bool isPendingChange (const StateID stateId)	  const noexcept;
	HFSM2_CONSTEXPR(14)	bool isPendingEnter	 (const StateID stateId)	  const noexcept;
	HFSM2_CONSTEXPR(14)	bool isPendingExit	 (const StateID stateId)	  const noexcept;

	HFSM2_CONSTEXPR(14)	const Parent&	  forkParent(const ForkID forkId) const noexcept;

	HFSM2_CONSTEXPR(14)	OrthoBits requestedOrthoFork(const ForkID forkId)		noexcept;

	HFSM2_CONSTEXPR(14)	bool requestImmediate(const Transition& request)		noexcept;
	HFSM2_CONSTEXPR(14)	void requestScheduled(const StateID stateId)			noexcept;

	HFSM2_CONSTEXPR(14)	void clearRequests	 ()									noexcept;
	HFSM2_CONSTEXPR(14)	void clear			 ()									noexcept;

	HFSM2_CONSTEXPR(11)	bool empty			 ()							  const noexcept;

	StateParents stateParents;
	CompoParents compoParents;
	OrthoParents orthoParents;
	OrthoUnits	 orthoUnits;
	RegionSizes	 regionSizes;

	CompoForks compoRequested{INVALID_SHORT};
	OrthoForks orthoRequested;
	CompoForks compoActive	 {INVALID_SHORT};
	CompoForks compoResumable{INVALID_SHORT};

	CompoRemains compoRemains;

#if HFSM2_PLANS_AVAILABLE()
	CompoStatuses compoStatuses;
#endif
};

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
					 , TPayload>> final
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;

	static constexpr Long  STATE_COUNT		= StateList::SIZE;
	static constexpr Long  REGION_COUNT		= RegionList::SIZE;
	static constexpr Short COMPO_REGIONS	= NCompoCount;

	using Payload		= TPayload;
	using Transition	= TransitionT<Payload>;

	using StateParents	= StaticArrayT<Parent, STATE_COUNT>;
	using CompoParents	= StaticArrayT<Parent, COMPO_REGIONS>;
	using RegionSizes	= StaticArrayT<Short,  REGION_COUNT>;

	using CompoForks	= StaticArrayT<Short,  COMPO_REGIONS>;
	using OrthoForks	= BitArrayT	  <0>;
	using CompoRemains	= BitArrayT	  <COMPO_REGIONS>;

#if HFSM2_PLANS_AVAILABLE()
	using CompoStatuses	= BitArrayT	  <COMPO_REGIONS>;
#endif

	using BackUp		= BackUpT<RegistryT>;

	HFSM2_CONSTEXPR(14)	Short activeSubState (const StateID stateId)	  const noexcept;

	HFSM2_CONSTEXPR(11)	bool isActive		 ()							  const noexcept;
	HFSM2_CONSTEXPR(14)	bool isActive		 (const StateID stateId)	  const noexcept;
	HFSM2_CONSTEXPR(14)	bool isResumable	 (const StateID stateId)	  const noexcept;

	HFSM2_CONSTEXPR(14)	bool isPendingChange (const StateID stateId)	  const noexcept;
	HFSM2_CONSTEXPR(14)	bool isPendingEnter	 (const StateID stateId)	  const noexcept;
	HFSM2_CONSTEXPR(14)	bool isPendingExit	 (const StateID stateId)	  const noexcept;

	HFSM2_CONSTEXPR(14)	const Parent& forkParent(const ForkID forkId)	  const noexcept;

	HFSM2_CONSTEXPR(14)	bool requestImmediate(const Transition& request)		noexcept;
	HFSM2_CONSTEXPR(14)	void requestScheduled(const StateID stateId)			noexcept;

	HFSM2_CONSTEXPR(14)	void clearRequests	 ()									noexcept;
	HFSM2_CONSTEXPR(14)	void clear			 ()									noexcept;

	HFSM2_CONSTEXPR(11)	bool empty			 ()							  const noexcept;

	StateParents stateParents;
	CompoParents compoParents;
	RegionSizes	 regionSizes;

	CompoForks compoRequested{INVALID_SHORT};
	OrthoForks orthoRequested;
	CompoForks compoActive	 {INVALID_SHORT};
	CompoForks compoResumable{INVALID_SHORT};

	CompoRemains compoRemains;

#if HFSM2_PLANS_AVAILABLE()
	CompoStatuses compoStatuses;
#endif
};

template <typename TRegistry>
HFSM2_CONSTEXPR(14)
void
backup(const TRegistry& registry,
	   BackUpT<TRegistry>& copy) noexcept
{
	overwrite(copy.compoRequested, registry.compoRequested);
	overwrite(copy.orthoRequested, registry.orthoRequested);
	overwrite(copy.compoResumable, registry.compoResumable);
}

template <typename TRegistry>
HFSM2_CONSTEXPR(14)
void
restore(TRegistry& registry,
		const BackUpT<TRegistry>& copy) noexcept
{
	overwrite(registry.compoRequested, copy.compoRequested);
	overwrite(registry.orthoRequested, copy.orthoRequested);
	overwrite(registry.compoResumable, copy.compoResumable);
}

}
}


namespace hfsm2 {
namespace detail {

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
Short
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::activeSubState(const StateID stateId) const noexcept {
	const StateID subStateId = stateId + 1;

	if (HFSM2_CHECKED(	 stateId < STATE_COUNT) &&
		HFSM2_CHECKED(subStateId < STATE_COUNT))
	{
		if (const Parent parent = stateParents[subStateId]) {
			HFSM2_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return compoActive[parent.forkId - 1];
		}
	}

	return INVALID_SHORT;
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(11)
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isActive() const noexcept {
	return compoActive[0] != INVALID_SHORT;
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isActive(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT)) {
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

	return false;
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isResumable(const StateID stateId) const noexcept {
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

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isPendingChange(const StateID stateId) const noexcept {
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

	return false;
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isPendingEnter(const StateID stateId) const noexcept {
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

	return false;
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isPendingExit(const StateID stateId) const noexcept {
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

	return false;
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
const Parent&
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::forkParent(const ForkID forkId) const noexcept {
	HFSM2_ASSERT(forkId != 0);

	return forkId > 0 ?
		compoParents[ forkId - 1] :
		orthoParents[-forkId - 1];
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
typename RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::OrthoBits
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::requestedOrthoFork(const ForkID forkId) noexcept {
	HFSM2_ASSERT(forkId < 0);
	const Units& units = orthoUnits[-forkId - 1];

	return orthoRequested.bits(units);
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::requestImmediate(const Transition& request) noexcept {
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

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::requestScheduled(const StateID stateId) noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT)) {
		const Parent parent = stateParents[stateId];

		HFSM2_ASSERT(parent.forkId != 0);
		if (parent.forkId > 0)
			compoResumable[parent.forkId - 1] = parent.prong;
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::clearRequests() noexcept {
	compoRequested.clear();
	orthoRequested.clear();
	compoRemains  .clear();
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::clear() noexcept {
	clearRequests();

	compoActive	  .clear();
	compoResumable.clear();
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(11)
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::empty() const noexcept {
	return compoRequested.empty()
		&& orthoRequested.empty()
		&& compoRemains	 .empty()
		&& compoActive	 .empty()
		&& compoResumable.empty();
}

}
}

namespace hfsm2 {
namespace detail {

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_ HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
Short
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::activeSubState(const StateID stateId) const noexcept {
	const StateID subStateId = stateId + 1;

	if (HFSM2_CHECKED(	 stateId < STATE_COUNT) &&
		HFSM2_CHECKED(subStateId < STATE_COUNT))
	{
		if (const Parent parent = stateParents[subStateId]) {
			HFSM2_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return compoActive[parent.forkId - 1];
		}
	}

	return INVALID_SHORT;
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_ HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(11)
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isActive() const noexcept {
	return compoActive[0] != INVALID_SHORT;
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_ HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isActive(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT)) {
		if (Parent parent = stateParents[stateId]) {
			HFSM2_ASSERT(parent.forkId > 0);

			return parent.prong == compoActive[parent.forkId - 1];
		} else
			return true;
	}

	return false;
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_ HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isResumable(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM2_ASSERT(parent.forkId > 0);

			return parent.prong == compoResumable[parent.forkId - 1];
		}

	return false;
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_ HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isPendingChange(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM2_ASSERT(parent.forkId > 0);

			return compoRequested[parent.forkId - 1] !=
				   compoActive	 [parent.forkId - 1];
		}

	return false;
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_ HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isPendingEnter(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM2_ASSERT(parent.forkId > 0);

			return parent.prong != compoActive	 [parent.forkId - 1] &&
				   parent.prong == compoRequested[parent.forkId - 1];
		}

	return false;
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_ HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::isPendingExit(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM2_ASSERT(parent.forkId > 0);

			return parent.prong == compoActive	 [parent.forkId - 1] &&
				   parent.prong != compoRequested[parent.forkId - 1];
		}

	return false;
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_ HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
const Parent&
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::forkParent(const ForkID forkId) const noexcept {
	HFSM2_ASSERT(forkId > 0);

	return compoParents[forkId - 1];
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_ HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::requestImmediate(const Transition& request) noexcept {
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

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_ HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::requestScheduled(const StateID stateId) noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT)) {
		const Parent parent = stateParents[stateId];

		HFSM2_ASSERT(parent.forkId > 0);
		compoResumable[parent.forkId - 1] = parent.prong;
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_ HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::clearRequests() noexcept {
	compoRequested.clear();
	orthoRequested.clear();
	compoRemains  .clear();
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_ HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::clear() noexcept {
	clearRequests();

	compoActive	  .clear();
	compoResumable.clear();
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_ HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(11)
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC_, 0, 0 HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::empty() const noexcept {
	return compoRequested.empty()
		&& orthoRequested.empty()
		&& compoRemains	 .empty()
		&& compoActive	 .empty()
		&& compoResumable.empty();
}

}
}

namespace hfsm2 {
namespace detail {

#pragma pack(push, 1)

struct Status final {
	enum class Result {
		NONE,
		SUCCESS,
		FAILURE
	};

	Result result = Result::NONE;
	bool outerTransition = false;

	HFSM2_CONSTEXPR(11)	Status(const Result result_ = Result::NONE,
							   const bool outerTransition_ = false)		noexcept;

	HFSM2_CONSTEXPR(11)	explicit operator bool()				  const noexcept;

	HFSM2_CONSTEXPR(14)	void clear()									noexcept;
};

#pragma pack(pop)

HFSM2_CONSTEXPR(14) Status  operator |  (Status& lhs, const Status rhs)	noexcept;
HFSM2_CONSTEXPR(14) Status& operator |= (Status& lhs, const Status rhs)	noexcept;

template <>
HFSM2_CONSTEXPR(11) Status  filler<Status>()							noexcept;

#if HFSM2_PLANS_AVAILABLE()

#pragma pack(push, 2)

struct TaskLink final {
	Long prev		= INVALID_LONG;
	Long next		= INVALID_LONG;
};

template <>
HFSM2_CONSTEXPR(11)
TaskLink
filler<TaskLink>()														noexcept	{
	return TaskLink{};
}

struct Bounds final {
	Long first		= INVALID_LONG;
	Long last		= INVALID_LONG;
};

#pragma pack(pop)

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
					 , TPayload>> final
{
	using StateList			= TStateList;
	using RegionList		= TRegionList;
	using Payload			= TPayload;

	static constexpr Long STATE_COUNT	= StateList ::SIZE;
	static constexpr Long REGION_COUNT	= RegionList::SIZE;
	static constexpr Long TASK_CAPACITY	= NTaskCapacity;

	using Task				= TaskT		  <Payload>;
	using Tasks				= TaskListT   <Payload,  TASK_CAPACITY>;
	using TaskLinks			= StaticArrayT<TaskLink, TASK_CAPACITY>;
	using Payloads			= StaticArrayT<Payload,  TASK_CAPACITY>;

	using TasksBounds		= ArrayT	  <Bounds, REGION_COUNT>;
	using TasksBits			= BitArrayT   <			STATE_COUNT>;
	using RegionBits		= BitArrayT   <		   REGION_COUNT>;
	using RegionStatuses	= StaticArrayT<Status, REGION_COUNT>;

	Tasks tasks;
	TaskLinks taskLinks;
	Payloads taskPayloads;
	TasksBits payloadExists;

	TasksBounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	RegionBits planExists;
	RegionStatuses headStatuses;
	RegionStatuses subStatuses;

	HFSM2_CONSTEXPR(14)	void clearTaskStatus  (const StateID stateId)		noexcept;
	HFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID stateId) const noexcept;

	HFSM2_CONSTEXPR(14)	void clearRegionStatuses()							noexcept;
	HFSM2_CONSTEXPR(14)	void clear()										noexcept;

#if HFSM2_ASSERT_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void verifyPlans()							  const noexcept;
	HFSM2_CONSTEXPR(14)	Long verifyPlan(const RegionID stateId)		  const noexcept;
#endif
};

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
					 , void>> final
{
	using StateList			= TStateList;
	using RegionList		= TRegionList;

	static constexpr Long  STATE_COUNT	= StateList ::SIZE;
	static constexpr Long REGION_COUNT	= RegionList::SIZE;
	static constexpr Long TASK_CAPACITY	= NTaskCapacity;

	using Task				= TaskT		  <void>;
	using Tasks				= TaskListT	  <void,	 TASK_CAPACITY>;
	using TaskLinks			= StaticArrayT<TaskLink, TASK_CAPACITY>;

	using TasksBounds		= ArrayT	  <Bounds, REGION_COUNT>;
	using TasksBits			= BitArrayT	  <			STATE_COUNT>;
	using RegionBits		= BitArrayT	  <		   REGION_COUNT>;
	using RegionStatuses	= StaticArrayT<Status, REGION_COUNT>;

	Tasks tasks;
	TaskLinks taskLinks;

	TasksBounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	RegionBits planExists;
	RegionStatuses headStatuses;
	RegionStatuses subStatuses;

	HFSM2_CONSTEXPR(14)	void clearTaskStatus  (const StateID stateId)		noexcept;
	HFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID stateId) const noexcept;

	HFSM2_CONSTEXPR(14)	void clearRegionStatuses()							noexcept;
	HFSM2_CONSTEXPR(14)	void clear()										noexcept;

#if HFSM2_ASSERT_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void verifyPlans()							  const noexcept;
	HFSM2_CONSTEXPR(14)	Long verifyPlan(const RegionID stateId)		  const noexcept;
#endif
};

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
					 , TPayload>> final
{
	HFSM2_CONSTEXPR(14)	void clearTaskStatus  (const StateID)				noexcept	{}
	HFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID)		  const noexcept	{}

	HFSM2_CONSTEXPR(14)	void clearRegionStatuses()							noexcept	{}
	HFSM2_CONSTEXPR(14)	void clear()										noexcept	{}

#if HFSM2_ASSERT_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void verifyPlans()							  const noexcept	{}
#endif
};

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NOrthoCount
		, Long NOrthoUnits
		, Long NSubstitutionLimit
		, Long NTaskCapacity>
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
					 , void>> final
{
	HFSM2_CONSTEXPR(14)	void clearTaskStatus  (const StateID)				noexcept	{}
	HFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID)		  const noexcept	{}

	HFSM2_CONSTEXPR(14)	void clearRegionStatuses()							noexcept	{}
	HFSM2_CONSTEXPR(14)	void clear()										noexcept	{}

#if HFSM2_ASSERT_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void verifyPlans()							  const noexcept	{}
#endif
};

#endif

}
}


namespace hfsm2 {
namespace detail {

HFSM2_CONSTEXPR(11)
Status::Status(const Result result_,
			   const bool outerTransition_) noexcept
	: result{result_}
	, outerTransition{outerTransition_}
{}

HFSM2_CONSTEXPR(11)
Status::operator bool() const noexcept	{
	return result != Result::NONE || outerTransition;
}

HFSM2_CONSTEXPR(14)
void
Status::clear() noexcept {
	result = Result::NONE;
	outerTransition = false;
}

HFSM2_CONSTEXPR(14)
Status
operator | (Status& lhs,
			const Status rhs) noexcept
{
	const Status::Result result = lhs.result > rhs.result ?
		lhs.result : rhs.result;

	return Status{result, lhs.outerTransition || rhs.outerTransition};
}

HFSM2_CONSTEXPR(14)
Status&
operator |= (Status& lhs,
			 const Status rhs) noexcept
{
	const Status::Result result = lhs.result > rhs.result ?
		lhs.result : rhs.result;

	lhs = Status{result, lhs.outerTransition || rhs.outerTransition};

	return lhs;
}

template <>
HFSM2_CONSTEXPR(11)
Status
filler<Status>() noexcept {
	return Status{};
}

#if HFSM2_PLANS_AVAILABLE()

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::verifyEmptyStatus(const StateID HFSM2_IF_ASSERT(stateId)) const noexcept {
#if HFSM2_ASSERT_AVAILABLE()

	if (stateId != INVALID_STATE_ID) {
		HFSM2_ASSERT(!tasksSuccesses.get(stateId));
		HFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::clearRegionStatuses() noexcept {
	headStatuses.clear();
	subStatuses	.clear();
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::clear() noexcept {
	tasks		  .clear();
	taskLinks	  .clear();
	taskPayloads  .clear();
	payloadExists .clear();

	tasksBounds	  .clear();
	tasksSuccesses.clear();
	tasksFailures .clear();
	planExists	  .clear();

	clearRegionStatuses();
}

#if HFSM2_ASSERT_AVAILABLE()

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::verifyPlans() const noexcept {
	Long planCount = 0;

	for (RegionID regionId = 0; regionId < REGION_COUNT; ++regionId)
		planCount += verifyPlan(regionId);

	HFSM2_ASSERT(tasks.count() == planCount);
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
HFSM2_CONSTEXPR(14)
Long
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::verifyPlan(const RegionID regionId) const noexcept {
	Long length = 0;
	const Bounds& bounds = tasksBounds[regionId];

	if (bounds.first != INVALID_LONG) {
		HFSM2_ASSERT(bounds.last != INVALID_LONG);

		for (Long slow = bounds.first, fast = slow; ; ) {
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

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::verifyEmptyStatus(const StateID HFSM2_IF_ASSERT(stateId)) const noexcept {
#if HFSM2_ASSERT_AVAILABLE()

	if (stateId != INVALID_STATE_ID) {
		HFSM2_ASSERT(!tasksSuccesses.get(stateId));
		HFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::clearRegionStatuses() noexcept {
	headStatuses.clear();
	subStatuses	.clear();
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::clear() noexcept {
	tasks		  .clear();
	taskLinks	  .clear();

	tasksBounds	  .clear();
	tasksSuccesses.clear();
	tasksFailures .clear();
	planExists	  .clear();

	clearRegionStatuses();
}

#if HFSM2_ASSERT_AVAILABLE()

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::verifyPlans() const noexcept {
	Long planCount = 0;

	for (RegionID regionId = 0; regionId < REGION_COUNT; ++regionId)
		planCount += verifyPlan(regionId);

	HFSM2_ASSERT(tasks.count() == planCount);
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
HFSM2_CONSTEXPR(14)
Long
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::verifyPlan(const RegionID regionId) const noexcept {
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
#endif

}
}

namespace hfsm2 {
namespace detail {

#if HFSM2_PLANS_AVAILABLE()

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

	template <typename, typename>
	friend class R_;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	static constexpr Long TASK_CAPACITY = Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using Task			= typename PlanData::Task;
	using TaskLinks		= typename PlanData::TaskLinks;

	struct IteratorT final {
		HFSM2_CONSTEXPR(14)	IteratorT(const CPlanT& plan)					noexcept;

		HFSM2_CONSTEXPR(14)	explicit operator bool()				  const noexcept;

		HFSM2_CONSTEXPR(14)	void operator ++()								noexcept;

		HFSM2_CONSTEXPR(11)	const Task& operator  *()				  const noexcept	{ return  _plan._planData.tasks[_curr];		}
		HFSM2_CONSTEXPR(11)	const Task* operator ->()				  const noexcept	{ return &_plan._planData.tasks[_curr];		}

		HFSM2_CONSTEXPR(14)	Long next()								  const noexcept;

		const CPlanT& _plan;
		Long _curr;
		Long _next;
	};

private:
	HFSM2_CONSTEXPR(11)	CPlanT(const PlanData& planData,
							   const RegionID regionId_)					noexcept
		: _planData{planData}
		, _bounds{planData.tasksBounds[regionId_]}
	{}

	template <typename TState>
	static constexpr StateID  stateId()										noexcept	{ return					   index<StateList , TState>() ;	}

	template <typename TState>
	static constexpr RegionID regionId()									noexcept	{ return static_cast<RegionID>(index<RegionList, TState>());	}

public:
	HFSM2_CONSTEXPR(14)	explicit operator bool()					  const noexcept;

	HFSM2_CONSTEXPR(14)	IteratorT first()									noexcept	{ return IteratorT{*this};					}

private:
	const PlanData& _planData;
	const Bounds& _bounds;
};

template <typename TArgs>
class PlanBaseT {
	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using Registry		= RegistryT<Args>;

	static constexpr Long  TASK_CAPACITY	= Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using Task			= typename PlanData::Task;
	using Tasks			= typename PlanData::Tasks;
	using TaskLinks		= typename PlanData::TaskLinks;
	using TaskIndex		= typename TaskLinks::Index;

	struct CIteratorT final {
		HFSM2_CONSTEXPR(14)	CIteratorT(const PlanBaseT& plan)				noexcept;

		HFSM2_CONSTEXPR(14)	explicit operator bool()				  const noexcept;

		HFSM2_CONSTEXPR(14)	void operator ++()								noexcept;

		HFSM2_CONSTEXPR(14)	const Task& operator  *()				  const noexcept	{ return  _plan._planData.tasks[_curr];		}
		HFSM2_CONSTEXPR(11)	const Task* operator ->()				  const noexcept	{ return &_plan._planData.tasks[_curr];		}

		HFSM2_CONSTEXPR(14)	Long next()								  const noexcept;

		const PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

	struct IteratorT final {
		HFSM2_CONSTEXPR(14)	IteratorT(PlanBaseT& plan)						noexcept;

		HFSM2_CONSTEXPR(14)	explicit operator bool()				  const noexcept;

		HFSM2_CONSTEXPR(14)	void operator ++()								noexcept;

		HFSM2_CONSTEXPR(14)	Task& operator  *()								noexcept	{ return  _plan._planData.tasks[_curr];		}
		HFSM2_CONSTEXPR(14)	Task* operator ->()								noexcept	{ return &_plan._planData.tasks[_curr];		}

		HFSM2_CONSTEXPR(14)	void remove()									noexcept	{ _plan.remove(_curr);						}

		HFSM2_CONSTEXPR(14)	Long next()								  const noexcept;

		PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

protected:
	HFSM2_CONSTEXPR(11)	PlanBaseT(Registry& registry,
								  PlanData& planData,
								  const RegionID regionId_)					noexcept;

	template <typename TState>
	static constexpr StateID  stateId()										noexcept	{ return					   index<StateList , TState>() ;	}

	template <typename TState>
	static constexpr RegionID regionId()									noexcept	{ return static_cast<RegionID>(index<RegionList, TState>());	}

	HFSM2_CONSTEXPR(14)	bool append(const StateID origin,
									const StateID destination,
									const TransitionType transitionType)	noexcept;

	HFSM2_CONSTEXPR(14)	bool linkTask(const Long index)						noexcept;

	HFSM2_CONSTEXPR(14)	void clearTasks()									noexcept;

public:
	HFSM2_CONSTEXPR(14)	explicit operator bool()					  const noexcept;

	/// @brief Clear all tasks from the plan
	HFSM2_CONSTEXPR(14)	void clear()										noexcept;

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, acts depending on the region type)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool change   (const StateID origin, const StateID destination)		noexcept	{ return append	  (origin, destination, TransitionType::CHANGE	 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool change   (const StateID destination)							noexcept	{ return change	  (stateId<TOrigin>() , destination				 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool change   ()													noexcept	{ return change	  (stateId<TOrigin>() , stateId<TDestination>()	 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the initial state)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool restart  (const StateID origin, const StateID destination)		noexcept	{ return append	  (origin, destination, TransitionType::RESTART	 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the initial state)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool restart  (const StateID destination)							noexcept	{ return restart  (stateId<TOrigin>() , destination				 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the initial state)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool restart  ()													noexcept	{ return restart  (stateId<TOrigin>() , stateId<TDestination>()	 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool resume   (const StateID origin, const StateID destination)		noexcept	{ return append	  (origin, destination, TransitionType::RESUME	 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool resume   (const StateID destination)							noexcept	{ return resume	  (stateId<TOrigin>() , destination				 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool resume   ()													noexcept	{ return resume	  (stateId<TOrigin>() , stateId<TDestination>()	 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool select   (const StateID origin, const StateID destination)		noexcept	{ return append	  (origin, destination, TransitionType::SELECT	 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool select   (const StateID destination)							noexcept	{ return select	  (stateId<TOrigin>() , destination				 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool select   ()													noexcept	{ return select	  (stateId<TOrigin>() , stateId<TDestination>()	 );	}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	bool utilize  (const StateID origin, const StateID destination) 	noexcept	{ return append	  (origin, destination, TransitionType::UTILIZE	 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool utilize  (const StateID destination)							noexcept	{ return utilize  (stateId<TOrigin>() , destination				 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool utilize  ()													noexcept	{ return utilize  (stateId<TOrigin>() , stateId<TDestination>()	 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	bool randomize(const StateID origin, const StateID destination)		noexcept	{ return append	  (origin, destination, TransitionType::RANDOMIZE);	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool randomize(const StateID destination)							noexcept	{ return randomize(stateId<TOrigin>() , destination				 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool randomize()													noexcept	{ return randomize(stateId<TOrigin>() , stateId<TDestination>()	 );	}

#endif

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool schedule (const StateID origin, const StateID destination)		noexcept	{ return append	  (origin, destination, TransitionType::SCHEDULE );	}

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool schedule (const StateID destination)							noexcept	{ return schedule (stateId<TOrigin>() , destination				 );	}

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool schedule ()													noexcept	{ return schedule (stateId<TOrigin>() , stateId<TDestination>()	 );	}

	/// @brief Begin iteration over plan tasks for the current region
	/// @return IteratorT to the first task
	HFSM2_CONSTEXPR(14)	 IteratorT first()													noexcept	{ return  IteratorT{*this};											}

	/// @brief Begin iteration over plan tasks
	/// @return CIteratorT to the first task
	HFSM2_CONSTEXPR(11)	CIteratorT first()											  const noexcept	{ return CIteratorT{*this};											}

private:
	HFSM2_CONSTEXPR(14)	void remove(const Long task)										noexcept;

protected:
	Registry& _registry;
	PlanData& _planData;
	const RegionID _regionId;
	Bounds& _bounds;
};

template <typename TArgs>
class PlanT;

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
					 , NTaskCapacity
					 , TPayload>;

	using Payload		= typename Args::Payload;

	using PlanBase		= PlanBaseT<Args>;

	using PlanBase::PlanBase;

	using PlanBase::linkTask;

	HFSM2_CONSTEXPR(14)	bool append(const StateID origin,
									const StateID destination,
									const TransitionType transitionType,
									const Payload& payload)										noexcept;

public:

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, acts depending on the region type)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool changeWith   (const StateID origin, const StateID destination, const Payload& payload) noexcept	{ return append(origin								 , destination								 , TransitionType::CHANGE   ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool changeWith   (						 const StateID destination, const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::CHANGE   ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool changeWith   (													const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::CHANGE   ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the initial state)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool restartWith  (const StateID origin, const StateID destination, const Payload& payload) noexcept	{ return append(origin								 , destination								 , TransitionType::RESTART  ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the initial state)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool restartWith  (						 const StateID destination, const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::RESTART  ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the initial state)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool restartWith  (													const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::RESTART  ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool resumeWith   (const StateID origin, const StateID destination, const Payload& payload) noexcept	{ return append(origin								 , destination								 , TransitionType::RESUME   ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool resumeWith   (						 const StateID destination, const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::RESUME   ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool resumeWith   (													const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::RESUME   ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool selectWith   (const StateID origin, const StateID destination, const Payload& payload) noexcept	{ return append(origin								 , destination								 , TransitionType::SELECT   ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool selectWith   (						 const StateID destination, const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::SELECT   ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool selectWith   (													const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::SELECT   ,	   payload );	}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	bool utilizeWith  (const StateID origin, const StateID destination, const Payload& payload) noexcept	{ return append(origin								 , destination								 , TransitionType::UTILIZE  ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool utilizeWith  (						 const StateID destination, const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::UTILIZE  ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool utilizeWith  (													const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::UTILIZE  ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	bool randomizeWith(const StateID origin, const StateID destination, const Payload& payload) noexcept	{ return append(origin								 , destination								 , TransitionType::RANDOMIZE,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool randomizeWith(						 const StateID destination, const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::RANDOMIZE,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool randomizeWith(													const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::RANDOMIZE,	   payload );	}

#endif

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool scheduleWith (const StateID origin, const StateID destination, const Payload& payload) noexcept	{ return append(origin								 , destination								 , TransitionType::SCHEDULE ,	   payload );	}

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool scheduleWith (						 const StateID destination, const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::SCHEDULE ,	   payload );	}

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool scheduleWith (													const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::SCHEDULE ,	   payload );	}

private:
	using PlanBase::_planData;
	using PlanBase::_regionId;
};

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
	template <typename, typename>
	friend class R_;

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

}
}


namespace hfsm2 {
namespace detail {

#if HFSM2_PLANS_AVAILABLE()

template <typename TArgs>
HFSM2_CONSTEXPR(14)
CPlanT<TArgs>::IteratorT::IteratorT(const CPlanT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
CPlanT<TArgs>::IteratorT::operator bool() const noexcept {
	HFSM2_ASSERT(_curr < CPlanT::TASK_CAPACITY ||
				 _curr == INVALID_LONG);

	return _curr < CPlanT::TASK_CAPACITY;
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
CPlanT<TArgs>::IteratorT::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
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

template <typename TArgs>
HFSM2_CONSTEXPR(14)
CPlanT<TArgs>::operator bool() const noexcept {
	HFSM2_ASSERT(_bounds.first < TASK_CAPACITY &&
				 _bounds.last  < TASK_CAPACITY ||
				 _bounds.last == INVALID_LONG);

	return _bounds.first < TASK_CAPACITY;
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
PlanBaseT<TArgs>::CIteratorT::CIteratorT(const PlanBaseT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
	, _next{next()}
{}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
PlanBaseT<TArgs>::CIteratorT::operator bool() const noexcept {
	HFSM2_ASSERT(_curr < PlanBaseT::TASK_CAPACITY ||
				 _curr == INVALID_LONG);

	return _curr < PlanBaseT::TASK_CAPACITY;
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
PlanBaseT<TArgs>::CIteratorT::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
Long
PlanBaseT<TArgs>::CIteratorT::next() const noexcept {
	if (_curr < PlanBaseT::TASK_CAPACITY) {
		const TaskLink& link = _plan._planData.taskLinks[_curr];

		return link.next;
	} else {
		HFSM2_ASSERT(_curr == INVALID_LONG);

		return INVALID_LONG;
	}
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
PlanBaseT<TArgs>::IteratorT::IteratorT(PlanBaseT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
	, _next{next()}
{}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
PlanBaseT<TArgs>::IteratorT::operator bool() const noexcept {
	HFSM2_ASSERT(_curr < PlanBaseT::TASK_CAPACITY ||
				 _curr == INVALID_LONG);

	return _curr < PlanBaseT::TASK_CAPACITY;
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
PlanBaseT<TArgs>::IteratorT::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
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

template <typename TArgs>
HFSM2_CONSTEXPR(11)
PlanBaseT<TArgs>::PlanBaseT(Registry& registry,
							PlanData& planData,
							const RegionID regionId_) noexcept
	: _registry{registry}
	, _planData{planData}
	, _regionId{regionId_}
	, _bounds{planData.tasksBounds[regionId_]}
{}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
bool
PlanBaseT<TArgs>::append(const StateID origin,
						 const StateID destination,
						 const TransitionType transitionType) noexcept
{
	if (_planData.tasks.count() < TASK_CAPACITY) {
		_planData.planExists.set(_regionId);

		return linkTask(_planData.tasks.emplace(origin, destination, transitionType));
	} else
		return false;
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
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

			TaskLink& lastLink = _planData.taskLinks[_bounds.last];
			HFSM2_ASSERT(lastLink.next == INVALID_LONG);

			lastLink.next  = index;

			TaskLink& currLink = _planData.taskLinks[index];
			HFSM2_ASSERT(currLink.prev == INVALID_LONG);

			currLink.prev  = _bounds.last;

			_bounds.last   = index;
		}

		return true;
	} else
		return false;
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
PlanBaseT<TArgs>::clearTasks() noexcept {
	if (_bounds.first < TaskLinks::CAPACITY) {
		HFSM2_ASSERT(_bounds.last < TaskLinks::CAPACITY);

		for (Long index = _bounds.first;
			 index != INVALID_LONG;
			 )
		{
			HFSM2_ASSERT(index < TaskLinks::CAPACITY);

			const TaskLink& link = _planData.taskLinks[index];
			HFSM2_ASSERT(index == _bounds.first ?
							 link.prev == INVALID_LONG :
							 link.prev <  TaskLinks::CAPACITY);

			const Long next = link.next;

			remove(index);

			index = next;
		}

		_bounds.first = INVALID_LONG;
		_bounds.last  = INVALID_LONG;
	} else {
		HFSM2_ASSERT(_bounds.first == INVALID_LONG);
		HFSM2_ASSERT(_bounds.last  == INVALID_LONG);
	}
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
PlanBaseT<TArgs>::operator bool() const noexcept {
	HFSM2_ASSERT(_bounds.first < TASK_CAPACITY &&
				 _bounds.last  < TASK_CAPACITY ||
				 _bounds.last == INVALID_LONG);

	return _bounds.first < TASK_CAPACITY;
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
PlanBaseT<TArgs>::clear() noexcept {
	clearTasks();

	for (Short i = _regionId;
		 i < _registry.regionSizes[_regionId];
		 ++i)
	{
		_planData.tasksSuccesses.clear(i);
		_planData.tasksFailures .clear(i);
	}
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
PlanBaseT<TArgs>::remove(const Long index) noexcept {
	HFSM2_ASSERT(_planData.planExists.get(_regionId));
	HFSM2_ASSERT(_bounds.first < TaskLinks::CAPACITY);
	HFSM2_ASSERT(_bounds.last  < TaskLinks::CAPACITY);

	HFSM2_ASSERT(index < TaskLinks::CAPACITY);

	TaskLink& link = _planData.taskLinks[index];

	if (link.prev < TaskLinks::CAPACITY) {
		TaskLink& prev = _planData.taskLinks[link.prev];
		prev.next = link.next;
	} else {
		HFSM2_ASSERT(_bounds.first == index);
		_bounds.first = link.next;
	}

	if (link.next < TaskLinks::CAPACITY) {
		TaskLink& next = _planData.taskLinks[link.next];
		next.prev = link.prev;
	} else {
		HFSM2_ASSERT(_bounds.last == index);
		_bounds.last = link.prev;
	}

	link.prev = INVALID_LONG;
	link.next = INVALID_LONG;

	_planData.tasks.remove(index);
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
HFSM2_CONSTEXPR(14)
bool
PlanT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::append(const StateID origin,
																								   const StateID destination,
																								   const TransitionType transitionType,
																								   const Payload& payload) noexcept
{
	_planData.planExists.set(_regionId);

	return linkTask(_planData.tasks.emplace(origin, destination, transitionType, payload));
}

#endif

}
}

namespace hfsm2 {
namespace detail {

template <typename TArgs>
struct CoreT {
	using Context			= typename TArgs::Context;
	using PureContext		= typename TArgs::PureContext;

	using Registry			= RegistryT<TArgs>;

	using Payload			= typename TArgs::Payload;
	using Transition		= TransitionT<Payload>;
	using TransitionSet		= ArrayT<Transition, TArgs::COMPO_REGIONS>;
	using TransitionSets	= ArrayT<Transition, TArgs::COMPO_REGIONS * TArgs::SUBSTITUTION_LIMIT>;

#if HFSM2_PLANS_AVAILABLE()
	using PlanData			= PlanDataT<TArgs>;
#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	using TransitionTargets	= StaticArrayT<Short, TArgs::STATE_COUNT>;
#endif

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using RNG				= typename TArgs::RNG;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger			= typename TArgs::Logger;
#endif

	HFSM2_CONSTEXPR(14)	explicit CoreT(Context& context_
									 HFSM2_IF_UTILITY_THEORY(, RNG& rng_)
									 HFSM2_IF_LOG_INTERFACE(, Logger* const logger_ = nullptr))		noexcept;

	HFSM2_CONSTEXPR(14)	explicit CoreT(PureContext&& context_
									 HFSM2_IF_UTILITY_THEORY(, RNG& rng_)
									 HFSM2_IF_LOG_INTERFACE(, Logger* const logger_ = nullptr))		noexcept;

	HFSM2_CONSTEXPR(14) CoreT(const CoreT&  other)													noexcept;
	HFSM2_CONSTEXPR(14) CoreT(		CoreT&& other)													noexcept;

	Context context;
	Registry registry;
	TransitionSet requests;
	HFSM2_IF_PLANS(PlanData planData);
	HFSM2_IF_TRANSITION_HISTORY(TransitionTargets transitionTargets);
	HFSM2_IF_TRANSITION_HISTORY(TransitionSets previousTransitions);
	HFSM2_IF_UTILITY_THEORY(RNG& rng);
	HFSM2_IF_LOG_INTERFACE(Logger* logger);
};

}
}


namespace hfsm2 {
namespace detail {

template <typename TArgs>
HFSM2_CONSTEXPR(14)
CoreT<TArgs>::CoreT(Context& context_
				  HFSM2_IF_UTILITY_THEORY(, RNG& rng_)
				  HFSM2_IF_LOG_INTERFACE(, Logger* const logger_)) noexcept
	: context{context_}
	HFSM2_IF_UTILITY_THEORY(, rng{rng_})
	HFSM2_IF_LOG_INTERFACE(, logger{logger_})
{}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
CoreT<TArgs>::CoreT(PureContext&& context_
				  HFSM2_IF_UTILITY_THEORY(, RNG& rng_)
				  HFSM2_IF_LOG_INTERFACE(, Logger* const logger_)) noexcept
	: context{move(context_)}
	HFSM2_IF_UTILITY_THEORY(, rng	{rng_	})
	HFSM2_IF_LOG_INTERFACE (, logger{logger_})
{}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
CoreT<TArgs>::CoreT(const CoreT& other) noexcept
	: context {other.context }
	, registry{other.registry}
	, requests{other.requests}
	HFSM2_IF_PLANS			   (, planData			 {other.planData		   })
	HFSM2_IF_TRANSITION_HISTORY(, transitionTargets  {other.transitionTargets  })
	HFSM2_IF_TRANSITION_HISTORY(, previousTransitions{other.previousTransitions})
	HFSM2_IF_UTILITY_THEORY	   (, rng				 {other.rng				   })
	HFSM2_IF_LOG_INTERFACE	   (, logger			 {other.logger			   })
{}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
CoreT<TArgs>::CoreT(CoreT&& other) noexcept
	: context {move(other.context )}
	, registry{move(other.registry)}
	, requests{move(other.requests)}
	HFSM2_IF_PLANS			   (, planData			 {move(other.planData			)})
	HFSM2_IF_TRANSITION_HISTORY(, transitionTargets  {move(other.transitionTargets	)})
	HFSM2_IF_TRANSITION_HISTORY(, previousTransitions{move(other.previousTransitions)})
	HFSM2_IF_UTILITY_THEORY	   (, rng				 {move(other.rng				)})
	HFSM2_IF_LOG_INTERFACE	   (, logger			 {move(other.logger				)})
{}

}
}

namespace hfsm2 {
namespace detail {

template <typename TArgs>
class ConstControlT {
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

	using Core				= CoreT<TArgs>;

	using Registry			= RegistryT<TArgs>;

	using Payload			= typename TArgs::Payload;
	using Transition		= TransitionT<Payload>;
	using TransitionSet		= ArrayT<Transition, TArgs::COMPO_REGIONS>;
	using TransitionSets	= ArrayT<Transition, TArgs::COMPO_REGIONS * TArgs::SUBSTITUTION_LIMIT>;

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	using TransitionTargets	= StaticArrayT<Short, TArgs::STATE_COUNT>;
#endif

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using RNG				= typename TArgs::RNG;
#endif

#if HFSM2_PLANS_AVAILABLE()
	using PlanData			= PlanDataT<TArgs>;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger			= typename TArgs::Logger;
#endif

	struct Origin final {
		HFSM2_CONSTEXPR(14)	Origin(ConstControlT& control_,
								   const StateID stateId_)					noexcept;

		HFSM2_CONSTEXPR(20)	~Origin()										noexcept;

		ConstControlT& control;
		const StateID prevId;
	};

	struct Region {
		HFSM2_CONSTEXPR(14)	Region(ConstControlT& control,
								   const RegionID regionId_)				noexcept;

		HFSM2_CONSTEXPR(20)	~Region()										noexcept;

		ConstControlT& control;
		const RegionID prevId;
	};

	HFSM2_CONSTEXPR(11)	ConstControlT(const Core& core)						noexcept
		: _core{core}
	{}

	HFSM2_CONSTEXPR(14)	void   setRegion(const RegionID regionId_)			noexcept;
	HFSM2_CONSTEXPR(14)	void resetRegion(const RegionID regionId_)			noexcept;

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void pinLastTransition(const StateID stateId_,
											   const Short index)			noexcept;
#endif

public:

#if HFSM2_PLANS_AVAILABLE()
	using CPlan				= CPlanT<TArgs>;
#endif

	/// @brief Get current state's identifier
	/// @return Numeric state identifier
	HFSM2_CONSTEXPR(11) StateID stateId()							  const noexcept	{ return _originId;												}

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()										noexcept	{ return					   index<StateList , TState>() ;	}

	/// @brief Get region identifier for a region type
	/// @tparam TState Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static constexpr RegionID regionId()									noexcept	{ return static_cast<RegionID>(index<RegionList, TState>());	}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	HFSM2_CONSTEXPR(11)	const Context& _()							  const noexcept	{ return _core.context;											}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	HFSM2_CONSTEXPR(11)	const Context& context()					  const noexcept	{ return _core.context;											}

	/// @brief Inspect current transition requests
	/// @return Array of transition requests
	HFSM2_CONSTEXPR(11)	const TransitionSet& requests()				  const noexcept	{ return _core.requests;										}

	/// @brief Get active sub-state's index for the current region
	/// @return Region's active sub-state index
	HFSM2_CONSTEXPR(14)	Short activeSubState()						  const noexcept	{ return _core.registry.activeSubState(_originId);				}

	/// @brief Get region's active sub-state's index
	/// @param stateId Region's head state ID
	/// @return Region's active sub-state index
	HFSM2_CONSTEXPR(14)	Short activeSubState(const StateID stateId_)  const noexcept	{ return _core.registry.activeSubState(stateId_);				}

	/// @brief Get region's active sub-state's index
	/// @tparam TState Region's head state type
	/// @return Region's active sub-state index
	template <typename TState>
	HFSM2_CONSTEXPR(14)	Short activeSubState()						  const noexcept	{ return _core.registry.activeSubState(stateId<TState>());		}

	/// @brief Check if a state is active
	/// @param stateId State identifier
	/// @return State active status
	HFSM2_CONSTEXPR(11)	bool isActive   (const StateID stateId_)	  const noexcept	{ return _core.registry.isActive(stateId_);						}

	/// @brief Check if a state is active
	/// @tparam TState State type
	/// @return State active status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isActive	()							  const noexcept	{ return isActive(stateId<TState>());							}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @param stateId State identifier
	/// @return State resumable status
	HFSM2_CONSTEXPR(11)	bool isResumable(const StateID stateId_)	  const noexcept	{ return _core.registry.isResumable(stateId_);					}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @tparam TState State type
	/// @return State resumable status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isResumable()							  const noexcept	{ return isResumable(stateId<TState>());						}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @param stateId State identifier
	/// @return State scheduled status
	HFSM2_CONSTEXPR(11)	bool isScheduled(const StateID stateId_)	  const noexcept	{ return isResumable(stateId_);									}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @tparam TState State type
	/// @return State scheduled status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isScheduled()							  const noexcept	{ return isResumable(stateId<TState>());						}

#if HFSM2_PLANS_AVAILABLE()

	/// @brief Access read-only plan for the current region
	/// @return Plan for the current region
	HFSM2_CONSTEXPR(11)	CPlan plan()								  const noexcept	{ return CPlan{_core.registry, _core.planData, _regionId};						}

	/// @brief Access read-only plan for a region
	/// @param regionId Region identifier
	/// @return Read-only plan for the region
	HFSM2_CONSTEXPR(14)	CPlan plan(const RegionID regionId_)		  const noexcept	{ return CPlan{_core.registry, _core.planData, regionId_};						}

	/// @brief Access read-only plan for a region
	/// @tparam TRegion Region head state type
	/// @return Read-only Plan for the region
	template <typename TRegion>
	HFSM2_CONSTEXPR(14)	CPlan plan()								  const noexcept	{ return CPlan{_core.registry, _core.planData, regionId<TRegion>()};			}

#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get transitions processed during last 'update()', 'react()' or 'replayTransitions()'
	/// @return Array of last transition requests
	HFSM2_CONSTEXPR(11)	const TransitionSets& previousTransitions()					  const noexcept	{ return _core.previousTransitions;				}

	/// @brief Get the last transition that caused the state to be activated
	/// @param stateId State identifier
	/// @return Pointer to the last transition that activated the state
	HFSM2_CONSTEXPR(14)	const Transition* lastTransitionTo(const StateID stateId_)	  const noexcept;

	/// @brief Get the last transition that caused the state to be activated
	/// @tparam TState State type
	/// @return Pointer to the last transition that activated the state
	template <typename TState>
	HFSM2_CONSTEXPR(11)	const Transition* lastTransitionTo()						  const noexcept	{ return lastTransitionTo(stateId<TState>());	}

	/// @brief Get the last transition that caused the current state to be activated
	/// @return Pointer to the last transition that activated the current state
	HFSM2_CONSTEXPR(14)	const Transition* lastTransition()							  const noexcept;

#endif

protected:
	const Core& _core;
	StateID _originId = INVALID_STATE_ID;
	RegionID _regionId = 0;
};

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

	using Core				= CoreT<TArgs>;

	using Registry			= RegistryT<TArgs>;

	using Payload			= typename TArgs::Payload;
	using Transition		= TransitionT<Payload>;
	using TransitionSet		= ArrayT<Transition, TArgs::COMPO_REGIONS>;
	using TransitionSets	= ArrayT<Transition, TArgs::COMPO_REGIONS * TArgs::SUBSTITUTION_LIMIT>;

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	using TransitionTargets	= StaticArrayT<Short, TArgs::STATE_COUNT>;
#endif

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using RNG				= typename TArgs::RNG;
#endif

#if HFSM2_PLANS_AVAILABLE()
	using PlanData			= PlanDataT<TArgs>;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger			= typename TArgs::Logger;
#endif

	struct Origin final {
		HFSM2_CONSTEXPR(14)	Origin(ControlT& control_,
								   const StateID stateId_)					noexcept;

		HFSM2_CONSTEXPR(20)	~Origin()										noexcept;

		ControlT& control;
		const StateID prevId;
	};

	struct Region {
		HFSM2_CONSTEXPR(14)	Region(ControlT& control,
								   const RegionID regionId_)				noexcept;

		HFSM2_CONSTEXPR(20)	~Region()										noexcept;

		ControlT& control;
		const RegionID prevId;
	};

	HFSM2_CONSTEXPR(11)	ControlT(Core& core)								noexcept
		: _core{core}
	{}

	HFSM2_CONSTEXPR(14)	void   setRegion(const RegionID regionId_)			noexcept;
	HFSM2_CONSTEXPR(14)	void resetRegion(const RegionID regionId_)			noexcept;

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void pinLastTransition(const StateID stateId_,
											   const Short index)			noexcept;
#endif

public:

#if HFSM2_PLANS_AVAILABLE()
	using CPlan				= CPlanT<TArgs>;
#endif

	/// @brief Get current state's identifier
	/// @return Numeric state identifier
	constexpr StateID stateId()										  const noexcept	{ return _originId;												}

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()										noexcept	{ return					   index<StateList , TState>() ;	}

	/// @brief Get region identifier for a region type
	/// @tparam TState Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static constexpr RegionID regionId()									noexcept	{ return static_cast<RegionID>(index<RegionList, TState>());	}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	HFSM2_CONSTEXPR(14)		  Context& _()									noexcept	{ return _core.context;											}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	HFSM2_CONSTEXPR(11)	const Context& _()							  const noexcept	{ return _core.context;											}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	HFSM2_CONSTEXPR(14)		  Context& context()							noexcept	{ return _core.context;											}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	HFSM2_CONSTEXPR(11)	const Context& context()					  const noexcept	{ return _core.context;											}

	/// @brief Inspect current transition requests
	/// @return Array of transition requests
	HFSM2_CONSTEXPR(11)	const TransitionSet& requests()				  const noexcept	{ return _core.requests;										}

	/// @brief Get active sub-state's index for the current region
	/// @return Region's active sub-state index
	HFSM2_CONSTEXPR(14)	Short activeSubState()						  const noexcept	{ return _core.registry.activeSubState(_originId);				}

	/// @brief Get region's active sub-state's index
	/// @param stateId Region's head state ID
	/// @return Region's active sub-state index
	HFSM2_CONSTEXPR(14)	Short activeSubState(const StateID stateId_)  const noexcept	{ return _core.registry.activeSubState(stateId_);				}

	/// @brief Get region's active sub-state's index
	/// @tparam TState Region's head state type
	/// @return Region's active sub-state index
	template <typename TState>
	HFSM2_CONSTEXPR(14)	Short activeSubState()						  const noexcept	{ return _core.registry.activeSubState(stateId<TState>());		}

	/// @brief Check if a state is active
	/// @param stateId State identifier
	/// @return State active status
	HFSM2_CONSTEXPR(11)	bool isActive   (const StateID stateId_)	  const noexcept	{ return _core.registry.isActive(stateId_);						}

	/// @brief Check if a state is active
	/// @tparam TState State type
	/// @return State active status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isActive	()							  const noexcept	{ return isActive(stateId<TState>());							}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @param stateId State identifier
	/// @return State resumable status
	HFSM2_CONSTEXPR(11)	bool isResumable(const StateID stateId_)	  const noexcept	{ return _core.registry.isResumable(stateId_);					}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @tparam TState State type
	/// @return State resumable status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isResumable()							  const noexcept	{ return isResumable(stateId<TState>());						}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @param stateId State identifier
	/// @return State scheduled status
	HFSM2_CONSTEXPR(11)	bool isScheduled(const StateID stateId_)	  const noexcept	{ return isResumable(stateId_);									}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @tparam TState State type
	/// @return State scheduled status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isScheduled()							  const noexcept	{ return isResumable(stateId<TState>());						}

#if HFSM2_PLANS_AVAILABLE()

	/// @brief Access read-only plan for the current region
	/// @return Plan for the current region
	HFSM2_CONSTEXPR(11)	CPlan plan()								  const noexcept	{ return CPlan{_core.registry, _core.planData, _regionId};						}

	/// @brief Access read-only plan for a region
	/// @param regionId Region identifier
	/// @return Read-only plan for the region
	HFSM2_CONSTEXPR(14)	CPlan plan(const RegionID regionId_)		  const noexcept	{ return CPlan{_core.registry, _core.planData, regionId_};						}

	/// @brief Access read-only plan for a region
	/// @tparam TRegion Region head state type
	/// @return Read-only plan for the region
	template <typename TRegion>
	HFSM2_CONSTEXPR(14)	CPlan plan()										noexcept	{ return CPlan{_core.registry, _core.planData, regionId<TRegion>()};			}

	/// @brief Access read-only plan for a region
	/// @tparam TRegion Region head state type
	/// @return Read-only Plan for the region
	template <typename TRegion>
	HFSM2_CONSTEXPR(14)	CPlan plan()								  const noexcept	{ return CPlan{_core.registry, _core.planData, regionId<TRegion>()};			}

#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get transitions processed during last 'update()', 'react()' or 'replayTransitions()'
	/// @return Array of last transition requests
	HFSM2_CONSTEXPR(11)	const TransitionSets& previousTransitions()					  const noexcept	{ return _core.previousTransitions;				}

	/// @brief Get the last transition that caused the state to be activated
	/// @param stateId State identifier
	/// @return Pointer to the last transition that activated the state
	HFSM2_CONSTEXPR(14)	const Transition* lastTransitionTo(const StateID stateId_)	  const noexcept;

	/// @brief Get the last transition that caused the state to be activated
	/// @tparam TState State type
	/// @return Pointer to the last transition that activated the state
	template <typename TState>
	HFSM2_CONSTEXPR(11)	const Transition* lastTransitionTo()						  const noexcept	{ return lastTransitionTo(stateId<TState>());	}

	/// @brief Get the last transition that caused the current state to be activated
	/// @return Pointer to the last transition that activated the current state
	HFSM2_CONSTEXPR(14)	const Transition* lastTransition()							  const noexcept;

#endif

protected:
	Core& _core;
	StateID _originId = INVALID_STATE_ID;
	RegionID _regionId = 0;
};

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

	template <typename, typename>
	friend class RV_;

protected:
	using Control		= ControlT<TArgs>;

	using typename Control::StateList;
	using typename Control::RegionList;

	using typename Control::Core;

	using TransitionSets	= typename Core::TransitionSets;

#if HFSM2_PLANS_AVAILABLE()
	using typename Control::PlanData;
#endif

	struct Region {
		HFSM2_CONSTEXPR(14)	Region(PlanControlT& control,
								   const RegionID regionId_,
								   const StateID index,
								   const Long size)					noexcept;

		HFSM2_CONSTEXPR(20)	~Region()								noexcept;

		PlanControlT& control;
		const RegionID prevId;
		const Long prevIndex;
		const Long prevSize;
	};

	HFSM2_CONSTEXPR(11)	PlanControlT(Core& core,
									 const TransitionSets& currentTransitions)	noexcept
		: Control{core}
		, _currentTransitions{currentTransitions}
	{}

	HFSM2_CONSTEXPR(14)	void   setRegion(const RegionID regionId_,
										 const StateID index,
										 const Long size)			noexcept;

	HFSM2_CONSTEXPR(14)	void resetRegion(const RegionID regionId_,
										 const StateID index,
										 const Long size)			noexcept;

public:

#if HFSM2_PLANS_AVAILABLE()
	using typename Control::CPlan;

	using Plan			= PlanT<TArgs>;

	/// @brief Access plan for the current region
	/// @return Plan for the current region
	HFSM2_CONSTEXPR(14)  Plan plan()								noexcept	{ return  Plan{_core.registry, _core.planData, _regionId							};	}

	/// @brief Access plan for a region
	/// @param regionId
	/// @return Plan for the region
	HFSM2_CONSTEXPR(14)  Plan plan(const RegionID regionId_)		noexcept	{ return  Plan{_core.registry, _core.planData,  regionId_							};	}

	/// @brief Access plan for a region
	/// @tparam TRegion Region head state type
	/// @return Plan for the region
	template <typename TRegion>
	HFSM2_CONSTEXPR(14)  Plan plan()								noexcept	{ return  Plan{_core.registry, _core.planData, Control::template regionId<TRegion>()};	}

	/// @brief Access plan for the current region
	/// @return Plan for the current region
	HFSM2_CONSTEXPR(11)	CPlan plan()						  const noexcept	{ return CPlan{_core.registry, _core.planData, _regionId							};	}

	/// @brief Access plan for a region
	/// @param regionId
	/// @return Plan for the region
	HFSM2_CONSTEXPR(11)	CPlan plan(const RegionID regionId_)  const noexcept	{ return CPlan{_core.registry, _core.planData,  regionId_							};	}

	/// @brief Access plan for a region
	/// @tparam TRegion Region head state type
	/// @return Plan for the region
	template <typename TRegion>
	HFSM2_CONSTEXPR(11)	CPlan plan()						  const noexcept	{ return CPlan{_core.registry, _core.planData, Control::template regionId<TRegion>()};	}

#endif

	/// @brief Get current transition requests
	/// @return ArrayT of pending transition requests
	HFSM2_CONSTEXPR(11)	const TransitionSets& currentTransitions()	  const noexcept	{ return _currentTransitions;															}

protected:
	using Control::_core;
	using Control::_regionId;

	const TransitionSets& _currentTransitions;

	StateID _regionStateId = 0;
	Long _regionSize = StateList::SIZE;
	Status _status;
};

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

#if HFSM2_PLANS_AVAILABLE()
	using TasksBits		= BitArrayT<StateList::SIZE>;
#endif

	struct Lock final {
		HFSM2_CONSTEXPR(14)	Lock(FullControlBaseT& control_)		noexcept;
		HFSM2_CONSTEXPR(20)	~Lock()									noexcept;

		FullControlBaseT* const control;
	};

	using PlanControl::PlanControl;

#if HFSM2_PLANS_AVAILABLE()

	template <typename TState>
	HFSM2_CONSTEXPR(14)	Status buildPlanStatus() noexcept;

#endif

public:
	using PlanControl::context;

	/// @brief Transition into a state
	///   (if transitioning into a region, acts depending on the region type)
	/// @param stateId State identifier
	HFSM2_CONSTEXPR(14)	void changeTo (const StateID stateId_)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void changeTo ()							noexcept	{ changeTo (PlanControl::template stateId<TState>());	}

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the initial state)
	/// @param stateId State identifier
	HFSM2_CONSTEXPR(14)	void restart  (const StateID stateId_)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the initial state)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void restart  ()							noexcept	{ restart  (PlanControl::template stateId<TState>());	}

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param stateId State identifier
	HFSM2_CONSTEXPR(14)	void resume   (const StateID stateId_)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void resume   ()							noexcept	{ resume   (PlanControl::template stateId<TState>());	}

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @param stateId State identifier
	HFSM2_CONSTEXPR(14)	void select   (const StateID stateId_)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void select   ()							noexcept	{ select   (PlanControl::template stateId<TState>());	}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId State identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void utilize  (const StateID stateId_)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState State type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void utilize  ()							noexcept	{ utilize  (PlanControl::template stateId<TState>());	}

	/// @brief Transition into a state
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId State identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void randomize(const StateID stateId_)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState State type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void randomize()							noexcept	{ randomize(PlanControl::template stateId<TState>());	}

#endif

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId State identifier
	HFSM2_CONSTEXPR(14)	void schedule (const StateID stateId_)		noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void schedule ()							noexcept	{ schedule (PlanControl::template stateId<TState>());	}

#if HFSM2_PLANS_AVAILABLE()

	/// @brief Succeed a plan task for the current state
	HFSM2_CONSTEXPR(14)	void succeed()								noexcept	{ succeed  (_originId							   );	}

	/// @brief Succeed a plan task for a state
	/// @param stateId State ID
	HFSM2_CONSTEXPR(14) void succeed(const StateID stateId_)		noexcept;

	/// @brief Succeed a plan task for a state
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14) void succeed()								noexcept	{ succeed  (PlanControl::template stateId<TState>());	}

	/// @brief Fail a plan task for the current state
	HFSM2_CONSTEXPR(14)	void fail	()								noexcept	{ fail	   (_originId							   );	}

	/// @brief Fail a plan task for a state
	/// @param stateId State ID
	HFSM2_CONSTEXPR(14) void fail	(const StateID stateId_)		noexcept;

	/// @brief Fail a plan task for a state
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14) void fail	()								noexcept	{ fail	   (PlanControl::template stateId<TState>());	}

#endif

protected:
	using PlanControl::_core;
	using PlanControl::_originId;
	using PlanControl::_regionId;

	using PlanControl::_regionStateId;
	using PlanControl::_regionSize;
	using PlanControl::_status;

	bool _locked = false;
};

template <typename TArgs>
class FullControlT;

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

#if HFSM2_PLANS_AVAILABLE()
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

	using FullControlBase::FullControlBase;

#if HFSM2_PLANS_AVAILABLE()

	template <typename TState>
	HFSM2_CONSTEXPR(14)	Status updatePlan(TState& headState,
										  const Status subStatus)		noexcept;

#endif

public:
	using FullControlBase::context;

	using FullControlBase::isActive;
	using FullControlBase::changeTo;

	HFSM2_IF_PLANS(using FullControlBase::plan);

	/// @brief Transition into a state
	///   (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void changeWith   (const StateID  stateId_,
										   const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void changeWith   (const Payload& payload)		noexcept	{ changeWith   (FullControlBase::template stateId<TState>(),	  payload );	}

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void restartWith  (const StateID  stateId_,
										   const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void restartWith  (const Payload& payload)		noexcept	{ restartWith  (FullControlBase::template stateId<TState>(),	  payload );	}

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void resumeWith   (const StateID  stateId_,
										   const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void resumeWith   (const Payload& payload)		noexcept	{ resumeWith   (FullControlBase::template stateId<TState>(),	  payload );	}

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void selectWith   (const StateID  stateId_,
										   const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void selectWith   (const Payload& payload)		noexcept	{ selectWith   (FullControlBase::template stateId<TState>(),	  payload );	}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void utilizeWith  (const StateID  stateId_,
										   const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void utilizeWith  (const Payload& payload)		noexcept	{ utilizeWith  (FullControlBase::template stateId<TState>(),	  payload );	}

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///   proportional to 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void randomizeWith(const StateID  stateId_,
										   const Payload& payload)		noexcept;

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///   proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void randomizeWith(const Payload& payload)		noexcept	{ randomizeWith(FullControlBase::template stateId<TState>(),	  payload );	}

#endif

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void scheduleWith (const StateID  stateId_,
										   const Payload& payload)		noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void scheduleWith (const Payload& payload)		noexcept	{ scheduleWith (FullControlBase::template stateId<TState>(),	  payload );	}

protected:
	using FullControlBase::_core;
	using FullControlBase::_originId;
	using FullControlBase::_regionId;

	using FullControlBase::_regionStateId;
	using FullControlBase::_regionSize;
	using FullControlBase::_status;

	using FullControlBase::_locked;
};

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

#if HFSM2_PLANS_AVAILABLE()
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

	using FullControlBase::FullControlBase;

#if HFSM2_PLANS_AVAILABLE()

	template <typename TState>
	HFSM2_CONSTEXPR(14)	Status updatePlan(TState& headState,
										  const Status subStatus)		noexcept;

#endif

public:
	using FullControlBase::isActive;
	using FullControlBase::changeTo;

	HFSM2_IF_PLANS(using FullControlBase::plan);

protected:
	using FullControlBase::_core;
	using FullControlBase::_regionId;

	using FullControlBase::_status;
};

template <typename TArgs>
class GuardControlT final
	: public FullControlT<TArgs>
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename>
	friend class R_;

	using FullControl	= FullControlT<TArgs>;

	using typename FullControl::Core;

	using typename FullControl::Context;

	using typename FullControl::Registry;
	using typename FullControl::TransitionSet;
	using typename FullControl::TransitionSets;

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	using typename FullControl::TransitionTargets;
#endif

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename FullControl::RNG;
#endif

#if HFSM2_PLANS_AVAILABLE()
	using typename FullControl::PlanData;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename FullControl::Logger;
#endif

	HFSM2_CONSTEXPR(11)	GuardControlT(Core& core,
									  const TransitionSets& currentTransitions,
									  const TransitionSet& pendingTransitions) noexcept
		: FullControl{core, currentTransitions}
		, _pendingTransitions{pendingTransitions}
	{}

public:
	using FullControl::context;

	/// @brief Check if a state is going to be activated or deactivated
	/// @param stateId State identifier
	/// @return State pending activation/deactivation status
	HFSM2_CONSTEXPR(11)	bool isPendingChange(const StateID stateId_)  const noexcept	{ return _core.registry.isPendingChange(stateId_);					}

	/// @brief Check if a state is going to be activated or deactivated
	/// @tparam TState State type
	/// @return State pending activation/deactivation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingChange()						  const noexcept	{ return isPendingChange(FullControl::template stateId<TState>());	}

	/// @brief Check if a state is going to be activated
	/// @param stateId State identifier
	/// @return State pending activation status
	HFSM2_CONSTEXPR(11)	bool isPendingEnter (const StateID stateId_)  const noexcept	{ return _core.registry.isPendingEnter (stateId_);					}

	/// @brief Check if a state is going to be activated
	/// @tparam TState State type
	/// @return State pending activation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingEnter ()						  const noexcept	{ return isPendingEnter (FullControl::template stateId<TState>());	}

	/// @brief Check if a state is going to be deactivated
	/// @param stateId State identifier
	/// @return State pending deactivation status
	HFSM2_CONSTEXPR(11)	bool isPendingExit	(const StateID stateId_)  const noexcept	{ return _core.registry.isPendingExit  (stateId_);					}

	/// @brief Check if a state is going to be deactivated
	/// @tparam TState State type
	/// @return State pending deactivation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingExit  ()						  const noexcept	{ return isPendingExit  (FullControl::template stateId<TState>());	}

	/// @brief Get pending transition requests
	/// @return Array of pending transition requests
	HFSM2_CONSTEXPR(11)	const TransitionSet&  pendingTransitions()	  const noexcept	{ return _pendingTransitions;										}

	/// @brief Cancel pending transition requests
	///   (can be used to substitute a transition into the current state with a different one)
	HFSM2_CONSTEXPR(14)	void cancelPendingTransitions()						noexcept;

private:
	using FullControl::_core;
	using FullControl::_originId;

	const TransitionSet& _pendingTransitions;
	bool _cancelled = false;
};

}
}


namespace hfsm2 {
namespace detail {

template <typename TArgs>
HFSM2_CONSTEXPR(14)
ConstControlT<TArgs>::Origin::Origin(ConstControlT& control_,
									 const StateID stateId_) noexcept
	: control{control_}
	, prevId{control._originId}
{
	HFSM2_ASSERT(stateId_ < StateList::SIZE);
	control._originId = stateId_;
}

template <typename TArgs>
HFSM2_CONSTEXPR(20)
ConstControlT<TArgs>::Origin::~Origin() noexcept {
	control._originId = prevId;
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
ConstControlT<TArgs>::Region::Region(ConstControlT& control_,
									 const RegionID regionId_) noexcept
	: control{control_}
	, prevId{control._regionId}
{
	control.setRegion(regionId_);
}

template <typename TArgs>
HFSM2_CONSTEXPR(20)
ConstControlT<TArgs>::Region::~Region() noexcept {
	control.resetRegion(prevId);
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
ControlT<TArgs>::Origin::Origin(ControlT& control_,
								const StateID stateId_) noexcept
	: control{control_}
	, prevId{control._originId}
{
	HFSM2_ASSERT(stateId_ < StateList::SIZE);
	control._originId = stateId_;
}

template <typename TArgs>
HFSM2_CONSTEXPR(20)
ControlT<TArgs>::Origin::~Origin() noexcept {
	control._originId = prevId;
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
ControlT<TArgs>::Region::Region(ControlT& control_,
								const RegionID regionId_) noexcept
	: control{control_}
	, prevId{control._regionId}
{
	control.setRegion(regionId_);
}

template <typename TArgs>
HFSM2_CONSTEXPR(20)
ControlT<TArgs>::Region::~Region() noexcept {
	control.resetRegion(prevId);
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
ControlT<TArgs>::setRegion(const RegionID regionId_) noexcept {
	HFSM2_ASSERT(_regionId <= regionId_ && regionId_ < RegionList::SIZE);

	_regionId = regionId_;
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
ControlT<TArgs>::resetRegion(const RegionID regionId_) noexcept { //-V524
	HFSM2_ASSERT(regionId_ <= _regionId && _regionId < RegionList::SIZE);

	_regionId = regionId_;
}

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
ControlT<TArgs>::pinLastTransition(const StateID stateId_,
								   const Short index) noexcept
{
	if (index != INVALID_SHORT) {
		HFSM2_ASSERT(index < TransitionSets::CAPACITY);

		if (!_core.registry.isActive(stateId_))
			_core.transitionTargets[stateId_] = index;
	}
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
const typename ControlT<TArgs>::Transition*
ControlT<TArgs>::lastTransitionTo(const StateID stateId_) const noexcept {
	if (HFSM2_CHECKED(stateId_ < StateList::SIZE)) {
		const Short index = _core.transitionTargets[stateId_];

		if (index < _core.previousTransitions.count())
			return &_core.previousTransitions[index];
	}

	return nullptr;
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
const typename ControlT<TArgs>::Transition*
ControlT<TArgs>::lastTransition() const noexcept {
	HFSM2_ASSERT(_originId < _core.transitionTargets.CAPACITY);

	return lastTransitionTo(_originId);
}

#endif

template <typename TArgs>
HFSM2_CONSTEXPR(14)
PlanControlT<TArgs>::Region::Region(PlanControlT& control_,
									const RegionID regionId_,
									const StateID index,
									const Long size) noexcept
	: control  {control_}
	, prevId   {control._regionId}
	, prevIndex{control._regionStateId}
	, prevSize {control._regionSize}
{
	control.setRegion(regionId_, index, size);
}

template <typename TArgs>
HFSM2_CONSTEXPR(20)
PlanControlT<TArgs>::Region::~Region() noexcept {
	control.resetRegion(prevId, prevIndex, prevSize);
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
PlanControlT<TArgs>::setRegion(const RegionID regionId_,
							   const StateID index,
							   const Long size) noexcept
{
	HFSM2_ASSERT(_regionId <= regionId_ && regionId_ <  RegionList::SIZE);
	HFSM2_ASSERT(_regionStateId <= index && index + size <= _regionStateId + _regionSize);

	_regionId	   = regionId_;
	_regionStateId = index;
	_regionSize	   = size;
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
PlanControlT<TArgs>::resetRegion(const RegionID regionId_, //-V524
								 const StateID index,
								 const Long size) noexcept
{
	HFSM2_ASSERT(regionId_ <= _regionId && _regionId < RegionList::SIZE);
	HFSM2_ASSERT(index <= _regionStateId && _regionStateId + _regionSize <= index + size);

	_regionId	   = regionId_;
	_regionStateId = index;
	_regionSize	   = size;

	_status.clear();
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
FullControlBaseT<TArgs>::Lock::Lock(FullControlBaseT& control_) noexcept
	: control{!control_._locked ? &control_ : nullptr}
{
	if (control)
		control->_locked = true;
}

template <typename TArgs>
HFSM2_CONSTEXPR(20)
FullControlBaseT<TArgs>::Lock::~Lock() noexcept {
	if (control)
		control->_locked = false;
}

#if HFSM2_PLANS_AVAILABLE()

template <typename TArgs>
template <typename TState>
HFSM2_CONSTEXPR(14)
Status
FullControlBaseT<TArgs>::buildPlanStatus() noexcept {
	constexpr StateID STATE_ID = TState::STATE_ID;

	switch (_status.result) {
	case Status::Result::NONE:
		HFSM2_BREAK();
		break;

	case Status::Result::SUCCESS:
		_core.planData.tasksSuccesses.template set<STATE_ID>();

		HFSM2_LOG_PLAN_STATUS(context(), _regionId, StatusEvent::SUCCEEDED);
		break;

	case Status::Result::FAILURE:
		_core.planData.tasksFailures .template set<STATE_ID>();

		HFSM2_LOG_PLAN_STATUS(context(), _regionId, StatusEvent::FAILED);
		break;

	default:
		HFSM2_BREAK();
	}

	return {_status.result};
}

#endif

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::changeTo(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::CHANGE});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::CHANGE, stateId_);
	}
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::restart(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::RESTART});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESTART, stateId_);
	}
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::resume(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::RESUME});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESUME, stateId_);
	}
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::select(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::SELECT});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SELECT, stateId_);
	}
}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::utilize(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::UTILIZE});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::UTILIZE, stateId_);
	}
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::randomize(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::RANDOMIZE});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RANDOMIZE, stateId_);
	}
}

#endif

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::schedule(const StateID stateId_) noexcept {
	_core.requests.emplace(Transition{_originId, stateId_, TransitionType::SCHEDULE});

	HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SCHEDULE, stateId_);
}

#if HFSM2_PLANS_AVAILABLE()

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::succeed(const StateID stateId_) noexcept {
	_status.result = Status::Result::SUCCESS;

	_core.planData.tasksSuccesses.set(stateId_);

	// TODO: promote taskSuccess all the way up for all regions without plans
	if (_regionId < RegionList::SIZE && !_core.planData.planExists.get(_regionId)) {
		HFSM2_ASSERT(_regionStateId < StateList::SIZE);

		_core.planData.tasksSuccesses.set(_regionStateId);
	}

	HFSM2_LOG_TASK_STATUS(context(), _regionId, stateId_, StatusEvent::SUCCEEDED);
}

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::fail(const StateID stateId_) noexcept {
	_status.result = Status::Result::FAILURE;

	_core.planData.tasksFailures.set(stateId_);

	// TODO: promote taskFailure all the way up for all regions without plans
	if (_regionId < RegionList::SIZE && !_core.planData.planExists.get(_regionId)) {
		HFSM2_ASSERT(_regionStateId < StateList::SIZE);

		_core.planData.tasksFailures.set(_regionStateId);
	}

	HFSM2_LOG_TASK_STATUS(context(), _regionId, stateId_, StatusEvent::FAILED);
}

#endif

#if HFSM2_PLANS_AVAILABLE()

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
template <typename TState>
HFSM2_CONSTEXPR(14)
Status
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::updatePlan(TState& headState,
																											   const Status subStatus) noexcept
{
	constexpr StateID STATE_ID = TState::STATE_ID;

	HFSM2_ASSERT(subStatus);

	if (subStatus.result == Status::Result::FAILURE) {
		_status.result = Status::Result::FAILURE;
		headState.wrapPlanFailed(*this);

		plan().clear();

		return FullControlBase::template buildPlanStatus<TState>();
	} else if (subStatus.result == Status::Result::SUCCESS) {
		if (Plan p = plan(_regionId)) {
			TasksBits processed;

			for (auto it = p.first(); it; ++it) {
				if (isActive(it->origin) &&
					_core.planData.tasksSuccesses.get(it->origin) &&
					!processed.get(it->origin))
				{
					Origin origin{*this, STATE_ID};

					if (const Payload* const payload = it->payload())
						changeWith(it->destination, *it->payload());
					else
						changeTo  (it->destination);

					_core.planData.tasksSuccesses.clear(it->origin);
					processed.set(it->origin);
					it.remove();
				}
			}

			return Status{};
		} else {
			_status.result = Status::Result::SUCCESS;
			headState.wrapPlanSucceeded(*this);

			plan().clear();

			return FullControlBase::template buildPlanStatus<TState>();
		}
	} else
		return Status{};
}

#endif

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::changeWith(const StateID  stateId_,
																																const Payload& payload) noexcept
{
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::CHANGE, payload});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::CHANGE, stateId_);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::restartWith(const StateID  stateId_,
																																 const Payload& payload) noexcept
{
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::RESTART, payload});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESTART, stateId_);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::resumeWith(const StateID  stateId_,
																																const Payload& payload) noexcept
{
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::RESUME, payload});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESUME, stateId_);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::selectWith(const StateID  stateId_,
																																const Payload& payload) noexcept
{
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::SELECT, payload});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SELECT, stateId_);
	}
}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::utilizeWith(const StateID  stateId_,
																																 const Payload& payload) noexcept
{
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::UTILIZE, payload});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::UTILIZE, stateId_);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::randomizeWith(const StateID  stateId_,
																																   const Payload& payload) noexcept
{
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::RANDOMIZE, payload});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RANDOMIZE, stateId_);
	}
}

#endif

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::scheduleWith(const StateID  stateId_,
																																  const Payload& payload) noexcept
{
	_core.requests.emplace(Transition{_originId, stateId_, TransitionType::SCHEDULE, payload});

	HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SCHEDULE, stateId_);
}

#if HFSM2_PLANS_AVAILABLE()

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
template <typename TState>
HFSM2_CONSTEXPR(14)
Status
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::updatePlan(TState& headState,
																												const Status subStatus) noexcept
{
	constexpr StateID STATE_ID = TState::STATE_ID;

	HFSM2_ASSERT(subStatus);

	if (subStatus.result == Status::Result::FAILURE) {
		_status.result = Status::Result::FAILURE;
		headState.wrapPlanFailed(*this);

		if (Plan p = plan(_regionId))
			p.clear();

		return FullControlBase::template buildPlanStatus<TState>();
	} else if (subStatus.result == Status::Result::SUCCESS) {
		if (Plan p = plan(_regionId)) {
			TasksBits processed;

			for (auto it = p.first(); it; ++it) {
				if (isActive(it->origin) &&
					_core.planData.tasksSuccesses.get(it->origin) &&
					!processed.get(it->origin))
				{
					Origin origin{*this, STATE_ID};
					changeTo(it->destination);

					_core.planData.tasksSuccesses.clear(it->origin);
					processed.set(it->origin);
					it.remove();
				}
			}

			return Status{};
		} else {
			_status.result = Status::Result::SUCCESS;
			headState.wrapPlanSucceeded(*this);

			return FullControlBase::template buildPlanStatus<TState>();
		}
	} else
		return Status{};
}

#endif

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
GuardControlT<TArgs>::cancelPendingTransitions() noexcept {
	_cancelled = true;

	HFSM2_LOG_CANCELLED_PENDING(context(), _originId);
}

}
}

namespace hfsm2 {
namespace detail {

template <typename TArgs>
class B_ {
	template <typename...>
	friend struct A_;

protected:
	using Context		= typename TArgs::Context;

	using Short			= ::hfsm2::Short;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank			= typename TArgs::Rank;
	using Utility		= typename TArgs::Utility;
#endif

	using StateList		= typename TArgs::StateList;
	using RegionList	= typename TArgs::RegionList;

	using ConstControl	= ConstControlT<TArgs>;
	using Control		= ControlT	   <TArgs>;
	using PlanControl	= PlanControlT <TArgs>;

#if HFSM2_PLANS_AVAILABLE()
	using Plan			= PlanT<TArgs>;
#endif

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

public:

	HFSM2_CONSTEXPR(14)	void	entryGuard	  (GuardControl&		)			noexcept	{}

	HFSM2_CONSTEXPR(14)	void	enter		  ( PlanControl&		)			noexcept	{}
	HFSM2_CONSTEXPR(14)	void	reenter		  ( PlanControl&		)			noexcept	{}

	HFSM2_CONSTEXPR(14)	void	preUpdate	  ( FullControl&		)			noexcept	{}
	HFSM2_CONSTEXPR(14)	void	update		  ( FullControl&		)			noexcept	{}
	HFSM2_CONSTEXPR(14)	void	postUpdate	  ( FullControl&		)			noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	preReact	  (const TEvent&		,
												FullControl&		)			noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	react		  (const TEvent&		,
												FullControl&		)			noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	postReact	  (const TEvent&		,
												FullControl&		)			noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	query		  (		 TEvent&		,
											   ConstControl&		)	  const noexcept	{}

	HFSM2_CONSTEXPR(14)	void	exitGuard	  (GuardControl&		)			noexcept	{}

	HFSM2_CONSTEXPR(14)	void	exit		  ( PlanControl&		)			noexcept	{}

	template <typename TState>
	static constexpr StateID  stateId()											noexcept	{ return					   index<StateList , TState>() ;	}

	template <typename TState>
	static constexpr RegionID regionId()										noexcept	{ return static_cast<RegionID>(index<RegionList, TState>());	}
};

template <typename...>
struct A_;

template <typename TFirst, typename... TRest>
struct HFSM2_EMPTY_BASES A_<TFirst, TRest...>
	: TFirst
	, A_<TRest...>
{
	using typename TFirst::Context;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename TFirst::Rank;
	using typename TFirst::Utility;
#endif

	using typename TFirst::StateList;
	using typename TFirst::RegionList;

	using typename TFirst::ConstControl;
	using typename TFirst::Control;
	using typename TFirst::PlanControl;

#if HFSM2_PLANS_AVAILABLE()
	using typename TFirst::Plan;
#endif

	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;
	using TFirst::regionId;

	HFSM2_CONSTEXPR(14)	void	wideEntryGuard(GuardControl& control)			noexcept;

	HFSM2_CONSTEXPR(14)	void	wideEnter	  ( PlanControl& control)			noexcept;
	HFSM2_CONSTEXPR(14)	void	wideReenter	  ( PlanControl& control)			noexcept;

	HFSM2_CONSTEXPR(14)	void	widePreUpdate ( FullControl& control)			noexcept;
	HFSM2_CONSTEXPR(14)	void	wideUpdate	  ( FullControl& control)			noexcept;
	HFSM2_CONSTEXPR(14)	void	widePostUpdate( FullControl& control)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	widePreReact  (const TEvent& event	,
												FullControl& control)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	wideReact	  (const TEvent& event	,
												FullControl& control)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	widePostReact (const TEvent& event	,
												FullControl& control)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	wideQuery	  (		 TEvent& event	,
											   ConstControl& control)	  const noexcept;

	HFSM2_CONSTEXPR(14)	void	wideExitGuard (GuardControl& control)			noexcept;

	HFSM2_CONSTEXPR(14)	void	wideExit	  ( PlanControl& control)			noexcept;

};

template <typename TFirst>
struct A_<TFirst>
	: TFirst
{
	using typename TFirst::Context;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename TFirst::Rank;
	using typename TFirst::Utility;
#endif

	using typename TFirst::StateList;
	using typename TFirst::RegionList;

	using typename TFirst::ConstControl;
	using typename TFirst::Control;
	using typename TFirst::PlanControl;

#if HFSM2_PLANS_AVAILABLE()
	using typename TFirst::Plan;
#endif

	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;
	using TFirst::regionId;

	HFSM2_CONSTEXPR(14)	Short	select		  (const Control&		)			noexcept	{ return 0;					}

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	Rank	rank		  (const Control&		)			noexcept	{ return Rank   {0};		}
	HFSM2_CONSTEXPR(14)	Utility utility		  (const Control&		)			noexcept	{ return Utility{1};		}
#endif

	HFSM2_CONSTEXPR(14)	void	entryGuard	  (GuardControl&		)			noexcept	{}

	HFSM2_CONSTEXPR(14)	void	enter		  ( PlanControl&		)			noexcept	{}
	HFSM2_CONSTEXPR(14)	void	reenter		  ( PlanControl&		)			noexcept	{}

	HFSM2_CONSTEXPR(14)	void	preUpdate	  ( FullControl&		)			noexcept	{}
	HFSM2_CONSTEXPR(14)	void	update		  ( FullControl&		)			noexcept	{}
	HFSM2_CONSTEXPR(14)	void	postUpdate	  ( FullControl&		)			noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	preReact	  (const TEvent&		,
											    FullControl&		)			noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	react		  (const TEvent&		,
											    FullControl&		)			noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	postReact	  (const TEvent&		,
											    FullControl&		)			noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	query		  (		 TEvent&		,
											   ConstControl&		)	  const noexcept	{}

	HFSM2_CONSTEXPR(14)	void	exitGuard	  (GuardControl&		)			noexcept	{}

	HFSM2_CONSTEXPR(14)	void	exit		  ( PlanControl&		)			noexcept	{}

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void	planSucceeded ( FullControl& control)			noexcept	{ control.succeed();		}
	HFSM2_CONSTEXPR(14)	void	planFailed	  ( FullControl& control)			noexcept	{ control.fail();			}
#endif

	HFSM2_CONSTEXPR(14)	void	wideEntryGuard(GuardControl& control)			noexcept;

	HFSM2_CONSTEXPR(14)	void	wideEnter	  ( PlanControl& control)			noexcept;
	HFSM2_CONSTEXPR(14)	void	wideReenter	  ( PlanControl& control)			noexcept;

	HFSM2_CONSTEXPR(14)	void	widePreUpdate ( FullControl& control)			noexcept;
	HFSM2_CONSTEXPR(14)	void	wideUpdate	  ( FullControl& control)			noexcept;
	HFSM2_CONSTEXPR(14)	void	widePostUpdate( FullControl& control)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	widePreReact  (const TEvent& event	,
												FullControl& control)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	wideReact	  (const TEvent& event	,
												FullControl& control)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	widePostReact (const TEvent& event	,
												FullControl& control)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	wideQuery	  (		 TEvent& event	,
											   ConstControl& control)	  const noexcept;

	HFSM2_CONSTEXPR(14)	void	wideExitGuard (GuardControl& control)			noexcept;

	HFSM2_CONSTEXPR(14)	void	wideExit	  ( PlanControl& control)			noexcept;

};

template <typename TArgs>
using EmptyT = A_<B_<TArgs>>;

}
}


namespace hfsm2 {
namespace detail {

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::wideEntryGuard(GuardControl& control) noexcept {
	TF		 ::	   entryGuard(				control);
	A_<TR...>::wideEntryGuard(				control);
}

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::wideEnter(PlanControl& control) noexcept {
	TF		 ::	   enter(			  control);
	A_<TR...>::wideEnter(			  control);
}

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::wideReenter(PlanControl& control) noexcept {
	TF		 ::	   Reenter(				control);
	A_<TR...>::wideReenter(				control);
}

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::widePreUpdate(FullControl& control) noexcept {
	TF		 ::	   preUpdate(			  control);
	A_<TR...>::widePreUpdate(			  control);
}

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::wideUpdate(FullControl& control) noexcept {
	TF		 ::	   update(			   control);
	A_<TR...>::wideUpdate(			   control);
}

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::widePostUpdate(FullControl& control) noexcept {
	A_<TR...>::widePostUpdate(			   control);
	TF		 ::	   postUpdate(			   control);
}

template <typename TF, typename... TR>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::widePreReact(const TEvent& event,
							FullControl& control) noexcept
{
	TF		 ::	   preReact(			  event, control);
	A_<TR...>::widePreReact(			  event, control);
}

template <typename TF, typename... TR>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::wideReact(const TEvent& event,
						 FullControl& control) noexcept
{
	TF		 ::	   react(			   event, control);
	A_<TR...>::wideReact(			   event, control);
}

template <typename TF, typename... TR>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::widePostReact(const TEvent& event,
							 FullControl& control) noexcept
{
	A_<TR...>::widePostReact(			   event, control);
	TF		 ::	   postReact(			   event, control);
}

template <typename TF, typename... TR>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::wideQuery(TEvent& event,
						 ConstControl& control) const noexcept
{
	A_<TR...>::wideQuery(		 event, control);
	TF		 ::	   query(		 event, control);
}

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::wideExitGuard(GuardControl& control) noexcept {
	A_<TR...>::wideExitGuard(			   control);
	TF		 ::	   exitGuard(			   control);
}

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::wideExit(PlanControl& control) noexcept {
	A_<TR...>::wideExit(			 control);
	TF		 ::	   exit(			 control);
}

}
}

namespace hfsm2 {
namespace detail {

template <typename TF>
HFSM2_CONSTEXPR(14)
void
A_<TF>::wideEntryGuard(GuardControl& control) noexcept {
	TF::	entryGuard(				 control);
}

template <typename TF>
HFSM2_CONSTEXPR(14)
void
A_<TF>::wideEnter(PlanControl& control) noexcept {
	TF::	enter(			   control);
}

template <typename TF>
HFSM2_CONSTEXPR(14)
void
A_<TF>::wideReenter(PlanControl& control) noexcept {
	TF::	reenter(			 control);
}

template <typename TF>
HFSM2_CONSTEXPR(14)
void
A_<TF>::widePreUpdate(FullControl& control) noexcept {
	TF::	preUpdate(			   control);
}

template <typename TF>
HFSM2_CONSTEXPR(14)
void
A_<TF>::wideUpdate(FullControl& control) noexcept {
	TF::	update(				control);
}

template <typename TF>
HFSM2_CONSTEXPR(14)
void
A_<TF>::widePostUpdate(FullControl& control) noexcept {
	TF::	postUpdate(				control);
}

template <typename TF>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF>::widePreReact(const TEvent& event,
					 FullControl& control) noexcept
{
	TF::	preReact(			   event, control);
}

template <typename TF>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF>::wideReact(const TEvent& event,
				  FullControl& control) noexcept
{
	TF::	react(				event, control);
}

template <typename TF>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF>::widePostReact(const TEvent& event,
					  FullControl& control) noexcept
{
	TF::	postReact(				event, control);
}

template <typename TF>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF>::wideQuery(TEvent& event,
				  ConstControl& control) const noexcept
{
	TF::	query(		  event, control);
}

template <typename TF>
HFSM2_CONSTEXPR(14)
void
A_<TF>::wideExitGuard(GuardControl& control) noexcept {
	TF::	exitGuard(				control);
}

template <typename TF>
HFSM2_CONSTEXPR(14)
void
A_<TF>::wideExit(PlanControl& control) noexcept {
	TF::	exit(			  control);
}

}
}

namespace hfsm2 {
namespace detail {

#if HFSM2_DEBUG_STATE_TYPE_AVAILABLE() || HFSM2_STRUCTURE_REPORT_AVAILABLE() || HFSM2_LOG_INTERFACE_AVAILABLE()

struct None final {};

#endif

template <typename TIndices,
		  typename TArgs,
		  typename THead>
struct S_
	: THead
{
	static constexpr StateID STATE_ID = TIndices::STATE_ID;

	using Context		= typename TArgs::Context;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank			= typename TArgs::Rank;
	using Utility		= typename TArgs::Utility;
	using UP			= typename TArgs::UP;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger		= typename TArgs::Logger;
#endif

	using ConstControl	= ConstControlT<TArgs>;
	using ScopedCOrigin	= typename ConstControl::Origin;

	using Control		= ControlT	   <TArgs>;
	using Registry		= RegistryT	   <TArgs>;
	using StateParents	= typename Registry::StateParents;

	using PlanControl	= PlanControlT <TArgs>;
	using ScopedOrigin	= typename PlanControl::Origin;

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

	using Head			= THead;

	HFSM2_CONSTEXPR(14)	Parent	stateParent			 (	   Control& control)								noexcept	{ return control._core.registry.stateParents[STATE_ID];	}

	HFSM2_CONSTEXPR(14)	void	deepRegister		 (	  Registry& registry, const Parent parent)			noexcept;

	HFSM2_CONSTEXPR(14)	Short	wrapSelect			 (	   Control& control)								noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	Rank	wrapRank			 (	   Control& control)								noexcept;
	HFSM2_CONSTEXPR(14)	Utility wrapUtility			 (	   Control& control)								noexcept;
#endif

	HFSM2_CONSTEXPR(14)	bool	deepForwardEntryGuard(GuardControl&		   )								noexcept	{ return false;		}
	HFSM2_CONSTEXPR(14)	bool	deepEntryGuard		 (GuardControl&	control)								noexcept;

	HFSM2_CONSTEXPR(14)	void	deepEnter			 ( PlanControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	void	deepReenter			 ( PlanControl& control)								noexcept;

	HFSM2_CONSTEXPR(14)	Status	deepPreUpdate		 ( FullControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	Status	deepUpdate			 ( FullControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	Status	deepPostUpdate		 ( FullControl& control)								noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	deepPreReact		 ( FullControl& control, const TEvent&	event)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	deepReact			 ( FullControl& control, const TEvent&	event)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	deepPostReact		 ( FullControl& control, const TEvent&	event)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	deepQuery			 (ConstControl& control,	   TEvent&  event)	  const noexcept;

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14)	Status	deepUpdatePlans		 ( FullControl& control)								noexcept;
#endif

	HFSM2_CONSTEXPR(14)	bool	deepForwardExitGuard (GuardControl&		   )								noexcept	{ return false;		}
	HFSM2_CONSTEXPR(14)	bool	deepExitGuard		 (GuardControl&	control)								noexcept;

	HFSM2_CONSTEXPR(14)	void	deepExit			 ( PlanControl& control)								noexcept;

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void	wrapPlanSucceeded	 ( FullControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	void	wrapPlanFailed		 ( FullControl& control)								noexcept;
#endif

	HFSM2_CONSTEXPR(14)	void	deepForwardActive	 (	   Control&,		 const Request		  )			noexcept	{}
	HFSM2_CONSTEXPR(14)	void	deepForwardRequest	 (	   Control& control, const Request request)			noexcept;

	HFSM2_CONSTEXPR(14)	void	deepRequestChange	 (	   Control& control, const Request request)			noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestRestart	 (	   Control& control, const Request request)			noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestResume	 (	   Control& control, const Request request)			noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestSelect	 (	   Control& control, const Request request)			noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void	deepRequestUtilize	 (	   Control& control, const Request request)			noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestRandomize (	   Control& control, const Request request)			noexcept;

	HFSM2_CONSTEXPR(14)	UP		deepReportChange	 (	   Control& control)								noexcept;
	HFSM2_CONSTEXPR(14)	UP		deepReportUtilize	 (	   Control& control)								noexcept;
	HFSM2_CONSTEXPR(14)	Rank	deepReportRank		 (	   Control& control)								noexcept;
	HFSM2_CONSTEXPR(14)	Utility deepReportRandomize  (	   Control& control)								noexcept;
#endif

	HFSM2_CONSTEXPR(14)	void	deepChangeToRequested(	   Control&)										noexcept	{}

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename TArgs::WriteStream;
	using ReadStream	= typename TArgs::ReadStream;

	HFSM2_CONSTEXPR(14)	void	deepSaveActive		 (const Registry&, WriteStream&)				  const noexcept	{}
	HFSM2_CONSTEXPR(14)	void	deepSaveResumable	 (const Registry&, WriteStream&)				  const noexcept	{}

	HFSM2_CONSTEXPR(14)	void	deepLoadRequested	 (		Registry&, ReadStream& )				  const noexcept	{}
	HFSM2_CONSTEXPR(14)	void	deepLoadResumable	 (		Registry&, ReadStream& )				  const noexcept	{}
#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename TArgs::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static
	HFSM2_CONSTEXPR(NO)	const char* name() noexcept;

	HFSM2_CONSTEXPR(14)	void deepGetNames(const Long parent,
										  const RegionType region,
										  const Short depth,
										  StructureStateInfos& stateInfos)							  const noexcept;
#endif

#if HFSM2_DEBUG_STATE_TYPE_AVAILABLE() || HFSM2_STRUCTURE_REPORT_AVAILABLE() || HFSM2_LOG_INTERFACE_AVAILABLE()

	using Empty			= EmptyT<TArgs>;

	static HFSM2_CONSTEXPR(11)	bool isBare()																noexcept	{ return std::is_base_of<Head, Empty>::value;	}

	HFSM2_IF_TYPEINDEX(const std::type_index TYPE = isBare() ? typeid(None) : typeid(Head));

	static constexpr Long NAME_COUNT = isBare() ? 0 : 1;

#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()

	template <typename TReturn, typename THost, typename... TParams>
	HFSM2_CONSTEXPR(14)
	void log(TReturn (THost::*)(TParams...),
			 Logger& logger,
			 const Context& context,
			 const Method method)																	  const noexcept
	{
		logger.recordMethod(context, STATE_ID, method);
	}

	template <typename TReturn, typename THost, typename... TParams>
	HFSM2_CONSTEXPR(14)
	void log(TReturn (THost::*)(TParams...) const,
			 Logger& logger,
			 const Context& context,
			 const Method method)																	  const noexcept
	{
		logger.recordMethod(context, STATE_ID, method);
	}

	template <typename TReturn, typename... TParams>
	HFSM2_CONSTEXPR(14)
	void log(TReturn (Empty::*)(TParams...),
			 Logger&,
			 const Context&,
			 const Method)																			  const noexcept
	{}

	template <typename TReturn, typename... TParams>
	HFSM2_CONSTEXPR(14)
	void log(TReturn (Empty::*)(TParams...) const,
			 Logger&,
			 const Context&,
			 const Method)																			  const noexcept
	{}

#endif

};

}
}


namespace hfsm2 {
namespace detail {

template <StateID NS, typename TA, typename TH>
struct RegisterT final {
	using StateParents	= StaticArrayT<Parent, TA::STATE_COUNT>;
	using StateList		= typename TA::StateList;

	static constexpr StateID STATE_ID = NS;

	static
	HFSM2_CONSTEXPR(14)
	void
	execute(StateParents& stateParents,
			const Parent parent) noexcept
	{
		constexpr StateID HEAD_ID = index<StateList, TH>();
		static_assert(STATE_ID == HEAD_ID, "");

		stateParents[STATE_ID] = parent;
	}
};

template <StateID NS, typename TA>
struct RegisterT<NS, TA, EmptyT<TA>> final {
	using StateParents = StaticArrayT<Parent, TA::STATE_COUNT>;

	static
	HFSM2_CONSTEXPR(14)
	void
	execute(StateParents&, const Parent) noexcept {}
};

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepRegister(Registry& registry,
							  const Parent parent) noexcept
{
	using Register = RegisterT<STATE_ID, TA, Head>;
	Register::execute(registry.stateParents, parent);
}

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
Short
S_<TN_, TA, TH>::wrapSelect(Control& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::select,
						   Method::SELECT);

	return Head::select(static_cast<const Control&>(control));
}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA, TH>::Rank
S_<TN_, TA, TH>::wrapRank(Control& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::rank,
						   Method::RANK);

	return Head::rank(static_cast<const Control&>(control));
}

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA, TH>::Utility
S_<TN_, TA, TH>::wrapUtility(Control& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::utility,
						   Method::UTILITY);

	return Head::utility(static_cast<const Control&>(control));
}

#endif

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
bool
S_<TN_, TA, TH>::deepEntryGuard(GuardControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::entryGuard,
						   Method::ENTRY_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	Head::wideEntryGuard(control);
	Head::	  entryGuard(control);

	return !cancelledBefore && control._cancelled;
}

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepEnter(PlanControl& control) noexcept {
	HFSM2_IF_PLANS(control._core.planData.verifyEmptyStatus(STATE_ID));

	HFSM2_LOG_STATE_METHOD(&Head::enter,
						   Method::ENTER);

	ScopedOrigin origin{control, STATE_ID};

	Head::wideEnter(control);
	Head::	  enter(control);
}

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepReenter(PlanControl& control) noexcept {
	HFSM2_IF_PLANS(control._core.planData.verifyEmptyStatus(STATE_ID));

	HFSM2_LOG_STATE_METHOD(&Head::reenter,
						   Method::REENTER);

	ScopedOrigin origin{control, STATE_ID};

	Head::wideReenter(control);
	Head::	  reenter(control);
}

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
Status
S_<TN_, TA, TH>::deepPreUpdate(FullControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::preUpdate,
						   Method::PRE_UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	Head::widePreUpdate(control);
	Head::	  preUpdate(control);

	return control._status;
}

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
Status
S_<TN_, TA, TH>::deepUpdate(FullControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::update,
						   Method::UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	Head::wideUpdate(control);
	Head::	  update(control);

	return control._status;
}

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
Status
S_<TN_, TA, TH>::deepPostUpdate(FullControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::postUpdate,
						   Method::POST_UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	Head::	  postUpdate(control);
	Head::widePostUpdate(control);

	return control._status;
}

template <typename TN_, typename TA, typename TH>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
S_<TN_, TA, TH>::deepPreReact(FullControl& control,
							  const TEvent& event) noexcept
{
	auto method = static_cast<void (Head::*)(const TEvent&, FullControl&)>(&Head::preReact);

	HFSM2_LOG_STATE_METHOD(method,
						   Method::PRE_REACT);

	ScopedOrigin origin{control, STATE_ID};

	Head::widePreReact(event, control);
	(this->*method) (event, control);

	return control._status;
}

template <typename TN_, typename TA, typename TH>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
S_<TN_, TA, TH>::deepReact(FullControl& control,
						   const TEvent& event) noexcept
{
	auto method = static_cast<void (Head::*)(const TEvent&, FullControl&)>(&Head::react);

	HFSM2_LOG_STATE_METHOD(method,
						   Method::REACT);

	ScopedOrigin origin{control, STATE_ID};

	Head::wideReact(event, control);
	(this->*method)(event, control);

	return control._status;
}

template <typename TN_, typename TA, typename TH>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
S_<TN_, TA, TH>::deepPostReact(FullControl& control,
							   const TEvent& event) noexcept
{
	auto method = static_cast<void (Head::*)(const TEvent&, FullControl&)>(&Head::postReact);

	HFSM2_LOG_STATE_METHOD(method,
						   Method::POST_REACT);

	ScopedOrigin origin{control, STATE_ID};

	(this->*method)	   (event, control);
	Head::widePostReact(event, control);

	return control._status;
}

template <typename TN_, typename TA, typename TH>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepQuery(ConstControl& control,
						   TEvent&  event) const noexcept
{
	auto method = static_cast<void (Head::*)(TEvent&, ConstControl&) const>(&Head::query);

	HFSM2_LOG_STATE_METHOD(method,
						   Method::QUERY);

	ScopedCOrigin origin{control, STATE_ID};

	(this->*method)(event, control);
	Head::wideQuery(event, control);
}

#if HFSM2_PLANS_AVAILABLE()

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
Status
S_<TN_, TA, TH>::deepUpdatePlans(FullControl& control) noexcept {
	if (control._core.planData.tasksFailures .get(STATE_ID))
		return Status{Status::Result::FAILURE};
	else
	if (control._core.planData.tasksSuccesses.get(STATE_ID))
		return Status{Status::Result::SUCCESS};
	else
		return Status{};
}

#endif

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
bool
S_<TN_, TA, TH>::deepExitGuard(GuardControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::exitGuard,
						   Method::EXIT_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	Head::wideExitGuard(control);
	Head::	  exitGuard(control);

	return !cancelledBefore && control._cancelled;
}

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
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
	Head::	  exit(control);
	Head::wideExit(control);

	HFSM2_IF_PLANS(control._core.planData.clearTaskStatus(STATE_ID));
}

#if HFSM2_PLANS_AVAILABLE()

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::wrapPlanSucceeded(FullControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::planSucceeded,
						   Method::PLAN_SUCCEEDED);

	ScopedOrigin origin{control, STATE_ID};

	Head::planSucceeded(control);
}

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::wrapPlanFailed(FullControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::planFailed,
						   Method::PLAN_FAILED);

	ScopedOrigin origin{control, STATE_ID};

	Head::planFailed(control);
}

#endif

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepForwardRequest(Control& HFSM2_IF_TRANSITION_HISTORY(control),
									const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepRequestChange(Control& HFSM2_IF_TRANSITION_HISTORY(control),
								   const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepRequestRestart(Control& HFSM2_IF_TRANSITION_HISTORY(control),
									const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepRequestResume(Control& HFSM2_IF_TRANSITION_HISTORY(control),
								   const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepRequestSelect(Control& HFSM2_IF_TRANSITION_HISTORY(control),
								   const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepRequestUtilize(Control& HFSM2_IF_TRANSITION_HISTORY(control),
									const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepRequestRandomize(Control& HFSM2_IF_TRANSITION_HISTORY(control),
									  const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA, TH>::UP
S_<TN_, TA, TH>::deepReportChange(Control& control) noexcept {
	const Utility utility = wrapUtility(control);

	const Parent parent = stateParent(control);

	return {utility, parent.prong};
}

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA, TH>::UP
S_<TN_, TA, TH>::deepReportUtilize(Control& control) noexcept {
	const Utility utility = wrapUtility(control);
	const Parent  parent  = stateParent(control);

	return {utility, parent.prong};
}

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA, TH>::Rank
S_<TN_, TA, TH>::deepReportRank(Control& control) noexcept {
	return wrapRank(control);
}

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA, TH>::Utility
S_<TN_, TA, TH>::deepReportRandomize(Control& control) noexcept {
	return wrapUtility(control);
}

#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(NO)
const char*
S_<TN_, TA, TH>::name() noexcept {
	if (isBare())
		return "";
	else {
		const std::type_index type = typeid(Head);
		const char* const raw = type.name();

	#if defined(_MSC_VER)

		Short first =
			raw[0] == 's' ? 7 : // Struct
			raw[0] == 'c' ? 6 : // Class
							0;

		for (Short c = first; raw[c]; ++c)
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

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepGetNames(const Long parent,
							  const RegionType region,
							  const Short depth,
							  StructureStateInfos& stateInfos) const noexcept
{
	stateInfos.emplace(StructureStateInfo{parent, region, depth, name()});
}

#endif

}
}

namespace hfsm2 {
namespace detail {

template <typename>
struct SI_;

template <Strategy, typename, typename...>
struct CI_;

template <typename>
struct CSI_;

template <typename, typename...>
struct OI_;

template <typename...>
struct OSI_;

template <typename TI, typename... TR>
struct OSI_<TI, TR...>;

template <typename TI>
struct OSI_<TI>;

template <typename...>
struct WrapInfoT;

template <typename TH>
struct WrapInfoT<	 TH> final {
	using Type = SI_<TH>;
};

template <Strategy SG, typename TH, typename... TS>
struct WrapInfoT<CI_<SG, TH, TS...>> final {
	using Type = CI_<SG, TH, TS...>;
};

template <typename... TS>
struct WrapInfoT<OI_<TS...>> final {
	using Type = OI_<TS...>;
};

template <typename... TS>
using WrapInfo = typename WrapInfoT<TS...>::Type;

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

#if HFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  ACTIVE_BITS	  = 0;
	static constexpr Long  RESUMABLE_BITS = 0;
#endif

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
	static constexpr Short REGION_COUNT	  = RegionList::SIZE;
};

template <typename TI, typename... TR>
struct CSI_<TL_<TI, TR...>> final {
	using Initial			= WrapInfo<TI>;
	using Remaining			= CSI_<TL_<TR...>>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;
	using RegionList		= Merge<typename Initial::RegionList, typename Remaining::RegionList>;

	static constexpr Long  REVERSE_DEPTH  = max(Initial::REVERSE_DEPTH,	  Remaining::REVERSE_DEPTH);
	static constexpr Short COMPO_REGIONS  =		Initial::COMPO_REGIONS  + Remaining::COMPO_REGIONS;
	static constexpr Long  COMPO_PRONGS	  =		Initial::COMPO_PRONGS   + Remaining::COMPO_PRONGS;
	static constexpr Short ORTHO_REGIONS  =		Initial::ORTHO_REGIONS  + Remaining::ORTHO_REGIONS;
	static constexpr Short ORTHO_UNITS	  =		Initial::ORTHO_UNITS    + Remaining::ORTHO_UNITS;

#if HFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  ACTIVE_BITS	  = max(Initial::ACTIVE_BITS,	  Remaining::ACTIVE_BITS);
	static constexpr Long  RESUMABLE_BITS =		Initial::RESUMABLE_BITS + Remaining::RESUMABLE_BITS;
#endif

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
	static constexpr Short REGION_COUNT	  = RegionList::SIZE;
};

template <typename TI>
struct CSI_<TL_<TI>> final {
	using Initial			= WrapInfo<TI>;
	using StateList			= typename Initial::StateList;
	using RegionList		= typename Initial::RegionList;

	static constexpr Long  REVERSE_DEPTH  = Initial::REVERSE_DEPTH;
	static constexpr Short COMPO_REGIONS  = Initial::COMPO_REGIONS;
	static constexpr Long  COMPO_PRONGS	  = Initial::COMPO_PRONGS;
	static constexpr Short ORTHO_REGIONS  = Initial::ORTHO_REGIONS;
	static constexpr Short ORTHO_UNITS	  = Initial::ORTHO_UNITS;

#if HFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  ACTIVE_BITS	  = Initial::ACTIVE_BITS;
	static constexpr Long  RESUMABLE_BITS = Initial::RESUMABLE_BITS;
#endif

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
	static constexpr Short REGION_COUNT	  = RegionList::SIZE;
};

template <Strategy TStrategy, typename THead, typename... TSubStates>
struct CI_ final {
	static constexpr Strategy STRATEGY = TStrategy;

	using Head				= THead;
	using HeadInfo			= SI_<Head>;
	using SubStates			= CSI_<TL_<TSubStates...>>;
	using StateList			= Merge<typename HeadInfo::StateList, typename SubStates::StateList>;
	using RegionList		= Merge<typename HeadInfo::StateList, typename SubStates::RegionList>;

	static constexpr Short WIDTH		  = sizeof...(TSubStates);
	static constexpr Long  REVERSE_DEPTH  = SubStates::REVERSE_DEPTH + 1;
	static constexpr Short COMPO_REGIONS  = SubStates::COMPO_REGIONS + 1;
	static constexpr Long  COMPO_PRONGS	  = SubStates::COMPO_PRONGS + WIDTH;
	static constexpr Short ORTHO_REGIONS  = SubStates::ORTHO_REGIONS;
	static constexpr Short ORTHO_UNITS	  = SubStates::ORTHO_UNITS;

#if HFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  WIDTH_BITS	  = bitContain(WIDTH);
	static constexpr Long  ACTIVE_BITS	  = SubStates::ACTIVE_BITS	+ WIDTH_BITS;
	static constexpr Long  RESUMABLE_BITS = SubStates::RESUMABLE_BITS + WIDTH_BITS + 1;
#endif

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
	static constexpr Short REGION_COUNT	  = RegionList::SIZE;
};

template <typename TI, typename... TR>
struct OSI_<TI, TR...> final {
	using Initial			= WrapInfo<TI>;
	using Remaining			= OSI_<TR...>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;
	using RegionList		= Merge<typename Initial::RegionList, typename Remaining::RegionList>;

	static constexpr Long  REVERSE_DEPTH  = max(Initial::REVERSE_DEPTH,	  Remaining::REVERSE_DEPTH);
	static constexpr Short COMPO_REGIONS  =		Initial::COMPO_REGIONS  + Remaining::COMPO_REGIONS;
	static constexpr Long  COMPO_PRONGS	  =		Initial::COMPO_PRONGS   + Remaining::COMPO_PRONGS;
	static constexpr Short ORTHO_REGIONS  =		Initial::ORTHO_REGIONS  + Remaining::ORTHO_REGIONS;
	static constexpr Short ORTHO_UNITS	  =		Initial::ORTHO_UNITS    + Remaining::ORTHO_UNITS;

#if HFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  ACTIVE_BITS	  =		Initial::ACTIVE_BITS    + Remaining::ACTIVE_BITS;
	static constexpr Long  RESUMABLE_BITS =		Initial::RESUMABLE_BITS + Remaining::RESUMABLE_BITS;
#endif
};

template <typename TI>
struct OSI_<TI> final {
	using Initial			= WrapInfo<TI>;
	using StateList			= typename Initial::StateList;
	using RegionList		= typename Initial::RegionList;

	static constexpr Long  REVERSE_DEPTH	= Initial::REVERSE_DEPTH;
	static constexpr Short COMPO_REGIONS	= Initial::COMPO_REGIONS;
	static constexpr Long  COMPO_PRONGS		= Initial::COMPO_PRONGS;
	static constexpr Short ORTHO_REGIONS	= Initial::ORTHO_REGIONS;
	static constexpr Short ORTHO_UNITS		= Initial::ORTHO_UNITS;

#if HFSM2_SERIALIZATION_AVAILABLE()
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
	static constexpr Short ORTHO_UNITS		= SubStates::ORTHO_UNITS + contain(WIDTH, 8);

#if HFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  ACTIVE_BITS		= SubStates::ACTIVE_BITS;
	static constexpr Long  RESUMABLE_BITS	= SubStates::RESUMABLE_BITS;
#endif

	static constexpr Long  STATE_COUNT		= StateList::SIZE;
	static constexpr Short REGION_COUNT		= RegionList::SIZE;
};

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
	using PureContext	= Undecorate<Context>;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank			= typename TConfig::Rank;
	using Utility		= typename TConfig::Utility;
	using RNG			= typename TConfig::RNG;
	using UP			= typename TConfig::UP;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger		= typename TConfig::LoggerInterface;
#endif

	using StateList		= TStateList;
	using RegionList	= TRegionList;

	static constexpr Long  STATE_COUNT		  = StateList::SIZE;
	static constexpr Short REGION_COUNT		  = RegionList::SIZE;
	static constexpr Short COMPO_REGIONS	  = NCompoCount;
	static constexpr Short ORTHO_REGIONS	  = NOrthoCount;
	static constexpr Short ORTHO_UNITS		  = NOrthoUnits;

#if HFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Short SERIAL_BITS		  = NSerialBits;
#endif

	static constexpr Short SUBSTITUTION_LIMIT = NSubstitutionLimit;

#if HFSM2_PLANS_AVAILABLE()
	static constexpr Long  TASK_CAPACITY	  = NTaskCapacity;
#endif

	using Payload		= TPayload;

#if HFSM2_SERIALIZATION_AVAILABLE()
	using SerialBuffer	= StreamBufferT  <SERIAL_BITS>;
	using WriteStream	= BitWriteStreamT<SERIAL_BITS>;
	using ReadStream	= BitReadStreamT <SERIAL_BITS>;
#endif

	HFSM2_IF_STRUCTURE_REPORT(using StructureStateInfos = ArrayT<StructureStateInfo, STATE_COUNT>);
};

template <StateID NStateID,
		  Short NCompoIndex,
		  Short NOrthoIndex,
		  Short NOrthoUnit>
struct I_ final {
	static constexpr StateID STATE_ID	 = NStateID;
	static constexpr Short	 COMPO_INDEX = NCompoIndex;
	static constexpr Short	 ORTHO_INDEX = NOrthoIndex;
	static constexpr Short	 ORTHO_UNIT	 = NOrthoUnit;
};

template <typename, typename, typename>
struct S_;

template <typename, typename, Strategy, typename, typename...>
struct C_;

template <typename, typename, Strategy, Short, typename>
struct CS_;

template <typename, typename, typename, typename...>
struct O_;

template <typename, typename, Short, typename...>
struct OS_;

template <typename, typename>
class InstanceT;

template <typename, typename...>
struct MaterialT_;

template <typename TN, typename TA, typename TH>
struct MaterialT_  <TN, TA, TH> final {
	using Type = S_<TN, TA, TH>;
};

template <typename TN, typename TA, Strategy SG, 			  typename... TS>
struct MaterialT_  <TN, TA, CI_<SG, void,       TS...>> final {
	using Type = C_<TN, TA,     SG, EmptyT<TA>, TS...>;
};

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
struct MaterialT_  <TN, TA, CI_<SG, TH,	TS...>> final {
	using Type = C_<TN, TA,     SG, TH,	TS...>;
};

template <typename TN, typename TA,				 typename... TS>
struct MaterialT_  <TN, TA, OI_<void,       TS...>> final {
	using Type = O_<TN, TA,     EmptyT<TA>, TS...>;
};

template <typename TN, typename TA, typename TH, typename... TS>
struct MaterialT_  <TN, TA, OI_<TH,			TS...>> final {
	using Type = O_<TN, TA,     TH,			TS...>;
};

template <typename TN, typename... TS>
using MaterialT = typename MaterialT_<TN, TS...>::Type;

template <typename TConfig,
		  typename TApex>
struct RF_ final {
	using Context		= typename TConfig::Context;
	using Apex			= TApex;

	using StateList		= typename Apex::StateList;
	using RegionList	= typename Apex::RegionList;

	static constexpr Long  STATE_COUNT			= Apex::STATE_COUNT;
	static constexpr Short REGION_COUNT			= Apex::REGION_COUNT;

	static constexpr Long  SUBSTITUTION_LIMIT	= TConfig::SUBSTITUTION_LIMIT;

#if HFSM2_PLANS_AVAILABLE()
	static constexpr Long  TASK_CAPACITY		= TConfig::TASK_CAPACITY != INVALID_LONG ?
													  TConfig::TASK_CAPACITY : Apex::COMPO_PRONGS * 2;
#endif

	using Payload	= typename TConfig::Payload;

	static constexpr Short COMPO_REGIONS		= Apex::COMPO_REGIONS;
	static constexpr Short ORTHO_REGIONS		= Apex::ORTHO_REGIONS;
	static constexpr Short ORTHO_UNITS			= Apex::ORTHO_UNITS;

#if HFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  ACTIVE_BITS			= Apex::ACTIVE_BITS;
	static constexpr Long  RESUMABLE_BITS		= Apex::RESUMABLE_BITS;
	static constexpr Long  SERIAL_BITS			= 1 + ACTIVE_BITS + RESUMABLE_BITS;
#endif

	using Args			= ArgsT<Context
							  , TConfig
							  , StateList
							  , RegionList
							  , COMPO_REGIONS
							  , ORTHO_REGIONS
							  , ORTHO_UNITS
							  HFSM2_IF_SERIALIZATION(, SERIAL_BITS)
							  , SUBSTITUTION_LIMIT
							  HFSM2_IF_PLANS(, TASK_CAPACITY)
							  , Payload>;

	using Instance		= InstanceT<TConfig, Apex>;

	using ConstControl	= ConstControlT<Args>;
	using Control		= ControlT	   <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using State			= EmptyT<Args>;

	template <typename... TInjections>
	using StateT		= A_<TInjections...>;

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger		= typename TConfig::LoggerInterface;
#endif

	template <typename TState>
	static constexpr bool	  contains()		noexcept	{ return					contains<StateList , TState>() ;	}

	template <typename TState>
	static constexpr StateID  stateId()			noexcept	{ return					   index<StateList , TState>() ;	}

	template <typename TState>
	static constexpr RegionID regionId()		noexcept	{ return static_cast<RegionID>(index<RegionList, TState>());	}
};

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
struct CSubMaterialT;

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
struct CSubMaterialT<TN, TA, SG, NI, TL_<TS...>> final {
	using Type = CS_<TN, TA, SG, NI, TL_<TS...>>;
};

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
using CSubMaterial = typename CSubMaterialT<TN, TA, SG, NI, TS...>::Type;

template <typename>
struct InfoT;

template <typename TN, typename TA, typename TH>
struct InfoT<S_<TN, TA, TH>> final {
	using Type = SI_<	TH>;
};

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
struct InfoT<C_<TN, TA, SG, TH, TS...>> final {
	using Type = CI_<	SG, TH, TS...>;
};

template <typename TN, typename TA, typename TH, typename... TS>
struct InfoT<O_<TN, TA, TH, TS...>> final {
	using Type = OI_<	TH, TS...>;
};

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
struct InfoT<CS_<TN, TA, SG, NI, TL_<TS...>>> final {
	using Type = CSI_<			 TL_<TS...>>;
};

template <typename T>
using Info = typename InfoT<T>::Type;

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
struct LHalfCST;

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
struct LHalfCST<TN, TA, SG, NI, TL_<TS...>> final {
	using Type = CS_<TN,
					 TA,
					 SG,
					 NI,
					 LHalfTypes<TS...>>;
};

template <typename TN, typename TA, Strategy SG, Short NI, typename TL>
using LHalfCS = typename LHalfCST<TN, TA, SG, NI, TL>::Type;

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
struct RHalfCST;

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
struct RHalfCST<TN, TA, SG, NI, TL_<TS...>> final {
	using Indices		= TN;
	static constexpr StateID  INITIAL_ID  = Indices::STATE_ID;
	static constexpr Short	  COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	  ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	  ORTHO_UNIT  = Indices::ORTHO_UNIT;

	using LStateList	= LHalfTypes<TS...>;

	using LHalfInfo		= CSI_<LStateList>;

	using Type			= CS_<I_<INITIAL_ID  + LHalfInfo::STATE_COUNT,
								 COMPO_INDEX + LHalfInfo::COMPO_REGIONS,
								 ORTHO_INDEX + LHalfInfo::ORTHO_REGIONS,
								 ORTHO_UNIT  + LHalfInfo::ORTHO_UNITS>,
							  TA,
							  SG,
							  NI + LStateList::SIZE,
							  RHalfTypes<TS...>>;
};

template <typename TN, typename TA, Strategy SG, Short NI, typename TL>
using RHalfCS = typename RHalfCST<TN, TA, SG, NI, TL>::Type;

template <typename TN, typename TA, typename TI>
using InitialOS = MaterialT<TN, TA, TI>;

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
struct RemainingOST final {
	using Indices		= TN;
	static constexpr StateID INITIAL_ID	 = Indices::STATE_ID;
	static constexpr Short	 COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	 ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	 ORTHO_UNIT	 = Indices::ORTHO_UNIT;

	using InitialInfo	= WrapInfo<TI>;

	using Type			= OS_<I_<INITIAL_ID  + InitialInfo::STATE_COUNT,
								 COMPO_INDEX + InitialInfo::COMPO_REGIONS,
								 ORTHO_INDEX + InitialInfo::ORTHO_REGIONS,
								 ORTHO_UNIT  + InitialInfo::ORTHO_UNITS>,
							  TA,
							  NI + 1,
							  TR...>;
};

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
using RemainingOS = typename RemainingOST<TN, TA, NI, TI, TR...>::Type;

}
}

namespace hfsm2 {
namespace detail {

template <typename TIndices,
		  typename TArgs,
		  Strategy TStrategy,
		  Short NIndex,
		  typename... TStates>
struct HFSM2_EMPTY_BASES CS_<TIndices, TArgs, TStrategy, NIndex, TL_<TStates...>>
	: LHalfCS<TIndices,
			  TArgs,
			  TStrategy,
			  NIndex,
			  TL_<TStates...>>
	, RHalfCS<TIndices,
			  TArgs,
			  TStrategy,
			  NIndex,
			  TL_<TStates...>>
{
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
	using SubStateList	= TL_<TStates...>;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;

	using ConstControl	= ConstControlT<Args>;
	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using LStateList	= LHalfTypes<TStates...>;
	using LHalfInfo		= CSI_<LStateList>;

	static constexpr Short	  L_PRONG	  = PRONG_INDEX;
	static constexpr Short	  R_PRONG	  = PRONG_INDEX + LStateList::SIZE;

	using LHalf			= LHalfCS<Indices,
								  Args,
								  STRATEGY,
								  PRONG_INDEX,
								  SubStateList>;

	using RHalf			= RHalfCS<Indices,
								  Args,
								  STRATEGY,
								  PRONG_INDEX,
								  SubStateList>;

	HFSM2_CONSTEXPR(14)	void	wideRegister			   (Registry& registry, const Parent parent)								noexcept;

	HFSM2_CONSTEXPR(14)	bool	wideForwardEntryGuard	   (GuardControl& control,						  const Short prong)		noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideEntryGuard			   (GuardControl& control,						  const Short prong)		noexcept;

	HFSM2_CONSTEXPR(14)	void	wideEnter				   ( PlanControl& control,						  const Short prong)		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideReenter				   ( PlanControl& control,						  const Short prong)		noexcept;

	HFSM2_CONSTEXPR(14)	Status	widePreUpdate			   ( FullControl& control,						  const Short prong)		noexcept;
	HFSM2_CONSTEXPR(14)	Status	wideUpdate				   ( FullControl& control,						  const Short prong)		noexcept;
	HFSM2_CONSTEXPR(14)	Status	widePostUpdate			   ( FullControl& control,						  const Short prong)		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	widePreReact			   ( FullControl& control, const TEvent& event,	  const Short prong)		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	wideReact				   ( FullControl& control, const TEvent& event,	  const Short prong)		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	widePostReact			   ( FullControl& control, const TEvent& event,	  const Short prong)		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	wideQuery				   (ConstControl& control,		 TEvent& event,	  const Short prong)  const noexcept;

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14)	Status	wideUpdatePlans			   ( FullControl& control,						  const Short prong)		noexcept;
#endif

	HFSM2_CONSTEXPR(14)	bool	wideForwardExitGuard	   (GuardControl& control,						  const Short prong)		noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideExitGuard			   (GuardControl& control,						  const Short prong)		noexcept;

	HFSM2_CONSTEXPR(14)	void	wideExit				   ( PlanControl& control,						  const Short prong)		noexcept;

	HFSM2_CONSTEXPR(14)	void	wideForwardActive		   (	 Control& control, const Request request, const Short prong)		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideForwardRequest		   (	 Control& control, const Request request, const Short prong)		noexcept;

	HFSM2_CONSTEXPR(14)	void	wideRequestChangeComposite (	 Control& control, const Request request				   )		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestChangeResumable (	 Control& control, const Request request, const Short prong)		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestChangeSelectable(	 Control& control, const Request request, const Short prong)		noexcept;

	HFSM2_CONSTEXPR(14)	void	wideRequestRestart		   (	 Control& control, const Request request				   )		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestResume		   (	 Control& control, const Request request, const Short prong)		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestSelect		   (	 Control& control, const Request request, const Short prong)		noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	UP		wideReportChangeComposite  (	 Control& control																  )		noexcept;
	HFSM2_CONSTEXPR(14)	UP		wideReportChangeResumable  (	 Control& control,						  const Short prong						  )		noexcept;
	HFSM2_CONSTEXPR(14)	UP		wideReportChangeSelectable (	 Control& control,						  const Short prong						  )		noexcept;
	HFSM2_CONSTEXPR(14)	UP		wideReportChangeUtilitarian(	 Control& control																  )		noexcept;
	HFSM2_CONSTEXPR(14)	Utility wideReportChangeRandom	   (	 Control& control, Utility* const options, const Rank* const ranks, const Rank top)		noexcept;

	HFSM2_CONSTEXPR(14)	UP		wideReportUtilize		   (	 Control& control																  )		noexcept;
	HFSM2_CONSTEXPR(14)	Rank	wideReportRank			   (	 Control& control,								 Rank* const ranks				  )		noexcept;
	HFSM2_CONSTEXPR(14)	Utility wideReportRandomize		   (	 Control& control, Utility* const options, const Rank* const ranks, const Rank top)		noexcept;
#endif

	HFSM2_CONSTEXPR(14)	void	wideChangeToRequested	   ( PlanControl& control,						  const Short prong)		noexcept;

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_CONSTEXPR(14)	void	wideSaveActive			   (const Registry& registry, WriteStream& stream, const Short prong)  const noexcept;
	HFSM2_CONSTEXPR(14)	void	wideSaveResumable		   (const Registry& registry, WriteStream& stream	 				)  const noexcept;

	HFSM2_CONSTEXPR(14)	void	wideLoadRequested		   (      Registry& registry, ReadStream&  stream, const Short prong)  const noexcept;
	HFSM2_CONSTEXPR(14)	void	wideLoadResumable		   (      Registry& registry, ReadStream&  stream	 				)  const noexcept;
#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr Long NAME_COUNT = LHalf::NAME_COUNT + RHalf::NAME_COUNT;

	HFSM2_CONSTEXPR(14)	void wideGetNames(const Long parent,
										  const RegionType region,
										  const Short depth,
										  StructureStateInfos& stateInfos)	  const noexcept;
#endif

};

template <typename TIndices,
		  typename TArgs,
		  Strategy TStrategy,
		  Short NIndex,
		  typename TState>
struct CS_<TIndices,
		   TArgs,
		   TStrategy,
		   NIndex,
		   TL_<TState>>
	: MaterialT<TIndices,
				TArgs,
				TState>
{
	using Indices		= TIndices;
	static constexpr StateID INITIAL_ID	= Indices::STATE_ID;
	static constexpr Short COMPO_INDEX	= Indices::COMPO_INDEX;
	static constexpr Short ORTHO_INDEX	= Indices::ORTHO_INDEX;
	static constexpr Short ORTHO_UNIT	= Indices::ORTHO_UNIT;

	static constexpr Strategy	STRATEGY	= TStrategy;

	static constexpr Short REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr Short PRONG_INDEX	= NIndex;

	using Args			= TArgs;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;

	using ConstControl	= ConstControlT<Args>;
	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Single		= MaterialT<Indices,
									Args,
									TState>;

	HFSM2_CONSTEXPR(14)	void	wideRegister			   (Registry& registry, const Parent parent)								noexcept;

	HFSM2_CONSTEXPR(14)	bool	wideForwardEntryGuard	   (GuardControl& control,						  const Short prong)		noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideEntryGuard			   (GuardControl& control,						  const Short prong)		noexcept;

	HFSM2_CONSTEXPR(14)	void	wideEnter				   ( PlanControl& control,						  const Short prong)		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideReenter				   ( PlanControl& control,						  const Short prong)		noexcept;

	HFSM2_CONSTEXPR(14)	Status	widePreUpdate			   ( FullControl& control,						  const Short prong)		noexcept;
	HFSM2_CONSTEXPR(14)	Status	wideUpdate				   ( FullControl& control,						  const Short prong)		noexcept;
	HFSM2_CONSTEXPR(14)	Status	widePostUpdate			   ( FullControl& control,						  const Short prong)		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	widePreReact			   ( FullControl& control, const TEvent& event,	  const Short prong)		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	wideReact				   ( FullControl& control, const TEvent& event,	  const Short prong)		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	widePostReact			   ( FullControl& control, const TEvent& event,	  const Short prong)		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	wideQuery				   (ConstControl& control,		 TEvent& event,	  const Short prong)  const noexcept;

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14)	Status	wideUpdatePlans			   (FullControl	& control,						  const Short prong)		noexcept;
#endif

	HFSM2_CONSTEXPR(14)	bool	wideForwardExitGuard	   (GuardControl& control,						  const Short prong)		noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideExitGuard			   (GuardControl& control,						  const Short prong)		noexcept;

	HFSM2_CONSTEXPR(14)	void	wideExit				   ( PlanControl& control,						  const Short prong)		noexcept;

	HFSM2_CONSTEXPR(14)	void	wideForwardActive		   (	 Control& control, const Request request, const Short prong)		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideForwardRequest		   (	 Control& control, const Request request, const Short prong)		noexcept;

	HFSM2_CONSTEXPR(14)	void	wideRequestChangeComposite (	 Control& control, const Request request				   )		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestChangeResumable (	 Control& control, const Request request, const Short prong)		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestChangeSelectable(	 Control& control, const Request request, const Short prong)		noexcept;

	HFSM2_CONSTEXPR(14)	void	wideRequestRestart		   (	 Control& control, const Request request				   )		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestResume		   (	 Control& control, const Request request, const Short prong)		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestSelect		   (	 Control& control, const Request request, const Short prong)		noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	UP		wideReportChangeComposite  (	 Control& control																  )		noexcept;
	HFSM2_CONSTEXPR(14)	UP		wideReportChangeResumable  (	 Control& control						, const Short prong						  )		noexcept;
	HFSM2_CONSTEXPR(14)	UP		wideReportChangeSelectable (	 Control& control						, const Short prong						  )		noexcept;
	HFSM2_CONSTEXPR(14)	UP		wideReportChangeUtilitarian(	 Control& control																  )		noexcept;
	HFSM2_CONSTEXPR(14)	Utility wideReportChangeRandom	   (	 Control& control, Utility* const options, const Rank* const ranks, const Rank top)		noexcept;

	HFSM2_CONSTEXPR(14)	UP		wideReportUtilize		   (	 Control& control																  )		noexcept;
	HFSM2_CONSTEXPR(14)	Rank	wideReportRank			   (	 Control& control,								 Rank* const ranks				  )		noexcept;
	HFSM2_CONSTEXPR(14)	Utility wideReportRandomize		   (	 Control& control, Utility* const options, const Rank* const ranks, const Rank top)		noexcept;
#endif

	HFSM2_CONSTEXPR(14)	void	wideChangeToRequested	   ( PlanControl& control, const Short prong)								noexcept;

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_CONSTEXPR(14)	void	wideSaveActive			   (const Registry& registry, WriteStream& stream, const Short prong)  const noexcept;
	HFSM2_CONSTEXPR(14)	void	wideSaveResumable		   (const Registry& registry, WriteStream& stream	 				)  const noexcept;

	HFSM2_CONSTEXPR(14)	void	wideLoadRequested		   (      Registry& registry, ReadStream&  stream, const Short prong)  const noexcept;
	HFSM2_CONSTEXPR(14)	void	wideLoadResumable		   (      Registry& registry, ReadStream&  stream	 				)  const noexcept;
#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr Long NAME_COUNT = Single::NAME_COUNT;

	HFSM2_CONSTEXPR(14)	void wideGetNames(const Long parent,
										  const RegionType region,
										  const Short depth,
										  StructureStateInfos& stateInfos)	  const noexcept;
#endif

};

}
}


namespace hfsm2 {
namespace detail {

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideRegister(Registry& registry,
											  const Parent parent) noexcept
{
	LHalf::wideRegister(registry, Parent{parent.forkId, L_PRONG});
	RHalf::wideRegister(registry, Parent{parent.forkId, R_PRONG});
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, TL_<TS...>>::wideForwardEntryGuard(GuardControl& control,
													   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return LHalf::wideForwardEntryGuard(control, prong);
	else
		return RHalf::wideForwardEntryGuard(control, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, TL_<TS...>>::wideEntryGuard(GuardControl& control,
												const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return LHalf::wideEntryGuard(control, prong);
	else
		return RHalf::wideEntryGuard(control, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideEnter(PlanControl& control,
										   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideEnter(control, prong);
	else
		RHalf::wideEnter(control, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReenter(PlanControl& control,
											 const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideReenter(control, prong);
	else
		RHalf::wideReenter(control, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<TS...>>::widePreUpdate(FullControl& control,
											   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::widePreUpdate(control, prong) :
		RHalf::widePreUpdate(control, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<TS...>>::wideUpdate(FullControl& control,
											const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::wideUpdate(control, prong) :
		RHalf::wideUpdate(control, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<TS...>>::widePostUpdate(FullControl& control,
												const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::widePostUpdate(control, prong) :
		RHalf::widePostUpdate(control, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<TS...>>::widePreReact(FullControl& control,
											  const TEvent& event,
											  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::widePreReact(control, event, prong) :
		RHalf::widePreReact(control, event, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReact(FullControl& control,
										   const TEvent& event,
										   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::wideReact(control, event, prong) :
		RHalf::wideReact(control, event, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<TS...>>::widePostReact(FullControl& control,
											   const TEvent& event,
											   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::widePostReact(control, event, prong) :
		RHalf::widePostReact(control, event, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideQuery(ConstControl& control,
										   TEvent& event,
										   const Short prong) const noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::wideQuery(control, event, prong) :
		RHalf::wideQuery(control, event, prong);
}

#if HFSM2_PLANS_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<TS...>>::wideUpdatePlans(FullControl& control,
												 const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::wideUpdatePlans(control, prong) :
		RHalf::wideUpdatePlans(control, prong);
}

#endif

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, TL_<TS...>>::wideForwardExitGuard(GuardControl& control,
													  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return LHalf::wideForwardExitGuard(control, prong);
	else
		return RHalf::wideForwardExitGuard(control, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, TL_<TS...>>::wideExitGuard(GuardControl& control,
											   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return LHalf::wideExitGuard(control, prong);
	else
		return RHalf::wideExitGuard(control, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideExit(PlanControl& control,
										  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideExit(control, prong);
	else
		RHalf::wideExit(control, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideForwardActive(Control& control,
												   const Request request,
												   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideForwardActive(control, request, prong);
	else
		RHalf::wideForwardActive(control, request, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideForwardRequest(Control& control,
													const Request request,
													const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideForwardRequest(control, request, prong);
	else
		RHalf::wideForwardRequest(control, request, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideRequestChangeComposite(Control& control,
															const Request request) noexcept
{
	LHalf::wideRequestChangeComposite(control, request);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideRequestChangeResumable(Control& control,
															const Request request,
															const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideRequestChangeResumable(control, request, prong);
	else
		RHalf::wideRequestChangeResumable(control, request, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideRequestChangeSelectable(Control& control,
															 const Request request,
															 const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideRequestChangeSelectable(control, request, prong);
	else
		RHalf::wideRequestChangeSelectable(control, request, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideRequestRestart(Control& control,
													const Request request) noexcept
{
	LHalf::wideRequestRestart(control, request);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideRequestResume(Control& control,
												   const Request request,
												   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideRequestResume(control, request, prong);
	else
		RHalf::wideRequestResume(control, request, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideRequestSelect(Control& control,
												   const Request request,
												   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideRequestSelect(control, request, prong);
	else
		RHalf::wideRequestSelect(control, request, prong);
}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReportUtilize(Control& control) noexcept {
	const UP l = LHalf::wideReportUtilize(control);
	const UP r = RHalf::wideReportUtilize(control);

	return l.utility >= r.utility ?
		l : r;
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::Rank
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReportRank(Control& control,
												Rank* const ranks) noexcept
{
	HFSM2_ASSERT(ranks);

	const Rank l = LHalf::wideReportRank(control, ranks);
	const Rank r = RHalf::wideReportRank(control, ranks + LStateList::SIZE);

	return l >= r?
		l : r;
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::Utility
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReportRandomize(Control& control,
													 Utility* const options,
													 const Rank* const ranks,
													 const Rank top) noexcept
{
	HFSM2_ASSERT(options && ranks);

	const Utility l = LHalf::wideReportRandomize(control, options,					  ranks,					top);
	const Utility r = RHalf::wideReportRandomize(control, options + LStateList::SIZE, ranks + LStateList::SIZE, top);

	return { l + r };
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReportChangeComposite(Control& control) noexcept {
	return LHalf::wideReportChangeComposite(control);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReportChangeResumable(Control& control,
														   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return LHalf::wideReportChangeResumable(control, prong);
	else
		return RHalf::wideReportChangeResumable(control, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReportChangeSelectable(Control& control,
															const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return LHalf::wideReportChangeSelectable(control, prong);
	else
		return RHalf::wideReportChangeSelectable(control, prong);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReportChangeUtilitarian(Control& control) noexcept {
	const UP l = LHalf::wideReportChangeUtilitarian(control);
	const UP r = RHalf::wideReportChangeUtilitarian(control);

	return l.utility >= r.utility ?
		l : r;
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::Utility
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReportChangeRandom(Control& control,
														Utility* const options,
														const Rank* const ranks,
														const Rank top) noexcept
{
	HFSM2_ASSERT(options && ranks);

	const Utility l = LHalf::wideReportChangeRandom(control, options,					 ranks,					   top);
	const Utility r = RHalf::wideReportChangeRandom(control, options + LStateList::SIZE, ranks + LStateList::SIZE, top);

	return { l + r };
}

#endif

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideChangeToRequested(PlanControl& control,
													   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideChangeToRequested(control, prong);
	else
		RHalf::wideChangeToRequested(control, prong);
}

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideSaveActive(const Registry& registry,
												WriteStream& stream,
												const Short prong) const noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG) {
		LHalf::wideSaveActive   (registry, stream, prong);
		RHalf::wideSaveResumable(registry, stream);
	} else {
		LHalf::wideSaveResumable(registry, stream);
		RHalf::wideSaveActive   (registry, stream, prong);
	}
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideSaveResumable(const Registry& registry,
												   WriteStream& stream) const noexcept
{
	LHalf::wideSaveResumable(registry, stream);
	RHalf::wideSaveResumable(registry, stream);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideLoadRequested(Registry& registry,
												   ReadStream& stream,
												   const Short prong) const noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG) {
		LHalf::wideLoadRequested(registry, stream, prong);
		RHalf::wideLoadResumable(registry, stream);
	} else {
		LHalf::wideLoadResumable(registry, stream);
		RHalf::wideLoadRequested(registry, stream, prong);
	}
}

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideLoadResumable(Registry& registry,
												   ReadStream& stream) const noexcept
{
	LHalf::wideLoadResumable(registry, stream);
	RHalf::wideLoadResumable(registry, stream);
}

#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideGetNames(const Long parent,
											  const RegionType /*region*/,
											  const Short depth,
											  StructureStateInfos& stateInfos) const noexcept
{
	LHalf::wideGetNames(parent, StructureStateInfo::RegionType::COMPOSITE, depth, stateInfos);
	RHalf::wideGetNames(parent, StructureStateInfo::RegionType::COMPOSITE, depth, stateInfos);
}

#endif

}
}

namespace hfsm2 {
namespace detail {

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideRegister(Registry& registry,
										  const Parent parent) noexcept
{
	Single::deepRegister(registry, Parent{parent.forkId, PRONG_INDEX});
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, TL_<T>>::wideForwardEntryGuard(GuardControl& control,
												   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepForwardEntryGuard(control);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, TL_<T>>::wideEntryGuard(GuardControl& control,
											const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepEntryGuard(control);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideEnter(PlanControl& control,
									   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepEnter(control);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideReenter(PlanControl& control,
										 const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepReenter(control);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<T>>::widePreUpdate(FullControl& control,
										   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPreUpdate(control);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<T>>::wideUpdate(FullControl& control,
										const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepUpdate(control);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<T>>::widePostUpdate(FullControl& control,
											const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPostUpdate(control);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<T>>::widePreReact(FullControl& control,
										  const TEvent& event,
										  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPreReact(control, event);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<T>>::wideReact(FullControl& control,
									   const TEvent& event,
									   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepReact(control, event);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<T>>::widePostReact(FullControl& control,
										   const TEvent& event,
										   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPostReact(control, event);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideQuery(ConstControl& control,
									   TEvent& event,
									   const Short HFSM2_IF_ASSERT(prong)) const noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepQuery(control, event);
}

#if HFSM2_PLANS_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<T>>::wideUpdatePlans(FullControl& control,
											 const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepUpdatePlans(control);
}

#endif

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, TL_<T>>::wideForwardExitGuard(GuardControl& control,
												  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepForwardExitGuard(control);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, TL_<T>>::wideExitGuard(GuardControl& control,
										   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepExitGuard(control);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideExit(PlanControl& control,
									  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepExit(control);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideForwardActive(Control& control,
											   const Request request,
											   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepForwardActive(control, request);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideForwardRequest(Control& control,
												const Request request,
												const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepForwardRequest(control, request);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideRequestChangeComposite(Control& control,
														const Request request) noexcept
{
	Single::deepRequestChange(control, request);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideRequestChangeResumable(Control& control,
														const Request request,
														const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepRequestChange(control, request);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideRequestChangeSelectable(Control& control,
														 const Request request,
														 const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepRequestChange(control, request);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideRequestRestart(Control& control,
												const Request request) noexcept
{
	Single::deepRequestRestart(control, request);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideRequestResume(Control& control,
											   const Request request,
											   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepRequestResume(control, request);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideRequestSelect(Control& control,
											   const Request request,
											   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepRequestSelect(control, request);
}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<T>>::wideReportUtilize(Control& control) noexcept {
	return Single::deepReportUtilize(control);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::Rank
CS_<TN, TA, SG, NI, TL_<T>>::wideReportRank(Control& control,
											Rank* const ranks) noexcept
{
	HFSM2_ASSERT(ranks);

	*ranks = Single::deepReportRank(control);

	return *ranks;
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::Utility
CS_<TN, TA, SG, NI, TL_<T>>::wideReportRandomize(Control& control,
												 Utility* const options,
												 const Rank* const ranks,
												 const Rank top) noexcept
{
	HFSM2_ASSERT(options && ranks);

	*options = (*ranks == top) ?
		Single::deepReportRandomize(control) : Utility{0};

	return *options;
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<T>>::wideReportChangeComposite(Control& control) noexcept {
	return Single::deepReportChange(control);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<T>>::wideReportChangeResumable(Control& control,
													   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepReportChange(control);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<T>>::wideReportChangeSelectable(Control& control,
														const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepReportChange(control);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<T>>::wideReportChangeUtilitarian(Control& control) noexcept {
	return Single::deepReportChange(control);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::Utility
CS_<TN, TA, SG, NI, TL_<T>>::wideReportChangeRandom(Control& control,
													Utility* const options,
													const Rank* const ranks,
													const Rank top) noexcept
{
	HFSM2_ASSERT(options && ranks);

	*options = (*ranks == top) ?
		Single::deepReportChange(control).utility : Utility{0};

	return *options;
}

#endif

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideChangeToRequested(PlanControl& control,
												   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepChangeToRequested(control);
}

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideSaveActive(const Registry& registry,
											WriteStream& stream,
											const Short HFSM2_IF_ASSERT(prong)) const noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepSaveActive(registry, stream);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideSaveResumable(const Registry& registry,
											   WriteStream& stream) const noexcept
{
	Single::deepSaveResumable(registry, stream);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideLoadRequested(Registry& registry,
											   ReadStream& stream,
											   const Short HFSM2_IF_ASSERT(prong)) const noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepLoadRequested(registry, stream);
}

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideLoadResumable(Registry& registry,
											   ReadStream& stream) const noexcept
{
	Single::deepLoadResumable(registry, stream);
}

#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideGetNames(const Long parent,
										  const RegionType /*region*/,
										  const Short depth,
										  StructureStateInfos& stateInfos) const noexcept
{
	Single::deepGetNames(parent, StructureStateInfo::RegionType::COMPOSITE, depth, stateInfos);
}

#endif

}
}

namespace hfsm2 {
namespace detail {

template <typename TIndices,
		  typename TArgs,
		  Strategy TStrategy,
		  typename THead,
		  typename... TSubStates>
struct HFSM2_EMPTY_BASES C_
	: S_<TIndices, TArgs, THead>
	, CS_<I_<TIndices::STATE_ID + 1,
			 TIndices::COMPO_INDEX + 1,
			 TIndices::ORTHO_INDEX,
			 TIndices::ORTHO_UNIT>,
		  TArgs,
		  TStrategy,
		  0,
		  TL_<TSubStates...>>
{
	using Indices		= TIndices;
	static constexpr StateID  HEAD_ID	  = Indices::STATE_ID;
	static constexpr Short	  COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	  ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	  ORTHO_UNIT  = Indices::ORTHO_UNIT;

	static constexpr Strategy STRATEGY	  = TStrategy;

	static constexpr Short	  REGION_ID	  = COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID	  COMPO_ID	  = COMPO_INDEX + 1;

	using Args			= TArgs;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;

	using ConstControl	= ConstControlT<Args>;
	using Control		= ControlT	   <Args>;
	using ScopedOrigin	= typename Control::Origin;

	using PlanControl	= PlanControlT <Args>;
	using ScopedRegion	= typename PlanControl::Region;

#if HFSM2_PLANS_AVAILABLE()
	using Plan			= typename PlanControl::Plan;
#endif

	using FullControl	= FullControlT <Args>;
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
							  TL_<TSubStates...>>;

	using Info			= CI_<STRATEGY, Head, TSubStates...>;

	static constexpr Short WIDTH		  = Info::WIDTH;

#if HFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Short WIDTH_BITS	  = Info::WIDTH_BITS;
#endif

	static constexpr Short REGION_SIZE	  = Info::STATE_COUNT;

#if HFSM2_SERIALIZATION_AVAILABLE()
	HFSM2_CONSTEXPR(11)	static Short	compoRequested		  (const Registry& registry)	noexcept	{ return registry.compoRequested[COMPO_INDEX];	}
	HFSM2_CONSTEXPR(11)	static Short&	compoRequested		  (		 Registry& registry)	noexcept	{ return registry.compoRequested[COMPO_INDEX];	}

	HFSM2_CONSTEXPR(11)	static Short	compoActive			  (const Registry& registry)	noexcept	{ return registry.compoActive	[COMPO_INDEX];	}
	HFSM2_CONSTEXPR(11)	static Short&	compoActive			  (		 Registry& registry)	noexcept	{ return registry.compoActive	[COMPO_INDEX];	}

	HFSM2_CONSTEXPR(11)	static Short	compoResumable		  (const Registry& registry)	noexcept	{ return registry.compoResumable[COMPO_INDEX];	}
	HFSM2_CONSTEXPR(11)	static Short&	compoResumable		  (		 Registry& registry)	noexcept	{ return registry.compoResumable[COMPO_INDEX];	}
#endif

	HFSM2_CONSTEXPR(11)	static Short&	compoRequested		  (		  Control& control )	noexcept	{ return control._core.registry.compoRequested[COMPO_INDEX];	}
	HFSM2_CONSTEXPR(11)	static Short&	compoActive			  (		  Control& control )	noexcept	{ return control._core.registry.compoActive	  [COMPO_INDEX];	}
	HFSM2_CONSTEXPR(11)	static Short	compoActive			  (	 ConstControl& control )	noexcept	{ return control._core.registry.compoActive	  [COMPO_INDEX];	}
	HFSM2_CONSTEXPR(11)	static Short&	compoResumable		  (		  Control& control )	noexcept	{ return control._core.registry.compoResumable[COMPO_INDEX];	}

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(11)	static Status&	headStatus			  (		  Control& control )	noexcept	{ return control._core.planData.headStatuses  [REGION_ID];		}
	HFSM2_CONSTEXPR(11)	static Status&	subStatus			  (		  Control& control )	noexcept	{ return control._core.planData.subStatuses   [REGION_ID];		}
#endif

	HFSM2_CONSTEXPR(11)	static bool		compoRemain			  (		  Control& control )	noexcept	{ return control._core.registry.compoRemains.template get<COMPO_INDEX>(); }

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	Short	resolveRandom				  (		  Control& control,
															   const Utility(& options)[WIDTH], const Utility sum,
															   const Rank	(& ranks)  [WIDTH], const Rank	  top)  const noexcept;
#endif

	HFSM2_CONSTEXPR(14)	void	deepRegister				  (Registry& registry, const Parent parent)				noexcept;

	HFSM2_CONSTEXPR(14)	bool	deepForwardEntryGuard		  (GuardControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	bool	deepEntryGuard				  (GuardControl& control)								noexcept;

	HFSM2_CONSTEXPR(14)	void	deepEnter					  ( PlanControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	void	deepReenter					  ( PlanControl& control)								noexcept;

	HFSM2_CONSTEXPR(14)	Status	deepPreUpdate				  ( FullControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	Status	deepUpdate					  ( FullControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	Status	deepPostUpdate				  ( FullControl& control)								noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	deepPreReact				  ( FullControl& control, const TEvent& event)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	deepReact					  ( FullControl& control, const TEvent& event)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	deepPostReact				  ( FullControl& control, const TEvent& event)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	deepQuery					  (ConstControl& control,		TEvent& event)	  const noexcept;

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14)	Status	deepUpdatePlans				  ( FullControl& control)								noexcept;
#endif

	HFSM2_CONSTEXPR(14)	bool	deepForwardExitGuard		  (GuardControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	bool	deepExitGuard				  (GuardControl& control)								noexcept;

	HFSM2_CONSTEXPR(14)	void	deepExit					  ( PlanControl& control)								noexcept;

	HFSM2_CONSTEXPR(14)	void	deepForwardActive			  (		Control& control, const Request request)		noexcept;
	HFSM2_CONSTEXPR(14)	void	deepForwardRequest			  (		Control& control, const Request request)		noexcept;

	HFSM2_CONSTEXPR(14)	void	deepRequest					  (		Control& control, const Request request)		noexcept;

#if HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()

	template <Strategy = STRATEGY>
	HFSM2_CONSTEXPR(14)	void	deepRequestChange			  (		Control& control, const Request request, const Short = INVALID_SHORT)	noexcept;

	template <>
	HFSM2_CONSTEXPR(14)	void	deepRequestChange<Composite>  (		Control& control, const Request request, const Short)	noexcept	{ deepRequestChangeComposite  (control, request); }

	template <>
	HFSM2_CONSTEXPR(14)	void	deepRequestChange<Resumable>  (		Control& control, const Request request, const Short)	noexcept	{ deepRequestChangeResumable  (control, request); }

	template <>
	HFSM2_CONSTEXPR(14)	void	deepRequestChange<Selectable> (		Control& control, const Request request, const Short)	noexcept	{ deepRequestChangeSelectable (control, request); }

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	template <>
	HFSM2_CONSTEXPR(14)	void	deepRequestChange<Utilitarian>(		Control& control, const Request request, const Short)	noexcept	{ deepRequestChangeUtilitarian(control, request); }

	template <>
	HFSM2_CONSTEXPR(14)	void	deepRequestChange<RandomUtil> (		Control& control, const Request request, const Short)	noexcept	{ deepRequestChangeRandom	  (control, request); }
#endif

#else

	HFSM2_CONSTEXPR(14)	void	deepRequestChange			  (		Control& control, const Request request)	noexcept;

#endif

	HFSM2_CONSTEXPR(14)	void	deepRequestChangeComposite	  (		Control& control, const Request request)	noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestChangeResumable	  (		Control& control, const Request request)	noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestChangeSelectable	  (		Control& control, const Request request)	noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void	deepRequestChangeUtilitarian  (		Control& control, const Request request)	noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestChangeRandom		  (		Control& control, const Request request)	noexcept;
#endif

	HFSM2_CONSTEXPR(14)	void	deepRequestRestart			  (		Control& control, const Request request)	noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestResume			  (		Control& control, const Request request)	noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestSelect			  (		Control& control, const Request request)	noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void	deepRequestUtilize			  (		Control& control, const Request request)	noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestRandomize		  (		Control& control, const Request request)	noexcept;
#endif

#if HFSM2_UTILITY_THEORY_AVAILABLE()
#if HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()

	template <Strategy = STRATEGY>
	HFSM2_CONSTEXPR(14)	UP		deepReportChange			  (		Control& control)	noexcept;

	template <>
	HFSM2_CONSTEXPR(14)	UP		deepReportChange<Composite>   (		Control& control)	noexcept	{ return deepReportChangeComposite  (control); }

	template <>
	HFSM2_CONSTEXPR(14)	UP		deepReportChange<Resumable>   (		Control& control)	noexcept	{ return deepReportChangeResumable  (control); }

	template <>
	HFSM2_CONSTEXPR(14)	UP		deepReportChange<Selectable>  (		Control& control)	noexcept	{ return deepReportChangeSelectable (control); }

	template <>
	HFSM2_CONSTEXPR(14)	UP		deepReportChange<Utilitarian> (		Control& control)	noexcept	{ return deepReportChangeUtilitarian(control); }

	template <>
	HFSM2_CONSTEXPR(14)	UP		deepReportChange<RandomUtil>  (		Control& control)	noexcept	{ return deepReportChangeRandom		(control); }

#else

	HFSM2_CONSTEXPR(14)	UP		deepReportChange			  (		Control& control)	noexcept;

#endif

	HFSM2_CONSTEXPR(14)	UP		deepReportChangeComposite	  (		Control& control)	noexcept;
	HFSM2_CONSTEXPR(14)	UP		deepReportChangeResumable	  (		Control& control)	noexcept;
	HFSM2_CONSTEXPR(14)	UP		deepReportChangeSelectable	  (		Control& control)	noexcept;
	HFSM2_CONSTEXPR(14)	UP		deepReportChangeUtilitarian   (		Control& control)	noexcept;
	HFSM2_CONSTEXPR(14)	UP		deepReportChangeRandom		  (		Control& control)	noexcept;

	HFSM2_CONSTEXPR(14)	UP		deepReportUtilize			  (		Control& control)	noexcept;
	HFSM2_CONSTEXPR(14)	Rank	deepReportRank				  (		Control& control)	noexcept;
	HFSM2_CONSTEXPR(14)	Utility deepReportRandomize			  (		Control& control)	noexcept;

#endif

	HFSM2_CONSTEXPR(14)	void	deepChangeToRequested		  ( PlanControl& control)	noexcept;

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_CONSTEXPR(14)	void	deepSaveActive				  (const Registry& registry, WriteStream& stream)  const noexcept;
	HFSM2_CONSTEXPR(14)	void	deepSaveResumable			  (const Registry& registry, WriteStream& stream)  const noexcept;

	HFSM2_CONSTEXPR(14)	void	deepLoadRequested			  (		 Registry& registry, ReadStream&  stream)  const noexcept;
	HFSM2_CONSTEXPR(14)	void	deepLoadResumable			  (		 Registry& registry, ReadStream&  stream)  const noexcept;
#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr Long NAME_COUNT = HeadState::NAME_COUNT + SubStates::NAME_COUNT;

	HFSM2_CONSTEXPR(14)	void deepGetNames(const Long parent,
										  const RegionType region,
										  const Short depth,
										  StructureStateInfos& stateInfos)  const noexcept;
#endif

};

}
}


namespace hfsm2 {
namespace detail {

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
Short
C_<TN, TA, SG, TH, TS...>::resolveRandom(Control& control,
										 const Utility(& options)[WIDTH],
										 const Utility sum,
										 const Rank(& ranks)[WIDTH],
										 const Rank top) const noexcept
{
	const Utility random = control._core.rng.next();
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

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRegister(Registry& registry,
										const Parent parent) noexcept
{
	registry.compoParents[COMPO_INDEX] = parent;
	registry.regionSizes [REGION_ID]   = REGION_SIZE;

	HeadState::deepRegister(registry, parent);
	SubStates::wideRegister(registry, Parent{COMPO_ID});
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
bool
C_<TN, TA, SG, TH, TS...>::deepForwardEntryGuard(GuardControl& control) noexcept {
	const Short requested = compoRequested(control);

	const Short active	  = compoActive   (control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested == INVALID_SHORT)
		return SubStates::wideForwardEntryGuard(control, active);
	else
		return SubStates::wideEntryGuard	   (control, requested);
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
bool
C_<TN, TA, SG, TH, TS...>::deepEntryGuard(GuardControl& control) noexcept {
	const Short requested = compoRequested(control);
	HFSM2_ASSERT(requested < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return HeadState::deepEntryGuard(control) ||
		   SubStates::wideEntryGuard(control, requested);
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepEnter(PlanControl& control) noexcept {
	Short& requested = compoRequested(control);
	Short& active	 = compoActive	 (control);
	Short& resumable = compoResumable(control);

	HFSM2_ASSERT(requested < WIDTH);
	HFSM2_ASSERT(active	  == INVALID_SHORT);

	active	  = requested;

	if (requested == resumable)
		resumable = INVALID_SHORT;

	requested = INVALID_SHORT;

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HeadState::deepEnter(control);
	SubStates::wideEnter(control, active);
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepReenter(PlanControl& control) noexcept {
	Short& active	 = compoActive   (control);
	Short& resumable = compoResumable(control);
	Short& requested = compoRequested(control);

	HFSM2_ASSERT(active	   < WIDTH &&
				 requested < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HeadState::deepReenter(control);

	if (active == requested)
		SubStates::wideReenter(control, active);
	else {
		SubStates::wideExit	  (control, active);

		active  = requested;

		if (requested == resumable)
			resumable = INVALID_SHORT;

		SubStates::wideEnter  (control, active);
	}

	requested = INVALID_SHORT;
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
Status
C_<TN, TA, SG, TH, TS...>::deepPreUpdate(FullControl& control) noexcept {
	HFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const Status h =
		HeadState::deepPreUpdate(control);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePreUpdate(control, active);

	return h;
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
Status
C_<TN, TA, SG, TH, TS...>::deepUpdate(FullControl& control) noexcept {
	HFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const Status h =
		HeadState::deepUpdate(control);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::wideUpdate(control, active);

	return h;
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
Status
C_<TN, TA, SG, TH, TS...>::deepPostUpdate(FullControl& control) noexcept {
	HFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePostUpdate(control, active);

	const Status h =
		HeadState::deepPostUpdate(control);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	return h;
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
C_<TN, TA, SG, TH, TS...>::deepPreReact(FullControl& control,
										const TEvent& event) noexcept
{
	HFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const Status h =
		HeadState::deepPreReact(control, event);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePreReact(control, event, active);

	return h;
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
C_<TN, TA, SG, TH, TS...>::deepReact(FullControl& control,
									 const TEvent& event) noexcept
{
	HFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const Status h =
		HeadState::deepReact(control, event);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::wideReact(control, event, active);

	return h;
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
C_<TN, TA, SG, TH, TS...>::deepPostReact(FullControl& control,
										 const TEvent& event) noexcept
{
	HFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePostReact(control, event, active);

	const Status h =
		HeadState::deepPostReact(control, event);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	return h;
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepQuery(ConstControl& control,
									 TEvent& event) const noexcept
{
	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	HeadState::deepQuery(control, event);
	SubStates::wideQuery(control, event, active);
}

#if HFSM2_PLANS_AVAILABLE()

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
Status
C_<TN, TA, SG, TH, TS...>::deepUpdatePlans(FullControl& control) noexcept {
	HFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	const Status h =	headStatus(control) |
		HeadState::deepUpdatePlans(control);

	const Status s =	 subStatus(control) |
		SubStates::wideUpdatePlans(control, active);

	if (h)
		return h;
	else {
		if (s.outerTransition)
			return Status{Status::Result::NONE, true};

		ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

		const bool planExists = control._core.planData.planExists.template get<REGION_ID>();

		return s && planExists ?
			control.updatePlan(static_cast<HeadState&>(*this), s) : s;
	}
}

#endif

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
bool
C_<TN, TA, SG, TH, TS...>::deepForwardExitGuard(GuardControl& control) noexcept {
	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (compoRequested(control) == INVALID_SHORT)
		return SubStates::wideForwardExitGuard(control, active);
	else
		return SubStates::wideExitGuard		  (control, active);
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
bool
C_<TN, TA, SG, TH, TS...>::deepExitGuard(GuardControl& control) noexcept {
	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return HeadState::deepExitGuard(control) ||
		   SubStates::wideExitGuard(control, active);
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepExit(PlanControl& control) noexcept {
	Short& active	 = compoActive   (control);
	HFSM2_ASSERT(active < WIDTH);

	Short& resumable = compoResumable(control);

	SubStates::wideExit(control, active);
	HeadState::deepExit(control);

	resumable = active;
	active	  = INVALID_SHORT;

#if HFSM2_PLANS_AVAILABLE()
	Plan plan = control.plan(REGION_ID);
	plan.clear();
#endif
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepForwardActive(Control& control,
											 const Request request) noexcept
{
	HFSM2_ASSERT(control._core.registry.isActive(HEAD_ID));

	const Short requested = compoRequested(control);

	if (requested == INVALID_SHORT) {
		const Short active = compoActive(control);

		SubStates::wideForwardActive (control, request, active);
	} else
		SubStates::wideForwardRequest(control, request, requested);
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepForwardRequest(Control& control,
											  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const Short requested = compoRequested(control);

	if (requested != INVALID_SHORT)
		SubStates::wideForwardRequest(control, request, requested);
	else
		deepRequest					 (control, request);
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
HFSM2_CONSTEXPR(14)
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

	case TransitionType::SELECT:
		deepRequestSelect	(control, request);
		break;

#if HFSM2_UTILITY_THEORY_AVAILABLE()

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

#if !HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
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

	case Strategy::Selectable:
		deepRequestChangeSelectable	(control, request);
		break;

#if HFSM2_UTILITY_THEORY_AVAILABLE()

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

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestChangeComposite(Control& control,
													  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Short& requested = compoRequested(control);

	requested = 0;

	SubStates::wideRequestChangeComposite(control, request);
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestChangeResumable(Control& control,
													  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const Short  resumable = compoResumable(control);
		  Short& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT) ?
		resumable : 0;

	HFSM2_ASSERT(requested < WIDTH);

	SubStates::wideRequestChangeResumable(control, request, requested);
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestChangeSelectable(Control& control,
													   const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Short& requested = compoRequested(control);
	requested = HeadState::wrapSelect(control);

	HFSM2_ASSERT(requested < WIDTH);

	HFSM2_LOG_SELECT_RESOLUTION(control.context(), HEAD_ID, requested);
}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestChangeUtilitarian(Control& control,
														const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const UP s = SubStates::wideReportChangeUtilitarian(control);
	HFSM2_ASSERT(s.prong < WIDTH);

	Short& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestChangeRandom(Control& control,
												   const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Rank ranks[WIDTH] = { Rank{} };
	Rank top = SubStates::wideReportRank(control, ranks);

	Utility options[WIDTH] = { Utility{} };
	const UP sum = SubStates::wideReportChangeRandom(control, options, ranks, top);

	Short& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum.utility, ranks, top);
	HFSM2_ASSERT(requested < WIDTH);
}

#endif

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestRestart(Control& control,
											  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Short& requested = compoRequested(control);

	requested = 0;

	SubStates::wideRequestRestart(control, request);
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestResume(Control& control,
											 const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const Short  resumable = compoResumable(control);
		  Short& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT) ?
		resumable : 0;

	HFSM2_ASSERT(requested < WIDTH);

	SubStates::wideRequestResume(control, request, requested);
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestSelect(Control& control,
											 const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Short& requested = compoRequested(control);
	requested = HeadState::wrapSelect(control);

	HFSM2_ASSERT(requested < WIDTH);

	HFSM2_LOG_SELECT_RESOLUTION(control.context(), HEAD_ID, requested);
}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestUtilize(Control& control,
											  const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const UP s = SubStates::wideReportUtilize(control);

	Short& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_ASSERT(s.prong < WIDTH);

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestRandomize(Control& control,
												const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Rank ranks[WIDTH] = { Rank{} };
	Rank top = SubStates::wideReportRank(control, ranks);

	Utility options[WIDTH] = { Utility{} };
	const Utility sum = SubStates::wideReportRandomize(control, options, ranks, top);

	Short& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum, ranks, top);
	HFSM2_ASSERT(requested < WIDTH);
}

#endif

#if HFSM2_UTILITY_THEORY_AVAILABLE()
#if !HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChange(Control& control) noexcept {
	switch (STRATEGY) {
	case Strategy::Composite:
		return deepReportChangeComposite  (control);

	case Strategy::Resumable:
		return deepReportChangeResumable  (control);

	case Strategy::Selectable:
		return deepReportChangeSelectable (control);

	case Strategy::Utilitarian:
		return deepReportChangeUtilitarian(control);

	case Strategy::RandomUtil:
		return deepReportChangeRandom	  (control);

	default:
		HFSM2_BREAK();
		return UP{};
	}
}

#endif

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChangeComposite(Control& control) noexcept {
	Short& requested = compoRequested(control);
	requested = 0;

	const UP h = HeadState::deepReportChange(control);
	const UP s = SubStates::wideReportChangeComposite(control);

	return {
		h.utility * s.utility,
		h.prong
	};
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChangeResumable(Control& control) noexcept {
	const Short  resumable = compoResumable(control);
		  Short& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT) ?
		resumable : 0;

	const UP h = HeadState::deepReportChange(control);
	const UP s = SubStates::wideReportChangeResumable(control, requested);

	return {
		h.utility * s.utility,
		h.prong
	};
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChangeSelectable(Control& control) noexcept {
	const Short  resumable = compoResumable(control);
		  Short& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT) ?
		resumable : 0;

	HFSM2_ASSERT(requested < WIDTH);

	const UP h = HeadState::deepReportChange		  (control);
	const UP s = SubStates::wideReportChangeSelectable(control, requested);

	return {
		h.utility * s.utility,
		h.prong
	};
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChangeUtilitarian(Control& control) noexcept {
	const UP h = HeadState::deepReportChange(control);
	const UP s = SubStates::wideReportChangeUtilitarian(control);

	Short& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);

	return {
		h.utility * s.utility,
		h.prong
	};
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChangeRandom(Control& control) noexcept {
	const UP h = HeadState::deepReportChange(control);

	Rank ranks[WIDTH] = { Rank{} };
	Rank top = SubStates::wideReportRank(control, ranks);

	Utility options[WIDTH] = { Utility{} };
	const UP sum = SubStates::wideReportChangeRandom(control, options, ranks, top);

	Short& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum.utility, ranks, top);
	HFSM2_ASSERT(requested < WIDTH);

	return {
		h.utility * options[requested],
		h.prong
	};
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportUtilize(Control& control) noexcept {
	const UP h = HeadState::deepReportUtilize(control);
	const UP s = SubStates::wideReportUtilize(control);

	Short& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);

	return {
		h.utility * s.utility,
		h.prong
	};
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::Rank
C_<TN, TA, SG, TH, TS...>::deepReportRank(Control& control) noexcept {
	return HeadState::wrapRank(control);
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::Utility
C_<TN, TA, SG, TH, TS...>::deepReportRandomize(Control& control) noexcept {
	const Utility h = HeadState::wrapUtility(control);

	Rank ranks[WIDTH] = { Rank{} };
	Rank top = SubStates::wideReportRank(control, ranks);

	Utility options[WIDTH] = { Utility{} };
	const Utility sum = SubStates::wideReportRandomize(control, options, ranks, top);

	Short& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum, ranks, top);
	HFSM2_ASSERT(requested < WIDTH);

	return h * options[requested];
}

#endif

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepChangeToRequested(PlanControl& control) noexcept {
	Short& requested = compoRequested(control);
	Short& active	 = compoActive	 (control);
	Short& resumable = compoResumable(control);

	HFSM2_ASSERT(active < WIDTH);

	if (requested == INVALID_SHORT)
		SubStates::wideChangeToRequested(control, active);
	else if (requested != active) {
		SubStates::wideExit   (control, active);

		HFSM2_ASSERT(requested < WIDTH);

		resumable = active;
		active	  = requested;
		requested = INVALID_SHORT;

		SubStates::wideEnter  (control, active);
	} else if (compoRemain(control)) {
		SubStates::wideExit   (control, active);

		requested = INVALID_SHORT;

		SubStates::wideEnter  (control, active);
	} else {
		requested = INVALID_SHORT;

		SubStates::wideReenter(control, active);
	}
}

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
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

	SubStates::wideSaveActive(registry,stream, active);
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
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

	SubStates::wideSaveResumable(registry, stream);
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
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

	SubStates::wideLoadRequested(registry, stream, requested);
}

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
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

	SubStates::wideLoadResumable(registry, stream);
}

#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepGetNames(const Long parent,
										const RegionType regionType,
										const Short depth,
										StructureStateInfos& stateInfos) const noexcept
{
	HeadState::deepGetNames(parent,					regionType,								   depth,	  stateInfos);
	SubStates::wideGetNames(stateInfos.count() - 1, StructureStateInfo::RegionType::COMPOSITE, depth + 1, stateInfos);
}

#endif

}
}

namespace hfsm2 {
namespace detail {

template <typename, typename, Short, typename...>
struct OS_;

template <typename TIndices,
		  typename TArgs,
		  Short NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct HFSM2_EMPTY_BASES OS_<TIndices, TArgs, NIndex, TInitial, TRemaining...>
	: InitialOS<TIndices,
				TArgs,
				TInitial>
	, RemainingOS<TIndices,
				  TArgs,
				  NIndex,
				  TInitial,
				  TRemaining...>
{
	using Indices		= TIndices;
	static constexpr StateID  INITIAL_ID  = Indices::STATE_ID;
	static constexpr Short	  COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	  ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	  ORTHO_UNIT  = Indices::ORTHO_UNIT;

	static constexpr Short	  PRONG_INDEX = NIndex;

	using Args			= TArgs;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;
	using OrthoForks	= typename Registry::OrthoForks;
	using ProngBits		= typename OrthoForks::Bits;
	using ProngCBits	= typename OrthoForks::CBits;

	using ConstControl	= ConstControlT<Args>;
	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using InitialInfo	= WrapInfo<TInitial>;
	using InitialStates	= typename InitialInfo::StateList;

	using Initial		= InitialOS<TIndices,
									TArgs,
									TInitial>;

	using Remaining		= RemainingOS<TIndices,
									  TArgs,
									  NIndex,
									  TInitial,
									  TRemaining...>;

	HFSM2_CONSTEXPR(14)	void	wideRegister		 (Registry& registry, const ForkID forkId)										noexcept;

	HFSM2_CONSTEXPR(14)	bool	wideForwardEntryGuard(GuardControl& control,						const ProngCBits prongs)		noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideForwardEntryGuard(GuardControl& control												   )		noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideEntryGuard		 (GuardControl& control												   )		noexcept;

	HFSM2_CONSTEXPR(14)	void	wideEnter			 ( PlanControl& control												   )		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideReenter			 ( PlanControl& control												   )		noexcept;

	HFSM2_CONSTEXPR(14)	Status	widePreUpdate		 ( FullControl& control												   )		noexcept;
	HFSM2_CONSTEXPR(14)	Status	wideUpdate			 ( FullControl& control												   )		noexcept;
	HFSM2_CONSTEXPR(14)	Status	widePostUpdate		 ( FullControl& control												   )		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	widePreReact		 ( FullControl& control, const TEvent& event						   )		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	wideReact			 ( FullControl& control, const TEvent& event						   )		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	widePostReact		 ( FullControl& control, const TEvent& event						   )		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	wideQuery			 (ConstControl& control,	   TEvent& event						   )  const noexcept;

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14)	Status	wideUpdatePlans		 ( FullControl& control												   )		noexcept;
#endif

	HFSM2_CONSTEXPR(14)	bool	wideForwardExitGuard (GuardControl& control,						const ProngCBits prongs)		noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideForwardExitGuard (GuardControl& control												   )		noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideExitGuard		 (GuardControl& control												   )		noexcept;

	HFSM2_CONSTEXPR(14)	void	wideExit			 ( PlanControl& control												   )		noexcept;

	HFSM2_CONSTEXPR(14)	void	wideForwardActive	 (	   Control& control, const Request request, const ProngCBits prongs)		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideForwardRequest	 (	   Control& control, const Request request						   )		noexcept;

	HFSM2_CONSTEXPR(14)	void	wideRequestChange	 (	   Control& control, const Request request						   )		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestRestart	 (	   Control& control, const Request request						   )		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestResume	 (	   Control& control, const Request request						   )		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestSelect	 (	   Control& control, const Request request						   )		noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void	wideRequestUtilize	 (	   Control& control, const Request request						   )		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestRandomize (	   Control& control, const Request request						   )		noexcept;

	HFSM2_CONSTEXPR(14)	Utility wideReportChange	 (	   Control& control												   )		noexcept;
	HFSM2_CONSTEXPR(14)	Utility wideReportUtilize	 (	   Control& control												   )		noexcept;
	HFSM2_CONSTEXPR(14)	Utility wideReportRandomize  (	   Control& control												   )		noexcept;
#endif

	HFSM2_CONSTEXPR(14)	void	wideChangeToRequested( PlanControl& control												   )		noexcept;

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_CONSTEXPR(14)	void	wideSaveActive		  (const Registry& registry, WriteStream& stream)  const noexcept;
	HFSM2_CONSTEXPR(14)	void	wideSaveResumable	  (const Registry& registry, WriteStream& stream)  const noexcept;

	HFSM2_CONSTEXPR(14)	void	wideLoadRequested	  (		 Registry& registry, ReadStream&  stream)  const noexcept;
	HFSM2_CONSTEXPR(14)	void	wideLoadResumable	  (		 Registry& registry, ReadStream&  stream)  const noexcept;
#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename Args::StructureStateInfos;

	static constexpr Long NAME_COUNT	 = Initial::NAME_COUNT  + Remaining::NAME_COUNT;

	HFSM2_CONSTEXPR(14)	void wideGetNames(const Long parent,
										  const Short depth,
										  StructureStateInfos& stateInfos)	  const noexcept;
#endif

};

template <typename TIndices,
		  typename TArgs,
		  Short NIndex,
		  typename TInitial>
struct OS_<TIndices, TArgs, NIndex, TInitial>
	: InitialOS<TIndices,
				TArgs,
				TInitial>
{
	using Indices		= TIndices;
	static constexpr StateID INITIAL_ID	 = Indices::STATE_ID;
	static constexpr Short	 COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	 ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	 ORTHO_UNIT	 = Indices::ORTHO_UNIT;

	static constexpr Short	 PRONG_INDEX = NIndex;

	using Args			= TArgs;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;
	using OrthoForks	= typename Registry::OrthoForks;
	using ProngBits		= typename OrthoForks::Bits;
	using ProngCBits= typename OrthoForks::CBits;

	using ConstControl	= ConstControlT<Args>;
	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Initial		= InitialOS<TIndices,
									TArgs,
									TInitial>;

	HFSM2_CONSTEXPR(14)	void	wideRegister		 (Registry& registry, const ForkID forkId)										noexcept;

	HFSM2_CONSTEXPR(14)	bool	wideForwardEntryGuard(GuardControl& control,						const ProngCBits prongs)		noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideForwardEntryGuard(GuardControl& control												   )		noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideEntryGuard		 (GuardControl& control												   )		noexcept;

	HFSM2_CONSTEXPR(14)	void	wideEnter			 ( PlanControl& control												   )		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideReenter			 ( PlanControl& control												   )		noexcept;

	HFSM2_CONSTEXPR(14)	Status	widePreUpdate		 ( FullControl& control												   )		noexcept;
	HFSM2_CONSTEXPR(14)	Status	wideUpdate			 ( FullControl& control												   )		noexcept;
	HFSM2_CONSTEXPR(14)	Status	widePostUpdate		 ( FullControl& control												   )		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	widePreReact		 ( FullControl& control, const TEvent& event						   )		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	wideReact			 ( FullControl& control, const TEvent& event						   )		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	widePostReact		 ( FullControl& control, const TEvent& event						   )		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	wideQuery			 (ConstControl& control,	   TEvent& event						   )  const noexcept;

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14)	Status	wideUpdatePlans		 ( FullControl& control												   )		noexcept;
#endif

	HFSM2_CONSTEXPR(14)	bool	wideForwardExitGuard (GuardControl& control,						const ProngCBits prongs)		noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideForwardExitGuard (GuardControl& control												   )		noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideExitGuard		 (GuardControl& control												   )		noexcept;

	HFSM2_CONSTEXPR(14)	void	wideExit			 ( PlanControl& control												   )		noexcept;

	HFSM2_CONSTEXPR(14)	void	wideForwardActive	 (	   Control& control, const Request request, const ProngCBits prongs)		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideForwardRequest	 (	   Control& control, const Request request						   )		noexcept;

	HFSM2_CONSTEXPR(14)	void	wideRequestChange	 (	   Control& control, const Request request						   )		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestRestart	 (	   Control& control, const Request request						   )		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestResume	 (	   Control& control, const Request request						   )		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestSelect	 (	   Control& control, const Request request						   )		noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void	wideRequestUtilize	 (	   Control& control, const Request request						   )		noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestRandomize (	   Control& control, const Request request						   )		noexcept;

	HFSM2_CONSTEXPR(14)	Utility wideReportChange	 (	   Control& control												   )		noexcept;
	HFSM2_CONSTEXPR(14)	Utility wideReportUtilize	 (	   Control& control												   )		noexcept;
	HFSM2_CONSTEXPR(14)	Utility wideReportRandomize  (	   Control& control												   )		noexcept;
#endif

	HFSM2_CONSTEXPR(14)	void	wideChangeToRequested( PlanControl& control												   )		noexcept;

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_CONSTEXPR(14)	void	wideSaveActive		  (const Registry& registry, WriteStream& stream)  const noexcept;
	HFSM2_CONSTEXPR(14)	void	wideSaveResumable	  (const Registry& registry, WriteStream& stream)  const noexcept;

	HFSM2_CONSTEXPR(14)	void	wideLoadRequested	  (		 Registry& registry, ReadStream&  stream)  const noexcept;
	HFSM2_CONSTEXPR(14)	void	wideLoadResumable	  (		 Registry& registry, ReadStream&  stream)  const noexcept;
#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename Args::StructureStateInfos;

	static constexpr Long NAME_COUNT	 = Initial::NAME_COUNT;

	HFSM2_CONSTEXPR(14)	void wideGetNames(const Long parent,
										  const Short depth,
										  StructureStateInfos& stateInfos)							  const noexcept;
#endif

};

}
}


namespace hfsm2 {
namespace detail {

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideRegister(Registry& registry,
										 const ForkID forkId) noexcept
{
	Initial	 ::deepRegister(registry, Parent{forkId, PRONG_INDEX});
	Remaining::wideRegister(registry, forkId);
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardEntryGuard(GuardControl& control,
												  const ProngCBits prongs) noexcept
{
	const bool i = prongs.get(PRONG_INDEX) ?
				   Initial	::deepForwardEntryGuard(control) : false;

	const bool r = Remaining::wideForwardEntryGuard(control, prongs);

	return i || r;
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardEntryGuard(GuardControl& control) noexcept {
	const bool i = Initial	::deepForwardEntryGuard(control);
	const bool r = Remaining::wideForwardEntryGuard(control);

	return i || r;
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI, TR...>::wideEntryGuard(GuardControl& control) noexcept {
	const bool i = Initial	::deepEntryGuard(control);
	const bool r = Remaining::wideEntryGuard(control);

	return i || r;
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideEnter(PlanControl& control) noexcept {
	Initial	 ::deepEnter(control);
	Remaining::wideEnter(control);
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideReenter(PlanControl& control) noexcept {
	Initial	 ::deepReenter(control);
	Remaining::wideReenter(control);
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI, TR...>::widePreUpdate(FullControl& control) noexcept {
	Status status;
	status |= Initial	  ::deepPreUpdate(			   control);
	status |= Remaining	  ::widePreUpdate(			   control);

	return status;
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI, TR...>::wideUpdate(FullControl& control) noexcept {
	Status status;
	status |= Initial	  ::deepUpdate(				control);
	status |= Remaining	  ::wideUpdate(				control);

	return status;
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI, TR...>::widePostUpdate(FullControl& control) noexcept {
	Status status;
	status |= Initial	  ::deepPostUpdate(				control);
	status |= Remaining	  ::widePostUpdate(				control);

	return status;
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI, TR...>::widePreReact(FullControl& control,
										 const TEvent& event) noexcept
{
	Status status;
	status |= Initial	  ::deepPreReact(			  control, event);
	status |= Remaining	  ::widePreReact(			  control, event);

	return status;
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI, TR...>::wideReact(FullControl& control,
									  const TEvent& event) noexcept
{
	Status status;
	status |= Initial	  ::deepReact(			   control, event);
	status |= Remaining	  ::wideReact(			   control, event);

	return status;
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI, TR...>::widePostReact(FullControl& control,
										  const TEvent& event) noexcept
{
	Status status;
	status |= Initial	  ::deepPostReact(			   control, event);
	status |= Remaining	  ::widePostReact(			   control, event);

	return status;
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideQuery(ConstControl& control,
									  TEvent& event) const noexcept
{
	Initial				  ::wideQuery(				control, event);
	Remaining			  ::wideQuery(				control, event);
}

#if HFSM2_PLANS_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI, TR...>::wideUpdatePlans(FullControl& control) noexcept {
	Status status;
	status |= Initial	  ::deepUpdatePlans(			 control);
	status |= Remaining	  ::wideUpdatePlans(			 control);

	return status;
}

#endif

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardExitGuard(GuardControl& control,
												 const ProngCBits prongs) noexcept
{
	const bool i = prongs.get(PRONG_INDEX) ?
				   Initial	::deepForwardExitGuard(control) : false;

	const bool r = Remaining::wideForwardExitGuard(control, prongs);

	return i || r;
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardExitGuard(GuardControl& control) noexcept {
	const bool i = Initial	::deepForwardExitGuard(control);
	const bool r = Remaining::wideForwardExitGuard(control);

	return i || r;
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI, TR...>::wideExitGuard(GuardControl& control) noexcept {
	const bool i = Initial	::deepExitGuard(control);
	const bool r = Remaining::wideExitGuard(control);

	return i || r;
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideExit(PlanControl& control) noexcept {
	Initial	 ::deepExit(control);
	Remaining::wideExit(control);
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideForwardActive(Control& control,
											  const Request request,
											  const ProngCBits prongs) noexcept
{
	if (prongs.get(PRONG_INDEX))
		Initial::deepForwardActive(control, request);

	Remaining  ::wideForwardActive(control, request, prongs);
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideForwardRequest(Control& control,
											   const Request request) noexcept
{
	Initial	 ::deepForwardRequest(control, request);
	Remaining::wideForwardRequest(control, request);
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideRequestChange(Control& control,
											  const Request request) noexcept
{
	Initial	 ::deepRequestChange(control, request);
	Remaining::wideRequestChange(control, request);
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideRequestRestart(Control& control,
											   const Request request) noexcept
{
	Initial	 ::deepRequestRestart(control, request);
	Remaining::wideRequestRestart(control, request);
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideRequestResume(Control& control,
											  const Request request) noexcept
{
	Initial	 ::deepRequestResume(control, request);
	Remaining::wideRequestResume(control, request);
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideRequestSelect(Control& control,
											  const Request request) noexcept
{
	Initial	 ::deepRequestSelect(control, request);
	Remaining::wideRequestSelect(control, request);
}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideRequestUtilize(Control& control,
											   const Request request) noexcept
{
	Initial	 ::deepRequestUtilize(control, request);
	Remaining::wideRequestUtilize(control, request);
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideRequestRandomize(Control& control,
												 const Request request) noexcept
{
	Initial	 ::deepRequestRandomize(control, request);
	Remaining::wideRequestRandomize(control, request);
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
typename TA::Utility
OS_<TN, TA, NI, TI, TR...>::wideReportChange(Control& control) noexcept {
	const UP	  i = Initial  ::deepReportChange(control);
	const Utility r = Remaining::wideReportChange(control);

	return i.utility + r;
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
typename TA::Utility
OS_<TN, TA, NI, TI, TR...>::wideReportUtilize(Control& control) noexcept {
	const UP	  i = Initial  ::deepReportUtilize(control);
	const Utility r = Remaining::wideReportUtilize(control);

	return i.utility + r;
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
typename TA::Utility
OS_<TN, TA, NI, TI, TR...>::wideReportRandomize(Control& control) noexcept {
	const Utility i = Initial  ::deepReportRandomize(control);
	const Utility r = Remaining::wideReportRandomize(control);

	return i + r;
}

#endif

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideChangeToRequested(PlanControl& control) noexcept {
	Initial	 ::deepChangeToRequested(control);
	Remaining::wideChangeToRequested(control);
}

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideSaveActive(const Registry& registry,
										   WriteStream& stream) const noexcept
{
	Initial	 ::deepSaveActive(registry, stream);
	Remaining::wideSaveActive(registry, stream);
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideSaveResumable(const Registry& registry,
											  WriteStream& stream) const noexcept
{
	Initial	 ::deepSaveResumable(registry, stream);
	Remaining::wideSaveResumable(registry, stream);
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideLoadRequested(Registry& registry,
											  ReadStream& stream) const noexcept
{
	Initial	 ::deepLoadRequested(registry, stream);
	Remaining::wideLoadRequested(registry, stream);
}

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideLoadResumable(Registry& registry,
											  ReadStream& stream) const noexcept
{
	Initial	 ::deepLoadResumable(registry, stream);
	Remaining::wideLoadResumable(registry, stream);
}

#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideGetNames(const Long parent,
										 const Short depth,
										 StructureStateInfos& stateInfos) const noexcept
{
	Initial	 ::deepGetNames(parent, StructureStateInfo::RegionType::ORTHOGONAL, depth, stateInfos);
	Remaining::wideGetNames(parent,												depth, stateInfos);
}

#endif

}
}

namespace hfsm2 {
namespace detail {

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRegister(Registry& registry,
								  const ForkID forkId) noexcept
{
	Initial		   ::deepRegister(			registry, Parent{forkId, PRONG_INDEX});
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI>::wideForwardEntryGuard(GuardControl& control,
										   const ProngCBits prongs) noexcept
{
	return prongs.get(PRONG_INDEX) ?
		Initial	   ::deepForwardEntryGuard(				 control) : false;
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI>::wideForwardEntryGuard(GuardControl& control) noexcept {
	return Initial ::deepForwardEntryGuard(				 control);
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI>::wideEntryGuard(GuardControl& control) noexcept {
	return Initial ::deepEntryGuard(			  control);
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideEnter(PlanControl& control) noexcept {
	Initial		   ::deepEnter(				control);
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideReenter(PlanControl& control) noexcept {
	Initial		   ::deepReenter(			  control);
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI>::widePreUpdate(FullControl& control) noexcept {
	return Initial ::deepPreUpdate(				control);
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI>::wideUpdate(FullControl& control) noexcept {
	return Initial ::deepUpdate(			 control);
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI>::widePostUpdate(FullControl& control) noexcept {
	return Initial ::deepPostUpdate(			 control);
}

template <typename TN, typename TA, Short NI, typename TI>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI>::widePreReact(FullControl& control,
								  const TEvent& event) noexcept
{
	return Initial ::deepPreReact(			   control, event);
}

template <typename TN, typename TA, Short NI, typename TI>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI>::wideReact(FullControl& control,
							   const TEvent& event) noexcept
{
	return Initial ::deepReact(				control, event);
}

template <typename TN, typename TA, Short NI, typename TI>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI>::widePostReact(FullControl& control,
								   const TEvent& event) noexcept
{
	return Initial ::deepPostReact(				control, event);
}

template <typename TN, typename TA, Short NI, typename TI>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideQuery(ConstControl& control,
							   TEvent& event) const noexcept
{
	Initial		   ::wideQuery(				 control, event);
}

#if HFSM2_PLANS_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI>::wideUpdatePlans(FullControl& control) noexcept {
	return Initial ::deepUpdatePlans(			  control);
}

#endif

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI>::wideForwardExitGuard(GuardControl& control,
										  const ProngCBits prongs) noexcept
{
	return prongs.get(PRONG_INDEX) ?
		Initial	   ::deepForwardExitGuard(				control) : false;
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI>::wideForwardExitGuard(GuardControl& control) noexcept {
	return Initial ::deepForwardExitGuard(				control);
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI>::wideExitGuard(GuardControl& control) noexcept {
	return Initial ::deepExitGuard(				 control);
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideExit(PlanControl& control) noexcept {
	Initial		   ::deepExit(			   control);
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideForwardActive(Control& control,
									   const Request request,
									   const ProngCBits prongs) noexcept
{
	if (prongs.get(PRONG_INDEX))
		Initial	   ::deepForwardActive(			control, request);
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideForwardRequest(Control& control,
										const Request request) noexcept
{
	Initial		   ::deepForwardRequest(		 control, request);
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRequestChange(Control& control,
									   const Request request) noexcept
{
	Initial		   ::deepRequestChange(			control, request);
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRequestRestart(Control& control,
										const Request request) noexcept
{
	Initial		   ::deepRequestRestart(		 control, request);
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRequestResume(Control& control,
									   const Request request) noexcept
{
	Initial		   ::deepRequestResume(			control, request);
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRequestSelect(Control& control,
									   const Request request) noexcept
{
	Initial		   ::deepRequestSelect(			control, request);
}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRequestUtilize(Control& control,
										const Request request) noexcept
{
	Initial		   ::deepRequestUtilize(		 control, request);
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRequestRandomize(Control& control,
										  const Request request) noexcept
{
	Initial		   ::deepRequestRandomize(		   control, request);
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
typename TA::Utility
OS_<TN, TA, NI, TI>::wideReportChange(Control& control) noexcept {
	const UP i = Initial::deepReportChange(control);

	return i.utility;
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
typename TA::Utility
OS_<TN, TA, NI, TI>::wideReportUtilize(Control& control) noexcept {
	const UP i = Initial::deepReportUtilize(control);

	return i.utility;
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
typename TA::Utility
OS_<TN, TA, NI, TI>::wideReportRandomize(Control& control) noexcept {
	return Initial::deepReportRandomize(control);
}

#endif

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideChangeToRequested(PlanControl& control) noexcept {
	Initial		   ::deepChangeToRequested(				control);
}

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideSaveActive(const Registry& registry,
									WriteStream& stream) const noexcept
{
	Initial		   ::deepSaveActive(				registry, stream);
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideSaveResumable(const Registry& registry,
									   WriteStream& stream) const noexcept
{
	Initial		   ::deepSaveResumable(				   registry, stream);
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideLoadRequested(Registry& registry,
									   ReadStream& stream) const noexcept
{
	Initial		   ::deepLoadRequested(			 registry, stream);
}

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideLoadResumable(Registry& registry,
									   ReadStream& stream) const noexcept
{
	Initial		   ::deepLoadResumable(			 registry, stream);
}

#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideGetNames(const Long parent,
								  const Short depth,
								  StructureStateInfos& stateInfos) const noexcept
{
	Initial		   ::deepGetNames(parent, StructureStateInfo::RegionType::ORTHOGONAL, depth, stateInfos);
}

#endif

}
}

namespace hfsm2 {
namespace detail {

template <typename TIndices,
		  typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct HFSM2_EMPTY_BASES O_
	: S_<TIndices, TArgs, THead>
	, OS_<I_<TIndices::STATE_ID + 1,
			 TIndices::COMPO_INDEX,
			 TIndices::ORTHO_INDEX + 1,
			 TIndices::ORTHO_UNIT + contain(OI_<THead, TSubStates...>::WIDTH, 8)>,
		  TArgs,
		  0,
		  TSubStates...>
{
	using Indices		= TIndices;
	static constexpr StateID HEAD_ID	 = Indices::STATE_ID;
	static constexpr Short	 COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	 ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	 ORTHO_UNIT	 = Indices::ORTHO_UNIT;

	static constexpr Short	 REGION_ID	 = COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID	 ORTHO_ID	 = static_cast<ForkID>(-ORTHO_INDEX - 1);

	using Args			= TArgs;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
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

	using ConstControl	= ConstControlT<Args>;
	using Control		= ControlT	   <Args>;
	using ScopedOrigin	= typename Control::Origin;

	using PlanControl	= PlanControlT <Args>;
	using ScopedRegion	= typename PlanControl::Region;

	using FullControl	= FullControlT <Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;

	using HeadState		= S_<Indices, Args, Head>;

	using SubStates		= OS_<I_<HEAD_ID + 1,
								 COMPO_INDEX,
								 ORTHO_INDEX + 1,
								 ORTHO_UNIT + contain(WIDTH, 8)>,
							  Args,
							  0,
							  TSubStates...>;

	HFSM2_CONSTEXPR(11)	static ProngBits  orthoRequested(	   Registry& registry)		noexcept	{ return			   registry.orthoRequested.template  bits<ORTHO_UNIT, WIDTH>();	}
	HFSM2_CONSTEXPR(11)	static ProngCBits orthoRequested(const Registry& registry)		noexcept	{ return			   registry.orthoRequested.template cbits<ORTHO_UNIT, WIDTH>();	}

	HFSM2_CONSTEXPR(11)	static ProngBits  orthoRequested(		Control& control )		noexcept	{ return control._core.registry.orthoRequested.template  bits<ORTHO_UNIT, WIDTH>();	}
	HFSM2_CONSTEXPR(11)	static ProngCBits orthoRequested(const	Control& control )		noexcept	{ return control._core.registry.orthoRequested.template cbits<ORTHO_UNIT, WIDTH>();	}

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(11)	static Status&	  headStatus	(		Control& control )		noexcept	{ return control._core.planData.headStatuses[REGION_ID];	}
	HFSM2_CONSTEXPR(11)	static Status&	  subStatus		(		Control& control )		noexcept	{ return control._core.planData.subStatuses [REGION_ID];	}
#endif

	HFSM2_CONSTEXPR(14)	void	deepRegister		 (Registry& registry, const Parent parent)				noexcept;

	HFSM2_CONSTEXPR(14)	bool	deepForwardEntryGuard(GuardControl& control						  )			noexcept;
	HFSM2_CONSTEXPR(14)	bool	deepEntryGuard		 (GuardControl& control						  )			noexcept;

	HFSM2_CONSTEXPR(14)	void	deepEnter			 ( PlanControl& control						  )			noexcept;
	HFSM2_CONSTEXPR(14)	void	deepReenter			 ( PlanControl& control						  )			noexcept;

	HFSM2_CONSTEXPR(14)	Status	deepPreUpdate		 ( FullControl& control						  )			noexcept;
	HFSM2_CONSTEXPR(14)	Status	deepUpdate			 ( FullControl& control						  )			noexcept;
	HFSM2_CONSTEXPR(14)	Status	deepPostUpdate		 ( FullControl& control						  )			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	deepPreReact		 ( FullControl& control, const TEvent& event  )			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	deepReact			 ( FullControl& control, const TEvent& event  )			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	deepPostReact		 ( FullControl& control, const TEvent& event  )			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	deepQuery			 (ConstControl& control,	   TEvent& event  )	  const noexcept;

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14)	Status	deepUpdatePlans		 ( FullControl& control						  )			noexcept;
#endif

	HFSM2_CONSTEXPR(14)	bool	deepForwardExitGuard (GuardControl& control						  )			noexcept;
	HFSM2_CONSTEXPR(14)	bool	deepExitGuard		 (GuardControl& control						  )			noexcept;

	HFSM2_CONSTEXPR(14)	void	deepExit			 ( PlanControl& control						  )			noexcept;

	HFSM2_CONSTEXPR(14)	void	deepForwardActive	 (	   Control& control, const Request request)			noexcept;
	HFSM2_CONSTEXPR(14)	void	deepForwardRequest	 (	   Control& control, const Request request)			noexcept;

	HFSM2_CONSTEXPR(14)	void	deepRequest			 (	   Control& control, const Request request)			noexcept;

	HFSM2_CONSTEXPR(14)	void	deepRequestChange	 (	   Control& control, const Request request)			noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestRestart	 (	   Control& control, const Request request)			noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestResume	 (	   Control& control, const Request request)			noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestSelect	 (	   Control& control, const Request request)			noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void	deepRequestUtilize	 (	   Control& control, const Request request)			noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestRandomize (	   Control& control, const Request request)			noexcept;

	HFSM2_CONSTEXPR(14)	UP		deepReportChange	 (	   Control& control						  )			noexcept;

	HFSM2_CONSTEXPR(14)	UP		deepReportUtilize	 (	   Control& control						  )			noexcept;
	HFSM2_CONSTEXPR(14)	Rank	deepReportRank		 (	   Control& control						  )			noexcept;
	HFSM2_CONSTEXPR(14)	Utility deepReportRandomize  (	   Control& control						  )			noexcept;
#endif

	HFSM2_CONSTEXPR(14)	void	deepChangeToRequested( PlanControl& control						  )			noexcept;

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_CONSTEXPR(14)	void	deepSaveActive		 (const Registry& registry, WriteStream& stream)  const noexcept;
	HFSM2_CONSTEXPR(14)	void	deepSaveResumable	 (const Registry& registry, WriteStream& stream)  const noexcept;

	HFSM2_CONSTEXPR(14)	void	deepLoadRequested	 (		Registry& registry, ReadStream&  stream)  const noexcept;
	HFSM2_CONSTEXPR(14)	void	deepLoadResumable	 (		Registry& registry, ReadStream&  stream)  const noexcept;
#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr Long NAME_COUNT	 = HeadState::NAME_COUNT  + SubStates::NAME_COUNT;

	HFSM2_CONSTEXPR(14)	void deepGetNames(const Long parent,
										  const RegionType region,
										  const Short depth,
										  StructureStateInfos& stateInfos)							  const noexcept;
#endif

};

}
}


namespace hfsm2 {
namespace detail {

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepRegister(Registry& registry,
										const Parent parent) noexcept
{
	registry.orthoParents[ORTHO_INDEX] = parent;
	registry.orthoUnits	 [ORTHO_INDEX] = Units{ORTHO_UNIT, WIDTH};
	registry.regionSizes [REGION_ID]   = REGION_SIZE;

	HeadState::deepRegister(registry, parent);
	SubStates::wideRegister(registry, ORTHO_ID);
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
bool
O_<TN, TA, TH, TS...>::deepForwardEntryGuard(GuardControl& control) noexcept {
	const ProngCBits requested = orthoRequested(static_cast<const GuardControl&>(control));

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested)
		return SubStates::wideForwardEntryGuard(control, requested);
	else
		return SubStates::wideForwardEntryGuard(control);
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
bool
O_<TN, TA, TH, TS...>::deepEntryGuard(GuardControl& control) noexcept {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return HeadState::deepEntryGuard(control) ||
		   SubStates::wideEntryGuard(control);
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepEnter(PlanControl& control) noexcept {
	ProngBits requested = orthoRequested(control);
	requested.clear();

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HeadState::deepEnter(control);
	SubStates::wideEnter(control);
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepReenter(PlanControl& control) noexcept {
	ProngBits requested = orthoRequested(control);
	requested.clear();

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HeadState::deepReenter(control);
	SubStates::wideReenter(control);
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
Status
O_<TN, TA, TH, TS...>::deepPreUpdate(FullControl& control) noexcept {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const Status h =
		HeadState::deepPreUpdate(control);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePreUpdate(control);

	return h;
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
Status
O_<TN, TA, TH, TS...>::deepUpdate(FullControl& control) noexcept {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const Status h =
		HeadState::deepUpdate(control);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::wideUpdate(control);

	return h;
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
Status
O_<TN, TA, TH, TS...>::deepPostUpdate(FullControl& control) noexcept {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePostUpdate(control);

	const Status h =
		HeadState::deepPostUpdate(control);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	return h;
}

template <typename TN, typename TA, typename TH, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
O_<TN, TA, TH, TS...>::deepPreReact(FullControl& control,
									const TEvent& event) noexcept
{
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const Status h =
		HeadState::deepPreReact(control, event);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePreReact(control, event);

	return h;
}

template <typename TN, typename TA, typename TH, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
O_<TN, TA, TH, TS...>::deepReact(FullControl& control,
								 const TEvent& event) noexcept
{
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const Status h =
		HeadState::deepReact(control, event);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::wideReact(control, event);

	return h;
}

template <typename TN, typename TA, typename TH, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
O_<TN, TA, TH, TS...>::deepPostReact(FullControl& control,
									 const TEvent& event) noexcept
{
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePostReact(control, event);

	const Status h =
		HeadState::deepPostReact(control, event);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	return h;
}

template <typename TN, typename TA, typename TH, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepQuery(ConstControl& control,
								 TEvent& event) const noexcept
{
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	SubStates::wideQuery(control, event);
}

#if HFSM2_PLANS_AVAILABLE()

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
Status
O_<TN, TA, TH, TS...>::deepUpdatePlans(FullControl& control) noexcept {
	const Status h =	headStatus(control) |
		HeadState::deepUpdatePlans(control);

	const Status s =	 subStatus(control) |
		SubStates::wideUpdatePlans(control);

	if (h)
		return h;
	else {
		if (s.outerTransition)
			return Status{Status::Result::NONE, true};

		ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

		const bool planExists = control._core.planData.planExists.template get<REGION_ID>();

		return s && planExists ?
			control.updatePlan(static_cast<HeadState&>(*this), s) : s;
	}
}

#endif

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
bool
O_<TN, TA, TH, TS...>::deepForwardExitGuard(GuardControl& control) noexcept {
	const ProngCBits requested = orthoRequested(static_cast<const GuardControl&>(control));

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested)
		return SubStates::wideForwardExitGuard(control, requested);
	else
		return SubStates::wideForwardExitGuard(control);
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
bool
O_<TN, TA, TH, TS...>::deepExitGuard(GuardControl& control) noexcept {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return HeadState::deepExitGuard(control) ||
		   SubStates::wideExitGuard(control);
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepExit(PlanControl& control) noexcept {
	SubStates::wideExit(control);
	HeadState::deepExit(control);
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepForwardActive(Control& control,
										 const Request request) noexcept
{
	HFSM2_ASSERT(control._core.registry.isActive(HEAD_ID));

	const ProngCBits requested = orthoRequested(static_cast<const Control&>(control));
	HFSM2_ASSERT(!!requested);

	SubStates::wideForwardActive(control, request, requested);
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepForwardRequest(Control& control,
										  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const ProngCBits requested = orthoRequested(static_cast<const Control&>(control));

	if (requested)
		SubStates::wideForwardRequest(control, request);
	else
		deepRequest					 (control, request);
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepRequest(Control& control,
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

	case TransitionType::SELECT:
		deepRequestSelect	(control, request);
		break;

#if HFSM2_UTILITY_THEORY_AVAILABLE()

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

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepRequestChange(Control& control,
										 const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	SubStates::wideRequestChange(control, request);
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepRequestRestart(Control& control,
										  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	SubStates::wideRequestRestart(control, request);
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepRequestResume(Control& control,
										 const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	SubStates::wideRequestResume(control, request);
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepRequestSelect(Control& control,
										 const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	SubStates::wideRequestSelect(control, request);
}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepRequestUtilize(Control& control,
										  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	SubStates::wideRequestUtilize(control, request);
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepRequestRandomize(Control& control,
											const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	SubStates::wideRequestRandomize(control, request);
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
O_<TN, TA, TH, TS...>::deepReportChange(Control& control) noexcept {
	const UP	  h = HeadState::deepReportChange(control);
	const Utility s = SubStates::wideReportChange(control);

	const Utility sub = s / WIDTH;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, INVALID_STATE_ID, sub);

	return {
		h.utility * sub,
		h.prong
	};
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
O_<TN, TA, TH, TS...>::deepReportUtilize(Control& control) noexcept {
	const UP	  h = HeadState::deepReportUtilize(control);
	const Utility s = SubStates::wideReportUtilize(control);

	const Utility sub = s / WIDTH;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, INVALID_STATE_ID, sub);

	return {
		h.utility * sub,
		h.prong
	};
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::Rank
O_<TN, TA, TH, TS...>::deepReportRank(Control& control) noexcept {
	return HeadState::wrapRank(control);
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::Utility
O_<TN, TA, TH, TS...>::deepReportRandomize(Control& control) noexcept {
	const Utility h = HeadState::wrapUtility(control);
	const Utility s = SubStates::wideReportRandomize(control);

	const Utility sub = s / WIDTH;

	HFSM2_LOG_RANDOM_RESOLUTION(control.context(), HEAD_ID, INVALID_STATE_ID, sub);

	return h * sub;
}

#endif

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepChangeToRequested(PlanControl& control) noexcept {
	SubStates::wideChangeToRequested(control);
}

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepSaveActive(const Registry& registry,
									  WriteStream& stream) const noexcept
{
	SubStates::wideSaveActive(registry, stream);
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepSaveResumable(const Registry& registry,
										 WriteStream& stream) const noexcept
{
	SubStates::wideSaveResumable(registry, stream);
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepLoadRequested(Registry& registry,
										 ReadStream& stream) const noexcept
{
	SubStates::wideLoadRequested(registry, stream);
}

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepLoadResumable(Registry& registry,
										 ReadStream& stream) const noexcept
{
	SubStates::wideLoadResumable(registry, stream);
}

#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepGetNames(const Long parent,
									const RegionType region,
									const Short depth,
									StructureStateInfos& stateInfos) const noexcept
{
	HeadState::deepGetNames(parent, region,			depth,	   stateInfos);
	SubStates::wideGetNames(stateInfos.count() - 1, depth + 1, stateInfos);
}

#endif

}
}
namespace hfsm2 {
namespace detail {

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
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
	using Activation		 = TActivation;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank				 = TRank;
	using Utility			 = TUtility;
	using RNG				 = TRNG;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using LoggerInterface	 = LoggerInterfaceT<FEATURE_TAG, Context HFSM2_IF_UTILITY_THEORY(, Utility)>;
#endif

	static constexpr Long SUBSTITUTION_LIMIT = NSubstitutionLimit;

#if HFSM2_PLANS_AVAILABLE()
	static constexpr Long TASK_CAPACITY		 = NTaskCapacity;
#endif

	using Payload			 = TPayload;
	using Transition		 = TransitionT<Payload>;

#if HFSM2_PLANS_AVAILABLE()
	using Task				 = TaskT<Payload>;
#endif

	/// @brief Set Context type
	/// @tparam T Context type for data shared between states and/or data interface between FSM and external code
	template <typename T>
	using ContextT			 = G_<FEATURE_TAG, T	  , Activation HFSM2_IF_UTILITY_THEORY(, Rank, Utility, RNG), SUBSTITUTION_LIMIT HFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

	/// @brief Select manual activation strategy
	using ManualActivation	 = G_<FEATURE_TAG, Context, Manual	   HFSM2_IF_UTILITY_THEORY(, Rank, Utility, RNG), SUBSTITUTION_LIMIT HFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Set Rank type
	/// @tparam T Rank type for 'TRank State::rank() const' method
	template <typename T>
	using RankT				 = G_<FEATURE_TAG, Context, Activation						   , T   , Utility, RNG , SUBSTITUTION_LIMIT HFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

	/// @brief Set Utility type
	/// @tparam T Utility type for 'TUtility State::utility() const' method
	template <typename T>
	using UtilityT			 = G_<FEATURE_TAG, Context, Activation						   , Rank, T      , RNG , SUBSTITUTION_LIMIT HFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

	/// @brief Set RNG type
	/// @tparam T RNG type used in 'Random' regions
	template <typename T>
	using RandomT			 = G_<FEATURE_TAG, Context, Activation						   , Rank, Utility, T   , SUBSTITUTION_LIMIT HFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

#endif

	/// @brief Set Substitution limit
	/// @tparam N Maximum number times 'guard()' methods can substitute their states for others
	template <Long N>
	using SubstitutionLimitN = G_<FEATURE_TAG, Context, Activation HFSM2_IF_UTILITY_THEORY(, Rank, Utility, RNG), N                  HFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

#if HFSM2_PLANS_AVAILABLE()

	/// @brief Set Task capacity
	/// @tparam N Maximum number of tasks across all plans
	template <Long N>
	using TaskCapacityN		 = G_<FEATURE_TAG, Context, Activation HFSM2_IF_UTILITY_THEORY(, Rank, Utility, RNG), SUBSTITUTION_LIMIT                , N             , Payload>;

#endif

	/// @brief Set Transition Payload type
	/// @tparam T Utility type for 'TUtility State::utility() const' method
	template <typename T>
	using PayloadT			 = G_<FEATURE_TAG, Context, Activation HFSM2_IF_UTILITY_THEORY(, Rank, Utility, RNG), SUBSTITUTION_LIMIT HFSM2_IF_PLANS(, TASK_CAPACITY), T      >;

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	struct UP final {
		HFSM2_CONSTEXPR(14)	UP(const Utility utility_ = Utility{1},
							   const Short prong_ = INVALID_SHORT)		noexcept
			: utility{utility_}
			, prong{prong_}
		{}

		Utility utility;
		Short prong;
	};

#endif

};

template <typename TConfig>
struct M_;

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct M_	   <G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>> final {
	using Cfg = G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>;

	static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

	using Context			= TContext;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Utility			= TUtility;
#endif

	using Payload			= TPayload;
	using Transition		= TransitionT<Payload>;

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using LoggerInterface	= typename Cfg::LoggerInterface;
#endif

	/// @brief Composite region ('changeTo<>()' into the region acts as 'restart<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Composite			 = CI_<Strategy::Composite,	   THead, TSubStates...>;

	/// @brief Headless composite region ('changeTo<>()' into the region acts as 'restart<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using CompositePeers	 = CI_<Strategy::Composite,    void,  TSubStates...>;

	/// @brief Resumable region ('changeTo<>()' into the region acts as 'resume<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Resumable			  = CI_<Strategy::Resumable,   THead, TSubStates...>;

	/// @brief Headless resumable region ('changeTo<>()' into the region acts as 'resume<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using ResumablePeers	  = CI_<Strategy::Resumable,   void,  TSubStates...>;

	/// @brief Selectable region ('changeTo<>()' into the region acts as 'select<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Selectable		  = CI_<Strategy::Selectable,  THead, TSubStates...>;

	/// @brief Headless selectable region ('changeTo<>()' into the region acts as 'select<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using SelectablePeers	  = CI_<Strategy::Selectable,  void,  TSubStates...>;

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Utilitarian region ('changeTo<>()' into the region acts as 'utilize<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Utilitarian		  = CI_<Strategy::Utilitarian, THead, TSubStates...>;

	/// @brief Headless utilitarian region ('changeTo<>()' into the region acts as 'utilize<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using UtilitarianPeers	  = CI_<Strategy::Utilitarian, void,  TSubStates...>;

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

	/// @brief Orthogonal region (when activated, activates all sub-states)
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Orthogonal		  = OI_<THead, TSubStates...>;

	/// @brief Headless orthogonal region (when activated, activates all sub-states)
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using OrthogonalPeers	  = OI_<void,  TSubStates...>;

	/// @brief Root ('changeTo<>()' into the root region acts as 'restart<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Root				  = RF_<Cfg, Composite  <THead, TSubStates...>>;

	/// @brief Headless root ('changeTo<>()' into the root region acts as 'restart<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using PeerRoot			  = RF_<Cfg, CompositePeers  <  TSubStates...>>;

	/// @brief Resumable root ('changeTo<>()' into the root region acts as 'resume<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using ResumableRoot		  = RF_<Cfg, Resumable  <THead, TSubStates...>>;

	/// @brief Headless resumable root ('changeTo<>()' into the root region acts as 'resume<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using ResumablePeerRoot	  = RF_<Cfg, ResumablePeers  <  TSubStates...>>;

	/// @brief Selectable root ('changeTo<>()' into the region acts as 'select<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using SelectableRoot	  = RF_<Cfg, Selectable <THead, TSubStates...>>;

	/// @brief Headless selectable root ('changeTo<>()' into the region acts as 'select<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using SelectablePeerRoot  = RF_<Cfg, SelectablePeers <  TSubStates...>>;

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Utilitarian root ('changeTo<>()' into the root region acts as 'utilize<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using UtilitarianRoot	  = RF_<Cfg, Utilitarian<THead, TSubStates...>>;

	/// @brief Headless utilitarian root ('changeTo<>()' into the root region acts as 'utilize<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using UtilitarianPeerRoot = RF_<Cfg, UtilitarianPeers<  TSubStates...>>;

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

	/// @brief Orthogonal root (all sub-states are active at all times)
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using OrthogonalRoot	  = RF_<Cfg, Orthogonal <THead, TSubStates...>>;

	/// @brief Headless orthogonal root (all sub-states are active at all times)
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using OrthogonalPeerRoot  = RF_<Cfg, OrthogonalPeers <  TSubStates...>>;

};

}

/// @brief Type configuration for MachineT<>
using Config = detail::G_<HFSM2_FEATURE_TAG, EmptyContext, Automatic HFSM2_IF_UTILITY_THEORY(, int8_t, float, RNGT<float>), 4 HFSM2_IF_PLANS(, INVALID_LONG), void>;

/// @brief 'Template namespace' for FSM classes
/// @tparam TConfig 'ConfigT<>' type configuration for MachineT<>
/// @see ConfigT<>
template <typename TConfig = Config>
using MachineT = detail::M_<TConfig>;

/// @brief 'Template namespace' for FSM classes parametrized with default types
using Machine = MachineT<>;

}


namespace hfsm2 {
namespace detail {

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename TConfig,
		  typename TApex>
class R_ {
public:
	static constexpr FeatureTag FEATURE_TAG = TConfig::FEATURE_TAG;

	using Context				= typename TConfig::Context;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank					= typename TConfig::Rank;
	using Utility				= typename TConfig::Utility;
	using RNG					= typename TConfig::RNG;
#endif

	using Payload				= typename TConfig::Payload;

protected:
	using Forward				= RF_<TConfig, TApex>;
	using StateList				= typename Forward::StateList;
	using RegionList			= typename Forward::RegionList;

	static constexpr Long STATE_COUNT	= StateList ::SIZE;
	static constexpr Long REGION_COUNT	= RegionList::SIZE;

	using Args					= typename Forward::Args;
	using PureContext			= typename Args::PureContext;

	static_assert(Args::STATE_COUNT	   < static_cast<unsigned>(-1), "Too many states in the FSM. Change 'Short' type.");
	static_assert(Args::STATE_COUNT	  == static_cast<unsigned>(StateList::SIZE), "STATE_COUNT != StateList::SIZE");
	static_assert(Args::COMPO_REGIONS >= 1, "State machine needs at least one composite region");

	using Core					= CoreT<Args>;

	using Apex					= MaterialT<I_<0, 0, 0, 0>, Args, TApex>;

	using Registry				= RegistryT<Args>;
	using CompoForks			= typename Registry::CompoForks;
	using RegistryBackUp		= typename Registry::BackUp;

	using ConstControl			= ConstControlT<Args>;
	using Control				= ControlT	   <Args>;
	using PlanControl			= PlanControlT <Args>;
	using FullControl			= FullControlT <Args>;
	using GuardControl			= GuardControlT<Args>;

	static constexpr Long	SUBSTITUTION_LIMIT	= Forward::SUBSTITUTION_LIMIT;

#if HFSM2_PLANS_AVAILABLE()
	using PlanData				= PlanDataT<Args>;
#endif

public:
#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream			= typename Args::WriteStream;
	using ReadStream			= typename Args::ReadStream;
#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	static constexpr Long NAME_COUNT = Apex::NAME_COUNT;
#endif

public:
	using Info					= WrapInfo<TApex>;

	/// @brief Transition
	using Transition			= typename Control::Transition;

	/// @brief Array of transitions
	using TransitionSet			= typename Control::TransitionSet;
	using TransitionSets		= typename Control::TransitionSets;

#if HFSM2_PLANS_AVAILABLE()
	using CPlan					= CPlanT<Args>;
	using Plan					=  PlanT<Args>;

	static constexpr Long TASK_CAPACITY = Forward::TASK_CAPACITY;
#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	using TransitionTargets		= typename Control::TransitionTargets;
#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using Prefix				= StaticArrayT<wchar_t, Info::REVERSE_DEPTH * 2 + 2>;
	using Prefixes				= StaticArrayT<Prefix, STATE_COUNT>;

	using NamedStates			= BitArrayT<STATE_COUNT>;

	using StructureStateInfos	= typename Args::StructureStateInfos;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger				= typename TConfig::LoggerInterface;
#endif

public:

	HFSM2_CONSTEXPR(14)	explicit R_(Context& context
								  HFSM2_IF_UTILITY_THEORY(, RNG& rng)
								  HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))		noexcept;

	HFSM2_CONSTEXPR(14)	explicit R_(PureContext&& context
								  HFSM2_IF_UTILITY_THEORY(, RNG& rng)
								  HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))		noexcept;

	HFSM2_CONSTEXPR(NO) R_(const R_& )															noexcept = default;
	HFSM2_CONSTEXPR(NO) R_(		 R_&&)															noexcept = default;

	HFSM2_CONSTEXPR(20)	~R_()																	noexcept;

	/// @brief Access context
	/// @return context
	HFSM2_CONSTEXPR(14)		  Context& context()												noexcept	{ return _core.context;		}

	/// @brief Access context
	/// @return context
	HFSM2_CONSTEXPR(11)	const Context& context()										  const noexcept	{ return _core.context;		}

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()															noexcept	{ return					   index<StateList , TState>() ;	}

	/// @brief Get region identifier for a region type
	/// @tparam TState Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static constexpr RegionID regionId()														noexcept	{ return static_cast<RegionID>(index<RegionList, TState>());	}

	/// @brief Access state instance
	/// @tparam TState State type
	/// @return State instance
	template <typename TState>
	HFSM2_CONSTEXPR(14)		  TState& access()													noexcept	{ return static_cast<	   TState&>(_apex);		}

	/// @brief Access state instance
	/// @tparam TState State type
	/// @return State instance
	template <typename TState>
	HFSM2_CONSTEXPR(11)	const TState& access()											  const noexcept	{ return static_cast<const TState&>(_apex);		}

	/// @brief Trigger FSM update cycle (recursively call 'update()' from the root down to the leaf states,
	///   on all active states, then process requested transitions)
	HFSM2_CONSTEXPR(14)	void update()															noexcept;

	/// @brief Have FSM react to an event (recursively call matching 'react<>()' from the root down to the leaf states,
	///   on all active states, then process requested transitions)
	/// @tparam TEvent Event type
	/// @param event Event to react to
	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void react(const TEvent& event)											noexcept;

	/// @brief Recursively call 'query()' from the root down to the leaf states, on all active states
	/// @tparam TEvent Event type
	/// @param event Event to react to
	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void query(TEvent& event)										  const noexcept;

	/// @brief Get region's active sub-state's index
	/// @param stateId Region's head state ID
	/// @return Region's active sub-state index
	HFSM2_CONSTEXPR(14)	Short activeSubState (const StateID stateId_)					  const noexcept	{ return _core.registry.activeSubState(stateId_			);	}

	/// @brief Get region's active sub-state's index
	/// @tparam TState Region's head state type
	/// @return Region's active sub-state index
	template <typename TState>
	HFSM2_CONSTEXPR(14)	Short activeSubState ()											  const noexcept	{ return _core.registry.activeSubState(stateId<TState>());	}

	/// @brief Check if a state is active
	/// @param stateId Destination state identifier
	/// @return State active status
	HFSM2_CONSTEXPR(11)	bool isActive		 (const StateID stateId_)					  const noexcept	{ return _core.registry.isActive   (stateId_);			}

	/// @brief Check if a state is active
	/// @tparam TState Destination state type
	/// @return State active status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isActive		 ()											  const noexcept	{ return isActive	(stateId<TState>());				}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @param stateId Destination state identifier
	/// @return State resumable status
	HFSM2_CONSTEXPR(11)	bool isResumable	 (const StateID stateId_)					  const noexcept	{ return _core.registry.isResumable(stateId_);			}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @tparam TState Destination state type
	/// @return State resumable status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isResumable	 ()											  const noexcept	{ return isResumable(stateId<TState>());				}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @param stateId Destination state identifier
	/// @return State scheduled status
	HFSM2_CONSTEXPR(11)	bool isScheduled	 (const StateID stateId_)					  const noexcept	{ return isResumable(stateId_);							}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @tparam TState Destination state type
	/// @return State scheduled status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isScheduled	 ()											  const noexcept	{ return isResumable<TState>();							}

#if HFSM2_PLANS_AVAILABLE()

	/// @brief Access read-only plan for the root region
	/// @return Plan for the root region
	HFSM2_CONSTEXPR(14)  Plan plan()															noexcept	{ return  Plan{_core.registry, _core.planData, 0				  };	}

	/// @brief Access plan for a region
	/// @param regionId Region ID
	/// @return Plan for the region
	HFSM2_CONSTEXPR(14)  Plan plan(const RegionID regionId_)									noexcept	{ return  Plan{_core.registry, _core.planData, regionId_		  };	}

	/// @brief Access plan for a region
	/// @tparam TRegion Region head state type
	/// @return Plan for the region
	template <typename TRegion>
	HFSM2_CONSTEXPR(14)  Plan plan()															noexcept	{ return  Plan{_core.registry, _core.planData, regionId<TRegion>()};	}

	/// @brief Access plan for the root region
	/// @return Plan for the root region
	HFSM2_CONSTEXPR(11)	CPlan plan()													  const noexcept	{ return CPlan{_core.registry, _core.planData, 0				  };	}

	/// @brief Access plan for a region
	/// @param regionId Region ID
	/// @return Plan for the region
	HFSM2_CONSTEXPR(11)	CPlan plan(const RegionID regionId_)							  const noexcept	{ return CPlan{_core.registry, _core.planData, regionId_		  };	}

	/// @brief Access plan for a region
	/// @tparam TRegion Region head state type
	/// @return Plan for the region
	template <typename TRegion>
	HFSM2_CONSTEXPR(11)	CPlan plan()													  const noexcept	{ return CPlan{_core.registry, _core.planData, regionId<TRegion>()};	}

	/// @brief Succeed a plan task for a state
	/// @param stateId State ID
	HFSM2_CONSTEXPR(14) void succeed(const StateID stateId_)									noexcept;

	/// @brief Succeed a plan task for a state
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14) void succeed()															noexcept	{ succeed(stateId<TState>());							}

	/// @brief Fail a plan task for a state
	/// @param stateId State ID
	HFSM2_CONSTEXPR(14) void fail	(const StateID stateId_)									noexcept;

	/// @brief Fail a plan task for a state
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14) void fail	()															noexcept	{ fail	 (stateId<TState>());							}

#endif

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	HFSM2_CONSTEXPR(14)	void changeTo			(const StateID stateId_)						noexcept;

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void changeTo			()												noexcept	{ changeTo			(stateId<TState>());				}

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	HFSM2_CONSTEXPR(14)	void restart			(const StateID stateId_)						noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void restart			()												noexcept	{ restart			(stateId<TState>());				}

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	HFSM2_CONSTEXPR(14)	void resume				(const StateID stateId_)						noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void resume				()												noexcept	{ resume			(stateId<TState>());				}

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @param stateId Destination state identifier
	HFSM2_CONSTEXPR(14)	void select				(const StateID stateId_)						noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void select				()												noexcept	{ select			(stateId<TState>());				}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void utilize			(const StateID stateId_)						noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void utilize			()												noexcept	{ utilize			(stateId<TState>());				}

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void randomize			(const StateID stateId_)						noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void randomize			()												noexcept	{ randomize			(stateId<TState>());				}

#endif

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId Destination state identifier
	HFSM2_CONSTEXPR(14)	void schedule			(const StateID stateId_)						noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void schedule			()												noexcept	{ schedule			(stateId<TState>());				}

	/// @brief Transition into a state
	///   (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	HFSM2_CONSTEXPR(14)	void immediateChangeTo	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateChangeTo	()												noexcept	{ immediateChangeTo	(stateId<TState>());				}

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	HFSM2_CONSTEXPR(14)	void immediateRestart	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateRestart	()												noexcept	{ immediateRestart	(stateId<TState>());				}

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	HFSM2_CONSTEXPR(14)	void immediateResume	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateResume	()												noexcept	{ immediateResume	(stateId<TState>());				}

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @param stateId Destination state identifier
	HFSM2_CONSTEXPR(14)	void immediateSelect	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateSelect	()												noexcept	{ immediateSelect	(stateId<TState>());				}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void immediateUtilize	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateUtilize	()												noexcept	{ immediateUtilize	(stateId<TState>());				}

	/// @brief Transition into a state
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void immediateRandomize	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateRandomize	()												noexcept	{ immediateRandomize(stateId<TState>());				}

#endif

	/// @brief Check if a state is going to be activated or deactivated
	/// @param stateId Destination state identifier
	/// @return State pending activation/deactivation status
	HFSM2_CONSTEXPR(11)	bool isPendingChange	(const StateID stateId_)				  const noexcept	{ return _core.registry.isPendingChange(stateId_);		}

	/// @brief Check if a state is going to be activated or deactivated
	/// @tparam TState Destination state type
	/// @return State pending activation/deactivation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingChange	()										  const noexcept	{ return isPendingChange(stateId<TState>());			}

	/// @brief Check if a state is going to be activated
	/// @param stateId Destination state identifier
	/// @return State pending activation status
	HFSM2_CONSTEXPR(11)	bool isPendingEnter		(const StateID stateId_)				  const noexcept	{ return _core.registry.isPendingEnter (stateId_);		}

	/// @brief Check if a state is going to be activated
	/// @tparam TState Destination state type
	/// @return State pending activation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingEnter		()										  const noexcept	{ return isPendingEnter (stateId<TState>());			}

	/// @brief Check if a state is going to be deactivated
	/// @param stateId Destination state identifier
	/// @return State pending deactivation status
	HFSM2_CONSTEXPR(11)	bool isPendingExit		(const StateID stateId_)				  const noexcept	{ return _core.registry.isPendingExit  (stateId_);		}

	/// @brief Check if a state is going to be deactivated
	/// @tparam TState Destination state type
	/// @return State pending deactivation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingExit		()										  const noexcept	{ return isPendingExit  (stateId<TState>());			}

	/// @brief Reset FSM to initial state (recursively 'exit()' currently active states, 'enter()' initial states)
	HFSM2_CONSTEXPR(14)	void reset()															noexcept;

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get the list of transitions recorded during last 'update()'
	/// @return Array of last recorded transitions
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	HFSM2_CONSTEXPR(11)	const TransitionSets& previousTransitions()						  const noexcept	{ return _core.previousTransitions;						}

	/// @brief Force process transitions (skips 'guard()' calls)
	///  Can be used to synchronize multiple FSMs
	/// @param transitions 'TransitionHistory' to replay
	/// @param count Number of transitions
	/// @return Success status
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	HFSM2_CONSTEXPR(14)	bool replayTransitions(const Transition* const transitions,
											   const Short count)								noexcept;

	/// @brief Force process transitions (skips 'guard()' calls)
	///  Can be used to synchronize multiple FSMs
	/// @param transitions Array of 'Transition' to replay
	/// @return Success status
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	template <Long NCount>
	HFSM2_CONSTEXPR(14)	bool replayTransitions(const ArrayT<Transition, NCount>& transitions)	noexcept;

	/// @brief Force process a transition (skips 'guard()' calls)
	///  Can be used to synchronize multiple FSMs
	/// @param transition 'Transition' to replay
	/// @return Success status
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	HFSM2_CONSTEXPR(14)	bool replayTransition (const Transition& transition)					noexcept	{ return replayTransitions(&transition, 1);				}

	/// @brief Get the last transition that caused the state to be activated
	/// @param stateId State identifier
	/// @return Pointer to the last transition that activated the state
	HFSM2_CONSTEXPR(14)	const Transition* lastTransitionTo(const StateID stateId_)		  const noexcept;

	/// @brief Get the last transition that caused the state to be activated
	/// @tparam TState State type
	/// @return Pointer to the last transition that activated the state
	template <typename TState>
	HFSM2_CONSTEXPR(14)	const Transition* lastTransitionTo()							  const noexcept	{ return lastTransitionTo(stateId<TState>());			}

#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

	/// @brief Array of 'StructureEntry' representing FSM structure
	/// @see HFSM2_ENABLE_STRUCTURE_REPORT
	using Structure				= ArrayT<StructureEntry, NAME_COUNT>;

	/// @brief Array of 'int8_t' representing FSM activation history (negative - 'update()' cycles since deactivated, positive - 'update()' cycles since activated)
	/// @see HFSM2_ENABLE_STRUCTURE_REPORT
	using ActivityHistory		= ArrayT<int8_t,		 NAME_COUNT>;

	/// @brief Get the array of 'StructureEntry' representing FSM structure
	/// @return FSM structure
	/// @see HFSM2_ENABLE_STRUCTURE_REPORT
	HFSM2_CONSTEXPR(11)	const Structure&	   structure()								  const noexcept	{ return _structure;									}

	/// @brief Get the array of 'int8_t' representing FSM activation history (negative - 'update()' cycles since deactivated, positive - 'update()' cycles since activated)
	/// @return FSM activation history
	/// @see HFSM2_ENABLE_STRUCTURE_REPORT
	HFSM2_CONSTEXPR(11)	const ActivityHistory& activityHistory()						  const noexcept	{ return _activityHistory;								}

#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()

	/// @brief Attach logger
	/// @param logger A logger implementing 'hfsm2::LoggerInterfaceT<>' interface
	/// @see HFSM2_ENABLE_LOG_INTERFACE
	HFSM2_CONSTEXPR(14)	void attachLogger(Logger* const logger)									noexcept	{ _core.logger = logger;								}

#endif

protected:
	HFSM2_CONSTEXPR(14)	void initialEnter()														noexcept;
	HFSM2_CONSTEXPR(14)	void finalExit()														noexcept;

	HFSM2_CONSTEXPR(14)	void processRequest()													noexcept;
	HFSM2_CONSTEXPR(14)	void processTransitions(TransitionSets& currentTransitions)				noexcept;

	HFSM2_CONSTEXPR(14)	bool applyRequest(const TransitionSets& currentTransitions,
										  Control& control,
										  const Transition& request,
										  const Short index)									noexcept;

	HFSM2_CONSTEXPR(14)	bool applyRequests(const TransitionSets& currentTransitions,
										   Control& control)									noexcept;

	HFSM2_CONSTEXPR(14)	bool cancelledByEntryGuards(const TransitionSets& currentTransitions,
													const TransitionSet&  pendingTransitions)	noexcept;

	HFSM2_CONSTEXPR(14)	bool cancelledByGuards(const TransitionSets& currentTransitions,
											   const TransitionSet&  pendingTransitions)		noexcept;

#if HFSM2_SERIALIZATION_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void save(WriteStream& stream)									  const noexcept;
	HFSM2_CONSTEXPR(14)	void load( ReadStream& stream)											noexcept;
#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	bool applyRequests(Control& control,
										   const Transition* const transitions,
										   const Short count)									noexcept;
#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void getStateNames()													noexcept;
	HFSM2_CONSTEXPR(14)	void udpateActivity()													noexcept;

	Prefixes _prefixes;
	NamedStates _namedStates;

	Structure _structure;
	ActivityHistory _activityHistory;
#endif

protected:
	Core _core;
	Apex _apex;
};

// Automatic / manual [de]activation

template <typename, typename>
class RV_;

// Automatic enter() / exit()

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RV_		   <G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 R_<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = R_<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

protected:
	using typename Base::Context;
	using typename Base::PureContext;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

#if HFSM2_SERIALIZATION_AVAILABLE()
	using typename Base::Args;
	using typename Base::WriteStream;
	using typename Base::ReadStream;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	HFSM2_CONSTEXPR(14)	explicit RV_(Context& context
								   HFSM2_IF_UTILITY_THEORY(, RNG& rng)
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	HFSM2_CONSTEXPR(14)	explicit RV_(PureContext&& context
								   HFSM2_IF_UTILITY_THEORY(, RNG& rng)
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	HFSM2_CONSTEXPR(14)	RV_(const RV_&  other)													noexcept;
	HFSM2_CONSTEXPR(14)	RV_(	  RV_&& other)													noexcept;

	HFSM2_CONSTEXPR(20)	~RV_()																	noexcept;

#if HFSM2_SERIALIZATION_AVAILABLE()

	/// @brief Buffer for serialization
	/// @see `HFSM2_ENABLE_SERIALIZATION`
	using SerialBuffer			= typename Args::SerialBuffer;

	/// @brief Serialize FSM into 'buffer'
	/// @param buffer `SerialBuffer` to serialize to
	/// @see `HFSM2_ENABLE_SERIALIZATION`
	HFSM2_CONSTEXPR(14)	void save(		SerialBuffer& buffer)							  const noexcept;

	/// @brief De-serialize FSM from 'buffer'
	/// @param buffer `SerialBuffer` to de-serialize from
	/// @see `HFSM2_ENABLE_SERIALIZATION`
	HFSM2_CONSTEXPR(14)	void load(const SerialBuffer& buffer)									noexcept;

#endif

private:
#if HFSM2_SERIALIZATION_AVAILABLE()
	using Base::save;
	using Base::load;
#endif

private:
	using Base::initialEnter;
	using Base::finalExit;
};

// Manual enter() / exit()

template <FeatureTag NFeatureTag
		, typename TContext

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RV_		   <G_<NFeatureTag, TContext, Manual HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 R_<G_<NFeatureTag, TContext, Manual HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = R_<G_<NFeatureTag, TContext, Manual HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	using typename Base::Transition;

protected:
	HFSM2_IF_UTILITY_THEORY(using typename Base::RNG);

#if HFSM2_SERIALIZATION_AVAILABLE()
	using typename Base::PlanControl;
	using typename Base::TransitionSets;

	using typename Base::Args;
	using typename Base::WriteStream;
	using typename Base::ReadStream;
#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	using typename Base::PlanControl;
	using typename Base::TransitionSets;
#endif

public:
	using Base::Base;

	/// @brief Check if FSM is active
	/// @return FSM active status
	HFSM2_CONSTEXPR(11)	bool isActive()												  const noexcept	{ return _core.registry.isActive();	}

	using Base::isActive;

	/// @brief Manually start the FSM
	///  Can be used with UE4 to start / stop the FSM in BeginPlay() / EndPlay()
	HFSM2_CONSTEXPR(14)	void enter()														noexcept	{ initialEnter();	}

	/// @brief Manually stop the FSM
	///  Can be used with UE4 to start / stop the FSM in BeginPlay() / EndPlay()
	HFSM2_CONSTEXPR(14)	void exit()															noexcept	{ finalExit();		}

#if HFSM2_SERIALIZATION_AVAILABLE()

	/// @brief Buffer for serialization
	/// @see `HFSM2_ENABLE_SERIALIZATION`
	using SerialBuffer			= typename Args::SerialBuffer;

	/// @brief Serialize FSM into 'buffer'
	/// @param buffer `SerialBuffer` to serialize to
	/// @see `HFSM2_ENABLE_SERIALIZATION`
	HFSM2_CONSTEXPR(14)	void save(		SerialBuffer& buffer)							  const noexcept;

	/// @brief De-serialize FSM from 'buffer'
	/// @param buffer `SerialBuffer` to de-serialize from
	/// @see `HFSM2_ENABLE_SERIALIZATION`
	HFSM2_CONSTEXPR(14)	void load(const SerialBuffer& buffer)									noexcept;

#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Start the FSM from a specific state
	///  Can be used to synchronize multiple FSMs
	/// @param transitions 'TransitionHistory' to replay
	/// @param count Number of transitions
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	HFSM2_CONSTEXPR(14)	bool replayEnter(const Transition* const transitions,
										 const Short count)									noexcept;

	/// @brief Start the FSM from a specific state
	///  Can be used to synchronize multiple FSMs
	/// @param transitions Array of 'Transition' to replay
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	template <Long NCount>
	HFSM2_CONSTEXPR(14)	bool replayEnter(const ArrayT<Transition, NCount>& transitions)		noexcept;

	/// @brief Start the FSM from a specific state
	///  Can be used to synchronize multiple FSMs
	/// @param transition 'Transition' to replay
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	HFSM2_CONSTEXPR(14)	bool replayEnter(const Transition& transition)						noexcept	{ return replayEnter(&transition, 1);	}

#endif

private:
#if HFSM2_SERIALIZATION_AVAILABLE()
	using Base::save;
	using Base::load;

	HFSM2_CONSTEXPR(14)	void loadEnter(ReadStream& stream)										noexcept;
#endif

protected:
	using Base::initialEnter;
	using Base::finalExit;

	using Base::_core;

#if HFSM2_SERIALIZATION_AVAILABLE() || HFSM2_TRANSITION_HISTORY_AVAILABLE()
	using Base::_apex;
#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	using Base::applyRequests;
	HFSM2_IF_STRUCTURE_REPORT(using Base::udpateActivity);
#endif
};

template <typename TConfig,
		  typename TApex>
class RP_;

// Non-'void' payloads

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RP_			<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RV_<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RV_<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

	using Transition = TransitionT<TPayload>;

public:
	using typename Base::Payload;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

public:
	using Base::Base;
	using Base::processRequest;

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID  stateId()												noexcept	{ return Base::template stateId<TState>();				}

	/// @brief Get region identifier for a region type
	/// @tparam TState Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static constexpr RegionID regionId()											noexcept	{ return Base::template regionId<TState>();				}

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void changeWith				(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void changeWith				(const Payload& payload)		noexcept	{ changeWith			(stateId<TState>(), payload );	}

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void restartWith			(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void restartWith			(const Payload& payload)		noexcept	{ restartWith			(stateId<TState>(), payload );	}

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void resumeWith				(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void resumeWith				(const Payload& payload)		noexcept	{ resumeWith			(stateId<TState>(), payload );	}

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void selectWith				(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void selectWith				(const Payload& payload)		noexcept	{ selectWith			(stateId<TState>(), payload );	}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void utilizeWith			(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void utilizeWith			(const Payload& payload)		noexcept	{ utilizeWith			(stateId<TState>(), payload );	}

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void randomizeWith			(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void randomizeWith			(const Payload& payload)		noexcept	{ randomizeWith			(stateId<TState>(), payload );	}

#endif

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void scheduleWith			(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void scheduleWith			(const Payload& payload)		noexcept	{ scheduleWith			(stateId<TState>(), payload );	}

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void immediateChangeWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateChangeWith	(const Payload& payload)		noexcept	{ immediateChangeWith   (stateId<TState>(), payload );	}

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void immediateRestartWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateRestartWith	(const Payload& payload)		noexcept	{ immediateRestartWith	(stateId<TState>(), payload );	}

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void immediateResumeWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateResumeWith	(const Payload& payload)		noexcept	{ immediateResumeWith	(stateId<TState>(), payload );	}

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void immediateSelectWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateSelectWith	(const Payload& payload)		noexcept	{ immediateSelectWith	(stateId<TState>(), payload );	}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void immediateUtilizeWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateUtilizeWith	(const Payload& payload)		noexcept	{ immediateUtilizeWith	(stateId<TState>(), payload );	}

	/// @brief Transition into a state
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void immediateRandomizeWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateRandomizeWith	(const Payload& payload)		noexcept	{ immediateRandomizeWith(stateId<TState>(), payload );	}

#endif

protected:
	using Base::_core;
};

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TApex>
class RP_			<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>
	: public	 RV_<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>
{
	using Base = RV_<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>;

public:
#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

public:
	using Base::Base;
};

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename, typename>
class RC_;

// TContext

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RC_			<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RP_<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RP_<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;
	using typename Base::PureContext;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	HFSM2_CONSTEXPR(14)	explicit RC_(Context& context
								   HFSM2_IF_UTILITY_THEORY(, RNG& rng)
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	HFSM2_CONSTEXPR(14)	explicit RC_(PureContext&& context
								   HFSM2_IF_UTILITY_THEORY(, RNG& rng)
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	HFSM2_CONSTEXPR(NO) RC_(const RC_& )														noexcept = default;
	HFSM2_CONSTEXPR(NO) RC_(	  RC_&&)														noexcept = default;

private:
	using Base::_core;
};

// TContext&

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RC_			<G_<NFeatureTag, TContext&, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RP_<G_<NFeatureTag, TContext&, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RP_<G_<NFeatureTag, TContext&, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	using Base::Base;

private:
	using Base::_core;
};

// TContext*

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RC_			<G_<NFeatureTag, TContext*, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RP_<G_<NFeatureTag, TContext*, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RP_<G_<NFeatureTag, TContext*, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
#if HFSM2_UTILITY_THEORY_AVAILABLE()

	HFSM2_CONSTEXPR(14)	explicit RC_(Context context
								   , RNG& rng
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

#else

	HFSM2_CONSTEXPR(14)	explicit RC_(Context context = nullptr
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

#endif

	HFSM2_CONSTEXPR(NO)	RC_(const RC_& )														noexcept = default;
	HFSM2_CONSTEXPR(NO)	RC_(	  RC_&&)														noexcept = default;

	HFSM2_CONSTEXPR(14)	void setContext(Context context)										noexcept	{ _core.context = context; }

private:
	using Base::_core;
};

// TContext == EmptyContext

template <FeatureTag NFeatureTag
		, typename TActivation

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RC_			<G_<NFeatureTag, EmptyContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RP_<G_<NFeatureTag, EmptyContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	, public EmptyContext
{
	using Base = RP_<G_<NFeatureTag, EmptyContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	HFSM2_CONSTEXPR(14)	explicit RC_(RNG& rng
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

#else

	HFSM2_CONSTEXPR(14)	explicit RC_(HFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr))	noexcept;

#endif

	HFSM2_CONSTEXPR(NO)	RC_(const RC_& )														noexcept = default;
	HFSM2_CONSTEXPR(NO)	RC_(	  RC_&&)														noexcept = default;
};

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename TConfig,
		  typename TApex>
class InstanceT final
	: public	 RC_<TConfig, TApex>
{
	using Base = RC_<TConfig, TApex>;

public:
	using Base::Base;
};

#if HFSM2_UTILITY_THEORY_AVAILABLE()

// TRNG == RNGT<TUtility>

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation
		, typename TRank
		, typename TUtility
		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class InstanceT	    <G_<NFeatureTag, TContext, TActivation, TRank, TUtility, RNGT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex> final
	: public	 RC_<G_<NFeatureTag, TContext, TActivation, TRank, TUtility, RNGT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	, public RNGT<TUtility>
{
	using Base = RC_<G_<NFeatureTag, TContext, TActivation, TRank, TUtility, RNGT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	HFSM2_CONSTEXPR(14)	explicit InstanceT(Context& context
										 HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept
		: Base{context
			 , static_cast<RNGT<TUtility>&>(*this)
			 HFSM2_IF_LOG_INTERFACE(, logger)}
		, RNGT<TUtility>{0}
	{}
};

// TContext == EmptyContext
// TRNG == RNGT<TUtility>

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TActivation
		, typename TRank
		, typename TUtility
		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class InstanceT		<G_<NFeatureTag, EmptyContext, TActivation, TRank, TUtility, RNGT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex> final
	: public	 RC_<G_<NFeatureTag, EmptyContext, TActivation, TRank, TUtility, RNGT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	, public RNGT<TUtility>
{
	using Base = RC_<G_<NFeatureTag, EmptyContext, TActivation, TRank, TUtility, RNGT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	HFSM2_CONSTEXPR(14)	explicit InstanceT(HFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr))	noexcept
		: Base{static_cast<RNGT<TUtility>&>(*this)
			 HFSM2_IF_LOG_INTERFACE(, logger)}
		, RNGT<TUtility>{0}
	{}
};

#endif

}
}


namespace hfsm2 {
namespace detail {

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
R_<TG, TA>::R_(Context& context
			 HFSM2_IF_UTILITY_THEORY(, RNG& rng)
			 HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: _core{context
		  HFSM2_IF_UTILITY_THEORY(, rng)
		  HFSM2_IF_LOG_INTERFACE(, logger)}
{
	_apex.deepRegister(_core.registry, Parent{});

	HFSM2_IF_STRUCTURE_REPORT(getStateNames());
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
R_<TG, TA>::R_(PureContext&& context
			 HFSM2_IF_UTILITY_THEORY(, RNG& rng)
			 HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: _core{move(context)
		  HFSM2_IF_UTILITY_THEORY(, rng)
		  HFSM2_IF_LOG_INTERFACE(, logger)}
{
	_apex.deepRegister(_core.registry, Parent{});

	HFSM2_IF_STRUCTURE_REPORT(getStateNames());
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(20)
R_<TG, TA>::~R_() noexcept {
	HFSM2_IF_ASSERT(HFSM2_IF_PLANS(_core.planData.verifyPlans()));
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::update() noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	TransitionSets emptyTransitions;
	FullControl control{_core, emptyTransitions};

	_apex. deepPreUpdate(control);
	_apex.	  deepUpdate(control);
	_apex.deepPostUpdate(control);

#if HFSM2_PLANS_AVAILABLE()
	_apex.deepUpdatePlans(control);
	_core.planData.clearRegionStatuses();
#endif

	processRequest();
}

template <typename TG, typename TA>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::react(const TEvent& event) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	TransitionSets emptyTransitions;
	FullControl control{_core, emptyTransitions};

	_apex. deepPreReact(control, event);
	_apex.	  deepReact(control, event);
	_apex.deepPostReact(control, event);

#if HFSM2_PLANS_AVAILABLE()
	_apex.deepUpdatePlans(control);
	_core.planData.clearRegionStatuses();
#endif

	processRequest();
}

template <typename TG, typename TA>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::query(TEvent& event) const noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	ConstControl control{_core};

	_apex.deepQuery(control, event);
}

#if HFSM2_PLANS_AVAILABLE()

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::succeed(const StateID stateId_) noexcept {
	_core.planData.tasksSuccesses.set(stateId_);

	HFSM2_LOG_TASK_STATUS(_core.context, INVALID_REGION_ID, stateId_, StatusEvent::SUCCEEDED);
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::fail(const StateID stateId_) noexcept {
	_core.planData.tasksFailures.set(stateId_);

	HFSM2_LOG_TASK_STATUS(_core.context, INVALID_REGION_ID, stateId_, StatusEvent::FAILED);
}

#endif

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::changeTo(const StateID stateId_) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::CHANGE});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::CHANGE, stateId_);
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::restart(const StateID stateId_) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::RESTART});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::RESTART, stateId_);
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::resume(const StateID stateId_) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::RESUME});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::RESUME, stateId_);
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::select(const StateID stateId_) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::SELECT});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::SELECT, stateId_);
}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::utilize(const StateID stateId_) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::UTILIZE});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::UTILIZE, stateId_);
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::randomize(const StateID stateId_) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::RANDOMIZE});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::RANDOMIZE, stateId_);
}

#endif

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::schedule(const StateID stateId_) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::SCHEDULE});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::SCHEDULE, stateId_);
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::immediateChangeTo(const StateID stateId_) noexcept {
	changeTo(stateId_);

	processRequest();
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::immediateRestart(const StateID stateId_) noexcept {
	restart(stateId_);

	processRequest();
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::immediateResume(const StateID stateId_) noexcept {
	resume(stateId_);

	processRequest();
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::immediateSelect(const StateID stateId_) noexcept {
	select(stateId_);

	processRequest();
}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::immediateUtilize(const StateID stateId_) noexcept {
	utilize(stateId_);

	processRequest();
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::immediateRandomize(const StateID stateId_) noexcept {
	randomize(stateId_);

	processRequest();
}

#endif

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::reset() noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	TransitionSets currentTransitions;
	PlanControl control{_core, currentTransitions};

	_apex.deepExit(control);

	HFSM2_IF_TRANSITION_HISTORY(_core.transitionTargets.clear());
	HFSM2_IF_TRANSITION_HISTORY(_core.previousTransitions.clear());

	_core.registry.clear();
	// TODO: clear _core.requests
	// TODO: clear _core.rng			// HFSM2_IF_UTILITY_THEORY()
	// TODO: clear _core.planData		// HFSM2_IF_PLANS()
	// TODO: clear _activityHistory		// HFSM2_IF_STRUCTURE_REPORT()

	_apex.deepRequestChange(control, {TransitionType::RESTART, INVALID_SHORT});
	_apex.deepEnter(control);
}

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::replayTransitions(const Transition* const transitions,
							  const Short count) noexcept
{
	HFSM2_ASSERT(transitions);
	HFSM2_ASSERT(_core.registry.isActive());

	_core.transitionTargets.clear();
	_core.previousTransitions.clear();

	if (HFSM2_CHECKED(transitions && count)) {
		TransitionSets currentTransitions;
		PlanControl control{_core, currentTransitions};

		if (HFSM2_CHECKED(applyRequests(control, transitions, count))) {
			for (Short i = 0; i < count; ++i)
				_core.previousTransitions.emplace(transitions[i]);

			_apex.deepChangeToRequested(control);

			_core.registry.clearRequests();

			HFSM2_IF_ASSERT(HFSM2_IF_PLANS(_core.planData.verifyPlans()));
			HFSM2_IF_STRUCTURE_REPORT(udpateActivity());

			return true;
		}
	}

	return false;
}

template <typename TG, typename TA>
template <Long NCount>
HFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::replayTransitions(const ArrayT<Transition, NCount>& transitions) noexcept {
	if (transitions.count())
		return replayTransitions(&transitions[0],
								 transitions.count());
	else
		return false;
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
const typename R_<TG, TA>::Transition*
R_<TG, TA>::lastTransitionTo(const StateID stateId_) const noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	if (HFSM2_CHECKED(stateId_ < StateList::SIZE)) {
		const Short index = _core.transitionTargets[stateId_];

		if (index < _core.previousTransitions.count())
			return &_core.previousTransitions[index];
	}

	return nullptr;
}

#endif

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::initialEnter() noexcept {
	HFSM2_ASSERT(!_core.registry.isActive());
	HFSM2_ASSERT(_core.requests.count() == 0);
	HFSM2_IF_TRANSITION_HISTORY(_core.transitionTargets.clear());
	HFSM2_IF_TRANSITION_HISTORY(HFSM2_ASSERT(_core.previousTransitions.count() == 0));

	TransitionSets currentTransitions;
	TransitionSet  pendingTransitions;

	RegistryBackUp undo;
	PlanControl control{_core, currentTransitions};

	_apex.deepRequestChange(control, {TransitionType::RESTART, INVALID_SHORT});

	cancelledByEntryGuards(currentTransitions,
						   pendingTransitions);

	for (Long i = 0;
		 i < SUBSTITUTION_LIMIT && _core.requests.count();
		 ++i)
	{
		backup(_core.registry, undo);

		if (applyRequests(currentTransitions,
						  control))
		{
			pendingTransitions = _core.requests;
			_core.requests.clear();

			if (cancelledByEntryGuards(currentTransitions,
									   pendingTransitions))
			{
				HFSM2_BREAK();

				restore(_core.registry, undo);
			} else
				currentTransitions += pendingTransitions;

			pendingTransitions.clear();
		} else
			_core.requests.clear();
	}
	HFSM2_ASSERT(_core.requests.count() == 0);
	HFSM2_IF_TRANSITION_HISTORY(_core.previousTransitions = currentTransitions);

	_apex.deepEnter(control);

	_core.registry.clearRequests();

	HFSM2_IF_ASSERT(HFSM2_IF_PLANS(_core.planData.verifyPlans()));
	HFSM2_IF_STRUCTURE_REPORT(udpateActivity());
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::finalExit() noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	TransitionSets emptyTransitions;
	PlanControl control{_core, emptyTransitions};

	_apex.deepExit(control);

	_core.registry.clear();
	_core.requests.clear();

#if HFSM2_PLANS_AVAILABLE()
	_core.planData.clear();
#endif

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	// TODO: _core.rng.clear();
#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	_core.transitionTargets  .clear();
	_core.previousTransitions.clear();
#endif

	HFSM2_IF_STRUCTURE_REPORT(udpateActivity());
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::processRequest() noexcept {
	HFSM2_IF_ASSERT(HFSM2_IF_PLANS(_core.planData.verifyPlans()));

	TransitionSets currentTransitions;
	HFSM2_IF_TRANSITION_HISTORY(_core.transitionTargets.clear());

	if (_core.requests.count()) {
		processTransitions(currentTransitions);

		HFSM2_IF_ASSERT(HFSM2_IF_PLANS(_core.planData.verifyPlans()));
		HFSM2_IF_STRUCTURE_REPORT(udpateActivity());
	}

	HFSM2_IF_TRANSITION_HISTORY(_core.previousTransitions = currentTransitions);
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::processTransitions(TransitionSets& currentTransitions) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());
	HFSM2_ASSERT(_core.requests.count());

	TransitionSet pendingTransitions;

	RegistryBackUp registryUndo;
	PlanControl control{_core, currentTransitions};

	for (Long i = 0;
		i < SUBSTITUTION_LIMIT && _core.requests.count();
		++i)
	{
		backup(_core.registry, registryUndo);

		if (applyRequests(currentTransitions,
						  control))
		{
			pendingTransitions = _core.requests;
			_core.requests.clear();

			if (cancelledByGuards(currentTransitions,
								  pendingTransitions))
			{
				HFSM2_IF_TRANSITION_HISTORY(_core.transitionTargets.clear());
				restore(_core.registry, registryUndo);
			} else
				currentTransitions += pendingTransitions;

			pendingTransitions.clear();
		} else
			_core.requests.clear();
	}
	HFSM2_ASSERT(_core.requests.count() == 0);

	if (currentTransitions.count())
		_apex.deepChangeToRequested(control);

	_core.registry.clearRequests();
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::applyRequest(const TransitionSets& currentTransitions,
						 Control& control,
						 const Transition& request,
						 const Short index) noexcept
{
	for (Short i = 0; i < currentTransitions.count(); ++i)
		if (currentTransitions[i] == request)
			return false;

	switch (request.type) {
	case TransitionType::CHANGE:
	case TransitionType::RESTART:
	case TransitionType::RESUME:
	case TransitionType::SELECT:

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	case TransitionType::UTILIZE:
	case TransitionType::RANDOMIZE:
#endif

		// TODO: have both return success status
		if (_core.registry.requestImmediate(request))
			_apex.deepForwardActive(control, {request.type, index});
		else
			_apex.deepRequest	   (control, {request.type, index});

		return true;

	case TransitionType::SCHEDULE:
		_core.registry.requestScheduled(request.destination);

		return false;

	default:
		HFSM2_BREAK();

		return false;
	}
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::applyRequests(const TransitionSets& currentTransitions,
						  Control& control) noexcept
{
	bool changesMade = false;

	for (Short i = 0; i < _core.requests.count(); ++i)
		changesMade |= applyRequest(currentTransitions, control, _core.requests[i], i);

	return changesMade;
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::cancelledByEntryGuards(const TransitionSets& currentTransitions,
								   const TransitionSet&  pendingTransitions) noexcept
{
	GuardControl guardControl{_core,
							  currentTransitions,
							  pendingTransitions};

	return _apex.deepEntryGuard(guardControl);
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::cancelledByGuards(const TransitionSets& currentTransitions,
							  const TransitionSet&  pendingTransitions) noexcept
{
	GuardControl guardControl{_core,
							  currentTransitions,
							  pendingTransitions};

	return _apex.deepForwardExitGuard(guardControl) ||
		   _apex.deepForwardEntryGuard(guardControl);
}

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::save(WriteStream& stream) const noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	_apex.deepSaveActive(_core.registry, stream);

	// TODO: save(stream, _core.requests);

#if HFSM2_PLANS_AVAILABLE()
	// TODO: save(stream, _core.planData);
#endif

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	// TODO: save(stream, _core.rng);
#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	// TODO: save(stream, _core.transitionTargets);
	// TODO: save(stream, _core.previousTransitions);
#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	// TODO: save(stream, _activityHistory);
#endif
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::load(ReadStream& stream) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	_core.registry.clearRequests();
	_core.registry.compoResumable.clear();
	_apex.deepLoadRequested(_core.registry, stream);

	_core.requests.clear();
	// TODO: load(stream, _core.requests);

#if HFSM2_PLANS_AVAILABLE()
	_core.planData.clear();
	// TODO: load(stream, _core.planData);
#endif

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	// TODO: load(stream, _core.rng);
#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	_core.transitionTargets  .clear();
	_core.previousTransitions.clear();
#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	// TODO: load(stream, _activityHistory);
#endif

	TransitionSets emptyTransitions;
	PlanControl control{_core, emptyTransitions};

	_apex.deepChangeToRequested(control);

	HFSM2_IF_STRUCTURE_REPORT(udpateActivity());
}

#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::applyRequests(Control& control,
						  const Transition* const transitions,
						  const Short count) noexcept
{
	TransitionSets currentTransitions;

	if (HFSM2_CHECKED(transitions && count)) {
		bool changesMade = false;

		for (Short i = 0; i < count; ++i)
			changesMade |= applyRequest(currentTransitions, control, transitions[i], i);

		return changesMade;
	} else
		return false;
}

#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::getStateNames() noexcept {
	StructureStateInfos stateInfos;
	_apex.deepGetNames(static_cast<Long>(-1), StructureStateInfo::RegionType::COMPOSITE, 0, stateInfos);

	Long margin = static_cast<Long>(-1);
	for (Long s = 0; s < stateInfos.count(); ++s) {
		const StructureStateInfo& stateInfo = stateInfos[s];
		Prefix& prefix = _prefixes[s];

		if (stateInfo.name[0] != '\0')
			_namedStates.set(s);

		if (margin > stateInfo.depth && stateInfo.name[0] != '\0')
			margin = stateInfo.depth;

		if (stateInfo.depth == 0)
			prefix[0] = L'\0';
		else {
			const Long mark = stateInfo.depth * 2 - 1;

			prefix[mark + 0] = stateInfo.regionType == StructureStateInfo::RegionType::COMPOSITE ? L'└' : L'╙';
			prefix[mark + 1] = L' ';
			prefix[mark + 2] = L'\0';

			for (Long d = mark; d > 0; --d)
				prefix[d - 1] = L' ';

			for (Long r = s; r > stateInfo.parent; --r) {
				Prefix& prefixAbove = _prefixes[r - 1];

				switch (prefixAbove[mark]) {
				case L' ':
					prefixAbove[mark] = stateInfo.regionType == StructureStateInfo::RegionType::COMPOSITE ? L'│' : L'║';
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
	for (Long s = 0; s < stateInfos.count(); ++s) {
		const StructureStateInfo& stateInfo = stateInfos[s];
		Prefix& prefix = _prefixes[s];
		const Long space = stateInfo.depth * 2;

		if (stateInfo.name[0] != L'\0') {
			_structure.emplace(StructureEntry{false, &prefix[margin * 2], stateInfo.name});
			_activityHistory.emplace(static_cast<int8_t>(0));
		} else if (s + 1 < stateInfos.count()) {
			Prefix& nextPrefix = _prefixes[s + 1];

			if (s > 0)
				for (Long c = 0; c <= space; ++c)
					nextPrefix[c] = prefix[c];

			const Long mark = space + 1;

			switch (nextPrefix[mark]) {
			case L'├':
				nextPrefix[mark] = stateInfo.depth == margin ? L'┌' : L'┬';
				break;
			case L'╟':
				nextPrefix[mark] = stateInfo.depth == margin ? L'╓' : L'╥';
				break;
			}
		}
	}
}

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::udpateActivity() noexcept {
	for (Long s = 0, i = 0; s < STATE_COUNT; ++s)
		if (_namedStates.get(s)) {
			_structure[i].isActive = isActive(s);

			typename ActivityHistory::Item& activity = _activityHistory[i];

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

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
RV_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::RV_(Context& context
																								 HFSM2_IF_UTILITY_THEORY(, RNG& rng)
																								 HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
		 HFSM2_IF_UTILITY_THEORY(, rng)
		 HFSM2_IF_LOG_INTERFACE(, logger)}
{
	initialEnter();
}

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
RV_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::RV_(PureContext&& context
																								 HFSM2_IF_UTILITY_THEORY(, RNG& rng)
																								 HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{move(context)
		 HFSM2_IF_UTILITY_THEORY(, rng)
		 HFSM2_IF_LOG_INTERFACE(, logger)}
{
	initialEnter();
}

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
RV_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::RV_(const RV_& other) noexcept
	: Base{other}
{}

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
RV_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::RV_(RV_&& other) noexcept
	: Base{move(other)}
{}

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(20)
RV_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::~RV_() noexcept {
	finalExit();
}

#if HFSM2_SERIALIZATION_AVAILABLE()

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RV_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::save(SerialBuffer& buffer) const noexcept {
	WriteStream stream{buffer};

	stream.template write<1>(1);
	save(stream);
}

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RV_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::load(const SerialBuffer& buffer) noexcept {
	ReadStream stream{buffer};

	if (HFSM2_CHECKED(stream.template read<1>()))
		Base::load(stream);
}

#endif

#if HFSM2_SERIALIZATION_AVAILABLE()

template <FeatureTag NFT, typename TC HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RV_<G_<NFT, TC, Manual HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::save(SerialBuffer& buffer) const noexcept {
	WriteStream stream{buffer};

	if (isActive()) {
		stream.template write<1>(1);

		save(stream);
	}
	else
		stream.template write<1>(0);
}

template <FeatureTag NFT, typename TC HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RV_<G_<NFT, TC, Manual HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::load(const SerialBuffer& buffer) noexcept {
	ReadStream stream{buffer};

	if (stream.template read<1>()) {
		if (isActive())
			Base::load(stream);
		else
			loadEnter (stream);
	}
	else
		if (isActive())
			finalExit();
}

#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

template <FeatureTag NFT, typename TC HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
bool
RV_<G_<NFT, TC, Manual HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::replayEnter(const Transition* const transitions,
																											   const Short count) noexcept
{
	HFSM2_ASSERT(!_core.registry.isActive());
	HFSM2_ASSERT(_core.requests.count() == 0);

	_core.transitionTargets.clear();
	HFSM2_ASSERT(_core.previousTransitions.count() == 0);

	if (HFSM2_CHECKED(transitions && count)) {
		TransitionSets emptyTransitions;
		PlanControl control{_core, emptyTransitions};

		if (HFSM2_CHECKED(applyRequests(control, transitions, count))) {
			for (Short i = 0; i < count; ++i)
				_core.previousTransitions.emplace(transitions[i]);

			_apex.deepEnter(control);

			_core.registry.clearRequests();

			HFSM2_IF_ASSERT(HFSM2_IF_PLANS(_core.planData.verifyPlans()));
			HFSM2_IF_STRUCTURE_REPORT(udpateActivity());

			return true;
		}
	}

	return false;
}

template <FeatureTag NFT, typename TC HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
template <Long NCount>
HFSM2_CONSTEXPR(14)
bool
RV_<G_<NFT, TC, Manual HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::replayEnter(const ArrayT<Transition, NCount>& transitions)  noexcept {
	if (transitions.count())
		return replayEnter(&transitions[0],
						   transitions.count());
	else
		return false;
}

#endif

#if HFSM2_SERIALIZATION_AVAILABLE()

template <FeatureTag NFT, typename TC HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RV_<G_<NFT, TC, Manual HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::loadEnter(ReadStream& stream) noexcept {
	HFSM2_ASSERT(_core.registry.empty());
	_apex.deepLoadRequested(_core.registry, stream);

	HFSM2_ASSERT(_core.requests.empty());

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_ASSERT(_core.planData.empty() == 0);
#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	HFSM2_ASSERT(_core.transitionTargets  .empty());
	HFSM2_ASSERT(_core.previousTransitions.empty());
#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	//HFSM2_ASSERT(_activityHistory.empty());
#endif

	TransitionSets emptyTransitions;
	PlanControl control{_core, emptyTransitions};

	_apex.deepEnter(control);

	HFSM2_IF_STRUCTURE_REPORT(udpateActivity());
}

#endif

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::changeWith(const StateID  stateId_,
																										  const Payload& payload) noexcept
{
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::CHANGE, payload});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::CHANGE, stateId_);
}

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::restartWith(const StateID  stateId_,
																										   const Payload& payload) noexcept
{
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::RESTART, payload});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::RESTART, stateId_);
}

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::resumeWith(const StateID  stateId_,
																										  const Payload& payload) noexcept
{
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::RESUME, payload});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::RESUME, stateId_);
}

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::selectWith(const StateID  stateId_,
																										  const Payload& payload) noexcept
{
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::SELECT, payload});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::SELECT, stateId_);
}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <FeatureTag NFT, typename TC, typename TV, typename TR, typename TU, typename TG, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV, TR, TU, TG, NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::utilizeWith(const StateID  stateId_,
																				 const Payload& payload) noexcept
{
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::UTILIZE, payload});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::UTILIZE, stateId_);
}

template <FeatureTag NFT, typename TC, typename TV, typename TR, typename TU, typename TG, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV, TR, TU, TG, NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::randomizeWith(const StateID  stateId_,
																				   const Payload& payload) noexcept
{
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::RANDOMIZE, payload});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::RANDOMIZE, stateId_);
}

#endif

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::scheduleWith(const StateID  stateId_,
																											const Payload& payload) noexcept
{
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::SCHEDULE, payload});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::SCHEDULE, stateId_);
}

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::immediateChangeWith(const StateID  stateId_,
																												   const Payload& payload) noexcept
{
	changeWith(stateId_, payload);

	processRequest();
}

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::immediateRestartWith(const StateID  stateId_,
																													const Payload& payload) noexcept
{
	restartWith(stateId_, payload);

	processRequest();
}

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::immediateResumeWith(const StateID  stateId_,
																												   const Payload& payload) noexcept
{
	resumeWith(stateId_, payload);

	processRequest();
}

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::immediateSelectWith(const StateID  stateId_,
																												   const Payload& payload) noexcept
{
	selectWith(stateId_, payload);

	processRequest();
}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <FeatureTag NFT, typename TC, typename TV, typename TR, typename TU, typename TG, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV, TR, TU, TG, NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::immediateUtilizeWith(const StateID  stateId_,
																						  const Payload& payload) noexcept
{
	utilizeWith(stateId_, payload);

	processRequest();
}

template <FeatureTag NFT, typename TC, typename TV, typename TR, typename TU, typename TG, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV, TR, TU, TG, NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::immediateRandomizeWith(const StateID  stateId_,
																							const Payload& payload) noexcept
{
	randomizeWith(stateId_, payload);

	processRequest();
}

#endif

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
RC_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::RC_(Context& context
																								 HFSM2_IF_UTILITY_THEORY(, RNG& rng)
																								 HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
		 HFSM2_IF_UTILITY_THEORY(, rng)
		 HFSM2_IF_LOG_INTERFACE(, logger)}
{}

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
RC_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::RC_(PureContext&& context
																								 HFSM2_IF_UTILITY_THEORY(, RNG& rng)
																								 HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{move(context)
		 HFSM2_IF_UTILITY_THEORY(, rng)
		 HFSM2_IF_LOG_INTERFACE(, logger)}
{}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <FeatureTag NFT, typename TC, typename TV, typename TR, typename TU, typename TG, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
RC_<G_<NFT, TC*, TV, TR, TU, TG, NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::RC_(Context context
																		, RNG& rng
																		HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
		 , rng
		 HFSM2_IF_LOG_INTERFACE(, logger)}
{}

#else

template <FeatureTag NFT, typename TC, typename TV, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
RC_<G_<NFT, TC*, TV, NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::RC_(Context context
															HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
		 HFSM2_IF_LOG_INTERFACE(, logger)}
{}

#endif

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <FeatureTag NFT, typename TV, typename TR, typename TU, typename TG, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
RC_<G_<NFT, EmptyContext, TV, TR, TU, TG, NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::RC_(RNG& rng
																				 HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{static_cast<EmptyContext&>(*this)
		 , rng
		 HFSM2_IF_LOG_INTERFACE(, logger)}
{}

#else

template <FeatureTag NFT, typename TV, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
RC_<G_<NFT, EmptyContext, TV, NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::RC_(HFSM2_IF_LOG_INTERFACE(Logger* const logger)) noexcept
	: Base{static_cast<EmptyContext&>(*this)
		 HFSM2_IF_LOG_INTERFACE(, logger)}
{}

#endif

}
}

#if defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
#endif

#ifdef __clang__
	#pragma clang diagnostic pop
#endif

#if _MSC_VER == 1900
	#pragma warning(pop)
#endif

//#undef HFSM2_UNUSED

#undef HFSM2_ATTRIBUTE
#undef HFSM2_ATTRIBUTE_FALLTHROUGH
#undef HFSM2_ATTRIBUTE_NO_UNIQUE_ADDRESS

#undef HFSM2_CONSTEXPR
#undef HFSM2_CONSTEXPR_NO
#undef HFSM2_CONSTEXPR_11
#undef HFSM2_CONSTEXPR_14
#undef HFSM2_CONSTEXPR_17
#undef HFSM2_CONSTEXPR_20

//#undef HFSM2_ARCHITECTURE
//#undef HFSM2_ARCHITECTURE_64
//#undef HFSM2_ARCHITECTURE_32
#undef HFSM2_64BIT_OR_32BIT

//#undef HFSM2_BREAK
#undef HFSM2_BREAK_AVAILABLE

#undef HFSM2_IF_DEBUG
#undef HFSM2_UNLESS_DEBUG
#undef HFSM2_DEBUG_OR

//#undef HFSM2_ASSERT_AVAILABLE
#undef HFSM2_IF_ASSERT
//#undef HFSM2_CHECKED
#undef HFSM2_ASSERT
#undef HFSM2_ASSERT_OR

#undef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE

#undef HFSM2_IF_TYPEINDEX
#undef HFSM2_TYPEINDEX_AVAILABLE
#undef HFSM2_IF_TYPEINDEX

//#undef HFSM2_DEBUG_STATE_TYPE_AVAILABLE

//#undef HFSM2_PLANS_AVAILABLE
#undef HFSM2_IF_PLANS

#undef HFSM2_SERIALIZATION_AVAILABLE
#undef HFSM2_IF_SERIALIZATION

#undef HFSM2_STRUCTURE_REPORT_AVAILABLE
#undef HFSM2_IF_STRUCTURE_REPORT

//#undef HFSM2_TRANSITION_HISTORY_AVAILABLE
#undef HFSM2_IF_TRANSITION_HISTORY

//#undef HFSM2_UTILITY_THEORY_AVAILABLE
#undef HFSM2_IF_UTILITY_THEORY

#undef HFSM2_VERBOSE_DEBUG_LOG_AVAILABLE
#undef HFSM2_LOG_INTERFACE_AVAILABLE
#undef HFSM2_IF_LOG_INTERFACE

#undef HFSM2_LOG_TRANSITION

#if HFSM2_PLANS_AVAILABLE()
	#undef HFSM2_LOG_TASK_STATUS
	#undef HFSM2_LOG_PLAN_STATUS
#endif

#undef HFSM2_LOG_CANCELLED_PENDING

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	#undef HFSM2_LOG_UTILITY_RESOLUTION
	#undef HFSM2_LOG_RANDOM_RESOLUTION
#endif

#undef HFSM2_LOG_STATE_METHOD

