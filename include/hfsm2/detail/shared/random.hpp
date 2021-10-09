#if HFSM2_UTILITY_THEORY_AVAILABLE()

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

HFSM2_CONSTEXPR(14)	float  uniform(const uint32_t uint)		  noexcept;
HFSM2_CONSTEXPR(14)	double uniform(const uint64_t uint)		  noexcept;

////////////////////////////////////////////////////////////////////////////////

template <typename>
class SimpleRandomT;

//------------------------------------------------------------------------------
// SplitMix64 (http://xoshiro.di.unimi.it/splitmix64.c)

template <>
class SimpleRandomT<uint64_t> {
public:
	constexpr SimpleRandomT()								  noexcept {}

	HFSM2_CONSTEXPR(14)	SimpleRandomT(const uint64_t seed)	  noexcept;

	HFSM2_CONSTEXPR(14)	uint64_t uint64()					  noexcept;

private:
	HFSM2_CONSTEXPR(14)	uint64_t raw64()					  noexcept;

private:

	uint64_t _state = 0;
};

//------------------------------------------------------------------------------
// SplitMix32 (https://groups.google.com/forum/#!topic/prng/VFjdFmbMgZI)

template <>
class SimpleRandomT<uint32_t> {
public:
	constexpr SimpleRandomT()								  noexcept {}

	HFSM2_CONSTEXPR(14)	SimpleRandomT(const uint32_t seed)	  noexcept;

	HFSM2_CONSTEXPR(14)	uint32_t uint32()					  noexcept;

private:
	HFSM2_CONSTEXPR(14)	uint32_t raw32()					  noexcept;

private:
	uint32_t _state = 0;
};

////////////////////////////////////////////////////////////////////////////////

template <typename>
class BaseRandomT;

//------------------------------------------------------------------------------

template <>
class BaseRandomT<uint64_t> {
protected:
	using Simple = SimpleRandomT<uint64_t>;

public:
	HFSM2_CONSTEXPR(20) BaseRandomT()						  noexcept;

	HFSM2_CONSTEXPR(20) BaseRandomT(const uint64_t s)		  noexcept	{ seed(s);	}
	HFSM2_CONSTEXPR(20) BaseRandomT(const uint64_t(& s)[4])	  noexcept	{ seed(s);	}

	HFSM2_CONSTEXPR(14)	void seed(const uint64_t s)			  noexcept;
	HFSM2_CONSTEXPR(14)	void seed(const uint64_t(& s)[4])	  noexcept;

protected:
	uint64_t _state[4];
};

//------------------------------------------------------------------------------

template <>
class BaseRandomT<uint32_t> {
protected:
	using Simple = SimpleRandomT<uint32_t>;

public:
	HFSM2_CONSTEXPR(20) BaseRandomT()						  noexcept;

	HFSM2_CONSTEXPR(20) BaseRandomT(const uint32_t s)		  noexcept	{ seed(s);	}
	HFSM2_CONSTEXPR(20) BaseRandomT(const uint32_t(& s)[4])	  noexcept	{ seed(s);	}

	HFSM2_CONSTEXPR(14)	void seed(const uint32_t s)			  noexcept;
	HFSM2_CONSTEXPR(14)	void seed(const uint32_t(& s)[4])	  noexcept;

protected:
	uint32_t _state[4];
};

////////////////////////////////////////////////////////////////////////////////

template <typename>
class FloatRandomT;

//------------------------------------------------------------------------------
// xoshiro256+ (http://xoshiro.di.unimi.it/xoshiro256plus.c)

template <>
class FloatRandomT<uint64_t>
	: BaseRandomT<uint64_t>
{
	using Base = BaseRandomT<uint64_t>;

public:
	using Base::BaseRandomT;

	HFSM2_CONSTEXPR(14)	double	float64()					  noexcept	{ return uniform(uint64());			}
	HFSM2_CONSTEXPR(14)	float	float32()					  noexcept	{ return uniform(uint32());			}

	HFSM2_CONSTEXPR(14)	uint64_t uint64()					  noexcept;
	HFSM2_CONSTEXPR(14)	uint32_t uint32()					  noexcept	{ return (uint32_t) uint64();		}

	HFSM2_CONSTEXPR(14)	float next()						  noexcept	{ return float32();					}
	HFSM2_CONSTEXPR(14)	void  jump()						  noexcept;
};

//------------------------------------------------------------------------------
// xoshiro128+ (http://xoshiro.di.unimi.it/xoshiro128plus.c)

template <>
class FloatRandomT<uint32_t>
	: BaseRandomT<uint32_t>
{
	using Base = BaseRandomT<uint32_t>;

public:
	using Base::BaseRandomT;

	HFSM2_CONSTEXPR(14)	double	float64()					  noexcept	{ return uniform(uint64());			}
	HFSM2_CONSTEXPR(14)	float	float32()					  noexcept	{ return uniform(uint32());			}

	HFSM2_CONSTEXPR(14)	uint64_t uint64()					  noexcept	{ return widen(uint32(), uint32());	}
	HFSM2_CONSTEXPR(14)	uint32_t uint32()					  noexcept;

	HFSM2_CONSTEXPR(14)	float next()						  noexcept	{ return float32();					}
	HFSM2_CONSTEXPR(14)	void  jump()						  noexcept;
};

////////////////////////////////////////////////////////////////////////////////

template <typename>
class IntRandomT;

//------------------------------------------------------------------------------
// xoshiro256** (https://prng.di.unimi.it/xoshiro256starstar.c)

template <>
class IntRandomT<uint64_t>
	: BaseRandomT<uint64_t>
{
	using Base = BaseRandomT<uint64_t>;

public:
	using Base::BaseRandomT;

	HFSM2_CONSTEXPR(14)	double	float64()					  noexcept	{ return uniform(uint64());			}
	HFSM2_CONSTEXPR(14)	float	float32()					  noexcept	{ return uniform(uint32());			}

	HFSM2_CONSTEXPR(14)	uint64_t uint64()					  noexcept;
	HFSM2_CONSTEXPR(14)	uint32_t uint32()					  noexcept	{ return (uint32_t) uint64();		}

	HFSM2_CONSTEXPR(14)	void jump()							  noexcept;
};

//------------------------------------------------------------------------------
// xoshiro128** (https://prng.di.unimi.it/xoshiro128starstar.c)

template <>
class IntRandomT<uint32_t>
	: BaseRandomT<uint32_t>
{
	using Base = BaseRandomT<uint32_t>;

public:
	using Base::BaseRandomT;

	HFSM2_CONSTEXPR(14)	double	float64()					  noexcept	{ return uniform(uint64());			}
	HFSM2_CONSTEXPR(14)	float	float32()					  noexcept	{ return uniform(uint32());			}

	HFSM2_CONSTEXPR(14)	uint64_t uint64()					  noexcept	{ return widen(uint32(), uint32());	}
	HFSM2_CONSTEXPR(14)	uint32_t uint32()					  noexcept;

	HFSM2_CONSTEXPR(14)	void jump()							  noexcept;
};

////////////////////////////////////////////////////////////////////////////////

}

using SimpleRandom = detail::SimpleRandomT<uintptr_t>;
using FloatRandom  = detail::FloatRandomT <uintptr_t>;
using IntRandom	   = detail::IntRandomT	  <uintptr_t>;

//------------------------------------------------------------------------------

template <typename T>
class RNGT;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <>
class RNGT<float>
	: public detail::FloatRandomT<uintptr_t>
{
public:
	using Base = detail::FloatRandomT<uintptr_t>;

	using Base::Base;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <>
class RNGT<uintptr_t>
	: public detail::IntRandomT<uintptr_t>
{
public:
	using Base = detail::IntRandomT<uintptr_t>;

	using Base::Base;
};

////////////////////////////////////////////////////////////////////////////////

}

#endif

#include "random.inl"
