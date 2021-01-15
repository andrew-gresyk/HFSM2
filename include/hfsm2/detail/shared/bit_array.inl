namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
BitArray<TI, NC>::Bits::operator bool() const noexcept {
	const Short fullUnits = _width / UNIT_WIDTH;

	// TODO: cover this case
	for (Index i = 0; i < fullUnits; ++i)
		if (_storage[i])
			return true;

	const Short bit = _width % UNIT_WIDTH;
	const Unit mask = (1 << bit) - 1;
	const Unit& u = _storage[fullUnits];

	return (u & mask) != 0;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
void
BitArray<TI, NC>::Bits::clear() noexcept {
	const Index unitCount = contain(_width, UNIT_WIDTH);

	for (Index i = 0; i < unitCount; ++i)
		_storage[i] = Unit{0};
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
template <Short NIndex>
bool
BitArray<TI, NC>::Bits::get() const noexcept {
	constexpr Index INDEX = NIndex;
	HFSM2_ASSERT(INDEX < _width);

	constexpr Index BIT_UNIT  = INDEX / UNIT_WIDTH;
	constexpr Index BIT_INDEX = INDEX % UNIT_WIDTH;
	constexpr Unit MASK = 1 << BIT_INDEX;

	return (_storage[BIT_UNIT] & MASK) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
template <Short NIndex>
void
BitArray<TI, NC>::Bits::set() noexcept {
	constexpr Index INDEX = NIndex;
	HFSM2_ASSERT(INDEX < _width);

	constexpr Index BIT_UNIT  = INDEX / UNIT_WIDTH;
	constexpr Index BIT_INDEX = INDEX % UNIT_WIDTH;
	constexpr Unit MASK = 1 << BIT_INDEX;

	_storage[BIT_UNIT] |= MASK;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
template <Short NIndex>
void
BitArray<TI, NC>::Bits::clear() noexcept {
	constexpr Index INDEX = NIndex;
	HFSM2_ASSERT(INDEX < _width);

	constexpr Index BIT_UNIT  = INDEX / UNIT_WIDTH;
	constexpr Index BIT_INDEX = INDEX % UNIT_WIDTH;
	constexpr Unit MASK = 1 << BIT_INDEX;

	_storage[BIT_UNIT] &= ~MASK;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
bool
BitArray<TI, NC>::Bits::get(const Index index) const noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
void
BitArray<TI, NC>::Bits::set(const Index index) noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
void
BitArray<TI, NC>::Bits::clear(const Index index) noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	_storage[unit] &= ~mask;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
BitArray<TI, NC>::CBits::operator bool() const noexcept {
	const Short fullUnits = _width / UNIT_WIDTH;

	for (Index i = 0; i < fullUnits; ++i)
		if (_storage[i])
			return true;

	const Short bit = _width % UNIT_WIDTH;
	const Unit mask = (1 << bit) - 1;
	const Unit& u = _storage[fullUnits];

	return (u & mask) != 0;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
template <Short NIndex>
bool
BitArray<TI, NC>::CBits::get() const noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < _width, "");

	constexpr Index BIT_UNIT  = INDEX / UNIT_WIDTH;
	constexpr Index BIT_INDEX = INDEX % UNIT_WIDTH;
	constexpr Unit MASK = 1 << BIT_INDEX;

	return (_storage[BIT_UNIT] & MASK) != 0;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
bool
BitArray<TI, NC>::CBits::get(const Index index) const noexcept {
	HFSM2_ASSERT(index < _width);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
void
BitArray<TI, NC>::clear() noexcept {
	for (Unit& unit: _storage)
		unit = Unit{0};
}

////////////////////////////////////////////////////////////////////////////////

template <typename TI, Short NC>
template <Short NIndex>
bool
BitArray<TI, NC>::get() const noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < BIT_COUNT, "");

	constexpr Index BIT_UNIT  = INDEX / UNIT_WIDTH;
	constexpr Index BIT_INDEX = INDEX % UNIT_WIDTH;
	constexpr Unit MASK = 1 << BIT_INDEX;

	return (_storage[BIT_UNIT] & MASK) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
template <Short NIndex>
void
BitArray<TI, NC>::set() noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < BIT_COUNT, "");

	constexpr Index BIT_UNIT  = INDEX / UNIT_WIDTH;
	constexpr Index BIT_INDEX = INDEX % UNIT_WIDTH;
	constexpr Unit MASK = 1 << BIT_INDEX;

	_storage[BIT_UNIT] |= MASK;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
template <Short NIndex>
void
BitArray<TI, NC>::clear() noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < BIT_COUNT, "");

	constexpr Index BIT_UNIT  = INDEX / UNIT_WIDTH;
	constexpr Index BIT_INDEX = INDEX % UNIT_WIDTH;
	constexpr Unit MASK = 1 << BIT_INDEX;

	_storage[BIT_UNIT] &= ~MASK;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
bool
BitArray<TI, NC>::get(const Index index) const noexcept {
	HFSM2_ASSERT(index < BIT_COUNT);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
void
BitArray<TI, NC>::set(const Index index) noexcept {
	HFSM2_ASSERT(index < BIT_COUNT);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
void
BitArray<TI, NC>::clear(const Index index) noexcept {
	HFSM2_ASSERT(index < BIT_COUNT);

	const Index unit = index / UNIT_WIDTH;
	const Index bit  = index % UNIT_WIDTH;
	const Unit mask = 1 << bit;

	_storage[unit] &= ~mask;
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
template <Short NUnit, Short NWidth>
typename BitArray<TI, NC>::Bits
BitArray<TI, NC>::bits() noexcept {
	constexpr Short UNIT  = NUnit;
	constexpr Short WIDTH = NWidth;
	static_assert(UNIT + contain(WIDTH, UNIT_WIDTH) <= UNIT_COUNT, "");

	return Bits{_storage + UNIT, WIDTH};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
template <Short NUnit, Short NWidth>
typename BitArray<TI, NC>::CBits
BitArray<TI, NC>::bits() const noexcept {
	constexpr Short UNIT  = NUnit;
	constexpr Short WIDTH = NWidth;
	static_assert(UNIT + contain(WIDTH, UNIT_WIDTH) <= UNIT_COUNT, "");

	return CBits{_storage + UNIT, WIDTH};
}

//------------------------------------------------------------------------------

template <typename TI, Short NC>
typename BitArray<TI, NC>::Bits
BitArray<TI, NC>::bits(const Units& units) noexcept {
	HFSM2_ASSERT(units.unit + contain(units.width, UNIT_WIDTH) <= UNIT_COUNT);

	return Bits{_storage + units.unit, units.width};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, Short NC>
typename BitArray<TI, NC>::CBits
BitArray<TI, NC>::bits(const Units& units) const noexcept {
	HFSM2_ASSERT(units.unit + contain(units.width, UNIT_WIDTH) <= UNIT_COUNT);

	return CBits{_storage + units.unit, units.width};
}

////////////////////////////////////////////////////////////////////////////////

}
}
