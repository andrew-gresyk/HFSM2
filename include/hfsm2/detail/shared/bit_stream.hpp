namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

template <Long NBitCapacity>
struct StreamBuffer {
	static constexpr Long BIT_CAPACITY	= NBitCapacity;
	static constexpr Long BYTE_COUNT	= contain(BIT_CAPACITY, 8u);

	using Size = UnsignedCapacity<BIT_CAPACITY>;
	using Data = uint8_t[BYTE_COUNT];

	void clear()										  noexcept;

	//Size write(const uint8_t byte)					  noexcept;

	Size bitSize;
	Data payload;
};

////////////////////////////////////////////////////////////////////////////////

template <Long NBitCapacity>
class BitWriteStream final {
public:
	static constexpr Long BIT_CAPACITY = NBitCapacity;

	using Buffer = StreamBuffer<BIT_CAPACITY>;

public:
	BitWriteStream(Buffer& _buffer)						  noexcept;

	template <Short NBitWidth>
	void write(const UnsignedBitWidth<NBitWidth> item)	  noexcept;

private:
	Buffer& _buffer;
};

//------------------------------------------------------------------------------

template <Long NBitCapacity>
class BitReadStream final {
public:
	static constexpr Long BIT_CAPACITY = NBitCapacity;

	using Buffer = StreamBuffer<BIT_CAPACITY>;

public:
	BitReadStream(const Buffer& buffer)					  noexcept
		: _buffer{buffer}
	{}

	template <Short NBitWidth>
	UnsignedBitWidth<NBitWidth> read()					  noexcept;

	Long cursor()									const noexcept	{ return _cursor;	}

private:
	const Buffer& _buffer;

	Long _cursor = 0;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "bit_stream.inl"
