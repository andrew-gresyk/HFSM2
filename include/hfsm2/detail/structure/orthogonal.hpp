namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NHeadIndex,
		  ShortIndex NCompoIndex,
		  ShortIndex NOrthoIndex,
		  typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct _O final {
	static constexpr StateID	HEAD_ID		= NHeadIndex;
	static constexpr ShortIndex COMPO_INDEX	= NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	= NOrthoIndex;
	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID		ORTHO_ID	= (ForkID) -ORTHO_INDEX - 1;

	using Args			= TArgs;
	using Head			= THead;

	using Utility		= typename Args::Utility;
	using UProng		= typename Args::UProng;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using Payload		= typename Args::Payload;

	using Request		= RequestT<Payload>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;

	using Control		= ControlT<Args>;

	using PlanControl	= PlanControlT<Args>;
	using ScopedOrigin	= typename PlanControl::Origin;
	using ScopedRegion	= typename PlanControl::Region;

	using FullControl	= FullControlT<Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;

	using HeadState		= _S <HEAD_ID, Args, Head>;
	using SubStates		= _OS<HEAD_ID + 1, COMPO_INDEX, ORTHO_INDEX + 1, Args, 0, TSubStates...>;
	using AllForward	= _OF<Head, TSubStates...>;

	static constexpr ShortIndex REGION_SIZE	= AllForward::STATE_COUNT;
	static constexpr ShortIndex PRONG_COUNT	= sizeof...(TSubStates);

	HFSM_INLINE		  OrthoFork& orthoRequested(	  StateRegistry& stateRegistry)			{ return stateRegistry.requested.ortho[ORTHO_INDEX];	}
	HFSM_INLINE const OrthoFork& orthoRequested(const StateRegistry& stateRegistry) const	{ return stateRegistry.requested.ortho[ORTHO_INDEX];	}

	HFSM_INLINE		  OrthoFork& orthoRequested(	  Control& control)						{ return orthoRequested(control._stateRegistry);		}
	HFSM_INLINE const OrthoFork& orthoRequested(const Control& control) const				{ return orthoRequested(control._stateRegistry);		}

	HFSM_INLINE void   deepRegister			(StateRegistry& stateRegistry, const Parent parent);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool   deepForwardEntryGuard(GuardControl& control,							const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE bool   deepEntryGuard		(GuardControl& control,							const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepEnter			(PlanControl& control,							const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE Status deepUpdate			(FullControl& control,							const ShortIndex = INVALID_SHORT_INDEX);

	template <typename TEvent>
	HFSM_INLINE void   deepReact			(FullControl& control,	const TEvent& event,	const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE bool   deepForwardExitGuard	(GuardControl& control,							const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE bool   deepExitGuard		(GuardControl& control,							const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepExit				(PlanControl& control,							const ShortIndex = INVALID_SHORT_INDEX);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void   deepForwardActive	(Control& control, const RequestType request,	const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE void   deepForwardRequest	(Control& control, const RequestType request,	const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepRequest			(Control& control, const RequestType request);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE UProng deepRequestChange	(Control& control,								const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE UProng deepReportChange		(Control& control,								const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepRequestRemain	(StateRegistry& stateRegistry);
	HFSM_INLINE void   deepRequestRestart	(StateRegistry& stateRegistry);
	HFSM_INLINE void   deepRequestResume	(StateRegistry& stateRegistry,					const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepRequestUtilize	(Control& control);
	HFSM_INLINE UProng deepReportUtilize	(Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void   deepEnterRequested	(PlanControl& control,							const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE void   deepChangeToRequested(PlanControl& control,							const ShortIndex = INVALID_SHORT_INDEX);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using RegionType		= typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT	 = HeadState::NAME_COUNT  + SubStates::NAME_COUNT;

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

#include "orthogonal.inl"
