namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, LongIndex NC>
template <typename... TA>
LongIndex
List<T, NC>::emplace(TA... args) {
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

		new (&cell.item) Item{std::forward<TA>(args)...};

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

template <typename T, LongIndex NC>
void
List<T, NC>::remove(const Index i) {
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

template <typename T, LongIndex NC>
T&
List<T, NC>::operator[] (const Index i) {
	HFSM_IF_ASSERT(verifyStructure());

	return _cells[i].item;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, LongIndex NC>
const T&
List<T, NC>::operator[] (const Index i) const {
	HFSM_IF_ASSERT(verifyStructure());

	return _cells[i].item;
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_ASSERT

template <typename T, LongIndex NC>
void
List<T, NC>::verifyStructure(const Index occupied) const {
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
