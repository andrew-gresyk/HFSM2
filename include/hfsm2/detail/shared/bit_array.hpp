namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

struct Units {
	inline Units(Short unit_  = INVALID_SHORT,
				 Short width_ = INVALID_SHORT) noexcept
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
	using Unit	= unsigned char;

	static constexpr Index UNIT_COUNT = NUnitCount;
	static constexpr Index UNIT_WIDTH = sizeof(Unit) * 8;
	static constexpr Index BIT_COUNT  = UNIT_COUNT * UNIT_WIDTH;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class Bits {
		template <typename, Short>
		friend class BitArrayT;

	private:
		HFSM2_INLINE explicit Bits(Unit* const storage,
								   const Index width)		  noexcept
			: _storage{storage}
			, _width{width}
		{}

	public:
		HFSM2_INLINE explicit operator bool()			const noexcept;

		HFSM2_INLINE void clear()							  noexcept;

		template <Short NIndex>
		HFSM2_INLINE bool get()							const noexcept;

		template <Short NIndex>
		HFSM2_INLINE void set()								  noexcept;

		template <Short NIndex>
		HFSM2_INLINE void clear()							  noexcept;

		HFSM2_INLINE bool get  (const Index index)		const noexcept;
		HFSM2_INLINE void set  (const Index index)			  noexcept;
		HFSM2_INLINE void clear(const Index index)			  noexcept;

	private:
		Unit* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class CBits {
		template <typename, Short>
		friend class BitArrayT;

	private:
		HFSM2_INLINE explicit CBits(const Unit* const storage,
									const Index width)		  noexcept
			: _storage{storage}
			, _width{width}
		{}

	public:
		HFSM2_INLINE explicit operator bool()			const noexcept;

		template <Short NIndex>
		HFSM2_INLINE bool get()							const noexcept;

		HFSM2_INLINE bool get(const Index index)		const noexcept;

	private:
		const Unit* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	BitArrayT() noexcept {
		clear();
	}

	HFSM2_INLINE void clear()								  noexcept;

	template <Short NIndex>
	HFSM2_INLINE bool get()								const noexcept;

	template <Short NIndex>
	HFSM2_INLINE void set()									  noexcept;

	template <Short NIndex>
	HFSM2_INLINE void clear()								  noexcept;

	HFSM2_INLINE bool get  (const Index index)			const noexcept;
	HFSM2_INLINE void set  (const Index index)				  noexcept;
	HFSM2_INLINE void clear(const Index index)				  noexcept;

	template <Short NUnit, Short NWidth>
	HFSM2_INLINE  Bits bits()								  noexcept;

	template <Short NUnit, Short NWidth>
	HFSM2_INLINE CBits bits()							const noexcept;

	HFSM2_INLINE  Bits bits(const Units& units)				  noexcept;
	HFSM2_INLINE CBits bits(const Units& units)			const noexcept;

private:
	Unit _storage[UNIT_COUNT];
};

//------------------------------------------------------------------------------

template <typename TIndex>
class BitArrayT<TIndex, 0> final {
public:
	HFSM2_INLINE void clear() noexcept {}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "bit_array.inl"
