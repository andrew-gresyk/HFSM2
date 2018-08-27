namespace hfsm {
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
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			 = TArgs;

	using Context		 = typename Args::Context;
	using StateList		 = typename Args::StateList;
	using PayloadList	 = typename Args::PayloadList;

	using StateParents	 = Array<Parent, StateList::SIZE>;
	using Request		 = RequestT<PayloadList>;
	using RequestType	 = typename Request::Type;

	using Registry		 = RegistryT   <Args>;
	using PlanControl	 = PlanControlT<Args>;
	using FullControl	 = FullControlT<Args>;

	using Initial		 = typename WrapMaterial<INITIAL_ID, COMPO_INDEX, ORTHO_INDEX, Args, TInitial>::Type;
	using InitialForward = typename WrapForward<TInitial>::Type;

	using Remaining		 = _CS<INITIAL_ID + InitialForward::STATE_COUNT,
						 	   COMPO_INDEX + InitialForward::COMPO_COUNT,
						 	   ORTHO_INDEX + InitialForward::ORTHO_COUNT,
						 	   Args, NIndex + 1, TRemaining...>;
	using Forward		 = _CSF<TInitial, TRemaining...>;

	_CS(Registry& registry, const ForkID fork);

	inline void   wideForwardGuard		(const ShortIndex prong, FullControl& control);
	inline void   wideGuard				(const ShortIndex prong, FullControl& control);

	inline void   wideEnterInitial		(						 PlanControl& control);
	inline void   wideEnter				(const ShortIndex prong, PlanControl& control);

	inline Status wideUpdate			(const ShortIndex prong, FullControl& control);

	template <typename TEvent>
	inline void   wideReact				(const ShortIndex prong, const TEvent& event,
																 FullControl& control);

	inline void   wideExit				(const ShortIndex prong, PlanControl& control);

	inline void   wideForwardRequest	(Registry& registry, const ShortIndex prong, const RequestType request);
	inline void   wideRequestRemain		(Registry& registry);
	inline void   wideRequestRestart	(Registry& registry);
	inline void   wideRequestResume		(Registry& registry, const ShortIndex prong);
	inline void   wideChangeToRequested	(Registry& registry, const ShortIndex prong, PlanControl& control);

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
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			 = TArgs;

	using Context		 = typename Args::Context;
	using StateList		 = typename Args::StateList;
	using PayloadList	 = typename Args::PayloadList;

	using StateParents	 = Array<Parent, StateList::SIZE>;
	using Request		 = RequestT<PayloadList>;
	using RequestType	 = typename Request::Type;

	using Registry		 = RegistryT   <Args>;
	using PlanControl	 = PlanControlT<Args>;
	using FullControl	 = FullControlT<Args>;

	using Initial		 = typename WrapMaterial<INITIAL_ID, COMPO_INDEX, ORTHO_INDEX, Args, TInitial>::Type;
	using Forward		 = _CSF<TInitial>;

	_CS(Registry& registry, const ForkID fork);

	inline void   wideForwardGuard		(const ShortIndex prong, FullControl& control);
	inline void   wideGuard				(const ShortIndex prong, FullControl& control);

	inline void   wideEnterInitial		(						 PlanControl& control);
	inline void   wideEnter				(const ShortIndex prong, PlanControl& control);

	inline Status wideUpdate			(const ShortIndex prong, FullControl& control);

	template <typename TEvent>
	inline void   wideReact				(const ShortIndex prong, const TEvent& event,
				  												 FullControl& control);

	inline void   wideExit				(const ShortIndex prong, PlanControl& control);

	inline void   wideForwardRequest	(Registry& registry, const ShortIndex prong, const RequestType transition);
	inline void   wideRequestRemain		(Registry& registry);
	inline void   wideRequestRestart	(Registry& registry);
	inline void   wideRequestResume		(Registry& registry, const ShortIndex prong);
	inline void   wideChangeToRequested	(Registry& registry, const ShortIndex prong, PlanControl& control);

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
