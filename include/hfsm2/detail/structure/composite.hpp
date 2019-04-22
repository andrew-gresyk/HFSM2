namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NHeadIndex,
		  ShortIndex NCompoIndex,
		  ShortIndex NOrthoIndex,
		  typename TArgs,
		  RegionStrategy TStrategy,
		  typename THead,
		  typename... TSubStates>
struct _C {
	static constexpr StateID	HEAD_ID		 = NHeadIndex;
	static constexpr ShortIndex COMPO_INDEX	 = NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	 = NOrthoIndex;
	static constexpr ShortIndex REGION_ID	 = COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID		COMPO_ID	 = COMPO_INDEX + 1;

	static constexpr RegionStrategy STRATEGY = TStrategy;

	using Args			= TArgs;
	using Head			= THead;

	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using PayloadList	= typename Args::PayloadList;

	using Request		= RequestT<PayloadList>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;

	using Control		= ControlT<Args>;

	using PlanControl	= PlanControlT<Args>;
	using ScopedOrigin	= typename PlanControl::Origin;
	using ScopedRegion	= typename PlanControl::Region;
	using Plan			= PlanT<Args>;

	using FullControl	= FullControlT<Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;


	using HeadState		= _S <HEAD_ID, Args, Head>;
	using SubStates		= _CS<HEAD_ID + 1, COMPO_INDEX + 1, ORTHO_INDEX, Args, STRATEGY, 0, TSubStates...>;
	using AllForward	= _CF<STRATEGY, Head, TSubStates...>;

	static constexpr ShortIndex REGION_SIZE	= AllForward::STATE_COUNT;

	ShortIndex& compoActive   (Control& control)					{ return control._stateRegistry.compoActive	   [COMPO_INDEX];	}
	ShortIndex& compoResumable(Control& control)					{ return control._stateRegistry.resumable.compo[COMPO_INDEX];	}
	ShortIndex& compoRequested(Control& control)					{ return control._stateRegistry.requested.compo[COMPO_INDEX];	}

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

#if defined _MSC_VER || defined __clang__

	template <RegionStrategy TG = STRATEGY>
	HFSM_INLINE UProng deepRequestChange			 (Control& control,	const ShortIndex = INVALID_SHORT_INDEX);

	template <>
	HFSM_INLINE UProng deepRequestChange<Composite>	 (Control& control,	const ShortIndex)	{ return deepRequestChangeComposite  (control); }

	template <>
	HFSM_INLINE UProng deepRequestChange<Resumable>	 (Control& control,	const ShortIndex)	{ return deepRequestChangeResumable  (control); }

	template <>
	HFSM_INLINE UProng deepRequestChange<Utilitarian>(Control& control,	const ShortIndex)	{ return deepRequestChangeUtilitarian(control); }

#else

	HFSM_INLINE UProng deepRequestChange			 (Control& control,						const ShortIndex = INVALID_SHORT_INDEX);

#endif

	HFSM_INLINE UProng deepRequestChangeComposite	 (Control& control);
	HFSM_INLINE UProng deepRequestChangeResumable	 (Control& control);
	HFSM_INLINE UProng deepRequestChangeUtilitarian	 (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _MSC_VER || defined __clang__

	template <RegionStrategy TG = STRATEGY>
	HFSM_INLINE UProng deepReportChange				 (Control& control,	const ShortIndex = INVALID_SHORT_INDEX);

	template <>
	HFSM_INLINE UProng deepReportChange<Composite>   (Control& control,	const ShortIndex)	{ return deepReportChangeComposite  (control); }

	template <>
	HFSM_INLINE UProng deepReportChange<Resumable>   (Control& control,	const ShortIndex)	{ return deepReportChangeResumable  (control); }

	template <>
	HFSM_INLINE UProng deepReportChange<Utilitarian> (Control& control,	const ShortIndex)	{ return deepReportChangeUtilitarian(control); }

#else

	HFSM_INLINE UProng deepReportChange				 (Control& control,	const ShortIndex = INVALID_SHORT_INDEX);

#endif

	HFSM_INLINE UProng deepReportChangeComposite	 (Control& control);
	HFSM_INLINE UProng deepReportChangeResumable	 (Control& control);
	HFSM_INLINE UProng deepReportChangeUtilitarian   (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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

	static constexpr LongIndex NAME_COUNT = HeadState::NAME_COUNT + SubStates::NAME_COUNT;

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

#include "composite.inl"
