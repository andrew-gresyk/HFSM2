namespace hfsm2 {

////////////////////////////////////////////////////////////////////////////////

#if 0

namespace detail {

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

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename... Ts>
struct TypeList
	: private	 detail::ITL_Impl<detail::IndexSequenceFor<Ts...>, Ts...>
{
	using Base = detail::ITL_Impl<detail::IndexSequenceFor<Ts...>, Ts...>;

	static constexpr Long SIZE = sizeof...(Ts);

	template <typename T>
	static constexpr Long index() {
		return Base::template select<T>(TypeList{});
	}
};

template <typename TList, typename T>
constexpr Long index   () { return TList::template index   <T>(); }

template <typename TList, typename T>
constexpr bool contains() { return std::is_base_of<detail::ITL_EntryT<T>, TList>::value; }

////////////////////////////////////////////////////////////////////////////////

namespace detail {

template <typename...>
struct PrependT;

template <typename T, typename... Ts>
struct PrependT<T, TypeList<Ts...>> {
	using Type = TypeList<T, Ts...>;
};

}

template <typename... Ts>
using Prepend = typename detail::PrependT<Ts...>::Type;

//------------------------------------------------------------------------------

namespace detail {

template <typename...>
struct MergeT;

template <typename... Ts1, typename... Ts2>
struct MergeT<TypeList<Ts1...>, TypeList<Ts2...>> {
	using Type = TypeList<Ts1..., Ts2...>;
};

}

template <typename... Ts>
using Merge = typename detail::MergeT<Ts...>::Type;

//------------------------------------------------------------------------------

namespace detail {

template <Long, Long, typename...>
struct LowerT;

template <Long H, Long I, typename TFirst, typename... TRest>
struct LowerT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<(I < H),
										   Prepend<TFirst, typename LowerT<H, I + 1, TRest...>::Type>,
										   typename LowerT<H, I + 1, TRest...>::Type>::type;
};

template <Long H, Long I>
struct LowerT<H, I> {
	using Type = TypeList<>;
};

}

template <typename... Ts>
using Lower = typename detail::LowerT<sizeof...(Ts) / 2, 0, Ts...>::Type;

//------------------------------------------------------------------------------

namespace detail {

template <Long, Long, typename...>
struct UpperT;

template <Long H, Long I, typename TFirst, typename... TRest>
struct UpperT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<(I < H),
										   typename UpperT<H, I + 1, TRest...>::Type,
										   TypeList<TFirst, TRest...>>::type;
};

template <Long H, Long I>
struct UpperT<H, I> {
	using Type = TypeList<>;
};

}

template <typename... Ts>
using Upper = typename detail::UpperT<sizeof...(Ts) / 2, 0, Ts...>::Type;

////////////////////////////////////////////////////////////////////////////////

#else

template <typename... Ts>
struct TypeList {
	static constexpr Long SIZE = sizeof...(Ts);
};

//------------------------------------------------------------------------------

namespace detail {

template <Long N>
struct Const {
	static constexpr Long Value = N;
};

//------------------------------------------------------------------------------

template <typename...>
struct PrependT;

template <typename T, typename... Ts>
struct PrependT<T, TypeList<Ts...>> {
	using Type = TypeList<T, Ts...>;
};

}

template <typename... Ts>
using Prepend = typename detail::PrependT<Ts...>::Type;

//------------------------------------------------------------------------------

namespace detail {

template <typename...>
struct MergeT;

template <typename... Ts1, typename... Ts2>
struct MergeT<TypeList<Ts1...>, TypeList<Ts2...>> {
	using Type = TypeList<Ts1..., Ts2...>;
};

}

template <typename... Ts>
using Merge = typename detail::MergeT<Ts...>::Type;

//------------------------------------------------------------------------------

namespace detail {

template <Long, Long, typename...>
struct LowerT;

template <Long H, Long I, typename TFirst, typename... TRest>
struct LowerT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<(I < H),
										   Prepend<TFirst, typename LowerT<H, I + 1, TRest...>::Type>,
										   typename LowerT<H, I + 1, TRest...>::Type
										   >::type;
};

template <Long H, Long I>
struct LowerT<H, I> {
	using Type = TypeList<>;
};

}

template <typename... Ts>
using Lower = typename detail::LowerT<sizeof...(Ts) / 2, 0, Ts...>::Type;

//------------------------------------------------------------------------------

namespace detail {

template <Long, Long, typename...>
struct UpperT;

template <Long H, Long I, typename TFirst, typename... TRest>
struct UpperT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<(I < H),
		typename UpperT<H, I + 1, TRest...>::Type,
		TypeList<TFirst, TRest...>>::type;
};

template <Long H, Long I>
struct UpperT<H, I> {
	using Type = TypeList<>;
};

}

template <typename... Ts>
using Upper = typename detail::UpperT<sizeof...(Ts) / 2, 0, Ts...>::Type;

//------------------------------------------------------------------------------

namespace detail {

template<Long, typename...>
struct FindImpl
	: Const<INVALID_LONG>
{};

template<Long N, typename T, typename TFirst, typename... TRest>
struct FindImpl<N, T, TFirst, TRest...>
	: FindImpl<N + 1, T, TRest...>
{};

template<Long N, typename T, typename... Ts>
struct FindImpl<N, T, T, Ts...>
	: Const<N>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename, typename>
struct Find;

template <typename T, typename... Ts>
struct Find<T, TypeList<Ts...>>
	: FindImpl<0, T, Ts...>
{};

}

//------------------------------------------------------------------------------

template <typename TList, typename T>
constexpr Long index   () { return detail::Find<T, TList>::Value;	  }

template <typename TList, typename T>
constexpr bool contains() { return index<TList, T>() != INVALID_LONG; }

#endif

////////////////////////////////////////////////////////////////////////////////

}
