#pragma once

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

	using Control			= ControlT<Args>;

	using PlanControl		= PlanControlT<Args>;
	using Plan				= PlanT<Args>;

	using FullControl		= FullControlT <Args>;
	using GuardControl		= GuardControlT<Args>;


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

	HFSM_INLINE float utility		   (const typename First::Control&)			{ return 0.0f;			}

	HFSM_INLINE void  entryGuard	   (typename First::GuardControl&)			{}
	HFSM_INLINE void  enter			   (typename First::PlanControl&)			{}
	HFSM_INLINE void  update		   (typename First::FullControl&)			{}
	template <typename TEvent>
	HFSM_INLINE void  react			   (const TEvent&,
					 				    typename First::FullControl&)			{}
	HFSM_INLINE void  exitGuard		   (typename First::GuardControl&)			{}
	HFSM_INLINE void  exit			   (typename First::PlanControl&)			{}

	HFSM_INLINE void  planSucceeded	   (typename First::FullControl& control)	{ control.succeed();	}
	HFSM_INLINE void  planFailed	   (typename First::FullControl& control)	{ control.fail();		}

	HFSM_INLINE void  widePreEntryGuard(typename First::Context& context);
	HFSM_INLINE void  widePreEnter	   (typename First::Context& context);
	HFSM_INLINE void  widePreUpdate	   (typename First::Context& context);
	template <typename TEvent>
	HFSM_INLINE void  widePreReact	   (const TEvent& event,
					 				    typename First::Context& context);
	HFSM_INLINE void  widePreExitGuard (typename First::Context& context);
	HFSM_INLINE void  widePostExit	   (typename First::Context& context);

	template <typename T>
	static constexpr StateID  stateId()			{ return			StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()		{ return (RegionID) RegionList::template index<T>();	}
};

template <typename TArgs>
using Empty = _B<BareT<TArgs>>;

////////////////////////////////////////////////////////////////////////////////

}
}

#include "injections.inl"
