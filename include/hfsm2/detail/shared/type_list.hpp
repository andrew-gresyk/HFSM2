#pragma once

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename... Ts>
struct _TL {
	static constexpr LongIndex SIZE = sizeof...(Ts);
};

//------------------------------------------------------------------------------

template <typename...>
struct PrependT;

template <typename T, typename... Ts>
struct PrependT<T, _TL<Ts...>> {
	using Type = _TL<T, Ts...>;
};

template <typename... Ts>
using Prepend = typename PrependT<Ts...>::Type;

//------------------------------------------------------------------------------

template <size_t, size_t, typename...>
struct LesserT;

template <size_t H, size_t I, typename TFirst, typename... TRest>
struct LesserT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<(I < H),
										   Prepend<TFirst, typename LesserT<H, I + 1, TRest...>::Type>,
										   typename LesserT<H, I + 1, TRest...>::Type>::type;
};

template <size_t H, size_t I>
struct LesserT<H, I> {
	using Type = _TL<>;
};

template <typename... Ts>
using SplitL = typename LesserT<sizeof...(Ts) / 2, 0, Ts...>::Type;

//------------------------------------------------------------------------------

template <size_t, size_t, typename...>
struct GreaterT;

template <size_t H, size_t I, typename TFirst, typename... TRest>
struct GreaterT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<(I < H),
										   typename GreaterT<H, I + 1, TRest...>::Type,
										   _TL<TFirst, TRest...>>::type;
};

template <size_t H, size_t I>
struct GreaterT<H, I> {
	using Type = _TL<>;
};

template <typename... Ts>
using SplitR = typename GreaterT<sizeof...(Ts) / 2, 0, Ts...>::Type;

////////////////////////////////////////////////////////////////////////////////

template<LongIndex N>
struct IndexConstant {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<LongIndex... Ns>
struct IndexSequence {
	using Type = IndexSequence<Ns...>;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<typename, typename>
struct MakeIndexSequence_Impl {};

template<LongIndex N, LongIndex... Ns>
struct MakeIndexSequence_Impl<IndexConstant<N>,
							  IndexSequence<Ns...>>
	: MakeIndexSequence_Impl<IndexConstant<N - 1>,
							 IndexSequence<N - 1, Ns...>>
{};

template<LongIndex... Ns>
struct MakeIndexSequence_Impl<IndexConstant<0>,
							  IndexSequence<Ns...>>
	: IndexSequence<Ns...>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<LongIndex N>
using MakeIndexSequence = typename MakeIndexSequence_Impl<IndexConstant<N>,
														  IndexSequence<>>::Type;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<class... Ts>
using IndexSequenceFor = MakeIndexSequence<sizeof...(Ts)>;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct IndexedTypeList_EntryT {};

template <typename T, std::size_t N>
struct IndexedTypeList_EntryN
	: IndexedTypeList_EntryT<T>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
struct IndexedTypeList_Impl;

template <LongIndex... Ns, typename... Ts>
struct IndexedTypeList_Impl<IndexSequence<Ns...>, Ts...>
	: IndexedTypeList_EntryN<Ts, Ns>...
{
	template <typename T, std::size_t N>
	static constexpr LongIndex select(IndexedTypeList_EntryN<T, N>) { return (LongIndex) N; }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename... Ts>
struct _ITL
	: private IndexedTypeList_Impl<IndexSequenceFor<Ts...>, Ts...>
{
	using Base = IndexedTypeList_Impl<IndexSequenceFor<Ts...>, Ts...>;

	static constexpr LongIndex SIZE = sizeof...(Ts);

	template <typename T>
	static constexpr bool contains() { return std::is_base_of<IndexedTypeList_EntryT<T>, _ITL>::value; }

	template <typename T>
	static constexpr
	typename std::enable_if< std::is_base_of<IndexedTypeList_EntryT<T>, _ITL>::value, LongIndex>::type
	index() {
		return Base::template select<T>(_ITL{});
	}

	template <typename T>
	static constexpr
	typename std::enable_if<!std::is_base_of<IndexedTypeList_EntryT<T>, _ITL>::value, LongIndex>::type
	index() {
		return INVALID_LONG_INDEX;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
struct IndexedT;

template <typename... Ts>
struct IndexedT<_TL<Ts...>> {
	using Type = _ITL<Ts...>;
};

template <typename T>
using Indexed = typename IndexedT<T>::Type;

//------------------------------------------------------------------------------

template <typename...>
struct MergeT;

template <typename... Ts1, typename... Ts2>
struct MergeT<_TL<Ts1...>, _TL<Ts2...>> {
	using TypeList = _TL<Ts1..., Ts2...>;
};

template <typename... Ts>
using Merge = typename MergeT<Ts...>::TypeList;

////////////////////////////////////////////////////////////////////////////////

}
}
