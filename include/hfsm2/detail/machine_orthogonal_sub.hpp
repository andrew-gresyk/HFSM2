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

	using StateParents	 = Array<Parent, Args::STATE_COUNT>;
	using Request		 = RequestT<PayloadList>;
	using RequestType	 = typename Request::Type;

	using StateData		 = StateDataT  <Args>;
	using Control		 = ControlT	   <Args>;
	using FullControl	 = FullControlT<Args>;

	using Initial		 = typename WrapMaterial<INITIAL_ID, COMPO_INDEX, ORTHO_INDEX, Args, TInitial>::Type;
	using InitialForward = typename WrapForward<TInitial>::Type;

	using Remaining		 = _OS<INITIAL_ID + InitialForward::STATE_COUNT,
						 	   COMPO_INDEX + InitialForward::COMPO_COUNT,
						 	   ORTHO_INDEX + InitialForward::ORTHO_COUNT,
						 	   Args, PRONG_INDEX + 1, TRemaining...>;
	using Forward		 = _OSF<TInitial, TRemaining...>;

	using Prongs		 = typename OrthoFork::Prongs;

	_OS(StateData& stateData, const ForkID forkId);

	HSFM_INLINE void   wideForwardGuard		(const Prongs& prongs,
					 						 FullControl& control);
	HSFM_INLINE void   wideForwardGuard		(FullControl& control);
	HSFM_INLINE void   wideGuard			(FullControl& control);

	HSFM_INLINE void   wideEnterInitial		(Control& control);
	HSFM_INLINE void   wideEnter			(Control& control);

	HSFM_INLINE Status wideUpdate			(FullControl& control);

	template <typename TEvent>
	HSFM_INLINE void   wideReact			(const TEvent& event,
											 FullControl& control);

	HSFM_INLINE void   wideExit				(Control& control);

	HSFM_INLINE void   wideForwardRequest	(StateData& stateData, const Prongs& prongs, const RequestType request);
	HSFM_INLINE void   wideRequestRemain	(StateData& stateData);
	HSFM_INLINE void   wideRequestRestart	(StateData& stateData);
	HSFM_INLINE void   wideRequestResume	(StateData& stateData);
	HSFM_INLINE void   wideChangeToRequested(StateData& stateData,
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
struct _OS<NInitialID, NCompoIndex, NOrthoIndex, TArgs, NIndex, TInitial> {
	static constexpr StateID	INITIAL_ID	= NInitialID;
	static constexpr ShortIndex COMPO_INDEX	= NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	= NOrthoIndex;
	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			 = TArgs;

	using PayloadList	 = typename Args::PayloadList;

	using StateParents	 = Array<Parent, Args::STATE_COUNT>;
	using Request		 = RequestT<PayloadList>;
	using RequestType	 = typename Request::Type;

	using StateData		 = StateDataT  <Args>;
	using Control		 = ControlT	   <Args>;
	using FullControl	 = FullControlT<Args>;

	using Initial		 = typename WrapMaterial<INITIAL_ID, COMPO_INDEX, ORTHO_INDEX, Args, TInitial>::Type;
	using Forward		 = _OSF<TInitial>;

	using Prongs		 = typename OrthoFork::Prongs;

	_OS(StateData& stateData, const ForkID forkId);

	HSFM_INLINE void   wideForwardGuard		(const Prongs& prongs, FullControl& control);
	HSFM_INLINE void   wideForwardGuard		(FullControl& control);
	HSFM_INLINE void   wideGuard			(FullControl& control);

	HSFM_INLINE void   wideEnterInitial		(Control& control);
	HSFM_INLINE void   wideEnter			(Control& control);

	HSFM_INLINE Status wideUpdate			(FullControl& control);

	template <typename TEvent>
	HSFM_INLINE void   wideReact			(const TEvent& event, FullControl& control);

	HSFM_INLINE void   wideExit				(Control& control);

	HSFM_INLINE void   wideForwardRequest	(StateData& stateData, const Prongs& prongs, const RequestType transition);
	HSFM_INLINE void   wideRequestRemain	(StateData& stateData);
	HSFM_INLINE void   wideRequestRestart	(StateData& stateData);
	HSFM_INLINE void   wideRequestResume	(StateData& stateData);
	HSFM_INLINE void   wideChangeToRequested(StateData& stateData,
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

#include "machine_orthogonal_sub_1.inl"
#include "machine_orthogonal_sub_2.inl"
