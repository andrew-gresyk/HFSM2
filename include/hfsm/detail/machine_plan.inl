namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

Status::Status(const bool success_,
			   const bool failure_,
			   const bool innerTransition_,
			   const bool outerTransition_)
		: success{success_}
		, failure{failure_}
		, innerTransition{innerTransition_}
		, outerTransition{outerTransition_}
	{}

//------------------------------------------------------------------------------

void
Status::clear() {
	success = false;
	failure = false;
	innerTransition = false;
	outerTransition = false;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TStateList, LongIndex NPlanCapacity>
PlanT<TStateList, NPlanCapacity>::Iterator::Iterator(PlanT& plan)
	: _plan{plan}
	, _curr{plan._taskIndices.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TStateList, LongIndex NPlanCapacity>
PlanT<TStateList, NPlanCapacity>::Iterator::operator bool() const {
	assert(_curr < PlanT::CAPACITY || _curr == INVALID_LONG_INDEX);

	return _curr < PlanT::CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TStateList, LongIndex NPlanCapacity>
void
PlanT<TStateList, NPlanCapacity>::Iterator::operator ++() {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TStateList, LongIndex NPlanCapacity>
void
PlanT<TStateList, NPlanCapacity>::Iterator::remove() {
	_plan.remove(_curr);
}

//------------------------------------------------------------------------------

template <typename TStateList, LongIndex NPlanCapacity>
LongIndex
PlanT<TStateList, NPlanCapacity>::Iterator::next() const {
	if (_curr < PlanT::CAPACITY) {
		const TaskTransition& task = _plan._tasks[_curr];

		return task.next;
	} else {
		assert(_curr == INVALID_LONG_INDEX);

		return INVALID_LONG_INDEX;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TStateList, LongIndex NPlanCapacity>
PlanT<TStateList, NPlanCapacity>::PlanT(Tasks& tasks,
										StateTasks& stateTasks,
										const StateID plannerId)
	: _tasks{tasks}
	, _taskIndices{stateTasks[plannerId]}
{}

//------------------------------------------------------------------------------

template <typename TStateList, LongIndex NPlanCapacity>
PlanT<TStateList, NPlanCapacity>::operator bool() const {
	if (_taskIndices.first < PlanT::CAPACITY) {
		assert(_taskIndices.last < PlanT::CAPACITY);
		return true;
	} else {
		assert(_taskIndices.last == INVALID_LONG_INDEX);
		return false;
	}
}

//------------------------------------------------------------------------------

template <typename TStateList, LongIndex NPlanCapacity>
void
PlanT<TStateList, NPlanCapacity>::clear() {
	if (_taskIndices.first < Tasks::CAPACITY) {
		assert(_taskIndices.last < Tasks::CAPACITY);

		for (LongIndex index = _taskIndices.first;
			 index != INVALID_LONG_INDEX;
			 )
		{
			assert(index < Tasks::CAPACITY);

			const auto& task = _tasks[index];
			assert(index == _taskIndices.first ?
				   task.prev == INVALID_LONG_INDEX :
				   task.prev <  Tasks::CAPACITY);

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

	if (_taskIndices.first < Tasks::CAPACITY) {
		assert(_taskIndices.last < Tasks::CAPACITY);

		auto& last  = _tasks[_taskIndices.last];
		last.next = index;

		auto& next = _tasks[index];
		next.prev  = _taskIndices.last;

		_taskIndices.last = index;
	} else {
		assert(_taskIndices.first == INVALID_LONG_INDEX &&
			   _taskIndices.last  == INVALID_LONG_INDEX);

		_taskIndices.first = index;
		_taskIndices.last  = index;
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
void
PlanT<TStateList, NPlanCapacity>::remove(const LongIndex task) {
	assert(_taskIndices.first < Tasks::CAPACITY &&
		   _taskIndices.last  < Tasks::CAPACITY);

	assert(task < Tasks::CAPACITY);

	const TaskTransition& curr = _tasks[task];

	if (curr.prev < Tasks::CAPACITY) {
		TaskTransition& prev = _tasks[curr.prev];
		prev.next = curr.next;
	} else {
		assert(_taskIndices.first == task);
		_taskIndices.first = curr.next;
	}

	if (curr.next < Tasks::CAPACITY) {
		TaskTransition& next = _tasks[curr.next];
		next.prev = curr.prev;
	} else {
		assert(_taskIndices.last == task);
		_taskIndices.last = curr.prev;
	}

	_tasks.remove(task);
}

////////////////////////////////////////////////////////////////////////////////

}
}
