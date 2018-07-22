namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TPayloadList,
		  typename THead,
		  typename... TSubStates>
struct _C final {
	using Context			= TContext;
	using Control			= ControlT<Context>;
	using PayloadList		= TPayloadList;
	using StateRegistryBase	= StateRegistryBaseT<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using TransitionControl	= TransitionControlT<Context, PayloadList>;
	using ControlLock		= ControlLockT<Context, PayloadList>;
	using Head				= THead;
	using Fork				= ForkT<Head>;
	using State				= _S<Context, PayloadList, Head>;
	using SubStates			= _CS<Context, PayloadList, 0, TSubStates...>;
	using StateList			= typename MergeT<typename State::StateList, typename SubStates::StateList>::TypeList;
	HSFM_IF_DEBUG(StateList stateList);

	enum : LongIndex {
		REVERSE_DEPTH = SubStates::REVERSE_DEPTH + 1,
		DEEP_WIDTH	  = SubStates::DEEP_WIDTH,
		STATE_COUNT	  = State::STATE_COUNT + SubStates::STATE_COUNT,
		FORK_COUNT	  = SubStates::FORK_COUNT + 1,
		PRONG_COUNT	  = SubStates::PRONG_COUNT + sizeof...(TSubStates),
		WIDTH		  = sizeof...(TSubStates),
	};

	_C(StateRegistryBase& stateRegistry,
	   const Parent parent,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

	inline void deepRegister		 (StateRegistryBase& stateRegistry, const Parent parent);

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
	enum : LongIndex {
		NAME_COUNT	 = State::NAME_COUNT  + SubStates::NAME_COUNT,
	};

	void deepGetNames(const LongIndex parent,
					  const enum StructureStateInfo::RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;

	void deepIsActive(const bool isActive,
					  LongIndex& index,
					  MachineStructure& structure) const;
#endif
	Fork _fork;
	State _state;
	SubStates _subStates;

	HSFM_IF_DEBUG(const std::type_index _type = typeid(Head));
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_composite.inl"
