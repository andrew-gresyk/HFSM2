namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TStateList, LongIndex NPlanCapacity>
void
PlanT<TStateList, NPlanCapacity>::clear() {
	if (_taskIndices.first < _tasks.CAPACITY) {
		assert(_taskIndices.last < _tasks.CAPACITY);

		for (LongIndex index = _taskIndices.first;
			 index != INVALID_LONG_INDEX;
			 )
		{
			assert(index < _tasks.CAPACITY);

			const auto& task = _tasks[index];
			const LongIndex next = task.next;

			_tasks.remove(index);

			index = next;
		}

		_taskIndices.first = INVALID_LONG_INDEX;
		_taskIndices.last  = INVALID_LONG_INDEX;
	} else
		assert(_taskIndices.first == INVALID_LONG_INDEX &&
			   _taskIndices.last  == INVALID_LONG_INDEX);
}

//------------------------------------------------------------------------------

template <typename TStateList, LongIndex NPlanCapacity>
void
PlanT<TStateList, NPlanCapacity>::add(const StateID origin,
									  const StateID destination)
{
	const auto index = _tasks.emplace(origin, destination);

	if (_taskIndices.first < _tasks.CAPACITY) {
		assert(_taskIndices.last < _tasks.CAPACITY);

		auto& last = _tasks[_taskIndices.last];
		last.next = index;

		_taskIndices.last = index;
	} else {
		assert(_taskIndices.first == INVALID_LONG_INDEX &&
			   _taskIndices.last  == INVALID_LONG_INDEX);

		_taskIndices.first = index;
		_taskIndices.last  = index;
	}

	_taskIndices.last = index;
}

//------------------------------------------------------------------------------

template <typename TStateList, LongIndex NPlanCapacity>
template <typename TOrigin, typename TDestination>
void
PlanT<TStateList, NPlanCapacity>::add() {
	using Origin = TOrigin;
	using Destination = TDestination;

	add(stateId<Origin>(), stateId<Destination>());
}

//------------------------------------------------------------------------------

template <typename TStateList, LongIndex NPlanCapacity>
const TaskTransition*
PlanT<TStateList, NPlanCapacity>::next() const {
	if (_taskIndices.first < _tasks.CAPACITY) {
		assert(_taskIndices.last < _tasks.CAPACITY);

		return &_tasks[_taskIndices.first];
	} else {
		assert(_taskIndices.first == INVALID_LONG_INDEX &&
			   _taskIndices.last  == INVALID_LONG_INDEX);

		return nullptr;
	}
}

//------------------------------------------------------------------------------

template <typename TStateList, LongIndex NPlanCapacity>
void
PlanT<TStateList, NPlanCapacity>::advance() {
}

////////////////////////////////////////////////////////////////////////////////

}
}
