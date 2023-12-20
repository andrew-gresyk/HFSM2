namespace hfsm2 {

//------------------------------------------------------------------------------

struct EmptyPayload final {};

struct Automatic;
struct Manual;

struct TopDown;
struct BottomUp;

//------------------------------------------------------------------------------

using Short		 = uint8_t;
static constexpr Short		INVALID_SHORT		= UINT8_MAX;

using Prong		 = Short;
static constexpr Prong		INVALID_PRONG		= INVALID_SHORT;

using RegionID	 = Short;
static constexpr RegionID	INVALID_REGION_ID	= INVALID_SHORT;

using ForkID	 = int8_t;
static constexpr ForkID		INVALID_FORK_ID		= INT8_MIN;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

using Long		 = uint16_t;
static constexpr Long		INVALID_LONG		= UINT16_MAX;

using StateID	 = Long;
static constexpr StateID	INVALID_STATE_ID	= INVALID_LONG;

////////////////////////////////////////////////////////////////////////////////

template <
	bool B
  , typename TT
  , typename TF
>
struct ConditionalT final {
	using Type = TT;
};

template <
	typename TT
  , typename TF
>
struct ConditionalT<false, TT, TF> final {
	using Type = TF;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <
	bool B
  , typename TT
  , typename TF
>
using Conditional = typename ConditionalT<B, TT, TF>::Type;

//------------------------------------------------------------------------------

template <typename, typename>
struct IsSameT final {
	static constexpr bool Value = false;
};

template <typename T>
struct IsSameT<T, T> final {
	static constexpr bool Value = true;
};

template <
	typename T0,
	typename T1
>
static constexpr bool IsSame = IsSameT<T0, T1>::Value;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct RemoveConstT final {
	using Type = T;
};

template <typename T>
struct RemoveConstT<const T> final {
	using Type = T;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
using RemoveConst = typename RemoveConstT<T>::Type;

//------------------------------------------------------------------------------

template <typename T>
struct RemoveReferenceT final {
	using Type = T;
};

template <typename T>
struct RemoveReferenceT<T&> final {
	using Type = T;
};

template <typename T>
struct RemoveReferenceT<T&&> final {
	using Type = T;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
using RemoveReference = typename RemoveReferenceT<T>::Type;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
using Undecorate = RemoveConst<RemoveReference<T>>;

//------------------------------------------------------------------------------

template <typename T>
struct IsValueReferenceT final {
	static constexpr bool VALUE = false;
};

template <typename T>
struct IsValueReferenceT<T&> final {
	static constexpr bool VALUE = true;
};

////////////////////////////////////////////////////////////////////////////////

template <uint64_t N>
using UIndex = Conditional<N <= (1ull <<  8),  uint8_t,
			   Conditional<N <= (1ull << 16), uint16_t,
			   Conditional<N <= (1ull << 32), uint32_t,
												  void>>>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <uint64_t N>
using UCapacity = Conditional<N < (1ull <<  8),  uint8_t,
				  Conditional<N < (1ull << 16), uint16_t,
				  Conditional<N < (1ull << 32), uint32_t,
													void>>>;

////////////////////////////////////////////////////////////////////////////////

HFSM2_CONSTEXPR(11)
Short
bitContain(const Short v)												noexcept	{
	return v <= 1 << 0 ? 0 :
		   v <= 1 << 1 ? 1 :
		   v <= 1 << 2 ? 2 :
		   v <= 1 << 3 ? 3 :
		   v <= 1 << 4 ? 4 :
		   v <= 1 << 5 ? 5 :
		   v <= 1 << 6 ? 6 :
		   v <= 1 << 7 ? 7 :
						 8 ;
}

//------------------------------------------------------------------------------

template <uint64_t N>
using UBitWidth = Conditional<N <=  8,  uint8_t,
				  Conditional<N <= 16, uint16_t,
				  Conditional<N <= 32, uint32_t,
										   void>>>;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
HFSM2_CONSTEXPR(11)
T&&
forward(RemoveReference<T>& t)											noexcept	{
	return static_cast<T&&>(t);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
HFSM2_CONSTEXPR(11)
T&&
forward(RemoveReference<T>&& t)											noexcept	{
	static_assert(!IsValueReferenceT<T>::VALUE, "");

	return static_cast<T&&>(t);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
HFSM2_CONSTEXPR(11)
RemoveReference<T>&&
move(T&& t)																noexcept	{
	return static_cast<RemoveReference<T>&&>(t);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T0,
		  typename T1>
HFSM2_CONSTEXPR(11)
T0
min(const T0 t0,
	const T1 t1)														noexcept
{
	return t0 < static_cast<T0>(t1) ?
		   t0 : static_cast<T0>(t1);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T0,
		  typename T1>
HFSM2_CONSTEXPR(11)
T0
max(const T0 t0,
	const T1 t1)														noexcept
{
	return t0 > static_cast<T0>(t1) ?
		   t0 : static_cast<T0>(t1);
}

//------------------------------------------------------------------------------

template <typename TIndex,
		  typename TElement,
		  TIndex NCount>
HFSM2_CONSTEXPR(11)
TIndex
count(const TElement (&)[NCount])										noexcept	{
	return NCount;
}

//------------------------------------------------------------------------------

template <typename T,
		  typename TT>
HFSM2_CONSTEXPR(11)
T
contain(const T x,
		const TT to)													noexcept
{
	return (x + static_cast<T>(to) - 1) / static_cast<T>(to);
}

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(11)
uint64_t
widen(const uint32_t x, const uint32_t y)								noexcept
{
	return static_cast<uint64_t>(x) << 32 | y;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
HFSM2_CONSTEXPR(14)
void
fill(T& a, const char value)											noexcept	{
	memset(&a, static_cast<int>(value), sizeof(a));
}

//------------------------------------------------------------------------------

template <typename T>
HFSM2_CONSTEXPR(14)
void
swap(T& l, T& r)														noexcept	{
	T t = move(l);
	l = move(r);
	r = move(t);
}

//------------------------------------------------------------------------------

template <
	typename TTo
  , typename TFrom
>
HFSM2_CONSTEXPR(14)
void
overwrite(TTo& to, const TFrom& from)									noexcept	{
	static_assert(sizeof(TTo) == sizeof(TFrom), "");

#if defined(__GNUC__) || defined(__GNUG__)
	memcpy  (&to,			  &from, sizeof(from));
#else
	memcpy_s(&to, sizeof(to), &from, sizeof(from));
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <
	typename TO
  , typename TI
>
HFSM2_CONSTEXPR(14)
TO
reinterpret(const TI& in)												noexcept	{
	TO out{};

	overwrite(out, in);

	return out;
}

////////////////////////////////////////////////////////////////////////////////

template <int>
struct StaticPrintConstT;

template <typename>
struct StaticPrintTypeT;

////////////////////////////////////////////////////////////////////////////////

}
