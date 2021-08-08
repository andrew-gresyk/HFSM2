namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
HFSM2_CONSTEXPR(14)
BitArrayT<TI, NC>::Bits::operator bool() const noexcept {
	const Short fullUnits = _width / 8;

	// TODO: cover this case
	for (Index i = 0; i < fullUnits; ++i)
		if (_storage[i])
			return true;

	const Short bit = _width % 8;
	const uint8_t mask = (1 << bit) - 1;
	const uint8_t& unit = _storage[fullUnits];

	return (unit & mask) != 0;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
HFSM2_CONSTEXPR(14)
void
BitArrayT<TI, NC>::Bits::clear() noexcept {
	const Index unitCount = contain(_width, 8);

	for (Index i = 0; i < unitCount; ++i)
		_storage[i] = uint8_t{0};
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<TI, NC>::Bits::get() const noexcept {
	constexpr Index INDEX = NIndex;
	HFSM2_ASSERT(INDEX < _width);

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	return (_storage[unitIndex] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
void
BitArrayT<TI, NC>::Bits::set() noexcept {
	constexpr Index INDEX = NIndex;
	HFSM2_ASSERT(INDEX < _width);

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	_storage[unitIndex] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
void
BitArrayT<TI, NC>::Bits::clear() noexcept {
	constexpr Index INDEX = NIndex;
	HFSM2_ASSERT(INDEX < _width);

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	_storage[unitIndex] &= ~mask;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<TI, NC>::Bits::get(const Index index) const noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / 8;
	const Index bit  = index % 8;
	const uint8_t mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
HFSM2_CONSTEXPR(14)
void
BitArrayT<TI, NC>::Bits::set(const Index index) noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / 8;
	const Index bit  = index % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
HFSM2_CONSTEXPR(14)
void
BitArrayT<TI, NC>::Bits::clear(const Index index) noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / 8;
	const Index bit  = index % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] &= ~mask;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
HFSM2_CONSTEXPR(14)
BitArrayT<TI, NC>::CBits::operator bool() const noexcept {
	const Short fullUnits = _width / 8;

	for (Index i = 0; i < fullUnits; ++i)
		if (_storage[i])
			return true;

	const Short bit = _width % 8;
	const uint8_t mask = (1 << bit) - 1;
	const uint8_t& unit = _storage[fullUnits];

	return (unit & mask) != 0;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<TI, NC>::CBits::get() const noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < _width, "");

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	return (_storage[unitIndex] & mask) != 0;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<TI, NC>::CBits::get(const Index index) const noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / 8;
	const Index bit  = index % 8;
	const uint8_t mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
HFSM2_CONSTEXPR(14)
void
BitArrayT<TI, NC>::clear() noexcept {
	for (uint8_t& unit : _storage)
		unit = uint8_t{0};
}

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<TI, NC>::get() const noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < BIT_COUNT, "");

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	return (_storage[unitIndex] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
void
BitArrayT<TI, NC>::set() noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < BIT_COUNT, "");

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	_storage[unitIndex] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
void
BitArrayT<TI, NC>::clear() noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < BIT_COUNT, "");

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	_storage[unitIndex] &= ~mask;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<TI, NC>::get(const Index index) const noexcept {
	HFSM2_ASSERT(index < BIT_COUNT);

	const Index unit = index / 8;
	const Index bit  = index % 8;
	const uint8_t mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
HFSM2_CONSTEXPR(14)
void
BitArrayT<TI, NC>::set(const Index index) noexcept {
	HFSM2_ASSERT(index < BIT_COUNT);

	const Index unit = index / 8;
	const Index bit  = index % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
HFSM2_CONSTEXPR(14)
void
BitArrayT<TI, NC>::clear(const Index index) noexcept {
	HFSM2_ASSERT(index < BIT_COUNT);

	const Index unit = index / 8;
	const Index bit  = index % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] &= ~mask;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
template <Short NUnit, Short NWidth>
HFSM2_CONSTEXPR(14)
typename BitArrayT<TI, NC>::Bits
BitArrayT<TI, NC>::bits() noexcept {
	constexpr Short UNIT  = NUnit;
	constexpr Short WIDTH = NWidth;
	static_assert(UNIT + contain(WIDTH, 8) <= UNIT_COUNT, "");

	return Bits{_storage + UNIT, WIDTH};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
template <Short NUnit, Short NWidth>
HFSM2_CONSTEXPR(14)
typename BitArrayT<TI, NC>::CBits
BitArrayT<TI, NC>::cbits() const noexcept {
	constexpr Short UNIT  = NUnit;
	constexpr Short WIDTH = NWidth;
	static_assert(UNIT + contain(WIDTH, 8) <= UNIT_COUNT, "");

	return CBits{_storage + UNIT, WIDTH};
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
HFSM2_CONSTEXPR(14)
typename BitArrayT<TI, NC>::Bits
BitArrayT<TI, NC>::bits(const Units& units) noexcept {
	HFSM2_ASSERT(units.unit + contain(units.width, 8) <= UNIT_COUNT);

	return Bits{_storage + units.unit, units.width};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
HFSM2_CONSTEXPR(11)
typename BitArrayT<TI, NC>::CBits
BitArrayT<TI, NC>::cbits(const Units& units) const noexcept {
	HFSM2_ASSERT(units.unit + contain(units.width, 8) <= UNIT_COUNT);

	return CBits{_storage + units.unit, units.width};
}

////////////////////////////////////////////////////////////////////////////////

}
}
