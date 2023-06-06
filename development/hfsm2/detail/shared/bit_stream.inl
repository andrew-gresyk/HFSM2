#if HFSM2_SERIALIZATION_AVAILABLE()

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <Long NBitCapacity>
HFSM2_CONSTEXPR(14)
bool
StreamBufferT<NBitCapacity>::operator == (const StreamBuffer& buffer) const noexcept {
	for (hfsm2::Long i = 0; i < BYTE_COUNT; ++i)
		if (_data[i] != buffer._data[i])
			return false;

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <Long NBitCapacity>
HFSM2_CONSTEXPR(14)
bool
StreamBufferT<NBitCapacity>::operator != (const StreamBuffer& buffer) const noexcept {
	for (hfsm2::Long i = 0; i < BYTE_COUNT; ++i)
		if (_data[i] != buffer._data[i])
			return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////////

template <Long NBC>
template <Short NBitWidth>
HFSM2_CONSTEXPR(14)
void
BitWriteStreamT<NBC>::write(const UBitWidth<NBitWidth> item) noexcept {
	constexpr Short BIT_WIDTH = NBitWidth;
	static_assert(BIT_WIDTH > 0, "STATIC ASSERT");

	HFSM2_ASSERT(_cursor + BIT_WIDTH <= BIT_CAPACITY);

	using Item = UBitWidth<BIT_WIDTH>;

	Item itemBits = item;

	for (Short itemWidth = BIT_WIDTH; itemWidth; ) {
		const Long	byteIndex		= _cursor >> 3;
		uint8_t&	byte			= _buffer._data[byteIndex];

		const Short byteChunkStart	= _cursor & 0x7;
		const Short byteDataWidth	= 8 - byteChunkStart;
		const Short byteChunkWidth	= min(byteDataWidth, itemWidth);
		const Item	byteChunk		= itemBits << byteChunkStart;

		byte		|= byteChunk;
		itemBits   >>= byteChunkWidth;
		itemWidth	-= byteChunkWidth;
		_cursor		+= byteChunkWidth;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <Long NBC>
template <Short NBitWidth>
HFSM2_CONSTEXPR(14)
UBitWidth<NBitWidth>
BitReadStreamT<NBC>::read() noexcept {
	constexpr Short BIT_WIDTH = NBitWidth;
	static_assert(BIT_WIDTH > 0, "STATIC ASSERT");

	HFSM2_ASSERT(_cursor + BIT_WIDTH <= BIT_CAPACITY);

	using Item = UBitWidth<BIT_WIDTH>;

	Item item = 0;

	for (Short itemCursor = 0, itemWidth = BIT_WIDTH; itemWidth; ) {
		const Long	byteIndex		= _cursor >> 3;
		const uint8_t& byte			= _buffer._data[byteIndex];

		const Short byteChunkStart	= _cursor & 0x7;
		const Short byteDataWidth	= 8 - byteChunkStart;
		const Short byteChunkWidth	= min(byteDataWidth, itemWidth);
		const Short byteChunkMask	= (1 << byteChunkWidth) - 1;

		const Item	byteChunk		= (byte >> byteChunkStart) & byteChunkMask;
		const Item	itemChunk		= byteChunk << itemCursor;

		item		|= itemChunk;
		itemCursor	+= byteChunkWidth;
		itemWidth	-= byteChunkWidth;
		_cursor		+= byteChunkWidth;
	}

	return item;
}

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
