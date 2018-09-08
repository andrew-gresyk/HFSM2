#pragma once

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template<LongIndex N>
struct IndexConstant {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<LongIndex... Ns>
struct IndexSequence {
	typedef IndexSequence<Ns...> Type;
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
struct TypeList_EntryT {};

template <typename T, std::size_t N>
struct TypeList_EntryN {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
struct TypeList_Impl;

template <LongIndex... Ns, typename... Ts>
struct TypeList_Impl<IndexSequence<Ns...>, Ts...>
	: TypeList_EntryT<Ts>...
	, TypeList_EntryN<Ts, Ns>...
{
	template <typename T, std::size_t N>
	static constexpr LongIndex select(TypeList_EntryN<T, N>) { return (LongIndex) N; }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
class VariantT;

template <typename... Ts>
struct _TL
	: private TypeList_Impl<IndexSequenceFor<Ts...>, Ts...>
{
	using Base = TypeList_Impl<IndexSequenceFor<Ts...>, Ts...>;
	using Container = VariantT<Ts...>;

	static constexpr LongIndex SIZE = sizeof...(Ts);

	template <typename T>
	static constexpr bool contains() { return std::is_base_of<TypeList_EntryT<T>, _TL>::value; }

	template <typename T>
	static constexpr
	typename std::enable_if< std::is_base_of<TypeList_EntryT<T>, _TL>::value, LongIndex>::type
	index() {
		return Base::template select<T>(_TL{});
	}

	template <typename T>
	static constexpr
	typename std::enable_if<!std::is_base_of<TypeList_EntryT<T>, _TL>::value, LongIndex>::type
	index() {
		return INVALID_LONG_INDEX;
	}
};

//------------------------------------------------------------------------------

template <typename...>
struct MergeT;

template <typename... Ts1, typename... Ts2>
struct MergeT<_TL<Ts1...>, _TL<Ts2...>> {
	using TypeList = _TL<Ts1..., Ts2...>;
};

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

// TODO: add assignment op and templated type conversion op

template <typename... Ts>
class alignas(alignof(void*)) VariantT {
	using Types = _TL<Ts...>;

public:
	template <typename T>
	static constexpr LongIndex index() {
		return Types::template index<T>();
	}

	template <typename T>
	static constexpr bool contains() {
		return Types::template contains<T>();
	}

	inline VariantT() = default;

	template <typename T,
			  typename = typename std::enable_if<contains<T>()>::type>
	inline VariantT(T* const p)
		: _pointer(p)
		, _index(index<T>())
	{
		HFSM_ASSERT(_index != INVALID_LONG_INDEX);
	}

	inline explicit operator bool() const { return _index != INVALID_LONG_INDEX; }

	inline void reset() {
		_pointer = nullptr;
		_index = INVALID_LONG_INDEX;
	}

	template <typename T>
	inline typename std::enable_if<contains<T>(), T>::type*
	get() const {
		const auto INDEX = index<T>();

		return INDEX == _index ?
			reinterpret_cast<T*>(_pointer) : nullptr;
	}

private:
	void* _pointer = nullptr;
	LongIndex _index = INVALID_LONG_INDEX;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}
