namespace hfsm2 {

//------------------------------------------------------------------------------

struct EmptyContext {};
struct EmptyPayload {};

//------------------------------------------------------------------------------

using Short		 = uint8_t;
static constexpr Short		INVALID_SHORT		= UINT8_MAX;

using RegionID	 = Short;
static constexpr RegionID	INVALID_REGION_ID	= INVALID_SHORT;

using ForkID	 = int8_t;
static constexpr ForkID		INVALID_FORK_ID		= INT8_MIN;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

using Long		 = uint16_t;
static constexpr Long		INVALID_LONG		= UINT16_MAX;

using StateID	 = Long;
static constexpr StateID	INVALID_STATE_ID	= INVALID_LONG;

//------------------------------------------------------------------------------

namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <bool B, class TT, class TF>
struct ConditionalT {
	using Type = TT;
};

template <class TT, class TF>
struct ConditionalT<false, TT, TF> {
	using Type = TF;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <bool B, class TT, class TF>
using Conditional = typename ConditionalT<B, TT, TF>::Type;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
HFSM2_INLINE void
fill(T& a, const char value) noexcept {
	memset(&a, (int) value, sizeof(a));
}

//------------------------------------------------------------------------------

template <typename TIndex, typename TElement, TIndex NCount>
constexpr TIndex
count(const TElement(&)[NCount]) noexcept {
	return NCount;
}

////////////////////////////////////////////////////////////////////////////////

template <int N1, int N2>
struct Min {
	static constexpr auto VALUE = N1 < N2 ? N1 : N2;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <int N1, int N2>
struct Max {
	static constexpr auto VALUE = N1 > N2 ? N1 : N2;
};

//------------------------------------------------------------------------------

template <typename T>
constexpr T
min(const T t1, const T t2) noexcept {
	return t1 < t2 ?
		t1 : t2;
}


//------------------------------------------------------------------------------

template <uint64_t NCapacity>
struct UnsignedCapacityT {
	static constexpr uint64_t CAPACITY = NCapacity;

	using Type = Conditional<CAPACITY <= UINT8_MAX,  uint8_t,
				 Conditional<CAPACITY <= UINT16_MAX, uint16_t,
				 Conditional<CAPACITY <= UINT32_MAX, uint32_t,
													 uint64_t>>>;
};

template <uint64_t NCapacity>
using UnsignedCapacity = typename UnsignedCapacityT<NCapacity>::Type;

//------------------------------------------------------------------------------

template <uint64_t NBitWidth>
struct UnsignedBitWidthT {
	static constexpr Short BIT_WIDTH = NBitWidth;

	using Type = Conditional<BIT_WIDTH <= 8,  uint8_t,
				 Conditional<BIT_WIDTH <= 16, uint16_t,
				 Conditional<BIT_WIDTH <= 32, uint32_t,
											  uint64_t>>>;

	static_assert(BIT_WIDTH <= 64, "STATIC ASSERT");
};

template <uint64_t NCapacity>
using UnsignedBitWidth = typename UnsignedBitWidthT<NCapacity>::Type;

//------------------------------------------------------------------------------

template <typename T1, typename T2>
constexpr T1
contain(const T1 x, const T2 to) noexcept {
	return (x + (T1) to - 1) / (T1) to;
}

//------------------------------------------------------------------------------

constexpr Short
bitWidth(const Short x) noexcept {
	return x <=   2 ? 1 :
		   x <=   4 ? 2 :
		   x <=   8 ? 3 :
		   x <=  16 ? 4 :
		   x <=  32 ? 5 :
		   x <=  64 ? 6 :
		   x <= 128 ? 7 :
					  8 ;
}

//------------------------------------------------------------------------------

template <typename TTo, typename TFrom>
HFSM2_INLINE void
overwrite(TTo& to, const TFrom& from) noexcept {
	static_assert(sizeof(TTo) == sizeof(TFrom), "STATIC ASSERT");

#if defined(__GNUC__) || defined(__GNUG__)
	memcpy  (&to,			  &from, sizeof(from));
#else
	memcpy_s(&to, sizeof(to), &from, sizeof(from));
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TO, typename TI>
HFSM2_INLINE TO
convert(const TI& in) noexcept {
	TO out;

	overwrite(out, in);

	return out;
}

////////////////////////////////////////////////////////////////////////////////

template <int>
struct StaticPrintConst;

template <typename>
struct StaticPrintType;

////////////////////////////////////////////////////////////////////////////////

}
}
