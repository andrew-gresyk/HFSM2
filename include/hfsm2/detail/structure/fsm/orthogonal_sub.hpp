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

	using Args			 = TArgs;

	using PayloadList	 = typename Args::PayloadList;

	using Request		 = RequestT<PayloadList>;
	using RequestType	 = typename Request::Type;

	using StateRegistry	 = StateRegistryT<Args>;
	using StateParents	 = typename StateRegistry::StateParents;

	using Control		 = ControlT		 <Args>;
	using FullControl	 = FullControlT  <Args>;
	using GuardControl	 = GuardControlT <Args>;

	using Initial		 = Material<INITIAL_ID, COMPO_INDEX, ORTHO_INDEX, Args, TInitial>;
	using InitialForward = Wrap<TInitial>;

	using Remaining		 = _OS<INITIAL_ID  + InitialForward::STATE_COUNT,
						 	   COMPO_INDEX + InitialForward::COMPO_COUNT,
						 	   ORTHO_INDEX + InitialForward::ORTHO_COUNT,
						 	   Args,
							   PRONG_INDEX + 1,
							   TRemaining...>;

	using AllForward	 = _OSF<TInitial, TRemaining...>;

	HFSM_INLINE void   wideRegister			(StateRegistry& stateRegistry, const ForkID forkId);

	HFSM_INLINE bool   wideForwardEntryGuard(GuardControl& control,									 const OrthoFork& prongs);
	HFSM_INLINE bool   wideForwardEntryGuard(GuardControl& control);
	HFSM_INLINE bool   wideEntryGuard		(GuardControl& control);

	HFSM_INLINE void   wideEnterInitial		(Control& control);
	HFSM_INLINE void   wideEnter			(Control& control);

	HFSM_INLINE Status wideUpdate			(FullControl& control);

	template <typename TEvent>
	HFSM_INLINE void   wideReact			(const TEvent& event, FullControl& control);

	HFSM_INLINE bool   wideForwardExitGuard	(GuardControl& control,									  const OrthoFork& prongs);
	HFSM_INLINE bool   wideForwardExitGuard	(GuardControl& control);
	HFSM_INLINE bool   wideExitGuard		(GuardControl& control);

	HFSM_INLINE void   wideExit				(Control& control);

	HFSM_INLINE void   wideForwardActive	(StateRegistry& stateRegistry, const RequestType request, const OrthoFork& prongs);
	HFSM_INLINE void   wideForwardRequest	(StateRegistry& stateRegistry, const RequestType request, const OrthoFork& prongs);
	HFSM_INLINE void   wideRequestRemain	(StateRegistry& stateRegistry);
	HFSM_INLINE void   wideRequestRestart	(StateRegistry& stateRegistry);
	HFSM_INLINE void   wideRequestResume	(StateRegistry& stateRegistry);

	HFSM_INLINE void   wideChangeToRequested(StateRegistry& stateRegistry, Control& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
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

	using Args			 = TArgs;

	using PayloadList	 = typename Args::PayloadList;

	using Request		 = RequestT<PayloadList>;
	using RequestType	 = typename Request::Type;

	using StateRegistry	 = StateRegistryT<Args>;
	using StateParents	 = typename StateRegistry::StateParents;

	using Control		 = ControlT		 <Args>;
	using FullControl	 = FullControlT  <Args>;
	using GuardControl	 = GuardControlT <Args>;

	using Initial		 = Material<INITIAL_ID, COMPO_INDEX, ORTHO_INDEX, Args, TInitial>;
	using AllForward	 = _OSF<TInitial>;

	HFSM_INLINE void   wideRegister			(StateRegistry& stateRegistry, const ForkID forkId);

	HFSM_INLINE bool   wideForwardEntryGuard(GuardControl& control,										 const OrthoFork& prongs);
	HFSM_INLINE bool   wideForwardEntryGuard(GuardControl& control);
	HFSM_INLINE bool   wideEntryGuard		(GuardControl& control);

	HFSM_INLINE void   wideEnterInitial		(Control& control);
	HFSM_INLINE void   wideEnter			(Control& control);

	HFSM_INLINE Status wideUpdate			(FullControl& control);

	template <typename TEvent>
	HFSM_INLINE void   wideReact			(const TEvent& event, FullControl& control);

	HFSM_INLINE bool   wideForwardExitGuard	(GuardControl& control,										 const OrthoFork& prongs);
	HFSM_INLINE bool   wideForwardExitGuard	(GuardControl& control);
	HFSM_INLINE bool   wideExitGuard		(GuardControl& control);

	HFSM_INLINE void   wideExit				(Control& control);

	HFSM_INLINE void   wideForwardActive	(StateRegistry& stateRegistry, const RequestType transition, const OrthoFork& prongs);
	HFSM_INLINE void   wideForwardRequest	(StateRegistry& stateRegistry, const RequestType transition, const OrthoFork& prongs);
	HFSM_INLINE void   wideRequestRemain	(StateRegistry& stateRegistry);
	HFSM_INLINE void   wideRequestRestart	(StateRegistry& stateRegistry);
	HFSM_INLINE void   wideRequestResume	(StateRegistry& stateRegistry);

	HFSM_INLINE void   wideChangeToRequested(StateRegistry& stateRegistry, Control& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
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
