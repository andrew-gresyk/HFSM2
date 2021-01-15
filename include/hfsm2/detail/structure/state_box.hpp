namespace hfsm2 {

//------------------------------------------------------------------------------

template <typename>
struct Guard {
	template <typename TArgs>
	static HFSM2_INLINE void execute(hfsm2::detail::GuardControlT<TArgs>&) noexcept {}
};

namespace detail {

//------------------------------------------------------------------------------

HFSM2_IF_DEBUG(struct None {});

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct DynamicBox final {
	using Type = T;

	static constexpr bool isBare()							  noexcept	{ return false;								}

	union {
		Type t_;
	};

	HFSM2_INLINE  DynamicBox()								  noexcept	{}
	HFSM2_INLINE ~DynamicBox()								  noexcept	{}

	HFSM2_INLINE void guard(GuardControlT<TArgs>& control)	  noexcept	{ Guard<Type>::execute(control);			}

	HFSM2_INLINE void construct()							  noexcept;
	HFSM2_INLINE void  destruct()							  noexcept;

	HFSM2_INLINE	   Type& get()							  noexcept	{ HFSM2_ASSERT(initialized_); return t_;	}
	HFSM2_INLINE const Type& get()						const noexcept	{ HFSM2_ASSERT(initialized_); return t_;	}

	HFSM2_IF_ASSERT(bool initialized_ = false);

	HFSM2_IF_DEBUG(const std::type_index TYPE = typeid(Type));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct StaticBox final {
	using Type = T;

	static constexpr bool isBare() noexcept	{ return std::is_base_of<Type, StaticEmptyT<TArgs>>::value;	}

	Type t_;

	HFSM2_INLINE void guard(GuardControlT<TArgs>& control)	  noexcept;

	HFSM2_INLINE void construct()							  noexcept	{}
	HFSM2_INLINE void  destruct()							  noexcept	{}

	HFSM2_INLINE	   Type& get()							  noexcept	{ return t_;								}
	HFSM2_INLINE const Type& get()						const noexcept	{ return t_;								}

	HFSM2_IF_DEBUG(const std::type_index TYPE = isBare() ? typeid(None) : typeid(Type));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct BoxifyT final {
	using Type = Conditional<
					 std::is_base_of<Dynamic_, T>::value,
					 DynamicBox<T, TArgs>,
					 StaticBox <T, TArgs>
				 >;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, typename TArgs>
using Boxify = typename BoxifyT<T, TArgs>::Type;

//------------------------------------------------------------------------------

}
}

#include "state_box.inl"
