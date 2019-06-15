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
	using Utility		= typename TArgs::Utility;
	using StateList		= typename TArgs::StateList;
	using RegionList	= typename TArgs::RegionList;

	using Control		= ControlT<TArgs>;

	using PlanControl	= PlanControlT<TArgs>;
	using Plan			= PlanT<TArgs>;

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

public:
	HFSM_INLINE void preEntryGuard(Context&)									{}

	HFSM_INLINE void preEnter	  (Context&)									{}
	HFSM_INLINE void preReenter	  (Context&)									{}

	HFSM_INLINE void preUpdate	  (Context&)									{}

	template <typename TEvent>
	HFSM_INLINE void preReact	  (const TEvent&,
								   Context&)									{}

	HFSM_INLINE void preExitGuard (Context&)									{}

	HFSM_INLINE void postExit	  (Context&)									{}
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

	HFSM_INLINE void widePreEntryGuard(typename TFirst::Context& context);

	HFSM_INLINE void widePreEnter	  (typename TFirst::Context& context);
	HFSM_INLINE void widePreReenter	  (typename TFirst::Context& context);

	HFSM_INLINE void widePreUpdate	  (typename TFirst::Context& context);

	template <typename TEvent>
	HFSM_INLINE void widePreReact	  (const TEvent& event,
									   typename TFirst::Context& context);

	HFSM_INLINE void widePreExitGuard (typename TFirst::Context& context);

	HFSM_INLINE void widePostExit	  (typename TFirst::Context& context);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TFirst>
struct B_<TFirst>
	: TFirst
{
	HFSM_INLINE float utility		   (const typename TFirst::Control&)		{ return typename TFirst::Utility{1.0f};	}

	HFSM_INLINE void  entryGuard	   (typename TFirst::GuardControl&)			{}

	HFSM_INLINE void  enter			   (typename TFirst::PlanControl&)			{}
	HFSM_INLINE void  reenter		   (typename TFirst::PlanControl&)			{}

	HFSM_INLINE void  update		   (typename TFirst::FullControl&)			{}

	template <typename TEvent>
	HFSM_INLINE void  react			   (const TEvent&,
					 				    typename TFirst::FullControl&)			{}

	HFSM_INLINE void  exitGuard		   (typename TFirst::GuardControl&)			{}

	HFSM_INLINE void  exit			   (typename TFirst::PlanControl&)			{}

	HFSM_INLINE void  planSucceeded	   (typename TFirst::FullControl& control)	{ control.succeed();	}
	HFSM_INLINE void  planFailed	   (typename TFirst::FullControl& control)	{ control.fail();		}

	HFSM_INLINE void  widePreEntryGuard(typename TFirst::Context& context);

	HFSM_INLINE void  widePreEnter	   (typename TFirst::Context& context);
	HFSM_INLINE void  widePreReenter   (typename TFirst::Context& context);

	HFSM_INLINE void  widePreUpdate	   (typename TFirst::Context& context);

	template <typename TEvent>
	HFSM_INLINE void  widePreReact	   (const TEvent& event,
					 				    typename TFirst::Context& context);

	HFSM_INLINE void  widePreExitGuard (typename TFirst::Context& context);

	HFSM_INLINE void  widePostExit	   (typename TFirst::Context& context);

	template <typename T>
	static constexpr StateID  stateId()			{ return			typename TFirst::StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()		{ return (RegionID) typename TFirst::RegionList::template index<T>();	}
};

template <typename TArgs>
using Empty = B_<InjectionT<TArgs>>;

////////////////////////////////////////////////////////////////////////////////

}
}

#include "injections.inl"
