namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID, typename, ShortIndex, typename...>
struct _CS;

//------------------------------------------------------------------------------

template <StateID TInitialID,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct _CS<TInitialID, TArgs, NIndex, TInitial, TRemaining...> {
	static constexpr StateID INITIAL_ID		 = TInitialID;
	static constexpr LongIndex PRONG_INDEX	 = NIndex;

	using Args				= TArgs;

	using Context			= typename Args::Context;
	using Config			= typename Args::Config;
	using StateList			= typename Args::StateList;
	using PayloadList		= typename Args::PayloadList;

	using StateRegistry		= Array<Parent, StateList::SIZE>;
	using PlanControl		= PlanControlT<Context, StateList, Args::PLAN_CAPACITY>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using FullControl		= FullControlT<Context, StateList, PayloadList, Args::PLAN_CAPACITY>;

	using Initial			= typename WrapMaterial<INITIAL_ID, Args, TInitial>::Type;
	using InitialForward	= typename WrapForward<TInitial>::Type;
	using Remaining			= _CS<INITIAL_ID + InitialForward::STATE_COUNT, Args, NIndex + 1, TRemaining...>;
	using Forward			= _CSF<TInitial, TRemaining...>;

	_CS(StateRegistry& stateRegistry,
		const ShortIndex fork,
		Parents& forkParents,
		ForkPointers& forkPointers);

	inline void   wideForwardGuard		(const ShortIndex prong, FullControl& control);
	inline void   wideGuard				(const ShortIndex prong, FullControl& control);

	inline void   wideEnterInitial		(						 PlanControl& control);
	inline void   wideEnter				(const ShortIndex prong, PlanControl& control);

	inline Status wideUpdate			(const ShortIndex prong, FullControl& control);

	template <typename TEvent>
	inline void   wideReact				(const ShortIndex prong, const TEvent& event,
																 FullControl& control);

	inline void   wideExit				(const ShortIndex prong, PlanControl& control);

	inline void   wideForwardRequest	(const ShortIndex prong, const TransitionType transition);
	inline void   wideRequestRemain		();
	inline void   wideRequestRestart	();
	inline void   wideRequestResume		(const ShortIndex prong);
	inline void   wideChangeToRequested	(const ShortIndex prong, PlanControl& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT  + Remaining::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	Initial initial;
	Remaining remaining;
};

//------------------------------------------------------------------------------

template <StateID TInitialID,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename TInitial>
struct _CS<TInitialID, TArgs, NIndex, TInitial> {
	static constexpr StateID INITIAL_ID		 = TInitialID;
	static constexpr LongIndex PRONG_INDEX	 = NIndex;

	using Args				= TArgs;

	using Context			= typename Args::Context;
	using Config			= typename Args::Config;
	using StateList			= typename Args::StateList;
	using PayloadList		= typename Args::PayloadList;

	using StateRegistry		= Array<Parent, StateList::SIZE>;
	using PlanControl		= PlanControlT<Context, StateList, Args::PLAN_CAPACITY>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using FullControl		= FullControlT<Context, StateList, PayloadList, Args::PLAN_CAPACITY>;

	using Initial			= typename WrapMaterial<INITIAL_ID, Args, TInitial>::Type;
	using Forward			= _CSF<TInitial>;

	_CS(StateRegistry& stateRegistry,
		const ShortIndex fork,
		Parents& forkParents,
		ForkPointers& forkPointers);

	inline void   wideForwardGuard		(const ShortIndex prong, FullControl& control);
	inline void   wideGuard				(const ShortIndex prong, FullControl& control);

	inline void   wideEnterInitial		(						 PlanControl& control);
	inline void   wideEnter				(const ShortIndex prong, PlanControl& control);

	inline Status wideUpdate			(const ShortIndex prong, FullControl& control);

	template <typename TEvent>
	inline void   wideReact				(const ShortIndex prong, const TEvent& event,
				  												 FullControl& control);

	inline void   wideExit				(const ShortIndex prong, PlanControl& control);

	inline void   wideForwardRequest	(const ShortIndex prong, const TransitionType transition);
	inline void   wideRequestRemain		();
	inline void   wideRequestRestart	();
	inline void   wideRequestResume		(const ShortIndex prong);
	inline void   wideChangeToRequested	(const ShortIndex prong, PlanControl& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	Initial initial;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_composite_sub_1.inl"
#include "machine_composite_sub_2.inl"
