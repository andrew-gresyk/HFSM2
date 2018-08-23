namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

Bit::operator bool() const {
	return _array.get(_index);
}

//------------------------------------------------------------------------------

void
Bit::operator = (const bool value) {
	_array.set(_index, value);
}

//------------------------------------------------------------------------------

ConstBit::operator bool() const {
	return _array.get(_index);
}

////////////////////////////////////////////////////////////////////////////////

void
BitArray::clear() {
	StorageUnit* const s = storage();

	const ShortIndex count = storageUnitCount();
	for (ShortIndex i = 0; i < count; ++i)
		s[i] = 0;
}

//------------------------------------------------------------------------------

BitArray::operator bool() const {
	StorageUnit result = 0;

	const StorageUnit* const s = storage();

	const ShortIndex count = storageUnitCount();
	for (ShortIndex i = 0; i < count; ++i)
		result |= s[i];

	return result != 0;
}

//------------------------------------------------------------------------------

bool
BitArray::get(const ShortIndex i) const {
	assert(i < capacity);

	const auto d = div(i, sizeof(StorageUnit) * 8);

	const StorageUnit& unit = storage()[d.quot];
	const ShortIndex mask = 1 << d.rem;

	return (unit & mask) != 0;
}

//------------------------------------------------------------------------------

void
BitArray::set(const ShortIndex i, const bool value) {
	assert(i < capacity);

	const auto d = div(i, sizeof(StorageUnit) * 8);

	StorageUnit& unit = storage()[d.quot];

	if (value) {
		const ShortIndex mask = 1 << d.rem;
		unit |= mask;
	} else {
		const ShortIndex mask = ~(1 << d.rem);
		unit &= mask;
	}
}

////////////////////////////////////////////////////////////////////////////////

}
}
