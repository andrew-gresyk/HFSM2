namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC_>
template <typename N>
HFSM2_CONSTEXPR(14)
T&
StaticArrayT<T, NC_>::operator[] (const N index) noexcept {
	HFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[static_cast<Index>(index)];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC_>
template <typename N>
HFSM2_CONSTEXPR(14)
const T&
StaticArrayT<T, NC_>::operator[] (const N index) const noexcept	{
	HFSM2_ASSERT(0 <= index && index < CAPACITY);

	return _items[static_cast<Index>(index)];
}

//------------------------------------------------------------------------------

template <typename T, Long NC_>
HFSM2_CONSTEXPR(14)
void
StaticArrayT<T, NC_>::fill(const Item filler) noexcept {
	for (Item& item : _items)
		item = filler;
}

//------------------------------------------------------------------------------

template <typename T, Long NC_>
HFSM2_CONSTEXPR(14)
bool
StaticArrayT<T, NC_>::operator == (const This& other) const noexcept {
	for (Index i = 0; i < CAPACITY; ++i)
		if (_items[i] != other._items[i])
			return false;

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC_>
HFSM2_CONSTEXPR(14)
bool
StaticArrayT<T, NC_>::operator != (const This& other) const noexcept {
	for (Index i = 0; i < CAPACITY; ++i)
		if (_items[i] != other._items[i])
			return true;

	return false;
}

//------------------------------------------------------------------------------

template <typename T, Long NC_>
HFSM2_CONSTEXPR(14)
bool
StaticArrayT<T, NC_>::empty() const noexcept {
	for (const Item& item : _items)
		if (item != filler<Item>())
			return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////

}
}
