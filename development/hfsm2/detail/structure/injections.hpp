namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class InjectionT {
	template <typename...>
	friend struct B_;

protected:
	using Context		= typename TArgs::Context;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank			= typename TArgs::Rank;
	using Utility		= typename TArgs::Utility;
#endif

	using StateList		= typename TArgs::StateList;
	using RegionList	= typename TArgs::RegionList;

	using Control		= ControlT<TArgs>;
	using PlanControl	= PlanControlT<TArgs>;

#if HFSM2_PLANS_AVAILABLE()
	using Plan			= PlanT<TArgs>;
#endif

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

public:
	HFSM2_CONSTEXPR(14)	void preEntryGuard(Context&)	  noexcept {}

	HFSM2_CONSTEXPR(14)	void preEnter	  (Context&)	  noexcept {}
	HFSM2_CONSTEXPR(14)	void preReenter	  (Context&)	  noexcept {}

	HFSM2_CONSTEXPR(14)	void preUpdate	  (Context&)	  noexcept {}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void preReact	  (const TEvent&,
										   Context&)	  noexcept {}

	HFSM2_CONSTEXPR(14)	void preExitGuard (Context&)	  noexcept {}

	HFSM2_CONSTEXPR(14)	void postExit	  (Context&)	  noexcept {}

	template <typename T>
	static constexpr StateID  stateId()					  noexcept { return index<StateList, T>();				}

	template <typename T>
	static constexpr RegionID regionId()				  noexcept { return (RegionID) index<RegionList, T>();	}
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

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename TFirst::Rank;
	using typename TFirst::Utility;
#endif

	using typename TFirst::StateList;
	using typename TFirst::RegionList;

	using typename TFirst::Control;
	using typename TFirst::PlanControl;

#if HFSM2_PLANS_AVAILABLE()
	using typename TFirst::Plan;
#endif

	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;
	using TFirst::regionId;

	HFSM2_CONSTEXPR(14)	void widePreEntryGuard(Context& context)		 noexcept;

	HFSM2_CONSTEXPR(14)	void widePreEnter	  (Context& context)		 noexcept;
	HFSM2_CONSTEXPR(14)	void widePreReenter   (Context& context)		 noexcept;

	HFSM2_CONSTEXPR(14)	void widePreUpdate	  (Context& context)		 noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void widePreReact	  (const TEvent& event,
											   Context& context)		 noexcept;

	HFSM2_CONSTEXPR(14)	void widePreExitGuard (Context& context)		 noexcept;

	HFSM2_CONSTEXPR(14)	void widePostExit	  (Context& context)		 noexcept;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TFirst>
struct B_<TFirst>
	: TFirst
{
	using typename TFirst::Context;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename TFirst::Rank;
	using typename TFirst::Utility;
#endif

	using typename TFirst::StateList;
	using typename TFirst::RegionList;

	using typename TFirst::Control;
	using typename TFirst::PlanControl;

#if HFSM2_PLANS_AVAILABLE()
	using typename TFirst::Plan;
#endif

	using typename TFirst::FullControl;
	using typename TFirst::GuardControl;

	using TFirst::stateId;
	using TFirst::regionId;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	Rank	rank			 (const Control&)		  noexcept { return Rank   {0};		}
	HFSM2_CONSTEXPR(14)	Utility utility			 (const Control&)		  noexcept { return Utility{1};		}
#endif

	HFSM2_CONSTEXPR(14)	void	entryGuard		 (GuardControl&)		  noexcept {}

	HFSM2_CONSTEXPR(14)	void	enter			 (PlanControl&)			  noexcept {}
	HFSM2_CONSTEXPR(14)	void	reenter			 (PlanControl&)			  noexcept {}

	HFSM2_CONSTEXPR(14)	void	update			 (FullControl&)			  noexcept {}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	react			 (const TEvent&,
												  FullControl&)			  noexcept {}

	HFSM2_CONSTEXPR(14)	void	exitGuard		 (GuardControl&)		  noexcept {}

	HFSM2_CONSTEXPR(14)	void	exit			 (PlanControl&)			  noexcept {}

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void	planSucceeded	 (FullControl& control)	  noexcept { control.succeed();		}
	HFSM2_CONSTEXPR(14)	void	planFailed		 (FullControl& control)	  noexcept { control.fail();		}
#endif

	HFSM2_CONSTEXPR(14)	void	widePreEntryGuard(Context& context)		  noexcept;

	HFSM2_CONSTEXPR(14)	void	widePreEnter	 (Context& context)		  noexcept;
	HFSM2_CONSTEXPR(14)	void	widePreReenter   (Context& context)		  noexcept;

	HFSM2_CONSTEXPR(14)	void	widePreUpdate	 (Context& context)		  noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	widePreReact	 (const TEvent& event,
												  Context& context)		  noexcept;

	HFSM2_CONSTEXPR(14)	void	widePreExitGuard (Context& context)		  noexcept;

	HFSM2_CONSTEXPR(14)	void	widePostExit	 (Context& context)		  noexcept;
};

//------------------------------------------------------------------------------

template <typename TArgs>
using EmptyT = B_<InjectionT<TArgs>>;

////////////////////////////////////////////////////////////////////////////////

}
}

#include "injections.inl"
