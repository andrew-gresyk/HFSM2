namespace hfsm2 {
namespace detail {

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
struct MakeIndexSequenceT {};

template<LongIndex N, LongIndex... Ns>
struct MakeIndexSequenceT<IndexConstant<N>,
						  IndexSequence<Ns...>>
	: MakeIndexSequenceT<IndexConstant<N - 1>,
						 IndexSequence<N - 1, Ns...>>
{};

template<LongIndex... Ns>
struct MakeIndexSequenceT<IndexConstant<0>,
							  IndexSequence<Ns...>>
	: IndexSequence<Ns...>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<LongIndex N>
using MakeIndexSequence = typename MakeIndexSequenceT<IndexConstant<N>,
													  IndexSequence<>>::Type;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<class... Ts>
using IndexSequenceFor = MakeIndexSequence<sizeof...(Ts)>;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct ITL_EntryT {};

template <typename T, LongIndex N>
struct ITL_EntryN
	: ITL_EntryT<T>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
struct ITL_Impl;

template <LongIndex... Ns, typename... Ts>
struct ITL_Impl<IndexSequence<Ns...>, Ts...>
	: ITL_EntryN<Ts, Ns>...
{
	template <typename T, LongIndex N>
	static constexpr LongIndex select(ITL_EntryN<T, N>) { return (LongIndex) N; }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename... Ts>
struct ITL_
	: private ITL_Impl<IndexSequenceFor<Ts...>, Ts...>
{
	using Base = ITL_Impl<IndexSequenceFor<Ts...>, Ts...>;

	static constexpr LongIndex SIZE = sizeof...(Ts);

	template <typename T>
	static constexpr bool contains() { return std::is_base_of<ITL_EntryT<T>, ITL_>::value; }

	template <typename T>
	static constexpr
	typename std::enable_if< std::is_base_of<ITL_EntryT<T>, ITL_>::value, LongIndex>::type
	index() {
		return Base::template select<T>(ITL_{});
	}

	template <typename T>
	static constexpr
	typename std::enable_if<!std::is_base_of<ITL_EntryT<T>, ITL_>::value, LongIndex>::type
	index() {
		return INVALID_LONG_INDEX;
	}
};

////////////////////////////////////////////////////////////////////////////////

template <typename...>
struct PrependT;

template <typename T, typename... Ts>
struct PrependT<T, ITL_<Ts...>> {
	using Type = ITL_<T, Ts...>;
};

template <typename... Ts>
using Prepend = typename PrependT<Ts...>::Type;

//------------------------------------------------------------------------------

template <typename...>
struct MergeT;

template <typename... Ts1, typename... Ts2>
struct MergeT<ITL_<Ts1...>, ITL_<Ts2...>> {
	using Type = ITL_<Ts1..., Ts2...>;
};

template <typename... Ts>
using Merge = typename MergeT<Ts...>::Type;

//------------------------------------------------------------------------------

template <LongIndex, LongIndex, typename...>
struct LesserT;

template <LongIndex H, LongIndex I, typename TFirst, typename... TRest>
struct LesserT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<(I < H),
										   Prepend<TFirst, typename LesserT<H, I + 1, TRest...>::Type>,
										   typename LesserT<H, I + 1, TRest...>::Type>::type;
};

template <LongIndex H, LongIndex I>
struct LesserT<H, I> {
	using Type = ITL_<>;
};

template <typename... Ts>
using SplitL = typename LesserT<sizeof...(Ts) / 2, 0, Ts...>::Type;

//------------------------------------------------------------------------------

template <LongIndex, LongIndex, typename...>
struct GreaterT;

template <LongIndex H, LongIndex I, typename TFirst, typename... TRest>
struct GreaterT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<(I < H),
										   typename GreaterT<H, I + 1, TRest...>::Type,
										   ITL_<TFirst, TRest...>>::type;
};

template <LongIndex H, LongIndex I>
struct GreaterT<H, I> {
	using Type = ITL_<>;
};

template <typename... Ts>
using SplitR = typename GreaterT<sizeof...(Ts) / 2, 0, Ts...>::Type;

////////////////////////////////////////////////////////////////////////////////

}
}
