namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID, ShortIndex, ShortIndex, typename, ShortIndex, typename...>
struct _CS;

//------------------------------------------------------------------------------

template <StateID NInitialID,
		  ShortIndex NCompoIndex,
		  ShortIndex NOrthoIndex,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct _CS<NInitialID, NCompoIndex, NOrthoIndex, TArgs, NIndex, TInitial, TRemaining...> {
	static constexpr StateID	INITIAL_ID	= NInitialID;
	static constexpr ShortIndex COMPO_INDEX	= NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	= NOrthoIndex;
	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			 = TArgs;

	using Context		 = typename Args::Context;
	using StateList		 = typename Args::StateList;
	using RegionList	 = typename Args::RegionList;
	using PayloadList	 = typename Args::PayloadList;

	using StateParents	 = Array<Parent, StateList::SIZE>;
	using Request		 = RequestT<PayloadList>;
	using RequestType	 = typename Request::Type;

	using StateData		 = StateDataT  <Args>;
	using Control		 = ControlT	   <Args>;
	using FullControl	 = FullControlT<Args>;
	using GuardControl	 = GuardControlT<Args>;

	using Initial		 = typename WrapMaterial<INITIAL_ID, COMPO_INDEX, ORTHO_INDEX, Args, TInitial>::Type;
	using InitialForward = typename WrapForward<TInitial>::Type;

	using Remaining		 = _CS<INITIAL_ID + InitialForward::STATE_COUNT,
						 	   COMPO_INDEX + InitialForward::COMPO_COUNT,
						 	   ORTHO_INDEX + InitialForward::ORTHO_COUNT,
						 	   Args, NIndex + 1, TRemaining...>;
	using Forward		 = _CSF<TInitial, TRemaining...>;

	HFSM_INLINE void   wideRegister			(StateData& stateData, const ForkID forkId);

	HFSM_INLINE bool   wideForwardGuard		(const ShortIndex prong, GuardControl& control);
	HFSM_INLINE bool   wideGuard			(const ShortIndex prong, GuardControl& control);

	HFSM_INLINE void   wideEnterInitial		(						 Control& control);
	HFSM_INLINE void   wideEnter			(const ShortIndex prong, Control& control);

	HFSM_INLINE Status wideUpdate			(const ShortIndex prong, FullControl& control);

	template <typename TEvent>
	HFSM_INLINE void   wideReact			(const ShortIndex prong, const TEvent& event,
																	 FullControl& control);

	HFSM_INLINE void   wideExit				(const ShortIndex prong, Control& control);

	HFSM_INLINE void   wideForwardRequest	(StateData& stateData, const ShortIndex prong, const RequestType request);
	HFSM_INLINE void   wideRequestRemain	(StateData& stateData);
	HFSM_INLINE void   wideRequestRestart	(StateData& stateData);
	HFSM_INLINE void   wideRequestResume	(StateData& stateData, const ShortIndex prong);
	HFSM_INLINE void   wideChangeToRequested(StateData& stateData, const ShortIndex prong,
											 Control& control);

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
struct _CS<NInitialID, NCompoIndex, NOrthoIndex, TArgs, NIndex, TInitial> {
	static constexpr StateID	INITIAL_ID	= NInitialID;
	static constexpr ShortIndex COMPO_INDEX	= NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	= NOrthoIndex;
	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			 = TArgs;

	using Context		 = typename Args::Context;
	using StateList		 = typename Args::StateList;
	using RegionList	 = typename Args::RegionList;
	using PayloadList	 = typename Args::PayloadList;

	using StateParents	 = Array<Parent, StateList::SIZE>;
	using Request		 = RequestT<PayloadList>;
	using RequestType	 = typename Request::Type;

	using StateData		 = StateDataT  <Args>;
	using Control		 = ControlT	   <Args>;
	using FullControl	 = FullControlT<Args>;
	using GuardControl	 = GuardControlT<Args>;

	using Initial		 = typename WrapMaterial<INITIAL_ID, COMPO_INDEX, ORTHO_INDEX, Args, TInitial>::Type;
	using Forward		 = _CSF<TInitial>;

	HFSM_INLINE void   wideRegister			(StateData& stateData, const ForkID forkId);

	HFSM_INLINE bool   wideForwardGuard		(const ShortIndex prong, GuardControl& control);
	HFSM_INLINE bool   wideGuard			(const ShortIndex prong, GuardControl& control);

	HFSM_INLINE void   wideEnterInitial		(						 Control& control);
	HFSM_INLINE void   wideEnter			(const ShortIndex prong, Control& control);

	HFSM_INLINE Status wideUpdate			(const ShortIndex prong, FullControl& control);

	template <typename TEvent>
	HFSM_INLINE void   wideReact			(const ShortIndex prong, const TEvent& event,
				  													 FullControl& control);

	HFSM_INLINE void   wideExit				(const ShortIndex prong, Control& control);

	HFSM_INLINE void   wideForwardRequest	(StateData& stateData, const ShortIndex prong, const RequestType transition);
	HFSM_INLINE void   wideRequestRemain	(StateData& stateData);
	HFSM_INLINE void   wideRequestRestart	(StateData& stateData);
	HFSM_INLINE void   wideRequestResume	(StateData& stateData, const ShortIndex prong);
	HFSM_INLINE void   wideChangeToRequested(StateData& stateData, const ShortIndex prong,
											 Control& control);

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

#include "machine_composite_sub_1.inl"
#include "machine_composite_sub_2.inl"
