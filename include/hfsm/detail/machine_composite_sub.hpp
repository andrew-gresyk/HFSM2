namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename, typename, ShortIndex, typename...>
struct _CS;

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TPayloadList,
		  ShortIndex NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct _CS<TContext, TPayloadList, NIndex, TInitial, TRemaining...> {
	using Context			= TContext;
	using Control			= ControlT<Context>;
	using PayloadList		= TPayloadList;
	using StateRegistryBase	= StateRegistryBaseT<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= enum Transition::Type;
	using TransitionControl	= TransitionControlT<Context, PayloadList>;
	using Initial			= typename WrapState<Context, PayloadList, TInitial>::Type;
	using Remaining			= _CS<Context, PayloadList, NIndex + 1, TRemaining...>;
	using StateList			= typename MergeT<typename Initial::StateList, typename Remaining::StateList>::TypeList;
	HSFM_IF_DEBUG(StateList stateList);

	enum : LongIndex {
		PRONG_INDEX	  = NIndex,
		REVERSE_DEPTH = Max<Initial::REVERSE_DEPTH, Remaining::REVERSE_DEPTH>::VALUE,
		DEEP_WIDTH	  = Max<Initial::DEEP_WIDTH, Remaining::DEEP_WIDTH>::VALUE,
		STATE_COUNT	  = Initial::STATE_COUNT + Remaining::STATE_COUNT,
		FORK_COUNT	  = Initial::FORK_COUNT  + Remaining::FORK_COUNT,
		PRONG_COUNT	  = Initial::PRONG_COUNT + Remaining::PRONG_COUNT,
	};

	_CS(StateRegistryBase& stateRegistry,
		const ShortIndex fork,
		const TypeInfo parentInfo,
		Parents& forkParents,
		ForkPointers& forkPointers);

	void wideRegister(StateRegistryBase& stateRegistry,
					  const ShortIndex fork,
					  const TypeInfo parentInfo);

	inline void wideForwardGuard	 (const ShortIndex prong, TransitionControl& control);
	inline void wideGuard			 (const ShortIndex prong, TransitionControl& control);

	inline void wideEnterInitial	 (						  Control& control);
	inline void wideEnter			 (const ShortIndex prong, Control& control);

	inline bool wideUpdate			 (const ShortIndex prong, TransitionControl& control);

	template <typename TEvent>
	inline void wideReact			 (const ShortIndex prong, const TEvent& event, TransitionControl& control);

	inline void wideExit			 (const ShortIndex prong, Control& control);

	inline void wideForwardRequest	 (const ShortIndex prong, const TransitionType transition);
	inline void wideRequestRemain	 ();
	inline void wideRequestRestart	 ();
	inline void wideRequestResume	 (const ShortIndex prong);
	inline void wideChangeToRequested(const ShortIndex prong, Control& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	enum : LongIndex {
		NAME_COUNT	 = Initial::NAME_COUNT  + Remaining::NAME_COUNT,
	};

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;

	void wideIsActive(const ShortIndex prong,
					  LongIndex& index,
					  MachineStructure& structure) const;
#endif

	Initial initial;
	Remaining remaining;
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TPayloadList,
		  ShortIndex NIndex,
		  typename TInitial>
struct _CS<TContext, TPayloadList, NIndex, TInitial> {
	using Context			= TContext;
	using Control			= ControlT<Context>;
	using PayloadList		= TPayloadList;
	using StateRegistryBase	= StateRegistryBaseT<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= enum Transition::Type;
	using TransitionControl	= TransitionControlT<Context, PayloadList>;
	using Initial			= typename WrapState<Context, PayloadList, TInitial>::Type;
	using StateList			= typename Initial::StateList;
	HSFM_IF_DEBUG(StateList stateList);

	enum : LongIndex {
		PRONG_INDEX	  = NIndex,
		REVERSE_DEPTH = Initial::REVERSE_DEPTH,
		DEEP_WIDTH	  = Max<1, Initial::DEEP_WIDTH>::VALUE,
		STATE_COUNT	  = Initial::STATE_COUNT,
		FORK_COUNT	  = Initial::FORK_COUNT,
		PRONG_COUNT	  = Initial::PRONG_COUNT,
	};

	_CS(StateRegistryBase& stateRegistry,
		const ShortIndex fork,
		const TypeInfo parentInfo,
		Parents& forkParents,
		ForkPointers& forkPointers);

	void wideRegister(StateRegistryBase& stateRegistry,
					  const ShortIndex fork,
					  const TypeInfo parentInfo);

	inline void wideForwardGuard	 (const ShortIndex prong, TransitionControl& control);
	inline void wideGuard			 (const ShortIndex prong, TransitionControl& control);

	inline void wideEnterInitial	 (						  Control& control);
	inline void wideEnter			 (const ShortIndex prong, Control& control);

	inline bool wideUpdate			 (const ShortIndex prong, TransitionControl& control);

	template <typename TEvent>
	inline void wideReact			 (const ShortIndex prong, const TEvent& event, TransitionControl& control);

	inline void wideExit			 (const ShortIndex prong, Control& control);

	inline void wideForwardRequest	 (const ShortIndex prong, const TransitionType transition);
	inline void wideRequestRemain	 ();
	inline void wideRequestRestart	 ();
	inline void wideRequestResume	 (const ShortIndex prong);
	inline void wideChangeToRequested(const ShortIndex prong, Control& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	enum : LongIndex {
		NAME_COUNT	 = Initial::NAME_COUNT,
	};

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;

	void wideIsActive(const ShortIndex prong,
					  LongIndex& index,
					  MachineStructure& structure) const;
#endif

	Initial initial;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_composite_sub_1.inl"
#include "machine_composite_sub_2.inl"
