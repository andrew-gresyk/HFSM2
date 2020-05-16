#pragma once

namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

template <LongIndex NBitCapacity>
struct StreamBuffer {
	static constexpr LongIndex BIT_CAPACITY	= NBitCapacity;
	static constexpr LongIndex BYTE_COUNT	= roundUp(BIT_CAPACITY, 8);

	using Size = UnsignedCapacity<BIT_CAPACITY>;
	using Data = uint8_t[BYTE_COUNT];

	void clear();

	//Size write(const uint8_t byte);

	Size bitSize;
	Data payload;
};

////////////////////////////////////////////////////////////////////////////////

template <LongIndex NBitCapacity>
class BitWriteStream final {
public:
	static constexpr LongIndex BIT_CAPACITY = NBitCapacity;

	using Buffer = StreamBuffer<BIT_CAPACITY>;

public:
	BitWriteStream(Buffer& _buffer);

	template <ShortIndex NBitWidth>
	void write(const UnsignedBitWidth<NBitWidth> item);

private:
	Buffer& _buffer;
};

//------------------------------------------------------------------------------

template <LongIndex NBitCapacity>
class BitReadStream final {
public:
	static constexpr LongIndex BIT_CAPACITY = NBitCapacity;

	using Buffer = StreamBuffer<BIT_CAPACITY>;

public:
	BitReadStream(const Buffer& buffer)
		: _buffer{buffer}
	{}

	template <ShortIndex NBitWidth>
	UnsignedBitWidth<NBitWidth> read();

	LongIndex cursor() const								{ return _cursor;	}

private:
	const Buffer& _buffer;

	LongIndex _cursor = 0;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "bit_stream.inl"
