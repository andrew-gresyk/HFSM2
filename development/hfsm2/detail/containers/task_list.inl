#if HFSM2_PLANS_AVAILABLE()

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TP, Long NC>
template <typename... TA>
HFSM2_CONSTEXPR(14)
Long
TaskListT<TP, NC>::emplace(TA&&... args) noexcept {
	HFSM2_ASSERT(_last  <= CAPACITY);

	if (_count < CAPACITY) {
		HFSM2_ASSERT(_vacantHead < CAPACITY);
		HFSM2_ASSERT(_vacantTail < CAPACITY);

		const Index index = _vacantHead;
		Item& item = _items[index];

		if (_vacantHead != _vacantTail) {
			// recycle
			HFSM2_ASSERT(item.prev == INVALID);
			HFSM2_ASSERT(item.next != INVALID);

			_vacantHead = item.next;

			Item& head = _items[_vacantHead];
			HFSM2_ASSERT(head.prev == index);
			head.prev = INVALID;
		} else if (_last < CAPACITY - 1) {
			// grow
			++_last;
			_vacantHead = _last;
			_vacantTail = _last;

			Item& vacant = _items[_vacantHead];
			vacant.prev = INVALID;
			vacant.next = INVALID;
		} else {
			// last
			HFSM2_ASSERT(_count == CAPACITY - 1);

			_last = CAPACITY;
			_vacantHead = INVALID;
			_vacantTail = INVALID;
		}

		new (&item) Item{forward<TA>(args)...};
		++_count;

		HFSM2_IF_ASSERT(verifyStructure());

		return index;
	} else {
		// full
		HFSM2_ASSERT(_vacantHead == INVALID);
		HFSM2_ASSERT(_vacantTail == INVALID);
		HFSM2_ASSERT(_count		 == CAPACITY);
		HFSM2_BREAK();

		return INVALID;
	}
}

//------------------------------------------------------------------------------

template <typename TP, Long NC>
HFSM2_CONSTEXPR(14)
void
TaskListT<TP, NC>::remove(const Index i) noexcept {
	HFSM2_ASSERT(i < CAPACITY && _count);

	Item& item = _items[i];

	if (_count < CAPACITY) {
		HFSM2_ASSERT(_vacantHead < CAPACITY);
		HFSM2_ASSERT(_vacantTail < CAPACITY);

		item.prev = INVALID;
		item.next = _vacantHead;

		Item& head = _items[_vacantHead];
		head.prev = i;

		_vacantHead = i;
	} else {
		// 0 -> 1
		HFSM2_ASSERT(_count		 == CAPACITY);
		HFSM2_ASSERT(_vacantHead == INVALID);
		HFSM2_ASSERT(_vacantTail == INVALID);

		item.prev = INVALID;
		item.next = INVALID;

		_vacantHead = i;
		_vacantTail = i;
	}

	--_count;

	HFSM2_IF_ASSERT(verifyStructure());
}

//------------------------------------------------------------------------------

template <typename TP, Long NC>
HFSM2_CONSTEXPR(14)
typename TaskListT<TP, NC>::Item&
TaskListT<TP, NC>::operator[] (const Index i) noexcept {
	HFSM2_IF_ASSERT(verifyStructure());

	return _items[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TP, Long NC>
HFSM2_CONSTEXPR(11)
const typename TaskListT<TP, NC>::Item&
TaskListT<TP, NC>::operator[] (const Index i) const noexcept {
	HFSM2_IF_ASSERT(verifyStructure());

	return _items[i];
}

//------------------------------------------------------------------------------

#if HFSM2_ASSERT_AVAILABLE()

template <typename TP, Long NC>
void
TaskListT<TP, NC>::verifyStructure(const Index occupied) const noexcept {
	if (_count < CAPACITY) {
		HFSM2_ASSERT(_vacantHead < CAPACITY);
		HFSM2_ASSERT(_vacantTail < CAPACITY);

		HFSM2_ASSERT(_items[_vacantHead].prev == INVALID);
		HFSM2_ASSERT(_items[_vacantTail].next == INVALID);

		Index emptyCount = 1;

		for (Index c = _vacantHead; c != _vacantTail; ) {
			HFSM2_ASSERT(occupied != c);

			const Item& current = _items[c];

			const Long f = current.next;
			if (f != INVALID) {
				// next
				const Item& following = _items[f];

				HFSM2_ASSERT(following.prev == c);

				c = f;
				continue;
			} else {
				// end
				HFSM2_ASSERT(_vacantTail == c);
				HFSM2_ASSERT(_count		 == CAPACITY - emptyCount);

				break;
			}
		}
	} else {
		HFSM2_ASSERT(_vacantHead == INVALID);
		HFSM2_ASSERT(_vacantTail == INVALID);
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
