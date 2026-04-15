namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
void
BitFlatSetT<NCapacity>::set() noexcept {
	for (uint8_t& unit : _storage)
		unit = UINT8_MAX;

	constexpr Index lastBits = CAPACITY % 8;

#if defined _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4127) // conditional expression is constant
#endif

	if (lastBits != 0)
		_storage[UNIT_COUNT - 1] = lowMask(lastBits);

#if defined _MSC_VER
	#pragma warning(pop)
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
void
BitFlatSetT<NCapacity>::clear() noexcept {
	for (uint8_t& unit : _storage)
		unit = uint8_t{0};
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
bool
BitFlatSetT<NCapacity>::empty() const noexcept {
	for (Index i = 0; i < UNIT_COUNT; ++i)
		if (_storage[i])
			return false;

	return true;
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
template <Short NIndex>
HFSM2_CONSTEXPR(14)
bool
BitFlatSetT<NCapacity>::get() const noexcept {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < CAPACITY, "");

	return bitGet(_storage, INDEX);
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
template <typename TIndex>
HFSM2_CONSTEXPR(14)
bool
BitFlatSetT<NCapacity>::get(const TIndex index) const noexcept {
	HFSM2_ASSERT(index < CAPACITY);

	return bitGet(_storage, static_cast<Index>(index));
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
template <typename TIndex>
HFSM2_CONSTEXPR(14)
void
BitFlatSetT<NCapacity>::set(const TIndex index) noexcept {
	HFSM2_ASSERT(index < CAPACITY);

	bitSet(_storage, static_cast<Index>(index));
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
template <typename TIndex>
HFSM2_CONSTEXPR(14)
void
BitFlatSetT<NCapacity>::clear(const TIndex index) noexcept {
	HFSM2_ASSERT(index < CAPACITY);

	bitClear(_storage, static_cast<Index>(index));
}

//------------------------------------------------------------------------------

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
bool
BitFlatSetT<NCapacity>::operator & (const This& other) const noexcept {
	for (Index i = 0; i < UNIT_COUNT; ++i)
		if ((_storage[i] & other._storage[i]) == 0)
			return false;

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <unsigned NCapacity>
HFSM2_CONSTEXPR(14)
void
BitFlatSetT<NCapacity>::operator &= (const This& other) noexcept {
	for (Index i = 0; i < UNIT_COUNT; ++i)
		_storage[i] &= other._storage[i];
}

////////////////////////////////////////////////////////////////////////////////

}
}
