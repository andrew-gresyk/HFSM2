namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NInitialID,
		  ShortIndex NCompoIndex,
		  ShortIndex NOrthoIndex,
		  typename TArgs,
		  RegionStrategy TStrategy,
		  ShortIndex NIndex,
		  typename... TStates>
struct _CS {
	static constexpr StateID	INITIAL_ID	 = NInitialID;
	static constexpr ShortIndex COMPO_INDEX	 = NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	 = NOrthoIndex;
	static constexpr ShortIndex REGION_ID	 = COMPO_INDEX + ORTHO_INDEX;
	static constexpr ShortIndex PRONG_INDEX	 = NIndex;

	static constexpr RegionStrategy STRATEGY = TStrategy;

	using Args			= TArgs;

	using Utility		= typename Args::Utility;
	using UProng		= typename Args::UProng;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using Payload		= typename Args::Payload;

	using Request		= RequestT<Payload>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	static constexpr ShortIndex L_PRONG = PRONG_INDEX;

	using LStates		= SplitL<TStates...>;
	using LHalf			= CSubMaterial<INITIAL_ID,
									   COMPO_INDEX,
									   ORTHO_INDEX,
									   Args,
									   STRATEGY,
									   L_PRONG,
									   LStates>;
	using LHalfForward	= typename ForwardT<LHalf>::Type;

	static constexpr ShortIndex R_PRONG = PRONG_INDEX + LStates::SIZE;

	using RStates		= SplitR<TStates...>;
	using RHalf			= CSubMaterial<INITIAL_ID  + LHalfForward::STATE_COUNT,
									   COMPO_INDEX + LHalfForward::COMPO_COUNT,
									   ORTHO_INDEX + LHalfForward::ORTHO_COUNT,
									   Args,
									   STRATEGY,
									   R_PRONG,
									   RStates>;

	HFSM_INLINE void   deepRegister			(StateRegistry& stateRegistry, const Parent parent);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool   deepForwardEntryGuard(GuardControl& control,							const ShortIndex prong);
	HFSM_INLINE bool   deepEntryGuard		(GuardControl& control,							const ShortIndex prong);

	HFSM_INLINE void   deepEnter			(PlanControl& control,							const ShortIndex prong);

	HFSM_INLINE Status deepUpdate			(FullControl& control,							const ShortIndex prong);

	template <typename TEvent>
	HFSM_INLINE void   deepReact			(FullControl& control,	const TEvent& event,	const ShortIndex prong);

	HFSM_INLINE bool   deepForwardExitGuard	(GuardControl& control,							const ShortIndex prong);
	HFSM_INLINE bool   deepExitGuard		(GuardControl& control,							const ShortIndex prong);

	HFSM_INLINE void   deepExit				(PlanControl& control,							const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void   deepForwardActive	(Control& control,	const RequestType request,	const ShortIndex prong);
	HFSM_INLINE void   deepForwardRequest	(Control& control,	const RequestType request,	const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _MSC_VER || defined __clang_major__ && __clang_major__ >= 7

	template <RegionStrategy TG = STRATEGY>
	HFSM_INLINE UProng deepRequestChange			 (Control& control,	const ShortIndex = INVALID_SHORT_INDEX);

	template <>
	HFSM_INLINE UProng deepRequestChange<Composite>	 (Control& control,	const ShortIndex)		{ return deepRequestChangeComposite  (control);			}

	template <>
	HFSM_INLINE UProng deepRequestChange<Resumable>	 (Control& control,	const ShortIndex prong)	{ return deepRequestChangeResumable  (control, prong);	}

	template <>
	HFSM_INLINE UProng deepRequestChange<Utilitarian>(Control& control,	const ShortIndex)		{ return deepRequestChangeUtilitarian(control);			}

#else

	HFSM_INLINE UProng deepRequestChange			 (Control& control,	const ShortIndex = INVALID_SHORT_INDEX);

#endif

	HFSM_INLINE UProng deepRequestChangeComposite	 (Control& control);
	HFSM_INLINE UProng deepRequestChangeResumable	 (Control& control,	const ShortIndex prong);
	HFSM_INLINE UProng deepRequestChangeUtilitarian	 (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _MSC_VER || defined __clang_major__ && __clang_major__ >= 7

	template <RegionStrategy TG = STRATEGY>
	HFSM_INLINE UProng deepReportChange				 (Control& control,	const ShortIndex = INVALID_SHORT_INDEX);

	template <>
	HFSM_INLINE UProng deepReportChange<Composite>   (Control& control,	const ShortIndex)		{ return deepReportChangeComposite  (control);		}

	template <>
	HFSM_INLINE UProng deepReportChange<Resumable>   (Control& control,	const ShortIndex prong)	{ return deepReportChangeResumable  (control, prong);	}

	template <>
	HFSM_INLINE UProng deepReportChange<Utilitarian> (Control& control,	const ShortIndex)		{ return deepReportChangeUtilitarian(control);		}

#else

	HFSM_INLINE UProng deepReportChange				 (Control& control,	const ShortIndex = INVALID_SHORT_INDEX);

#endif

	HFSM_INLINE UProng deepReportChangeComposite	 (Control& control);
	HFSM_INLINE UProng deepReportChangeResumable	 (Control& control,	const ShortIndex prong);
	HFSM_INLINE UProng deepReportChangeUtilitarian   (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void   deepRequestRemain	(StateRegistry& stateRegistry);
	HFSM_INLINE void   deepRequestRestart	(StateRegistry& stateRegistry);
	HFSM_INLINE void   deepRequestResume	(StateRegistry& stateRegistry,					const ShortIndex prong);

	HFSM_INLINE UProng deepReportUtilize	(Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void   deepEnterRequested	(PlanControl& control,							const ShortIndex prong);
	HFSM_INLINE void   deepChangeToRequested(PlanControl& control,							const ShortIndex prong);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using RegionType		= typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT = LHalf::NAME_COUNT + RHalf::NAME_COUNT;

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	LHalf lHalf;
	RHalf rHalf;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "composite_sub.inl"
