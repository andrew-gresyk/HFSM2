namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

struct Units final {
	HFSM2_CONSTEXPR(11)	Units(Short unit_  = INVALID_SHORT,
							  Short width_ = INVALID_SHORT)				noexcept
		: unit {unit_ }
		, width{width_}
	{}

	Short unit;
	Short width;
};

//------------------------------------------------------------------------------

template <unsigned NCapacity>
class BitArrayT final {
public:
	using Index	= UCapacity<NCapacity>;

	static constexpr Index CAPACITY   = NCapacity;
	static constexpr Index UNIT_COUNT = contain(CAPACITY, 8);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class Bits {
		template <unsigned>
		friend class BitArrayT;

	private:
		HFSM2_CONSTEXPR(11)	explicit Bits(uint8_t* const storage,
										  const Index width)			noexcept
			: _storage{storage}
			, _width{width}
		{}

	public:
		HFSM2_CONSTEXPR(14)	explicit operator bool()			  const noexcept;

		HFSM2_CONSTEXPR(14)	void clear()								noexcept;

		template <Short NIndex>
		HFSM2_CONSTEXPR(14)	bool get()							  const noexcept;

		template <Short NIndex>
		HFSM2_CONSTEXPR(14)	void set()									noexcept;

		template <Short NIndex>
		HFSM2_CONSTEXPR(14)	void clear()								noexcept;

		HFSM2_CONSTEXPR(14)	bool get  (const Index index)		  const noexcept;
		HFSM2_CONSTEXPR(14)	void set  (const Index index)				noexcept;
		HFSM2_CONSTEXPR(14)	void clear(const Index index)				noexcept;

	private:
		uint8_t* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class CBits {
		template <unsigned>
		friend class BitArrayT;

	private:
		HFSM2_CONSTEXPR(11)	explicit CBits(const uint8_t* const storage,
										   const Index width)			noexcept
			: _storage{storage}
			, _width{width}
		{}

	public:
		HFSM2_CONSTEXPR(14)	explicit operator bool()			  const noexcept;

		template <Short NIndex>
		HFSM2_CONSTEXPR(14)	bool get()							  const noexcept;

		HFSM2_CONSTEXPR(14)	bool get(const Index index)			  const noexcept;

	private:
		const uint8_t* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	HFSM2_CONSTEXPR(14)	BitArrayT()										noexcept	{ clear();	}

	HFSM2_CONSTEXPR(14)	void clear()									noexcept;

	template <Short NIndex>
	HFSM2_CONSTEXPR(14)	bool get()								  const noexcept;

	template <Short NIndex>
	HFSM2_CONSTEXPR(14)	void set()										noexcept;

	template <Short NIndex>
	HFSM2_CONSTEXPR(14)	void clear()									noexcept;

	template <typename TIndex>
	HFSM2_CONSTEXPR(14)	bool get  (const TIndex index)			  const noexcept;

	template <typename TIndex>
	HFSM2_CONSTEXPR(14)	void set  (const TIndex index)					noexcept;

	template <typename TIndex>
	HFSM2_CONSTEXPR(14)	void clear(const TIndex index)					noexcept;

	template <Short NUnit, Short NWidth>
	HFSM2_CONSTEXPR(14)	 Bits  bits()									noexcept;

	template <Short NUnit, Short NWidth>
	HFSM2_CONSTEXPR(14)	CBits cbits()							  const noexcept;

	HFSM2_CONSTEXPR(14)	 Bits  bits(const Units& units)					noexcept;
	HFSM2_CONSTEXPR(14)	CBits cbits(const Units& units)			  const noexcept;

private:
	uint8_t _storage[UNIT_COUNT];
};

//------------------------------------------------------------------------------

template <>
class BitArrayT<0> final {
public:
	HFSM2_CONSTEXPR(14)	void clear()									noexcept	{}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "bit_array.inl"
