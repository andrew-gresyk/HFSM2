#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class Bare {
	template <typename...>
	friend struct _B;

protected:
	using Args				= TArgs;
	using Context			= typename Args::Context;
	using StateList			= typename Args::StateList;

	using Control			= ControlT			<Args>;
	using PlanControl		= PlanControlT		<Args>;
	using TransitionControl = TransitionControlT<Args>;
	using FullControl		= FullControlT		<Args>;

	using Plan				= typename PlanControl::Plan;

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
	inline void enter		 (typename First::PlanControl&)						{}
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

template <typename TArgs>
using Empty = _B<Bare<TArgs>>;

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_injections.inl"
