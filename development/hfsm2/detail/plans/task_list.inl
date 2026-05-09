#if HFSM2_PLANS_AVAILABLE()

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, Long NTC_, Long NRC_>
HFSM2_CONSTEXPR(14)
typename TaskListT<T, NTC_, NRC_>::This&
TaskListT<T, NTC_, NRC_>::operator = (const This& other) HFSM2_NOEXCEPT_17(noexcept(Item{other.item(0)})) {
	if (this == &other)
		return *this;

	clear();
	copyFrom(other);

	return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NTC_, Long NRC_>
HFSM2_CONSTEXPR(14)
typename TaskListT<T, NTC_, NRC_>::This&
TaskListT<T, NTC_, NRC_>::operator = (This&& other) HFSM2_NOEXCEPT_17(noexcept(Item{::hfsm2::move(other.item(0))})) {
	if (this == &other)
		return *this;

	clear();
	moveFrom(::hfsm2::move(other));

	return *this;
}

//------------------------------------------------------------------------------

template <typename T, Long NTC_, Long NRC_>
HFSM2_CONSTEXPR(14)
void
TaskListT<T, NTC_, NRC_>::clear() noexcept {
	for (Index i = 0; i < _last; ++i)
		if (_occupied.get(i))
			::hfsm2::destroy(item(i));

	reset();
}

//------------------------------------------------------------------------------

template <typename T, Long NTC_, Long NRC_>
template <typename... TA_>
HFSM2_CONSTEXPR(14)
typename TaskListT<T, NTC_, NRC_>::Index
TaskListT<T, NTC_, NRC_>::emplace(const RegionID regionId,
								  TA_&&... args) HFSM2_NOEXCEPT_17(noexcept(Item{::hfsm2::forward<TA_>(args)...}))
{
	HFSM2_ASSERT(regionId < REGION_COUNT);

	if (_count >= CAPACITY)
		return INVALID;

	const Index index = takeVacant();
	Slot& current = slot(index);

	HFSM2_ASSERT(current.prev == INVALID);
	HFSM2_ASSERT(current.next == INVALID);

	new (current.storage) Item{::hfsm2::forward<TA_>(args)...};

	_occupied.set(index);
	++_count;

	attach(regionId, index);

	HFSM2_IF_ASSERT(verifyStructure());

	return index;
}

//------------------------------------------------------------------------------

template <typename T, Long NTC_, Long NRC_>
HFSM2_CONSTEXPR(14)
void
TaskListT<T, NTC_, NRC_>::remove(const RegionID regionId,
								 const Index index) noexcept
{
	HFSM2_ASSERT(regionId < REGION_COUNT);
	HFSM2_ASSERT(occupied(index));

	detach(regionId, index);
	::hfsm2::destroy(item(index));

	_occupied.clear(index);
	--_count;

	releaseVacant(index);

	HFSM2_IF_ASSERT(verifyStructure());
}

//------------------------------------------------------------------------------

template <typename T, Long NTC_, Long NRC_>
HFSM2_CONSTEXPR(14)
void
TaskListT<T, NTC_, NRC_>::clearRegion(const RegionID regionId) noexcept {
	HFSM2_ASSERT(regionId < REGION_COUNT);

	for (Index index = bounds(regionId).first;
		 index != INVALID;
		 )
	{
		const Index nextIndex = next(index);
		remove(regionId, index);
		index = nextIndex;
	}
}

//------------------------------------------------------------------------------

template <typename T, Long NTC_, Long NRC_>
HFSM2_CONSTEXPR(14)
typename TaskListT<T, NTC_, NRC_>::Item&
TaskListT<T, NTC_, NRC_>::operator[] (const Index index) noexcept {
	HFSM2_ASSERT(occupied(index));

	return item(index);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NTC_, Long NRC_>
HFSM2_CONSTEXPR(14)
const typename TaskListT<T, NTC_, NRC_>::Item&
TaskListT<T, NTC_, NRC_>::operator[] (const Index index) const noexcept {
	HFSM2_ASSERT(occupied(index));

	return item(index);
}

//------------------------------------------------------------------------------

template <typename T, Long NTC_, Long NRC_>
HFSM2_CONSTEXPR(14)
void
TaskListT<T, NTC_, NRC_>::reset() noexcept {
	_count = 0;
	_vacantHead = INVALID;
	_last = 0;

	_occupied.clear();
	_bounds.clear();
}

//------------------------------------------------------------------------------

template <typename T, Long NTC_, Long NRC_>
HFSM2_CONSTEXPR(14)
void
TaskListT<T, NTC_, NRC_>::copyFrom(const This& other) HFSM2_NOEXCEPT_17(noexcept(Item{other.item(0)})) {
	_count		= other._count;
	_vacantHead	= other._vacantHead;
	_last		= other._last;
	_occupied	= other._occupied;
	_bounds		= other._bounds;

	for (Index i = 0; i < _last; ++i) {
		Slot& current = slot(i);
		const Slot& source = other.slot(i);

		current.prev = source.prev;
		current.next = source.next;

		if (_occupied.get(i))
			new (current.storage) Item{other.item(i)};
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, Long NTC_, Long NRC_>
HFSM2_CONSTEXPR(14)
void
TaskListT<T, NTC_, NRC_>::moveFrom(This&& other) HFSM2_NOEXCEPT_17(noexcept(Item{::hfsm2::move(other.item(0))})) {
	_count		= other._count;
	_vacantHead	= other._vacantHead;
	_last		= other._last;
	_occupied	= other._occupied;
	_bounds		= other._bounds;

	for (Index i = 0; i < _last; ++i) {
		Slot& current = slot(i);
		Slot& source = other.slot(i);

		current.prev = source.prev;
		current.next = source.next;

		if (_occupied.get(i))
			new (current.storage) Item{::hfsm2::move(other.item(i))};
	}

	other.clear();
}

//------------------------------------------------------------------------------

template <typename T, Long NTC_, Long NRC_>
HFSM2_CONSTEXPR(14)
typename TaskListT<T, NTC_, NRC_>::Index
TaskListT<T, NTC_, NRC_>::takeVacant() noexcept {
	if (_vacantHead < CAPACITY) {
		const Index index = _vacantHead;
		_vacantHead = slot(index).next;
		slot(index).next = INVALID;

		return index;
	}

	HFSM2_ASSERT(_last < CAPACITY);

	return _last++;
}

//------------------------------------------------------------------------------

template <typename T, Long NTC_, Long NRC_>
HFSM2_CONSTEXPR(14)
void
TaskListT<T, NTC_, NRC_>::releaseVacant(const Index index) noexcept {
	Slot& current = slot(index);
	HFSM2_ASSERT(current.prev == INVALID);
	current.next = _vacantHead;

	_vacantHead = index;
}

//------------------------------------------------------------------------------

template <typename T, Long NTC_, Long NRC_>
HFSM2_CONSTEXPR(14)
void
TaskListT<T, NTC_, NRC_>::attach(const RegionID regionId,
								 const Index index) noexcept
{
	Bounds& regionBounds = bounds(regionId);

	if (!regionBounds) {
		regionBounds.first = index;
		regionBounds.last  = index;
	}
	else {
		HFSM2_ASSERT(occupied(regionBounds.last));

		Slot& lastSlot = slot(regionBounds.last);
		HFSM2_ASSERT(lastSlot.next == INVALID);

		lastSlot.next = index;

		Slot& current = slot(index);
		current.prev = regionBounds.last;

		regionBounds.last = index;
	}
}

//------------------------------------------------------------------------------

template <typename T, Long NTC_, Long NRC_>
HFSM2_CONSTEXPR(14)
void
TaskListT<T, NTC_, NRC_>::detach(const RegionID regionId,
								 const Index index) noexcept
{
	Bounds& regionBounds = bounds(regionId);
	Slot& current = slot(index);

	if (current.prev < CAPACITY)
		slot(current.prev).next = current.next;
	else {
		HFSM2_ASSERT(regionBounds.first == index);
		regionBounds.first = current.next;
	}

	if (current.next < CAPACITY)
		slot(current.next).prev = current.prev;
	else {
		HFSM2_ASSERT(regionBounds.last == index);
		regionBounds.last = current.prev;
	}

	current.prev = INVALID;
	current.next = INVALID;
}

//------------------------------------------------------------------------------

#if HFSM2_ASSERT_AVAILABLE()

template <typename T, Long NTC_, Long NRC_>
void
TaskListT<T, NTC_, NRC_>::verifyStructure() const noexcept {
	HFSM2_ASSERT(_count <= _last);
	HFSM2_ASSERT(_last  <= CAPACITY);

	Index occupiedCount = 0;
	for (Index i = 0; i < _last; ++i)
		if (_occupied.get(i))
			++occupiedCount;

	HFSM2_ASSERT(occupiedCount == _count);

	BitFlatSetT<CAPACITY> visited;
	visited.clear();

	for (RegionID regionId = 0; regionId < REGION_COUNT; ++regionId) {
		const Bounds& regionBounds = bounds(regionId);

		if (!regionBounds) {
			HFSM2_ASSERT(regionBounds.last == INVALID);
			continue;
		}

		HFSM2_ASSERT(regionBounds.first < CAPACITY);
		HFSM2_ASSERT(regionBounds.last  < CAPACITY);

		for (Index slow = regionBounds.first, fast = slow, prev = INVALID; ; ) {
			HFSM2_ASSERT(slow < CAPACITY);
			HFSM2_ASSERT(_occupied.get(slow));
			HFSM2_ASSERT(!visited.get(slow));

			visited.set(slow);

			const Slot& current = slot(slow);
			HFSM2_ASSERT(current.prev == prev);

			if (slow != regionBounds.last) {
				HFSM2_ASSERT(current.next < CAPACITY);

				prev = slow;
				slow = current.next;

				if (fast != INVALID) {
					fast = slot(fast).next;

					if (fast != INVALID)
						fast = slot(fast).next;

					HFSM2_ASSERT(fast == INVALID || slow != fast);
				}
			}
			else {
				HFSM2_ASSERT(current.next == INVALID);
				break;
			}
		}
	}

	for (Index i = 0; i < _last; ++i)
		HFSM2_ASSERT(visited.get(i) == _occupied.get(i));

	BitFlatSetT<CAPACITY> free;
	free.clear();

	for (Index index = _vacantHead;
		 index != INVALID;
		 index = slot(index).next)
	{
		HFSM2_ASSERT(index < CAPACITY);
		HFSM2_ASSERT(!_occupied.get(index));
		HFSM2_ASSERT(!free.get(index));

		free.set(index);
	}

	for (Index i = 0; i < _last; ++i)
		HFSM2_ASSERT(free.get(i) || _occupied.get(i));
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
