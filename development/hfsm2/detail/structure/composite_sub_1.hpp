namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <
	typename TIndices
  , typename TArgs
  , Strategy TStrategy
  , Prong NProng
  , typename... TStates
>
struct HFSM2_EMPTY_BASES CS_<
							 TIndices
						   , TArgs
						   , TStrategy
						   , NProng
						   , TL_<TStates...>
						 >
	:					 LHalfCS<
							 TIndices
						   , TArgs
						   , TStrategy
						   , NProng
						   , TL_<TStates...>
						 >
	,					 RHalfCS<
							 TIndices
						   , TArgs
						   , TStrategy
						   , NProng
						   , TL_<TStates...>
						 >
{
	static_assert(sizeof...(TStates) >= 2, "");

	using Indices		= TIndices;
	static constexpr StateID  INITIAL_ID  = Indices::STATE_ID;
	static constexpr Short	  COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	  ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	  ORTHO_UNIT  = Indices::ORTHO_UNIT;

	static constexpr Strategy STRATEGY	  = TStrategy;

	static constexpr RegionID REGION_ID	  = COMPO_INDEX + ORTHO_INDEX;
	static constexpr Prong	  PRONG_INDEX = NProng;

	using Args			= TArgs;
	using SubStateList	= TL_<TStates...>;

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
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using LStateList	= LHalfTypes<TStates...>;
	using LHalfInfo		= CSI_<LStateList>;

	static constexpr Prong	  L_PRONG	  = PRONG_INDEX;
	static constexpr Prong	  R_PRONG	  = PRONG_INDEX + LStateList::SIZE;

	using LHalf			= LHalfCS<
							  Indices
							, Args
							, STRATEGY
							, PRONG_INDEX
							, SubStateList
						  >;

	using RHalf			= RHalfCS<
							  Indices
							, Args
							, STRATEGY
							, PRONG_INDEX
							, SubStateList
						  >;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void		wideRegister			   (Registry& registry, const Parent parent)								noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	bool		wideForwardEntryGuard	   (GuardControl& control,						  const Prong prong)		noexcept;
	HFSM2_CONSTEXPR(14)	bool		wideEntryGuard			   (GuardControl& control,						  const Prong prong)		noexcept;

	HFSM2_CONSTEXPR(14)	void		wideEnter				   ( PlanControl& control,						  const Prong prong)		noexcept;
	HFSM2_CONSTEXPR(14)	void		wideReenter				   ( PlanControl& control,						  const Prong prong)		noexcept;

	HFSM2_CONSTEXPR(14)	TaskStatus	widePreUpdate			   ( FullControl& control,						  const Prong prong)		noexcept;
	HFSM2_CONSTEXPR(14)	TaskStatus	wideUpdate				   ( FullControl& control,						  const Prong prong)		noexcept;
	HFSM2_CONSTEXPR(14)	TaskStatus	widePostUpdate			   ( FullControl& control,						  const Prong prong)		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	TaskStatus	widePreReact			   ( FullControl& control, const TEvent& event,	  const Prong prong)		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	TaskStatus	wideReact				   ( FullControl& control, const TEvent& event,	  const Prong prong)		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	TaskStatus	widePostReact			   ( FullControl& control, const TEvent& event,	  const Prong prong)		noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void		wideQuery				   (ConstControl& control,		 TEvent& event,	  const Prong prong)  const noexcept;

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14)	TaskStatus	wideUpdatePlans			   ( FullControl& control,						  const Prong prong)		noexcept;
#endif

	HFSM2_CONSTEXPR(14)	bool		wideForwardExitGuard	   (GuardControl& control,						  const Prong prong)		noexcept;
	HFSM2_CONSTEXPR(14)	bool		wideExitGuard			   (GuardControl& control,						  const Prong prong)		noexcept;

	HFSM2_CONSTEXPR(14)	void		wideExit				   ( PlanControl& control,						  const Prong prong)		noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void		wideForwardActive		   (	 Control& control, const Request request, const Prong prong)		noexcept;
	HFSM2_CONSTEXPR(14)	void		wideForwardRequest		   (	 Control& control, const Request request, const Prong prong)		noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void		wideRequestChangeComposite (	 Control& control, const Request request				   )		noexcept;
	HFSM2_CONSTEXPR(14)	void		wideRequestChangeResumable (	 Control& control, const Request request, const Prong prong)		noexcept;
	HFSM2_CONSTEXPR(14)	void		wideRequestChangeSelectable(	 Control& control, const Request request, const Prong prong)		noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void		wideRequestRestart		   (	 Control& control, const Request request				   )		noexcept;
	HFSM2_CONSTEXPR(14)	void		wideRequestResume		   (	 Control& control, const Request request, const Prong prong)		noexcept;
	HFSM2_CONSTEXPR(14)	void		wideRequestSelect		   (	 Control& control, const Request request, const Prong prong)		noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	UP			wideReportChangeComposite  (	 Control& control																  )		noexcept;
	HFSM2_CONSTEXPR(14)	UP			wideReportChangeResumable  (	 Control& control,						  const Prong prong						  )		noexcept;
	HFSM2_CONSTEXPR(14)	UP			wideReportChangeSelectable (	 Control& control,						  const Prong prong						  )		noexcept;
	HFSM2_CONSTEXPR(14)	UP			wideReportChangeUtilitarian(	 Control& control																  )		noexcept;
	HFSM2_CONSTEXPR(14)	Utility		wideReportChangeRandom	   (	 Control& control, Utility* const options, const Rank* const ranks, const Rank top)		noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	UP			wideReportUtilize		   (	 Control& control																  )		noexcept;
	HFSM2_CONSTEXPR(14)	Rank		wideReportRank			   (	 Control& control,								 Rank* const ranks				  )		noexcept;
	HFSM2_CONSTEXPR(14)	Utility		wideReportRandomize		   (	 Control& control, Utility* const options, const Rank* const ranks, const Rank top)		noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void		wideChangeToRequested	   ( PlanControl& control,						  const Prong prong)		noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_CONSTEXPR(14)	void		wideSaveActive			   (const Registry& registry, WriteStream& stream, const Prong prong)  const noexcept;
	HFSM2_CONSTEXPR(14)	void		wideSaveResumable		   (const Registry& registry, WriteStream& stream					)  const noexcept;

	HFSM2_CONSTEXPR(14)	void		wideLoadRequested		   (      Registry& registry,  ReadStream& stream, const Prong prong)  const noexcept;
	HFSM2_CONSTEXPR(14)	void		wideLoadResumable		   (      Registry& registry,  ReadStream& stream					)  const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename Args::StructureStateInfos;

	static constexpr Long NAME_COUNT = LHalf::NAME_COUNT + RHalf::NAME_COUNT;

	HFSM2_CONSTEXPR(14)	void wideGetNames(const Long parent,
										  const RegionType region,
										  const Short depth,
										  StructureStateInfos& stateInfos)	  const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "composite_sub_1.inl"
