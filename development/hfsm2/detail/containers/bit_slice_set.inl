namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndex>
HFSM2_CONSTEXPR(14)
bool
viewAny(const uint8_t* const storage,
		const TIndex width) noexcept
{
	const TIndex fullUnits = width / 8;

	for (TIndex i = 0; i < fullUnits; ++i)
		if (storage[i])
			return true;

	const TIndex bit = width % 8;
	if (bit == 0)
		return false;

	return (storage[fullUnits] & lowMask(bit)) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex>
HFSM2_CONSTEXPR(14)
void
viewClear(uint8_t* const storage,
		  const TIndex width) noexcept
{
	const TIndex fullUnits = width / 8;

	for (TIndex i = 0; i < fullUnits; ++i)
		storage[i] = uint8_t{0};

	const TIndex trailing = width % 8;
	if (trailing != 0)
		storage[fullUnits] &= static_cast<uint8_t>(~lowMask(trailing));
}

////////////////////////////////////////////////////////////////////////////////

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
BitSliceSetT<NCapacity>::Bits::operator bool() const noexcept {
	return viewAny(_storage, _width);
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
void
BitSliceSetT<NCapacity>::Bits::clear() noexcept {
	viewClear(_storage, _width);
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
void
BitSliceSetT<NCapacity>::Bits::set(const Index index) noexcept {
	HFSM2_ASSERT(index < _width);

	bitSet(_storage, index);
}

////////////////////////////////////////////////////////////////////////////////

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
BitSliceSetT<NCapacity>::CBits::operator bool() const noexcept {
	return viewAny(_storage, _width);
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
bool
BitSliceSetT<NCapacity>::CBits::get(const Index index) const noexcept {
	HFSM2_ASSERT(index < _width);

	return bitGet(_storage, index);
}

////////////////////////////////////////////////////////////////////////////////

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
void
BitSliceSetT<NCapacity>::clear() noexcept {
	for (uint8_t& unit : _storage)
		unit = uint8_t{0};
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
bool
BitSliceSetT<NCapacity>::empty() const noexcept {
	for (Index i = 0; i < UNIT_COUNT; ++i)
		if (_storage[i])
			return false;

	return true;
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
template <Short NUnit, Short NWidth>
HFSM2_CONSTEXPR(14)
typename BitSliceSetT<NCapacity>::Bits
BitSliceSetT<NCapacity>::bits() noexcept {
	constexpr Short UNIT  = NUnit;
	constexpr Short WIDTH = NWidth;
	static_assert(UNIT + ceilingDivide(WIDTH, 8) <= UNIT_COUNT, "");

	return Bits{_storage + UNIT, WIDTH};
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
template <Short NUnit, Short NWidth>
HFSM2_CONSTEXPR(14)
typename BitSliceSetT<NCapacity>::CBits
BitSliceSetT<NCapacity>::cbits() const noexcept {
	constexpr Short UNIT  = NUnit;
	constexpr Short WIDTH = NWidth;
	static_assert(UNIT + ceilingDivide(WIDTH, 8) <= UNIT_COUNT, "");

	return CBits{_storage + UNIT, WIDTH};
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
typename BitSliceSetT<NCapacity>::Bits
BitSliceSetT<NCapacity>::bits(const BitSlice& units) noexcept {
	HFSM2_ASSERT(units.byteOffset + ceilingDivide(units.bitWidth, 8) <= UNIT_COUNT);

	return Bits{_storage + units.byteOffset, units.bitWidth};
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
bool
BitSliceSetT<NCapacity>::operator != (const This& other) const noexcept {
	for (Index i = 0; i < UNIT_COUNT; ++i)
		if (_storage[i] != other._storage[i])
			return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////////

}
}
