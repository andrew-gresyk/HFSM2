namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename...>
struct A_;

template <typename TArgs>
using EmptyT = A_<B_<TArgs>>;

//------------------------------------------------------------------------------

template <typename TFirst, typename... TRest>
struct HFSM2_EMPTY_BASES A_<TFirst, TRest...>
	: TFirst
	, A_<TRest...>
{
	using First		= TFirst;
	using typename First::Context;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename First::Rank;
	using typename First::Utility;
#endif

	using typename First::StateList;
	using typename First::RegionList;

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

	using Rest		= A_<TRest...>;

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

#include "ancestors_1.inl"
