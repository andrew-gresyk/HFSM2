namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID, typename, typename, typename, ShortIndex, typename...>
struct _CS;

//------------------------------------------------------------------------------

template <StateID TInitialID,
		  typename TContext,
		  typename TStateList,
		  typename TPayloadList,
		  ShortIndex NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct _CS<TInitialID, TContext, TStateList, TPayloadList, NIndex, TInitial, TRemaining...> {
	static constexpr StateID INITIAL_ID = TInitialID;

	using Context			= TContext;
	using Control			= ControlT<Context>;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;
	using StateRegistry		= Array<Parent, StateList::SIZE>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using TransitionControl	= TransitionControlT<Context, StateList, PayloadList>;
	using Initial			= typename WrapMaterial<INITIAL_ID, Context, StateList, PayloadList, TInitial>::Type;
	using InitialForward	= typename WrapForward<TInitial>::Type;
	using Remaining			= _CS<INITIAL_ID + InitialForward::STATE_COUNT, Context, StateList, PayloadList, NIndex + 1, TRemaining...>;
	using Forward			= _CSF<TInitial, TRemaining...>;
	using OwnStateList		= typename Forward::StateList;

	static constexpr LongIndex PRONG_INDEX	 = NIndex;

	_CS(StateRegistry& stateRegistry,
		const ShortIndex fork,
		Parents& forkParents,
		ForkPointers& forkPointers);

	inline void wideForwardGuard	 (const ShortIndex prong, TransitionControl& control);
	inline void wideGuard			 (const ShortIndex prong, TransitionControl& control);

	inline void wideEnterInitial	 (						  Control& control);
	inline void wideEnter			 (const ShortIndex prong, Control& control);

	inline bool wideUpdate			 (const ShortIndex prong, TransitionControl& control);

	template <typename TEvent>
	inline void wideReact			 (const ShortIndex prong, const TEvent& event, TransitionControl& control);

	inline void wideExit			 (const ShortIndex prong, Control& control);

	inline void wideForwardRequest	 (const ShortIndex prong, const TransitionType transition);
	inline void wideRequestRemain	 ();
	inline void wideRequestRestart	 ();
	inline void wideRequestResume	 (const ShortIndex prong);
	inline void wideChangeToRequested(const ShortIndex prong, Control& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT  + Remaining::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	Initial initial;
	Remaining remaining;
	HSFM_IF_DEBUG(OwnStateList ownStateList);
};

//------------------------------------------------------------------------------

template <StateID TInitialID,
		  typename TContext,
		  typename TStateList,
		  typename TPayloadList,
		  ShortIndex NIndex,
		  typename TInitial>
struct _CS<TInitialID, TContext, TStateList, TPayloadList, NIndex, TInitial> {
	static constexpr StateID INITIAL_ID = TInitialID;

	using Context			= TContext;
	using StateList			= TStateList;
	using Control			= ControlT<Context>;
	using PayloadList		= TPayloadList;
	using StateRegistry		= Array<Parent, StateList::SIZE>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using TransitionControl	= TransitionControlT<Context, StateList, PayloadList>;
	using Initial			= typename WrapMaterial<INITIAL_ID, Context, StateList, PayloadList, TInitial>::Type;
	using Forward			= _CSF<TInitial>;
	using OwnStateList		= typename Forward::StateList;

	static constexpr LongIndex PRONG_INDEX	 = NIndex;

	_CS(StateRegistry& stateRegistry,
		const ShortIndex fork,
		Parents& forkParents,
		ForkPointers& forkPointers);

	inline void wideForwardGuard	 (const ShortIndex prong, TransitionControl& control);
	inline void wideGuard			 (const ShortIndex prong, TransitionControl& control);

	inline void wideEnterInitial	 (						  Control& control);
	inline void wideEnter			 (const ShortIndex prong, Control& control);

	inline bool wideUpdate			 (const ShortIndex prong, TransitionControl& control);

	template <typename TEvent>
	inline void wideReact			 (const ShortIndex prong, const TEvent& event, TransitionControl& control);

	inline void wideExit			 (const ShortIndex prong, Control& control);

	inline void wideForwardRequest	 (const ShortIndex prong, const TransitionType transition);
	inline void wideRequestRemain	 ();
	inline void wideRequestRestart	 ();
	inline void wideRequestResume	 (const ShortIndex prong);
	inline void wideChangeToRequested(const ShortIndex prong, Control& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	Initial initial;
	HSFM_IF_DEBUG(OwnStateList ownStateList);
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_composite_sub_1.inl"
#include "machine_composite_sub_2.inl"
