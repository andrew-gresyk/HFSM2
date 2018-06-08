namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TK, typename TV, unsigned TC, typename TH>
HashTable<TK, TV, TC, TH>::Item::Item(const Hash hash, const Key key)
	: _hash(hash | USED_MASK)
	, _keyWrap(key)
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TK, typename TV, unsigned TC, typename TH>
HashTable<TK, TV, TC, TH>::Item::Item(const Hash hash, const Key key, const Value value)
	: _hash(hash | USED_MASK)
	, _keyWrap(key)
	, _value(value)
{}

//------------------------------------------------------------------------------

template <typename TK, typename TV, unsigned TC, typename TH>
void
HashTable<TK, TV, TC, TH>::Item::swap(Item& other) {
	std::swap(_hash, other._hash);
	std::swap(_keyWrap, other._keyWrap);
	std::swap(_value, other._value);
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, unsigned TC, typename TH>
bool
HashTable<TK, TV, TC, TH>::Item::operator == (const Item& other) const {
	return _hash == other._hash && *_keyWrap == *other._keyWrap;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TK, typename TV, unsigned TC, typename TH>
bool
HashTable<TK, TV, TC, TH>::insert(const Key key, const Value value) {
	assert(_count < CAPACITY);

	if (_count < CAPACITY) {
		Item newcomer(hash(key), key, value);

		for (unsigned i = index(newcomer.hash()), newcomerDistance = 0;
			 ;
			 i = index(i + 1), ++newcomerDistance)
		{
			Item& resident = _items[i];

			if (resident.occupied()) {
				const unsigned residentDistance = probeCount(i);

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

template <typename TK, typename TV, unsigned TC, typename TH>
typename HashTable<TK, TV, TC, TH>::Value*
HashTable<TK, TV, TC, TH>::find(const Key key) {
	const unsigned index = locate(key);

	return index != INVALID ?
		_items[index].value() : nullptr;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TK, typename TV, unsigned TC, typename TH>
const typename HashTable<TK, TV, TC, TH>::Value*
HashTable<TK, TV, TC, TH>::find(const Key key) const {
	const unsigned index = locate(key);

	return index != INVALID ?
		_items[index].value() : nullptr;
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, unsigned TC, typename TH>
unsigned
HashTable<TK, TV, TC, TH>::locate(const Key key) const {
	const Item item(hash(key), key);

	for (unsigned i = index(item.hash()), distance = 0;
		 ;
		 i = index(i + 1), ++distance)
	{
		const Item& resident = _items[i];

		if (item == resident)
			return i;
		else if (distance > probeCount(i))
			return INVALID;

		assert(distance < CAPACITY);
	}
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, unsigned TC, typename TH>
unsigned
HashTable<TK, TV, TC, TH>::probeCount(const unsigned i) const {
	assert(i < CAPACITY);

	return index(CAPACITY + i - index(_items[i].hash()));
}

//------------------------------------------------------------------------------

template <typename TK, typename TV, unsigned TC, typename TH>
unsigned
HashTable<TK, TV, TC, TH>::skipVacantForward(const unsigned i) const {
	assert(0 <= i && i <= CAPACITY);

	if (i < CAPACITY) {
		unsigned n = i;
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
