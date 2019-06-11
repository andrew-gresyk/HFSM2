namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct _O final {
	using Indices		= TIndices;
	static constexpr StateID	HEAD_ID		= Indices::STATE_ID;
	static constexpr ShortIndex COMPO_INDEX	= Indices::COMPO_INDEX;
	static constexpr ShortIndex ORTHO_INDEX	= Indices::ORTHO_INDEX;
	static constexpr ShortIndex ORTHO_UNIT	= Indices::ORTHO_UNIT;

	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID		ORTHO_ID	= (ForkID) -ORTHO_INDEX - 1;

	using Args			= TArgs;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using Payload		= typename Args::Payload;

	using Head			= THead;

	using Forward		= _OF<Head, TSubStates...>;
	static constexpr ShortIndex WIDTH		= Forward::WIDTH;
	static constexpr ShortIndex REGION_SIZE	= Forward::STATE_COUNT;
	static constexpr ShortIndex ORTHO_UNITS	= Forward::ORTHO_UNITS;

	using Request		= RequestT<Payload>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;
	using OrthoForks	= typename StateRegistry::AllForks::Ortho;
	using ProngBits		= typename OrthoForks::Bits;
	using ProngConstBits= typename OrthoForks::ConstBits;

	using Control		= ControlT<Args>;

	using PlanControl	= PlanControlT<Args>;
	using ScopedOrigin	= typename PlanControl::Origin;
	using ScopedRegion	= typename PlanControl::Region;

	using FullControl	= FullControlT<Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;

	using HeadState		= _S<Indices, Args, Head>;
	using SubStates		= _OS<_I<HEAD_ID + 1,
								 COMPO_INDEX,
								 ORTHO_INDEX + 1,
								 ORTHO_UNIT + ORTHO_UNITS>,
							  Args,
							  0,
							  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE ProngBits	   orthoRequested(		StateRegistry& stateRegistry)		{ return stateRegistry.requested.ortho.template bits<ORTHO_UNIT, WIDTH>();	}
	HFSM_INLINE ProngConstBits orthoRequested(const StateRegistry& stateRegistry) const	{ return stateRegistry.requested.ortho.template bits<ORTHO_UNIT, WIDTH>();	}

	HFSM_INLINE ProngBits	   orthoRequested(		Control& control)					{ return orthoRequested(control._stateRegistry);							}
	HFSM_INLINE ProngConstBits orthoRequested(const Control& control) const				{ return orthoRequested(control._stateRegistry);							}

	HFSM_INLINE void	deepRegister		 (StateRegistry& stateRegistry, const Parent parent);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool	deepForwardEntryGuard(GuardControl& control,						const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE bool	deepEntryGuard		 (GuardControl& control,						const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void	deepEnter			 (PlanControl& control,							const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE void	deepReenter			 (PlanControl& control,							const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE Status	deepUpdate			 (FullControl& control,							const ShortIndex = INVALID_SHORT_INDEX);

	template <typename TEvent>
	HFSM_INLINE Status	deepReact			 (FullControl& control,	const TEvent& event,	const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE bool	deepForwardExitGuard (GuardControl& control,						const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE bool	deepExitGuard		 (GuardControl& control,						const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void	deepExit			 (PlanControl& control,							const ShortIndex = INVALID_SHORT_INDEX);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepForwardActive	 (Control& control, const RequestType request,	const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE void	deepForwardRequest	 (Control& control, const RequestType request,	const ShortIndex = INVALID_SHORT_INDEX);

	HFSM_INLINE void	deepRequest			 (Control& control, const RequestType request);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepRequestChange	 (Control& control,								const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE void	deepRequestRemain	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	deepRequestRestart	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	deepRequestResume	 (StateRegistry& stateRegistry,					const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE void	deepRequestUtilize	 (Control& control);

	HFSM_INLINE UP		deepReportChange	 (Control& control,								const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE UP		deepReportUtilize	 (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepEnterRequested	 (PlanControl& control,							const ShortIndex = INVALID_SHORT_INDEX);
	HFSM_INLINE void	deepChangeToRequested(PlanControl& control,							const ShortIndex = INVALID_SHORT_INDEX);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

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

#include "orthogonal.inl"
