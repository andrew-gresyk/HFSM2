#pragma once

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

#include "machine_injections.inl"
