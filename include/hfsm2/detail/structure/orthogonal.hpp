namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct O_ final {
	using Indices		= TIndices;
	static constexpr StateID HEAD_ID	 = Indices::STATE_ID;
	static constexpr Short	 COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	 ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	 ORTHO_UNIT	 = Indices::ORTHO_UNIT;

	static constexpr Short	 REGION_ID	 = COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID	 ORTHO_ID	 = (ForkID) -ORTHO_INDEX - 1;

	using Args			= TArgs;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using Head			= THead;

	using Info			= OI_<Head, TSubStates...>;
	static constexpr Short WIDTH		= Info::WIDTH;
	static constexpr Short REGION_SIZE	= Info::STATE_COUNT;
	static constexpr Short ORTHO_UNITS	= Info::ORTHO_UNITS;

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;
	using OrthoForks	= typename Registry::OrthoForks;
	using ProngBits		= typename OrthoForks::Bits;
	using ProngCBits	= typename OrthoForks::CBits;

	using Control		= ControlT<Args>;
	using ScopedOrigin	= typename Control::Origin;

	using PlanControl	= PlanControlT<Args>;
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

#if HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()

	template <typename T>
	struct Accessor {
		HFSM2_CONSTEXPR(11) static			T& get(		 O_& o)						  noexcept	{ return o._subStates.template access<T>();	}
		HFSM2_CONSTEXPR(11) static const	T& get(const O_& o)						  noexcept	{ return o._subStates.template access<T>();	}
	};

	template <>
	struct Accessor<Head> {
		HFSM2_CONSTEXPR(11) static		 Head& get(		 O_& o)						  noexcept	{ return o._headState;						}
		HFSM2_CONSTEXPR(11) static const Head& get(const O_& o)						  noexcept	{ return o._headState;						}
	};

	template <typename T>
	HFSM2_CONSTEXPR(14)		  T&	access()										  noexcept	{ return Accessor<T>::get(*this);			}

	template <typename T>
	HFSM2_CONSTEXPR(11) const T&	access()									const noexcept	{ return Accessor<T>::get(*this);			}

#endif

	//----------------------------------------------------------------------

	HFSM2_CONSTEXPR(11) static ProngBits  orthoRequested(	   Registry& registry)	  noexcept	{ return		  registry.orthoRequested.template  bits<ORTHO_UNIT, WIDTH>();	}
	HFSM2_CONSTEXPR(11) static ProngCBits orthoRequested(const Registry& registry)	  noexcept	{ return		  registry.orthoRequested.template cbits<ORTHO_UNIT, WIDTH>();	}

	HFSM2_CONSTEXPR(11) static ProngBits  orthoRequested(	   Control&  control)	  noexcept	{ return control._registry.orthoRequested.template  bits<ORTHO_UNIT, WIDTH>();	}
	HFSM2_CONSTEXPR(11) static ProngCBits orthoRequested(const Control&  control)	  noexcept	{ return control._registry.orthoRequested.template cbits<ORTHO_UNIT, WIDTH>();	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	deepRegister		 (Registry& registry,
													  const Parent parent)			  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) bool	deepForwardEntryGuard(GuardControl& control)		  noexcept;
	HFSM2_CONSTEXPR(14) bool	deepEntryGuard		 (GuardControl& control)		  noexcept;

	HFSM2_CONSTEXPR(14) void	deepEnter			 (PlanControl&	control)		  noexcept;
	HFSM2_CONSTEXPR(14) void	deepReenter			 (PlanControl&	control)		  noexcept;

	HFSM2_CONSTEXPR(14) Status	deepUpdate			 (FullControl&	control)		  noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14) Status	deepReact			 (FullControl&	control,
													  const TEvent& event)			  noexcept;

	HFSM2_CONSTEXPR(14) bool	deepForwardExitGuard (GuardControl& control)		  noexcept;
	HFSM2_CONSTEXPR(14) bool	deepExitGuard		 (GuardControl& control)		  noexcept;

	HFSM2_CONSTEXPR(14) void	deepExit			 (PlanControl&	control)		  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	deepForwardActive	 (Control& control, const Request request)	  noexcept;
	HFSM2_CONSTEXPR(14) void	deepForwardRequest	 (Control& control, const Request request)	  noexcept;

	HFSM2_CONSTEXPR(14) void	deepRequest			 (Control& control, const Request request)	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	deepRequestChange	 (Control& control, const Request request)	  noexcept;
	HFSM2_CONSTEXPR(14) void	deepRequestRestart	 (Control& control, const Request request)	  noexcept;
	HFSM2_CONSTEXPR(14) void	deepRequestResume	 (Control& control, const Request request)	  noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14) void	deepRequestUtilize	 (Control& control, const Request request)	  noexcept;
	HFSM2_CONSTEXPR(14) void	deepRequestRandomize (Control& control, const Request request)	  noexcept;

	HFSM2_CONSTEXPR(14) UP		deepReportChange	 (Control& control)				  noexcept;

	HFSM2_CONSTEXPR(14) UP		deepReportUtilize	 (Control& control)				  noexcept;
	HFSM2_CONSTEXPR(14) Rank	deepReportRank		 (Control& control)				  noexcept;
	HFSM2_CONSTEXPR(14) Utility deepReportRandomize  (Control& control)				  noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	deepChangeToRequested(PlanControl&  control)		  noexcept;

	//----------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_CONSTEXPR(14) void	deepSaveActive		 (const Registry& registry, WriteStream& stream)	const noexcept;
	HFSM2_CONSTEXPR(14) void	deepSaveResumable	 (const Registry& registry, WriteStream& stream)	const noexcept;

	HFSM2_CONSTEXPR(14) void	deepLoadRequested	 (		Registry& registry, ReadStream&  stream)	const noexcept;
	HFSM2_CONSTEXPR(14) void	deepLoadResumable	 (		Registry& registry, ReadStream&  stream)	const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr Long NAME_COUNT	 = HeadState::NAME_COUNT  + SubStates::NAME_COUNT;

	HFSM2_CONSTEXPR(14) void deepGetNames(const Long parent,
										  const RegionType region,
										  const Short depth,
										  StructureStateInfos& stateInfos) const noexcept;
#endif

	//----------------------------------------------------------------------

	HeadState _headState;
	SubStates _subStates;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "orthogonal.inl"
