#pragma once

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class InjectionT {
	template <typename...>
	friend struct B_;

protected:
	using Context		= typename TArgs::Context;
	using Rank			= typename TArgs::Rank;
	using Utility		= typename TArgs::Utility;
	using StateList		= typename TArgs::StateList;
	using RegionList	= typename TArgs::RegionList;

	using Control		= ControlT<TArgs>;

	using PlanControl	= PlanControlT<TArgs>;
	using Plan			= PlanT<TArgs>;

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

public:
	HFSM_INLINE void preEntryGuard(Context&)		{}

	HFSM_INLINE void preEnter	  (Context&)		{}
	HFSM_INLINE void preReenter	  (Context&)		{}

	HFSM_INLINE void preUpdate	  (Context&)		{}

	template <typename TEvent>
	HFSM_INLINE void preReact	  (const TEvent&,
								   Context&)		{}

	HFSM_INLINE void preExitGuard (Context&)		{}

	HFSM_INLINE void postExit	  (Context&)		{}

	template <typename T>
	static constexpr StateID  stateId()		{ return			StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()	{ return (RegionID) RegionList::template index<T>();	}
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
	using typename TFirst::Rank;
	using typename TFirst::Utility;
	using typename TFirst::StateList;
	using typename TFirst::RegionList;

	using typename TFirst::Control;
	using typename TFirst::PlanControl;
	using typename TFirst::Plan;
	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;
	using TFirst::regionId;

	HFSM_INLINE void widePreEntryGuard(Context& context);

	HFSM_INLINE void widePreEnter	  (Context& context);
	HFSM_INLINE void widePreReenter	  (Context& context);

	HFSM_INLINE void widePreUpdate	  (Context& context);

	template <typename TEvent>
	HFSM_INLINE void widePreReact	  (const TEvent& event,
									   Context& context);

	HFSM_INLINE void widePreExitGuard (Context& context);

	HFSM_INLINE void widePostExit	  (Context& context);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TFirst>
struct B_<TFirst>
	: TFirst
{
	using typename TFirst::Context;
	using typename TFirst::Rank;
	using typename TFirst::Utility;
	using typename TFirst::StateList;
	using typename TFirst::RegionList;

	using typename TFirst::Control;
	using typename TFirst::PlanControl;
	using typename TFirst::Plan;
	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;
	using TFirst::regionId;

	HFSM_INLINE Rank	rank			 (const Control&)			{ return Rank	   {0};	}

	HFSM_INLINE Utility utility			 (const Control&)			{ return Utility{1.0f};	}

	HFSM_INLINE void	entryGuard		 (GuardControl&)			{}

	HFSM_INLINE void	enter			 (PlanControl&)				{}
	HFSM_INLINE void	reenter			 (PlanControl&)				{}

	HFSM_INLINE void	update			 (FullControl&)				{}

	template <typename TEvent>
	HFSM_INLINE void	react			 (const TEvent&,
										  FullControl&)				{}

	HFSM_INLINE void	exitGuard		 (GuardControl&)			{}

	HFSM_INLINE void	exit			 (PlanControl&)				{}

	HFSM_INLINE void	planSucceeded	 (FullControl& control)		{ control.succeed();	}
	HFSM_INLINE void	planFailed		 (FullControl& control)		{ control.fail();		}

	HFSM_INLINE void	widePreEntryGuard(Context& context);

	HFSM_INLINE void	widePreEnter	 (Context& context);
	HFSM_INLINE void	widePreReenter   (Context& context);

	HFSM_INLINE void	widePreUpdate	 (Context& context);

	template <typename TEvent>
	HFSM_INLINE void	widePreReact	 (const TEvent& event,
					 					  Context& context);

	HFSM_INLINE void	widePreExitGuard (Context& context);

	HFSM_INLINE void	widePostExit	 (Context& context);
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

#include "injections.inl"
