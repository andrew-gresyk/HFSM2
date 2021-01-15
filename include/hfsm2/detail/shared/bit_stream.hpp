#ifdef HFSM2_ENABLE_SERIALIZATION

namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

template <Long NBitCapacity>
struct StreamBufferT {
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
class BitWriteStreamT final {
public:
	static constexpr Long BIT_CAPACITY = NBitCapacity;

	using Buffer = StreamBufferT<BIT_CAPACITY>;

public:
	BitWriteStreamT(Buffer& _buffer)					  noexcept;

	template <Short NBitWidth>
	void write(const UnsignedBitWidth<NBitWidth> item)	  noexcept;

private:
	Buffer& _buffer;
};

//------------------------------------------------------------------------------

template <Long NBitCapacity>
class BitReadStreamT final {
public:
	static constexpr Long BIT_CAPACITY = NBitCapacity;

	using Buffer = StreamBufferT<BIT_CAPACITY>;

public:
	BitReadStreamT(const Buffer& buffer)				  noexcept
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

#endif
