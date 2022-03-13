namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename, typename, Short, typename...>
struct OS_;

//------------------------------------------------------------------------------

template <typename TIndices,
		  typename TArgs,
		  Short NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct HFSM2_EMPTY_BASES OS_<TIndices, TArgs, NIndex, TInitial, TRemaining...>
	: InitialOS<TIndices,
				TArgs,
				TInitial>
	, RemainingOS<TIndices,
				  TArgs,
				  NIndex,
				  TInitial,
				  TRemaining...>
{
	using Indices		= TIndices;
	static constexpr StateID INITIAL_ID	 = Indices::STATE_ID;
	static constexpr Short	 COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	 ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	 ORTHO_UNIT	 = Indices::ORTHO_UNIT;

	static constexpr Short	 PRONG_INDEX = NIndex;

	using Args			= TArgs;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;
	using OrthoForks	= typename Registry::OrthoForks;
	using ProngBits		= typename OrthoForks::Bits;
	using ProngCBits	= typename OrthoForks::CBits;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using InitialInfo	= WrapInfo<TInitial>;
	using InitialStates	= typename InitialInfo::StateList;

	using Initial		= InitialOS<TIndices,
									TArgs,
									TInitial>;

	using Remaining		= RemainingOS<TIndices,
									  TArgs,
									  NIndex,
									  TInitial,
									  TRemaining...>;

	//----------------------------------------------------------------------

	HFSM2_CONSTEXPR(14)	void	wideRegister		 (Registry& registry, const ForkID forkId)							  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	bool	wideForwardEntryGuard(GuardControl& control,				   const ProngCBits prongs)	  noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideForwardEntryGuard(GuardControl& control											  )	  noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideEntryGuard		 (GuardControl& control											  )	  noexcept;

	HFSM2_CONSTEXPR(14)	void	wideEnter			 (PlanControl&	control											  )	  noexcept;
	HFSM2_CONSTEXPR(14)	void	wideReenter			 (PlanControl&	control											  )	  noexcept;

	HFSM2_CONSTEXPR(14)	Status	wideUpdate			 (FullControl&	control											  )	  noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	wideReact			 (FullControl&	control, const TEvent& event					  )	  noexcept;

	HFSM2_CONSTEXPR(14)	bool	wideForwardExitGuard (GuardControl& control,				   const ProngCBits prongs)	  noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideForwardExitGuard (GuardControl& control											  )	  noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideExitGuard		 (GuardControl& control											  )	  noexcept;

	HFSM2_CONSTEXPR(14)	void	wideExit			 (PlanControl&	control											  )	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void	wideForwardActive	 (Control& control, const Request request, const ProngCBits prongs)	  noexcept;
	HFSM2_CONSTEXPR(14)	void	wideForwardRequest	 (Control& control, const Request request						  )	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void	wideRequestChange	 (Control& control, const Request request						  )	  noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestRestart	 (Control& control, const Request request						  )	  noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestResume	 (Control& control, const Request request						  )	  noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void	wideRequestUtilize	 (Control& control, const Request request						  )	  noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestRandomize (Control& control, const Request request						  )	  noexcept;

	HFSM2_CONSTEXPR(14)	Utility wideReportChange	 (Control& control												  )	  noexcept;
	HFSM2_CONSTEXPR(14)	Utility wideReportUtilize	 (Control& control												  )	  noexcept;
	HFSM2_CONSTEXPR(14)	Utility wideReportRandomize  (Control& control												  )	  noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void	wideChangeToRequested(PlanControl&  control											  )	  noexcept;

	//----------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_CONSTEXPR(14)	void	wideSaveActive		  (const Registry& registry, WriteStream& stream)	const noexcept;
	HFSM2_CONSTEXPR(14)	void	wideSaveResumable	  (const Registry& registry, WriteStream& stream)	const noexcept;

	HFSM2_CONSTEXPR(14)	void	wideLoadRequested	  (		 Registry& registry, ReadStream&  stream)	const noexcept;
	HFSM2_CONSTEXPR(14)	void	wideLoadResumable	  (		 Registry& registry, ReadStream&  stream)	const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename Args::StructureStateInfos;

	static constexpr Long NAME_COUNT	 = Initial::NAME_COUNT  + Remaining::NAME_COUNT;

	HFSM2_CONSTEXPR(14)	void wideGetNames(const Long parent,
										  const Short depth,
										  StructureStateInfos& stateInfos) const noexcept;
#endif

	//----------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  Short NIndex,
		  typename TInitial>
struct OS_<TIndices, TArgs, NIndex, TInitial>
	: InitialOS<TIndices,
				TArgs,
				TInitial>
{
	using Indices		= TIndices;
	static constexpr StateID INITIAL_ID	 = Indices::STATE_ID;
	static constexpr Short	 COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	 ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	 ORTHO_UNIT	 = Indices::ORTHO_UNIT;

	static constexpr Short	 PRONG_INDEX = NIndex;

	using Args			= TArgs;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;
	using OrthoForks	= typename Registry::OrthoForks;
	using ProngBits		= typename OrthoForks::Bits;
	using ProngCBits= typename OrthoForks::CBits;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Initial		= InitialOS<TIndices,
									TArgs,
									TInitial>;

	//----------------------------------------------------------------------

	HFSM2_CONSTEXPR(14)	void	wideRegister		 (Registry& registry, const ForkID forkId)							  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	bool	wideForwardEntryGuard(GuardControl& control,				   const ProngCBits prongs)	  noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideForwardEntryGuard(GuardControl& control											  )	  noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideEntryGuard		 (GuardControl& control											  )	  noexcept;

	HFSM2_CONSTEXPR(14)	void	wideEnter			 (PlanControl&	control											  )	  noexcept;
	HFSM2_CONSTEXPR(14)	void	wideReenter			 (PlanControl&	control											  )	  noexcept;

	HFSM2_CONSTEXPR(14)	Status	wideUpdate			 (FullControl&	control											  )	  noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	Status	wideReact			 (FullControl&	control, const TEvent& event					  )	  noexcept;

	HFSM2_CONSTEXPR(14)	bool	wideForwardExitGuard (GuardControl& control,				   const ProngCBits prongs)	  noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideForwardExitGuard (GuardControl& control											  )	  noexcept;
	HFSM2_CONSTEXPR(14)	bool	wideExitGuard		 (GuardControl& control											  )	  noexcept;

	HFSM2_CONSTEXPR(14)	void	wideExit			 (PlanControl&	control											  )	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void	wideForwardActive	 (Control& control, const Request request, const ProngCBits prongs)	  noexcept;
	HFSM2_CONSTEXPR(14)	void	wideForwardRequest	 (Control& control, const Request request						  )	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void	wideRequestChange	 (Control& control, const Request request						  )	  noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestRestart	 (Control& control, const Request request						  )	  noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestResume	 (Control& control, const Request request						  )	  noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void	wideRequestUtilize	 (Control& control, const Request request						  )	  noexcept;
	HFSM2_CONSTEXPR(14)	void	wideRequestRandomize (Control& control, const Request request						  )	  noexcept;

	HFSM2_CONSTEXPR(14)	Utility wideReportChange	 (Control& control												  )	  noexcept;
	HFSM2_CONSTEXPR(14)	Utility wideReportUtilize	 (Control& control												  )	  noexcept;
	HFSM2_CONSTEXPR(14)	Utility wideReportRandomize  (Control& control												  )	  noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void	wideChangeToRequested(PlanControl&  control											  )	  noexcept;

	//----------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_CONSTEXPR(14)	void	wideSaveActive		  (const Registry& registry, WriteStream& stream)	const noexcept;
	HFSM2_CONSTEXPR(14)	void	wideSaveResumable	  (const Registry& registry, WriteStream& stream)	const noexcept;

	HFSM2_CONSTEXPR(14)	void	wideLoadRequested	  (		 Registry& registry, ReadStream&  stream)	const noexcept;
	HFSM2_CONSTEXPR(14)	void	wideLoadResumable	  (		 Registry& registry, ReadStream&  stream)	const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename Args::StructureStateInfos;

	static constexpr Long NAME_COUNT	 = Initial::NAME_COUNT;

	HFSM2_CONSTEXPR(14)	void wideGetNames(const Long parent,
										  const Short depth,
										  StructureStateInfos& stateInfos) const;
#endif

	//----------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "orthogonal_sub_1.inl"
#include "orthogonal_sub_2.inl"
