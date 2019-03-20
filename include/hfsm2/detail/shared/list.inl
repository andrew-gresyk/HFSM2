namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TItem, LongIndex NCapacity>
template <typename... TArgs>
LongIndex
List<TItem, NCapacity>::emplace(TArgs... args) {
	if (_count < CAPACITY) {
		HFSM_ASSERT(_vacantHead < CAPACITY);
		HFSM_ASSERT(_vacantTail < CAPACITY);

		const Index result = _vacantHead;
		auto& cell = _cells[result];
		++_count;

		if (_vacantHead != _vacantTail) {
			// recycle
			HFSM_ASSERT(cell.links.prev == INVALID);
			HFSM_ASSERT(cell.links.next != INVALID);

			_vacantHead = cell.links.next;

			auto& head = _cells[_vacantHead];
			HFSM_ASSERT(head.links.prev == result);
			head.links.prev = INVALID;
		} else if (_last < CAPACITY) {
			// grow
			++_last;
			_vacantHead = _last;
			_vacantTail = _last;

			auto& nextVacant = _cells[_vacantHead];
			nextVacant.links.prev = INVALID;
			nextVacant.links.next = INVALID;
		} else {
			HFSM_ASSERT(_count == CAPACITY);

			_vacantHead = INVALID;
			_vacantTail = INVALID;
		}

		HFSM_IF_ASSERT(verifyStructure());

		new (&cell.item) Item{std::forward<TArgs>(args)...};

		return result;
	} else {
		// full
		HFSM_ASSERT(_vacantHead == INVALID);
		HFSM_ASSERT(_vacantTail == INVALID);
		HFSM_ASSERT(_count == CAPACITY);
		HFSM_BREAK();

		return INVALID;
	}
}

//------------------------------------------------------------------------------

template <typename TItem, LongIndex NCapacity>
void
List<TItem, NCapacity>::remove(const Index i) {
	HFSM_ASSERT(i < CAPACITY && _count);

	auto& fresh = _cells[i];

	if (_count < CAPACITY) {
		HFSM_ASSERT(_vacantHead < CAPACITY);
		HFSM_ASSERT(_vacantTail < CAPACITY);

		fresh.links.prev = INVALID;
		fresh.links.next = _vacantHead;

		auto& head = _cells[_vacantHead];
		head.links.prev = i;

		_vacantHead = i;
	} else {
		// 0 -> 1
		HFSM_ASSERT(_count == CAPACITY);
		HFSM_ASSERT(_vacantHead == INVALID);
		HFSM_ASSERT(_vacantTail == INVALID);

		fresh.links.prev = INVALID;
		fresh.links.next = INVALID;

		_vacantHead = i;
		_vacantTail = i;
	}

	--_count;

	HFSM_IF_ASSERT(verifyStructure());
}

//------------------------------------------------------------------------------

template <typename TItem, LongIndex NCapacity>
TItem&
List<TItem, NCapacity>::operator[] (const Index i) {
	HFSM_IF_ASSERT(verifyStructure());

	return _cells[i].item;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TItem, LongIndex NCapacity>
const TItem&
List<TItem, NCapacity>::operator[] (const Index i) const {
	HFSM_IF_ASSERT(verifyStructure());

	return _cells[i].item;
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_ASSERT

template <typename TItem, LongIndex NCapacity>
void
List<TItem, NCapacity>::verifyStructure(const Index occupied) const {
	if (_count < CAPACITY) {
		HFSM_ASSERT(_vacantHead < CAPACITY);
		HFSM_ASSERT(_vacantTail < CAPACITY);

		HFSM_ASSERT(_cells[_vacantHead].links.prev == INVALID);
		HFSM_ASSERT(_cells[_vacantTail].links.next == INVALID);

		auto emptyCount = 1;

		for (auto c = _vacantHead; c != _vacantTail; ) {
			HFSM_ASSERT(occupied != c);

			const auto& current = _cells[c];

			const auto f = current.links.next;
			if (f != INVALID) {
				// next
				const auto& following = _cells[f];

				HFSM_ASSERT(following.links.prev == c);

				c = f;
				continue;
			} else {
				// end
				HFSM_ASSERT(_vacantTail == c);
				HFSM_ASSERT(_count == CAPACITY - emptyCount);

				break;
			}
		}
	} else {
		HFSM_ASSERT(_vacantHead == INVALID);
		HFSM_ASSERT(_vacantTail == INVALID);
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
