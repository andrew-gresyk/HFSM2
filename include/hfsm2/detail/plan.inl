namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

Status::Status(const bool success_,
			   const bool failure_,
			   const bool outerTransition_)
		: success{success_}
		, failure{failure_}
		, outerTransition{outerTransition_}
	{}

//------------------------------------------------------------------------------

void
Status::clear() {
	success = false;
	failure = false;
	outerTransition = false;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
ConstPlanT<TArgs>::Iterator::Iterator(const ConstPlanT& plan)
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
ConstPlanT<TArgs>::Iterator::operator bool() const {
	HFSM_ASSERT(_curr < ConstPlanT::TASK_CAPACITY || _curr == INVALID_LONG_INDEX);

	return _curr < ConstPlanT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
ConstPlanT<TArgs>::Iterator::operator ++() {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
LongIndex
ConstPlanT<TArgs>::Iterator::next() const {
	if (_curr < ConstPlanT::TASK_CAPACITY) {
		const TaskLink& task = _plan._planData.taskLinks[_curr];

		return task.next;
	} else {
		HFSM_ASSERT(_curr == INVALID_LONG_INDEX);

		return INVALID_LONG_INDEX;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
ConstPlanT<TArgs>::ConstPlanT(const PlanData& planData,
							  const RegionID regionId)

	: _planData{planData}
	, _bounds{planData.tasksBounds[regionId]}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
ConstPlanT<TArgs>::operator bool() const {
	if (_bounds.first < TASK_CAPACITY) {
		HFSM_ASSERT(_bounds.last < TASK_CAPACITY);
		return true;
	} else {
		HFSM_ASSERT(_bounds.last == INVALID_LONG_INDEX);
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
PlanT<TArgs>::Iterator::Iterator(PlanT& plan)
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
PlanT<TArgs>::Iterator::operator bool() const {
	HFSM_ASSERT(_curr < PlanT::TASK_CAPACITY || _curr == INVALID_LONG_INDEX);

	return _curr < PlanT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanT<TArgs>::Iterator::operator ++() {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanT<TArgs>::Iterator::remove() {
	_plan.remove(_curr);
}

//------------------------------------------------------------------------------

template <typename TArgs>
LongIndex
PlanT<TArgs>::Iterator::next() const {
	if (_curr < PlanT::TASK_CAPACITY) {
		const TaskLink& task = _plan._planData.taskLinks[_curr];

		return task.next;
	} else {
		HFSM_ASSERT(_curr == INVALID_LONG_INDEX);

		return INVALID_LONG_INDEX;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
PlanT<TArgs>::PlanT(PlanData& planData,
					const RegionID regionId)

	: _planData{planData}
	, _planExists{planData.planExists[regionId]}
	, _bounds{planData.tasksBounds[regionId]}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
PlanT<TArgs>::operator bool() const {
	if (_bounds.first < TASK_CAPACITY) {
		HFSM_ASSERT(_bounds.last < TASK_CAPACITY);
		return true;
	} else {
		HFSM_ASSERT(_bounds.last == INVALID_LONG_INDEX);
		return false;
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
typename PlanDataT<TArgs>::TaskLinks::Index
PlanT<TArgs>::append(const Transition transition,
					 const StateID origin,
					 const StateID destination)
{
	_planExists = true;
	const TaskIndex index = _planData.taskLinks.emplace(transition, origin, destination);

	if (_bounds.first < TaskLinks::CAPACITY) {
		HFSM_ASSERT(_bounds.last < TaskLinks::CAPACITY);

		auto& last  = _planData.taskLinks[_bounds.last];
		last.next = index;

		auto& next = _planData.taskLinks[index];
		next.prev  = _bounds.last;

		_bounds.last = index;
	} else {
		HFSM_ASSERT(_bounds.first == INVALID_LONG_INDEX &&
					_bounds.last  == INVALID_LONG_INDEX);

		_bounds.first = index;
		_bounds.last  = index;
	}

	return index;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanT<TArgs>::clear() {
	if (_bounds.first < TaskLinks::CAPACITY) {
		HFSM_ASSERT(_bounds.last < TaskLinks::CAPACITY);

		for (LongIndex index = _bounds.first;
			 index != INVALID_LONG_INDEX;
			 )
		{
			HFSM_ASSERT(index < TaskLinks::CAPACITY);

			const auto& task = _planData.taskLinks[index];
			HFSM_ASSERT(index == _bounds.first ?
				   task.prev == INVALID_LONG_INDEX :
				   task.prev <  TaskLinks::CAPACITY);

			const LongIndex next = task.next;

			_planData.taskLinks.remove(index);

			index = next;
		}

		_bounds.first = INVALID_LONG_INDEX;
		_bounds.last  = INVALID_LONG_INDEX;
	} else
		HFSM_ASSERT(_bounds.first == INVALID_LONG_INDEX &&
			   _bounds.last  == INVALID_LONG_INDEX);
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanT<TArgs>::remove(const LongIndex task) {
	HFSM_ASSERT(_planExists &&
				_bounds.first < TaskLinks::CAPACITY &&
				_bounds.last  < TaskLinks::CAPACITY);

	HFSM_ASSERT(task < TaskLinks::CAPACITY);

	const TaskLink& curr = _planData.taskLinks[task];

	if (curr.prev < TaskLinks::CAPACITY) {
		TaskLink& prev = _planData.taskLinks[curr.prev];
		prev.next = curr.next;
	} else {
		HFSM_ASSERT(_bounds.first == task);
		_bounds.first = curr.next;
	}

	if (curr.next < TaskLinks::CAPACITY) {
		TaskLink& next = _planData.taskLinks[curr.next];
		next.prev = curr.prev;
	} else {
		HFSM_ASSERT(_bounds.last == task);
		_bounds.last = curr.prev;
	}

	_planData.taskLinks.remove(task);
}

////////////////////////////////////////////////////////////////////////////////

}
}
