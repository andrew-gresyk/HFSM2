#pragma once

namespace hfsm {
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
	using StateList			= TypeListT<Head>;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct _CSF<TInitial, TRemaining...> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using Remaining			= _CSF<TRemaining...>;
	using StateList			= typename MergeT<typename Initial::StateList, typename Remaining::StateList>::TypeList;
};

template <typename TInitial>
struct _CSF<TInitial> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using StateList			= typename Initial::StateList;
};

template <typename THead, typename... TSubStates>
struct _CF final {
	using Head				= THead;
	using State				= _SF<Head>;
	using SubStates			= _CSF<TSubStates...>;
	using StateList			= typename MergeT<typename State::StateList, typename SubStates::StateList>::TypeList;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct _OSF<TInitial, TRemaining...> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using Remaining			= _OSF<TRemaining...>;
	using StateList			= typename MergeT<typename Initial::StateList, typename Remaining::StateList>::TypeList;
};

template <typename TInitial>
struct _OSF<TInitial> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using StateList			= typename Initial::StateList;
};

template <typename THead,
		  typename... TSubStates>
struct _OF final {
	using Head				= THead;
	using State				= _SF<Head>;
	using SubStates			= _OSF<TSubStates...>;
	using StateList			= typename MergeT<typename State::StateList, typename SubStates::StateList>::TypeList;
};

////////////////////////////////////////////////////////////////////////////////

template <StateID, typename, typename, typename, typename>
struct _S;

template <StateID, typename, typename, typename, typename, typename...>
struct _C;

template <StateID, typename, typename, typename, typename, typename...>
struct _O;

template <typename, typename, ShortIndex, typename>
class _R;

//------------------------------------------------------------------------------

template <StateID, typename...>
struct WrapMaterial;

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
struct WrapMaterial<TID, TC, TSL, TPL, TH> {
	using Type = _S<TID, TC, TSL, TPL, TH>;
};

template <StateID TID, typename TC, typename TSL, typename TPL,				 typename... TS>
struct WrapMaterial<TID, TC, TSL, TPL, _CF<void, TS...>> {
	using Type = _C<TID, TC, TSL, TPL, Base<TC, TSL, TPL>, TS...>;
};

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
struct WrapMaterial<TID, TC, TSL, TPL, _CF<TH, TS...>> {
	using Type = _C<TID, TC, TSL, TPL, TH, TS...>;
};

template <StateID TID, typename TC, typename TSL, typename TPL,				 typename... TS>
struct WrapMaterial<TID, TC, TSL, TPL, _OF<void, TS...>> {
	using Type = _O<TID, TC, TSL, TPL, Base<TC, TSL, TPL>, TS...>;
};

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
struct WrapMaterial<TID, TC, TSL, TPL, _OF<TH, TS...>> {
	using Type = _O<TID, TC, TSL, TPL, TH, TS...>;
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TPayloadList,
		  ShortIndex TMaxSubstitutions,
		  typename TApex>
struct _RF final {
	static constexpr ShortIndex MAX_SUBSTITUTIONS = TMaxSubstitutions;

	using Context			= TContext;
	using PayloadList		= TPayloadList;

	using Forward			= TApex;
	using StateList			= typename Forward::StateList;
	using TransitionControl	= TransitionControlT<Context, StateList, PayloadList>;

	using Instance			= _R<Context, PayloadList, MAX_SUBSTITUTIONS, Forward>;

	using Bare = ::hfsm::detail::Bare<Context, StateList, PayloadList>;
	using Base = ::hfsm::detail::Base<Context, StateList, PayloadList>;

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
