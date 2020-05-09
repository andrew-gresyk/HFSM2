namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename, typename, ShortIndex, typename...>
struct OS_;

//------------------------------------------------------------------------------

template <typename TIndices,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct OS_<TIndices, TArgs, NIndex, TInitial, TRemaining...> final {
	using Indices		= TIndices;
	static constexpr StateID	INITIAL_ID	= Indices::STATE_ID;
	static constexpr ShortIndex COMPO_INDEX	= Indices::COMPO_INDEX;
	static constexpr ShortIndex ORTHO_INDEX	= Indices::ORTHO_INDEX;
	static constexpr ShortIndex ORTHO_UNIT	= Indices::ORTHO_UNIT;

	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			= TArgs;
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;
	using OrthoForks	= typename StateRegistry::AllForks::Ortho;
	using ProngBits		= typename OrthoForks::Bits;
	using ProngConstBits= typename OrthoForks::ConstBits;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Initial		= Material<I_<INITIAL_ID,
									  COMPO_INDEX,
									  ORTHO_INDEX,
									  ORTHO_UNIT>,
								   Args,
								   TInitial>;

	using InitialInfo	= WrapInfo<TInitial>;
	using InitialStates	= typename InitialInfo::StateList;

	using Remaining		= OS_<I_<INITIAL_ID  + InitialInfo::STATE_COUNT,
								 COMPO_INDEX + InitialInfo::COMPO_REGIONS,
								 ORTHO_INDEX + InitialInfo::ORTHO_REGIONS,
								 ORTHO_UNIT  + InitialInfo::ORTHO_UNITS>,
							  Args,
							  PRONG_INDEX + 1,
							  TRemaining...>;

	using Info	= OSI_<TInitial, TRemaining...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_EXPLICIT_MEMBER_SPECIALIZATION

	template <typename T>
	HFSM_INLINE		  T& access();

	template <typename T>
	HFSM_INLINE const T& access() const;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideRegister		 (StateRegistry& stateRegistry, const ForkID forkId);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool	wideForwardEntryGuard(GuardControl&	control,							const ProngConstBits prongs);
	HFSM_INLINE bool	wideForwardEntryGuard(GuardControl&	control);
	HFSM_INLINE bool	wideEntryGuard		 (GuardControl&	control);

	HFSM_INLINE void	wideConstruct		 (PlanControl&	control);

	HFSM_INLINE void	wideEnter			 (PlanControl&	control);
	HFSM_INLINE void	wideReenter			 (PlanControl&	control);

	HFSM_INLINE Status	wideUpdate			 (FullControl&	control);

	template <typename TEvent>
	HFSM_INLINE Status	wideReact			 (FullControl&	control, const TEvent& event);

	HFSM_INLINE bool	wideForwardExitGuard (GuardControl&	control,							const ProngConstBits prongs);
	HFSM_INLINE bool	wideForwardExitGuard (GuardControl&	control);
	HFSM_INLINE bool	wideExitGuard		 (GuardControl&	control);

	HFSM_INLINE void	wideExit			 (PlanControl&	control);

	HFSM_INLINE void	wideDestruct		 (PlanControl&  control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideForwardActive	 (Control& control, const Request::Type request,	const ProngConstBits prongs);
	HFSM_INLINE void	wideForwardRequest	 (Control& control, const Request::Type request,	const ProngConstBits prongs);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideRequestChange	 (Control& control);
	HFSM_INLINE void	wideRequestRemain	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestRestart	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestResume	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestUtilize	 (Control& control);
	HFSM_INLINE void	wideRequestRandomize (Control& control);

	HFSM_INLINE Utility	wideReportChange	 (Control& control);
	HFSM_INLINE Utility	wideReportUtilize	 (Control& control);
	HFSM_INLINE Utility	wideReportRandomize	 (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideChangeToRequested(PlanControl& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;

	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT  + Remaining::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	Initial initial;
	Remaining remaining;
};

//------------------------------------------------------------------------------

template <typename TIndices,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename TInitial>
struct OS_<TIndices, TArgs, NIndex, TInitial> final {
	using Indices		= TIndices;
	static constexpr StateID	INITIAL_ID	= Indices::STATE_ID;
	static constexpr ShortIndex COMPO_INDEX	= Indices::COMPO_INDEX;
	static constexpr ShortIndex ORTHO_INDEX	= Indices::ORTHO_INDEX;
	static constexpr ShortIndex ORTHO_UNIT	= Indices::ORTHO_UNIT;

	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			= TArgs;
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;
	using OrthoForks	= typename StateRegistry::AllForks::Ortho;
	using ProngBits		= typename OrthoForks::Bits;
	using ProngConstBits= typename OrthoForks::ConstBits;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Initial		= Material<I_<INITIAL_ID,
									  COMPO_INDEX,
									  ORTHO_INDEX,
									  ORTHO_UNIT>,
								   Args,
								   TInitial>;

	using Info	= OSI_<TInitial>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_EXPLICIT_MEMBER_SPECIALIZATION

	template <typename T>
	HFSM_INLINE		  T& access()			 { return initial.template access<T>();	}

	template <typename T>
	HFSM_INLINE const T& access() const		 { return initial.template access<T>();	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideRegister		 (StateRegistry& stateRegistry, const ForkID forkId);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool	wideForwardEntryGuard(GuardControl&	control,							const ProngConstBits prongs);
	HFSM_INLINE bool	wideForwardEntryGuard(GuardControl&	control);
	HFSM_INLINE bool	wideEntryGuard		 (GuardControl&	control);

	HFSM_INLINE void	wideConstruct		 (PlanControl&	control);

	HFSM_INLINE void	wideEnter			 (PlanControl&	control);
	HFSM_INLINE void	wideReenter			 (PlanControl&	control);

	HFSM_INLINE Status	wideUpdate			 (FullControl&	control);

	template <typename TEvent>
	HFSM_INLINE Status	wideReact			 (FullControl&	control, const TEvent& event);

	HFSM_INLINE bool	wideForwardExitGuard (GuardControl&	control,							const ProngConstBits prongs);
	HFSM_INLINE bool	wideForwardExitGuard (GuardControl&	control);
	HFSM_INLINE bool	wideExitGuard		 (GuardControl&	control);

	HFSM_INLINE void	wideExit			 (PlanControl&	control);

	HFSM_INLINE void	wideDestruct		 (PlanControl&  control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideForwardActive	 (Control& control, const Request::Type request,	const ProngConstBits prongs);
	HFSM_INLINE void	wideForwardRequest	 (Control& control, const Request::Type request,	const ProngConstBits prongs);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideRequestChange	 (Control& control);
	HFSM_INLINE void	wideRequestRemain	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestRestart	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestResume	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestUtilize	 (Control& control);
	HFSM_INLINE void	wideRequestRandomize (Control& control);

	HFSM_INLINE Utility	wideReportChange	 (Control& control);
	HFSM_INLINE Utility	wideReportUtilize	 (Control& control);
	HFSM_INLINE Utility	wideReportRandomize	 (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideChangeToRequested(PlanControl& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;

	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	Initial initial;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "orthogonal_sub_1.inl"
#include "orthogonal_sub_2.inl"
