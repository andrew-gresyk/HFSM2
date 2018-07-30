namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID THeadID,
		  typename TContext,
		  typename TStateList,
		  typename TPayloadList,
		  typename THead,
		  typename... TSubStates>
struct _C final {
	static constexpr StateID HEAD_ID = THeadID;

	using Context			= TContext;
	using Control			= ControlT<Context>;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;
	using StateRegistry		= Array<Parent, StateList::SIZE>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using TransitionControl	= TransitionControlT<Context, StateList, PayloadList>;
	using ControlLock		= typename TransitionControl::Lock;
	using Head				= THead;
	using State				= _S <HEAD_ID,	   Context, StateList, PayloadList, Head>;
	using SubStates			= _CS<HEAD_ID + 1, Context, StateList, PayloadList, 0, TSubStates...>;
	using Forward			= _CF<Head, TSubStates...>;
	using OwnStateList		= typename Forward::StateList;

	_C(StateRegistry& stateRegistry,
	   const Parent parent,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

	inline void deepForwardGuard	 (TransitionControl& control);
	inline void deepGuard			 (TransitionControl& control);

	inline void deepEnterInitial	 (Control& control);
	inline void deepEnter			 (Control& control);

	inline bool deepUpdate			 (TransitionControl& control);

	template <typename TEvent>
	inline void deepReact			 (const TEvent& event, TransitionControl& control);

	inline void deepExit			 (Control& control);

	inline void deepForwardRequest	 (const TransitionType transition);
	inline void deepRequestRemain	 ();
	inline void deepRequestRestart	 ();
	inline void deepRequestResume	 ();
	inline void deepChangeToRequested(Control& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using RegionType		= typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT	 = State::NAME_COUNT + SubStates::NAME_COUNT;

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif
	Fork _fork;
	State _state;
	SubStates _subStates;

	HSFM_IF_DEBUG(const std::type_index _type = typeid(Head));
	HSFM_IF_DEBUG(OwnStateList ownStateList);
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_composite.inl"
