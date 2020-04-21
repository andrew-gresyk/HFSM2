#pragma once

namespace hfsm2 {

//------------------------------------------------------------------------------

template <typename>
struct Guard {
	template <typename TArgs>
	static void execute(hfsm2::detail::GuardControlT<TArgs>&) {}
};

//------------------------------------------------------------------------------

namespace detail {

//------------------------------------------------------------------------------

HFSM_IF_DEBUG(struct None {});

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct DBox final {
	static constexpr bool isBare()								{ return false;					}

	union {
		T t_;
	};

	HFSM_INLINE  DBox() {}
	HFSM_INLINE ~DBox() {}

	HFSM_INLINE void guard(GuardControlT<TArgs>& control)		{ Guard<T>::execute(control);	}

	HFSM_INLINE void construct();
	HFSM_INLINE void destruct();

	HFSM_INLINE		  T& get()							{ HFSM_ASSERT(initialized_); return t_;	}
	HFSM_INLINE const T& get() const					{ HFSM_ASSERT(initialized_); return t_;	}

	HFSM_IF_ASSERT(bool initialized_ = false);

	HFSM_IF_DEBUG(const std::type_index TYPE = typeid(T));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct SBox final {
	static constexpr bool isBare()	{ return std::is_base_of<T, StaticEmptyT<TArgs>>::value;	}

	T t_;

	HFSM_INLINE void guard(GuardControlT<TArgs>& control);

	HFSM_INLINE void construct()																{}
	HFSM_INLINE void destruct()																	{}

	HFSM_INLINE		  T& get()									{ return t_;					}
	HFSM_INLINE const T& get() const							{ return t_;					}

	HFSM_IF_DEBUG(const std::type_index TYPE = isBare() ? typeid(None) : typeid(T));
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TArgs>
struct BoxifyT final {
	using Type = typename std::conditional<
					 std::is_base_of<Dynamic_, T>::value,
					 DBox<T, TArgs>,
					 SBox<T, TArgs>
				 >::type;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, typename TArgs>
using Boxify = typename BoxifyT<T, TArgs>::Type;

//------------------------------------------------------------------------------

}
}

#include "state_box.inl"
