#if HFSM2_PLANS_AVAILABLE()

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TItem, Long NTaskCapacity, Long NRegionCount>
class TaskListT {
public:
	using Item		= TItem;
	using Index		= UCapacity<NTaskCapacity>;

	static constexpr Index CAPACITY		= NTaskCapacity;
	static constexpr Long  REGION_COUNT	= NRegionCount;
	static constexpr Index INVALID		= static_cast<Index>(-1);

	struct Bounds final {
		Index first	= INVALID;
		Index last	= INVALID;

		HFSM2_CONSTEXPR(11)	explicit operator bool()				  const noexcept	{ return first != INVALID;	}
		HFSM2_CONSTEXPR(14)	void clear()									noexcept	{ first = INVALID;
																						  last  = INVALID;	}
	};

private:
	using Storage = uint8_t[sizeof(Item)];
	using This	  = TaskListT<Item, CAPACITY, REGION_COUNT>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Slot {
		Index prev = INVALID;
		Index next = INVALID;

#ifdef _MSC_VER
		#pragma warning(push)
		#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

		alignas(Item) Storage storage;

#ifdef _MSC_VER
		#pragma warning(pop)
#endif
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	HFSM2_CONSTEXPR(14)	 TaskListT()										noexcept	{ reset();	}
	HFSM2_CONSTEXPR(14)	 TaskListT(const This& other)				  HFSM2_NOEXCEPT_17(noexcept(Item{              other.item(0) }))	{ copyFrom(              other );		}
	HFSM2_CONSTEXPR(14)	 TaskListT(This&& other)					  HFSM2_NOEXCEPT_17(noexcept(Item{::hfsm2::move(other.item(0))}))	{ moveFrom(::hfsm2::move(other));		}
	HFSM2_CONSTEXPR(20)	~TaskListT()										noexcept	{ clear();	}

	HFSM2_CONSTEXPR(14)	This& operator = (const This & other)		  HFSM2_NOEXCEPT_17(noexcept(Item{              other.item(0) }));
	HFSM2_CONSTEXPR(14)	This& operator = (      This&& other)		  HFSM2_NOEXCEPT_17(noexcept(Item{::hfsm2::move(other.item(0))}));

	HFSM2_CONSTEXPR(14)	void clear()										noexcept;

	template <typename... TArgs>
	HFSM2_CONSTEXPR(14)	Index emplace   (const RegionID regionId,
										 TArgs&&... args)			  HFSM2_NOEXCEPT_17(noexcept(Item{::hfsm2::forward<TArgs>(args)...}));

	HFSM2_CONSTEXPR(14)	void remove     (const RegionID regionId,
										 const Index index)					noexcept;

	HFSM2_CONSTEXPR(14)	void clearRegion(const RegionID regionId)			noexcept;

	HFSM2_CONSTEXPR(14)		  Item& operator[] (const Index index)			noexcept;
	HFSM2_CONSTEXPR(14)	const Item& operator[] (const Index index)	  const noexcept;

	HFSM2_CONSTEXPR(14)		  Bounds& bounds(const RegionID regionId)		noexcept	{ HFSM2_ASSERT(regionId < REGION_COUNT);	return _bounds[regionId];	}
	HFSM2_CONSTEXPR(14)	const Bounds& bounds(const RegionID regionId) const noexcept	{ HFSM2_ASSERT(regionId < REGION_COUNT);	return _bounds[regionId];	}

	HFSM2_CONSTEXPR(14)	Index prev(const Index index)				  const noexcept	{ HFSM2_ASSERT(occupied(index));			return slot(index).prev;	}
	HFSM2_CONSTEXPR(14)	Index next(const Index index)				  const noexcept	{ HFSM2_ASSERT(occupied(index));			return slot(index).next;	}

	HFSM2_CONSTEXPR(11)	Index count()								  const noexcept	{ return _count;														}
	HFSM2_CONSTEXPR(11)	bool  empty()								  const noexcept	{ return _count == 0;													}
	HFSM2_CONSTEXPR(11)	bool  occupied(const Index index)			  const noexcept	{ return index < _last && _occupied.get(index);							}

	HFSM2_CONSTEXPR(11) static Index invalid()								noexcept	{ return INVALID;														}

private:
	HFSM2_CONSTEXPR(14)		  Item& item(const Index index)					noexcept	{ return *::hfsm2::reinterpret_launder<Item>(slot(index).storage);		}
	HFSM2_CONSTEXPR(11)	const Item& item(const Index index)			  const noexcept	{ return *::hfsm2::reinterpret_launder<Item>(slot(index).storage);		}

	HFSM2_CONSTEXPR(14)		  Slot& slot(const Index index)					noexcept	{ return _slots[index];													}
	HFSM2_CONSTEXPR(11)	const Slot& slot(const Index index)			  const noexcept	{ return _slots[index];													}

	HFSM2_CONSTEXPR(14)	void reset()										noexcept;
	HFSM2_CONSTEXPR(14)	void copyFrom(const This& other)			  HFSM2_NOEXCEPT_17(noexcept(Item{              other.item(0) }));
	HFSM2_CONSTEXPR(14)	void moveFrom(This&& other)					  HFSM2_NOEXCEPT_17(noexcept(Item{::hfsm2::move(other.item(0))}));

	HFSM2_CONSTEXPR(14)	Index takeVacant()									noexcept;
	HFSM2_CONSTEXPR(14)	void releaseVacant(const Index index)				noexcept;

	HFSM2_CONSTEXPR(14)	void attach(const RegionID regionId,
									const Index index)						noexcept;

	HFSM2_CONSTEXPR(14)	void detach(const RegionID regionId,
									const Index index)						noexcept;

	HFSM2_IF_ASSERT(void verifyStructure() const noexcept);

private:
	Index _count		= 0;
	Index _vacantHead	= INVALID;
	Index _last			= 0;

	BitFlatSetT<CAPACITY> _occupied;
	StaticArrayT<Bounds, REGION_COUNT> _bounds;
	Slot _slots[CAPACITY];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//#if __cplusplus == 201402L
//
//template <typename T, Long NTC_, Long NRC_>
//constexpr typename TaskListT<T, NTC_, NRC_>::Index
//TaskListT<T, NTC_, NRC_>::INVALID;
//
//#endif

//------------------------------------------------------------------------------

template <typename TItem, Long NRegionCount>
class TaskListT<TItem, 0, NRegionCount> final {
};

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#include "task_list.inl"
