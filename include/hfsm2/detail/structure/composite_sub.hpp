namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  Strategy TStrategy,
		  ShortIndex NIndex,
		  typename... TStates>
struct _CS {
	using Indices		= TIndices;
	static constexpr StateID	INITIAL_ID	= Indices::STATE_ID;
	static constexpr ShortIndex COMPO_INDEX	= Indices::COMPO_INDEX;
	static constexpr ShortIndex ORTHO_INDEX	= Indices::ORTHO_INDEX;
	static constexpr ShortIndex ORTHO_UNIT	= Indices::ORTHO_UNIT;

	static constexpr Strategy	STRATEGY	= TStrategy;

	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			= TArgs;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
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
	using LHalf			= CSubMaterial<_I<INITIAL_ID,
										  COMPO_INDEX,
										  ORTHO_INDEX,
										  ORTHO_UNIT>,
									   Args,
									   STRATEGY,
									   L_PRONG,
									   LStates>;
	using LHalfForward	= typename ForwardT<LHalf>::Type;

	static constexpr ShortIndex R_PRONG = PRONG_INDEX + LStates::SIZE;

	using RStates		= SplitR<TStates...>;
	using RHalf			= CSubMaterial<_I<INITIAL_ID  + LHalfForward::STATE_COUNT,
										  COMPO_INDEX + LHalfForward::COMPO_REGIONS,
										  ORTHO_INDEX + LHalfForward::ORTHO_REGIONS,
										  ORTHO_UNIT  + LHalfForward::ORTHO_UNITS>,
									   Args,
									   STRATEGY,
									   R_PRONG,
									   RStates>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepRegister				  (StateRegistry& stateRegistry, const Parent parent);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool	deepForwardEntryGuard		  (GuardControl& control,							const ShortIndex prong);
	HFSM_INLINE bool	deepEntryGuard				  (GuardControl& control,							const ShortIndex prong);

	HFSM_INLINE void	deepEnter					  (PlanControl& control,							const ShortIndex prong);
	HFSM_INLINE void	deepReenter					  (PlanControl& control,							const ShortIndex prong);

	HFSM_INLINE Status	deepUpdate					  (FullControl& control,							const ShortIndex prong);

	template <typename TEvent>
	HFSM_INLINE Status	deepReact					  (FullControl& control, const TEvent& event,		const ShortIndex prong);

	HFSM_INLINE bool	deepForwardExitGuard		  (GuardControl& control,							const ShortIndex prong);
	HFSM_INLINE bool	deepExitGuard				  (GuardControl& control,							const ShortIndex prong);

	HFSM_INLINE void	deepExit					  (PlanControl& control,							const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepForwardActive			  (Control& control,	 const RequestType request,	const ShortIndex prong);
	HFSM_INLINE void	deepForwardRequest			  (Control& control,	 const RequestType request,	const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM_EXPLICIT_MEMBER_SPECIALIZATION

	template <Strategy TG = STRATEGY>
	HFSM_INLINE void	deepRequestChange			  (Control& control,	 const ShortIndex = INVALID_SHORT_INDEX);

	template <>
	HFSM_INLINE void	deepRequestChange<Composite>  (Control& control,	 const ShortIndex)			{ deepRequestChangeComposite(control);			}

	template <>
	HFSM_INLINE	void	deepRequestChange<Resumable>  (Control& control,	 const ShortIndex prong)	{ deepRequestChangeResumable(control, prong);	}

#else

	HFSM_INLINE void	deepRequestChange			  (Control& control,	 const ShortIndex = INVALID_SHORT_INDEX);

#endif

	HFSM_INLINE void	deepRequestChangeComposite	  (Control& control);
	HFSM_INLINE void	deepRequestChangeResumable	  (Control& control,	 const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepRequestRemain			  (StateRegistry& stateRegistry);
	HFSM_INLINE void	deepRequestRestart			  (StateRegistry& stateRegistry);
	HFSM_INLINE void	deepRequestResume			  (StateRegistry& stateRegistry, const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM_EXPLICIT_MEMBER_SPECIALIZATION

	template <Strategy TG = STRATEGY>
	HFSM_INLINE UP		deepReportChange			  (Control& control,	 const ShortIndex = INVALID_SHORT_INDEX);

	template <>
	HFSM_INLINE UP		deepReportChange<Composite>   (Control& control,	 const ShortIndex)		 { return deepReportChangeComposite  (control);			}

	template <>
	HFSM_INLINE UP		deepReportChange<Resumable>   (Control& control,	 const ShortIndex prong) { return deepReportChangeResumable  (control, prong);	}

	template <>
	HFSM_INLINE UP		deepReportChange<Utilitarian> (Control& control,	 const ShortIndex)		 { return deepReportChangeUtilitarian(control);			}

#else

	HFSM_INLINE UP		deepReportChange			  (Control& control,	 const ShortIndex = INVALID_SHORT_INDEX);

#endif

	HFSM_INLINE UP		deepReportChangeComposite	  (Control& control);
	HFSM_INLINE UP		deepReportChangeResumable	  (Control& control,	 const ShortIndex prong);
	HFSM_INLINE UP		deepReportChangeUtilitarian	  (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE UP		deepReportUtilize			  (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepEnterRequested			  (PlanControl& control, const ShortIndex prong);
	HFSM_INLINE void	deepChangeToRequested		  (PlanControl& control, const ShortIndex prong);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

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
