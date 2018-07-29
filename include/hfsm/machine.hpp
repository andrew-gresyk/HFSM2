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
#include <utility>

//------------------------------------------------------------------------------

// TODO: define better check macros

#ifdef HFSM_ENABLE_ALIGNMENT_CHEKS
	#define HFSM_IF_ALIGNMENT_CHEKS(...)	__VA_ARGS__
#else
	#define HFSM_IF_ALIGNMENT_CHEKS(...)
#endif

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
static constexpr ShortIndex	INVALID_SHORT_INDEX = UINT8_MAX;

// TEMP
using LongIndex  = uint16_t;
static constexpr LongIndex	INVALID_LONG_INDEX	= UINT16_MAX;

using StateID	 = LongIndex;
static constexpr StateID	INVALID_STATE_ID	= INVALID_LONG_INDEX;

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

template <LongIndex, typename...>
struct TypeListBuilder;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <LongIndex NIndex, typename TFirst, typename... TRest>
struct TypeListBuilder<NIndex, TFirst, TRest...>
	: TypeListBuilder<NIndex + 1, TRest...>
{
	static constexpr LongIndex INDEX = NIndex;

	static_assert(INDEX < (1 << 8 * sizeof(LongIndex)), "Too many types");

	using Type = TFirst;
	using Base = TypeListBuilder<INDEX + 1, TRest...>;

	template <typename T>
	static constexpr LongIndex index() {
		return std::is_same<T, Type>::value ? INDEX : Base::template index<T>();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <LongIndex NIndex, typename TFirst>
struct TypeListBuilder<NIndex, TFirst> {
	static constexpr LongIndex INDEX = NIndex;

	static_assert(NIndex < (1 << 8 * sizeof(LongIndex)), "Too many types");

	using Type = TFirst;

	template <typename T>
	static constexpr LongIndex index() {
		return std::is_same<T, Type>::value ? INDEX : INVALID_LONG_INDEX;
	}
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

	static constexpr LongIndex SIZE = sizeof...(Ts);

	template <typename T>
	static constexpr LongIndex index() {
		return Base::template index<T>();
	}

	template <typename T>
	static constexpr bool contains() {
		return Base::template index<T>() != INVALID_LONG_INDEX;
	}
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

// TODO: add assignment op and templated type conversion op

template <typename... Ts>
class VariantT {
	using Types = TypeListT<Ts...>;

public:
	inline VariantT() = default;

	template <typename T, typename = typename std::enable_if<Types::template contains<T>()>::type>
	inline VariantT(T* const p)
		: _pointer(p)
		, _index(Types::template index<T>())
	{
		HFSM_IF_ALIGNMENT_CHEKS(assert((((uintptr_t) this) & 0x7) == 0));
		assert(_index != INVALID_LONG_INDEX);
	}

	inline explicit operator bool() const { return _index != INVALID_LONG_INDEX; }

	inline void reset() {
		_pointer = nullptr;
		_index = INVALID_LONG_INDEX;
	}

	template <typename T>
	inline typename std::enable_if<Types::template contains<T>(), T>::type*
	get() const {
		const auto INDEX = Types::template index<T>();

		assert(INDEX == _index);

		return INDEX == _index ?
			reinterpret_cast<T*>(_pointer) : nullptr;
	}

private:
	alignas(alignof(void*)) void* _pointer = nullptr;
	LongIndex _index = INVALID_LONG_INDEX;
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
	recordMethod(const StateID origin,
				 const Method method) = 0;

	virtual
	void
	recordTransition(const Transition transition,
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

//------------------------------------------------------------------------------


namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext>
class ControlT {
	template <StateID, typename, typename, typename, typename>
	friend struct _S;

	template <StateID, typename, typename, typename, typename, typename...>
	friend struct _C;

	template <StateID, typename, typename, typename, typename, typename...>
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

template <typename, typename, typename>
struct ControlLockT;

template <typename TContext, typename TStateList, typename TPayloadList>
class TransitionControlT final
	: public ControlT<TContext>
{
	using Context			= TContext;
	using Base				= ControlT<Context>;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using TransitionQueue	= TransitionQueueT<TPayloadList>;

	template <typename, typename, ShortIndex, typename>
	friend class _R;

	template <typename, typename, typename>
	friend struct ControlLockT;

private:
	inline TransitionControlT(Context& context,
							  TransitionQueue& requests,
							  LoggerInterface* const logger)
		: Base(context, logger)
		, _requests(requests)
	{}

	template <typename T>
	static constexpr LongIndex
	stateId()					{ return StateList::template index<T>();	}

public:
	using Lock				= ControlLockT<Context, StateList, PayloadList>;

	inline void changeTo(const StateID stateId);
	inline void resume	(const StateID stateId);
	inline void schedule(const StateID stateId);

	template <typename TState>
	inline void changeTo()						{ changeTo(stateId<TState>());	}

	template <typename TState>
	inline void resume()						{ resume  (stateId<TState>());	}

	template <typename TState>
	inline void schedule()						{ schedule(stateId<TState>());	}

	inline auto requestCount() const			{ return _requests.count();		}

private:
	TransitionQueue& _requests;
	bool _locked = false;
};

template <typename TContext, typename TStateList, typename TPayloadList>
struct ControlLockT {
	using Context			= TContext;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;
	using TransitionControl = TransitionControlT<Context, StateList, PayloadList>;

	inline ControlLockT(TransitionControl& control);
	inline ~ControlLockT();

	TransitionControl* const _control;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TSL, typename TPL>
void
TransitionControlT<TC, TSL, TPL>::changeTo(const StateID stateId) {
	if (!_locked) {
		const Transition transition{TransitionType::RESTART, stateId};
		_requests << transition;

	#ifdef HFSM_ENABLE_LOG_INTERFACE
		if (Base::_logger)
			Base::_logger->recordTransition(::hfsm::Transition::RESTART, stateId);
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TSL, typename TPL>
void
TransitionControlT<TC, TSL, TPL>::resume(const StateID stateId) {
	if (!_locked) {
		const Transition transition{TransitionType::RESUME, stateId};
		_requests << transition;

	#ifdef HFSM_ENABLE_LOG_INTERFACE
		if (Base::_logger)
			Base::_logger->recordTransition(::hfsm::Transition::RESUME, stateId);
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TSL, typename TPL>
void
TransitionControlT<TC, TSL, TPL>::schedule(const StateID stateId) {
	if (!_locked) {
		const Transition transition{TransitionType::SCHEDULE, stateId};
		_requests << transition;

	#ifdef HFSM_ENABLE_LOG_INTERFACE
		if (Base::_logger)
			Base::_logger->recordTransition(::hfsm::Transition::SCHEDULE, stateId);
	#endif
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TSL, typename TPL>
ControlLockT<TC, TSL, TPL>::ControlLockT(TransitionControl& control)
	: _control(!control._locked ? &control : nullptr)
{
	if (_control)
		_control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TC, typename TSL, typename TPL>
ControlLockT<TC, TSL, TPL>::~ControlLockT() {
	if (_control)
		_control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct Parent {
	ShortIndex fork  = INVALID_SHORT_INDEX;
	ShortIndex prong = INVALID_SHORT_INDEX;

	inline Parent() = default;

	inline Parent(const ShortIndex fork_,
				  const ShortIndex prong_)
		: fork(fork_)
		, prong(prong_)
	{
		HFSM_IF_ALIGNMENT_CHEKS(assert((((uintptr_t) this) & 0x1) == 0));
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

template <typename TPayloadList>
using StateRegistry2T = ArrayView<StateInfoT<TPayloadList>>;

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct Fork {
	ShortIndex self		 = INVALID_SHORT_INDEX;
	ShortIndex active	 = INVALID_SHORT_INDEX;
	ShortIndex resumable = INVALID_SHORT_INDEX;
	ShortIndex requested = INVALID_SHORT_INDEX;

	inline Fork(const ShortIndex self_)
		: self(self_)
	{
		HFSM_IF_ALIGNMENT_CHEKS(assert((((uintptr_t) this) & 0x3) == 0));
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
		: Fork(index)
	{
		forkParents[index] = parent;
	}
};

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
		HFSM_IF_ALIGNMENT_CHEKS(assert((((uintptr_t) this) & (sizeof(void*) - 1)) == 0));
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
struct TransitionInfoT {
	using TransitionPayloads = TPayloadList;
	using Transition = TransitionT<TPayloadList>;

	inline TransitionInfoT() = default;

	inline TransitionInfoT(const Transition transition_,
						   const ::hfsm::Method method_)
		: stateId{transition_.stateId}
		, method(method_)
		, transition(get<TransitionPayloads>(transition_.type))
	{
		HFSM_IF_ALIGNMENT_CHEKS(assert((((uintptr_t) this) & 0x7) == 0));
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

template <typename TContext, typename TStateList, typename TPayloadList>
class Bare {
	template <typename...>
	friend struct _B;

protected:
	using Context			= TContext;
	using Control			= ControlT<Context>;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;
	using TransitionControl = TransitionControlT<Context, StateList, PayloadList>;

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

template <typename TContext, typename TStateList, typename TPayloadList>
using Base = _B<Bare<TContext, TStateList, TPayloadList>>;

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
	using StateList			= TypeListT<Head>;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct _CSF<TInitial, TRemaining...> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using Remaining			= _CSF<TRemaining...>;
	using StateList			= typename MergeT<typename Initial::StateList, typename Remaining::StateList>::TypeList;
};

template <typename TInitial>
struct _CSF<TInitial> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using StateList			= typename Initial::StateList;
};

template <typename THead, typename... TSubStates>
struct _CF final {
	using Head				= THead;
	using State				= _SF<Head>;
	using SubStates			= _CSF<TSubStates...>;
	using StateList			= typename MergeT<typename State::StateList, typename SubStates::StateList>::TypeList;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct _OSF<TInitial, TRemaining...> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using Remaining			= _OSF<TRemaining...>;
	using StateList			= typename MergeT<typename Initial::StateList, typename Remaining::StateList>::TypeList;
};

template <typename TInitial>
struct _OSF<TInitial> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using StateList			= typename Initial::StateList;
};

template <typename THead,
		  typename... TSubStates>
struct _OF final {
	using Head				= THead;
	using State				= _SF<Head>;
	using SubStates			= _OSF<TSubStates...>;
	using StateList			= typename MergeT<typename State::StateList, typename SubStates::StateList>::TypeList;
};

////////////////////////////////////////////////////////////////////////////////

template <StateID, typename, typename, typename, typename>
struct _S;

template <StateID, typename, typename, typename, typename, typename...>
struct _C;

template <StateID, typename, typename, typename, typename, typename...>
struct _O;

template <typename, typename, ShortIndex, typename>
class _R;

//------------------------------------------------------------------------------

template <StateID, typename...>
struct WrapMaterial;

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
struct WrapMaterial<TID, TC, TSL, TPL, TH> {
	using Type = _S<TID, TC, TSL, TPL, TH>;
};

template <StateID TID, typename TC, typename TSL, typename TPL,				 typename... TS>
struct WrapMaterial<TID, TC, TSL, TPL, _CF<void, TS...>> {
	using Type = _C<TID, TC, TSL, TPL, Base<TC, TSL, TPL>, TS...>;
};

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
struct WrapMaterial<TID, TC, TSL, TPL, _CF<TH, TS...>> {
	using Type = _C<TID, TC, TSL, TPL, TH, TS...>;
};

template <StateID TID, typename TC, typename TSL, typename TPL,				 typename... TS>
struct WrapMaterial<TID, TC, TSL, TPL, _OF<void, TS...>> {
	using Type = _O<TID, TC, TSL, TPL, Base<TC, TSL, TPL>, TS...>;
};

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
struct WrapMaterial<TID, TC, TSL, TPL, _OF<TH, TS...>> {
	using Type = _O<TID, TC, TSL, TPL, TH, TS...>;
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TPayloadList,
		  ShortIndex TMaxSubstitutions,
		  typename TApex>
struct _RF final {
	static constexpr ShortIndex MAX_SUBSTITUTIONS = TMaxSubstitutions;

	using Context			= TContext;
	using PayloadList		= TPayloadList;

	using Forward			= TApex;
	using StateList			= typename Forward::StateList;
	using TransitionControl	= TransitionControlT<Context, StateList, PayloadList>;

	using Instance			= _R<Context, PayloadList, MAX_SUBSTITUTIONS, Forward>;

	using Bare = ::hfsm::detail::Bare<Context, StateList, PayloadList>;
	using Base = ::hfsm::detail::Base<Context, StateList, PayloadList>;

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

	static constexpr ShortIndex MAX_SUBSTITUTIONS = TMaxSubstitutions;

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
	using Root				 = _RF<Context, PayloadList, MAX_SUBSTITUTIONS, Composite <THead, TSubStates...>>;

	template <				  typename... TSubStates>
	using PeerRoot			 = _RF<Context, PayloadList, MAX_SUBSTITUTIONS, CompositePeers<	  TSubStates...>>;

	template <typename THead, typename... TSubStates>
	using OrthogonalRoot	 = _RF<Context, PayloadList, MAX_SUBSTITUTIONS, Orthogonal<THead, TSubStates...>>;

	template <				  typename... TSubStates>
	using OrthogonalPeerRoot = _RF<Context, PayloadList, MAX_SUBSTITUTIONS, OrthogonalPeers<  TSubStates...>>;

	//----------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////

}

template <ShortIndex TMaxSubstitutions = 4>
struct Config {
	enum : ShortIndex { MAX_SUBSTITUTIONS = TMaxSubstitutions };
};

//------------------------------------------------------------------------------

template <typename... Ts>
using TransitionPayloads = detail::TypeListT<Ts...>;

//------------------------------------------------------------------------------

template <typename...>
struct Machine;

template <typename TContext>
struct Machine<TContext>
	: detail::_M<TContext>
{};

template <typename TContext, typename TConfig>
struct Machine<TContext, TConfig>
	: detail::_M<TContext, TransitionPayloads<>, TConfig::MAX_SUBSTITUTIONS>
{};

template <typename TContext, typename... TPayloads>
struct Machine<TContext, TransitionPayloads<TPayloads...>>
	: detail::_M<TContext, TransitionPayloads<TPayloads...>>
{};

template <typename TContext, typename TConfig, typename... TPayloads>
struct Machine<TContext, TransitionPayloads<TPayloads...>, TConfig>
	: detail::_M<TContext, TransitionPayloads<TPayloads...>, TConfig::MAX_SUBSTITUTIONS>
{};

////////////////////////////////////////////////////////////////////////////////

}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TStateID,
		  typename TContext,
		  typename TStateList,
		  typename TPayloadList,
		  typename THead>
struct _S final {
	static constexpr StateID STATE_ID = TStateID;

	using Context			= TContext;
	using Control			= ControlT<Context>;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;
	using StateRegistry2	= StateRegistry2T<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionControl	= TransitionControlT<Context, StateList, PayloadList>;
	using Head				= THead;

	using Base				= ::hfsm::detail::Base<Context, StateList, PayloadList>;

	static constexpr LongIndex REVERSE_DEPTH = 1;
	static constexpr LongIndex DEEP_WIDTH	 = 0;
	static constexpr LongIndex STATE_COUNT	 = 1;
	static constexpr LongIndex FORK_COUNT	 = 0;
	static constexpr LongIndex PRONG_COUNT	 = 0;
	static constexpr LongIndex WIDTH		 = 1;

	_S(StateRegistry2& stateRegistry,
	   const Parent parent,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

	inline void deepForwardGuard	 (TransitionControl&)							{}
	inline bool deepGuard			 (TransitionControl& control);

	inline void deepEnterInitial	 (Control& control);
	inline void deepEnter			 (Control& control);

	inline bool deepUpdate			 (TransitionControl& control);

	template <typename TEvent>
	inline void deepReact			 (const TEvent& event, TransitionControl& control);

	inline void deepExit			 (Control& control);

	inline void deepForwardRequest	 (const typename Transition::Type)					{}
	inline void deepRequestRemain	 ()												{}
	inline void deepRequestRestart	 ()												{}
	inline void deepRequestResume	 ()												{}
	inline void deepChangeToRequested(Control&)										{}

#if defined HFSM_ENABLE_STRUCTURE_REPORT || defined HFSM_ENABLE_LOG_INTERFACE
	static constexpr bool isBare()	 { return std::is_same<Head, Base>::value;		 }

	static constexpr LongIndex NAME_COUNT = isBare() ? 0 : 1;
#endif

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using RegionType		= typename StructureStateInfo::RegionType;

	static const char* name();

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
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
		logger.recordMethod(STATE_ID, TMethodId);
	}
#endif

	Head _head;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

//------------------------------------------------------------------------------

namespace {
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
struct Register {
	using StateRegistry2 = StateRegistry2T<TPL>;

	static inline void
	execute(StateRegistry2& stateRegistry, const Parent parent) {
		static constexpr auto TYPE_ID = TSL::template index<TH>();
		assertEquality<TID, TYPE_ID>();

		auto& stateInfo = stateRegistry[TID];
		stateInfo.parent = parent;
		stateInfo.payload.reset();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL>
struct Register<TID, TC, TSL, TPL, Base<TC, TSL, TPL>> {
	using StateRegistry2 = StateRegistry2T<TPL>;

	static inline void
	execute(StateRegistry2&, const Parent) {}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
}

////////////////////////////////////////////////////////////////////////////////

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
_S<TID, TC, TSL, TPL, TH>::_S(StateRegistry2& stateRegistry,
							  const Parent parent,
							  Parents& /*forkParents*/,
							  ForkPointers& /*forkPointers*/)
{
	Register<TID, TC, TSL, TPL, TH>::execute(stateRegistry, parent);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
bool
_S<TID, TC, TSL, TPL, TH>::deepGuard(TransitionControl& control) {
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

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
void
_S<TID, TC, TSL, TPL, TH>::deepEnterInitial(Control& control) {
	deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
void
_S<TID, TC, TSL, TPL, TH>::deepEnter(Control& control) {
	_head.widePreEnter(control.context());
	_head.enter(control);

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		log<decltype(&Head::enter), LoggerInterface::Method::ENTER>(*logger);
#endif
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
bool
_S<TID, TC, TSL, TPL, TH>::deepUpdate(TransitionControl& control) {
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

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
template <typename TEvent>
void
_S<TID, TC, TSL, TPL, TH>::deepReact(const TEvent& event,
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

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
void
_S<TID, TC, TSL, TPL, TH>::deepExit(Control& control) {
	_head.exit(control);
	_head.widePostExit(control.context());

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		log<decltype(&Head::exit), LoggerInterface::Method::EXIT>(*logger);
#endif
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
const char*
_S<TID, TC, TSL, TPL, TH>::name() {
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

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
void
_S<TID, TC, TSL, TPL, TH>::deepGetNames(const LongIndex parent,
										const RegionType region,
										const ShortIndex depth,
										StructureStateInfos& _stateInfos) const
{
	_stateInfos << StructureStateInfo { parent, region, depth, name() };
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
void
_S<TID, TC, TSL, TPL, TH>::deepIsActive(const bool isActive,
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

template <StateID, typename, typename, typename, ShortIndex, typename...>
struct _CS;

//------------------------------------------------------------------------------

template <StateID TInitialID,
		  typename TContext,
		  typename TStateList,
		  typename TPayloadList,
		  ShortIndex NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct _CS<TInitialID, TContext, TStateList, TPayloadList, NIndex, TInitial, TRemaining...> {
	static constexpr StateID INITIAL_ID = TInitialID;

	using Context			= TContext;
	using Control			= ControlT<Context>;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;
	using StateRegistry2	= StateRegistry2T<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using TransitionControl	= TransitionControlT<Context, StateList, PayloadList>;
	using Initial			= typename WrapMaterial<INITIAL_ID, Context, StateList, PayloadList, TInitial>::Type;
	using Remaining			= _CS<INITIAL_ID + Initial::STATE_COUNT, Context, StateList, PayloadList, NIndex + 1, TRemaining...>;
	using Forward			= _CSF<TInitial, TRemaining...>;
	using DeepStateList		= typename Forward::StateList;

	static constexpr LongIndex PRONG_INDEX	 = NIndex;
	static constexpr LongIndex REVERSE_DEPTH = Max<Initial::REVERSE_DEPTH, Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr LongIndex DEEP_WIDTH	 = Max<Initial::DEEP_WIDTH, Remaining::DEEP_WIDTH>::VALUE;
	static constexpr LongIndex STATE_COUNT	 = Initial::STATE_COUNT + Remaining::STATE_COUNT;
	static constexpr LongIndex FORK_COUNT	 = Initial::FORK_COUNT  + Remaining::FORK_COUNT;
	static constexpr LongIndex PRONG_COUNT	 = Initial::PRONG_COUNT + Remaining::PRONG_COUNT;

	_CS(StateRegistry2& stateRegistry,
		const ShortIndex fork,
		Parents& forkParents,
		ForkPointers& forkPointers);

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
	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT  + Remaining::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;

	void wideIsActive(const ShortIndex prong,
					  LongIndex& index,
					  MachineStructure& structure) const;
#endif

	Initial initial;
	Remaining remaining;
	HSFM_IF_DEBUG(DeepStateList stateList);
};

//------------------------------------------------------------------------------

template <StateID TInitialID,
		  typename TContext,
		  typename TStateList,
		  typename TPayloadList,
		  ShortIndex NIndex,
		  typename TInitial>
struct _CS<TInitialID, TContext, TStateList, TPayloadList, NIndex, TInitial> {
	static constexpr StateID INITIAL_ID = TInitialID;

	using Context			= TContext;
	using StateList			= TStateList;
	using Control			= ControlT<Context>;
	using PayloadList		= TPayloadList;
	using StateRegistry2	= StateRegistry2T<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using TransitionControl	= TransitionControlT<Context, StateList, PayloadList>;
	using Initial			= typename WrapMaterial<INITIAL_ID, Context, StateList, PayloadList, TInitial>::Type;
	using Forward			= _CSF<TInitial>;
	using DeepStateList		= typename Forward::StateList;

	static constexpr LongIndex PRONG_INDEX	 = NIndex;
	static constexpr LongIndex REVERSE_DEPTH = Initial::REVERSE_DEPTH;
	static constexpr LongIndex DEEP_WIDTH	 = Max<1, Initial::DEEP_WIDTH>::VALUE;
	static constexpr LongIndex STATE_COUNT	 = Initial::STATE_COUNT;
	static constexpr LongIndex FORK_COUNT	 = Initial::FORK_COUNT;
	static constexpr LongIndex PRONG_COUNT	 = Initial::PRONG_COUNT;

	_CS(StateRegistry2& stateRegistry,
		const ShortIndex fork,
		Parents& forkParents,
		ForkPointers& forkPointers);

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
	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;

	void wideIsActive(const ShortIndex prong,
					  LongIndex& index,
					  MachineStructure& structure) const;
#endif

	Initial initial;
	HSFM_IF_DEBUG(DeepStateList stateList);
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::_CS(StateRegistry2& stateRegistry,
										   const ShortIndex fork,
										   Parents& forkParents,
										   ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork, PRONG_INDEX),
			  forkParents,
			  forkPointers)
	, remaining(stateRegistry,
				fork,
				forkParents,
				forkPointers)
{}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideForwardGuard(const ShortIndex prong,
														TransitionControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepForwardGuard(control);
	else
		remaining.wideForwardGuard(prong, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideGuard(const ShortIndex prong,
												 TransitionControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepGuard(control);
	else
		remaining.wideGuard(prong, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideEnterInitial(Control& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideEnter(const ShortIndex prong,
												 Control& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepEnter(control);
	else
		remaining.wideEnter(prong, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
bool
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideUpdate(const ShortIndex prong,
												  TransitionControl& control)
{
	return prong == PRONG_INDEX ?
		initial  .deepUpdate(control) :
		remaining.wideUpdate(prong, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
template <typename TEvent>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideReact(const ShortIndex prong,
												 const TEvent& event,
												 TransitionControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepReact(	   event, control);
	else
		remaining.wideReact(prong, event, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideExit(const ShortIndex prong,
												Control& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepExit(control);
	else
		remaining.wideExit(prong, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideForwardRequest(const ShortIndex prong,
														  const TransitionType transition)
{
	if (prong == PRONG_INDEX)
		initial	 .deepForwardRequest(		transition);
	else
		remaining.wideForwardRequest(prong, transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideRequestResume(const ShortIndex prong) {
	if (prong == PRONG_INDEX)
		initial	 .deepRequestResume();
	else
		remaining.wideRequestResume(prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideChangeToRequested(const ShortIndex prong,
															 Control& control)
{
	if (prong == PRONG_INDEX)
		initial	 .deepChangeToRequested(control);
	else
		remaining.wideChangeToRequested(prong, control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideGetNames(const LongIndex parent,
													const ShortIndex depth,
													StructureStateInfos& _stateInfos) const
{
	initial	 .deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
	remaining.wideGetNames(parent,								  depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideIsActive(const ShortIndex prong,
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

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
_CS<TID, TC, TSL, TPL, NI, TI>::_CS(StateRegistry2& stateRegistry,
									const ShortIndex fork,
									Parents& forkParents,
									ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork, PRONG_INDEX),
			  forkParents,
			  forkPointers)
{}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideForwardGuard(const ShortIndex HSFM_IF_ASSERT(prong),
												 TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideGuard(const ShortIndex HSFM_IF_ASSERT(prong),
										  TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepGuard(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideEnterInitial(Control& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideEnter(const ShortIndex HSFM_IF_ASSERT(prong),
										  Control& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepEnter(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
bool
_CS<TID, TC, TSL, TPL, NI, TI>::wideUpdate(const ShortIndex HSFM_IF_ASSERT(prong),
										   TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
template <typename TEvent>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideReact(const ShortIndex HSFM_IF_ASSERT(prong),
										  const TEvent& event,
										  TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideExit(const ShortIndex HSFM_IF_ASSERT(prong),
										 Control& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideForwardRequest(const ShortIndex HSFM_IF_ASSERT(prong),
												   const TransitionType transition)
{
	assert(prong == PRONG_INDEX);

	initial.deepForwardRequest(transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideRequestResume(const ShortIndex HSFM_IF_ASSERT(prong)) {
	assert(prong == PRONG_INDEX);

	initial.deepRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideChangeToRequested(const ShortIndex HSFM_IF_ASSERT(prong),
													  Control& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideGetNames(const LongIndex parent,
											 const ShortIndex depth,
											 StructureStateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideIsActive(const ShortIndex prong,
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

template <StateID THeadID,
		  typename TContext,
		  typename TStateList,
		  typename TPayloadList,
		  typename THead,
		  typename... TSubStates>
struct _C final {
	static constexpr StateID HEAD_ID = THeadID;

	using Context			= TContext;
	using Control			= ControlT<Context>;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;
	using StateRegistry2	= StateRegistry2T<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using TransitionControl	= TransitionControlT<Context, StateList, PayloadList>;
	using ControlLock		= typename TransitionControl::Lock;
	using Head				= THead;
	using Fork				= ForkT<Head>;
	using State				= _S <HEAD_ID,	   Context, StateList, PayloadList, Head>;
	using SubStates			= _CS<HEAD_ID + 1, Context, StateList, PayloadList, 0, TSubStates...>;
	using Forward			= _CF<Head, TSubStates...>;
	using DeepStateList		= typename Forward::StateList;

	static constexpr LongIndex REVERSE_DEPTH = SubStates::REVERSE_DEPTH + 1;
	static constexpr LongIndex DEEP_WIDTH	 = SubStates::DEEP_WIDTH;
	static constexpr LongIndex STATE_COUNT	 = State::STATE_COUNT + SubStates::STATE_COUNT;
	static constexpr LongIndex FORK_COUNT	 = SubStates::FORK_COUNT + 1;
	static constexpr LongIndex PRONG_COUNT	 = SubStates::PRONG_COUNT + sizeof...(TSubStates);
	static constexpr LongIndex WIDTH		 = sizeof...(TSubStates);

	_C(StateRegistry2& stateRegistry,
	   const Parent parent,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

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
	using RegionType		= typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT	 = State::NAME_COUNT + SubStates::NAME_COUNT;

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
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
	HSFM_IF_DEBUG(DeepStateList stateList);
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
_C<TID, TC, TSL, TPL, TH, TS...>::_C(StateRegistry2& stateRegistry,
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
				 forkParents,
				 forkPointers)
{}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepForwardGuard(TransitionControl& control) {
	assert(_fork.requested != INVALID_SHORT_INDEX);

	if (_fork.requested == _fork.active)
		_subStates.wideForwardGuard(_fork.requested, control);
	else
		_subStates.wideGuard	   (_fork.requested, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepGuard(TransitionControl& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.requested != INVALID_SHORT_INDEX);

	if (!_state	  .deepGuard(				  control))
		_subStates.wideGuard(_fork.requested, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepEnterInitial(Control& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX &&
		   _fork.requested == INVALID_SHORT_INDEX);

	_fork.active = 0;

	_state	  .deepEnter	   (control);
	_subStates.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepEnter(Control& control) {
	assert(_fork.active	   == INVALID_SHORT_INDEX &&
		   _fork.requested != INVALID_SHORT_INDEX);

	_fork.active	= _fork.requested;
	_fork.requested	= INVALID_SHORT_INDEX;

	_state	  .deepEnter(control);
	_subStates.wideEnter(_fork.active, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
bool
_C<TID, TC, TSL, TPL, TH, TS...>::deepUpdate(TransitionControl& control) {
	assert(_fork.active != INVALID_SHORT_INDEX);

	if (_state	  .deepUpdate(control)) {
		ControlLock lock(control);
		_subStates.wideUpdate(_fork.active, control);

		return true;
	} else
		return _subStates.wideUpdate(_fork.active, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
template <typename TEvent>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepReact(const TEvent& event,
											TransitionControl& control)
{
	assert(_fork.active != INVALID_SHORT_INDEX);

	_state	  .deepReact(			   event, control);
	_subStates.wideReact(_fork.active, event, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepExit(Control& control) {
	assert(_fork.active != INVALID_SHORT_INDEX);

	_subStates.wideExit(_fork.active, control);
	_state	  .deepExit(control);

	_fork.resumable	= _fork.active;
	_fork.active	= INVALID_SHORT_INDEX;
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepForwardRequest(const TransitionType transition) {
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

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepRequestRemain() {
	if (_fork.active == INVALID_SHORT_INDEX)
		_fork.requested = 0;

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepRequestRestart() {
	_fork.requested = 0;

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepRequestResume() {
	_fork.requested = _fork.resumable != INVALID_SHORT_INDEX ?
		_fork.resumable : 0;

	_subStates.wideRequestResume(_fork.requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepChangeToRequested(Control& control) {
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

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepGetNames(const LongIndex parent,
											   const RegionType region,
											   const ShortIndex depth,
											   StructureStateInfos& _stateInfos) const
{
	_state	  .deepGetNames(parent, region,			 depth,		_stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, depth + 1, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepIsActive(const bool isActive,
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

template <StateID, typename, typename, typename, ShortIndex, typename...>
struct _OS;

//------------------------------------------------------------------------------

template <StateID TInitialID,
		  typename TContext,
		  typename TStateList,
		  typename TPayloadList,
		  ShortIndex NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct _OS<TInitialID, TContext, TStateList, TPayloadList, NIndex, TInitial, TRemaining...> {
	static constexpr StateID INITIAL_ID = TInitialID;

	using Context			= TContext;
	using Control			= ControlT<Context>;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;
	using StateRegistry2	= StateRegistry2T<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using TransitionControl	= TransitionControlT<Context, StateList, PayloadList>;
	using Initial			= typename WrapMaterial<INITIAL_ID, Context, StateList, PayloadList, TInitial>::Type;
	using Remaining			= _OS<INITIAL_ID + Initial::STATE_COUNT, Context, StateList, PayloadList, NIndex + 1, TRemaining...>;
	using Forward			= _OSF<TInitial, TRemaining...>;
	using DeepStateList		= typename Forward::StateList;

	static constexpr LongIndex PRONG_INDEX	 = NIndex;
	static constexpr LongIndex REVERSE_DEPTH = Max<Initial::REVERSE_DEPTH, Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr LongIndex DEEP_WIDTH	 = Initial::DEEP_WIDTH  + Remaining::DEEP_WIDTH;
	static constexpr LongIndex STATE_COUNT	 = Initial::STATE_COUNT + Remaining::STATE_COUNT;
	static constexpr LongIndex FORK_COUNT	 = Initial::FORK_COUNT  + Remaining::FORK_COUNT;
	static constexpr LongIndex PRONG_COUNT	 = Initial::PRONG_COUNT + Remaining::PRONG_COUNT;

	_OS(StateRegistry2& stateRegistry,
		const ShortIndex fork,
		Parents& forkParents,
		ForkPointers& forkPointers);

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
	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT  + Remaining::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;

	void wideIsActive(const bool active,
					  LongIndex& index,
					  MachineStructure& structure) const;
#endif

	Initial initial;
	Remaining remaining;
	HSFM_IF_DEBUG(DeepStateList stateList);
};

//------------------------------------------------------------------------------

template <StateID TInitialID,
		  typename TContext,
		  typename TStateList,
		  typename TPayloadList,
		  ShortIndex NIndex,
		  typename TInitial>
struct _OS<TInitialID, TContext, TStateList, TPayloadList, NIndex, TInitial> {
	static constexpr StateID INITIAL_ID = TInitialID;

	using Context			= TContext;
	using Control			= ControlT<Context>;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;
	using StateRegistry2	= StateRegistry2T<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using TransitionControl	= TransitionControlT<Context, StateList, PayloadList>;
	using Initial			= typename WrapMaterial<INITIAL_ID, Context, StateList, PayloadList, TInitial>::Type;
	using Forward			= _OSF<TInitial>;
	using DeepStateList		= typename Forward::StateList;

	static constexpr LongIndex PRONG_INDEX	 = NIndex;
	static constexpr LongIndex REVERSE_DEPTH = Initial::REVERSE_DEPTH;
	static constexpr LongIndex DEEP_WIDTH	 = Initial::DEEP_WIDTH;
	static constexpr LongIndex STATE_COUNT	 = Initial::STATE_COUNT;
	static constexpr LongIndex FORK_COUNT	 = Initial::FORK_COUNT;
	static constexpr LongIndex PRONG_COUNT	 = Initial::PRONG_COUNT;

	_OS(StateRegistry2& stateRegistry,
		const ShortIndex fork,
		Parents& forkParents,
		ForkPointers& forkPointers);

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
	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;

	void wideIsActive(const bool active,
					  LongIndex& index,
					  MachineStructure& structure) const;
#endif

	Initial initial;
	HSFM_IF_DEBUG(DeepStateList stateList);
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::_OS(StateRegistry2& stateRegistry,
										   const ShortIndex fork,
										   Parents& forkParents,
										   ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork, PRONG_INDEX),
			  forkParents,
			  forkPointers)
	, remaining(stateRegistry,
				fork,
				forkParents,
				forkPointers)
{}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideForwardGuard(const ShortIndex prong,
														TransitionControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepForwardGuard(control);
	else
		remaining.wideForwardGuard(prong, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideForwardGuard(TransitionControl& control) {
	initial	 .deepForwardGuard(control);
	remaining.wideForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideGuard(TransitionControl& control) {
	initial	 .deepGuard(control);
	remaining.wideGuard(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideEnterInitial(Control& control) {
	initial  .deepEnterInitial(control);
	remaining.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideEnter(Control& control) {
	initial  .deepEnter(control);
	remaining.wideEnter(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
bool
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideUpdate(TransitionControl& control) {
	const bool r = initial.deepUpdate(control);

	return		 remaining.wideUpdate(control) || r;
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
template <typename TEvent>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideReact(const TEvent& event,
												 TransitionControl& control)
{
	initial  .deepReact(event, control);
	remaining.wideReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideExit(Control& control) {
	initial	 .deepExit(control);
	remaining.wideExit(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideForwardRequest(const ShortIndex prong,
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

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideRequestRemain() {
	initial	 .deepRequestRemain();
	remaining.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideRequestRestart() {
	initial	 .deepRequestRestart();
	remaining.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideRequestResume() {
	initial	 .deepRequestResume();
	remaining.wideRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideChangeToRequested(Control& control) {
	initial	 .deepChangeToRequested(control);
	remaining.wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideGetNames(const LongIndex parent,
													const ShortIndex depth,
													StructureStateInfos& _stateInfos) const
{
	initial	 .deepGetNames(parent, StructureStateInfo::ORTHOGONAL, depth, _stateInfos);
	remaining.wideGetNames(parent,								   depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideIsActive(const bool isActive,
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

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
_OS<TID, TC, TSL, TPL, NI, TI>::_OS(StateRegistry2& stateRegistry,
									const ShortIndex fork,
									Parents& forkParents,
									ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork, PRONG_INDEX),
			  forkParents,
			  forkPointers)
{}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideForwardGuard(const ShortIndex HSFM_IF_ASSERT(prong),
												 TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideForwardGuard(TransitionControl& control) {
	initial.deepForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideGuard(TransitionControl& control) {
	initial.deepGuard(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideEnterInitial(Control& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideEnter(Control& control) {
	initial.deepEnter(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
bool
_OS<TID, TC, TSL, TPL, NI, TI>::wideUpdate(TransitionControl& control) {
	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
template <typename TEvent>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideReact(const TEvent& event,
										  TransitionControl& control)
{
	initial.deepReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideExit(Control& control) {
	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideForwardRequest(const ShortIndex prong,
												   const TransitionType transition)
{
	assert(prong <= PRONG_INDEX);

	if (prong == PRONG_INDEX)
		initial.deepForwardRequest(transition);
	else
		initial.deepForwardRequest(Transition::REMAIN);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideRequestResume() {
	initial.deepRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideChangeToRequested(Control& control) {
	initial.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideGetNames(const LongIndex parent,
											 const ShortIndex depth,
											 StructureStateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StructureStateInfo::ORTHOGONAL, depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideIsActive(const bool isActive,
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

template <StateID THeadID,
		  typename TContext,
		  typename TStateList,
		  typename TPayloadList,
		  typename THead,
		  typename... TSubStates>
struct _O final {
	static constexpr StateID HEAD_ID = THeadID;

	using Context			= TContext;
	using Control			= ControlT<Context>;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;
	using StateRegistry2	= StateRegistry2T<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using TransitionControl	= TransitionControlT<Context, StateList, PayloadList>;
	using ControlLock		= typename TransitionControl::Lock;
	using Head				= THead;
	using Fork				= ForkT<Head>;
	using State				= _S <HEAD_ID,	   Context, StateList, PayloadList, Head>;
	using SubStates			= _OS<HEAD_ID + 1, Context, StateList, PayloadList, 0, TSubStates...>;
	using Forward			= _OF<Head, TSubStates...>;
	using DeepStateList		= typename Forward::StateList;

	static constexpr LongIndex REVERSE_DEPTH = SubStates::REVERSE_DEPTH + 1;
	static constexpr LongIndex DEEP_WIDTH	 = SubStates::DEEP_WIDTH;
	static constexpr LongIndex STATE_COUNT	 = State::STATE_COUNT + SubStates::STATE_COUNT;
	static constexpr LongIndex FORK_COUNT	 = SubStates::FORK_COUNT + 1;
	static constexpr LongIndex PRONG_COUNT	 = SubStates::PRONG_COUNT;
	static constexpr LongIndex WIDTH		 = sizeof...(TSubStates);

	_O(StateRegistry2& stateRegistry,
	   const Parent parent,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

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
	using RegionType		= typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT	 = State::NAME_COUNT  + SubStates::NAME_COUNT;

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;

	void deepIsActive(const bool isActive,
					  LongIndex& index,
					  MachineStructure& structure) const;
#endif

	Fork _fork;
	State _state;
	SubStates _subStates;

	HSFM_IF_DEBUG(DeepStateList stateList);
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
_O<TID, TC, TSL, TPL, TH, TS...>::_O(StateRegistry2& stateRegistry,
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
				 forkParents,
				 forkPointers)
{}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepForwardGuard(TransitionControl& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	if (_fork.requested != INVALID_SHORT_INDEX)
		_subStates.wideForwardGuard(_fork.requested, control);
	else
		_subStates.wideForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepGuard(TransitionControl& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	if (!_state	  .deepGuard(control))
		_subStates.wideGuard(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepEnterInitial(Control& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX &&
		   _fork.requested == INVALID_SHORT_INDEX);

	_state	  .deepEnter	   (control);
	_subStates.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepEnter(Control& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_state	  .deepEnter(control);
	_subStates.wideEnter(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
bool
_O<TID, TC, TSL, TPL, TH, TS...>::deepUpdate(TransitionControl& control) {
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

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
template <typename TEvent>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepReact(const TEvent& event,
											TransitionControl& control)
{
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_state	  .deepReact(event, control);
	_subStates.wideReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepExit(Control& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_subStates.wideExit(control);
	_state	  .deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepForwardRequest(const TransitionType transition) {
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

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepRequestRemain() {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepRequestRestart() {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepRequestResume() {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_subStates.wideRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepChangeToRequested(Control& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_subStates.wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepGetNames(const LongIndex parent,
											   const RegionType region,
											   const ShortIndex depth,
											   StructureStateInfos& _stateInfos) const
{
	_state	  .deepGetNames(parent, region,			 depth,		_stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, depth + 1, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepIsActive(const bool isActive,
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
	static constexpr ShortIndex MAX_SUBSTITUTIONS = TMaxSubstitutions;

	using Context			= TContext;
	using Control			= ControlT<Context>;
	using PayloadList		= TPayloadList;
	using StateRegistry2	= StateRegistry2T<PayloadList>;
	using Transition		= TransitionT<PayloadList>;

	using ForwardApex		= typename WrapForward<TApex>::Type;

	using StateList			= typename ForwardApex::StateList;
	using TransitionControl	= TransitionControlT<Context, StateList, PayloadList>;

	using Apex				= typename WrapMaterial<0, Context, StateList, PayloadList, TApex>::Type;

public:
	static constexpr LongIndex REVERSE_DEPTH  = Apex::REVERSE_DEPTH;
	static constexpr LongIndex DEEP_WIDTH	  = Apex::DEEP_WIDTH;
	static constexpr LongIndex STATE_COUNT	  = Apex::STATE_COUNT;
	static constexpr LongIndex FORK_COUNT	  = Apex::FORK_COUNT;
	static constexpr LongIndex PRONG_COUNT	  = Apex::PRONG_COUNT;
	static constexpr LongIndex WIDTH		  = Apex::WIDTH;

	static_assert(STATE_COUNT <  (ShortIndex) -1, "Too many states in the hierarchy. Change 'ShortIndex' type.");
	static_assert(STATE_COUNT == (ShortIndex) StateList::SIZE, "STATE_COUNT != StateList::SIZE");

private:
	using StateInfo				 = StateInfoT<PayloadList>;
	using StateRegistryStorage2	 = Array<StateInfo, STATE_COUNT>;

	static constexpr LongIndex STATE_CAPACITY = (LongIndex) 1.3 * Apex::STATE_COUNT;

	using ForkParentStorage		 = Array<Parent,	 FORK_COUNT>;
	using ForkPointerStorage	 = Array<Fork*,		 FORK_COUNT>;
	using TransitionQueueStorage = Array<Transition, FORK_COUNT>;

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static constexpr LongIndex NAME_COUNT	  = Apex::NAME_COUNT;

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
	stateId()									{ return StateList::template index<T>();	}

	void update();

	template <typename TEvent>
	inline void react(const TEvent& event);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	inline void changeTo(const StateID stateId);
	inline void resume	(const StateID stateId);
	inline void schedule(const StateID stateId);

	template <typename TState>
	inline void changeTo()							{ changeTo(stateId<TState>());			}

	template <typename TState>
	inline void resume	()							{ resume  (stateId<TState>());			}

	template <typename TState>
	inline void schedule()							{ schedule(stateId<TState>());			}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TPayload>
	inline void changeTo(const StateID stateId, TPayload* const payload);

	template <typename TPayload>
	inline void resume  (const StateID stateId, TPayload* const payload);

	template <typename TPayload>
	inline void schedule(const StateID stateId, TPayload* const payload);

	template <typename TState, typename TPayload>
	inline void changeTo(TPayload* const payload)	{ changeTo(stateId<TState>(), payload);	}

	template <typename TState, typename TPayload>
	inline void resume	(TPayload* const payload)	{ resume  (stateId<TState>(), payload);	}

	template <typename TState, typename TPayload>
	inline void schedule(TPayload* const payload)	{ schedule(stateId<TState>(), payload);	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	inline void resetStateData(const StateID stateId);

	template <typename TPayload>
	inline void setStateData(const StateID stateId, TPayload* const payload);

	inline bool isStateDataSet(const StateID stateId) const;

	template <typename TPayload>
	inline TPayload* getStateData(const StateID stateId) const;

	template <typename TState>
	inline void resetStateData()						{ resetStateData(stateId<TState>());				}

	template <typename TState, typename TPayload>
	inline void setStateData(TPayload* const payload)	{ setStateData(stateId<TState>(), payload);			}

	template <typename TState>
	inline bool isStateDataSet() const					{ return isStateDataSet(stateId<TState>());			}

	template <typename TState, typename TPayload>
	inline TPayload* getStateData() const				{ return getStateData<TPayload>(stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	inline bool isActive(const StateID stateId) const;

	template <typename T>
	inline bool isActive() const;

	inline bool isResumable(const StateID stateId) const;

	template <typename T>
	inline bool isResumable() const;

	inline bool isScheduled(const StateID stateId) const	{ return isResumable(stateId);	}

	template <typename T>
	inline bool isScheduled() const							{ return isResumable<T>();		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	const MachineStructure& structure() const						{ return _structure;		}
	const MachineActivity&  activity()  const						{ return _activityHistory;	}
#endif

#ifdef HFSM_ENABLE_LOG_INTERFACE
	void attachLogger(LoggerInterface* const logger)						{ _logger = logger;	}
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

	StateRegistryStorage2 _stateRegistry2;

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
	HSFM_IF_DEBUG(StateList stateList);
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
	, _apex{_stateRegistry2, Parent{}, _forkParents, _forkPointers}
	HFSM_IF_LOGGER(, _logger{logger})
{
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
_R<TC, TPL, TMS, TA>::changeTo(const StateID stateId)	{
	const Transition transition(Transition::Type::RESTART, stateId);
	_requests << transition;

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (_logger)
		_logger->recordTransition(LoggerInterface::Transition::RESTART, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::resume(const StateID stateId) {
	const Transition transition(Transition::Type::RESUME, stateId);
	_requests << transition;

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (_logger)
		_logger->recordTransition(LoggerInterface::Transition::RESUME, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::schedule(const StateID stateId) {
	const Transition transition(Transition::Type::SCHEDULE, stateId);
	_requests << transition;

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (_logger)
		_logger->recordTransition(LoggerInterface::Transition::SCHEDULE, stateId);
#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename TPayload>
void
_R<TC, TPL, TMS, TA>::changeTo(const StateID stateId,
							   TPayload* const payload)
{
	const Transition transition(Transition::Type::RESTART, stateId, payload);
	_requests << transition;

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (_logger)
		_logger->recordTransition(LoggerInterface::Transition::RESTART, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename TPayload>
void
_R<TC, TPL, TMS, TA>::resume(const StateID stateId,
							 TPayload* const payload)
{
	const Transition transition(Transition::Type::RESUME, stateId, payload);
	_requests << transition;

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (_logger)
		_logger->recordTransition(LoggerInterface::Transition::RESUME, state);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename TPayload>
void
_R<TC, TPL, TMS, TA>::schedule(const StateID stateId,
							   TPayload* const payload)
{
	const Transition transition(Transition::Type::SCHEDULE, stateId, payload);
	_requests << transition;

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (_logger)
		_logger->recordTransition(LoggerInterface::Transition::SCHEDULE, state);
#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::resetStateData(const StateID stateId) {
	auto& stateInfo = _stateRegistry2[stateId];

	stateInfo.payload.reset();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename TPayload>
void
_R<TC, TPL, TMS, TA>::setStateData(const StateID stateId,
								   TPayload* const payload)
{
	auto& stateInfo = _stateRegistry2[stateId];

	stateInfo.payload = payload;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
bool
_R<TC, TPL, TMS, TA>::isStateDataSet(const StateID stateId) const {
	const auto& stateInfo = _stateRegistry2[stateId];

	return !!stateInfo.payload;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename TPayload>
TPayload*
_R<TC, TPL, TMS, TA>::getStateData(const StateID stateId) const {
	const auto& stateInfo = _stateRegistry2[stateId];
	const auto& payload = stateInfo.payload;

	return payload.template get<TPayload>();
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
bool
_R<TC, TPL, TMS, TA>::isActive(const StateID stateId) const {
	const auto& stateInfo = _stateRegistry2[stateId];

	for (auto parent = stateInfo.parent; parent; parent = _forkParents[parent.fork]) {
		const auto& fork = *_forkPointers[parent.fork];

		if (fork.active != INVALID_SHORT_INDEX)
			return parent.prong == fork.active;
	}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename T>
bool
_R<TC, TPL, TMS, TA>::isActive() const {
	constexpr auto id = stateId<T>();
	static_assert(id != INVALID_STATE_ID, "State not in FSM");

	return isActive(id);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
bool
_R<TC, TPL, TMS, TA>::isResumable(const StateID stateId) const {
	const auto& stateInfo = _stateRegistry2[stateId];

	for (auto parent = stateInfo.parent; parent; parent = _forkParents[parent.fork]) {
		const auto& fork = *_forkPointers[parent.fork];

		if (fork.active != INVALID_SHORT_INDEX)
			return parent.prong == fork.resumable;
	}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename T>
bool
_R<TC, TPL, TMS, TA>::isResumable() const {
	constexpr auto id = stateId<T>();
	static_assert(id != INVALID_STATE_ID, "State not in FSM");

	return isResumable(id);
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
	const auto& stateInfo = _stateRegistry2[request.stateId];

	for (auto parent = stateInfo.parent; parent; parent = _forkParents[parent.fork]) {
		auto& fork = *_forkPointers[parent.fork];

		fork.requested = parent.prong;
	}

	_apex.deepForwardRequest(request.type);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::requestScheduled(const Transition request) {
	const auto& stateInfo = _stateRegistry2[request.stateId];

	const auto parent = stateInfo.parent;
	auto& fork = *_forkPointers[parent.fork];

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
