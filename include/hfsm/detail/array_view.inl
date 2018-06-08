namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
ArrayView<T>::ArrayView(const unsigned capacity)
	: _capacity(capacity)
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
ArrayView<T>::~ArrayView() {
	if (_count > 0)
		for (int i = _count - 1; i >= 0; --i)
			get(i).~Item();
}

//------------------------------------------------------------------------------

template <typename T>
unsigned
ArrayView<T>::resize(const unsigned count) {
	const unsigned clampedCount = count < _capacity ?
		count : _capacity;

	if (clampedCount > _count) {
		for (unsigned i = _count; i < clampedCount; ++i)
			get(i) = Item();
	}
	else if (clampedCount < _count) {
		for (unsigned i = _count - 1; i >= clampedCount; --i)
			get(i).~Item();
	}

	return _count = clampedCount;
}

//------------------------------------------------------------------------------

template <typename T>
template <typename TValue>
unsigned
ArrayView<T>::operator << (TValue&& value) {
	assert(_count < _capacity);

	new (&get(_count)) Item(std::move(value));

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T>
T&
ArrayView<T>::get(const unsigned i) {
	assert(0 <= i && i < _capacity);

	return data()[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
const T&
ArrayView<T>::get(const unsigned i) const {
	assert(0 <= i && i < _capacity);

	return data()[i];
}

////////////////////////////////////////////////////////////////////////////////

}
}
