namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndex, ShortIndex NCapacity>
BitArray<TIndex, NCapacity>::Bits::operator bool() const {
	const ShortIndex fullUnits = _width / (sizeof(Unit) * 8);

	for (Index i = 0; i < fullUnits; ++i)
		if (_storage[i])
			return true;

	const ShortIndex bit = _width % (sizeof(Unit) * 8);
	const Unit mask = (1 << bit) - 1;
	const Unit& u = _storage[fullUnits];

	return (u & mask) != 0;
}

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
void
BitArray<TIndex, NCapacity>::Bits::clear() {
	const Index count = (_width + 7) / (sizeof(Unit) * 8);

	for (Index i = 0; i < count; ++i)
		_storage[i] = Unit{0};
}

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
template <ShortIndex NIndex>
bool
BitArray<TIndex, NCapacity>::Bits::get() const {
	constexpr Index INDEX = NIndex;
	HFSM_ASSERT(INDEX < _width);

	constexpr Index BIT_UNIT  = INDEX / (sizeof(Unit) * 8);
	constexpr Index BIT_INDEX = INDEX % (sizeof(Unit) * 8);
	constexpr Unit MASK = 1 << BIT_INDEX;

	return (_storage[BIT_UNIT] & MASK) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex, ShortIndex NCapacity>
template <ShortIndex NIndex>
void
BitArray<TIndex, NCapacity>::Bits::set() {
	constexpr Index INDEX = NIndex;
	HFSM_ASSERT(INDEX < _width);

	constexpr Index BIT_UNIT  = INDEX / (sizeof(Unit) * 8);
	constexpr Index BIT_INDEX = INDEX % (sizeof(Unit) * 8);
	constexpr Unit MASK = 1 << BIT_INDEX;

	_storage[BIT_UNIT] |= MASK;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex, ShortIndex NCapacity>
template <ShortIndex NIndex>
void
BitArray<TIndex, NCapacity>::Bits::reset() {
	constexpr Index INDEX = NIndex;
	HFSM_ASSERT(INDEX < _width);

	constexpr Index BIT_UNIT  = INDEX / (sizeof(Unit) * 8);
	constexpr Index BIT_INDEX = INDEX % (sizeof(Unit) * 8);
	constexpr Unit MASK = 1 << BIT_INDEX;

	_storage[BIT_UNIT] &= ~MASK;
}

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
bool
BitArray<TIndex, NCapacity>::Bits::get(const Index index) const {
	HFSM_ASSERT(index < _width);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex, ShortIndex NCapacity>
void
BitArray<TIndex, NCapacity>::Bits::set(const Index index) {
	HFSM_ASSERT(index < _width);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex, ShortIndex NCapacity>
void
BitArray<TIndex, NCapacity>::Bits::reset(const Index index) {
	HFSM_ASSERT(index < _width);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	_storage[unit] &= ~mask;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TIndex, ShortIndex NCapacity>
BitArray<TIndex, NCapacity>::ConstBits::operator bool() const {
	const ShortIndex fullUnits = _width / (sizeof(Unit) * 8);

	for (Index i = 0; i < fullUnits; ++i)
		if (_storage[i])
			return true;

	const ShortIndex bit = _width % (sizeof(Unit) * 8);
	const Unit mask = (1 << bit) - 1;
	const Unit& u = _storage[fullUnits];

	return (u & mask) != 0;
}

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
template <ShortIndex NIndex>
bool
BitArray<TIndex, NCapacity>::ConstBits::get() const {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < _width, "");

	constexpr Index BIT_UNIT  = INDEX / (sizeof(Unit) * 8);
	constexpr Index BIT_INDEX = INDEX % (sizeof(Unit) * 8);
	constexpr Unit MASK = 1 << BIT_INDEX;

	return (_storage[BIT_UNIT] & MASK) != 0;
}

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
bool
BitArray<TIndex, NCapacity>::ConstBits::get(const Index index) const {
	HFSM_ASSERT(index < _width);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TIndex, ShortIndex NCapacity>
void
BitArray<TIndex, NCapacity>::clear() {
	for (Unit& unit: _storage)
		unit = Unit{0};
}

////////////////////////////////////////////////////////////////////////////////

template <typename TIndex, ShortIndex NCapacity>
template <ShortIndex NIndex>
bool
BitArray<TIndex, NCapacity>::get() const {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < CAPACITY, "");

	constexpr Index BIT_UNIT  = INDEX / (sizeof(Unit) * 8);
	constexpr Index BIT_INDEX = INDEX % (sizeof(Unit) * 8);
	constexpr Unit MASK = 1 << BIT_INDEX;

	return (_storage[BIT_UNIT] & MASK) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex, ShortIndex NCapacity>
template <ShortIndex NIndex>
void
BitArray<TIndex, NCapacity>::set() {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < CAPACITY, "");

	constexpr Index BIT_UNIT  = INDEX / (sizeof(Unit) * 8);
	constexpr Index BIT_INDEX = INDEX % (sizeof(Unit) * 8);
	constexpr Unit MASK = 1 << BIT_INDEX;

	_storage[BIT_UNIT] |= MASK;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex, ShortIndex NCapacity>
template <ShortIndex NIndex>
void
BitArray<TIndex, NCapacity>::reset() {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < CAPACITY, "");

	constexpr Index BIT_UNIT  = INDEX / (sizeof(Unit) * 8);
	constexpr Index BIT_INDEX = INDEX % (sizeof(Unit) * 8);
	constexpr Unit MASK = 1 << BIT_INDEX;

	_storage[BIT_UNIT] &= ~MASK;
}

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
bool
BitArray<TIndex, NCapacity>::get(const Index index) const {
	HFSM_ASSERT(index < CAPACITY);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex, ShortIndex NCapacity>
void
BitArray<TIndex, NCapacity>::set(const Index index) {
	HFSM_ASSERT(index < CAPACITY);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//template <typename TIndex, ShortIndex NCapacity>
//void
//BitArray<TIndex, NCapacity>::reset(const Index index) {
//	HFSM_ASSERT(index < CAPACITY);
//
//	const Index unit = index / (sizeof(Unit) * 8);
//	const Index bit  = index % (sizeof(Unit) * 8);
//	const Unit mask = 1 << bit;
//
//	_storage[unit] &= ~mask;
//}

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
template <ShortIndex NUnit, ShortIndex NWidth>
typename BitArray<TIndex, NCapacity>::Bits
BitArray<TIndex, NCapacity>::bits() {
	constexpr ShortIndex UNIT  = NUnit;
	constexpr ShortIndex WIDTH = NWidth;
	static_assert(UNIT + (WIDTH + 7) / (sizeof(Unit) * 8) <= CAPACITY, "");

	return Bits{_storage + UNIT, WIDTH};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex, ShortIndex NCapacity>
template <ShortIndex NUnit, ShortIndex NWidth>
typename BitArray<TIndex, NCapacity>::ConstBits
BitArray<TIndex, NCapacity>::bits() const {
	constexpr ShortIndex UNIT  = NUnit;
	constexpr ShortIndex WIDTH = NWidth;
	static_assert(UNIT + (WIDTH + 7) / (sizeof(Unit) * 8) <= CAPACITY, "");

	return ConstBits{_storage + UNIT, WIDTH};
}

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
typename BitArray<TIndex, NCapacity>::Bits
BitArray<TIndex, NCapacity>::bits(const Units& units) {
	HFSM_ASSERT(units.unit + (units.width + 7) / (sizeof(Unit) * 8) <= CAPACITY);

	return Bits{_storage + units.unit, units.width};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex, ShortIndex NCapacity>
typename BitArray<TIndex, NCapacity>::ConstBits
BitArray<TIndex, NCapacity>::bits(const Units& units) const {
	HFSM_ASSERT(units.unit + (units.width + 7) / (sizeof(Unit) * 8) <= CAPACITY);

	return ConstBits{_storage + units.unit, units.width};
}

////////////////////////////////////////////////////////////////////////////////

}
}
