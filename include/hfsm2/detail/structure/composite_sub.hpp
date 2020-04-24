namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  Strategy TStrategy,
		  ShortIndex NIndex,
		  typename... TStates>
struct CS_ final {
	static_assert(sizeof...(TStates) >= 2, "");

	using Indices		= TIndices;
	static constexpr StateID	INITIAL_ID	= Indices::STATE_ID;
	static constexpr ShortIndex COMPO_INDEX	= Indices::COMPO_INDEX;
	static constexpr ShortIndex ORTHO_INDEX	= Indices::ORTHO_INDEX;
	static constexpr ShortIndex ORTHO_UNIT	= Indices::ORTHO_UNIT;

	static constexpr Strategy	STRATEGY	= TStrategy;

	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			= TArgs;
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	static constexpr ShortIndex L_PRONG = PRONG_INDEX;

	using LStates		= SplitL<TStates...>;
	using LHalf			= CSubMaterial<I_<INITIAL_ID,
										  COMPO_INDEX,
										  ORTHO_INDEX,
										  ORTHO_UNIT>,
									   Args,
									   STRATEGY,
									   L_PRONG,
									   LStates>;
	using LHalfInfo		= typename InfoT<LHalf>::Type;

	static constexpr ShortIndex R_PRONG = PRONG_INDEX + LStates::SIZE;

	using RStates		= SplitR<TStates...>;
	using RHalf			= CSubMaterial<I_<INITIAL_ID  + LHalfInfo::STATE_COUNT,
										  COMPO_INDEX + LHalfInfo::COMPO_REGIONS,
										  ORTHO_INDEX + LHalfInfo::ORTHO_REGIONS,
										  ORTHO_UNIT  + LHalfInfo::ORTHO_UNITS>,
									   Args,
									   STRATEGY,
									   R_PRONG,
									   RStates>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_EXPLICIT_MEMBER_SPECIALIZATION

	template <typename T>
	HFSM_INLINE		  T& access();

	template <typename T>
	HFSM_INLINE const T& access() const;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideRegister				  (StateRegistry& stateRegistry, const Parent parent);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool	wideForwardEntryGuard		  (GuardControl& control,								const ShortIndex prong);
	HFSM_INLINE bool	wideEntryGuard				  (GuardControl& control,								const ShortIndex prong);

	HFSM_INLINE void	wideConstruct				  (PlanControl&  control,								const ShortIndex prong);

	HFSM_INLINE void	wideEnter					  (PlanControl&  control,								const ShortIndex prong);
	HFSM_INLINE void	wideReenter					  (PlanControl&  control,								const ShortIndex prong);

	HFSM_INLINE Status	wideUpdate					  (FullControl&  control,								const ShortIndex prong);

	template <typename TEvent>
	HFSM_INLINE Status	wideReact					  (FullControl&  control, const TEvent& event,			const ShortIndex prong);

	HFSM_INLINE bool	wideForwardExitGuard		  (GuardControl& control,								const ShortIndex prong);
	HFSM_INLINE bool	wideExitGuard				  (GuardControl& control,								const ShortIndex prong);

	HFSM_INLINE void	wideExit					  (PlanControl&  control,								const ShortIndex prong);

	HFSM_INLINE void	wideDestruct				  (PlanControl&  control,								const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideForwardActive			  (Control& control,	 const Request::Type request,	const ShortIndex prong);
	HFSM_INLINE void	wideForwardRequest			  (Control& control,	 const Request::Type request,	const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_EXPLICIT_MEMBER_SPECIALIZATION

	template <Strategy = STRATEGY>
	HFSM_INLINE void	wideRequestChange			  (Control& control,									const ShortIndex = INVALID_SHORT_INDEX);

	template <>
	HFSM_INLINE void	wideRequestChange<Composite>  (Control& control,									const ShortIndex)		{ wideRequestChangeComposite(control);			}

	template <>
	HFSM_INLINE	void	wideRequestChange<Resumable>  (Control& control,									const ShortIndex prong)	{ wideRequestChangeResumable(control, prong);	}

#else

	HFSM_INLINE void	wideRequestChange			  (Control& control,									const ShortIndex = INVALID_SHORT_INDEX);

#endif

	HFSM_INLINE void	wideRequestChangeComposite	  (Control& control);
	HFSM_INLINE void	wideRequestChangeResumable	  (Control& control,									const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideRequestRemain			  (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestRestart			  (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestResume			  (StateRegistry& stateRegistry,						const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE UP		wideReportChangeComposite	  (Control& control);
	HFSM_INLINE UP		wideReportChangeResumable	  (Control& control,									const ShortIndex prong);
	HFSM_INLINE UP		wideReportChangeUtilitarian	  (Control& control);
	HFSM_INLINE Utility	wideReportChangeRandom		  (Control& control,	 Utility* const options, const Rank* const ranks, const Rank top);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE UP		wideReportUtilize			  (Control& control);
	HFSM_INLINE Rank	wideReportRank				  (Control& control,	 Rank*	  const ranks);
	HFSM_INLINE Utility	wideReportRandomize			  (Control& control,	 Utility* const options, const Rank* const ranks, const Rank top);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideChangeToRequested		  (PlanControl& control, const ShortIndex prong);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT = LHalf::NAME_COUNT + RHalf::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	LHalf lHalf;
	RHalf rHalf;
};

//------------------------------------------------------------------------------

template <typename TIndices,
		  typename TArgs,
		  Strategy TStrategy,
		  ShortIndex NIndex,
		  typename TState>
struct CS_<TIndices, TArgs, TStrategy, NIndex, TState> final {
	using Indices		= TIndices;
	static constexpr StateID	INITIAL_ID	= Indices::STATE_ID;
	static constexpr ShortIndex COMPO_INDEX	= Indices::COMPO_INDEX;
	static constexpr ShortIndex ORTHO_INDEX	= Indices::ORTHO_INDEX;
	static constexpr ShortIndex ORTHO_UNIT	= Indices::ORTHO_UNIT;

	static constexpr Strategy	STRATEGY	= TStrategy;

	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			= TArgs;
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using State			= Material<I_<INITIAL_ID,
									  COMPO_INDEX,
									  ORTHO_INDEX,
									  ORTHO_UNIT>,
								   Args,
								   TState>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_EXPLICIT_MEMBER_SPECIALIZATION

	template <typename T>
	HFSM_INLINE		  T& access()					  { return state.template access<T>();	}

	template <typename T>
	HFSM_INLINE const T& access() const				  { return state.template access<T>();	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideRegister				  (StateRegistry& stateRegistry, const Parent parent);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool	wideForwardEntryGuard		  (GuardControl& control,								const ShortIndex prong);
	HFSM_INLINE bool	wideEntryGuard				  (GuardControl& control,								const ShortIndex prong);

	HFSM_INLINE void	wideConstruct				  (PlanControl& control,								const ShortIndex prong);

	HFSM_INLINE void	wideEnter					  (PlanControl& control,								const ShortIndex prong);
	HFSM_INLINE void	wideReenter					  (PlanControl& control,								const ShortIndex prong);

	HFSM_INLINE Status	wideUpdate					  (FullControl& control,								const ShortIndex prong);

	template <typename TEvent>
	HFSM_INLINE Status	wideReact					  (FullControl& control, const TEvent& event,			const ShortIndex prong);

	HFSM_INLINE bool	wideForwardExitGuard		  (GuardControl& control,								const ShortIndex prong);
	HFSM_INLINE bool	wideExitGuard				  (GuardControl& control,								const ShortIndex prong);

	HFSM_INLINE void	wideExit					  (PlanControl& control,								const ShortIndex prong);

	HFSM_INLINE void	wideDestruct				  (PlanControl& control,								const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideForwardActive			  (Control& control,	 const Request::Type request,	const ShortIndex prong);
	HFSM_INLINE void	wideForwardRequest			  (Control& control,	 const Request::Type request,	const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideRequestChangeComposite	  (Control& control);
	HFSM_INLINE void	wideRequestChangeResumable	  (Control& control,	 								const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideRequestRemain			  (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestRestart			  (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestResume			  (StateRegistry& stateRegistry,						const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE UP		wideReportChangeComposite	  (Control& control);
	HFSM_INLINE UP		wideReportChangeResumable	  (Control& control,									const ShortIndex prong);
	HFSM_INLINE UP		wideReportChangeUtilitarian	  (Control& control);
	HFSM_INLINE Utility	wideReportChangeRandom		  (Control& control,	 Utility* const options, const Rank* const ranks, const Rank top);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE UP		wideReportUtilize			  (Control& control);
	HFSM_INLINE Rank	wideReportRank				  (Control& control,	 Rank*	  const ranks);
	HFSM_INLINE Utility	wideReportRandomize			  (Control& control,	 Utility* const options, const Rank* const ranks, const Rank top);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideChangeToRequested		  (PlanControl& control,								const ShortIndex prong);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT = State::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	State state;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "composite_sub_1.inl"
#include "composite_sub_2.inl"
