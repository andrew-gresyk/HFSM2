namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NInitialID,
		  ShortIndex NCompoIndex,
		  ShortIndex NOrthoIndex,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename... TStates>
struct _CS {
	static constexpr StateID	INITIAL_ID	= NInitialID;
	static constexpr ShortIndex COMPO_INDEX	= NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	= NOrthoIndex;
	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			= TArgs;

	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using PayloadList	= typename Args::PayloadList;

	using Request		= RequestT<PayloadList>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;

	using Control		= ControlT	   <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	static constexpr ShortIndex L_PRONG = PRONG_INDEX;

	using LStates		= SplitL<TStates...>;
	using LHalf			= CSubMaterial<INITIAL_ID,
									   COMPO_INDEX,
									   ORTHO_INDEX,
									   Args,
									   L_PRONG,
									   LStates>;
	using LHalfForward	= typename ForwardT<LHalf>::Type;

	static constexpr ShortIndex R_PRONG = PRONG_INDEX + LStates::SIZE;

	using RStates		= SplitR<TStates...>;
	using RHalf			= CSubMaterial<INITIAL_ID  + LHalfForward::STATE_COUNT,
									   COMPO_INDEX + LHalfForward::COMPO_COUNT,
									   ORTHO_INDEX + LHalfForward::ORTHO_COUNT,
									   Args,
									   R_PRONG,
									   RStates>;

	HFSM_INLINE void   deepRegister			(StateRegistry& stateRegistry, const Parent parent);

	HFSM_INLINE bool   deepForwardEntryGuard(GuardControl& control,									  const ShortIndex prong);
	HFSM_INLINE bool   deepEntryGuard		(GuardControl& control,									  const ShortIndex prong);

	HFSM_INLINE void   deepEnterInitial		(Control& control);
	HFSM_INLINE void   deepEnter			(Control& control,										  const ShortIndex prong);

	HFSM_INLINE Status deepUpdate			(FullControl& control,									  const ShortIndex prong);

	template <typename TEvent>
	HFSM_INLINE void   deepReact			(FullControl& control,		   const TEvent& event,		  const ShortIndex prong);

	HFSM_INLINE bool   deepForwardExitGuard	(GuardControl& control,									  const ShortIndex prong);
	HFSM_INLINE bool   deepExitGuard		(GuardControl& control,									  const ShortIndex prong);

	HFSM_INLINE void   deepExit				(Control& control,										  const ShortIndex prong);

	HFSM_INLINE void   deepForwardActive	(StateRegistry& stateRegistry, const RequestType request, const ShortIndex prong);
	HFSM_INLINE void   deepForwardRequest	(StateRegistry& stateRegistry, const RequestType request, const ShortIndex prong);
	HFSM_INLINE void   deepRequestRemain	(StateRegistry& stateRegistry);
	HFSM_INLINE void   deepRequestChange	(StateRegistry& stateRegistry);
	HFSM_INLINE void   deepRequestRestart	(StateRegistry& stateRegistry);
	HFSM_INLINE void   deepRequestResume	(StateRegistry& stateRegistry,							  const ShortIndex prong);

	HFSM_INLINE void   deepChangeToRequested(StateRegistry& stateRegistry, Control& control,		  const ShortIndex prong);

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
