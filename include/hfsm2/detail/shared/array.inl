namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, LongIndex NC>
StaticArray<T, NC>::StaticArray(const Item filler) {
	fill(filler);
}

//------------------------------------------------------------------------------

template <typename T, LongIndex NC>
T&
StaticArray<T, NC>::operator[] (const LongIndex i) {
	HFSM_ASSERT(0 <= i && i < CAPACITY);

	return _items[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, LongIndex NC>
const T&
StaticArray<T, NC>::operator[] (const LongIndex i) const {
	HFSM_ASSERT(0 <= i && i < CAPACITY);

	return _items[i];
}

//------------------------------------------------------------------------------

template <typename T, LongIndex NC>
void
StaticArray<T, NC>::fill(const Item filler) {
	for (LongIndex i = 0; i < CAPACITY; ++i)
		_items[i] = filler;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, LongIndex NC>
template <typename TValue>
LongIndex
Array<T, NC>::operator << (TValue&& value) {
	HFSM_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{std::move(value)};

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T, LongIndex NC>
T&
Array<T, NC>::operator[] (const LongIndex i) {
	HFSM_ASSERT(0 <= i && i < CAPACITY);

	return _items[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, LongIndex NC>
const T&
Array<T, NC>::operator[] (const LongIndex i) const {
	HFSM_ASSERT(0 <= i && i < CAPACITY);

	return _items[i];
}

////////////////////////////////////////////////////////////////////////////////

}
}
