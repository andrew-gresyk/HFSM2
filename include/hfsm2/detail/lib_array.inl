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
Array<T, NC>::Array()
	: View(CAPACITY)
{
	HFSM_ASSERT(View::data() == _storage);
}

//------------------------------------------------------------------------------

template <typename T, LongIndex NC>
void
Array<T, NC>::operator = (const Array& other) {
	for (unsigned i = 0; i < CAPACITY; ++i)
		_storage[i] = other._storage[i];
}

////////////////////////////////////////////////////////////////////////////////

}
}
