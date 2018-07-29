#pragma once

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

#include "machine_injections.inl"
