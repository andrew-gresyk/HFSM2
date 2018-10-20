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

	using Control			= ControlT	  <Args>;
	using FullControl		= FullControlT<Args>;

	using Plan				= typename Control::Plan;

public:
	HSFM_INLINE void preGuard (Context&)										{}
	HSFM_INLINE void preEnter (Context&)										{}
	HSFM_INLINE void preUpdate(Context&)										{}
	template <typename TEvent>
	HSFM_INLINE void preReact (const TEvent&,
							   Context&)										{}
	HSFM_INLINE void postExit (Context&)										{}
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

	HSFM_INLINE void widePreGuard (typename First::Context& context);
	HSFM_INLINE void widePreEnter (typename First::Context& context);
	HSFM_INLINE void widePreUpdate(typename First::Context& context);
	template <typename TEvent>
	HSFM_INLINE void widePreReact (const TEvent& event,
								   typename First::Context& context);
	HSFM_INLINE void widePostExit (typename First::Context& context);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TFirst>
struct _B<TFirst>
	: TFirst
{
	using First				= TFirst;

	using StateList			= typename First::StateList;
	using RegionList		= typename First::RegionList;

	HSFM_INLINE void guard		  (typename First::FullControl&)				{}
	HSFM_INLINE void enter		  (typename First::Control&)					{}
	HSFM_INLINE void update		  (typename First::FullControl&)				{}
	template <typename TEvent>
	HSFM_INLINE void react		  (const TEvent&,
								   typename First::FullControl&)				{}
	HSFM_INLINE void exit		  (typename First::Control&)					{}

	HSFM_INLINE void planSucceeded(typename First::FullControl& control) { control.succeed(); }
	HSFM_INLINE void planFailed	  (typename First::FullControl& control) { control.fail();	  }

	HSFM_INLINE void widePreGuard (typename First::Context& context);
	HSFM_INLINE void widePreEnter (typename First::Context& context);
	HSFM_INLINE void widePreUpdate(typename First::Context& context);
	template <typename TEvent>
	HSFM_INLINE void widePreReact (const TEvent& event,
								   typename First::Context& context);
	HSFM_INLINE void widePostExit (typename First::Context& context);

	template <typename T>
	static constexpr LongIndex stateId()  { return StateList ::template index<T>();	}

	template <typename T>
	static constexpr LongIndex regionId() { return RegionList::template index<T>();	}
};

template <typename TArgs>
using Empty = _B<BareT<TArgs>>;

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_injections.inl"
