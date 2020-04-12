#pragma once

namespace hfsm2 {
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
struct WrapT;

template <typename TH_>
struct WrapT<TH_> {
	using Type = SI_<TH_>;
};

template <Strategy TG_, typename TH_, typename... TS_>
struct WrapT<	 CI_<TG_, TH_, TS_...>> {
	using Type = CI_<TG_, TH_, TS_...>;
};

template <typename... TS_>
struct WrapT<	 OI_<TS_...>> {
	using Type = OI_<TS_...>;
};

template <typename... TS_>
using Wrap = typename WrapT<TS_...>::Type;

//------------------------------------------------------------------------------

template <typename THead>
struct SI_ final {
	using Head				= THead;
	using StateList			= TL_<Head>;
	using RegionList		= TL_<>;

	static constexpr ShortIndex WIDTH		  = 1;
	static constexpr LongIndex  REVERSE_DEPTH = 1;
	static constexpr ShortIndex COMPO_REGIONS = 0;
	static constexpr LongIndex  COMPO_PRONGS  = 0;
	static constexpr ShortIndex ORTHO_REGIONS = 0;
	static constexpr ShortIndex ORTHO_UNITS	  = 0;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct CSI_<TInitial, TRemaining...> {
	using Initial			= Wrap<TInitial>;
	using Remaining			= CSI_<TRemaining...>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;
	using RegionList		= Merge<typename Initial::RegionList, typename Remaining::RegionList>;

	static constexpr LongIndex  REVERSE_DEPTH = Max<Initial::REVERSE_DEPTH,  Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr ShortIndex COMPO_REGIONS =		Initial::COMPO_REGIONS + Remaining::COMPO_REGIONS;
	static constexpr LongIndex  COMPO_PRONGS  =		Initial::COMPO_PRONGS  + Remaining::COMPO_PRONGS;
	static constexpr ShortIndex ORTHO_REGIONS =		Initial::ORTHO_REGIONS + Remaining::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS	  =		Initial::ORTHO_UNITS   + Remaining::ORTHO_UNITS;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
};

template <typename TInitial>
struct CSI_<TInitial> {
	using Initial			= Wrap<TInitial>;
	using StateList			= typename Initial::StateList;
	using RegionList		= typename Initial::RegionList;

	static constexpr LongIndex  REVERSE_DEPTH = Initial::REVERSE_DEPTH;
	static constexpr ShortIndex COMPO_REGIONS = Initial::COMPO_REGIONS;
	static constexpr LongIndex  COMPO_PRONGS  = Initial::COMPO_PRONGS;
	static constexpr ShortIndex ORTHO_REGIONS = Initial::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS	  = Initial::ORTHO_UNITS;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
};

template <Strategy TStrategy, typename THead, typename... TSubStates>
struct CI_ final {
	static constexpr Strategy STRATEGY = TStrategy;

	using Head				= THead;
	using HeadInfo			= SI_<Head>;
	using SubStates			= CSI_<TSubStates...>;
	using StateList			= Merge<typename HeadInfo::StateList, typename SubStates::StateList>;
	using RegionList		= Merge<typename HeadInfo::StateList, typename SubStates::RegionList>;

	static constexpr ShortIndex	WIDTH		  = sizeof...(TSubStates);
	static constexpr LongIndex	REVERSE_DEPTH = SubStates::REVERSE_DEPTH + 1;
	static constexpr ShortIndex	COMPO_REGIONS = SubStates::COMPO_REGIONS + 1;
	static constexpr LongIndex	COMPO_PRONGS  = SubStates::COMPO_PRONGS + WIDTH;
	static constexpr ShortIndex	ORTHO_REGIONS = SubStates::ORTHO_REGIONS;
	static constexpr ShortIndex	ORTHO_UNITS	  = SubStates::ORTHO_UNITS;

	static constexpr LongIndex	STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex	REGION_COUNT  = RegionList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct OSI_<TInitial, TRemaining...> {
	using Initial			= Wrap<TInitial>;
	using Remaining			= OSI_<TRemaining...>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;
	using RegionList		= Merge<typename Initial::RegionList, typename Remaining::RegionList>;

	static constexpr LongIndex  REVERSE_DEPTH = Max<Initial::REVERSE_DEPTH,  Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr ShortIndex COMPO_REGIONS =		Initial::COMPO_REGIONS + Remaining::COMPO_REGIONS;
	static constexpr LongIndex  COMPO_PRONGS  =		Initial::COMPO_PRONGS  + Remaining::COMPO_PRONGS;
	static constexpr ShortIndex ORTHO_REGIONS =		Initial::ORTHO_REGIONS + Remaining::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS	  =		Initial::ORTHO_UNITS   + Remaining::ORTHO_UNITS;
};

template <typename TInitial>
struct OSI_<TInitial> {
	using Initial			= Wrap<TInitial>;
	using StateList			= typename Initial::StateList;
	using RegionList		= typename Initial::RegionList;

	static constexpr LongIndex  REVERSE_DEPTH = Initial::REVERSE_DEPTH;
	static constexpr ShortIndex COMPO_REGIONS = Initial::COMPO_REGIONS;
	static constexpr LongIndex  COMPO_PRONGS  = Initial::COMPO_PRONGS;
	static constexpr ShortIndex ORTHO_REGIONS = Initial::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS	  = Initial::ORTHO_UNITS;
};

template <typename THead, typename... TSubStates>
struct OI_ final {
	using Head				= THead;
	using HeadInfo			= SI_<Head>;
	using SubStates			= OSI_<TSubStates...>;
	using StateList			= Merge<typename HeadInfo::StateList, typename SubStates::StateList>;
	using RegionList		= Merge<typename HeadInfo::StateList, typename SubStates::RegionList>;

	static constexpr ShortIndex WIDTH		  = sizeof...(TSubStates);
	static constexpr LongIndex  REVERSE_DEPTH = SubStates::REVERSE_DEPTH + 1;
	static constexpr ShortIndex COMPO_REGIONS = SubStates::COMPO_REGIONS;
	static constexpr LongIndex  COMPO_PRONGS  = SubStates::COMPO_PRONGS;
	static constexpr ShortIndex ORTHO_REGIONS = SubStates::ORTHO_REGIONS + 1;
	static constexpr ShortIndex ORTHO_UNITS	  = SubStates::ORTHO_UNITS + (WIDTH + 7) / 8;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NCompoCount,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  LongIndex NTaskCapacity>
struct ArgsT final {
	using Context	 = TContext;

	using Config_	 = TConfig;
	using Rank		 = typename Config_::Rank;
	using Utility	 = typename Config_::Utility;
	using RNG		 = typename Config_::RNG;
	using UP		 = typename Config_::UP;
	using Logger	 = typename Config_::Logger;

	using StateList	 = TStateList;
	using RegionList = TRegionList;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex COMPO_REGIONS = NCompoCount;
	static constexpr ShortIndex ORTHO_REGIONS = NOrthoCount;
	static constexpr ShortIndex ORTHO_UNITS	  = NOrthoUnits;
	static constexpr LongIndex  TASK_CAPACITY = NTaskCapacity;

	HFSM_IF_STRUCTURE(using StructureStateInfos = Array<StructureStateInfo, STATE_COUNT>);
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

template <typename TN_, typename TA_, typename TH_>
struct MaterialT   <TN_, TA_, TH_> {
	using Type = S_<TN_, TA_, TH_>;
};

template <typename TN_, typename TA_, Strategy TG_, 			 typename... TS_>
struct MaterialT   <TN_, TA_, CI_<TG_, void,	   TS_...>> {
	using Type = C_<TN_, TA_,	  TG_, Empty<TA_>, TS_...>;
};

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
struct MaterialT   <TN_, TA_, CI_<TG_, TH_,	TS_...>> {
	using Type = C_<TN_, TA_,	  TG_, TH_,	TS_...>;
};

template <typename TN_, typename TA_,				typename... TS_>
struct MaterialT   <TN_, TA_, OI_<void,		  TS_...>> {
	using Type = O_<TN_, TA_,	  Empty<TA_>, TS_...>;
};

template <typename TN_, typename TA_, typename TH_, typename... TS_>
struct MaterialT   <TN_, TA_, OI_<TH_,		  TS_...>> {
	using Type = O_<TN_, TA_,	  TH_,		  TS_...>;
};

template <typename TN_, typename... TS_>
using Material = typename MaterialT<TN_, TS_...>::Type;

//------------------------------------------------------------------------------

template <typename TConfig,
		  typename TApex>
struct RF_ final {
	using Config_		= TConfig;
	using Context		= typename Config_::Context;
	using Apex			= TApex;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static constexpr LongIndex SUBSTITUTION_LIMIT= Config_::SUBSTITUTION_LIMIT;

	static constexpr LongIndex TASK_CAPACITY	 = Config_::TASK_CAPACITY != INVALID_LONG_INDEX ?
													   Config_::TASK_CAPACITY : Apex::COMPO_PRONGS * 2;

	static constexpr ShortIndex COMPO_REGIONS	 = Apex::COMPO_REGIONS;
	static constexpr ShortIndex ORTHO_REGIONS	 = Apex::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS		 = Apex::ORTHO_UNITS;

	using StateList		= Indexed<typename Apex::StateList>;
	using RegionList	= Indexed<typename Apex::RegionList>;

	using Args			= ArgsT<Context,
								Config_,
								StateList,
								RegionList,
								COMPO_REGIONS,
								ORTHO_REGIONS,
								ORTHO_UNITS,
								TASK_CAPACITY>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Instance		= RW_<Config_, Apex>;

	using Control		= ControlT	   <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Injection		= InjectionT<Args>;

	using State			= Empty<Args>;

	template <typename... TInjections>
	using StateT		= B_<TInjections...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
struct CSubMaterialT;

//template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
//struct CSubMaterialT	 <TN_, TA_, TG_, NI_, TL_<T>> {
//	using Type = Material<TN_, TA_,				  T>;
//};

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
struct CSubMaterialT<TN_, TA_, TG_, NI_, TL_<TS_...>> {
	using Type = CS_<TN_, TA_, TG_, NI_,	 TS_...>;
};

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
using CSubMaterial = typename CSubMaterialT<TN_, TA_, TG_, NI_, TS_...>::Type;

//------------------------------------------------------------------------------

template <typename...>
struct InfoT;

template <typename TN_, typename TA_, typename TH_>
struct InfoT<S_<TN_, TA_, TH_>> {
	using Type = SI_<	  TH_>;
};

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
struct InfoT<C_<TN_, TA_, TG_, TH_, TS_...>> {
	using Type = CI_<	  TG_, TH_, TS_...>;
};

template <typename TN_, typename TA_, typename TH_, typename... TS_>
struct InfoT<O_<TN_, TA_, TH_, TS_...>> {
	using Type = OI_<	  TH_, TS_...>;
};

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
struct InfoT<CS_<TN_, TA_, TG_, NI_, TS_...>> {
	using Type = CSI_<				 TS_...>;
};

////////////////////////////////////////////////////////////////////////////////

}
}
