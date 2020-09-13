namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template<Long N>
struct IndexConstant {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<Long... Ns>
struct IndexSequence {
	using Type = IndexSequence<Ns...>;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<typename, typename>
struct MakeIndexSequenceT {};

template<Long N, Long... Ns>
struct MakeIndexSequenceT<IndexConstant<N>,
						  IndexSequence<Ns...>>
	: MakeIndexSequenceT<IndexConstant<N - 1>,
						 IndexSequence<N - 1, Ns...>>
{};

template<Long... Ns>
struct MakeIndexSequenceT<IndexConstant<0>,
							  IndexSequence<Ns...>>
	: IndexSequence<Ns...>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<Long N>
using MakeIndexSequence = typename MakeIndexSequenceT<IndexConstant<N>,
													  IndexSequence<>>::Type;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<class... Ts>
using IndexSequenceFor = MakeIndexSequence<sizeof...(Ts)>;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct ITL_EntryT {};

template <typename T, Long N>
struct ITL_EntryN
	: ITL_EntryT<T>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
struct ITL_Impl;

template <Long... Ns, typename... Ts>
struct ITL_Impl<IndexSequence<Ns...>, Ts...>
	: ITL_EntryN<Ts, Ns>...
{
	template <typename T, Long N>
	static constexpr Long select(ITL_EntryN<T, N>) { return (Long) N; }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename... Ts>
struct ITL_
	: private ITL_Impl<IndexSequenceFor<Ts...>, Ts...>
{
	using Base = ITL_Impl<IndexSequenceFor<Ts...>, Ts...>;

	static constexpr Long SIZE = sizeof...(Ts);

	template <typename T>
	static constexpr bool contains() { return std::is_base_of<ITL_EntryT<T>, ITL_>::value; }

	template <typename T>
	static constexpr
	typename std::enable_if< std::is_base_of<ITL_EntryT<T>, ITL_>::value, Long>::type
	index() {
		return Base::template select<T>(ITL_{});
	}

	template <typename T>
	static constexpr
	typename std::enable_if<!std::is_base_of<ITL_EntryT<T>, ITL_>::value, Long>::type
	index() {
		return INVALID_LONG;
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

template <Long, Long, typename...>
struct LesserT;

template <Long H, Long I, typename TFirst, typename... TRest>
struct LesserT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<(I < H),
										   Prepend<TFirst, typename LesserT<H, I + 1, TRest...>::Type>,
										   typename LesserT<H, I + 1, TRest...>::Type>::type;
};

template <Long H, Long I>
struct LesserT<H, I> {
	using Type = ITL_<>;
};

template <typename... Ts>
using SplitL = typename LesserT<sizeof...(Ts) / 2, 0, Ts...>::Type;

//------------------------------------------------------------------------------

template <Long, Long, typename...>
struct GreaterT;

template <Long H, Long I, typename TFirst, typename... TRest>
struct GreaterT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<(I < H),
										   typename GreaterT<H, I + 1, TRest...>::Type,
										   ITL_<TFirst, TRest...>>::type;
};

template <Long H, Long I>
struct GreaterT<H, I> {
	using Type = ITL_<>;
};

template <typename... Ts>
using SplitR = typename GreaterT<sizeof...(Ts) / 2, 0, Ts...>::Type;

////////////////////////////////////////////////////////////////////////////////

}
}
