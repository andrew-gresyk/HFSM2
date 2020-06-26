﻿namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename>
struct SI_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <Strategy, typename, typename...>
struct CI_;

template <typename...>
struct CSI_;

template <typename TInitial, typename... TRemaining>
struct CSI_<TInitial, TRemaining...>;

template <typename TInitial>
struct CSI_<TInitial>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename, typename...>
struct OI_;

template <typename...>
struct OSI_;

template <typename TInitial, typename... TRemaining>
struct OSI_<TInitial, TRemaining...>;

template <typename TInitial>
struct OSI_<TInitial>;

//------------------------------------------------------------------------------

template <typename...>
struct WrapInfoT;

template <typename TH>
struct WrapInfoT<	 TH> {
	using Type = SI_<TH>;
};

template <Strategy SG, typename TH, typename... TS>
struct WrapInfoT<	 CI_<SG, TH, TS...>> {
	using Type =	 CI_<SG, TH, TS...>;
};

template <typename... TS>
struct WrapInfoT<	 OI_<TS...>> {
	using Type =	 OI_<TS...>;
};

template <typename... TS>
using WrapInfo = typename WrapInfoT<TS...>::Type;

//------------------------------------------------------------------------------

template <typename THead>
struct SI_ final {
	using Head				= THead;
	using StateList			= ITL_<Head>;
	using RegionList		= ITL_<>;

	static constexpr ShortIndex WIDTH			= 1;
	static constexpr LongIndex  REVERSE_DEPTH	= 1;
	static constexpr ShortIndex COMPO_REGIONS	= 0;
	static constexpr LongIndex  COMPO_PRONGS	= 0;
	static constexpr ShortIndex ORTHO_REGIONS	= 0;
	static constexpr ShortIndex ORTHO_UNITS		= 0;

	static constexpr LongIndex  ACTIVE_BITS		= 0;
	static constexpr LongIndex  RESUMABLE_BITS	= 0;

	static constexpr LongIndex  STATE_COUNT		= StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT	= RegionList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct CSI_<TInitial, TRemaining...> {
	using Initial			= WrapInfo<TInitial>;
	using Remaining			= CSI_<TRemaining...>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;
	using RegionList		= Merge<typename Initial::RegionList, typename Remaining::RegionList>;

	static constexpr LongIndex  REVERSE_DEPTH	= Max<Initial::REVERSE_DEPTH,	Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr ShortIndex COMPO_REGIONS	=	  Initial::COMPO_REGIONS  + Remaining::COMPO_REGIONS;
	static constexpr LongIndex  COMPO_PRONGS	=	  Initial::COMPO_PRONGS   + Remaining::COMPO_PRONGS;
	static constexpr ShortIndex ORTHO_REGIONS	=	  Initial::ORTHO_REGIONS  + Remaining::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS		=	  Initial::ORTHO_UNITS    + Remaining::ORTHO_UNITS;

	static constexpr LongIndex  ACTIVE_BITS		= Max<Initial::ACTIVE_BITS,		Remaining::ACTIVE_BITS>::VALUE;
	static constexpr LongIndex  RESUMABLE_BITS	=	  Initial::RESUMABLE_BITS + Remaining::RESUMABLE_BITS;

	static constexpr LongIndex  STATE_COUNT		= StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT	= RegionList::SIZE;
};

template <typename TInitial>
struct CSI_<TInitial> {
	using Initial			= WrapInfo<TInitial>;
	using StateList			= typename Initial::StateList;
	using RegionList		= typename Initial::RegionList;

	static constexpr LongIndex  REVERSE_DEPTH	= Initial::REVERSE_DEPTH;
	static constexpr ShortIndex COMPO_REGIONS	= Initial::COMPO_REGIONS;
	static constexpr LongIndex  COMPO_PRONGS	= Initial::COMPO_PRONGS;
	static constexpr ShortIndex ORTHO_REGIONS	= Initial::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS		= Initial::ORTHO_UNITS;

	static constexpr LongIndex  ACTIVE_BITS		= Initial::ACTIVE_BITS;
	static constexpr LongIndex  RESUMABLE_BITS	= Initial::RESUMABLE_BITS;

	static constexpr LongIndex  STATE_COUNT		= StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT	= RegionList::SIZE;
};

template <Strategy TStrategy, typename THead, typename... TSubStates>
struct CI_ final {
	static constexpr Strategy STRATEGY = TStrategy;

	using Head				= THead;
	using HeadInfo			= SI_<Head>;
	using SubStates			= CSI_<TSubStates...>;
	using StateList			= Merge<typename HeadInfo::StateList, typename SubStates::StateList>;
	using RegionList		= Merge<typename HeadInfo::StateList, typename SubStates::RegionList>;

	static constexpr ShortIndex	WIDTH			= sizeof...(TSubStates);
	static constexpr LongIndex	REVERSE_DEPTH	= SubStates::REVERSE_DEPTH + 1;
	static constexpr ShortIndex	COMPO_REGIONS	= SubStates::COMPO_REGIONS + 1;
	static constexpr LongIndex	COMPO_PRONGS	= SubStates::COMPO_PRONGS + WIDTH;
	static constexpr ShortIndex	ORTHO_REGIONS	= SubStates::ORTHO_REGIONS;
	static constexpr ShortIndex	ORTHO_UNITS		= SubStates::ORTHO_UNITS;

	static constexpr LongIndex	WIDTH_BITS		= bitWidth(WIDTH);
	static constexpr LongIndex  ACTIVE_BITS		= SubStates::ACTIVE_BITS	+ WIDTH_BITS;
	static constexpr LongIndex  RESUMABLE_BITS	= SubStates::RESUMABLE_BITS + WIDTH_BITS + 1;

	static constexpr LongIndex	STATE_COUNT		= StateList::SIZE;
	static constexpr ShortIndex	REGION_COUNT	= RegionList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct OSI_<TInitial, TRemaining...> {
	using Initial			= WrapInfo<TInitial>;
	using Remaining			= OSI_<TRemaining...>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;
	using RegionList		= Merge<typename Initial::RegionList, typename Remaining::RegionList>;

	static constexpr LongIndex  REVERSE_DEPTH	= Max<Initial::REVERSE_DEPTH,	Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr ShortIndex COMPO_REGIONS	=	  Initial::COMPO_REGIONS  + Remaining::COMPO_REGIONS;
	static constexpr LongIndex  COMPO_PRONGS	=	  Initial::COMPO_PRONGS   + Remaining::COMPO_PRONGS;
	static constexpr ShortIndex ORTHO_REGIONS	=	  Initial::ORTHO_REGIONS  + Remaining::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS		=	  Initial::ORTHO_UNITS    + Remaining::ORTHO_UNITS;

	static constexpr LongIndex  ACTIVE_BITS		=	  Initial::ACTIVE_BITS    + Remaining::ACTIVE_BITS;
	static constexpr LongIndex  RESUMABLE_BITS	=	  Initial::RESUMABLE_BITS + Remaining::RESUMABLE_BITS;
};

template <typename TInitial>
struct OSI_<TInitial> {
	using Initial			= WrapInfo<TInitial>;
	using StateList			= typename Initial::StateList;
	using RegionList		= typename Initial::RegionList;

	static constexpr LongIndex  REVERSE_DEPTH	= Initial::REVERSE_DEPTH;
	static constexpr ShortIndex COMPO_REGIONS	= Initial::COMPO_REGIONS;
	static constexpr LongIndex  COMPO_PRONGS	= Initial::COMPO_PRONGS;
	static constexpr ShortIndex ORTHO_REGIONS	= Initial::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS		= Initial::ORTHO_UNITS;

	static constexpr LongIndex  ACTIVE_BITS		= Initial::ACTIVE_BITS;
	static constexpr LongIndex  RESUMABLE_BITS	= Initial::RESUMABLE_BITS;
};

template <typename THead, typename... TSubStates>
struct OI_ final {
	using Head				= THead;
	using HeadInfo			= SI_<Head>;
	using SubStates			= OSI_<TSubStates...>;
	using StateList			= Merge<typename HeadInfo::StateList, typename SubStates::StateList>;
	using RegionList		= Merge<typename HeadInfo::StateList, typename SubStates::RegionList>;

	static constexpr ShortIndex WIDTH			= sizeof...(TSubStates);
	static constexpr LongIndex  REVERSE_DEPTH	= SubStates::REVERSE_DEPTH + 1;
	static constexpr ShortIndex COMPO_REGIONS	= SubStates::COMPO_REGIONS;
	static constexpr LongIndex  COMPO_PRONGS	= SubStates::COMPO_PRONGS;
	static constexpr ShortIndex ORTHO_REGIONS	= SubStates::ORTHO_REGIONS + 1;
	static constexpr ShortIndex ORTHO_UNITS		= SubStates::ORTHO_UNITS + (WIDTH + 7) / 8;

	static constexpr LongIndex  ACTIVE_BITS		= SubStates::ACTIVE_BITS;
	static constexpr LongIndex  RESUMABLE_BITS	= SubStates::RESUMABLE_BITS;

	static constexpr LongIndex  STATE_COUNT		= StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT	= RegionList::SIZE;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NCompoCount,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  LongIndex NSerialBits,
		  LongIndex NTaskCapacity>
struct ArgsT final {
	using Context	 = TContext;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Rank		 = typename TConfig::Rank;
	using Utility	 = typename TConfig::Utility;
	using RNG		 = typename TConfig::RNG;
	using UP		 = typename TConfig::UP;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger	 = typename TConfig::Logger;
#endif

	using StateList	 = TStateList;
	using RegionList = TRegionList;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex COMPO_REGIONS = NCompoCount;
	static constexpr ShortIndex ORTHO_REGIONS = NOrthoCount;
	static constexpr ShortIndex ORTHO_UNITS	  = NOrthoUnits;
	static constexpr ShortIndex SERIAL_BITS	  = NSerialBits;

	static constexpr LongIndex  TASK_CAPACITY = NTaskCapacity;

#ifdef HFSM2_ENABLE_SERIALIZATION
	using SerialBuffer			= StreamBuffer	<SERIAL_BITS>;
	using WriteStream			= BitWriteStream<SERIAL_BITS>;
	using ReadStream			= BitReadStream	<SERIAL_BITS>;
#endif

	HFSM2_IF_STRUCTURE_REPORT(using StructureStateInfos = Array<StructureStateInfo, STATE_COUNT>);
};

//------------------------------------------------------------------------------

template <StateID NStateID,
		  ShortIndex NCompoIndex,
		  ShortIndex NOrthoIndex,
		  ShortIndex NOrthoUnit>
struct I_ {
	static constexpr StateID	STATE_ID	 = NStateID;
	static constexpr ShortIndex COMPO_INDEX	 = NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	 = NOrthoIndex;
	static constexpr ShortIndex ORTHO_UNIT	 = NOrthoUnit;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename, typename, typename>
struct S_;

template <typename, typename, Strategy, typename, typename...>
struct C_;

template <typename, typename, Strategy, ShortIndex, typename...>
struct CS_;

template <typename, typename, typename, typename...>
struct O_;

template <typename, typename, ShortIndex, typename...>
struct OS_;

template <typename, typename>
class RW_;

//------------------------------------------------------------------------------

template <typename, typename...>
struct MaterialT;

template <typename TN, typename TA, typename TH>
struct MaterialT   <TN, TA, TH> {
	using Type = S_<TN, TA, TH>;
};

template <typename TN, typename TA, Strategy SG, 			  typename... TS>
struct MaterialT   <TN, TA, CI_<SG, void,             TS...>> {
	using Type = C_<TN, TA,     SG, StaticEmptyT<TA>, TS...>;
};

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
struct MaterialT   <TN, TA, CI_<SG, TH,	TS...>> {
	using Type = C_<TN, TA,     SG, TH,	TS...>;
};

template <typename TN, typename TA,				 typename... TS>
struct MaterialT   <TN, TA, OI_<void,			  TS...>> {
	using Type = O_<TN, TA,     StaticEmptyT<TA>, TS...>;
};

template <typename TN, typename TA, typename TH, typename... TS>
struct MaterialT   <TN, TA, OI_<TH,				  TS...>> {
	using Type = O_<TN, TA,     TH,				  TS...>;
};

template <typename TN, typename... TS>
using Material = typename MaterialT<TN, TS...>::Type;

//------------------------------------------------------------------------------

template <typename TConfig,
		  typename TApex>
struct RF_ final {
	using Context		= typename TConfig::Context;
	using Apex			= TApex;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static constexpr LongIndex SUBSTITUTION_LIMIT= TConfig::SUBSTITUTION_LIMIT;

	static constexpr LongIndex TASK_CAPACITY	 = TConfig::TASK_CAPACITY != INVALID_LONG_INDEX ?
													   TConfig::TASK_CAPACITY : Apex::COMPO_PRONGS * 2;

	static constexpr ShortIndex COMPO_REGIONS	 = Apex::COMPO_REGIONS;
	static constexpr ShortIndex ORTHO_REGIONS	 = Apex::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS		 = Apex::ORTHO_UNITS;

	static constexpr LongIndex  ACTIVE_BITS		 = Apex::ACTIVE_BITS;
	static constexpr LongIndex  RESUMABLE_BITS	 = Apex::RESUMABLE_BITS;

	using StateList		= typename Apex::StateList;
	using RegionList	= typename Apex::RegionList;

	using Args			= ArgsT<Context,
								TConfig,
								StateList,
								RegionList,
								COMPO_REGIONS,
								ORTHO_REGIONS,
								ORTHO_UNITS,
								ACTIVE_BITS + RESUMABLE_BITS,
								TASK_CAPACITY>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Instance		= RW_<TConfig, Apex>;

	using Control		= ControlT	   <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Injection		= InjectionT<Args>;

	//----------------------------------------------------------------------

	using DynamicState	= DynamicEmptyT<Args>;

	template <typename... TInjections>
	using DynamicStateT	= DB_<TInjections...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using StaticState	= StaticEmptyT<Args>;

	template <typename... TInjections>
	using StaticStateT	= SB_<TInjections...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using State			= StaticState;

	template <typename... TInjections>
	using StateT		= StaticStateT<TInjections...>;

	//----------------------------------------------------------------------

	template <typename T>
	static constexpr bool contains() {
		return StateList::template index<T>() != INVALID_LONG_INDEX;
	}

	template <typename T>
	static constexpr StateID  stateId()		{ return			StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()	{ return (RegionID) RegionList::template index<T>();	}
};

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
struct CSubMaterialT;

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename... TS>
struct CSubMaterialT<TN, TA, SG, NI, ITL_<TS...>> {
	using Type = CS_<TN, TA, SG, NI,	  TS...>;
};

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename... TS>
using CSubMaterial = typename CSubMaterialT<TN, TA, SG, NI, TS...>::Type;

//------------------------------------------------------------------------------

template <typename...>
struct InfoT;

template <typename TN, typename TA, typename TH>
struct InfoT<S_<TN, TA, TH>> {
	using Type = SI_<	TH>;
};

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
struct InfoT<C_<TN, TA, SG, TH, TS...>> {
	using Type = CI_<	SG, TH, TS...>;
};

template <typename TN, typename TA, typename TH, typename... TS>
struct InfoT<O_<TN, TA, TH, TS...>> {
	using Type = OI_<	TH, TS...>;
};

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename... TS>
struct InfoT<CS_<TN, TA, SG, NI, TS...>> {
	using Type = CSI_<			 TS...>;
};

////////////////////////////////////////////////////////////////////////////////

}
}
