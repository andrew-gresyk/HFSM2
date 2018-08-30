namespace hfsm2 {
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

template <typename TSL, ShortIndex NRC, LongIndex NTC>
PlanT<TSL, NRC, NTC>::Iterator::Iterator(PlanT& plan)
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TSL, ShortIndex NRC, LongIndex NTC>
PlanT<TSL, NRC, NTC>::Iterator::operator bool() const {
	assert(_curr < PlanT::TASK_CAPACITY || _curr == INVALID_LONG_INDEX);

	return _curr < PlanT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TSL, ShortIndex NRC, LongIndex NTC>
void
PlanT<TSL, NRC, NTC>::Iterator::operator ++() {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TSL, ShortIndex NRC, LongIndex NTC>
void
PlanT<TSL, NRC, NTC>::Iterator::remove() {
	_plan.remove(_curr);
}

//------------------------------------------------------------------------------

template <typename TSL, ShortIndex NRC, LongIndex NTC>
LongIndex
PlanT<TSL, NRC, NTC>::Iterator::next() const {
	if (_curr < PlanT::TASK_CAPACITY) {
		const TaskLink& task = _plan._taskLinks[_curr];

		return task.next;
	} else {
		assert(_curr == INVALID_LONG_INDEX);

		return INVALID_LONG_INDEX;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TSL, ShortIndex NRC, LongIndex NTC>
PlanT<TSL, NRC, NTC>::PlanT(TaskLinks& tasks,
							TasksBounds& tasksBounds,
							const RegionID regionId)

	: _taskLinks{tasks}
	, _bounds{tasksBounds[regionId]}
{}

//------------------------------------------------------------------------------

template <typename TSL, ShortIndex NRC, LongIndex NTC>
PlanT<TSL, NRC, NTC>::operator bool() const {
	if (_bounds.first < PlanT::TASK_CAPACITY) {
		assert(_bounds.last < PlanT::TASK_CAPACITY);
		return true;
	} else {
		assert(_bounds.last == INVALID_LONG_INDEX);
		return false;
	}
}

//------------------------------------------------------------------------------

template <typename TSL, ShortIndex NRC, LongIndex NTC>
void
PlanT<TSL, NRC, NTC>::clear() {
	if (_bounds.first < TaskLinks::CAPACITY) {
		assert(_bounds.last < TaskLinks::CAPACITY);

		for (LongIndex index = _bounds.first;
			 index != INVALID_LONG_INDEX;
			 )
		{
			assert(index < TaskLinks::CAPACITY);

			const auto& task = _taskLinks[index];
			assert(index == _bounds.first ?
				   task.prev == INVALID_LONG_INDEX :
				   task.prev <  TaskLinks::CAPACITY);

			const LongIndex next = task.next;

			_taskLinks.remove(index);

			index = next;
		}

		_bounds.first = INVALID_LONG_INDEX;
		_bounds.last  = INVALID_LONG_INDEX;
	} else
		assert(_bounds.first == INVALID_LONG_INDEX &&
			   _bounds.last  == INVALID_LONG_INDEX);
}

//------------------------------------------------------------------------------

template <typename TSL, ShortIndex NRC, LongIndex NTC>
void
PlanT<TSL, NRC, NTC>::add(const StateID origin,
						  const StateID destination)
{
	const auto index = _taskLinks.emplace(origin, destination);

	if (_bounds.first < TaskLinks::CAPACITY) {
		assert(_bounds.last < TaskLinks::CAPACITY);

		auto& last  = _taskLinks[_bounds.last];
		last.next = index;

		auto& next = _taskLinks[index];
		next.prev  = _bounds.last;

		_bounds.last = index;
	} else {
		assert(_bounds.first == INVALID_LONG_INDEX &&
			   _bounds.last  == INVALID_LONG_INDEX);

		_bounds.first = index;
		_bounds.last  = index;
	}
}

//------------------------------------------------------------------------------

template <typename TSL, ShortIndex NRC, LongIndex NTC>
void
PlanT<TSL, NRC, NTC>::remove(const LongIndex task) {
	assert(_bounds.first < TaskLinks::CAPACITY &&
		   _bounds.last  < TaskLinks::CAPACITY);

	assert(task < TaskLinks::CAPACITY);

	const TaskLink& curr = _taskLinks[task];

	if (curr.prev < TaskLinks::CAPACITY) {
		TaskLink& prev = _taskLinks[curr.prev];
		prev.next = curr.next;
	} else {
		assert(_bounds.first == task);
		_bounds.first = curr.next;
	}

	if (curr.next < TaskLinks::CAPACITY) {
		TaskLink& next = _taskLinks[curr.next];
		next.prev = curr.prev;
	} else {
		assert(_bounds.last == task);
		_bounds.last = curr.prev;
	}

	_taskLinks.remove(task);
}

////////////////////////////////////////////////////////////////////////////////

}
}
