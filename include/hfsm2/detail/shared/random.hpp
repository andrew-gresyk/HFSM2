#ifdef HFSM2_ENABLE_UTILITY_THEORY

namespace hfsm2 {

////////////////////////////////////////////////////////////////////////////////
// http://xoshiro.di.unimi.it/splitmix64.c

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 26495) // variable is uninitialized
#endif

class SplitMix64 {
public:
	inline SplitMix64() {}
	inline SplitMix64(const uint64_t seed);

	inline uint64_t next();

private:
	inline uint64_t raw();

private:

	uint64_t _state;
};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

//------------------------------------------------------------------------------
// xoshiro256+ (http://xoshiro.di.unimi.it/xoshiro256plus.c)

class XoShiRo256Plus {
public:
	inline XoShiRo256Plus();
	inline XoShiRo256Plus(const uint64_t s);
	inline XoShiRo256Plus(const uint64_t(& s)[4]);

	inline void seed(const uint64_t s);
	inline void seed(const uint64_t(& s)[4]);

	inline float next();
	inline void jump();

private:
	inline uint64_t raw();

private:
	uint64_t _state[4];
};

////////////////////////////////////////////////////////////////////////////////
// https://groups.google.com/forum/#!topic/prng/VFjdFmbMgZI

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 26495) // variable is uninitialized
#endif

class SplitMix32 {
public:
	inline SplitMix32() {}
	inline SplitMix32(const uint32_t seed);

	inline uint32_t next();

private:
	inline uint32_t raw();

private:
	uint32_t _state;
};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

//------------------------------------------------------------------------------
// http://xoshiro.di.unimi.it/xoshiro128plus.c

class XoShiRo128Plus {
public:
	inline XoShiRo128Plus();
	inline XoShiRo128Plus(const uint32_t s);
	inline XoShiRo128Plus(const uint32_t(& s)[4]);

	inline void seed(const uint32_t s);
	inline void seed(const uint32_t(& s)[4]);

	inline float next();
	inline void jump();

private:
	inline uint32_t raw();

private:
	uint32_t _state[4];
};

//------------------------------------------------------------------------------

template <typename T>
class RandomT;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <>
class RandomT<float>
	: public HFSM2_64BIT_OR_32BIT(XoShiRo256Plus, XoShiRo128Plus)
{
public:
	using HFSM2_64BIT_OR_32BIT(XoShiRo256Plus::XoShiRo256Plus,
							   XoShiRo128Plus::XoShiRo128Plus);
};

////////////////////////////////////////////////////////////////////////////////

}

#endif

#include "random.inl"
