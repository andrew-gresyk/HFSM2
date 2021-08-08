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
struct OS_<TIndices, TArgs, NIndex, TInitial, TRemaining...> final {
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

	using Initial		= Material<I_<INITIAL_ID,
									  COMPO_INDEX,
									  ORTHO_INDEX,
									  ORTHO_UNIT>,
								   Args,
								   TInitial>;

	using InitialInfo	= WrapInfo<TInitial>;
	using InitialStates	= typename InitialInfo::StateList;

	using Remaining		= OS_<I_<INITIAL_ID  + InitialInfo::STATE_COUNT,
								 COMPO_INDEX + InitialInfo::COMPO_REGIONS,
								 ORTHO_INDEX + InitialInfo::ORTHO_REGIONS,
								 ORTHO_UNIT  + InitialInfo::ORTHO_UNITS>,
							  Args,
							  PRONG_INDEX + 1,
							  TRemaining...>;

	using Info	= OSI_<TInitial, TRemaining...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()

	template <typename T>
	HFSM2_CONSTEXPR(14)		  T& access()		  noexcept;

	template <typename T>
	HFSM2_CONSTEXPR(11) const T& access()	const noexcept;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	wideRegister		 (Registry& registry, const ForkID forkId)							  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) bool	wideForwardEntryGuard(GuardControl& control,				   const ProngCBits prongs)	  noexcept;
	HFSM2_CONSTEXPR(14) bool	wideForwardEntryGuard(GuardControl& control											  )	  noexcept;
	HFSM2_CONSTEXPR(14) bool	wideEntryGuard		 (GuardControl& control											  )	  noexcept;

	HFSM2_CONSTEXPR(14) void	wideEnter			 (PlanControl&	control											  )	  noexcept;
	HFSM2_CONSTEXPR(14) void	wideReenter			 (PlanControl&	control											  )	  noexcept;

	HFSM2_CONSTEXPR(14) Status	wideUpdate			 (FullControl&	control											  )	  noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14) Status	wideReact			 (FullControl&	control, const TEvent& event					  )	  noexcept;

	HFSM2_CONSTEXPR(14) bool	wideForwardExitGuard (GuardControl& control,				   const ProngCBits prongs)	  noexcept;
	HFSM2_CONSTEXPR(14) bool	wideForwardExitGuard (GuardControl& control											  )	  noexcept;
	HFSM2_CONSTEXPR(14) bool	wideExitGuard		 (GuardControl& control											  )	  noexcept;

	HFSM2_CONSTEXPR(14) void	wideExit			 (PlanControl&	control											  )	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	wideForwardActive	 (Control& control, const Request request, const ProngCBits prongs)	  noexcept;
	HFSM2_CONSTEXPR(14) void	wideForwardRequest	 (Control& control, const Request request						  )	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	wideRequestChange	 (Control& control, const Request request						  )	  noexcept;
	HFSM2_CONSTEXPR(14) void	wideRequestRestart	 (Control& control, const Request request						  )	  noexcept;
	HFSM2_CONSTEXPR(14) void	wideRequestResume	 (Control& control, const Request request						  )	  noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14) void	wideRequestUtilize	 (Control& control, const Request request						  )	  noexcept;
	HFSM2_CONSTEXPR(14) void	wideRequestRandomize (Control& control, const Request request						  )	  noexcept;

	HFSM2_CONSTEXPR(14) Utility wideReportChange	 (Control& control												  )	  noexcept;
	HFSM2_CONSTEXPR(14) Utility wideReportUtilize	 (Control& control												  )	  noexcept;
	HFSM2_CONSTEXPR(14) Utility wideReportRandomize  (Control& control												  )	  noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	wideChangeToRequested(PlanControl&  control											  )	  noexcept;

	//----------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_CONSTEXPR(14) void	wideSaveActive		  (const Registry& registry, WriteStream& stream)	const noexcept;
	HFSM2_CONSTEXPR(14) void	wideSaveResumable	  (const Registry& registry, WriteStream& stream)	const noexcept;

	HFSM2_CONSTEXPR(14) void	wideLoadRequested	  (		 Registry& registry, ReadStream&  stream)	const noexcept;
	HFSM2_CONSTEXPR(14) void	wideLoadResumable	  (		 Registry& registry, ReadStream&  stream)	const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename Args::StructureStateInfos;

	static constexpr Long NAME_COUNT	 = Initial::NAME_COUNT  + Remaining::NAME_COUNT;

	HFSM2_CONSTEXPR(14) void wideGetNames(const Long parent,
										  const Short depth,
										  StructureStateInfos& stateInfos) const noexcept;
#endif

	//----------------------------------------------------------------------

	Initial initial;
	Remaining remaining;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  Short NIndex,
		  typename TInitial>
struct OS_<TIndices, TArgs, NIndex, TInitial> final {
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

	using Initial		= Material<I_<INITIAL_ID,
									  COMPO_INDEX,
									  ORTHO_INDEX,
									  ORTHO_UNIT>,
								   Args,
								   TInitial>;

	using Info	= OSI_<TInitial>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()

	template <typename T>
	HFSM2_CONSTEXPR(14)		  T& access()		  noexcept	{ return initial.template access<T>();	}

	template <typename T>
	HFSM2_CONSTEXPR(14) const T& access()	const noexcept	{ return initial.template access<T>();	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	wideRegister		 (Registry& registry, const ForkID forkId)							  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) bool	wideForwardEntryGuard(GuardControl& control,				   const ProngCBits prongs)	  noexcept;
	HFSM2_CONSTEXPR(14) bool	wideForwardEntryGuard(GuardControl& control											  )	  noexcept;
	HFSM2_CONSTEXPR(14) bool	wideEntryGuard		 (GuardControl& control											  )	  noexcept;

	HFSM2_CONSTEXPR(14) void	wideEnter			 (PlanControl&	control											  )	  noexcept;
	HFSM2_CONSTEXPR(14) void	wideReenter			 (PlanControl&	control											  )	  noexcept;

	HFSM2_CONSTEXPR(14) Status	wideUpdate			 (FullControl&	control											  )	  noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14) Status	wideReact			 (FullControl&	control, const TEvent& event					  )	  noexcept;

	HFSM2_CONSTEXPR(14) bool	wideForwardExitGuard (GuardControl& control,				   const ProngCBits prongs)	  noexcept;
	HFSM2_CONSTEXPR(14) bool	wideForwardExitGuard (GuardControl& control											  )	  noexcept;
	HFSM2_CONSTEXPR(14) bool	wideExitGuard		 (GuardControl& control											  )	  noexcept;

	HFSM2_CONSTEXPR(14) void	wideExit			 (PlanControl&	control											  )	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	wideForwardActive	 (Control& control, const Request request, const ProngCBits prongs)	  noexcept;
	HFSM2_CONSTEXPR(14) void	wideForwardRequest	 (Control& control, const Request request						  )	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	wideRequestChange	 (Control& control, const Request request						  )	  noexcept;
	HFSM2_CONSTEXPR(14) void	wideRequestRestart	 (Control& control, const Request request						  )	  noexcept;
	HFSM2_CONSTEXPR(14) void	wideRequestResume	 (Control& control, const Request request						  )	  noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14) void	wideRequestUtilize	 (Control& control, const Request request						  )	  noexcept;
	HFSM2_CONSTEXPR(14) void	wideRequestRandomize (Control& control, const Request request						  )	  noexcept;

	HFSM2_CONSTEXPR(14) Utility wideReportChange	 (Control& control												  )	  noexcept;
	HFSM2_CONSTEXPR(14) Utility wideReportUtilize	 (Control& control												  )	  noexcept;
	HFSM2_CONSTEXPR(14) Utility wideReportRandomize  (Control& control												  )	  noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	wideChangeToRequested(PlanControl&  control											  )	  noexcept;

	//----------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_CONSTEXPR(14) void	wideSaveActive		  (const Registry& registry, WriteStream& stream)	const noexcept;
	HFSM2_CONSTEXPR(14) void	wideSaveResumable	  (const Registry& registry, WriteStream& stream)	const noexcept;

	HFSM2_CONSTEXPR(14) void	wideLoadRequested	  (		 Registry& registry, ReadStream&  stream)	const noexcept;
	HFSM2_CONSTEXPR(14) void	wideLoadResumable	  (		 Registry& registry, ReadStream&  stream)	const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename Args::StructureStateInfos;

	static constexpr Long NAME_COUNT	 = Initial::NAME_COUNT;

	HFSM2_CONSTEXPR(14) void wideGetNames(const Long parent,
										  const Short depth,
										  StructureStateInfos& stateInfos) const;
#endif

	//----------------------------------------------------------------------

	Initial initial;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "orthogonal_sub_1.inl"
#include "orthogonal_sub_2.inl"
