#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

using TypeListIndex = unsigned char;

enum : TypeListIndex { INVALID_TYPE_LIST_INDEX = (TypeListIndex) -1 };

//------------------------------------------------------------------------------

template <TypeListIndex, typename...>
struct TypeListBuilder;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <TypeListIndex NIndex, typename TFirst, typename... TRest>
struct TypeListBuilder<NIndex, TFirst, TRest...>
	: TypeListBuilder<NIndex + 1, TRest...>
{
	enum : TypeListIndex { INDEX = NIndex };

	static_assert(INDEX < (1 << 8 * sizeof(TypeListIndex)), "Too many types");

	using Type = TFirst;
	using Base = TypeListBuilder<INDEX + 1, TRest...>;

	template <typename T>
	static constexpr TypeListIndex index() {
		return std::is_same<T, Type>::value ? INDEX : Base::template index<T>();
	}

	template <typename TCheck>
	struct Index {
		using Check = TCheck;
		enum : TypeListIndex {
			VALUE = std::is_same<Check, Type>::value ? INDEX : Base::template Index<Check>::VALUE
		};
	};
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <TypeListIndex NIndex, typename TFirst>
struct TypeListBuilder<NIndex, TFirst> {
	enum : TypeListIndex { INDEX = NIndex };

	static_assert(NIndex < (1 << 8 * sizeof(TypeListIndex)), "Too many types");

	using Type = TFirst;

	template <typename T>
	static constexpr auto index() {
		return std::is_same<T, Type>::value ? INDEX : INVALID_TYPE_LIST_INDEX;
	}

	template <typename TCheck>
	struct Index {
		using Check = TCheck;
		enum : TypeListIndex {
			VALUE = std::is_same<Check, Type>::value ? INDEX : INVALID_TYPE_LIST_INDEX
		};
	};
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

	using IndexType = typename UnsignedIndex<sizeof...(Ts)>::Type;

	enum : IndexType {
		SIZE = sizeof...(Ts),
		INVALID_INDEX = (IndexType) - 1,
	};

	template <typename TCheck>
	struct Index {
		using Check = TCheck;
		enum : IndexType {
			VALUE = Base::template Index<Check>::VALUE
		};
	};

	template <typename T>
	static constexpr bool contains() {
		return Base::template index<T>() != INVALID_TYPE_LIST_INDEX;
	}

	template <typename TCheck>
	struct Contains {
		using Check = TCheck;
		enum : bool {
			VALUE = Base::template Index<Check>::VALUE != INVALID_TYPE_LIST_INDEX
		};
	};
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

template <typename... Ts>
class VariantT {
	using Types = TypeListT<Ts...>;
	using IndexType = typename Types::IndexType;

	enum : IndexType { INVALID_INDEX = Types::INVALID_INDEX };

public:
	inline VariantT() = default;

	template <typename T, typename = typename std::enable_if<Types::template Contains<T>::VALUE>::type>
	inline VariantT(T* const p)
		: _pointer(p)
		, _index(Types::template Index<T>::VALUE)
	{
		assert((((uintptr_t) this) & 0x7) == 0);
		assert(_index != INVALID_INDEX);
	}

	inline explicit operator bool() const { return _index != INVALID_INDEX; }

	inline void reset() {
		_pointer = nullptr;
		_index = INVALID_INDEX;
	}

	template <typename T>
	inline typename std::enable_if<Types::template Contains<T>::VALUE, T>::type*
	get() {
		const auto INDEX = Types::template Index<T>::VALUE;

		assert(INDEX == _index);

		return INDEX == _index ?
			reinterpret_cast<T*>(_pointer) : nullptr;
	}

private:
	alignas(alignof(void*)) void* _pointer = nullptr;
	IndexType _index = INVALID_INDEX;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}
