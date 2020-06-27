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
	static constexpr StateID	HEAD_ID		= Indices::STATE_ID;
	static constexpr ShortIndex COMPO_INDEX	= Indices::COMPO_INDEX;
	static constexpr ShortIndex ORTHO_INDEX	= Indices::ORTHO_INDEX;
	static constexpr ShortIndex ORTHO_UNIT	= Indices::ORTHO_UNIT;

	static constexpr Strategy	STRATEGY	= TStrategy;

	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID		COMPO_ID	= COMPO_INDEX + 1;

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

	using PlanControl	= PlanControlT<Args>;
	using ScopedOrigin	= typename PlanControl::Origin;
	using ScopedRegion	= typename PlanControl::Region;

#ifdef HFSM2_ENABLE_PLANS
	using Plan			= PlanT<Args>;
#endif

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
	static constexpr ShortIndex WIDTH		= Info::WIDTH;
	static constexpr ShortIndex WIDTH_BITS	= Info::WIDTH_BITS;
	static constexpr ShortIndex STATE_COUNT	= Info::STATE_COUNT;

	//----------------------------------------------------------------------

#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION
	template <typename T>
	struct Accessor {
		HFSM2_INLINE static		  T&	get(	  C_& c)							{ return c._subStates.template access<T>();		}
		HFSM2_INLINE static const T&	get(const C_& c)							{ return c._subStates.template access<T>();		}
	};

	template <>
	struct Accessor<Head> {
		HFSM2_INLINE static		  Head& get(	  C_& c)							{ return c._headState._headBox.get();			}
		HFSM2_INLINE static const Head& get(const C_& c)							{ return c._headState._headBox.get();			}
	};

	template <typename T>
	HFSM2_INLINE	   T&	access()												{ return Accessor<T>::get(*this);				}

	template <typename T>
	HFSM2_INLINE const T&	access() const											{ return Accessor<T>::get(*this);				}
#endif

	//----------------------------------------------------------------------

	HFSM2_INLINE	   ShortIndex& compoRequested(		Registry& registry) const	{ return registry.compoRequested[COMPO_INDEX];	}
	HFSM2_INLINE const ShortIndex& compoRequested(const Registry& registry) const	{ return registry.compoRequested[COMPO_INDEX];	}

	HFSM2_INLINE	   ShortIndex& compoActive   (		Registry& registry) const	{ return registry.compoActive	[COMPO_INDEX];	}
	HFSM2_INLINE const ShortIndex& compoActive   (const Registry& registry) const	{ return registry.compoActive	[COMPO_INDEX];	}

	HFSM2_INLINE	   ShortIndex& compoResumable(		Registry& registry) const	{ return registry.compoResumable[COMPO_INDEX];	}
	HFSM2_INLINE const ShortIndex& compoResumable(const Registry& registry) const	{ return registry.compoResumable[COMPO_INDEX];	}

	HFSM2_INLINE	   ShortIndex& compoRequested(		Control&  control) const	{ return compoRequested(control._registry);		}
	HFSM2_INLINE const ShortIndex& compoRequested(const Control&  control) const	{ return compoRequested(control._registry);		}

	HFSM2_INLINE	   ShortIndex& compoActive   (		Control&  control) const	{ return compoActive   (control._registry);		}
	HFSM2_INLINE const ShortIndex& compoActive   (const Control&  control) const	{ return compoActive   (control._registry);		}

	HFSM2_INLINE	   ShortIndex& compoResumable(		Control&  control) const	{ return compoResumable(control._registry);		}
	HFSM2_INLINE const ShortIndex& compoResumable(const Control&  control) const	{ return compoResumable(control._registry);		}

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE		  ShortIndex  resolveRandom	   (Control&  control,
														const Utility(& options)[Info::WIDTH], const Utility sum,
														const Rank	  (& ranks)  [Info::WIDTH], const Rank	  top) const;
#endif

	HFSM2_INLINE bool	 compoRemain				   (Control&  control)	{ return control._registry.compoRemains.template get<COMPO_INDEX>(); }

	HFSM2_INLINE void	 deepRegister				   (Registry& registry, const Parent parent);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE bool	 deepForwardEntryGuard		   (GuardControl& control);
	HFSM2_INLINE bool	 deepEntryGuard				   (GuardControl& control);

	HFSM2_INLINE void	 deepConstruct				   (PlanControl&  control);

	HFSM2_INLINE void	 deepEnter					   (PlanControl&  control);
	HFSM2_INLINE void	 deepReenter				   (PlanControl&  control);

	HFSM2_INLINE Status	 deepUpdate					   (FullControl&  control);

	template <typename TEvent>
	HFSM2_INLINE Status	 deepReact					   (FullControl&  control, const TEvent& event);

	HFSM2_INLINE bool	 deepForwardExitGuard		   (GuardControl& control);
	HFSM2_INLINE bool	 deepExitGuard				   (GuardControl& control);

	HFSM2_INLINE void	 deepExit					   (PlanControl&  control);

	HFSM2_INLINE void	 deepDestruct				   (PlanControl&  control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 deepForwardActive			   (Control& control, const Request::Type request);
	HFSM2_INLINE void	 deepForwardRequest			   (Control& control, const Request::Type request);

	HFSM2_INLINE void	 deepRequest				   (Control& control, const Request::Type request);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION

	template <Strategy = STRATEGY>
	HFSM2_INLINE void	 deepRequestChange			   (Control& control, const ShortIndex = INVALID_SHORT_INDEX);

	template <>
	HFSM2_INLINE void	 deepRequestChange<Composite>  (Control& control, const ShortIndex)	{ deepRequestChangeComposite  (control); }

	template <>
	HFSM2_INLINE void	 deepRequestChange<Resumable>  (Control& control, const ShortIndex)	{ deepRequestChangeResumable  (control); }

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	template <>
	HFSM2_INLINE void	 deepRequestChange<Utilitarian>(Control& control, const ShortIndex)	{ deepRequestChangeUtilitarian(control); }

	template <>
	HFSM2_INLINE void	 deepRequestChange<RandomUtil> (Control& control, const ShortIndex)	{ deepRequestChangeRandom	  (control); }

#endif

#else

	HFSM2_INLINE void	 deepRequestChange			   (Control& control);

#endif

	HFSM2_INLINE void	 deepRequestChangeComposite	   (Control& control);
	HFSM2_INLINE void	 deepRequestChangeResumable	   (Control& control);

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void	 deepRequestChangeUtilitarian  (Control& control);
	HFSM2_INLINE void	 deepRequestChangeRandom	   (Control& control);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 deepRequestRemain			   (Registry& registry);
	HFSM2_INLINE void	 deepRequestRestart			   (Registry& registry);
	HFSM2_INLINE void	 deepRequestResume			   (Registry& registry);

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void	 deepRequestUtilize			   (Control&  control);
	HFSM2_INLINE void	 deepRequestRandomize		   (Control&  control);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY
#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION

	template <Strategy = STRATEGY>
	HFSM2_INLINE UP		 deepReportChange			   (Control& control);

	template <>
	HFSM2_INLINE UP		 deepReportChange<Composite>   (Control& control)	{ return deepReportChangeComposite  (control); }

	template <>
	HFSM2_INLINE UP		 deepReportChange<Resumable>   (Control& control)	{ return deepReportChangeResumable  (control); }

	template <>
	HFSM2_INLINE UP		 deepReportChange<Utilitarian> (Control& control)	{ return deepReportChangeUtilitarian(control); }

	template <>
	HFSM2_INLINE UP		 deepReportChange<RandomUtil>  (Control& control)	{ return deepReportChangeRandom		(control); }

#else

	HFSM2_INLINE UP		 deepReportChange			   (Control& control);

#endif

	HFSM2_INLINE UP		 deepReportChangeComposite	   (Control& control);
	HFSM2_INLINE UP		 deepReportChangeResumable	   (Control& control);
	HFSM2_INLINE UP		 deepReportChangeUtilitarian   (Control& control);
	HFSM2_INLINE UP		 deepReportChangeRandom		   (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE UP		 deepReportUtilize			   (Control& control);
	HFSM2_INLINE Rank	 deepReportRank				   (Control& control);
	HFSM2_INLINE Utility deepReportRandomize		   (Control& control);

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 deepChangeToRequested		   (PlanControl& control);

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_INLINE void	 deepSaveActive				   (const Registry& registry, WriteStream& stream) const;
	HFSM2_INLINE void	 deepSaveResumable			   (const Registry& registry, WriteStream& stream) const;

	HFSM2_INLINE void	 deepLoadRequested			   (	  Registry& registry, ReadStream&  stream) const;
	HFSM2_INLINE void	 deepLoadResumable			   (	  Registry& registry, ReadStream&  stream) const;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT = HeadState::NAME_COUNT + SubStates::NAME_COUNT;

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
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
