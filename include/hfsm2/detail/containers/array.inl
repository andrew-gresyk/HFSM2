namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC>
template <typename N>
HFSM2_CONSTEXPR(14)
T&
StaticArrayT<T, NC>::operator[] (const N index) noexcept	{
	HFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[(Index) index];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
HFSM2_CONSTEXPR(11)
const T&
StaticArrayT<T, NC>::operator[] (const N index) const noexcept	{
	HFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[(Index) index];
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
HFSM2_CONSTEXPR(14)
void
StaticArrayT<T, NC>::fill(const Item filler) noexcept {
	for (Item& item : _items)
		item = filler;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC>
template <typename... TArgs>
HFSM2_CONSTEXPR(14)
typename ArrayT<T, NC>::Index
ArrayT<T, NC>::emplace(const TArgs&... args) noexcept {
	HFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{args...};

	return _count++;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename... TArgs>
HFSM2_CONSTEXPR(14)
typename ArrayT<T, NC>::Index
ArrayT<T, NC>::emplace(TArgs&&... args) noexcept {
	HFSM2_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{forward<TArgs>(args)...};

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T, Long NC>
template <typename N>
HFSM2_CONSTEXPR(14)
typename ArrayT<T, NC>::Item&
ArrayT<T, NC>::operator[] (const N index) noexcept {
	HFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[(Index) index];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <typename N>
HFSM2_CONSTEXPR(11)
const typename ArrayT<T, NC>::Item&
ArrayT<T, NC>::operator[] (const N index) const noexcept {
	HFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[(Index) index];
}

//------------------------------------------------------------------------------
// SPECIFIC
// SPECIFIC
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC>
template <Long N>
HFSM2_CONSTEXPR(14)
ArrayT<T, NC>&
ArrayT<T, NC>::operator += (const ArrayT<T, N>& other) noexcept {
	for (const auto& item : other)
		emplace(item);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}
}
