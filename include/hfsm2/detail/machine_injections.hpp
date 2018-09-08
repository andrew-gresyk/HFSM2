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
	using RegionList		= typename First::RegionList;

	inline void guard		 (typename First::FullControl&)						{}
	inline void enter		 (typename First::Control&)							{}
	inline void update		 (typename First::FullControl&)						{}
	template <typename TEvent>
	inline void react		 (const TEvent&,
							  typename First::FullControl&)						{}
	inline void exit		 (typename First::Control&)							{}

	inline void planSucceeded(typename First::FullControl& control) { control.succeed(); }
	inline void planFailed	 (typename First::FullControl& control) { control.fail();	 }

	inline void widePreGuard (typename First::Context& context);
	inline void widePreEnter (typename First::Context& context);
	inline void widePreUpdate(typename First::Context& context);
	template <typename TEvent>
	inline void widePreReact (const TEvent& event,
							  typename First::Context& context);
	inline void widePostExit (typename First::Context& context);

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
