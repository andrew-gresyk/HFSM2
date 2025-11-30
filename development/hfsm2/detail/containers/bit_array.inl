namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
BitArrayT<NCapacity>::Bits::operator bool() const noexcept {
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

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::Bits::clear() noexcept {
	const Index unitCount = contain(_width, 8);

	for (Index i = 0; i < unitCount; ++i)
		_storage[i] = uint8_t{0};
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::Bits::get() const noexcept {
	constexpr Index INDEX = NIndex;
	HFSM2_ASSERT(INDEX < _width);

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	return (_storage[unitIndex] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::Bits::set() noexcept {
	constexpr Index INDEX = NIndex;
	HFSM2_ASSERT(INDEX < _width);

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	_storage[unitIndex] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::Bits::clear() noexcept {
	constexpr Index INDEX = NIndex;
	HFSM2_ASSERT(INDEX < _width);

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	_storage[unitIndex] &= ~mask;
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::Bits::get(const Index index) const noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / 8;
	const Index bit  = index % 8;
	const uint8_t mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::Bits::set(const Index index) noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / 8;
	const Index bit  = index % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::Bits::clear(const Index index) noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / 8;
	const Index bit  = index % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] &= ~mask;
}

////////////////////////////////////////////////////////////////////////////////

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
BitArrayT<NCapacity>::CBits::operator bool() const noexcept {
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

template <unsigned NCapacity>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::CBits::get() const noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < _width, "");

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	return (_storage[unitIndex] & mask) != 0;
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::CBits::get(const Index index) const noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / 8;
	const Index bit  = index % 8;
	const uint8_t mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::set() noexcept {
	for (uint8_t& unit : _storage)
		unit = UINT8_MAX;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::clear() noexcept {
	for (uint8_t& unit : _storage)
		unit = uint8_t{0};
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::empty() const noexcept {
	for (const uint8_t& unit : _storage)
		if (unit != uint8_t{0})
			return false;

	return true;
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::get() const noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < CAPACITY, "");

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	return (_storage[unitIndex] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::set() noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < CAPACITY, "");

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	_storage[unitIndex] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::clear() noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < CAPACITY, "");

	constexpr Index unitIndex = INDEX / 8;
	constexpr Index bitIndex  = INDEX % 8;
	constexpr uint8_t mask = 1 << bitIndex;

	_storage[unitIndex] &= ~mask;
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
template <typename TIndex>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::get(const TIndex index) const noexcept {
	HFSM2_ASSERT(index < CAPACITY);

	const Index unit = static_cast<Index>(index) / 8;
	const Index bit  = static_cast<Index>(index) % 8;
	const uint8_t mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
template <typename TIndex>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::set(const TIndex index) noexcept {
	HFSM2_ASSERT(index < CAPACITY);

	const Index unit = static_cast<Index>(index) / 8;
	const Index bit  = static_cast<Index>(index) % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
template <typename TIndex>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::clear(const TIndex index) noexcept {
	HFSM2_ASSERT(index < CAPACITY);

	const Index unit = static_cast<Index>(index) / 8;
	const Index bit  = static_cast<Index>(index) % 8;
	const uint8_t mask = 1 << bit;

	_storage[unit] &= ~mask;
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::operator != (const BitArray& other) const noexcept {
	for (Index i = 0; i < UNIT_COUNT; ++i)
		if (_storage[i] != other._storage[i])
			return true;

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
bool
BitArrayT<NCapacity>::operator &  (const BitArray& other) const noexcept {
	for (Index i = 0; i < UNIT_COUNT; ++i)
		if ((_storage[i] & other._storage[i]) == 0)
			return false;

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
void
BitArrayT<NCapacity>::operator &= (const BitArray& other) noexcept {
	for (Index i = 0; i < UNIT_COUNT; ++i)
		_storage[i] &= other._storage[i];
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
template <Short NUnit, Short NWidth>
HFSM2_CONSTEXPR(14)
typename BitArrayT<NCapacity>::Bits
BitArrayT<NCapacity>::bits() noexcept {
	constexpr Short UNIT  = NUnit;
	constexpr Short WIDTH = NWidth;
	static_assert(UNIT + contain(WIDTH, 8) <= UNIT_COUNT, "");

	return Bits{_storage + UNIT, WIDTH};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
template <Short NUnit, Short NWidth>
HFSM2_CONSTEXPR(14)
typename BitArrayT<NCapacity>::CBits
BitArrayT<NCapacity>::cbits() const noexcept {
	constexpr Short UNIT  = NUnit;
	constexpr Short WIDTH = NWidth;
	static_assert(UNIT + contain(WIDTH, 8) <= UNIT_COUNT, "");

	return CBits{_storage + UNIT, WIDTH};
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
typename BitArrayT<NCapacity>::Bits
BitArrayT<NCapacity>::bits(const Units& units) noexcept {
	HFSM2_ASSERT(units.unit + contain(units.width, 8) <= UNIT_COUNT);

	return Bits{_storage + units.unit, units.width};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
typename BitArrayT<NCapacity>::CBits
BitArrayT<NCapacity>::cbits(const Units& units) const noexcept {
	HFSM2_ASSERT(units.unit + contain(units.width, 8) <= UNIT_COUNT);

	return CBits{_storage + units.unit, units.width};
}

////////////////////////////////////////////////////////////////////////////////

}
}
