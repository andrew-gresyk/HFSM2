namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndex>
BitArrayT<TIndex>::BitArrayT(const Index capacity_)
	: capacity{capacity_}
{
	clear();
}
//------------------------------------------------------------------------------

template <typename TIndex>
void
BitArrayT<TIndex>::clear() {
	StorageUnit* const s = storage();

	const LongIndex count = storageUnitCount();
	for (LongIndex i = 0; i < count; ++i)
		s[i] = 0;
}

//------------------------------------------------------------------------------

template <typename TIndex>
BitArrayT<TIndex>::operator bool() const {
	StorageUnit result = 0;

	const StorageUnit* const s = storage();

	const LongIndex count = storageUnitCount();
	for (LongIndex i = 0; i < count; ++i)
		result |= s[i];

	return result != 0;
}

//------------------------------------------------------------------------------

template <typename TIndex>
bool
BitArrayT<TIndex>::get(const Index i) const {
	HFSM_ASSERT(i < capacity);

	const auto d = div(i, sizeof(StorageUnit) * 8);

	const StorageUnit& unit = storage()[d.quot];
	const Index mask = 1 << d.rem;

	return (unit & mask) != 0;
}

//------------------------------------------------------------------------------

template <typename TIndex>
void
BitArrayT<TIndex>::set(const Index i, const bool value) {
	HFSM_ASSERT(i < capacity);

	const auto d = div(i, sizeof(StorageUnit) * 8);

	StorageUnit& unit = storage()[d.quot];

	if (value) {
		const Index mask = 1 << d.rem;
		unit |= mask;
	} else {
		const Index mask = ~(1 << d.rem);
		unit &= mask;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TIndex, TIndex NCapacity>
BitArrayStorageT<TIndex, NCapacity>::BitArrayStorageT()
	: BitArray{CAPACITY}
{
	HFSM_ASSERT(BitArray::storage() == _storage);
}

////////////////////////////////////////////////////////////////////////////////

}
}
