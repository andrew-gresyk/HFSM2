#ifdef HFSM2_ENABLE_SERIALIZATION

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <Long NBC>
void
StreamBufferT<NBC>::clear() noexcept {
	fill(_data, 0);
}

////////////////////////////////////////////////////////////////////////////////

template <Long NBC>
BitWriteStreamT<NBC>::BitWriteStreamT(Buffer& buffer,
									  const Long cursor) noexcept
	: _buffer{buffer}
	, _cursor{cursor}
{
	_buffer.clear();
}

//------------------------------------------------------------------------------

template <Long NBC>
template <Short NBitWidth>
void
BitWriteStreamT<NBC>::write(const UnsignedBitWidth<NBitWidth> item) noexcept {
	constexpr Short BIT_WIDTH = NBitWidth;
	static_assert(BIT_WIDTH > 0, "STATIC ASSERT");

	HFSM2_ASSERT(_cursor + BIT_WIDTH <= BIT_CAPACITY);

	using Item = UnsignedBitWidth<BIT_WIDTH>;

	Item itemBits = item;

	for (Short itemWidth = BIT_WIDTH; itemWidth; ) {
		const Long	byteIndex		= _cursor >> 3;
		uint8_t&	byte			= _buffer._data[byteIndex];

		const Short byteChunkStart	= _cursor & 0x7;
		const Short byteDataWidth	= 8 - byteChunkStart;
		const Short byteChunkWidth	= detail::min(byteDataWidth, itemWidth);
		const Item	byteChunk		= itemBits << byteChunkStart;

		byte		|= byteChunk;
		itemBits   >>= byteChunkWidth;
		itemWidth	-= byteChunkWidth;
		_cursor		+= byteChunkWidth;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <Long NBC>
BitReadStreamT<NBC>::BitReadStreamT(const Buffer& buffer,
									const Long cursor) noexcept
	: _buffer{buffer}
	, _cursor{cursor}
{}

//------------------------------------------------------------------------------

template <Long NBC>
template <Short NBitWidth>
UnsignedBitWidth<NBitWidth>
BitReadStreamT<NBC>::read() noexcept {
	constexpr Short BIT_WIDTH = NBitWidth;
	static_assert(BIT_WIDTH > 0, "STATIC ASSERT");

	HFSM2_ASSERT(_cursor <= BIT_CAPACITY);

	using Item = UnsignedBitWidth<BIT_WIDTH>;

	Item item = 0;
	Short itemCursor = 0;

	for (Short itemWidth = BIT_WIDTH; itemWidth; )
		if (HFSM2_CHECKED(_cursor + itemWidth <= BIT_CAPACITY)) {
			const Long	byteIndex		= _cursor >> 3;
			const uint8_t& byte			= _buffer._data[byteIndex];

			const Short byteChunkStart	= _cursor & 0x7;
			const Short byteDataWidth	= 8 - byteChunkStart;
			const Short byteChunkWidth	= detail::min(byteDataWidth, itemWidth);
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
