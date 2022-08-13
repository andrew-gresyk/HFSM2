namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename>
struct SI_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <Strategy, typename, typename...>
struct CI_;

template <typename>
struct CSI_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename, typename...>
struct OI_;

template <typename...>
struct OSI_;

template <typename TI, typename... TR>
struct OSI_<TI, TR...>;

template <typename TI>
struct OSI_<TI>;

//------------------------------------------------------------------------------

template <typename...>
struct WrapInfoT;

template <typename TH>
struct WrapInfoT<	 TH> final {
	using Type = SI_<TH>;
};

template <Strategy SG, typename TH, typename... TS>
struct WrapInfoT<CI_<SG, TH, TS...>> final {
	using Type = CI_<SG, TH, TS...>;
};

template <typename... TS>
struct WrapInfoT<OI_<TS...>> final {
	using Type = OI_<TS...>;
};

template <typename... TS>
using WrapInfo = typename WrapInfoT<TS...>::Type;

//------------------------------------------------------------------------------
// COMMON

template <typename THead>
struct SI_ final {
	using Head				= THead;
	using StateList			= TL_<Head>;
	using RegionList		= TL_<>;

	static constexpr Short WIDTH		  = 1;
	static constexpr Long  REVERSE_DEPTH  = 1;
	static constexpr Short COMPO_REGIONS  = 0;
	static constexpr Long  COMPO_PRONGS	  = 0;
	static constexpr Short ORTHO_REGIONS  = 0;
	static constexpr Short ORTHO_UNITS	  = 0;

#if HFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  ACTIVE_BITS	  = 0;
	static constexpr Long  RESUMABLE_BITS = 0;
#endif

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
	static constexpr Short REGION_COUNT	  = RegionList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, typename... TR>
struct CSI_<TL_<TI, TR...>> final {
	using Initial			= WrapInfo<TI>;
	using Remaining			= CSI_<TL_<TR...>>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;
	using RegionList		= Merge<typename Initial::RegionList, typename Remaining::RegionList>;

	static constexpr Long  REVERSE_DEPTH  = max(Initial::REVERSE_DEPTH,	  Remaining::REVERSE_DEPTH);
	static constexpr Short COMPO_REGIONS  =		Initial::COMPO_REGIONS  + Remaining::COMPO_REGIONS;
	static constexpr Long  COMPO_PRONGS	  =		Initial::COMPO_PRONGS   + Remaining::COMPO_PRONGS;
	static constexpr Short ORTHO_REGIONS  =		Initial::ORTHO_REGIONS  + Remaining::ORTHO_REGIONS;
	static constexpr Short ORTHO_UNITS	  =		Initial::ORTHO_UNITS    + Remaining::ORTHO_UNITS;

#if HFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  ACTIVE_BITS	  = max(Initial::ACTIVE_BITS,	  Remaining::ACTIVE_BITS);
	static constexpr Long  RESUMABLE_BITS =		Initial::RESUMABLE_BITS + Remaining::RESUMABLE_BITS;
#endif

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
	static constexpr Short REGION_COUNT	  = RegionList::SIZE;
};

template <typename TI>
struct CSI_<TL_<TI>> final {
	using Initial			= WrapInfo<TI>;
	using StateList			= typename Initial::StateList;
	using RegionList		= typename Initial::RegionList;

	static constexpr Long  REVERSE_DEPTH  = Initial::REVERSE_DEPTH;
	static constexpr Short COMPO_REGIONS  = Initial::COMPO_REGIONS;
	static constexpr Long  COMPO_PRONGS	  = Initial::COMPO_PRONGS;
	static constexpr Short ORTHO_REGIONS  = Initial::ORTHO_REGIONS;
	static constexpr Short ORTHO_UNITS	  = Initial::ORTHO_UNITS;

#if HFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  ACTIVE_BITS	  = Initial::ACTIVE_BITS;
	static constexpr Long  RESUMABLE_BITS = Initial::RESUMABLE_BITS;
#endif

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
	static constexpr Short REGION_COUNT	  = RegionList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <Strategy TStrategy, typename THead, typename... TSubStates>
struct CI_ final {
	static constexpr Strategy STRATEGY = TStrategy;

	using Head				= THead;
	using HeadInfo			= SI_<Head>;
	using SubStates			= CSI_<TL_<TSubStates...>>;
	using StateList			= Merge<typename HeadInfo::StateList, typename SubStates::StateList>;
	using RegionList		= Merge<typename HeadInfo::StateList, typename SubStates::RegionList>;

	static constexpr Short WIDTH		  = sizeof...(TSubStates);
	static constexpr Long  REVERSE_DEPTH  = SubStates::REVERSE_DEPTH + 1;
	static constexpr Short COMPO_REGIONS  = SubStates::COMPO_REGIONS + 1;
	static constexpr Long  COMPO_PRONGS	  = SubStates::COMPO_PRONGS + WIDTH;
	static constexpr Short ORTHO_REGIONS  = SubStates::ORTHO_REGIONS;
	static constexpr Short ORTHO_UNITS	  = SubStates::ORTHO_UNITS;

#if HFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  WIDTH_BITS	  = bitContain(WIDTH);
	static constexpr Long  ACTIVE_BITS	  = SubStates::ACTIVE_BITS	+ WIDTH_BITS;
	static constexpr Long  RESUMABLE_BITS = SubStates::RESUMABLE_BITS + WIDTH_BITS + 1;
#endif

	static constexpr Long  STATE_COUNT	  = StateList::SIZE;
	static constexpr Short REGION_COUNT	  = RegionList::SIZE;
};

// COMMON
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, typename... TR>
struct OSI_<TI, TR...> final {
	using Initial			= WrapInfo<TI>;
	using Remaining			= OSI_<TR...>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;
	using RegionList		= Merge<typename Initial::RegionList, typename Remaining::RegionList>;

	static constexpr Long  REVERSE_DEPTH  = max(Initial::REVERSE_DEPTH,	  Remaining::REVERSE_DEPTH);
	static constexpr Short COMPO_REGIONS  =		Initial::COMPO_REGIONS  + Remaining::COMPO_REGIONS;
	static constexpr Long  COMPO_PRONGS	  =		Initial::COMPO_PRONGS   + Remaining::COMPO_PRONGS;
	static constexpr Short ORTHO_REGIONS  =		Initial::ORTHO_REGIONS  + Remaining::ORTHO_REGIONS;
	static constexpr Short ORTHO_UNITS	  =		Initial::ORTHO_UNITS    + Remaining::ORTHO_UNITS;

#if HFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  ACTIVE_BITS	  =		Initial::ACTIVE_BITS    + Remaining::ACTIVE_BITS;
	static constexpr Long  RESUMABLE_BITS =		Initial::RESUMABLE_BITS + Remaining::RESUMABLE_BITS;
#endif
};

template <typename TI>
struct OSI_<TI> final {
	using Initial			= WrapInfo<TI>;
	using StateList			= typename Initial::StateList;
	using RegionList		= typename Initial::RegionList;

	static constexpr Long  REVERSE_DEPTH	= Initial::REVERSE_DEPTH;
	static constexpr Short COMPO_REGIONS	= Initial::COMPO_REGIONS;
	static constexpr Long  COMPO_PRONGS		= Initial::COMPO_PRONGS;
	static constexpr Short ORTHO_REGIONS	= Initial::ORTHO_REGIONS;
	static constexpr Short ORTHO_UNITS		= Initial::ORTHO_UNITS;

#if HFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  ACTIVE_BITS		= Initial::ACTIVE_BITS;
	static constexpr Long  RESUMABLE_BITS	= Initial::RESUMABLE_BITS;
#endif
};

template <typename THead, typename... TSubStates>
struct OI_ final {
	using Head				= THead;
	using HeadInfo			= SI_<Head>;
	using SubStates			= OSI_<TSubStates...>;
	using StateList			= Merge<typename HeadInfo::StateList, typename SubStates::StateList>;
	using RegionList		= Merge<typename HeadInfo::StateList, typename SubStates::RegionList>;

	static constexpr Short WIDTH			= sizeof...(TSubStates);
	static constexpr Long  REVERSE_DEPTH	= SubStates::REVERSE_DEPTH + 1;
	static constexpr Short COMPO_REGIONS	= SubStates::COMPO_REGIONS;
	static constexpr Long  COMPO_PRONGS		= SubStates::COMPO_PRONGS;
	static constexpr Short ORTHO_REGIONS	= SubStates::ORTHO_REGIONS + 1;
	static constexpr Short ORTHO_UNITS		= SubStates::ORTHO_UNITS + contain(WIDTH, 8);

#if HFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  ACTIVE_BITS		= SubStates::ACTIVE_BITS;
	static constexpr Long  RESUMABLE_BITS	= SubStates::RESUMABLE_BITS;
#endif

	static constexpr Long  STATE_COUNT		= StateList::SIZE;
	static constexpr Short REGION_COUNT		= RegionList::SIZE;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NCompoCount
		, Long NOrthoCount
		, Long NOrthoUnits
		HFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct ArgsT final {
	using Context		= TContext;
	using PureContext	= Undecorate<Context>;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank			= typename TConfig::Rank;
	using Utility		= typename TConfig::Utility;
	using RNG			= typename TConfig::RNG;
	using UP			= typename TConfig::UP;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger		= typename TConfig::LoggerInterface;
#endif

	using StateList		= TStateList;
	using RegionList	= TRegionList;

	static constexpr Long  STATE_COUNT		  = StateList::SIZE;
	static constexpr Short REGION_COUNT		  = RegionList::SIZE;
	static constexpr Short COMPO_REGIONS	  = NCompoCount;
	static constexpr Short ORTHO_REGIONS	  = NOrthoCount;
	static constexpr Short ORTHO_UNITS		  = NOrthoUnits;

#if HFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Short SERIAL_BITS		  = NSerialBits;
#endif

	static constexpr Short SUBSTITUTION_LIMIT = NSubstitutionLimit;

#if HFSM2_PLANS_AVAILABLE()
	static constexpr Long  TASK_CAPACITY	  = NTaskCapacity;
#endif

	using Payload		= TPayload;

#if HFSM2_SERIALIZATION_AVAILABLE()
	using SerialBuffer	= StreamBufferT  <SERIAL_BITS>;
	using WriteStream	= BitWriteStreamT<SERIAL_BITS>;
	using ReadStream	= BitReadStreamT <SERIAL_BITS>;
#endif

	HFSM2_IF_STRUCTURE_REPORT(using StructureStateInfos = ArrayT<StructureStateInfo, STATE_COUNT>);
};

//------------------------------------------------------------------------------

template <StateID NStateID,
		  Short NCompoIndex,
		  Short NOrthoIndex,
		  Short NOrthoUnit>
struct I_ final {
	static constexpr StateID STATE_ID	 = NStateID;
	static constexpr Short	 COMPO_INDEX = NCompoIndex;
	static constexpr Short	 ORTHO_INDEX = NOrthoIndex;
	static constexpr Short	 ORTHO_UNIT	 = NOrthoUnit;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename, typename, typename>
struct S_;

template <typename, typename, Strategy, typename, typename...>
struct C_;

template <typename, typename, Strategy, Short, typename>
struct CS_;

template <typename, typename, typename, typename...>
struct O_;

template <typename, typename, Short, typename...>
struct OS_;

template <typename, typename>
class InstanceT;

//------------------------------------------------------------------------------

template <typename, typename...>
struct MaterialT_;

template <typename TN, typename TA, typename TH>
struct MaterialT_  <TN, TA, TH> final {
	using Type = S_<TN, TA, TH>;
};

template <typename TN, typename TA, Strategy SG, 			  typename... TS>
struct MaterialT_  <TN, TA, CI_<SG, void,       TS...>> final {
	using Type = C_<TN, TA,     SG, EmptyT<TA>, TS...>;
};

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
struct MaterialT_  <TN, TA, CI_<SG, TH,	TS...>> final {
	using Type = C_<TN, TA,     SG, TH,	TS...>;
};

template <typename TN, typename TA,				 typename... TS>
struct MaterialT_  <TN, TA, OI_<void,       TS...>> final {
	using Type = O_<TN, TA,     EmptyT<TA>, TS...>;
};

template <typename TN, typename TA, typename TH, typename... TS>
struct MaterialT_  <TN, TA, OI_<TH,			TS...>> final {
	using Type = O_<TN, TA,     TH,			TS...>;
};

template <typename TN, typename... TS>
using MaterialT = typename MaterialT_<TN, TS...>::Type;

//------------------------------------------------------------------------------

template <typename TConfig,
		  typename TApex>
struct RF_ final {
	using Context		= typename TConfig::Context;
	using Apex			= TApex;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using StateList		= typename Apex::StateList;
	using RegionList	= typename Apex::RegionList;

	static constexpr Long  STATE_COUNT			= Apex::STATE_COUNT;
	static constexpr Short REGION_COUNT			= Apex::REGION_COUNT;

	static constexpr Long  SUBSTITUTION_LIMIT	= TConfig::SUBSTITUTION_LIMIT;

#if HFSM2_PLANS_AVAILABLE()
	static constexpr Long  TASK_CAPACITY		= TConfig::TASK_CAPACITY != INVALID_LONG ?
													  TConfig::TASK_CAPACITY : Apex::COMPO_PRONGS * 2;
#endif

	using Payload	= typename TConfig::Payload;

	static constexpr Short COMPO_REGIONS		= Apex::COMPO_REGIONS;
	static constexpr Short ORTHO_REGIONS		= Apex::ORTHO_REGIONS;
	static constexpr Short ORTHO_UNITS			= Apex::ORTHO_UNITS;

#if HFSM2_SERIALIZATION_AVAILABLE()
	static constexpr Long  ACTIVE_BITS			= Apex::ACTIVE_BITS;
	static constexpr Long  RESUMABLE_BITS		= Apex::RESUMABLE_BITS;
#endif

	using Args			= ArgsT<Context
							  , TConfig
							  , StateList
							  , RegionList
							  , COMPO_REGIONS
							  , ORTHO_REGIONS
							  , ORTHO_UNITS
							  HFSM2_IF_SERIALIZATION(, ACTIVE_BITS + RESUMABLE_BITS)
							  , SUBSTITUTION_LIMIT
							  HFSM2_IF_PLANS(, TASK_CAPACITY)
							  , Payload>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Instance		= InstanceT<TConfig, Apex>;

	using ConstControl	= ConstControlT<Args>;
	using Control		= ControlT	   <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using State			= EmptyT<Args>;

	template <typename... TInjections>
	using StateT		= A_<TInjections...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger		= typename TConfig::LoggerInterface;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TState>
	static constexpr bool	  contains()		noexcept	{ return					contains<StateList , TState>() ;	}

	template <typename TState>
	static constexpr StateID  stateId()			noexcept	{ return					   index<StateList , TState>() ;	}

	template <typename TState>
	static constexpr RegionID regionId()		noexcept	{ return static_cast<RegionID>(index<RegionList, TState>());	}
};

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
struct CSubMaterialT;

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
struct CSubMaterialT<TN, TA, SG, NI, TL_<TS...>> final {
	using Type = CS_<TN, TA, SG, NI, TL_<TS...>>;
};

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
using CSubMaterial = typename CSubMaterialT<TN, TA, SG, NI, TS...>::Type;

//------------------------------------------------------------------------------

template <typename>
struct InfoT;

template <typename TN, typename TA, typename TH>
struct InfoT<S_<TN, TA, TH>> final {
	using Type = SI_<	TH>;
};

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
struct InfoT<C_<TN, TA, SG, TH, TS...>> final {
	using Type = CI_<	SG, TH, TS...>;
};

template <typename TN, typename TA, typename TH, typename... TS>
struct InfoT<O_<TN, TA, TH, TS...>> final {
	using Type = OI_<	TH, TS...>;
};

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
struct InfoT<CS_<TN, TA, SG, NI, TL_<TS...>>> final {
	using Type = CSI_<			 TL_<TS...>>;
};

template <typename T>
using Info = typename InfoT<T>::Type;

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
struct LHalfCST;

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
struct LHalfCST<TN, TA, SG, NI, TL_<TS...>> final {
	using Type = CS_<TN,
					 TA,
					 SG,
					 NI,
					 LHalfTypes<TS...>>;
};

template <typename TN, typename TA, Strategy SG, Short NI, typename TL>
using LHalfCS = typename LHalfCST<TN, TA, SG, NI, TL>::Type;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
struct RHalfCST;

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
struct RHalfCST<TN, TA, SG, NI, TL_<TS...>> final {
	using Indices		= TN;
	static constexpr StateID  INITIAL_ID  = Indices::STATE_ID;
	static constexpr Short	  COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	  ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	  ORTHO_UNIT  = Indices::ORTHO_UNIT;

	using LStateList	= LHalfTypes<TS...>;

	using LHalfInfo		= CSI_<LStateList>;

	using Type			= CS_<I_<INITIAL_ID  + LHalfInfo::STATE_COUNT,
								 COMPO_INDEX + LHalfInfo::COMPO_REGIONS,
								 ORTHO_INDEX + LHalfInfo::ORTHO_REGIONS,
								 ORTHO_UNIT  + LHalfInfo::ORTHO_UNITS>,
							  TA,
							  SG,
							  NI + LStateList::SIZE,
							  RHalfTypes<TS...>>;
};

template <typename TN, typename TA, Strategy SG, Short NI, typename TL>
using RHalfCS = typename RHalfCST<TN, TA, SG, NI, TL>::Type;

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TI>
using InitialOS = MaterialT<TN, TA, TI>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
struct RemainingOST final {
	using Indices		= TN;
	static constexpr StateID INITIAL_ID	 = Indices::STATE_ID;
	static constexpr Short	 COMPO_INDEX = Indices::COMPO_INDEX;
	static constexpr Short	 ORTHO_INDEX = Indices::ORTHO_INDEX;
	static constexpr Short	 ORTHO_UNIT	 = Indices::ORTHO_UNIT;

	using InitialInfo	= WrapInfo<TI>;

	using Type			= OS_<I_<INITIAL_ID  + InitialInfo::STATE_COUNT,
								 COMPO_INDEX + InitialInfo::COMPO_REGIONS,
								 ORTHO_INDEX + InitialInfo::ORTHO_REGIONS,
								 ORTHO_UNIT  + InitialInfo::ORTHO_UNITS>,
							  TA,
							  NI + 1,
							  TR...>;
};

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
using RemainingOS = typename RemainingOST<TN, TA, NI, TI, TR...>::Type;

////////////////////////////////////////////////////////////////////////////////

}
}
