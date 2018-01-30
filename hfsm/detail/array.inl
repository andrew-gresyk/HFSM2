namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, unsigned TCapacity>
T&
StaticArray<T, TCapacity>::operator[] (const unsigned i) {
	assert(0 <= i && i < CAPACITY);

	return _items[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, unsigned TCapacity>
const T&
StaticArray<T, TCapacity>::operator[] (const unsigned i) const {
	assert(0 <= i && i < CAPACITY);

	return _items[i];
}

////////////////////////////////////////////////////////////////////////////////

}
}
