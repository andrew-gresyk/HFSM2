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
		HFSM_INLINE explicit Bits(Unit* const storage,
								  const Index width)
			: _storage{storage}
			, _width{width}
		{}

	public:
		HFSM_INLINE explicit operator bool() const;

		HFSM_INLINE void clear();

		template <ShortIndex NIndex>
		HFSM_INLINE bool get() const;

		template <ShortIndex NIndex>
		HFSM_INLINE void set();

		template <ShortIndex NIndex>
		HFSM_INLINE void reset();

		HFSM_INLINE bool get  (const Index index) const;
		HFSM_INLINE void set  (const Index index);
		HFSM_INLINE void reset(const Index index);

	private:
		Unit* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class ConstBits {
		template <typename, ShortIndex>
		friend class BitArray;

	private:
		HFSM_INLINE explicit ConstBits(const Unit* const storage,
									   const Index width)
			: _storage{storage}
			, _width{width}
		{}

	public:
		HFSM_INLINE explicit operator bool() const;

		template <ShortIndex NIndex>
		HFSM_INLINE bool get() const;

		HFSM_INLINE bool get(const Index index) const;

	private:
		const Unit* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	BitArray() {
		clear();
	}

	HFSM_INLINE void clear();

	template <ShortIndex NIndex>
	HFSM_INLINE bool get() const;

	template <ShortIndex NIndex>
	HFSM_INLINE void set();

	template <ShortIndex NIndex>
	HFSM_INLINE void reset();

	HFSM_INLINE bool get  (const Index index) const;
	HFSM_INLINE void set  (const Index index);
	//HFSM_INLINE void reset(const Index index);

	template <ShortIndex NUnit, ShortIndex NWidth>
	HFSM_INLINE		 Bits bits();

	template <ShortIndex NUnit, ShortIndex NWidth>
	HFSM_INLINE ConstBits bits() const;


	HFSM_INLINE		 Bits bits(const Units& units);
	HFSM_INLINE ConstBits bits(const Units& units) const;

private:
	Unit _storage[CAPACITY];
};

//------------------------------------------------------------------------------

template <typename TIndex>
class BitArray<TIndex, 0> final {
public:
	HFSM_INLINE void clear() {}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "bit_array.inl"
