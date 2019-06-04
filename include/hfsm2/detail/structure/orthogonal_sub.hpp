namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID, ShortIndex, ShortIndex, typename, ShortIndex, typename...>
struct _OS;

//------------------------------------------------------------------------------

template <StateID NInitialID,
		  ShortIndex NCompoIndex,
		  ShortIndex NOrthoIndex,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct _OS<NInitialID, NCompoIndex, NOrthoIndex, TArgs, NIndex, TInitial, TRemaining...> {
	static constexpr StateID	INITIAL_ID	= NInitialID;
	static constexpr ShortIndex COMPO_INDEX	= NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	= NOrthoIndex;
	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			= TArgs;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
	using Payload		= typename Args::Payload;

	using Request		= RequestT<Payload>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Initial		= Material<INITIAL_ID, COMPO_INDEX, ORTHO_INDEX, Args, TInitial>;
	using Forwarded		= Wrap<TInitial>;

	using Remaining		= _OS<INITIAL_ID  + Forwarded::STATE_COUNT,
							  COMPO_INDEX + Forwarded::COMPO_COUNT,
							  ORTHO_INDEX + Forwarded::ORTHO_COUNT,
							  Args,
						   PRONG_INDEX + 1,
						   TRemaining...>;

	using AllForward	= _OSF<TInitial, TRemaining...>;

	HFSM_INLINE void	wideRegister			(StateRegistry& stateRegistry, const ForkID forkId);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool	wideForwardEntryGuard(GuardControl& control,						const OrthoFork& prongs);
	HFSM_INLINE bool	wideForwardEntryGuard(GuardControl& control);
	HFSM_INLINE bool	wideEntryGuard		 (GuardControl& control);

	HFSM_INLINE void	wideEnter			 (PlanControl& control);
	HFSM_INLINE void	wideReenter			 (PlanControl& control);

	HFSM_INLINE Status	wideUpdate			 (FullControl& control);

	template <typename TEvent>
	HFSM_INLINE Status	wideReact			 (FullControl& control, const TEvent& event);

	HFSM_INLINE bool	wideForwardExitGuard (GuardControl& control,						const OrthoFork& prongs);
	HFSM_INLINE bool	wideForwardExitGuard (GuardControl& control);
	HFSM_INLINE bool	wideExitGuard		 (GuardControl& control);

	HFSM_INLINE void	wideExit			 (PlanControl& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideForwardActive	 (Control& control, const RequestType request,	const OrthoFork& prongs);
	HFSM_INLINE void	wideForwardRequest	 (Control& control, const RequestType request,	const OrthoFork& prongs);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideRequestChange	 (Control& control);
	HFSM_INLINE void	wideRequestRemain	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestRestart	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestResume	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestUtilize	 (Control& control);

	HFSM_INLINE Utility	wideReportChange	 (Control& control);
	HFSM_INLINE Utility	wideReportUtilize	 (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideEnterRequested	 (PlanControl& control);
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

template <StateID NInitialID,
		  ShortIndex NCompoIndex,
		  ShortIndex NOrthoIndex,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename TInitial>
struct _OS<NInitialID, NCompoIndex, NOrthoIndex, TArgs, NIndex, TInitial> {
	static constexpr StateID	INITIAL_ID	= NInitialID;
	static constexpr ShortIndex COMPO_INDEX	= NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	= NOrthoIndex;
	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			= TArgs;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
	using Payload		= typename Args::Payload;

	using Request		= RequestT<Payload>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;

	using Control		= ControlT		<Args>;
	using PlanControl	= PlanControlT	<Args>;
	using FullControl	= FullControlT	<Args>;
	using GuardControl	= GuardControlT<Args>;

	using Initial		= Material<INITIAL_ID, COMPO_INDEX, ORTHO_INDEX, Args, TInitial>;
	using AllForward	= _OSF<TInitial>;

	HFSM_INLINE void	wideRegister			(StateRegistry& stateRegistry, const ForkID forkId);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool	wideForwardEntryGuard(GuardControl& control,						const OrthoFork& prongs);
	HFSM_INLINE bool	wideForwardEntryGuard(GuardControl& control);
	HFSM_INLINE bool	wideEntryGuard		 (GuardControl& control);

	HFSM_INLINE void	wideEnter			 (PlanControl& control);
	HFSM_INLINE void	wideReenter			 (PlanControl& control);

	HFSM_INLINE Status	wideUpdate			 (FullControl& control);

	template <typename TEvent>
	HFSM_INLINE Status	wideReact			 (FullControl& control, const TEvent& event);

	HFSM_INLINE bool	wideForwardExitGuard (GuardControl& control,						const OrthoFork& prongs);
	HFSM_INLINE bool	wideForwardExitGuard (GuardControl& control);
	HFSM_INLINE bool	wideExitGuard		 (GuardControl& control);

	HFSM_INLINE void	wideExit			 (PlanControl& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideForwardActive	 (Control& control, const RequestType request,	const OrthoFork& prongs);
	HFSM_INLINE void	wideForwardRequest	 (Control& control, const RequestType request,	const OrthoFork& prongs);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideRequestChange	 (Control& control);
	HFSM_INLINE void	wideRequestRemain	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestRestart	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestResume	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestUtilize	 (Control& control);

	HFSM_INLINE Utility	wideReportChange	 (Control& control);
	HFSM_INLINE Utility	wideReportUtilize	 (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideEnterRequested	 (PlanControl& control);
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
