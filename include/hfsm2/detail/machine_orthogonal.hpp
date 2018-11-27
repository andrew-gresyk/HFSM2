namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NHeadIndex,
		  ShortIndex NCompoIndex,
		  ShortIndex NOrthoIndex,
		  typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct _O final {
	static constexpr StateID	HEAD_ID		= NHeadIndex;
	static constexpr ShortIndex COMPO_INDEX	= NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	= NOrthoIndex;
	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID		ORTHO_ID	= (ForkID) -ORTHO_INDEX - 1;

	using Args			= TArgs;
	using Head			= THead;

	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using PayloadList	= typename Args::PayloadList;

	using StateParents	= Array<Parent, Args::STATE_COUNT>;
	using Request		= RequestT<PayloadList>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;

	using Control		= ControlT<Args>;
	using ControlOrigin	= typename Control::Origin;
	using ControlRegion	= typename Control::Region;

	using FullControl	= FullControlT<Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;

	using HeadState		= _S <HEAD_ID, Args, Head>;
	using SubStates		= _OS<HEAD_ID + 1, COMPO_INDEX, ORTHO_INDEX + 1, Args, 0, TSubStates...>;
	using Forward		= _OF<Head, TSubStates...>;

	static constexpr ShortIndex REGION_SIZE	= Forward::STATE_COUNT;

	HFSM_INLINE		  OrthoFork& orthoRequested(	  StateRegistry& stateRegistry)			{ return stateRegistry.requested.ortho[ORTHO_INDEX];	}
	HFSM_INLINE const OrthoFork& orthoRequested(const StateRegistry& stateRegistry) const	{ return stateRegistry.requested.ortho[ORTHO_INDEX];	}

	HFSM_INLINE		  OrthoFork& orthoRequested(	  Control&   control)					{ return orthoRequested(control.stateRegistry());		}
	HFSM_INLINE const OrthoFork& orthoRequested(const Control&   control) const				{ return orthoRequested(control.stateRegistry());		}

	HFSM_INLINE void   deepRegister			(StateRegistry& stateRegistry, const Parent parent);

	HFSM_INLINE bool   deepForwardEntryGuard(GuardControl& control);
	HFSM_INLINE bool   deepEntryGuard		(GuardControl& control);

	HFSM_INLINE void   deepEnterInitial		(Control& control);
	HFSM_INLINE void   deepEnter			(Control& control);

	HFSM_INLINE Status deepUpdate			(FullControl& control);

	template <typename TEvent>
	HFSM_INLINE void   deepReact			(const TEvent& event,
											 FullControl& control);

	HFSM_INLINE bool   deepForwardExitGuard	(GuardControl& control);
	HFSM_INLINE bool   deepExitGuard		(GuardControl& control);

	HFSM_INLINE void   deepExit				(Control& control);

	HFSM_INLINE void   deepForwardRequest	(StateRegistry& stateRegistry, const RequestType transition);
	HFSM_INLINE void   deepRequestRemain	(StateRegistry& stateRegistry);
	HFSM_INLINE void   deepRequestRestart	(StateRegistry& stateRegistry);
	HFSM_INLINE void   deepRequestResume	(StateRegistry& stateRegistry);
	HFSM_INLINE void   deepChangeToRequested(StateRegistry& stateRegistry,
											 Control& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using RegionType		= typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT	 = HeadState::NAME_COUNT  + SubStates::NAME_COUNT;

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

#include "machine_orthogonal.inl"
