namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename, typename, ShortIndex, typename...>
struct OS_;

//------------------------------------------------------------------------------

template <typename TIndices,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct OS_<TIndices, TArgs, NIndex, TInitial, TRemaining...> final {
	using Indices		= TIndices;
	static constexpr StateID	INITIAL_ID	= Indices::STATE_ID;
	static constexpr ShortIndex COMPO_INDEX	= Indices::COMPO_INDEX;
	static constexpr ShortIndex ORTHO_INDEX	= Indices::ORTHO_INDEX;
	static constexpr ShortIndex ORTHO_UNIT	= Indices::ORTHO_UNIT;

	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			= TArgs;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;
	using OrthoForks	= typename Registry::OrthoForks;
	using ProngBits		= typename OrthoForks::Bits;
	using ProngConstBits= typename OrthoForks::ConstBits;

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

#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION
	template <typename T>
	HFSM2_INLINE	   T& access();

	template <typename T>
	HFSM2_INLINE const T& access() const;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideRegister		  (Registry& registry, const ForkID forkId);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE bool	 wideForwardEntryGuard(GuardControl& control,								const ProngConstBits prongs);
	HFSM2_INLINE bool	 wideForwardEntryGuard(GuardControl& control);
	HFSM2_INLINE bool	 wideEntryGuard		  (GuardControl& control);

	HFSM2_INLINE void	 wideConstruct		  (PlanControl&	 control);

	HFSM2_INLINE void	 wideEnter			  (PlanControl&	 control);
	HFSM2_INLINE void	 wideReenter		  (PlanControl&	 control);

	HFSM2_INLINE Status	 wideUpdate			  (FullControl&	 control);

	template <typename TEvent>
	HFSM2_INLINE Status	 wideReact			  (FullControl&	 control, const TEvent& event);

	HFSM2_INLINE bool	 wideForwardExitGuard (GuardControl& control,								const ProngConstBits prongs);
	HFSM2_INLINE bool	 wideForwardExitGuard (GuardControl& control);
	HFSM2_INLINE bool	 wideExitGuard		  (GuardControl& control);

	HFSM2_INLINE void	 wideExit			  (PlanControl&	 control);

	HFSM2_INLINE void	 wideDestruct		  (PlanControl&  control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideForwardActive	  (Control&  control, const TransitionType requestType,	const ProngConstBits prongs);
	HFSM2_INLINE void	 wideForwardRequest	  (Control&  control, const TransitionType requestType,	const ProngConstBits prongs);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideRequestChange	  (Control&  control);
	HFSM2_INLINE void	 wideRequestRemain	  (Registry& registry);
	HFSM2_INLINE void	 wideRequestRestart	  (Registry& registry);
	HFSM2_INLINE void	 wideRequestResume	  (Registry& registry);

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void	 wideRequestUtilize	  (Control&  control);
	HFSM2_INLINE void	 wideRequestRandomize (Control&  control);

	HFSM2_INLINE Utility wideReportChange	  (Control&  control);
	HFSM2_INLINE Utility wideReportUtilize	  (Control&  control);
	HFSM2_INLINE Utility wideReportRandomize  (Control&  control);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideChangeToRequested(PlanControl& control);

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_INLINE void	 wideSaveActive		  (const Registry& registry, WriteStream& stream) const;
	HFSM2_INLINE void	 wideSaveResumable	  (const Registry& registry, WriteStream& stream) const;

	HFSM2_INLINE void	 wideLoadRequested	  (		 Registry& registry, ReadStream&  stream) const;
	HFSM2_INLINE void	 wideLoadResumable	  (		 Registry& registry, ReadStream&  stream) const;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;

	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT  + Remaining::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	//----------------------------------------------------------------------

	Initial initial;
	Remaining remaining;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename TInitial>
struct OS_<TIndices, TArgs, NIndex, TInitial> final {
	using Indices		= TIndices;
	static constexpr StateID	INITIAL_ID	= Indices::STATE_ID;
	static constexpr ShortIndex COMPO_INDEX	= Indices::COMPO_INDEX;
	static constexpr ShortIndex ORTHO_INDEX	= Indices::ORTHO_INDEX;
	static constexpr ShortIndex ORTHO_UNIT	= Indices::ORTHO_UNIT;

	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			= TArgs;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
#endif

	using Registry		= RegistryT<Args>;
	using StateParents	= typename Registry::StateParents;
	using OrthoForks	= typename Registry::OrthoForks;
	using ProngBits		= typename OrthoForks::Bits;
	using ProngConstBits= typename OrthoForks::ConstBits;

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

#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION
	template <typename T>
	HFSM2_INLINE	   T& access()			  { return initial.template access<T>();	}

	template <typename T>
	HFSM2_INLINE const T& access() const	  { return initial.template access<T>();	}
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideRegister		  (Registry& registry, const ForkID forkId);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE bool	 wideForwardEntryGuard(GuardControl& control,								const ProngConstBits prongs);
	HFSM2_INLINE bool	 wideForwardEntryGuard(GuardControl& control);
	HFSM2_INLINE bool	 wideEntryGuard		  (GuardControl& control);

	HFSM2_INLINE void	 wideConstruct		  (PlanControl&  control);

	HFSM2_INLINE void	 wideEnter			  (PlanControl&  control);
	HFSM2_INLINE void	 wideReenter		  (PlanControl&  control);

	HFSM2_INLINE Status	 wideUpdate			  (FullControl&  control);

	template <typename TEvent>
	HFSM2_INLINE Status	 wideReact			  (FullControl&  control, const TEvent& event);

	HFSM2_INLINE bool	 wideForwardExitGuard (GuardControl& control,								const ProngConstBits prongs);
	HFSM2_INLINE bool	 wideForwardExitGuard (GuardControl& control);
	HFSM2_INLINE bool	 wideExitGuard		  (GuardControl& control);

	HFSM2_INLINE void	 wideExit			  (PlanControl&  control);

	HFSM2_INLINE void	 wideDestruct		  (PlanControl&  control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideForwardActive	  (Control&  control, const TransitionType requestType,	const ProngConstBits prongs);
	HFSM2_INLINE void	 wideForwardRequest	  (Control&  control, const TransitionType requestType,	const ProngConstBits prongs);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideRequestChange	  (Control&  control);
	HFSM2_INLINE void	 wideRequestRemain	  (Registry& registry);
	HFSM2_INLINE void	 wideRequestRestart	  (Registry& registry);
	HFSM2_INLINE void	 wideRequestResume	  (Registry& registry);

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void	 wideRequestUtilize	  (Control& control);
	HFSM2_INLINE void	 wideRequestRandomize (Control& control);

	HFSM2_INLINE Utility wideReportChange	  (Control& control);
	HFSM2_INLINE Utility wideReportUtilize	  (Control& control);
	HFSM2_INLINE Utility wideReportRandomize  (Control& control);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 wideChangeToRequested(PlanControl& control);

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION
	using WriteStream	= typename Args::WriteStream;
	using ReadStream	= typename Args::ReadStream;

	HFSM2_INLINE void	 wideSaveActive		  (const Registry& registry, WriteStream& stream) const;
	HFSM2_INLINE void	 wideSaveResumable	  (const Registry& registry, WriteStream& stream) const;

	HFSM2_INLINE void	 wideLoadRequested	  (		 Registry& registry, ReadStream&  stream) const;
	HFSM2_INLINE void	 wideLoadResumable	  (		 Registry& registry, ReadStream&  stream) const;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;

	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
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
