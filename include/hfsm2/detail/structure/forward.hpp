#pragma once

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename>
struct _SF;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <Strategy, typename, typename...>
struct _CF;

template <typename...>
struct _CSF;

template <typename TInitial, typename... TRemaining>
struct _CSF<TInitial, TRemaining...>;

template <typename TInitial>
struct _CSF<TInitial>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename, typename...>
struct _OF;

template <typename...>
struct _OSF;

template <typename TInitial, typename... TRemaining>
struct _OSF<TInitial, TRemaining...>;

template <typename TInitial>
struct _OSF<TInitial>;

//------------------------------------------------------------------------------

template <typename...>
struct WrapT;

template <typename TH>
struct WrapT<TH> {
	using Type = _SF<TH>;
};

template <Strategy TG, typename TH, typename... TS>
struct WrapT<	 _CF<TG, TH, TS...>> {
	using Type = _CF<TG, TH, TS...>;
};

template <typename... TS>
struct WrapT<	 _OF<TS...>> {
	using Type = _OF<TS...>;
};

template <typename... TS>
using Wrap = typename WrapT<TS...>::Type;

//------------------------------------------------------------------------------

template <typename THead>
struct _SF final {
	using Head				= THead;
	using StateList			= _TL<Head>;
	using RegionList		= _TL<>;

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
struct _CSF<TInitial, TRemaining...> {
	using Initial			= Wrap<TInitial>;
	using Remaining			= _CSF<TRemaining...>;
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
struct _CSF<TInitial> {
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
struct _CF final {
	static constexpr Strategy STRATEGY = TStrategy;

	using Head				= THead;
	using State				= _SF<Head>;
	using SubStates			= _CSF<TSubStates...>;
	using StateList			= Merge<typename State::StateList, typename SubStates::StateList>;
	using RegionList		= Merge<typename State::StateList, typename SubStates::RegionList>;

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
struct _OSF<TInitial, TRemaining...> {
	using Initial			= Wrap<TInitial>;
	using Remaining			= _OSF<TRemaining...>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;
	using RegionList		= Merge<typename Initial::RegionList, typename Remaining::RegionList>;

	static constexpr LongIndex  REVERSE_DEPTH = Max<Initial::REVERSE_DEPTH,  Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr ShortIndex COMPO_REGIONS =		Initial::COMPO_REGIONS + Remaining::COMPO_REGIONS;
	static constexpr LongIndex  COMPO_PRONGS  =		Initial::COMPO_PRONGS  + Remaining::COMPO_PRONGS;
	static constexpr ShortIndex ORTHO_REGIONS =		Initial::ORTHO_REGIONS + Remaining::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS	  =		Initial::ORTHO_UNITS   + Remaining::ORTHO_UNITS;
};

template <typename TInitial>
struct _OSF<TInitial> {
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
struct _OF final {
	using Head				= THead;
	using State				= _SF<Head>;
	using SubStates			= _OSF<TSubStates...>;
	using StateList			= Merge<typename State::StateList, typename SubStates::StateList>;
	using RegionList		= Merge<typename State::StateList, typename SubStates::RegionList>;

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
		  typename TPayload,
		  LongIndex NTaskCapacity>
struct ArgsT final {
	using Context	 = TContext;

	using Config	 = TConfig;
	using Utility	 = typename Config::Utility;
	using UP		 = typename Config::UP;
	using Logger	 = typename Config::Logger;

	using StateList	 = TStateList;
	using RegionList = TRegionList;
	using Payload	 = TPayload;

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
struct _I {
	static constexpr StateID	STATE_ID	 = NStateID;
	static constexpr ShortIndex COMPO_INDEX	 = NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	 = NOrthoIndex;
	static constexpr ShortIndex ORTHO_UNIT	 = NOrthoUnit;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename, typename, typename>
struct _S;

template <typename, typename, Strategy, typename, typename...>
struct _C;

template <typename, typename, Strategy, ShortIndex, typename...>
struct _CS;

template <typename, typename, typename, typename...>
struct _O;

template <typename, typename, typename, typename>
class _RW;

//------------------------------------------------------------------------------

template <typename, typename...>
struct MaterialT;

template <typename TN, typename TA, typename TH>
struct MaterialT   <TN, TA, TH> {
	using Type = _S<TN, TA, TH>;
};

template <typename TN, typename TA, Strategy TG, 			 typename... TS>
struct MaterialT   <TN, TA, _CF<TG, void,	   TS...>> {
	using Type = _C<TN, TA,		TG, Empty<TA>, TS...>;
};

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
struct MaterialT   <TN, TA, _CF<TG, TH,	TS...>> {
	using Type = _C<TN, TA,		TG, TH,	TS...>;
};

template <typename TN, typename TA,			  typename... TS>
struct MaterialT   <TN, TA, _OF<void,	   TS...>> {
	using Type = _O<TN, TA,		Empty<TA>, TS...>;
};

template <typename TN, typename TA, typename TH, typename... TS>
struct MaterialT   <TN, TA, _OF<TH,	 TS...>> {
	using Type = _O<TN, TA,		TH,	 TS...>;
};

template <typename TN, typename... TS>
using Material = typename MaterialT<TN, TS...>::Type;

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TPayload,
		  typename TApex>
struct _RF final {
	using Context		= TContext;
	using Config		= TConfig;
	using Payload		= TPayload;
	using Apex			= TApex;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static constexpr LongIndex MAX_PLAN_TASKS	 = Config::MAX_PLAN_TASKS;
	static constexpr LongIndex MAX_SUBSTITUTIONS = Config::MAX_SUBSTITUTIONS;

	static constexpr LongIndex TASK_CAPACITY	 = Config::MAX_PLAN_TASKS != INVALID_LONG_INDEX ?
													   Config::MAX_PLAN_TASKS : Apex::COMPO_PRONGS * 2;

	static constexpr ShortIndex COMPO_REGIONS	 = Apex::COMPO_REGIONS;
	static constexpr ShortIndex ORTHO_REGIONS	 = Apex::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS		 = Apex::ORTHO_UNITS;

	using StateList		= Indexed<typename Apex::StateList>;
	using RegionList	= Indexed<typename Apex::RegionList>;

	using Args			= ArgsT<Context,
								Config,
								StateList,
								RegionList,
								COMPO_REGIONS,
								ORTHO_REGIONS,
								ORTHO_UNITS,
								Payload,
								TASK_CAPACITY>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Instance		= _RW<Context, Config, Payload, Apex>;

	using Control		= ControlT	   <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Injection		= InjectionT<Args>;

	using State			= Empty<Args>;

	template <typename... TInjections>
	using StateT		= _B<TInjections...>;

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

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
struct CSubMaterialT;

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
struct CSubMaterialT	 <TN, TA, TG, NI, _TL<T>> {
	using Type = Material<TN, TA,			  T>;
};

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
struct CSubMaterialT<TN, TA, TG, NI, _TL<TS...>> {
	using Type = _CS<TN, TA, TG, NI,	 TS...>;
};

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
using CSubMaterial = typename CSubMaterialT<TN, TA, TG, NI, TS...>::Type;

//------------------------------------------------------------------------------

template <typename...>
struct ForwardT;

template <typename TN, typename TA, typename TH>
struct ForwardT<  _S<TN, TA, TH>> {
	using Type = _SF<		 TH>;
};

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
struct ForwardT<_C<TN, TA, TG, TH, TS...>> {
	using Type = _CF<	   TG, TH, TS...>;
};

template <typename TN, typename TA, typename TH, typename... TS>
struct ForwardT<_O<TN, TA, TH, TS...>> {
	using Type = _OF<			   TH, TS...>;
};

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
struct ForwardT<_CS<TN, TA, TG, NI, TS...>> {
	using Type = _CSF<				TS...>;
};

////////////////////////////////////////////////////////////////////////////////

}
}
