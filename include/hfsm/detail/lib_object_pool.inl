namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TI, ShortIndex NIC, LongIndex NSS>
ObjectPool<TI, NIC, NSS>::ObjectPool() {
	_offsets[0] = 0;
}

//------------------------------------------------------------------------------

template <typename TI, ShortIndex NIC, LongIndex NSS>
TI&
ObjectPool<TI, NIC, NSS>::operator[] (const ShortIndex i) {
	assert(i < _count);

	const ShortIndex offset = _offsets[i];
	assert(offset + sizeof(Interface) <= STORAGE_SIZE);

	return *reinterpret_cast<Interface*>(_storage + offset);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI, ShortIndex NIC, LongIndex NSS>
const TI&
ObjectPool<TI, NIC, NSS>::operator[] (const ShortIndex i) const {
	assert(i < _count);

	const ShortIndex offset = _offsets[i];
	assert(offset + sizeof(Interface) <= STORAGE_SIZE);

	return *reinterpret_cast<const Interface*>(_storage + offset);
}

//------------------------------------------------------------------------------

template <typename TI, ShortIndex NIC, LongIndex NSS>
template <typename TItem, typename... TArgs>
ShortIndex
ObjectPool<TI, NIC, NSS>::emplace(TArgs&&... args) {
	ShortIndex result = _count;

	using Item = TItem;
	static constexpr LongIndex SIZE = sizeof(Item);

	assert(_count < ITEM_COUNT);

	const ShortIndex offset = _offsets[_count];
	assert(offset + SIZE <= STORAGE_SIZE);

	new (_storage + offset) Item{std::forward<TArgs>(args)...};

	++_count;
	if (_count < ITEM_COUNT)
		_offsets[_count] = offset + SIZE;

	return result;
}

////////////////////////////////////////////////////////////////////////////////

}
}
