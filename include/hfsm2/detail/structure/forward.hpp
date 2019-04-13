#pragma once

namespace hfsm2 {

//------------------------------------------------------------------------------

template <LongIndex NMaxPlanTasks = INVALID_LONG_INDEX,
		  LongIndex NMaxSubstitutions = 4>
struct Config {
	static constexpr LongIndex MAX_PLAN_TASKS	 = NMaxPlanTasks;
	static constexpr LongIndex MAX_SUBSTITUTIONS = NMaxSubstitutions;
};

namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename>
struct _SF;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
struct _CSF;

template <typename TInitial, typename... TRemaining>
struct _CSF<TInitial, TRemaining...>;

template <typename TInitial>
struct _CSF<TInitial>;

template <typename, RegionStrategy, typename...>
struct _CF;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
struct _OSF;

template <typename TInitial, typename... TRemaining>
struct _OSF<TInitial, TRemaining...>;

template <typename TInitial>
struct _OSF<TInitial>;

template <typename, typename...>
struct _OF;

//------------------------------------------------------------------------------

template <typename...>
struct WrapT;

template <typename TH>
struct WrapT<TH> {
	using Type = _SF<TH>;
};

template <typename TH, RegionStrategy TG, typename... TS>
struct WrapT<	 _CF<TH, TG, TS...>> {
	using Type = _CF<TH, TG, TS...>;
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
	static constexpr ShortIndex COMPO_COUNT	  = 0;
	static constexpr ShortIndex ORTHO_COUNT	  = 0;
	static constexpr ShortIndex ORTHO_UNITS	  = 0;
	static constexpr LongIndex  PRONG_COUNT	  = 0;

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

	static constexpr LongIndex  REVERSE_DEPTH = Max<Initial::REVERSE_DEPTH, Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr ShortIndex COMPO_COUNT	  =		Initial::COMPO_COUNT +	Remaining::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT	  =		Initial::ORTHO_COUNT +	Remaining::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS	  =		Initial::ORTHO_UNITS +	Remaining::ORTHO_UNITS;
	static constexpr LongIndex  PRONG_COUNT	  =		Initial::PRONG_COUNT +	Remaining::PRONG_COUNT;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
};

template <typename TInitial>
struct _CSF<TInitial> {
	using Initial			= Wrap<TInitial>;
	using StateList			= typename Initial::StateList;
	using RegionList		= typename Initial::RegionList;

	static constexpr LongIndex  REVERSE_DEPTH = Initial::REVERSE_DEPTH;
	static constexpr ShortIndex COMPO_COUNT	  = Initial::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT	  = Initial::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS	  = Initial::ORTHO_UNITS;
	static constexpr LongIndex  PRONG_COUNT	  = Initial::PRONG_COUNT;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
};

template <typename THead, RegionStrategy TStrategy, typename... TSubStates>
struct _CF final {
	static constexpr RegionStrategy STRATEGY = TStrategy;

	using Head				= THead;
	using State				= _SF<Head>;
	using SubStates			= _CSF<TSubStates...>;
	using StateList			= Merge<typename State::StateList, typename SubStates::StateList>;
	using RegionList		= Merge<typename State::StateList, typename SubStates::RegionList>;

	static constexpr ShortIndex	WIDTH		  = sizeof...(TSubStates);
	static constexpr LongIndex	REVERSE_DEPTH = SubStates::REVERSE_DEPTH + 1;
	static constexpr ShortIndex	COMPO_COUNT	  = SubStates::COMPO_COUNT + 1;
	static constexpr ShortIndex	ORTHO_COUNT	  = SubStates::ORTHO_COUNT;
	static constexpr ShortIndex	ORTHO_UNITS	  = SubStates::ORTHO_UNITS;
	static constexpr LongIndex	PRONG_COUNT	  = WIDTH + SubStates::PRONG_COUNT;

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

	static constexpr LongIndex  REVERSE_DEPTH = Max<Initial::REVERSE_DEPTH, Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr ShortIndex COMPO_COUNT	  =		Initial::COMPO_COUNT +	Remaining::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT	  =		Initial::ORTHO_COUNT +	Remaining::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS	  =		Initial::ORTHO_UNITS +	Remaining::ORTHO_UNITS;
	static constexpr LongIndex  PRONG_COUNT	  =		Initial::PRONG_COUNT +	Remaining::PRONG_COUNT;
};

template <typename TInitial>
struct _OSF<TInitial> {
	using Initial			= Wrap<TInitial>;
	using StateList			= typename Initial::StateList;
	using RegionList		= typename Initial::RegionList;

	static constexpr LongIndex  REVERSE_DEPTH = Initial::REVERSE_DEPTH;
	static constexpr ShortIndex COMPO_COUNT	  = Initial::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT	  = Initial::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS	  = Initial::ORTHO_UNITS;
	static constexpr LongIndex  PRONG_COUNT	  = Initial::PRONG_COUNT;
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
	static constexpr ShortIndex COMPO_COUNT	  = SubStates::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT	  = SubStates::ORTHO_COUNT + 1;
	static constexpr ShortIndex ORTHO_UNITS	  = SubStates::ORTHO_UNITS + (WIDTH + 7) / 8;
	static constexpr LongIndex  PRONG_COUNT	  = SubStates::PRONG_COUNT;

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
		  typename TPayloadList,
		  LongIndex NTaskCapacity>
struct ArgsT final {
	using Context			= TContext;
	using Config			= TConfig;
	using StateList			= TStateList;
	using RegionList		= TRegionList;
	using PayloadList		= TPayloadList;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex COMPO_COUNT	  = NCompoCount;
	static constexpr ShortIndex ORTHO_COUNT	  = NOrthoCount;
	static constexpr ShortIndex ORTHO_UNITS	  = NOrthoUnits;
	static constexpr LongIndex  TASK_CAPACITY = NTaskCapacity;
};

//------------------------------------------------------------------------------

template <StateID, typename, typename>
struct _S;

template <StateID, ShortIndex, ShortIndex, typename, typename, RegionStrategy, typename...>
struct _C;

template <StateID, ShortIndex, ShortIndex, typename, ShortIndex, typename...>
struct _CS;

template <StateID, ShortIndex, ShortIndex, typename, typename, typename...>
struct _O;

template <typename, typename, typename, typename>
class _R;

//------------------------------------------------------------------------------

template <StateID, ShortIndex, ShortIndex, typename...>
struct MaterialT;

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH>
struct MaterialT   <NS, NC, NO, TA, TH> {
	using Type = _S<NS,			TA, TH>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA,			  RegionStrategy TG, typename... TS>
struct MaterialT   <NS, NC, NO, TA,  _CF<void, TG, TS...>> {
	using Type = _C<NS, NC, NO, TA, Empty<TA>, TG, TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, RegionStrategy TG, typename... TS>
struct MaterialT   <NS, NC, NO, TA, _CF<TH,	TG, TS...>> {
	using Type = _C<NS, NC, NO, TA,		TH,	TG, TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA,			  typename... TS>
struct MaterialT   <NS, NC, NO, TA, _OF<void,  TS...>> {
	using Type = _O<NS, NC, NO, TA, Empty<TA>, TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
struct MaterialT   <NS, NC, NO, TA, _OF<TH,	 TS...>> {
	using Type = _O<NS, NC, NO, TA,		TH,	 TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename... TS>
using Material = typename MaterialT<NS, NC, NO, TS...>::Type;

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TPayloadList,
		  typename TApex>
struct _RF final {
	using Context			= TContext;
	using Config			= TConfig;
	using PayloadList		= TPayloadList;
	using Apex				= TApex;

	static constexpr LongIndex MAX_PLAN_TASKS	 = Config::MAX_PLAN_TASKS;
	static constexpr LongIndex MAX_SUBSTITUTIONS = Config::MAX_SUBSTITUTIONS;

	static constexpr LongIndex TASK_CAPACITY	 = Config::MAX_PLAN_TASKS != INVALID_LONG_INDEX ?
													   Config::MAX_PLAN_TASKS : Apex::PRONG_COUNT * 2;

	using Instance			= _R<Context, Config, PayloadList, Apex>;

	static constexpr ShortIndex COMPO_COUNT		 = Apex::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT		 = Apex::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS		 = Apex::ORTHO_UNITS;

	using StateList			= Indexed<typename Apex::StateList>;
	using RegionList		= Indexed<typename Apex::RegionList>;

	using Args				= ArgsT<Context,
									Config,
									StateList,
									RegionList,
									COMPO_COUNT,
									ORTHO_COUNT,
									ORTHO_UNITS,
									PayloadList,
									TASK_CAPACITY>;

	using Control			= ControlT	   <Args>;
	using FullControl		= FullControlT <Args>;
	using GuardControl		= GuardControlT<Args>;

	using Bare				= BareT<Args>;
	using State				= Empty<Args>;

	template <typename... TInjections>
	using BaseT				= _B<TInjections...>;

	template <typename T>
	static constexpr LongIndex stateId() {
		return StateList::template index<T>();
	}

	template <typename T>
	static constexpr bool contains() {
		return StateList::template index<T>() != INVALID_LONG_INDEX;
	}

	template <typename T>
	static constexpr LongIndex regionId() {
		return RegionList::template index<T>();
	}
};

////////////////////////////////////////////////////////////////////////////////

template <StateID, ShortIndex, ShortIndex, typename, ShortIndex, typename...>
struct CSubMaterialT;

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename T>
struct CSubMaterialT	 <NS, NC, NO, TA, NI, _TL<T>> {
	using Type = Material<NS, NC, NO, TA,		  T>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
struct CSubMaterialT<NS, NC, NO, TA, NI, _TL<TS...>> {
	using Type = _CS<NS, NC, NO, TA, NI,	 TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
using CSubMaterial = typename CSubMaterialT<NS, NC, NO, TA, NI, TS...>::Type;

//------------------------------------------------------------------------------

template <typename...>
struct ForwardT;

template <StateID NS, typename TA, typename TH>
struct ForwardT<  _S<NS, TA, TH>> {
	using Type = _SF<		 TH>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, RegionStrategy TG, typename... TS>
struct ForwardT<_C<NS, NC, NO, TA, TH, TG, TS...>> {
	using Type = _CF<			   TH, TG, TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
struct ForwardT<_O<NS, NC, NO, TA, TH, TS...>> {
	using Type = _OF<			   TH, TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
struct ForwardT<_CS<NS, NC, NO, TA, NI, TS...>> {
	using Type = _CSF<					TS...>;
};

////////////////////////////////////////////////////////////////////////////////

}
}
