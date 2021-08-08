namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

struct Units {
	HFSM2_CONSTEXPR(11) Units(Short unit_  = INVALID_SHORT,
							  Short width_ = INVALID_SHORT)			  noexcept
		: unit {unit_ }
		, width{width_}
	{}

	Short unit;
	Short width;
};

//------------------------------------------------------------------------------

template <typename TIndex, Short NUnitCount>
class BitArrayT final {
public:
	using Index	= TIndex;

	static constexpr Index UNIT_COUNT = NUnitCount;
	static constexpr Index BIT_COUNT  = UNIT_COUNT * 8;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class Bits {
		template <typename, Short>
		friend class BitArrayT;

	private:
		HFSM2_CONSTEXPR(11)	explicit Bits(uint8_t* const storage,
										  const Index width)		  noexcept
			: _storage{storage}
			, _width{width}
		{}

	public:
		HFSM2_CONSTEXPR(14)	explicit operator bool()			const noexcept;

		HFSM2_CONSTEXPR(14)	void clear()							  noexcept;

		template <Short NIndex>
		HFSM2_CONSTEXPR(14)	bool get()							const noexcept;

		template <Short NIndex>
		HFSM2_CONSTEXPR(14)	void set()								  noexcept;

		template <Short NIndex>
		HFSM2_CONSTEXPR(14)	void clear()							  noexcept;

		HFSM2_CONSTEXPR(14)	bool get  (const Index index)		const noexcept;
		HFSM2_CONSTEXPR(14)	void set  (const Index index)			  noexcept;
		HFSM2_CONSTEXPR(14)	void clear(const Index index)			  noexcept;

	private:
		uint8_t* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class CBits {
		template <typename, Short>
		friend class BitArrayT;

	private:
		HFSM2_CONSTEXPR(11)	explicit CBits(const uint8_t* const storage,
									const Index width)				  noexcept
			: _storage{storage}
			, _width{width}
		{}

	public:
		HFSM2_CONSTEXPR(14)	explicit operator bool()			const noexcept;

		template <Short NIndex>
		HFSM2_CONSTEXPR(14)	bool get()							const noexcept;

		HFSM2_CONSTEXPR(14)	bool get(const Index index)			const noexcept;

	private:
		const uint8_t* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	HFSM2_CONSTEXPR(14)	BitArrayT()									  noexcept	{ clear();	}

	HFSM2_CONSTEXPR(14)	void clear()								  noexcept;

	template <Short NIndex>
	HFSM2_CONSTEXPR(14) bool get()								const noexcept;

	template <Short NIndex>
	HFSM2_CONSTEXPR(14) void set()									  noexcept;

	template <Short NIndex>
	HFSM2_CONSTEXPR(14) void clear()								  noexcept;

	HFSM2_CONSTEXPR(14) bool get  (const Index index)			const noexcept;
	HFSM2_CONSTEXPR(14) void set  (const Index index)				  noexcept;
	HFSM2_CONSTEXPR(14) void clear(const Index index)				  noexcept;

	template <Short NUnit, Short NWidth>
	HFSM2_CONSTEXPR(14)  Bits  bits()								  noexcept;

	template <Short NUnit, Short NWidth>
	HFSM2_CONSTEXPR(14) CBits cbits()							const noexcept;

	HFSM2_CONSTEXPR(14)  Bits  bits(const Units& units)				  noexcept;
	HFSM2_CONSTEXPR(11) CBits cbits(const Units& units)			const noexcept;

private:
	uint8_t _storage[UNIT_COUNT];
};

//------------------------------------------------------------------------------

template <typename TIndex>
class BitArrayT<TIndex, 0> final {
public:
	HFSM2_CONSTEXPR(11)	void clear()								  noexcept	{}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "bit_array.inl"
