#pragma once

namespace hfsm {

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

template <typename, typename...>
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
struct WrapForward;

template <typename TH>
struct WrapForward<TH> {
	using Type = _SF<TH>;
};

template <typename TH, typename... TS>
struct WrapForward<_CF<TH, TS...>> {
	using Type = _CF<TH, TS...>;
};

template <typename TH, typename... TS>
struct WrapForward<_OF<TH, TS...>> {
	using Type = _OF<TH, TS...>;
};

//------------------------------------------------------------------------------

template <typename THead>
struct _SF final {
	using Head				= THead;
	using StateList			= _TL<Head>;

	static constexpr LongIndex REVERSE_DEPTH	= 1;
	static constexpr LongIndex DEEP_WIDTH		= 0;
	static constexpr LongIndex STATE_COUNT		= 1;
	static constexpr LongIndex COMPOSITE_COUNT	= 0;
	static constexpr LongIndex ORTHOGONAL_COUNT	= 0;
	static constexpr LongIndex FORK_COUNT		= 0;
	static constexpr LongIndex PRONG_COUNT		= 0;
	static constexpr LongIndex WIDTH			= 1;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct _CSF<TInitial, TRemaining...> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using Remaining			= _CSF<TRemaining...>;
	using StateList			= typename MergeT<typename Initial::StateList, typename Remaining::StateList>::TypeList;

	static constexpr LongIndex REVERSE_DEPTH	= Max<Initial::REVERSE_DEPTH, Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr LongIndex DEEP_WIDTH		= Max<Initial::DEEP_WIDTH,	  Remaining::DEEP_WIDTH>::VALUE;
	static constexpr LongIndex STATE_COUNT		= Initial::STATE_COUNT		+ Remaining::STATE_COUNT;
	static constexpr LongIndex COMPOSITE_COUNT	= Initial::COMPOSITE_COUNT	+ Remaining::COMPOSITE_COUNT;
	static constexpr LongIndex ORTHOGONAL_COUNT	= Initial::ORTHOGONAL_COUNT + Remaining::ORTHOGONAL_COUNT;
	static constexpr LongIndex FORK_COUNT		= Initial::FORK_COUNT		+ Remaining::FORK_COUNT;
	static constexpr LongIndex PRONG_COUNT		= Initial::PRONG_COUNT		+ Remaining::PRONG_COUNT;
};

template <typename TInitial>
struct _CSF<TInitial> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using StateList			= typename Initial::StateList;

	static constexpr LongIndex REVERSE_DEPTH	= Initial::REVERSE_DEPTH;
	static constexpr LongIndex DEEP_WIDTH		= Max<1, Initial::DEEP_WIDTH>::VALUE;
	static constexpr LongIndex STATE_COUNT		= Initial::STATE_COUNT;
	static constexpr LongIndex COMPOSITE_COUNT	= Initial::COMPOSITE_COUNT;
	static constexpr LongIndex ORTHOGONAL_COUNT	= Initial::ORTHOGONAL_COUNT;
	static constexpr LongIndex FORK_COUNT		= Initial::FORK_COUNT;
	static constexpr LongIndex PRONG_COUNT		= Initial::PRONG_COUNT;
};

template <typename THead, typename... TSubStates>
struct _CF final {
	using Head				= THead;
	using State				= _SF<Head>;
	using SubStates			= _CSF<TSubStates...>;
	using StateList			= typename MergeT<typename State::StateList, typename SubStates::StateList>::TypeList;

	static constexpr LongIndex REVERSE_DEPTH	= SubStates::REVERSE_DEPTH + 1;
	static constexpr LongIndex DEEP_WIDTH		= SubStates::DEEP_WIDTH;
	static constexpr LongIndex STATE_COUNT		= State::STATE_COUNT + SubStates::STATE_COUNT;
	static constexpr LongIndex COMPOSITE_COUNT	= SubStates::COMPOSITE_COUNT + 1;
	static constexpr LongIndex ORTHOGONAL_COUNT	= SubStates::ORTHOGONAL_COUNT;
	static constexpr LongIndex FORK_COUNT		= SubStates::FORK_COUNT + 1;
	static constexpr LongIndex PRONG_COUNT		= SubStates::PRONG_COUNT + sizeof...(TSubStates);
	static constexpr LongIndex WIDTH			= sizeof...(TSubStates);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct _OSF<TInitial, TRemaining...> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using Remaining			= _OSF<TRemaining...>;
	using StateList			= typename MergeT<typename Initial::StateList, typename Remaining::StateList>::TypeList;

	static constexpr LongIndex REVERSE_DEPTH	= Max<Initial::REVERSE_DEPTH, Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr LongIndex DEEP_WIDTH		= Initial::DEEP_WIDTH		+ Remaining::DEEP_WIDTH;
	static constexpr LongIndex STATE_COUNT		= Initial::STATE_COUNT		+ Remaining::STATE_COUNT;
	static constexpr LongIndex COMPOSITE_COUNT	= Initial::COMPOSITE_COUNT	+ Remaining::COMPOSITE_COUNT;
	static constexpr LongIndex ORTHOGONAL_COUNT	= Initial::ORTHOGONAL_COUNT + Remaining::ORTHOGONAL_COUNT;
	static constexpr LongIndex FORK_COUNT		= Initial::FORK_COUNT		+ Remaining::FORK_COUNT;
	static constexpr LongIndex PRONG_COUNT		= Initial::PRONG_COUNT		+ Remaining::PRONG_COUNT;
};

template <typename TInitial>
struct _OSF<TInitial> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using StateList			= typename Initial::StateList;

	static constexpr LongIndex REVERSE_DEPTH	= Initial::REVERSE_DEPTH;
	static constexpr LongIndex DEEP_WIDTH		= Initial::DEEP_WIDTH;
	static constexpr LongIndex STATE_COUNT		= Initial::STATE_COUNT;
	static constexpr LongIndex COMPOSITE_COUNT	= Initial::COMPOSITE_COUNT;
	static constexpr LongIndex ORTHOGONAL_COUNT	= Initial::ORTHOGONAL_COUNT;
 	static constexpr LongIndex FORK_COUNT		= Initial::FORK_COUNT;
	static constexpr LongIndex PRONG_COUNT		= Initial::PRONG_COUNT;
};

template <typename THead, typename... TSubStates>
struct _OF final {
	using Head				= THead;
	using State				= _SF<Head>;
	using SubStates			= _OSF<TSubStates...>;
	using StateList			= typename MergeT<typename State::StateList, typename SubStates::StateList>::TypeList;

	static constexpr LongIndex REVERSE_DEPTH	= SubStates::REVERSE_DEPTH + 1;
	static constexpr LongIndex DEEP_WIDTH		= SubStates::DEEP_WIDTH;
	static constexpr LongIndex STATE_COUNT		= State::STATE_COUNT + SubStates::STATE_COUNT;
	static constexpr LongIndex COMPOSITE_COUNT	= SubStates::COMPOSITE_COUNT;
	static constexpr LongIndex ORTHOGONAL_COUNT	= SubStates::ORTHOGONAL_COUNT + 1;
	static constexpr LongIndex FORK_COUNT		= SubStates::FORK_COUNT + 1;
	static constexpr LongIndex PRONG_COUNT		= SubStates::PRONG_COUNT;
	static constexpr LongIndex WIDTH			= sizeof...(TSubStates);
};

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  LongIndex NCompositeCount,
		  LongIndex NOrthogonalCount,
		  typename TPayloadList,
		  LongIndex NPlanCapacity>
struct ArgsT final {
	using Context			= TContext;
	using Config			= TConfig;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;

	static constexpr LongIndex STATE_COUNT		= StateList::SIZE;
	static constexpr LongIndex COMPOSITE_COUNT	= NCompositeCount;
	static constexpr LongIndex ORTHOGONAL_COUNT	= NOrthogonalCount;
	static constexpr LongIndex PLAN_CAPACITY	= NPlanCapacity;
};

//------------------------------------------------------------------------------

template <StateID, typename, typename>
struct _S;

template <StateID, ShortIndex, ShortIndex, typename, typename, typename...>
struct _C;

template <StateID, ShortIndex, ShortIndex, typename, typename, typename...>
struct _Q;

template <StateID, ShortIndex, ShortIndex, typename, typename, typename...>
struct _O;

template <typename, typename, typename, typename>
class _R;

//------------------------------------------------------------------------------

template <StateID, ShortIndex, ShortIndex, typename...>
struct WrapMaterial;

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH>
struct WrapMaterial<NS, NC, NO, TA, TH> {
	using Type = _S<NS,			TA, TH>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA,			  typename... TS>
struct WrapMaterial<NS, NC, NO, TA, _CF<void, TS...>> {
	using Type = _C<NS, NC, NO, TA, Empty<TA>, TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
struct WrapMaterial<NS, NC, NO, TA, _CF<TH,	 TS...>> {
	using Type = _Q<NS, NC, NO, TA, TH,		  TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA,			  typename... TS>
struct WrapMaterial<NS, NC, NO, TA, _OF<void, TS...>> {
	using Type = _O<NS, NC, NO, TA, Empty<TA>, TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
struct WrapMaterial<NS, NC, NO, TA, _OF<TH,	 TS...>> {
	using Type = _O<NS, NC, NO, TA, TH,		  TS...>;
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TPayloadList,
		  typename TApex>
struct _RF final {
	using Context			= TContext;
	using Config			= TConfig;
	using PayloadList		= TPayloadList;
	using Forward			= TApex;

	static constexpr LongIndex MAX_PLAN_TASKS	 = Config::MAX_PLAN_TASKS;
	static constexpr LongIndex MAX_SUBSTITUTIONS = Config::MAX_SUBSTITUTIONS;

	static constexpr LongIndex PLAN_CAPACITY	 = Config::MAX_PLAN_TASKS != INVALID_LONG_INDEX ?
													   Config::MAX_PLAN_TASKS : Forward::PRONG_COUNT * 2;

	using Instance			= _R<Context, Config, PayloadList, Forward>;

	static constexpr LongIndex COMPOSITE_COUNT	 = Forward::COMPOSITE_COUNT;
	static constexpr LongIndex ORTHOGONAL_COUNT	 = Forward::ORTHOGONAL_COUNT;

	using StateList			= typename Forward::StateList;
	using Args				= ArgsT<Context, Config, StateList, COMPOSITE_COUNT, ORTHOGONAL_COUNT, PayloadList, PLAN_CAPACITY>;

	using PlanControl		= PlanControlT		<Args>;
	using TransitionControl	= TransitionControlT<Args>;
	using FullControl		= FullControlT		<Args>;

	using Bare				= ::hfsm::detail::Bare <Args>;
	using State				= ::hfsm::detail::Empty<Args>;

	template <typename... TInjections>
	using BaseT = _B<TInjections...>;

	template <typename T>
	static constexpr LongIndex stateId() {
		return StateList::template index<T>();
	}

	template <typename T>
	static constexpr bool contains() {
		return StateList::template index<T>() != INVALID_LONG_INDEX;
	}
};

////////////////////////////////////////////////////////////////////////////////

}
}
