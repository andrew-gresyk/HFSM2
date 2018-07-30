namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID, typename, typename, typename, ShortIndex, typename...>
struct _OS;

//------------------------------------------------------------------------------

template <StateID TInitialID,
		  typename TContext,
		  typename TStateList,
		  typename TPayloadList,
		  ShortIndex NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct _OS<TInitialID, TContext, TStateList, TPayloadList, NIndex, TInitial, TRemaining...> {
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
	using Remaining			= _OS<INITIAL_ID + InitialForward::STATE_COUNT, Context, StateList, PayloadList, NIndex + 1, TRemaining...>;
	using Forward			= _OSF<TInitial, TRemaining...>;
	using OwnStateList		= typename Forward::StateList;

	static constexpr LongIndex PRONG_INDEX	 = NIndex;

	_OS(StateRegistry& stateRegistry,
		const ShortIndex fork,
		Parents& forkParents,
		ForkPointers& forkPointers);

	inline void wideForwardGuard	 (const ShortIndex prong, TransitionControl& control);

	inline void wideForwardGuard	 (TransitionControl& control);
	inline void wideGuard			 (TransitionControl& control);

	inline void wideEnterInitial	 (Control& control);
	inline void wideEnter			 (Control& control);

	inline bool wideUpdate			 (TransitionControl& control);

	template <typename TEvent>
	inline void wideReact			 (const TEvent& event, TransitionControl& control);

	inline void wideExit			 (Control& control);

	inline void wideForwardRequest	 (const ShortIndex prong, const TransitionType transition);
	inline void wideRequestRemain	 ();
	inline void wideRequestRestart	 ();
	inline void wideRequestResume	 ();
	inline void wideChangeToRequested(Control& control);

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
struct _OS<TInitialID, TContext, TStateList, TPayloadList, NIndex, TInitial> {
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
	using Forward			= _OSF<TInitial>;
	using OwnStateList		= typename Forward::StateList;

	static constexpr LongIndex PRONG_INDEX	 = NIndex;

	_OS(StateRegistry& stateRegistry,
		const ShortIndex fork,
		Parents& forkParents,
		ForkPointers& forkPointers);

	inline void wideForwardGuard	 (const ShortIndex prong, TransitionControl& control);

	inline void wideForwardGuard	 (TransitionControl& control);
	inline void wideGuard			 (TransitionControl& control);

	inline void wideEnterInitial	 (Control& control);
	inline void wideEnter			 (Control& control);

	inline bool wideUpdate			 (TransitionControl& control);

	template <typename TEvent>
	inline void wideReact			 (const TEvent& event, TransitionControl& control);

	inline void wideExit			 (Control& control);

	inline void wideForwardRequest	 (const ShortIndex prong, const TransitionType transition);
	inline void wideRequestRemain	 ();
	inline void wideRequestRestart	 ();
	inline void wideRequestResume	 ();
	inline void wideChangeToRequested(Control& control);

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

#include "machine_orthogonal_sub_1.inl"
#include "machine_orthogonal_sub_2.inl"
