namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <LongIndex NBC>
void
StreamBuffer<NBC>::clear() {
	bitSize = 0;
	fill(payload, 0);
}

////////////////////////////////////////////////////////////////////////////////

template <LongIndex NBC>
BitWriteStream<NBC>::BitWriteStream(Buffer& buffer)
	: _buffer{buffer}
{
	_buffer.clear();
}

//------------------------------------------------------------------------------

template <LongIndex NBC>
template <ShortIndex NBitWidth>
void
BitWriteStream<NBC>::write(const UnsignedBitWidth<NBitWidth> item) {
	constexpr ShortIndex BIT_WIDTH = NBitWidth;
	static_assert(BIT_WIDTH > 0, "STATIC ASSERT");

	HFSM_ASSERT(_buffer.bitSize + BIT_WIDTH < BIT_CAPACITY);

	using Item = UnsignedBitWidth<BIT_WIDTH>;

	Item itemBits = item;

	for (ShortIndex itemWidth = BIT_WIDTH; itemWidth; ) {
		const LongIndex	 byteIndex		= _buffer.bitSize >> 3;
		uint8_t& byte = _buffer.payload[byteIndex];

		const ShortIndex byteChunkStart	= _buffer.bitSize & 0x7;
		const ShortIndex byteDataWidth	= 8 - byteChunkStart;
		const ShortIndex byteChunkWidth	= detail::min(byteDataWidth, itemWidth);
		const Item		 byteChunk		= itemBits << byteChunkStart;
		byte |= byteChunk;

		itemBits	>>= byteChunkWidth;
		itemWidth	 -= byteChunkWidth;
		_buffer.bitSize += byteChunkWidth;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <LongIndex NBC>
template <ShortIndex NBitWidth>
UnsignedBitWidth<NBitWidth>
BitReadStream<NBC>::read() {
	constexpr ShortIndex BIT_WIDTH = NBitWidth;
	static_assert(BIT_WIDTH > 0, "STATIC ASSERT");

	using Item = UnsignedBitWidth<BIT_WIDTH>;

	Item item = 0;
	ShortIndex itemCursor = 0;

	for (ShortIndex itemWidth = BIT_WIDTH; itemWidth; )
		if (HFSM_CHECKED(_cursor + itemWidth <= _buffer.bitSize)) {
			const LongIndex	 byteIndex		= _cursor >> 3;
			const uint8_t& byte = _buffer.payload[byteIndex];

			const ShortIndex byteChunkStart	= _cursor & 0x7;
			const ShortIndex byteDataWidth	= 8 - byteChunkStart;
			const ShortIndex byteChunkWidth	= detail::min(byteDataWidth, itemWidth);
			const ShortIndex byteChunkMask	= (1 << byteChunkWidth) - 1;
			const Item		 byteChunk		= (byte >> byteChunkStart) & byteChunkMask;

			const Item		 itemChunk		= byteChunk << itemCursor;
			item |= itemChunk;

			_cursor	   += byteChunkWidth;
			itemCursor += byteChunkWidth;
			itemWidth  -= byteChunkWidth;
		}

	return item;
}

////////////////////////////////////////////////////////////////////////////////

}
}
