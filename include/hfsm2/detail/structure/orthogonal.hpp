namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct O_ final {
	using Indices		= TIndices;
	static constexpr StateID	HEAD_ID		= Indices::STATE_ID;
	static constexpr ShortIndex COMPO_INDEX	= Indices::COMPO_INDEX;
	static constexpr ShortIndex ORTHO_INDEX	= Indices::ORTHO_INDEX;
	static constexpr ShortIndex ORTHO_UNIT	= Indices::ORTHO_UNIT;

	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID		ORTHO_ID	= (ForkID) -ORTHO_INDEX - 1;

	using Args			= TArgs;

#ifdef HFSM_ENABLE_UTILITY_THEORY
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using Head			= THead;

	using Info			= OI_<Head, TSubStates...>;
	static constexpr ShortIndex WIDTH		= Info::WIDTH;
	static constexpr ShortIndex REGION_SIZE	= Info::STATE_COUNT;
	static constexpr ShortIndex ORTHO_UNITS	= Info::ORTHO_UNITS;

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;
	using OrthoForks	= typename Registry::OrthoForks;
	using ProngBits		= typename OrthoForks::Bits;
	using ProngConstBits= typename OrthoForks::ConstBits;

	using Control		= ControlT<Args>;

	using PlanControl	= PlanControlT<Args>;
	using ScopedOrigin	= typename PlanControl::Origin;
	using ScopedRegion	= typename PlanControl::Region;

	using FullControl	= FullControlT<Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;

	using HeadState		= S_<Indices, Args, Head>;

	using SubStates		= OS_<I_<HEAD_ID + 1,
								 COMPO_INDEX,
								 ORTHO_INDEX + 1,
								 ORTHO_UNIT + (WIDTH + 7) / 8>,
							  Args,
							  0,
							  TSubStates...>;

	//----------------------------------------------------------------------

#ifdef HFSM_EXPLICIT_MEMBER_SPECIALIZATION
	template <typename T>
	struct Accessor {
		HFSM_INLINE static		 T&	   get(		 O_& o)							{ return o._subStates.template access<T>();	}
		HFSM_INLINE static const T&	   get(const O_& o)							{ return o._subStates.template access<T>();	}
	};

	template <>
	struct Accessor<Head> {
		HFSM_INLINE static		 Head& get(		 O_& o)							{ return o._headState._headBox.get();		}
		HFSM_INLINE static const Head& get(const O_& o)							{ return o._headState._headBox.get();		}
	};

	template <typename T>
	HFSM_INLINE		  T&	access()											{ return Accessor<T>::get(*this);			}

	template <typename T>
	HFSM_INLINE const T&	access() const										{ return Accessor<T>::get(*this);			}
#endif

	//----------------------------------------------------------------------

	HFSM_INLINE ProngBits	   orthoRequested(		Registry& registry)			{ return registry.orthoRequested.template bits<ORTHO_UNIT, WIDTH>();	}
	HFSM_INLINE ProngConstBits orthoRequested(const Registry& registry) const	{ return registry.orthoRequested.template bits<ORTHO_UNIT, WIDTH>();	}

	HFSM_INLINE ProngBits	   orthoRequested(		Control& control)			{ return orthoRequested(control._registry);								}
	HFSM_INLINE ProngConstBits orthoRequested(const Control& control) const		{ return orthoRequested(control._registry);								}

	HFSM_INLINE void	deepRegister		 (Registry& registry, const Parent parent);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool	deepForwardEntryGuard(GuardControl&	control);
	HFSM_INLINE bool	deepEntryGuard		 (GuardControl&	control);

	HFSM_INLINE void	deepConstruct		 (PlanControl&	control);

	HFSM_INLINE void	deepEnter			 (PlanControl&	control);
	HFSM_INLINE void	deepReenter			 (PlanControl&	control);

	HFSM_INLINE Status	deepUpdate			 (FullControl&	control);

	template <typename TEvent>
	HFSM_INLINE Status	deepReact			 (FullControl&	control, const TEvent& event);

	HFSM_INLINE bool	deepForwardExitGuard (GuardControl&	control);
	HFSM_INLINE bool	deepExitGuard		 (GuardControl&	control);

	HFSM_INLINE void	deepExit			 (PlanControl&	control);

	HFSM_INLINE void	deepDestruct		 (PlanControl&  control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepForwardActive	 (Control& control, const Request::Type request);
	HFSM_INLINE void	deepForwardRequest	 (Control& control, const Request::Type request);

	HFSM_INLINE void	deepRequest			 (Control& control, const Request::Type request);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepRequestChange	 (Control& control);
	HFSM_INLINE void	deepRequestRemain	 (Registry& registry);
	HFSM_INLINE void	deepRequestRestart	 (Registry& registry);
	HFSM_INLINE void	deepRequestResume	 (Registry& registry);

#ifdef HFSM_ENABLE_UTILITY_THEORY
	HFSM_INLINE void	deepRequestUtilize	 (Control& control);
	HFSM_INLINE void	deepRequestRandomize (Control& control);

	HFSM_INLINE UP		deepReportChange	 (Control& control);

	HFSM_INLINE UP		deepReportUtilize	 (Control& control);
	HFSM_INLINE Rank	deepReportRank		 (Control& control);
	HFSM_INLINE Utility	deepReportRandomize	 (Control& control);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepChangeToRequested(PlanControl& control);

	//----------------------------------------------------------------------

#ifdef HFSM_ENABLE_SERIALIZATION
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM_INLINE void	deepSaveActive	 (const Registry& registry, WriteStream& stream) const;
	HFSM_INLINE void	deepSaveResumable(const Registry& registry, WriteStream& stream) const;

	HFSM_INLINE void	deepLoadRequested(		Registry& registry, ReadStream&  stream) const;
	HFSM_INLINE void	deepLoadResumable(		Registry& registry, ReadStream&  stream) const;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT	 = HeadState::NAME_COUNT  + SubStates::NAME_COUNT;

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	//----------------------------------------------------------------------

	HeadState _headState;
	SubStates _subStates;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "orthogonal.inl"
