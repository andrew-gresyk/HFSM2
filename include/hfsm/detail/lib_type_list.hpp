#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <LongIndex, typename...>
struct TypeListBuilder;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <LongIndex NIndex, typename TFirst, typename... TRest>
struct TypeListBuilder<NIndex, TFirst, TRest...>
	: TypeListBuilder<NIndex + 1, TRest...>
{
	static constexpr LongIndex INDEX = NIndex;

	static_assert(INDEX < (1 << 8 * sizeof(LongIndex)), "Too many types");

	using Type = TFirst;
	using Base = TypeListBuilder<INDEX + 1, TRest...>;

	template <typename T>
	static constexpr LongIndex index() {
		return std::is_same<T, Type>::value ? INDEX : Base::template index<T>();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <LongIndex NIndex, typename TFirst>
struct TypeListBuilder<NIndex, TFirst> {
	static constexpr LongIndex INDEX = NIndex;

	static_assert(NIndex < (1 << 8 * sizeof(LongIndex)), "Too many types");

	using Type = TFirst;

	template <typename T>
	static constexpr LongIndex index() {
		return std::is_same<T, Type>::value ? INDEX : INVALID_LONG_INDEX;
	}
};

////////////////////////////////////////////////////////////////////////////////

template <typename...>
class VariantT;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename... Ts>
struct TypeListT
	: TypeListBuilder<0, Ts...>
{
	using Base = TypeListBuilder<0, Ts...>;
	using Container = VariantT<Ts...>;

	static constexpr LongIndex SIZE = sizeof...(Ts);

	template <typename T>
	static constexpr LongIndex index() {
		return Base::template index<T>();
	}

	template <typename T>
	static constexpr bool contains() {
		return Base::template index<T>() != INVALID_LONG_INDEX;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <>
struct TypeListT<>
	: TypeListT<void>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


template <typename...>
struct MergeT;

template <typename... Ts1, typename... Ts2>
struct MergeT<TypeListT<Ts1...>, TypeListT<Ts2...>> {
	using TypeList = TypeListT<Ts1..., Ts2...>;
};

//------------------------------------------------------------------------------

#pragma pack(push, 1)

// TODO: add assignment op and templated type conversion op

template <typename... Ts>
class VariantT {
	using Types = TypeListT<Ts...>;

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
		HFSM_IF_ALIGNMENT_CHEKS(assert((((uintptr_t) this) & (sizeof(void*) - 1)) == 0));
		assert(_index != INVALID_LONG_INDEX);
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
	alignas(alignof(void*)) void* _pointer = nullptr;
	LongIndex _index = INVALID_LONG_INDEX;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}
