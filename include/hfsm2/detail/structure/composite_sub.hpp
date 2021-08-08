﻿namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  Strategy TStrategy,
		  Short NIndex,
		  typename... TStates>
struct CS_ final {
	static_assert(sizeof...(TStates) >= 2, "");

	using Indices		= TIndices;
	static constexpr StateID  INITIAL_ID  = Indices::STATE_ID;
	static constexpr Short	  COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	  ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	  ORTHO_UNIT  = Indices::ORTHO_UNIT;

	static constexpr Strategy STRATEGY	  = TStrategy;

	static constexpr Short	  REGION_ID	  = COMPO_INDEX + ORTHO_INDEX;
	static constexpr Short	  PRONG_INDEX = NIndex;

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

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	static constexpr Short	  L_PRONG	  = PRONG_INDEX;

	using LStateList	= LHalfTypes<TStates...>;
	using LMaterial		= CSubMaterial<I_<INITIAL_ID,
										  COMPO_INDEX,
										  ORTHO_INDEX,
										  ORTHO_UNIT>,
									   Args,
									   STRATEGY,
									   L_PRONG,
									   LStateList>;

	using LHalfInfo		= Info<LMaterial>;

	static constexpr Short	  R_PRONG	  = PRONG_INDEX + LStateList::SIZE;

	using RStateList	= RHalfTypes<TStates...>;
	using RMaterial		= CSubMaterial<I_<INITIAL_ID  + LHalfInfo::STATE_COUNT,
										  COMPO_INDEX + LHalfInfo::COMPO_REGIONS,
										  ORTHO_INDEX + LHalfInfo::ORTHO_REGIONS,
										  ORTHO_UNIT  + LHalfInfo::ORTHO_UNITS>,
									   Args,
									   STRATEGY,
									   R_PRONG,
									   RStateList>;

	//----------------------------------------------------------------------

#if HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()

	template <typename T>
	HFSM2_CONSTEXPR(14)		  T& access()		  noexcept;

	template <typename T>
	HFSM2_CONSTEXPR(11) const T& access()	const noexcept;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	wideRegister			   (Registry& registry, const Parent parent)	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) bool	wideForwardEntryGuard	   (GuardControl& control, const Short prong)	  noexcept;
	HFSM2_CONSTEXPR(14) bool	wideEntryGuard			   (GuardControl& control, const Short prong)	  noexcept;

	HFSM2_CONSTEXPR(14) void	wideEnter				   (PlanControl&  control, const Short prong)	  noexcept;
	HFSM2_CONSTEXPR(14) void	wideReenter				   (PlanControl&  control, const Short prong)	  noexcept;

	HFSM2_CONSTEXPR(14) Status	wideUpdate				   (FullControl&  control, const Short prong)	  noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14) Status	wideReact				   (FullControl&  control,
															const TEvent& event,   const Short prong)	  noexcept;

	HFSM2_CONSTEXPR(14) bool	wideForwardExitGuard	   (GuardControl& control, const Short prong)	  noexcept;
	HFSM2_CONSTEXPR(14) bool	wideExitGuard			   (GuardControl& control, const Short prong)	  noexcept;

	HFSM2_CONSTEXPR(14) void	wideExit				   (PlanControl&  control, const Short prong)	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	wideForwardActive		   (Control& control, const Request request, const Short prong)	  noexcept;
	HFSM2_CONSTEXPR(14) void	wideForwardRequest		   (Control& control, const Request request, const Short prong)	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	wideRequestChangeComposite (Control& control, const Request request					  )	  noexcept;
	HFSM2_CONSTEXPR(14) void	wideRequestChangeResumable (Control& control, const Request request, const Short prong)	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	wideRequestRestart		   (Control& control, const Request request					  )	  noexcept;
	HFSM2_CONSTEXPR(14) void	wideRequestResume		   (Control& control, const Request request, const Short prong)	  noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14) UP		wideReportChangeComposite  (Control& control																 )	  noexcept;
	HFSM2_CONSTEXPR(14) UP		wideReportChangeResumable  (Control& control, const Short prong												 )	  noexcept;
	HFSM2_CONSTEXPR(14) UP		wideReportChangeUtilitarian(Control& control																 )	  noexcept;
	HFSM2_CONSTEXPR(14) Utility wideReportChangeRandom	   (Control& control, Utility* const options, const Rank* const ranks, const Rank top)	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) UP		wideReportUtilize		   (Control& control																 )	  noexcept;
	HFSM2_CONSTEXPR(14) Rank	wideReportRank			   (Control& control,								Rank* const ranks				 )	  noexcept;
	HFSM2_CONSTEXPR(14) Utility wideReportRandomize		   (Control& control, Utility* const options, const Rank* const ranks, const Rank top)	  noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	wideChangeToRequested	   (PlanControl& control, const Short prong) noexcept;

	//----------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_CONSTEXPR(14) void	wideSaveActive			   (const Registry& registry, WriteStream& stream, const Short prong)	const noexcept;
	HFSM2_CONSTEXPR(14) void	wideSaveResumable		   (const Registry& registry, WriteStream& stream	 				)	const noexcept;

	HFSM2_CONSTEXPR(14) void	wideLoadRequested		   (      Registry& registry, ReadStream&  stream, const Short prong)	const noexcept;
	HFSM2_CONSTEXPR(14) void	wideLoadResumable		   (      Registry& registry, ReadStream&  stream	 				)	const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr Long NAME_COUNT = LMaterial::NAME_COUNT + RMaterial::NAME_COUNT;

	HFSM2_CONSTEXPR(14)	void wideGetNames(const Long parent,
										  const RegionType region,
										  const Short depth,
										  StructureStateInfos& stateInfos) const noexcept;
#endif

	//----------------------------------------------------------------------

	LMaterial lHalf;
	RMaterial rHalf;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  Strategy TStrategy,
		  Short NIndex,
		  typename TState>
struct CS_<TIndices, TArgs, TStrategy, NIndex, TState> final {
	using Indices		= TIndices;
	static constexpr StateID INITIAL_ID	= Indices::STATE_ID;
	static constexpr Short COMPO_INDEX	= Indices::COMPO_INDEX;
	static constexpr Short ORTHO_INDEX	= Indices::ORTHO_INDEX;
	static constexpr Short ORTHO_UNIT	= Indices::ORTHO_UNIT;

	static constexpr Strategy	STRATEGY	= TStrategy;

	static constexpr Short REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr Short PRONG_INDEX	= NIndex;

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

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using State			= Material<I_<INITIAL_ID,
									  COMPO_INDEX,
									  ORTHO_INDEX,
									  ORTHO_UNIT>,
								   Args,
								   TState>;

	//----------------------------------------------------------------------

#if HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()

	template <typename T>
	HFSM2_CONSTEXPR(14)		  T& access()		  noexcept	 { return state.template access<T>();	}

	template <typename T>
	HFSM2_CONSTEXPR(11) const T& access()	const noexcept	 { return state.template access<T>();	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	wideRegister			   (Registry& registry, const Parent parent)	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) bool	wideForwardEntryGuard	   (GuardControl& control, const Short prong)	  noexcept;
	HFSM2_CONSTEXPR(14) bool	wideEntryGuard			   (GuardControl& control, const Short prong)	  noexcept;

	HFSM2_CONSTEXPR(14) void	wideEnter				   (PlanControl&  control, const Short prong)	  noexcept;
	HFSM2_CONSTEXPR(14) void	wideReenter				   (PlanControl&  control, const Short prong)	  noexcept;

	HFSM2_CONSTEXPR(14) Status	wideUpdate				   (FullControl&  control, const Short prong)	  noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14) Status	wideReact				   (FullControl&  control,
															const TEvent& event,   const Short prong)	  noexcept;

	HFSM2_CONSTEXPR(14) bool	wideForwardExitGuard	   (GuardControl& control, const Short prong)	  noexcept;
	HFSM2_CONSTEXPR(14) bool	wideExitGuard			   (GuardControl& control, const Short prong)	  noexcept;

	HFSM2_CONSTEXPR(14) void	wideExit				   (PlanControl&  control, const Short prong)	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	wideForwardActive		   (Control& control, const Request request, const Short prong)	  noexcept;
	HFSM2_CONSTEXPR(14) void	wideForwardRequest		   (Control& control, const Request request, const Short prong)	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	wideRequestChangeComposite (Control& control, const Request request					  )	  noexcept;
	HFSM2_CONSTEXPR(14) void	wideRequestChangeResumable (Control& control, const Request request, const Short prong)	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	wideRequestRestart		   (Control& control, const Request request					  )	  noexcept;
	HFSM2_CONSTEXPR(14) void	wideRequestResume		   (Control& control, const Request request, const Short prong)	  noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14) UP		wideReportChangeComposite  (Control& control																 )	  noexcept;
	HFSM2_CONSTEXPR(14) UP		wideReportChangeResumable  (Control& control, const Short prong												 )	  noexcept;
	HFSM2_CONSTEXPR(14) UP		wideReportChangeUtilitarian(Control& control																 )	  noexcept;
	HFSM2_CONSTEXPR(14) Utility wideReportChangeRandom	   (Control& control, Utility* const options, const Rank* const ranks, const Rank top)	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) UP		wideReportUtilize		   (Control& control																 )	  noexcept;
	HFSM2_CONSTEXPR(14) Rank	wideReportRank			   (Control& control,								Rank* const ranks				 )	  noexcept;
	HFSM2_CONSTEXPR(14) Utility wideReportRandomize		   (Control& control, Utility* const options, const Rank* const ranks, const Rank top)	  noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	wideChangeToRequested	   (PlanControl& control, const Short prong) noexcept;

	//----------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_CONSTEXPR(14) void	wideSaveActive			   (const Registry& registry, WriteStream& stream, const Short prong)	const noexcept;
	HFSM2_CONSTEXPR(14) void	wideSaveResumable		   (const Registry& registry, WriteStream& stream	 				)	const noexcept;

	HFSM2_CONSTEXPR(14) void	wideLoadRequested		   (      Registry& registry, ReadStream&  stream, const Short prong)	const noexcept;
	HFSM2_CONSTEXPR(14) void	wideLoadResumable		   (      Registry& registry, ReadStream&  stream	 				)	const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr Long NAME_COUNT = State::NAME_COUNT;

	HFSM2_CONSTEXPR(14)	void wideGetNames(const Long parent,
										  const RegionType region,
										  const Short depth,
										  StructureStateInfos& stateInfos) const noexcept;
#endif

	//----------------------------------------------------------------------

	State state;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "composite_sub_1.inl"
#include "composite_sub_2.inl"
