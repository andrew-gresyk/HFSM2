namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, LongIndex NC_>
StaticArray<T, NC_>::StaticArray(const Item filler) {
	fill(filler);
}

//------------------------------------------------------------------------------

template <typename T, LongIndex NC_>
template <typename N>
T&
StaticArray<T, NC_>::operator[] (const N i) {
	HFSM_ASSERT(0 <= i && i < CAPACITY);

	return _items[(LongIndex) i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, LongIndex NC_>
template <typename N>
const T&
StaticArray<T, NC_>::operator[] (const N i) const {
	HFSM_ASSERT(0 <= i && i < CAPACITY);

	return _items[(LongIndex) i];
}

//------------------------------------------------------------------------------

template <typename T, LongIndex NC_>
void
StaticArray<T, NC_>::fill(const Item filler) {
	for (LongIndex i = 0; i < CAPACITY; ++i)
		_items[i] = filler;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, LongIndex NC_>
template <typename TValue>
LongIndex
Array<T, NC_>::append(TValue&& value) {
	HFSM_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{std::move(value)};

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T, LongIndex NC_>
template <typename N>
T&
Array<T, NC_>::operator[] (const N i) {
	HFSM_ASSERT(0 <= i && i < CAPACITY);

	return _items[(LongIndex) i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, LongIndex NC_>
template <typename N>
const T&
Array<T, NC_>::operator[] (const N i) const {
	HFSM_ASSERT(0 <= i && i < CAPACITY);

	return _items[(LongIndex) i];
}

////////////////////////////////////////////////////////////////////////////////

}
}
