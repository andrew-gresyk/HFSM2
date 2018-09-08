namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, LongIndex NCapacity>
T&
StaticArray<T, NCapacity>::operator[] (const LongIndex i) {
	HFSM_ASSERT(0 <= i && i < CAPACITY);

	return _items[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, LongIndex NCapacity>
const T&
StaticArray<T, NCapacity>::operator[] (const LongIndex i) const {
	HFSM_ASSERT(0 <= i && i < CAPACITY);

	return _items[i];
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, LongIndex NCapacity>
Array<T, NCapacity>::Array()
	: View(CAPACITY)
{
	HFSM_ASSERT(View::data() == _storage);
}

////////////////////////////////////////////////////////////////////////////////

}
}
