namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  Strategy TStrategy,
		  typename THead,
		  typename... TSubStates>
struct C_ final {
	using Indices		= TIndices;
	static constexpr StateID  HEAD_ID	  = Indices::STATE_ID;
	static constexpr Short	  COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	  ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	  ORTHO_UNIT  = Indices::ORTHO_UNIT;

	static constexpr Strategy STRATEGY	  = TStrategy;

	static constexpr Short	  REGION_ID	  = COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID	  COMPO_ID	  = COMPO_INDEX + 1;

	using Args			= TArgs;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;

	using Control		= ControlT<Args>;
	using ScopedOrigin	= typename Control::Origin;

	using PlanControl	= PlanControlT<Args>;
	using ScopedRegion	= typename PlanControl::Region;

	using FullControl	= FullControlT<Args>;
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
							  TSubStates...>;

	using Info			= CI_<STRATEGY, Head, TSubStates...>;
	static constexpr Short WIDTH		  = Info::WIDTH;
	static constexpr Short WIDTH_BITS	  = Info::WIDTH_BITS;
	static constexpr Short REGION_SIZE	  = Info::STATE_COUNT;

	//----------------------------------------------------------------------

#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION
	template <typename T>
	struct Accessor {
		HFSM2_INLINE static		  T&	get(	  C_& c) noexcept				{ return c._subStates.template access<T>();		}
		HFSM2_INLINE static const T&	get(const C_& c) noexcept				{ return c._subStates.template access<T>();		}
	};

	template <>
	struct Accessor<Head> {
		HFSM2_INLINE static		  Head& get(	  C_& c) noexcept				{ return c._headState._headBox.get();			}
		HFSM2_INLINE static const Head& get(const C_& c) noexcept				{ return c._headState._headBox.get();			}
	};

	template <typename T>
	HFSM2_INLINE	   T&	access()	   noexcept								{ return Accessor<T>::get(*this);				}

	template <typename T>
	HFSM2_INLINE const T&	access() const noexcept								{ return Accessor<T>::get(*this);				}
#endif

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION
	HFSM2_INLINE Short	 compoRequested				   (const Registry& registry) const noexcept	{ return registry.compoRequested[COMPO_INDEX];	}
	HFSM2_INLINE Short&	 compoRequested				   (	  Registry& registry) const noexcept	{ return registry.compoRequested[COMPO_INDEX];	}

	HFSM2_INLINE Short	 compoActive				   (const Registry& registry) const noexcept	{ return registry.compoActive	[COMPO_INDEX];	}
	HFSM2_INLINE Short&	 compoActive				   (	  Registry& registry) const noexcept	{ return registry.compoActive	[COMPO_INDEX];	}

	HFSM2_INLINE Short	 compoResumable				   (const Registry& registry) const noexcept	{ return registry.compoResumable[COMPO_INDEX];	}
	HFSM2_INLINE Short&	 compoResumable				   (	  Registry& registry) const noexcept	{ return registry.compoResumable[COMPO_INDEX];	}
#endif

	HFSM2_INLINE Short&	 compoRequested				   (Control& control) const noexcept	{ return control._registry.compoRequested[COMPO_INDEX];	}
	HFSM2_INLINE Short&	 compoActive				   (Control& control) const noexcept	{ return control._registry.compoActive	 [COMPO_INDEX];	}
	HFSM2_INLINE Short&	 compoResumable				   (Control& control) const noexcept	{ return control._registry.compoResumable[COMPO_INDEX];	}

	HFSM2_INLINE bool	 compoRemain				   (Control& control) const noexcept	{ return control._registry.compoRemains.template get<COMPO_INDEX>(); }

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE Short	 resolveRandom				   (Control& control,
													    const Utility(& options)[Info::WIDTH], const Utility sum,
													    const Rank	 (& ranks)  [Info::WIDTH], const Rank	 top) const noexcept;
#endif

	HFSM2_INLINE void	 deepRegister				   (Registry& registry, const Parent parent) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE bool	 deepForwardEntryGuard		   (GuardControl& control) noexcept;
	HFSM2_INLINE bool	 deepEntryGuard				   (GuardControl& control) noexcept;

	HFSM2_INLINE void	 deepConstruct				   (PlanControl&  control) noexcept;

	HFSM2_INLINE void	 deepEnter					   (PlanControl&  control) noexcept;
	HFSM2_INLINE void	 deepReenter				   (PlanControl&  control) noexcept;

	HFSM2_INLINE Status	 deepUpdate					   (FullControl&  control) noexcept;

	template <typename TEvent>
	HFSM2_INLINE Status	 deepReact					   (FullControl&  control,
														const TEvent& event)   noexcept;

	HFSM2_INLINE bool	 deepForwardExitGuard		   (GuardControl& control) noexcept;
	HFSM2_INLINE bool	 deepExitGuard				   (GuardControl& control) noexcept;

	HFSM2_INLINE void	 deepExit					   (PlanControl&  control) noexcept;

	HFSM2_INLINE void	 deepDestruct				   (PlanControl&  control) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 deepForwardActive			   (Control& control, const Request request) noexcept;
	HFSM2_INLINE void	 deepForwardRequest			   (Control& control, const Request request) noexcept;

	HFSM2_INLINE void	 deepRequest				   (Control& control, const Request request) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION

	template <Strategy = STRATEGY>
	HFSM2_INLINE void	 deepRequestChange			   (Control& control, const Request request, const Short = INVALID_SHORT) noexcept;

	template <>
	HFSM2_INLINE void	 deepRequestChange<Composite>  (Control& control, const Request request, const Short) noexcept	{ deepRequestChangeComposite  (control, request); }

	template <>
	HFSM2_INLINE void	 deepRequestChange<Resumable>  (Control& control, const Request request, const Short) noexcept	{ deepRequestChangeResumable  (control, request); }

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	template <>
	HFSM2_INLINE void	 deepRequestChange<Utilitarian>(Control& control, const Request request, const Short) noexcept	{ deepRequestChangeUtilitarian(control, request); }

	template <>
	HFSM2_INLINE void	 deepRequestChange<RandomUtil> (Control& control, const Request request, const Short) noexcept	{ deepRequestChangeRandom	  (control, request); }

#endif

#else

	HFSM2_INLINE void	 deepRequestChange			   (Control& control, const Request request) noexcept;

#endif

	HFSM2_INLINE void	 deepRequestChangeComposite	   (Control& control, const Request request) noexcept;
	HFSM2_INLINE void	 deepRequestChangeResumable	   (Control& control, const Request request) noexcept;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void	 deepRequestChangeUtilitarian  (Control& control, const Request request) noexcept;
	HFSM2_INLINE void	 deepRequestChangeRandom	   (Control& control, const Request request) noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 deepRequestRestart			   (Control& control, const Request request) noexcept;
	HFSM2_INLINE void	 deepRequestResume			   (Control& control, const Request request) noexcept;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void	 deepRequestUtilize			   (Control& control, const Request request) noexcept;
	HFSM2_INLINE void	 deepRequestRandomize		   (Control& control, const Request request) noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY
#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION

	template <Strategy = STRATEGY>
	HFSM2_INLINE UP		 deepReportChange			   (Control& control) noexcept;

	template <>
	HFSM2_INLINE UP		 deepReportChange<Composite>   (Control& control) noexcept	{ return deepReportChangeComposite  (control); }

	template <>
	HFSM2_INLINE UP		 deepReportChange<Resumable>   (Control& control) noexcept	{ return deepReportChangeResumable  (control); }

	template <>
	HFSM2_INLINE UP		 deepReportChange<Utilitarian> (Control& control) noexcept	{ return deepReportChangeUtilitarian(control); }

	template <>
	HFSM2_INLINE UP		 deepReportChange<RandomUtil>  (Control& control) noexcept	{ return deepReportChangeRandom		(control); }

#else

	HFSM2_INLINE UP		 deepReportChange			   (Control& control) noexcept;

#endif

	HFSM2_INLINE UP		 deepReportChangeComposite	   (Control& control) noexcept;
	HFSM2_INLINE UP		 deepReportChangeResumable	   (Control& control) noexcept;
	HFSM2_INLINE UP		 deepReportChangeUtilitarian   (Control& control) noexcept;
	HFSM2_INLINE UP		 deepReportChangeRandom		   (Control& control) noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE UP		 deepReportUtilize			   (Control& control) noexcept;
	HFSM2_INLINE Rank	 deepReportRank				   (Control& control) noexcept;
	HFSM2_INLINE Utility deepReportRandomize		   (Control& control) noexcept;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 deepChangeToRequested		   (PlanControl& control) noexcept;

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_INLINE void	 deepSaveActive				   (const Registry& registry, WriteStream& stream) const noexcept;
	HFSM2_INLINE void	 deepSaveResumable			   (const Registry& registry, WriteStream& stream) const noexcept;

	HFSM2_INLINE void	 deepLoadRequested			   (	  Registry& registry, ReadStream&  stream) const noexcept;
	HFSM2_INLINE void	 deepLoadResumable			   (	  Registry& registry, ReadStream&  stream) const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr Long NAME_COUNT = HeadState::NAME_COUNT + SubStates::NAME_COUNT;

	void deepGetNames(const Long parent,
					  const RegionType region,
					  const Short depth,
					  StructureStateInfos& stateInfos) const noexcept;
#endif

	//----------------------------------------------------------------------

	HeadState _headState;
	SubStates _subStates;

	HFSM2_IF_DEBUG(static constexpr Info _info = Info{});
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "composite.inl"
