namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <
	typename TIndices
  , typename TArgs
  , typename THead
  , typename... TSubStates
>
struct HFSM2_EMPTY_BASES O_
	: S_<TIndices, TArgs, THead>
	, OS_<
		  I_<
			  TIndices::STATE_ID + 1
			, TIndices::COMPO_INDEX
			, TIndices::ORTHO_INDEX + 1
			, TIndices::ORTHO_UNIT + contain(OI_<THead, TSubStates...>::WIDTH, 8)
		  >
		, TArgs
		, 0
		, TSubStates...
	>
{
	using Indices		= TIndices;
	static constexpr StateID  HEAD_ID	  = Indices::STATE_ID;
	static constexpr Short	  COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	  ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	  ORTHO_UNIT  = Indices::ORTHO_UNIT;

	static constexpr RegionID REGION_ID	  = COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID	  ORTHO_ID	  = static_cast<ForkID>(-ORTHO_INDEX - 1);

	using Args			= TArgs;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using ReactOrder	= typename Args::ReactOrder;

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;
	using OrthoForks	= typename Registry::OrthoForks;
	using ProngBits		= typename OrthoForks::Bits;
	using ProngCBits	= typename OrthoForks::CBits;

	using ConstControl	= ConstControlT<Args>;
	using ScopedCRegion	= typename ConstControl::Region;

	using Control		= ControlT	   <Args>;
	using ScopedOrigin	= typename Control::Origin;

	using PlanControl	= PlanControlT <Args>;
	using ScopedRegion	= typename PlanControl::Region;

	using FullControl	= FullControlT <Args>;
	using ControlLock	= typename FullControl::Lock;
	using ScopedRegion	= typename PlanControl::Region;

	using GuardControl	= GuardControlT<Args>;
	using EventControl	= EventControlT<Args>;


	using Head			= THead;
	using HeadState		= S_<Indices, Args, Head>;

	using Info			= OI_<Head, TSubStates...>;
	static constexpr Short WIDTH		= Info::WIDTH;
	static constexpr Short REGION_SIZE	= Info::STATE_COUNT;
	static constexpr Short ORTHO_UNITS	= Info::ORTHO_UNITS;

	using SubStates		= OS_<
							  I_<
								  HEAD_ID + 1,
								  COMPO_INDEX,
								  ORTHO_INDEX + 1,
								  ORTHO_UNIT + contain(WIDTH, 8)
							  >,
							  Args,
							  0,
							  TSubStates...
						  >;

	using PreReactWrapper	= PreReactWrapperT <HeadState, SubStates, ReactOrder>;
	using ReactWrapper		= ReactWrapperT	   <HeadState, SubStates, ReactOrder>;
	using PostReactWrapper	= PostReactWrapperT<HeadState, SubStates, ReactOrder>;
	using QueryWrapper		= QueryWrapperT	   <HeadState, SubStates, ReactOrder>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(11)	static ProngBits  orthoRequested (		Registry& registry)		noexcept	{ return			   registry.orthoRequested.template  bits<ORTHO_UNIT, WIDTH>();	}
	HFSM2_CONSTEXPR(11)	static ProngCBits orthoRequested (const Registry& registry)		noexcept	{ return			   registry.orthoRequested.template cbits<ORTHO_UNIT, WIDTH>();	}

	HFSM2_CONSTEXPR(11)	static ProngBits  orthoRequested (		Control& control )		noexcept	{ return control._core.registry.orthoRequested.template  bits<ORTHO_UNIT, WIDTH>();	}
	HFSM2_CONSTEXPR(11)	static ProngCBits orthoRequested (const	Control& control )		noexcept	{ return control._core.registry.orthoRequested.template cbits<ORTHO_UNIT, WIDTH>();	}

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(11)	static TaskStatus& headStatus	 (		Control& control )		noexcept	{ return control._core.planData.headStatuses[REGION_ID];	}
	HFSM2_CONSTEXPR(11)	static TaskStatus&  subStatus	 (		Control& control )		noexcept	{ return control._core.planData.subStatuses [REGION_ID];	}
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void		deepRegister		 (	  Registry& registry, const Parent parent )			noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	bool		deepForwardEntryGuard(GuardControl& control						  )			noexcept;
	HFSM2_CONSTEXPR(14)	bool		deepEntryGuard		 (GuardControl& control						  )			noexcept;

	HFSM2_CONSTEXPR(14)	void		deepEnter			 ( PlanControl& control						  )			noexcept;
	HFSM2_CONSTEXPR(14)	void		deepReenter			 ( PlanControl& control						  )			noexcept;

	HFSM2_CONSTEXPR(14)	TaskStatus	deepPreUpdate		 ( FullControl& control						  )			noexcept;
	HFSM2_CONSTEXPR(14)	TaskStatus	deepUpdate			 ( FullControl& control						  )			noexcept;
	HFSM2_CONSTEXPR(14)	TaskStatus	deepPostUpdate		 ( FullControl& control						  )			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	TaskStatus	deepPreReact		 (EventControl& control, const TEvent& event  )			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	TaskStatus	deepReact			 (EventControl& control, const TEvent& event  )			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	TaskStatus	deepPostReact		 (EventControl& control, const TEvent& event  )			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void		deepQuery			 (ConstControl& control,	   TEvent& event  )	  const noexcept;

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14)	TaskStatus	deepUpdatePlans		 ( FullControl& control						  )			noexcept;
#endif

	HFSM2_CONSTEXPR(14)	bool		deepForwardExitGuard (GuardControl& control						  )			noexcept;
	HFSM2_CONSTEXPR(14)	bool		deepExitGuard		 (GuardControl& control						  )			noexcept;

	HFSM2_CONSTEXPR(14)	void		deepExit			 ( PlanControl& control						  )			noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void		deepForwardActive	 (	   Control& control, const Request request)			noexcept;
	HFSM2_CONSTEXPR(14)	void		deepForwardRequest	 (	   Control& control, const Request request)			noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void		deepRequest			 (	   Control& control, const Request request)			noexcept;

	HFSM2_CONSTEXPR(14)	void		deepRequestChange	 (	   Control& control, const Request request)			noexcept;
	HFSM2_CONSTEXPR(14)	void		deepRequestRestart	 (	   Control& control, const Request request)			noexcept;
	HFSM2_CONSTEXPR(14)	void		deepRequestResume	 (	   Control& control, const Request request)			noexcept;
	HFSM2_CONSTEXPR(14)	void		deepRequestSelect	 (	   Control& control, const Request request)			noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void		deepRequestUtilize	 (	   Control& control, const Request request)			noexcept;
	HFSM2_CONSTEXPR(14)	void		deepRequestRandomize (	   Control& control, const Request request)			noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	UP			deepReportChange	 (	   Control& control						  )			noexcept;

	HFSM2_CONSTEXPR(14)	UP			deepReportUtilize	 (	   Control& control						  )			noexcept;
	HFSM2_CONSTEXPR(14)	Rank		deepReportRank		 (	   Control& control						  )			noexcept;
	HFSM2_CONSTEXPR(14)	Utility		deepReportRandomize  (	   Control& control						  )			noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void		deepChangeToRequested( PlanControl& control						  )			noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_CONSTEXPR(14)	void		deepSaveActive		 (const Registry& registry, WriteStream& stream)  const noexcept;
	HFSM2_CONSTEXPR(14)	void		deepSaveResumable	 (const Registry& registry, WriteStream& stream)  const noexcept;

	HFSM2_CONSTEXPR(14)	void		deepLoadRequested	 (		Registry& registry,  ReadStream& stream)  const noexcept;
	HFSM2_CONSTEXPR(14)	void		deepLoadResumable	 (		Registry& registry,  ReadStream& stream)  const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename Args::StructureStateInfos;

	static constexpr Long NAME_COUNT	 = HeadState::NAME_COUNT  + SubStates::NAME_COUNT;

	HFSM2_CONSTEXPR(14)	void deepGetNames(const Long parent,
										  const RegionType region,
										  const Short depth,
										  StructureStateInfos& stateInfos)							  const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "orthogonal.inl"
