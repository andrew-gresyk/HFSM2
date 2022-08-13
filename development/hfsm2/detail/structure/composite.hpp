namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  Strategy TStrategy,
		  typename THead,
		  typename... TSubStates>
struct HFSM2_EMPTY_BASES C_
	: S_<TIndices, TArgs, THead>
	, CS_<I_<TIndices::STATE_ID + 1,
			 TIndices::COMPO_INDEX + 1,
			 TIndices::ORTHO_INDEX,
			 TIndices::ORTHO_UNIT>,
		  TArgs,
		  TStrategy,
		  0,
		  TL_<TSubStates...>>
{
	using Indices		= TIndices;
	static constexpr StateID  HEAD_ID	  = Indices::STATE_ID;
	static constexpr Short	  COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	  ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	  ORTHO_UNIT  = Indices::ORTHO_UNIT;

	static constexpr Strategy STRATEGY	  = TStrategy;

	static constexpr Short	  REGION_ID	  = COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID	  COMPO_ID	  = COMPO_INDEX + 1;

	using Args			= TArgs;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;

	using ConstControl	= ConstControlT<Args>;
	using Control		= ControlT	   <Args>;
	using ScopedOrigin	= typename Control::Origin;

	using PlanControl	= PlanControlT <Args>;
	using ScopedRegion	= typename PlanControl::Region;

	using FullControl	= FullControlT <Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;

	using Head			= THead;

	using HeadState		= S_<Indices, Args, Head>;

	using SubStates		= CS_<I_<HEAD_ID + 1,
								 COMPO_INDEX + 1,
								 ORTHO_INDEX,
								 ORTHO_UNIT>,
							  Args,
							  STRATEGY,
							  0,
							  TL_<TSubStates...>>;

	using Info			= CI_<STRATEGY, Head, TSubStates...>;

	static constexpr Short WIDTH		  = Info::WIDTH;

#if HFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Short WIDTH_BITS	  = Info::WIDTH_BITS;
#endif

	static constexpr Short REGION_SIZE	  = Info::STATE_COUNT;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_SERIALIZATION_AVAILABLE()
	HFSM2_CONSTEXPR(11)	static Short	compoRequested		  (const Registry& registry)	noexcept	{ return registry.compoRequested[COMPO_INDEX];	}
	HFSM2_CONSTEXPR(11)	static Short&	compoRequested		  (		 Registry& registry)	noexcept	{ return registry.compoRequested[COMPO_INDEX];	}

	HFSM2_CONSTEXPR(11)	static Short	compoActive			  (const Registry& registry)	noexcept	{ return registry.compoActive	[COMPO_INDEX];	}
	HFSM2_CONSTEXPR(11)	static Short&	compoActive			  (		 Registry& registry)	noexcept	{ return registry.compoActive	[COMPO_INDEX];	}

	HFSM2_CONSTEXPR(11)	static Short	compoResumable		  (const Registry& registry)	noexcept	{ return registry.compoResumable[COMPO_INDEX];	}
	HFSM2_CONSTEXPR(11)	static Short&	compoResumable		  (		 Registry& registry)	noexcept	{ return registry.compoResumable[COMPO_INDEX];	}
#endif

	HFSM2_CONSTEXPR(11)	static Short&	compoRequested		  (		  Control& control )	noexcept	{ return control._core.registry.compoRequested[COMPO_INDEX];	}
	HFSM2_CONSTEXPR(11)	static Short&	compoActive			  (		  Control& control )	noexcept	{ return control._core.registry.compoActive	  [COMPO_INDEX];	}
	HFSM2_CONSTEXPR(11)	static Short	compoActive			  (	 ConstControl& control )	noexcept	{ return control._core.registry.compoActive	  [COMPO_INDEX];	}
	HFSM2_CONSTEXPR(11)	static Short&	compoResumable		  (		  Control& control )	noexcept	{ return control._core.registry.compoResumable[COMPO_INDEX];	}

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(11)	static Status&	headStatus			  (		  Control& control )	noexcept	{ return control._core.planData.headStatuses  [REGION_ID];		}
	HFSM2_CONSTEXPR(11)	static Status&	subStatus			  (		  Control& control )	noexcept	{ return control._core.planData.subStatuses   [REGION_ID];		}
#endif

	HFSM2_CONSTEXPR(11)	static bool		compoRemain			  (		  Control& control )	noexcept	{ return control._core.registry.compoRemains.template get<COMPO_INDEX>(); }

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	Short	resolveRandom				  (		  Control& control,
															   const Utility(& options)[WIDTH], const Utility sum,
															   const Rank	(& ranks)  [WIDTH], const Rank	  top)  const noexcept;
#endif

	HFSM2_CONSTEXPR(14)	void	deepRegister				  (Registry& registry, const Parent parent)				noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	bool	deepForwardEntryGuard		  (GuardControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	bool	deepEntryGuard				  (GuardControl& control)								noexcept;

	HFSM2_CONSTEXPR(14)	void	deepEnter					  ( PlanControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	void	deepReenter					  ( PlanControl& control)								noexcept;

	HFSM2_CONSTEXPR(14)	Status	deepPreUpdate				  ( FullControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	Status	deepUpdate					  ( FullControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	Status	deepPostUpdate				  ( FullControl& control)								noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	deepPreReact				  ( FullControl& control, const TEvent& event)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	deepReact					  ( FullControl& control, const TEvent& event)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	deepPostReact				  ( FullControl& control, const TEvent& event)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void	deepQuery					  (ConstControl& control,		TEvent& event)	  const noexcept;

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14)	Status	deepUpdatePlans				  ( FullControl& control)								noexcept;
#endif

	HFSM2_CONSTEXPR(14)	bool	deepForwardExitGuard		  (GuardControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	bool	deepExitGuard				  (GuardControl& control)								noexcept;

	HFSM2_CONSTEXPR(14)	void	deepExit					  ( PlanControl& control)								noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void	deepForwardActive			  (		Control& control, const Request request)		noexcept;
	HFSM2_CONSTEXPR(14)	void	deepForwardRequest			  (		Control& control, const Request request)		noexcept;

	HFSM2_CONSTEXPR(14)	void	deepRequest					  (		Control& control, const Request request)		noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()

	template <Strategy = STRATEGY>
	HFSM2_CONSTEXPR(14)	void	deepRequestChange			  (		Control& control, const Request request, const Short = INVALID_SHORT)	noexcept;

	template <>
	HFSM2_CONSTEXPR(14)	void	deepRequestChange<Composite>  (		Control& control, const Request request, const Short)	noexcept	{ deepRequestChangeComposite  (control, request); }

	template <>
	HFSM2_CONSTEXPR(14)	void	deepRequestChange<Resumable>  (		Control& control, const Request request, const Short)	noexcept	{ deepRequestChangeResumable  (control, request); }

	template <>
	HFSM2_CONSTEXPR(14)	void	deepRequestChange<Selectable> (		Control& control, const Request request, const Short)	noexcept	{ deepRequestChangeSelectable (control, request); }

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	template <>
	HFSM2_CONSTEXPR(14)	void	deepRequestChange<Utilitarian>(		Control& control, const Request request, const Short)	noexcept	{ deepRequestChangeUtilitarian(control, request); }

	template <>
	HFSM2_CONSTEXPR(14)	void	deepRequestChange<RandomUtil> (		Control& control, const Request request, const Short)	noexcept	{ deepRequestChangeRandom	  (control, request); }
#endif

#else

	HFSM2_CONSTEXPR(14)	void	deepRequestChange			  (		Control& control, const Request request)	noexcept;

#endif

	HFSM2_CONSTEXPR(14)	void	deepRequestChangeComposite	  (		Control& control, const Request request)	noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestChangeResumable	  (		Control& control, const Request request)	noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestChangeSelectable	  (		Control& control, const Request request)	noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void	deepRequestChangeUtilitarian  (		Control& control, const Request request)	noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestChangeRandom		  (		Control& control, const Request request)	noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void	deepRequestRestart			  (		Control& control, const Request request)	noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestResume			  (		Control& control, const Request request)	noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestSelect			  (		Control& control, const Request request)	noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void	deepRequestUtilize			  (		Control& control, const Request request)	noexcept;
	HFSM2_CONSTEXPR(14)	void	deepRequestRandomize		  (		Control& control, const Request request)	noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()
#if HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()

	template <Strategy = STRATEGY>
	HFSM2_CONSTEXPR(14)	UP		deepReportChange			  (		Control& control)	noexcept;

	template <>
	HFSM2_CONSTEXPR(14)	UP		deepReportChange<Composite>   (		Control& control)	noexcept	{ return deepReportChangeComposite  (control); }

	template <>
	HFSM2_CONSTEXPR(14)	UP		deepReportChange<Resumable>   (		Control& control)	noexcept	{ return deepReportChangeResumable  (control); }

	template <>
	HFSM2_CONSTEXPR(14)	UP		deepReportChange<Selectable>  (		Control& control)	noexcept	{ return deepReportChangeSelectable (control); }

	template <>
	HFSM2_CONSTEXPR(14)	UP		deepReportChange<Utilitarian> (		Control& control)	noexcept	{ return deepReportChangeUtilitarian(control); }

	template <>
	HFSM2_CONSTEXPR(14)	UP		deepReportChange<RandomUtil>  (		Control& control)	noexcept	{ return deepReportChangeRandom		(control); }

#else

	HFSM2_CONSTEXPR(14)	UP		deepReportChange			  (		Control& control)	noexcept;

#endif

	HFSM2_CONSTEXPR(14)	UP		deepReportChangeComposite	  (		Control& control)	noexcept;
	HFSM2_CONSTEXPR(14)	UP		deepReportChangeResumable	  (		Control& control)	noexcept;
	HFSM2_CONSTEXPR(14)	UP		deepReportChangeSelectable	  (		Control& control)	noexcept;
	HFSM2_CONSTEXPR(14)	UP		deepReportChangeUtilitarian   (		Control& control)	noexcept;
	HFSM2_CONSTEXPR(14)	UP		deepReportChangeRandom		  (		Control& control)	noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	UP		deepReportUtilize			  (		Control& control)	noexcept;
	HFSM2_CONSTEXPR(14)	Rank	deepReportRank				  (		Control& control)	noexcept;
	HFSM2_CONSTEXPR(14)	Utility deepReportRandomize			  (		Control& control)	noexcept;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void	deepChangeToRequested		  ( PlanControl& control)	noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_CONSTEXPR(14)	void	deepSaveActive				  (const Registry& registry, WriteStream& stream)  const noexcept;
	HFSM2_CONSTEXPR(14)	void	deepSaveResumable			  (const Registry& registry, WriteStream& stream)  const noexcept;

	HFSM2_CONSTEXPR(14)	void	deepLoadRequested			  (		 Registry& registry, ReadStream&  stream)  const noexcept;
	HFSM2_CONSTEXPR(14)	void	deepLoadResumable			  (		 Registry& registry, ReadStream&  stream)  const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr Long NAME_COUNT = HeadState::NAME_COUNT + SubStates::NAME_COUNT;

	HFSM2_CONSTEXPR(14)	void deepGetNames(const Long parent,
										  const RegionType region,
										  const Short depth,
										  StructureStateInfos& stateInfos)  const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "composite.inl"
