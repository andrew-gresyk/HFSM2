#pragma once

//------------------------------------------------------------------------------

namespace std {

template <typename T>
class function;

}

//------------------------------------------------------------------------------

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <unsigned TIndex, typename... TTypes>
struct Get;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned TIndex, typename TFirst, typename... TRest>
struct Get<TIndex, TFirst, TRest...>
	: Get<TIndex - 1, TRest...>
{
	static_assert(TIndex <= 1 + sizeof...(TRest), "");
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TFirst, typename... TRest>
struct Get<0, TFirst, TRest...> {
	using Type = TFirst;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TLast>
struct Get<0, TLast> {
	using Type = TLast;
};

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct CallableTraits
	: CallableTraits<decltype(&T::operator())>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TReturn, typename... TArguments>
struct CallableTraits<TReturn(TArguments...)> {
	using Return = TReturn;
	using Argument = typename Get<0, TArguments...>::Type;
};

//------------------------------------------------------------------------------

template <typename TReturn, typename... TArguments>
struct CallableTraits<TReturn(*)(TArguments...)>
	: CallableTraits<TReturn(TArguments...)>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename ClassType, typename TReturn, typename... TArguments>
struct CallableTraits<TReturn(ClassType::*)(TArguments...)>
	: CallableTraits<TReturn(TArguments...)>
{
	typedef ClassType& Class;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename ClassType, typename TReturn, typename... TArguments>
struct CallableTraits<TReturn(ClassType::*)(TArguments...) const>
	: CallableTraits<TReturn(TArguments...)>
{
	typedef const ClassType& Class;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename FunctionType>
struct CallableTraits<std::function<FunctionType>>
	: CallableTraits<FunctionType>
{};

////////////////////////////////////////////////////////////////////////////////

}
}
