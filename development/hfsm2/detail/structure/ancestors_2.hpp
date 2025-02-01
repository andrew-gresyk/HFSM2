namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TFirst>
struct HFSM2_EMPTY_BASES A_<TFirst>
	: TFirst
{
	using First		= TFirst;
	using typename First::Context;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename First::Rank;
	using typename First::Utility;
#endif

	using typename First::StateList;
	using typename First::RegionList;

	using typename First::Payload;
	using typename First::Transition;

	using typename First::ConstControl;
	using typename First::Control;
	using typename First::PlanControl;

#if HFSM2_PLANS_AVAILABLE()
	using typename First::Plan;
#endif

	using typename First::FullControl;
	using typename First::GuardControl;
	using typename First::EventControl;

	using First::stateId;
	using First::regionId;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	Prong	 select		   (const Control&		 )			noexcept	{ return 0;					}

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	Rank	 rank		   (const Control&		 )			noexcept	{ return Rank   {0};		}
	HFSM2_CONSTEXPR(14)	Utility  utility	   (const Control&		 )			noexcept	{ return Utility{1};		}
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void	 entryGuard	   (GuardControl&		 )			noexcept	{}

	HFSM2_CONSTEXPR(14)	void	 enter		   ( PlanControl&		 )			noexcept	{}
	HFSM2_CONSTEXPR(14)	void	 reenter	   ( PlanControl&		 )			noexcept	{}

	HFSM2_CONSTEXPR(14)	void	 preUpdate	   ( FullControl&		 )			noexcept	{}
	HFSM2_CONSTEXPR(14)	void	 update		   ( FullControl&		 )			noexcept	{}
	HFSM2_CONSTEXPR(14)	void	 postUpdate	   ( FullControl&		 )			noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	 preReact	   (const TEvent&		 ,
												EventControl&		 )			noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	 react		   (const TEvent&		 ,
												EventControl&		 )			noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	 postReact	   (const TEvent&		 ,
												EventControl&		 )			noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	 query		   (	  TEvent&		 ,
											    ConstControl&		 )	  const noexcept	{}

	HFSM2_CONSTEXPR(14)	void	 exitGuard	   (GuardControl&		 )			noexcept	{}

	HFSM2_CONSTEXPR(14)	void	 exit		   ( PlanControl&		 )			noexcept	{}

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void	 planSucceeded ( FullControl& control)			noexcept;
	HFSM2_CONSTEXPR(14)	void	 planFailed	   ( FullControl& control)			noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void	 wideEntryGuard(GuardControl& control)			noexcept;

	HFSM2_CONSTEXPR(14)	void	 wideEnter	   ( PlanControl& control)			noexcept;
	HFSM2_CONSTEXPR(14)	void	 wideReenter   ( PlanControl& control)			noexcept;

	HFSM2_CONSTEXPR(14)	void	 widePreUpdate ( FullControl& control)			noexcept;
	HFSM2_CONSTEXPR(14)	void	 wideUpdate	   ( FullControl& control)			noexcept;
	HFSM2_CONSTEXPR(14)	void	 widePostUpdate( FullControl& control)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	 widePreReact  (const TEvent& event	 ,
												EventControl& control)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	 wideReact	   (const TEvent& event	 ,
												EventControl& control)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	 widePostReact (const TEvent& event	 ,
												EventControl& control)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	 wideQuery	   (	  TEvent& event	 ,
												ConstControl& control)	  const noexcept;

	HFSM2_CONSTEXPR(14)	void	 wideExitGuard (GuardControl& control)			noexcept;

	HFSM2_CONSTEXPR(14)	void	 wideExit	   ( PlanControl& control)			noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "ancestors_2.inl"
