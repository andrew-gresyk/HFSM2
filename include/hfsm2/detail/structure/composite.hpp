namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NHeadIndex,
		  ShortIndex NCompoIndex,
		  ShortIndex NOrthoIndex,
		  typename TArgs,
		  typename THead,
		  RegionStrategy TStrategy,
		  typename... TSubStates>
struct _C {
	static constexpr StateID	HEAD_ID		 = NHeadIndex;
	static constexpr ShortIndex COMPO_INDEX	 = NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	 = NOrthoIndex;
	static constexpr ShortIndex REGION_ID	 = COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID		COMPO_ID	 = COMPO_INDEX + 1;

	using Args			= TArgs;
	using Head			= THead;

	static constexpr RegionStrategy STRATEGY = TStrategy;

	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using PayloadList	= typename Args::PayloadList;

	using Request		= RequestT<PayloadList>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;

	using Control		= ControlT<Args>;
	using ScopedOrigin	= typename Control::Origin;
	using ScopedRegion	= typename Control::Region;

	using FullControl	= FullControlT<Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;

	using Plan			= PlanT<Args>;

	using HeadState		= _S <HEAD_ID, Args, Head>;
	using SubStates		= _CS<HEAD_ID + 1, COMPO_INDEX + 1, ORTHO_INDEX, Args, 0, TSubStates...>;
	using AllForward	= _CF<Head, STRATEGY, TSubStates...>;

	static constexpr ShortIndex REGION_SIZE	= AllForward::STATE_COUNT;

	ShortIndex& compoActive   (Control& control)	{ return control.stateRegistry().compoActive	[COMPO_INDEX];	}
	ShortIndex& compoResumable(Control& control)	{ return control.stateRegistry().resumable.compo[COMPO_INDEX];	}
	ShortIndex& compoRequested(Control& control)	{ return control.stateRegistry().requested.compo[COMPO_INDEX];	}

	HFSM_INLINE void   deepRegister			(StateRegistry& stateRegistry, const Parent parent);

	HFSM_INLINE bool   deepForwardEntryGuard(GuardControl& control,									  const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE bool   deepEntryGuard		(GuardControl& control,									  const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepEnterInitial		(Control& control);
	HFSM_INLINE void   deepEnter			(Control& control,										  const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE Status deepUpdate			(FullControl& control,									  const ShortIndex = INVALID_SHORT_INDEX);

	template <typename TEvent>
	HFSM_INLINE void   deepReact			(FullControl& control,		   const TEvent& event,		  const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE bool   deepForwardExitGuard	(GuardControl& control,									  const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE bool   deepExitGuard		(GuardControl& control,									  const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepExit				(Control& control,										  const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepForwardActive	(StateRegistry& stateRegistry, const RequestType request, const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE void   deepForwardRequest	(StateRegistry& stateRegistry, const RequestType request, const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepRequest			(StateRegistry& stateRegistry, const RequestType request);
	HFSM_INLINE void   deepRequestRemain	(StateRegistry& stateRegistry);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifndef __GNUC__
	// NOTE: beautiful local template specialization: Visual Studio || Clang == <3
	template <RegionStrategy TG = STRATEGY>
	HFSM_INLINE void   deepRequestChange		   (StateRegistry& stateRegistry);

	template <>
	HFSM_INLINE void   deepRequestChange<Composite>(StateRegistry& stateRegistry)					  { deepRequestRestart(stateRegistry);	}

	template <>
	HFSM_INLINE void   deepRequestChange<Resumable>(StateRegistry& stateRegistry)					  { deepRequestResume (stateRegistry);	}
#else
	// NOTE: ugly run-time logic: GCC == T_T
	HFSM_INLINE void   deepRequestChange		   (StateRegistry& stateRegistry);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void   deepRequestRestart	(StateRegistry& stateRegistry);
	HFSM_INLINE void   deepRequestResume	(StateRegistry& stateRegistry,							  const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void   deepChangeToRequested(StateRegistry& stateRegistry, Control& control,		  const ShortIndex = INVALID_SHORT_INDEX);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using RegionType		= typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT = HeadState::NAME_COUNT + SubStates::NAME_COUNT;

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	HeadState _headState;
	SubStates _subStates;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "composite.inl"
