namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TK, typename TV, LongIndex TC, typename TH>
HashTable<TK, TV, TC, TH>::Item::Item(const Hash hash, const Key key)
	: _keyWrap(key)
	, _hash(hash)
{}

//------------------------------------------------------------------------------

template <typename TK, typename TV, LongIndex TC, typename TH>
void
HashTable<TK, TV, TC, TH>::Item::swap(Item& other) {
	std::swap(_hash,    other._hash);
	std::swap(_keyWrap, other._keyWrap);
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, LongIndex TC, typename TH>
bool
HashTable<TK, TV, TC, TH>::Item::operator == (const Item& other) const {
	return _hash == other._hash &&
		   *_keyWrap == *other._keyWrap;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TK, typename TV, LongIndex TC, typename TH>
bool
HashTable<TK, TV, TC, TH>::insert(const Key key) {
	assert(_count < CAPACITY);

	if (_count < CAPACITY) {
		Item newcomer(hash(key), key);

		for (LongIndex i = index(newcomer.hash()), newcomerDistance = 0;
			 ;
			 i = index(i + 1), ++newcomerDistance)
		{
			Item& resident = _items[i];

			if (resident.occupied()) {
				const auto residentDistance = probeCount(i);

				if (newcomerDistance > residentDistance) {
					newcomerDistance = residentDistance;

					std::swap(newcomer, resident);
				}
			}
			else {
				resident = newcomer;
				++_count;

				return true;
			}

			assert(newcomerDistance < CAPACITY);
		}
	}
	else
		return false;
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, LongIndex TC, typename TH>
typename HashTable<TK, TV, TC, TH>::Value*
HashTable<TK, TV, TC, TH>::find(const Key key) {
	const auto index = locate(key);

	return index != INVALID ?
		_items[index].value() : nullptr;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TK, typename TV, LongIndex TC, typename TH>
const typename HashTable<TK, TV, TC, TH>::Value*
HashTable<TK, TV, TC, TH>::find(const Key key) const {
	const auto index = locate(key);

	return index != INVALID ?
		_items[index].value() : nullptr;
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, LongIndex TC, typename TH>
LongIndex
HashTable<TK, TV, TC, TH>::locate(const Key key) const {
	const auto keyHash = hash(key);

	for (LongIndex i = index(keyHash), distance = 0;
		 ;
		 i = index(i + 1), ++distance)
	{
		if (_items[i].key() == key)
			return i;
		else if (distance > CAPACITY)
			return INVALID;
	}
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, LongIndex TC, typename TH>
LongIndex
HashTable<TK, TV, TC, TH>::probeCount(const LongIndex i) const {
	assert(i < CAPACITY);

	return index(CAPACITY + i - index(_items[i].hash()));
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, LongIndex TC, typename TH>
LongIndex
HashTable<TK, TV, TC, TH>::skipVacantForward(const LongIndex i) const {
	assert(0 <= i && i <= CAPACITY);

	if (i < CAPACITY) {
		LongIndex n = i;
		for (; _items[n].vacant(); ++n)
			if (n < CAPACITY)
				continue;
			else
				return CAPACITY;

		return n;
	}
	else
		return i;
}

////////////////////////////////////////////////////////////////////////////////

}
}
