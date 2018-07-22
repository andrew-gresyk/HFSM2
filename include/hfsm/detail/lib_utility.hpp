#pragma once

#if defined _DEBUG && _MSC_VER
	#include <intrin.h>		// __debugbreak()
#endif

//------------------------------------------------------------------------------

#if defined _DEBUG && _MSC_VER
	#define HSFM_BREAK()			__debugbreak()
	#define HSFM_CHECKED(x)			(!!(x) || (HSFM_BREAK(), 0))
#else
	#define HSFM_BREAK()			((void) 0)
	#define HSFM_CHECKED(x)			x
#endif

#ifdef _DEBUG
	#define HSFM_IF_DEBUG(...)		__VA_ARGS__
#else
	#define HSFM_IF_DEBUG(...)
#endif

#ifndef NDEBUG
	#define HSFM_IF_ASSERT(...)		__VA_ARGS__
#else
	#define HSFM_IF_ASSERT(...)
#endif

//------------------------------------------------------------------------------

namespace hfsm {

using ShortIndex = uint8_t;
enum : ShortIndex { INVALID_SHORT_INDEX = UINT8_MAX  };

using LongIndex  = uint16_t;
enum : LongIndex  { INVALID_LONG_INDEX	= UINT16_MAX };

//------------------------------------------------------------------------------

namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
inline
void
fill(T& a, const char value) {
	memset(&a, (int) value, sizeof(a));
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, unsigned NCount>
inline
unsigned
count(const T(&)[NCount]) {
	return NCount;
}

//------------------------------------------------------------------------------

template <typename T, unsigned NCapacity>
inline
const T*
end(const T(& a)[NCapacity]) {
	return &a[NCapacity];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TReturn, typename T, unsigned NCapacity>
inline
const TReturn*
end(const T(& a)[NCapacity]) {
	return reinterpret_cast<const TReturn*>(&a[NCapacity]);
}

////////////////////////////////////////////////////////////////////////////////

template <int N1, int N2>
struct Min {
	enum {
		VALUE = N1 < N2 ? N1 : N2
	};
};

//------------------------------------------------------------------------------

template <int N1, int N2>
struct Max {
	enum {
		VALUE = N1 > N2 ? N1 : N2
	};
};

//------------------------------------------------------------------------------

template <unsigned NCapacity>
struct UnsignedIndex {
	enum : LongIndex { CAPACITY = NCapacity };

	using Type = typename std::conditional<CAPACITY <= UINT8_MAX,  uint8_t,
				 typename std::conditional<CAPACITY <= UINT16_MAX, uint16_t,
				 typename std::conditional<CAPACITY <= UINT32_MAX, uint32_t,
																   uint64_t>::type>::type>::type;

	static_assert(CAPACITY <= UINT64_MAX, "STATIC ASSERT");
};


////////////////////////////////////////////////////////////////////////////////

}
}
