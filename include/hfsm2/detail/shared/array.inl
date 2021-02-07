namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC>
StaticArrayT<T, NC>::StaticArrayT(const Item filler) noexcept {
	fill(filler);
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
template <typename N>
T&
StaticArrayT<T, NC>::operator[] (const N i) noexcept {
	HFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
const T&
StaticArrayT<T, NC>::operator[] (const N i) const noexcept {
	HFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
void
StaticArrayT<T, NC>::fill(const Item filler) noexcept {
	for (Index i = 0; i < CAPACITY; ++i)
		_items[i] = filler;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC>
template <typename TValue>
typename ArrayT<T, NC>::Index
ArrayT<T, NC>::append(const TValue& value) noexcept {
	HFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{value};

	return _count++;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename TValue>
typename ArrayT<T, NC>::Index
ArrayT<T, NC>::append(TValue&& value) noexcept {
	HFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{std::move(value)};

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
template <typename N>
T&
ArrayT<T, NC>::operator[] (const N i) noexcept {
	HFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
const T&
ArrayT<T, NC>::operator[] (const N i) const noexcept {
	HFSM2_ASSERT(0 <= i && i < CAPACITY);

	return _items[(Index) i];
}

//------------------------------------------------------------------------------
// SPECIFIC
// SPECIFIC

template <typename T, Long NC>
template <Long N>
ArrayT<T, NC>&
ArrayT<T, NC>::operator += (const ArrayT<T, N>& other) noexcept {
	for (const auto& item : other)
		append(item);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}
}
