namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <unsigned NCapacity>
class BitFlatSetT final {
public:
	using Index		= UCapacity<NCapacity>;

	static constexpr Index CAPACITY   = NCapacity;
	static constexpr Index UNIT_COUNT = ceilingDivide(CAPACITY, 8);

	using This		= BitFlatSetT<CAPACITY>;

public:
	HFSM2_CONSTEXPR(14)	BitFlatSetT()									noexcept	{ clear();			}

	HFSM2_CONSTEXPR(14)	void set()										noexcept;
	HFSM2_CONSTEXPR(14)	void clear()									noexcept;
	HFSM2_CONSTEXPR(14)	bool empty()							  const noexcept;

	template <Short NIndex>
	HFSM2_CONSTEXPR(14)	bool get()								  const noexcept;

	template <typename TIndex>
	HFSM2_CONSTEXPR(14)	bool get  (const TIndex index)			  const noexcept;

	template <typename TIndex>
	HFSM2_CONSTEXPR(14)	void set  (const TIndex index)					noexcept;

	template <typename TIndex>
	HFSM2_CONSTEXPR(14)	void clear(const TIndex index)					noexcept;

	HFSM2_CONSTEXPR(14)	bool operator &  (const This& other)	  const noexcept;
	HFSM2_CONSTEXPR(14)	void operator &= (const This& other)			noexcept;

private:
	uint8_t _storage[UNIT_COUNT] {};
};

//------------------------------------------------------------------------------

template <>
class BitFlatSetT<0> final {
public:
	HFSM2_CONSTEXPR(14)	void set()										noexcept	{}
	HFSM2_CONSTEXPR(14)	void clear()									noexcept	{}
	HFSM2_CONSTEXPR(11)	bool empty()							  const noexcept	{ return true;		}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "bit_flat_set.inl"
