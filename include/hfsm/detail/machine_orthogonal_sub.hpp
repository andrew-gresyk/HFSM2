namespace hfsm {
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
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			 = TArgs;

	using PayloadList	 = typename Args::PayloadList;

	using StateParents	 = Array<Parent, Args::STATE_COUNT>;
	using Request		 = RequestT<PayloadList>;
	using RequestType	 = typename Request::Type;

	using Registry		 = RegistryT   <Args>;
	using PlanControl	 = PlanControlT<Args>;
	using FullControl	 = FullControlT<Args>;

	using Initial		 = typename WrapMaterial<INITIAL_ID, COMPO_INDEX, ORTHO_INDEX, Args, TInitial>::Type;
	using InitialForward = typename WrapForward<TInitial>::Type;

	using Remaining		 = _OS<INITIAL_ID + InitialForward::STATE_COUNT,
						 	   COMPO_INDEX + InitialForward::COMPO_COUNT,
						 	   ORTHO_INDEX + InitialForward::ORTHO_COUNT,
						 	   Args, PRONG_INDEX + 1, TRemaining...>;
	using Forward		 = _OSF<TInitial, TRemaining...>;

	_OS(Registry& registry, const ForkID fork);

	inline void	  wideForwardGuard		(const BitArray& prongs,
										 FullControl& control);
	inline void	  wideForwardGuard		(FullControl& control);
	inline void	  wideGuard				(FullControl& control);

	inline void	  wideEnterInitial		(PlanControl& control);
	inline void	  wideEnter				(PlanControl& control);

	inline Status wideUpdate			(FullControl& control);

	template <typename TEvent>
	inline void   wideReact				(const TEvent& event,
										 FullControl& control);

	inline void   wideExit				(PlanControl& control);

	inline void   wideForwardRequest	(Registry& registry, const BitArray& prongs, const RequestType request);
	inline void   wideRequestRemain		(Registry& registry);
	inline void   wideRequestRestart	(Registry& registry);
	inline void   wideRequestResume		(Registry& registry);
	inline void   wideChangeToRequested	(Registry& registry,
										 PlanControl& control);

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
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			 = TArgs;

	using PayloadList	 = typename Args::PayloadList;

	using StateParents	 = Array<Parent, Args::STATE_COUNT>;
	using Transition	 = RequestT<PayloadList>;
	using RequestType = typename Transition::Type;

	using Registry		 = RegistryT   <Args>;
	using PlanControl	 = PlanControlT<Args>;
	using FullControl	 = FullControlT<Args>;

	using Initial		 = typename WrapMaterial<INITIAL_ID, COMPO_INDEX, ORTHO_INDEX, Args, TInitial>::Type;
	using Forward		 = _OSF<TInitial>;

	_OS(Registry& registry, const ForkID fork);

	inline void   wideForwardGuard		(const BitArray& prongs, FullControl& control);
	inline void   wideForwardGuard		(FullControl& control);
	inline void   wideGuard				(FullControl& control);

	inline void   wideEnterInitial		(PlanControl& control);
	inline void   wideEnter				(PlanControl& control);

	inline Status wideUpdate			(FullControl& control);

	template <typename TEvent>
	inline void   wideReact				(const TEvent& event, FullControl& control);

	inline void   wideExit				(PlanControl& control);

	inline void   wideForwardRequest	(Registry& registry, const BitArray& prongs, const RequestType transition);
	inline void   wideRequestRemain		(Registry& registry);
	inline void   wideRequestRestart	(Registry& registry);
	inline void   wideRequestResume		(Registry& registry);
	inline void   wideChangeToRequested	(Registry& registry, PlanControl& control);

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
