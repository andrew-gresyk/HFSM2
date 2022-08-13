#if HFSM2_UTILITY_THEORY_AVAILABLE()

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

HFSM2_CONSTEXPR(14)	float  uniform(const uint32_t uint)					noexcept;
HFSM2_CONSTEXPR(14)	double uniform(const uint64_t uint)					noexcept;

////////////////////////////////////////////////////////////////////////////////

template <unsigned>
class SimpleRandomT;

//------------------------------------------------------------------------------
// SplitMix64 (http://xoshiro.di.unimi.it/splitmix64.c)

template <>
class SimpleRandomT<8> {
public:
	constexpr SimpleRandomT()											noexcept	{}

	HFSM2_CONSTEXPR(14)	SimpleRandomT(const uint64_t seed)				noexcept;

	HFSM2_CONSTEXPR(14)	uint64_t uint64()								noexcept;

private:
	HFSM2_CONSTEXPR(14)	uint64_t raw64()								noexcept;

private:

	uint64_t _state = 0;
};

//------------------------------------------------------------------------------
// SplitMix32 (https://groups.google.com/forum/#!topic/prng/VFjdFmbMgZI)

template <>
class SimpleRandomT<4> {
public:
	HFSM2_CONSTEXPR(11)	SimpleRandomT()									noexcept	{}

	HFSM2_CONSTEXPR(14)	SimpleRandomT(const uint32_t seed)				noexcept;

	HFSM2_CONSTEXPR(14)	uint32_t uint32()								noexcept;

private:
	HFSM2_CONSTEXPR(14)	uint32_t raw32()								noexcept;

private:
	uint32_t _state = 0;
};

////////////////////////////////////////////////////////////////////////////////

template <unsigned>
class BaseRandomT;

//------------------------------------------------------------------------------

template <>
class BaseRandomT<8> {
protected:
	using SimpleRandom = SimpleRandomT<8>;

	HFSM2_CONSTEXPR(14)	BaseRandomT(SimpleRandom&& simple)				noexcept;

	HFSM2_CONSTEXPR(14)	void seed(SimpleRandom&& simple)				noexcept;

public:
	HFSM2_CONSTEXPR(14)	BaseRandomT()									noexcept;

	HFSM2_CONSTEXPR(14)	BaseRandomT(const uint64_t s)					noexcept;
	HFSM2_CONSTEXPR(14)	BaseRandomT(const uint64_t(& s)[4])				noexcept;

	HFSM2_CONSTEXPR(14)	void seed(const uint64_t s)						noexcept;
	HFSM2_CONSTEXPR(14)	void seed(const uint64_t(& s)[4])				noexcept;

protected:
	uint64_t _state[4];
};

//------------------------------------------------------------------------------

template <>
class BaseRandomT<4> {
protected:
	using SimpleRandom = SimpleRandomT<4>;

	HFSM2_CONSTEXPR(14)	BaseRandomT(SimpleRandom&& simple)				noexcept;

	HFSM2_CONSTEXPR(14)	void seed(SimpleRandom&& simple)				noexcept;

public:
	HFSM2_CONSTEXPR(14)	BaseRandomT()									noexcept;

	HFSM2_CONSTEXPR(14)	BaseRandomT(const uint32_t s)					noexcept;
	HFSM2_CONSTEXPR(14)	BaseRandomT(const uint32_t(& s)[4])				noexcept;

	HFSM2_CONSTEXPR(14)	void seed(const uint32_t s)						noexcept;
	HFSM2_CONSTEXPR(14)	void seed(const uint32_t(& s)[4])				noexcept;

protected:
	uint32_t _state[4];
};

////////////////////////////////////////////////////////////////////////////////

template <unsigned>
class FloatRandomT;

//------------------------------------------------------------------------------
// xoshiro256+ (http://xoshiro.di.unimi.it/xoshiro256plus.c)

template <>
class FloatRandomT<8>
	: BaseRandomT<8>
{
	using Base = BaseRandomT<8>;

public:
	using Base::BaseRandomT;

	HFSM2_CONSTEXPR(14)	double	float64()								noexcept	{ return uniform(uint64());					}
	HFSM2_CONSTEXPR(14)	float	float32()								noexcept	{ return uniform(uint32());					}

	HFSM2_CONSTEXPR(14)	uint64_t uint64()								noexcept;
	HFSM2_CONSTEXPR(14)	uint32_t uint32()								noexcept	{ return static_cast<uint32_t>(uint64());	}

	HFSM2_CONSTEXPR(14)	float next()									noexcept	{ return float32();							}
	HFSM2_CONSTEXPR(14)	void  jump()									noexcept;
};

//------------------------------------------------------------------------------
// xoshiro128+ (http://xoshiro.di.unimi.it/xoshiro128plus.c)

template <>
class FloatRandomT<4>
	: BaseRandomT<4>
{
	using Base = BaseRandomT<4>;

public:
	using Base::BaseRandomT;

	HFSM2_CONSTEXPR(14)	double	float64()								noexcept	{ return uniform(uint64());					}
	HFSM2_CONSTEXPR(14)	float	float32()								noexcept	{ return uniform(uint32());					}

	HFSM2_CONSTEXPR(14)	uint64_t uint64()								noexcept	{ return widen(uint32(), uint32());			}
	HFSM2_CONSTEXPR(14)	uint32_t uint32()								noexcept;

	HFSM2_CONSTEXPR(14)	float next()									noexcept	{ return float32();							}
	HFSM2_CONSTEXPR(14)	void  jump()									noexcept;
};

////////////////////////////////////////////////////////////////////////////////

template <unsigned>
class IntRandomT;

//------------------------------------------------------------------------------
// xoshiro256** (https://prng.di.unimi.it/xoshiro256starstar.c)

template <>
class IntRandomT<8>
	: BaseRandomT<8>
{
	using Base = BaseRandomT<8>;

public:
	using Base::BaseRandomT;

	HFSM2_CONSTEXPR(14)	double	float64()								noexcept	{ return uniform(uint64());					}
	HFSM2_CONSTEXPR(14)	float	float32()								noexcept	{ return uniform(uint32());					}

	HFSM2_CONSTEXPR(14)	uint64_t uint64()								noexcept;
	HFSM2_CONSTEXPR(14)	uint32_t uint32()								noexcept	{ return static_cast<uint32_t>(uint64());	}

	HFSM2_CONSTEXPR(14)	void jump()										noexcept;
};

//------------------------------------------------------------------------------
// xoshiro128** (https://prng.di.unimi.it/xoshiro128starstar.c)

template <>
class IntRandomT<4>
	: BaseRandomT<4>
{
	using Base = BaseRandomT<4>;

public:
	using Base::BaseRandomT;

	HFSM2_CONSTEXPR(14)	double	float64()								noexcept	{ return uniform(uint64());			}
	HFSM2_CONSTEXPR(14)	float	float32()								noexcept	{ return uniform(uint32());			}

	HFSM2_CONSTEXPR(14)	uint64_t uint64()								noexcept	{ return widen(uint32(), uint32());	}
	HFSM2_CONSTEXPR(14)	uint32_t uint32()								noexcept;

	HFSM2_CONSTEXPR(14)	void jump()										noexcept;
};

////////////////////////////////////////////////////////////////////////////////

}

using SimpleRandom = detail::SimpleRandomT<sizeof(void*)>;
using FloatRandom  = detail::FloatRandomT <sizeof(void*)>;
using IntRandom	   = detail::IntRandomT	  <sizeof(void*)>;

//------------------------------------------------------------------------------

template <typename T>
class RNGT;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <>
class RNGT<float>
	: public FloatRandom
{
public:
	using Base = FloatRandom;

	using Base::Base;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <>
class RNGT<uintptr_t>
	: public IntRandom
{
public:
	using Base = IntRandom;

	using Base::Base;
};

////////////////////////////////////////////////////////////////////////////////

}

#endif

#include "random.inl"
