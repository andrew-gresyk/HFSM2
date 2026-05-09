namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NC_>
HFSM2_CONSTEXPR(14)
DynamicArrayT<T, NC_>::DynamicArrayT(const This& other) HFSM2_NOEXCEPT_17(noexcept(Item{other.item(0)})) {
	for (const Item& item : other)
		emplace(item);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC_>
HFSM2_CONSTEXPR(14)
DynamicArrayT<T, NC_>::DynamicArrayT(This&& other) HFSM2_NOEXCEPT_17(noexcept(Item{::hfsm2::move(other.item(0))})) {
	for (Item& item : other)
		emplace(::hfsm2::move(item));

	other.clear();
}

//------------------------------------------------------------------------------

template <typename T, Long NC_>
HFSM2_CONSTEXPR(14)
DynamicArrayT<T, NC_>&
DynamicArrayT<T, NC_>::operator = (const This& other) HFSM2_NOEXCEPT_17(noexcept(Item{other.item(0)})) {
	if (this == &other)
		return *this;

	clear();

	for (const Item& item : other)
		emplace(item);

	return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC_>
HFSM2_CONSTEXPR(14)
DynamicArrayT<T, NC_>&
DynamicArrayT<T, NC_>::operator = (This&& other) HFSM2_NOEXCEPT_17(noexcept(Item{::hfsm2::move(other.item(0))})) {
	if (this == &other)
		return *this;

	clear();

	for (Index i = 0; i < other._count; ++i)
		emplace(::hfsm2::move(other.item(i)));

	other.clear();

	return *this;
}

//------------------------------------------------------------------------------

template <typename T, Long NC_>
template <typename... TArgs>
HFSM2_CONSTEXPR(14)
typename DynamicArrayT<T, NC_>::Index
DynamicArrayT<T, NC_>::emplace(TArgs&&... args) HFSM2_NOEXCEPT_17(noexcept(T{::hfsm2::forward<TArgs>(args)...})) {
	HFSM2_ASSERT(_count < CAPACITY);

	new (&_storage[_count * sizeof(Item)]) Item{::hfsm2::forward<TArgs>(args)...};

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T, Long NC_>
template <typename N>
HFSM2_CONSTEXPR(14)
typename DynamicArrayT<T, NC_>::Item&
DynamicArrayT<T, NC_>::operator[] (const N index) noexcept {
	HFSM2_ASSERT(0 <= index && index < _count);

	return item(static_cast<Index>(index));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NC_>
template <typename N>
HFSM2_CONSTEXPR(14)
const typename DynamicArrayT<T, NC_>::Item&
DynamicArrayT<T, NC_>::operator[] (const N index) const noexcept {
	HFSM2_ASSERT(0 <= index && index < _count);

	return item(static_cast<Index>(index));
}

//------------------------------------------------------------------------------

template <typename T, Long NC_>
HFSM2_CONSTEXPR(14)
void
DynamicArrayT<T, NC_>::clear() noexcept {
	for (Index i = _count; i > 0; --i)
		item(i - 1).~Item();

	_count = 0;
}

//------------------------------------------------------------------------------

template <typename T, Long NC_>
template <Long N>
HFSM2_CONSTEXPR(14)
DynamicArrayT<T, NC_>&
DynamicArrayT<T, NC_>::operator += (const DynamicArrayT<T, N>& other) HFSM2_NOEXCEPT_17(noexcept(Item{*other.begin()})) {
	HFSM2_ASSERT(static_cast<const void*>(this) != static_cast<const void*>(&other));
	HFSM2_ASSERT(_count + other.count() <= CAPACITY);

	for (const auto& item : other)
		emplace(item);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}
}
