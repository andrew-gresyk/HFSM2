#if HFSM2_UTILITY_THEORY_AVAILABLE()

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

HFSM2_CONSTEXPR(14)
float
uniform(const uint32_t uint) noexcept {
	return convert<float>(UINT32_C(0x7F) << 23 | uint >> 9) - 1.0f;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

HFSM2_CONSTEXPR(14)
double
uniform(const uint64_t uint) noexcept {
	return convert<double>(UINT64_C(0x3FF) << 52 | uint >> 12) - 1.0;
}

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(11)
uint32_t
rotl(const uint32_t x, const uint32_t k) noexcept {
	return (x << k) | (x >> (32 - k));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

HFSM2_CONSTEXPR(11)
uint64_t
rotl(const uint64_t x, const uint64_t k) noexcept {
	return (x << k) | (x >> (64 - k));
}

////////////////////////////////////////////////////////////////////////////////

HFSM2_CONSTEXPR(14)
SimpleRandomT<uint64_t>::SimpleRandomT(const uint64_t seed) noexcept
	: _state{seed}
{}

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(14)
uint64_t
SimpleRandomT<uint64_t>::uint64() noexcept {
	for (;;)
		if (const uint64_t number = raw64())
			return number;
}

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(14)
uint64_t
SimpleRandomT<uint64_t>::raw64() noexcept {
	uint64_t z = (_state += 0x9e3779b97f4a7c15);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;

	return z ^ (z >> 31);
}

////////////////////////////////////////////////////////////////////////////////

HFSM2_CONSTEXPR(14)
SimpleRandomT<uint32_t>::SimpleRandomT(const uint32_t seed) noexcept
	: _state{seed}
{}

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(14)
uint32_t
SimpleRandomT<uint32_t>::uint32() noexcept {
	for (;;)
		if (const uint32_t number = raw32())
			return number;
}

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(14)
uint32_t
SimpleRandomT<uint32_t>::raw32() noexcept {
	uint32_t z = (_state += 0x9E3779B9);
	z = (z ^ (z >> 16)) * 0x85ebca6b;
	z = (z ^ (z >> 13)) * 0xc2b2ae35;

	return z ^ (z >> 16);
}

////////////////////////////////////////////////////////////////////////////////

HFSM2_CONSTEXPR(20)
BaseRandomT<uint64_t>::BaseRandomT() noexcept {
	Simple generator;

	_state[0] = generator.uint64();
	_state[1] = generator.uint64();
	_state[2] = generator.uint64();
	_state[3] = generator.uint64();
}

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(14)
void
BaseRandomT<uint64_t>::seed(const uint64_t s) noexcept {
	Simple generator{s};

	_state[0] = generator.uint64();
	_state[1] = generator.uint64();
	_state[2] = generator.uint64();
	_state[3] = generator.uint64();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

HFSM2_CONSTEXPR(14)
void
BaseRandomT<uint64_t>::seed(const uint64_t(& s)[4]) noexcept {
	_state[0] = s[0];
	_state[1] = s[1];
	_state[2] = s[2];
	_state[3] = s[3];
}

////////////////////////////////////////////////////////////////////////////////

HFSM2_CONSTEXPR(20)
BaseRandomT<uint32_t>::BaseRandomT() noexcept {
	Simple generator;

	_state[0] = generator.uint32();
	_state[1] = generator.uint32();
	_state[2] = generator.uint32();
	_state[3] = generator.uint32();
}

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(14)
void
BaseRandomT<uint32_t>::seed(const uint32_t s) noexcept {
	Simple generator{s};

	_state[0] = generator.uint32();
	_state[1] = generator.uint32();
	_state[2] = generator.uint32();
	_state[3] = generator.uint32();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

HFSM2_CONSTEXPR(14)
void
BaseRandomT<uint32_t>::seed(const uint32_t(& s)[4]) noexcept {
	_state[0] = s[0];
	_state[1] = s[1];
	_state[2] = s[2];
	_state[3] = s[3];
}

////////////////////////////////////////////////////////////////////////////////

HFSM2_CONSTEXPR(14)
uint64_t
FloatRandomT<uint64_t>::uint64() noexcept {
	const uint64_t result = _state[0] + _state[3];

	const uint64_t t = _state[1] << 17;

	_state[2] ^= _state[0];
	_state[3] ^= _state[1];
	_state[1] ^= _state[2];
	_state[0] ^= _state[3];

	_state[2] ^= t;

	_state[3] = rotl(_state[3], 45);

	return result;
}

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(14)
void
FloatRandomT<uint64_t>::jump() noexcept {
	constexpr uint64_t JUMP[] = {
		0x180ec6d33cfd0aba,
		0xd5a61266f0c9392c,
		0xa9582618e03fc9aa,
		0x39abdc4529b1661c
	};

	uint64_t s0 = 0;
	uint64_t s1 = 0;
	uint64_t s2 = 0;
	uint64_t s3 = 0;

	for (unsigned i = 0; i < count<unsigned>(JUMP); ++i)
		for (int b = 0; b < 64; ++b) {
			if (JUMP[i] & UINT64_C(1) << b) {
				s0 ^= _state[0];
				s1 ^= _state[1];
				s2 ^= _state[2];
				s3 ^= _state[3];
			}
			uint64();
		}

	_state[0] = s0;
	_state[1] = s1;
	_state[2] = s2;
	_state[3] = s3;
}

////////////////////////////////////////////////////////////////////////////////

HFSM2_CONSTEXPR(14)
uint32_t
FloatRandomT<uint32_t>::uint32() noexcept {
	const uint32_t result = _state[0] + _state[3];

	const uint32_t t = _state[1] << 9;

	_state[2] ^= _state[0];
	_state[3] ^= _state[1];
	_state[1] ^= _state[2];
	_state[0] ^= _state[3];

	_state[2] ^= t;

	_state[3] = rotl(_state[3], 11);

	return result;
}

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(14)
void
FloatRandomT<uint32_t>::jump() noexcept {
	constexpr uint32_t JUMP[] = {
		0x8764000b,
		0xf542d2d3,
		0x6fa035c3,
		0x77f2db5b
	};

	uint32_t s0 = 0;
	uint32_t s1 = 0;
	uint32_t s2 = 0;
	uint32_t s3 = 0;

	for (unsigned i = 0; i < count<unsigned>(JUMP); ++i)
		for (unsigned b = 0; b < 32; ++b) {
			if (JUMP[i] & UINT32_C(1) << b) {
				s0 ^= _state[0];
				s1 ^= _state[1];
				s2 ^= _state[2];
				s3 ^= _state[3];
			}
			uint32();
		}

	_state[0] = s0;
	_state[1] = s1;
	_state[2] = s2;
	_state[3] = s3;
}

////////////////////////////////////////////////////////////////////////////////

HFSM2_CONSTEXPR(14)
uint64_t
IntRandomT<uint64_t>::uint64() noexcept {
	const uint64_t result = rotl(_state[1] * 5, 7) * 9;

	const uint64_t t = _state[1] << 17;

	_state[2] ^= _state[0];
	_state[3] ^= _state[1];
	_state[1] ^= _state[2];
	_state[0] ^= _state[3];

	_state[2] ^= t;

	_state[3] = rotl(_state[3], 45);

	return result;
}

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(14)
void
IntRandomT<uint64_t>::jump() noexcept {
	constexpr uint64_t JUMP[] = {
		0x180ec6d33cfd0aba,
		0xd5a61266f0c9392c,
		0xa9582618e03fc9aa,
		0x39abdc4529b1661c
	};

	uint64_t s0 = 0;
	uint64_t s1 = 0;
	uint64_t s2 = 0;
	uint64_t s3 = 0;

	for (unsigned i = 0; i < count<unsigned>(JUMP); ++i)
		for (int b = 0; b < 64; ++b) {
			if (JUMP[i] & UINT64_C(1) << b) {
				s0 ^= _state[0];
				s1 ^= _state[1];
				s2 ^= _state[2];
				s3 ^= _state[3];
			}
			uint64();
		}

	_state[0] = s0;
	_state[1] = s1;
	_state[2] = s2;
	_state[3] = s3;
}

////////////////////////////////////////////////////////////////////////////////

HFSM2_CONSTEXPR(14)
uint32_t
IntRandomT<uint32_t>::uint32() noexcept {
	const uint32_t result = rotl(_state[1] * 5, 7) * 9;

	const uint32_t t = _state[1] << 9;

	_state[2] ^= _state[0];
	_state[3] ^= _state[1];
	_state[1] ^= _state[2];
	_state[0] ^= _state[3];

	_state[2] ^= t;

	_state[3] = rotl(_state[3], 11);

	return result;
}

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(14)
void
IntRandomT<uint32_t>::jump() noexcept {
	constexpr uint32_t JUMP[] = {
		0x8764000b,
		0xf542d2d3,
		0x6fa035c3,
		0x77f2db5b
	};

	uint32_t s0 = 0;
	uint32_t s1 = 0;
	uint32_t s2 = 0;
	uint32_t s3 = 0;

	for (unsigned i = 0; i < count<unsigned>(JUMP); ++i)
		for (unsigned b = 0; b < 32; ++b) {
			if (JUMP[i] & UINT32_C(1) << b) {
				s0 ^= _state[0];
				s1 ^= _state[1];
				s2 ^= _state[2];
				s3 ^= _state[3];
			}
			uint32();
		}

	_state[0] = s0;
	_state[1] = s1;
	_state[2] = s2;
	_state[3] = s3;
}

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
