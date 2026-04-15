namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

struct BitSlice final {
	HFSM2_CONSTEXPR(11)	BitSlice(Short unit_  = INVALID_SHORT,
								 Short width_ = INVALID_SHORT)			noexcept
		: byteOffset{unit_ }
		, bitWidth  {width_}
	{}

	Short byteOffset;
	Short bitWidth;
};

//------------------------------------------------------------------------------

template <unsigned NCapacity>
class BitSliceSetT final {
public:
	using Index			= UCapacity<NCapacity>;

	static constexpr Index CAPACITY   = NCapacity;
	static constexpr Index UNIT_COUNT = ceilingDivide(CAPACITY, 8);

	using This	= BitSliceSetT<CAPACITY>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class Bits {
		template <unsigned>
		friend class BitSliceSetT;

	private:
		HFSM2_CONSTEXPR(11)	explicit Bits(uint8_t* const storage,
										  const Index width)			noexcept
			: _storage{storage}
			, _width  {width  }
		{}

	public:
		HFSM2_CONSTEXPR(14)	explicit operator bool()			  const noexcept;
		HFSM2_CONSTEXPR(14)	void clear()								noexcept;

		HFSM2_CONSTEXPR(14)	void set  (const Index index)				noexcept;
		HFSM2_CONSTEXPR(14)	void clear(const Index index)				noexcept;

	private:
		uint8_t* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class CBits {
		template <unsigned>
		friend class BitSliceSetT;

	private:
		HFSM2_CONSTEXPR(11)	explicit CBits(const uint8_t* const storage,
										   const Index width)			noexcept
			: _storage{storage}
			, _width  {width  }
		{}

	public:
		HFSM2_CONSTEXPR(14)	explicit operator bool()			  const noexcept;

		HFSM2_CONSTEXPR(14)	bool get(const Index index)			  const noexcept;

	private:
		const uint8_t* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	HFSM2_CONSTEXPR(14)	BitSliceSetT()									noexcept	{ clear();			}

	HFSM2_CONSTEXPR(14)	void clear()									noexcept;
	HFSM2_CONSTEXPR(14)	bool empty()							  const noexcept;

	template <Short NUnit, Short NWidth>
	HFSM2_CONSTEXPR(14)	 Bits  bits()									noexcept;

	template <Short NUnit, Short NWidth>
	HFSM2_CONSTEXPR(14)	CBits cbits()							  const noexcept;

	HFSM2_CONSTEXPR(14)	 Bits  bits(const BitSlice& units)				noexcept;
	HFSM2_CONSTEXPR(14)	CBits cbits(const BitSlice& units)		  const noexcept;

	HFSM2_CONSTEXPR(14)	bool operator != (const This& other)	  const noexcept;

private:
	uint8_t _storage[UNIT_COUNT] {};
};

//------------------------------------------------------------------------------

template <>
class BitSliceSetT<0> final {
public:
	HFSM2_CONSTEXPR(14)	void clear()									noexcept	{}
	HFSM2_CONSTEXPR(11)	bool empty()							  const noexcept	{ return true;		}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "bit_slice_set.inl"
