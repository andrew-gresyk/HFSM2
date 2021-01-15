#ifdef HFSM2_ENABLE_PLANS

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TP, Long NC>
template <typename... TA>
Long
TaskListT<TP, NC>::emplace(TA&&... args) noexcept {
	if (_count < CAPACITY) {
		HFSM2_ASSERT(_vacantHead < CAPACITY);
		HFSM2_ASSERT(_vacantTail < CAPACITY);

		const Index index = _vacantHead;
		auto& cell = _items[index];
		++_count;

		if (_vacantHead != _vacantTail) {
			// recycle
			HFSM2_ASSERT(cell.prev == INVALID);
			HFSM2_ASSERT(cell.next != INVALID);

			_vacantHead = cell.next;

			auto& head = _items[_vacantHead];
			HFSM2_ASSERT(head.prev == index);
			head.prev = INVALID;
		} else if (_last < CAPACITY - 1) {
			// grow
			++_last;
			_vacantHead = _last;
			_vacantTail = _last;

			auto& vacant = _items[_vacantHead];
			vacant.prev = INVALID;
			vacant.next = INVALID;
		} else {
			HFSM2_ASSERT(_count == CAPACITY);

			_vacantHead = INVALID;
			_vacantTail = INVALID;
		}

		HFSM2_IF_ASSERT(verifyStructure());

		new (&cell) Item{std::forward<TA>(args)...};

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
void
TaskListT<TP, NC>::remove(const Index i) noexcept {
	HFSM2_ASSERT(i < CAPACITY && _count);

	auto& fresh = _items[i];

	if (_count < CAPACITY) {
		HFSM2_ASSERT(_vacantHead < CAPACITY);
		HFSM2_ASSERT(_vacantTail < CAPACITY);

		fresh.prev = INVALID;
		fresh.next = _vacantHead;

		auto& head = _items[_vacantHead];
		head.prev = i;

		_vacantHead = i;
	} else {
		// 0 -> 1
		HFSM2_ASSERT(_count		 == CAPACITY);
		HFSM2_ASSERT(_vacantHead == INVALID);
		HFSM2_ASSERT(_vacantTail == INVALID);

		fresh.prev = INVALID;
		fresh.next = INVALID;

		_vacantHead = i;
		_vacantTail = i;
	}

	--_count;

	HFSM2_IF_ASSERT(verifyStructure());
}

//------------------------------------------------------------------------------

template <typename TP, Long NC>
typename TaskListT<TP, NC>::Item&
TaskListT<TP, NC>::operator[] (const Index i) noexcept {
	HFSM2_IF_ASSERT(verifyStructure());

	return _items[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TP, Long NC>
const typename TaskListT<TP, NC>::Item&
TaskListT<TP, NC>::operator[] (const Index i) const noexcept {
	HFSM2_IF_ASSERT(verifyStructure());

	return _items[i];
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_ASSERT

template <typename TP, Long NC>
void
TaskListT<TP, NC>::verifyStructure(const Index occupied) const noexcept {
	if (_count < CAPACITY) {
		HFSM2_ASSERT(_vacantHead < CAPACITY);
		HFSM2_ASSERT(_vacantTail < CAPACITY);

		HFSM2_ASSERT(_items[_vacantHead].prev == INVALID);
		HFSM2_ASSERT(_items[_vacantTail].next == INVALID);

		auto emptyCount = 1;

		for (auto c = _vacantHead; c != _vacantTail; ) {
			HFSM2_ASSERT(occupied != c);

			const auto& current = _items[c];

			const auto f = current.next;
			if (f != INVALID) {
				// next
				const auto& following = _items[f];

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
