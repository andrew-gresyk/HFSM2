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
	using Prong			= ::hfsm2::Prong;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank			= typename TArgs::Rank;
	using Utility		= typename TArgs::Utility;
#endif

	using StateList		= typename TArgs::StateList;
	using RegionList	= typename TArgs::RegionList;

	using Payload		= typename TArgs::Payload;
	using Transition	= TransitionT<Payload>;

	using ConstControl	= ConstControlT<TArgs>;
	using Control		= ControlT	   <TArgs>;
	using PlanControl	= PlanControlT <TArgs>;
	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;
	using EventControl	= EventControlT<TArgs>;

#if HFSM2_PLANS_AVAILABLE()
	using Plan			= PayloadPlanT <TArgs>;
#endif

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void	 entryGuard	  (GuardControl&		 )			noexcept	{}

	HFSM2_CONSTEXPR(14)	void	 enter		  ( PlanControl&		 )			noexcept	{}
	HFSM2_CONSTEXPR(14)	void	 reenter	  ( PlanControl&		 )			noexcept	{}

	HFSM2_CONSTEXPR(14)	void	 preUpdate	  ( FullControl&		 )			noexcept	{}
	HFSM2_CONSTEXPR(14)	void	 update		  ( FullControl&		 )			noexcept	{}
	HFSM2_CONSTEXPR(14)	void	 postUpdate	  ( FullControl&		 )			noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	 preReact	  (const TEvent&		 ,
											   EventControl&		 )			noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	 react		  (const TEvent&		 ,
											   EventControl&		 )			noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	 postReact	  (const TEvent&		 ,
											   EventControl&		 )			noexcept	{}

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	 query		  (		 TEvent&		 ,
								 			   ConstControl&		 )	  const noexcept	{}

	HFSM2_CONSTEXPR(14)	void	 exitGuard	  (GuardControl&		 )			noexcept	{}

	HFSM2_CONSTEXPR(14)	void	 exit		  ( PlanControl&		 )			noexcept	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TState>
	static
	HFSM2_CONSTEXPR(11) StateID  stateId()										noexcept	{ return					   index<StateList , TState>() ;	}

	template <typename TState>
	static
	HFSM2_CONSTEXPR(11) RegionID regionId()										noexcept	{ return static_cast<RegionID>(index<RegionList, TState>());	}
};

////////////////////////////////////////////////////////////////////////////////

}
}
