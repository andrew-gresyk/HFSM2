namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndex>
HFSM2_CONSTEXPR(14)
uint8_t
bitMask(const TIndex bit) noexcept {
	return static_cast<uint8_t>(1u << static_cast<unsigned>(bit));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex>
HFSM2_CONSTEXPR(14)
uint8_t
lowMask(const TIndex bitCount) noexcept {
	return static_cast<uint8_t>((1u << static_cast<unsigned>(bitCount)) - 1u);
}

//------------------------------------------------------------------------------


template <typename TIndex>
HFSM2_CONSTEXPR(14)
bool
bitGet(const uint8_t* const storage,
	   const TIndex index) noexcept
{
	const TIndex unit = index / 8;
	const TIndex bit  = index % 8;

	return (storage[unit] & bitMask(bit)) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex>
HFSM2_CONSTEXPR(14)
void
bitSet(uint8_t* const storage,
	   const TIndex index) noexcept
{
	const TIndex unit = index / 8;
	const TIndex bit  = index % 8;

	storage[unit] |= bitMask(bit);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex>
HFSM2_CONSTEXPR(14)
void
bitClear(uint8_t* const storage,
		 const TIndex index) noexcept
{
	const TIndex unit = index / 8;
	const TIndex bit  = index % 8;

	storage[unit] &= static_cast<uint8_t>(~bitMask(bit));
}

////////////////////////////////////////////////////////////////////////////////

}
}
