namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NHeadID,
		  ForkID NCompoID,
		  ForkID NOrthoID,
		  typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct _O final {
	static constexpr StateID HEAD_ID  = NHeadID;
	static constexpr StateID COMPO_ID = NCompoID;
	static constexpr StateID ORTHO_ID = NOrthoID;

	using Args				= TArgs;
	using Head				= THead;

	using Context			= typename Args::Context;
	using Config			= typename Args::Config;
	using StateList			= typename Args::StateList;
	using PayloadList		= typename Args::PayloadList;

	static constexpr LongIndex STATE_COUNT = StateList::SIZE;

	using StateParents		= Array<Parent, STATE_COUNT>;
	using PlanControl		= PlanControlT	<Context, StateList, Args::FORK_COUNT, Args::PLAN_CAPACITY>;
	using Transition		= TransitionT	<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using ControlLock		= ControlLockT	<Context, StateList, Args::FORK_COUNT, PayloadList>;
	using ControlRegion		= ControlRegionT<Context, StateList, Args::FORK_COUNT, PayloadList>;
	using FullControl		= FullControlT	<Context, StateList, Args::FORK_COUNT, PayloadList, Args::PLAN_CAPACITY>;

	using State				= _S <HEAD_ID, Args, Head>;
	using SubStates			= _OS<HEAD_ID + 1, COMPO_ID, ORTHO_ID + 1, Args, 0, TSubStates...>;
	using Forward			= _OF<Head, TSubStates...>;
	using SubStateList		= typename Forward::StateList;

	_O(StateParents& stateParents,
	   const Parent parent,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

	inline void   deepForwardGuard		(FullControl& control);
	inline void   deepGuard				(FullControl& control);

	inline void   deepEnterInitial		(PlanControl& control);
	inline void   deepEnter				(PlanControl& control);

	inline Status deepUpdate			(FullControl& control);

	template <typename TEvent>
	inline void   deepReact				(const TEvent& event,
										 FullControl& control);

	inline void   deepExit				(PlanControl& control);

	inline void   deepForwardRequest	(const TransitionType transition);
	inline void   deepRequestRemain		();
	inline void   deepRequestRestart	();
	inline void   deepRequestResume		();
	inline void   deepChangeToRequested	(PlanControl& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using RegionType		= typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT	 = State::NAME_COUNT  + SubStates::NAME_COUNT;

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	Fork _fork;
	State _state;
	SubStates _subStates;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_orthogonal.inl"
