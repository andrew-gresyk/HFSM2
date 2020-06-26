namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

struct Units {
	ShortIndex unit;
	ShortIndex width;
};

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
class BitArray final {
public:
	using Index	= TIndex;
	using Unit	= unsigned char;

	static constexpr Index CAPACITY	= NCapacity;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class Bits {
		template <typename, ShortIndex>
		friend class BitArray;

	private:
		HFSM2_INLINE explicit Bits(Unit* const storage,
								   const Index width)
			: _storage{storage}
			, _width{width}
		{}

	public:
		HFSM2_INLINE explicit operator bool() const;

		HFSM2_INLINE void clear();

		template <ShortIndex NIndex>
		HFSM2_INLINE bool get() const;

		template <ShortIndex NIndex>
		HFSM2_INLINE void set();

		template <ShortIndex NIndex>
		HFSM2_INLINE void reset();

		HFSM2_INLINE bool get  (const Index index) const;
		HFSM2_INLINE void set  (const Index index);
		HFSM2_INLINE void reset(const Index index);

	private:
		Unit* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class ConstBits {
		template <typename, ShortIndex>
		friend class BitArray;

	private:
		HFSM2_INLINE explicit ConstBits(const Unit* const storage,
										const Index width)
			: _storage{storage}
			, _width{width}
		{}

	public:
		HFSM2_INLINE explicit operator bool() const;

		template <ShortIndex NIndex>
		HFSM2_INLINE bool get() const;

		HFSM2_INLINE bool get(const Index index) const;

	private:
		const Unit* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	BitArray() {
		clear();
	}

	HFSM2_INLINE void clear();

	template <ShortIndex NIndex>
	HFSM2_INLINE bool get() const;

	template <ShortIndex NIndex>
	HFSM2_INLINE void set();

	template <ShortIndex NIndex>
	HFSM2_INLINE void reset();

	HFSM2_INLINE bool get  (const Index index) const;
	HFSM2_INLINE void set  (const Index index);
	//HFSM2_INLINE void reset(const Index index);

	template <ShortIndex NUnit, ShortIndex NWidth>
	HFSM2_INLINE	  Bits bits();

	template <ShortIndex NUnit, ShortIndex NWidth>
	HFSM2_INLINE ConstBits bits() const;


	HFSM2_INLINE	  Bits bits(const Units& units);
	HFSM2_INLINE ConstBits bits(const Units& units) const;

private:
	Unit _storage[CAPACITY];
};

//------------------------------------------------------------------------------

template <typename TIndex>
class BitArray<TIndex, 0> final {
public:
	HFSM2_INLINE void clear() {}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "bit_array.inl"
