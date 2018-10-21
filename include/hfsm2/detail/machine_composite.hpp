namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NHeadIndex,
		  ShortIndex NCompoIndex,
		  ShortIndex NOrthoIndex,
		  typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct _C {
	static constexpr StateID	HEAD_ID		= NHeadIndex;
	static constexpr ShortIndex COMPO_INDEX	= NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	= NOrthoIndex;
	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID		COMPO_ID	= COMPO_INDEX + 1;

	using Args			= TArgs;
	using Head			= THead;

	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using PayloadList	= typename Args::PayloadList;

	using StateParents	= Array<Parent, StateList::SIZE>;
	using Request		= RequestT<PayloadList>;
	using RequestType	= typename Request::Type;

	using StateData		= StateDataT<Args>;

	using Control		= ControlT<Args>;
	using ControlOrigin	= typename Control::Origin;
	using ControlRegion	= typename Control::Region;

	using FullControl	= FullControlT<Args>;
	using ControlLock	= typename FullControl::Lock;

	using Plan			= PlanT<Args>;

	using HeadState		= _S <HEAD_ID, Args, Head>;
	using SubStates		= _CS<HEAD_ID + 1, COMPO_INDEX + 1, ORTHO_INDEX, Args, 0, TSubStates...>;
	using Forward		= _CF<Head, TSubStates...>;

	static constexpr ShortIndex REGION_SIZE	= Forward::STATE_COUNT;

	_C(StateData& stateData, const Parent parent);

	HFSM_INLINE CompoFork& compoFork		(StateData& stateData)	{ return stateData.compoForks[COMPO_INDEX];	}
	HFSM_INLINE CompoFork& compoFork		(Control&   control)	{ return compoFork(control.stateData());	}

	HFSM_INLINE void   deepForwardGuard		(FullControl& control);
	HFSM_INLINE void   deepGuard			(FullControl& control);

	HFSM_INLINE void   deepEnterInitial		(Control& control);
	HFSM_INLINE void   deepEnter			(Control& control);

	HFSM_INLINE Status deepUpdate			(FullControl& control);

	template <typename TEvent>
	HFSM_INLINE void   deepReact			(const TEvent& event,
											 FullControl& control);

	HFSM_INLINE void   deepExit				(Control& control);

	HFSM_INLINE void   deepForwardRequest	(StateData& stateData, const RequestType request);
	HFSM_INLINE void   deepRequestRemain	(StateData& stateData);
	HFSM_INLINE void   deepRequestRestart	(StateData& stateData);
	HFSM_INLINE void   deepRequestResume	(StateData& stateData);
				void   deepChangeToRequested(StateData& stateData, Control& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using RegionType		= typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT	 = HeadState::NAME_COUNT + SubStates::NAME_COUNT;

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif
	HeadState _headState;
	SubStates _subStates;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_composite.inl"
