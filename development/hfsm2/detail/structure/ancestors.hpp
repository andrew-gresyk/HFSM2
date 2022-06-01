namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class B_ {
	template <typename...>
	friend struct A_;

protected:
	using Context		= typename TArgs::Context;

	using Short			= ::hfsm2::Short;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank			= typename TArgs::Rank;
	using Utility		= typename TArgs::Utility;
#endif

	using StateList		= typename TArgs::StateList;
	using RegionList	= typename TArgs::RegionList;

	using Control		= ControlT	  <TArgs>;
	using PlanControl	= PlanControlT<TArgs>;

#if HFSM2_PLANS_AVAILABLE()
	using Plan			= PlanT<TArgs>;
#endif

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void	entryGuard	  (GuardControl&		)	  noexcept	{}

	HFSM2_CONSTEXPR(14)	void	enter		  ( PlanControl&		)	  noexcept	{}
	HFSM2_CONSTEXPR(14)	void	reenter		  ( PlanControl&		)	  noexcept	{}

	HFSM2_CONSTEXPR(14)	void	preUpdate	  ( FullControl&		)	  noexcept	{}
	HFSM2_CONSTEXPR(14)	void	update		  ( FullControl&		)	  noexcept	{}
	HFSM2_CONSTEXPR(14)	void	postUpdate	  ( FullControl&		)	  noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	preReact	  (const TEvent&		,
												FullControl&		)	  noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	react		  (const TEvent&		,
												FullControl&		)	  noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	postReact	  (const TEvent&		,
												FullControl&		)	  noexcept	{}

	HFSM2_CONSTEXPR(14)	void	exitGuard	  (GuardControl&		)	  noexcept	{}

	HFSM2_CONSTEXPR(14)	void	exit		  ( PlanControl&		)	  noexcept	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TState>
	static constexpr StateID  stateId()									  noexcept	{ return					   index<StateList , TState>() ;	}

	template <typename TState>
	static constexpr RegionID regionId()								  noexcept	{ return static_cast<RegionID>(index<RegionList, TState>());	}
};

////////////////////////////////////////////////////////////////////////////////

template <typename...>
struct A_;

//------------------------------------------------------------------------------

template <typename TFirst, typename... TRest>
struct HFSM2_EMPTY_BASES A_<TFirst, TRest...>
	: TFirst
	, A_<TRest...>
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

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void	wideEntryGuard(GuardControl& control)	  noexcept;

	HFSM2_CONSTEXPR(14)	void	wideEnter	  ( PlanControl& control)	  noexcept;
	HFSM2_CONSTEXPR(14)	void	wideReenter	  ( PlanControl& control)	  noexcept;

	HFSM2_CONSTEXPR(14)	void	widePreUpdate ( FullControl& control)	  noexcept;
	HFSM2_CONSTEXPR(14)	void	wideUpdate	  ( FullControl& control)	  noexcept;
	HFSM2_CONSTEXPR(14)	void	widePostUpdate( FullControl& control)	  noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	widePreReact  (const TEvent& event,
												FullControl& control)	  noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	wideReact	  (const TEvent& event,
												FullControl& control)	  noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	widePostReact (const TEvent& event,
												FullControl& control)	  noexcept;

	HFSM2_CONSTEXPR(14)	void	wideExitGuard (GuardControl& control)	  noexcept;

	HFSM2_CONSTEXPR(14)	void	wideExit	  ( PlanControl& control)	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

//------------------------------------------------------------------------------

template <typename TFirst>
struct A_<TFirst>
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

	HFSM2_CONSTEXPR(14)	Short	select		  (const Control&		)	  noexcept	{ return 0;					}

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	Rank	rank		  (const Control&		)	  noexcept	{ return Rank   {0};		}
	HFSM2_CONSTEXPR(14)	Utility utility		  (const Control&		)	  noexcept	{ return Utility{1};		}
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void	entryGuard	  (GuardControl&		)	  noexcept	{}

	HFSM2_CONSTEXPR(14)	void	enter		  ( PlanControl&		)	  noexcept	{}
	HFSM2_CONSTEXPR(14)	void	reenter		  ( PlanControl&		)	  noexcept	{}

	HFSM2_CONSTEXPR(14)	void	preUpdate	  ( FullControl&		)	  noexcept	{}
	HFSM2_CONSTEXPR(14)	void	update		  ( FullControl&		)	  noexcept	{}
	HFSM2_CONSTEXPR(14)	void	postUpdate	  ( FullControl&		)	  noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	preReact	  (const TEvent&		,
											    FullControl&		)	  noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	react		  (const TEvent&		,
											    FullControl&		)	  noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	postReact	  (const TEvent&		,
											    FullControl&		)	  noexcept	{}

	HFSM2_CONSTEXPR(14)	void	exitGuard	  (GuardControl&		)	  noexcept	{}

	HFSM2_CONSTEXPR(14)	void	exit		  ( PlanControl&		)	  noexcept	{}

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void	planSucceeded ( FullControl& control)	  noexcept	{ control.succeed();		}
	HFSM2_CONSTEXPR(14)	void	planFailed	  ( FullControl& control)	  noexcept	{ control.fail();			}
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void	wideEntryGuard(GuardControl& control)	  noexcept;

	HFSM2_CONSTEXPR(14)	void	wideEnter	  ( PlanControl& control)	  noexcept;
	HFSM2_CONSTEXPR(14)	void	wideReenter	  ( PlanControl& control)	  noexcept;

	HFSM2_CONSTEXPR(14)	void	widePreUpdate ( FullControl& control)	  noexcept;
	HFSM2_CONSTEXPR(14)	void	wideUpdate	  ( FullControl& control)	  noexcept;
	HFSM2_CONSTEXPR(14)	void	widePostUpdate( FullControl& control)	  noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	widePreReact  (const TEvent& event,
												FullControl& control)	  noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	wideReact	  (const TEvent& event,
												FullControl& control)	  noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	widePostReact (const TEvent& event,
												FullControl& control)	  noexcept;

	HFSM2_CONSTEXPR(14)	void	wideExitGuard (GuardControl& control)	  noexcept;

	HFSM2_CONSTEXPR(14)	void	wideExit	  ( PlanControl& control)	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

//------------------------------------------------------------------------------

template <typename TArgs>
using EmptyT = A_<B_<TArgs>>;

////////////////////////////////////////////////////////////////////////////////

}
}

#include "ancestors_1.inl"
#include "ancestors_2.inl"
