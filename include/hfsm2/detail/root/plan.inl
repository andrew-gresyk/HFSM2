namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

Status::Status(const Result result_,
			   const bool outerTransition_) noexcept
	: result{result_}
	, outerTransition{outerTransition_}
{}

//------------------------------------------------------------------------------

void
Status::clear() noexcept {
	result = Result::NONE;
	outerTransition = false;
}

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_PLANS

template <typename TArgs>
CPlanT<TArgs>::IteratorT::IteratorT(const CPlanT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
CPlanT<TArgs>::IteratorT::operator bool() const noexcept {
	HFSM2_ASSERT(_curr < CPlanT::TASK_CAPACITY || _curr == INVALID_LONG);

	return _curr < CPlanT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
CPlanT<TArgs>::IteratorT::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
Long
CPlanT<TArgs>::IteratorT::next() const noexcept {
	if (_curr < CPlanT::TASK_CAPACITY) {
		const TaskLink& link = _plan._planData.taskLinks[_curr];

		return link.next;
	} else {
		HFSM2_ASSERT(_curr == INVALID_LONG);

		return INVALID_LONG;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
CPlanT<TArgs>::CPlanT(const PlanData& planData,
					  const RegionID regionId) noexcept
	: _planData{planData}
	, _bounds{planData.tasksBounds[regionId]}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
CPlanT<TArgs>::operator bool() const noexcept {
	if (_bounds.first < TASK_CAPACITY) {
		HFSM2_ASSERT(_bounds.last < TASK_CAPACITY);
		return true;
	} else {
		HFSM2_ASSERT(_bounds.last == INVALID_LONG);
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
PlanBaseT<TArgs>::IteratorT::IteratorT(PlanBaseT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
PlanBaseT<TArgs>::IteratorT::operator bool() const noexcept {
	HFSM2_ASSERT(_curr < PlanBaseT::TASK_CAPACITY || _curr == INVALID_LONG);

	return _curr < PlanBaseT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanBaseT<TArgs>::IteratorT::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanBaseT<TArgs>::IteratorT::remove() noexcept {
	_plan.remove(_curr);
}

//------------------------------------------------------------------------------

template <typename TArgs>
Long
PlanBaseT<TArgs>::IteratorT::next() const noexcept {
	if (_curr < PlanBaseT::TASK_CAPACITY) {
		const TaskLink& link = _plan._planData.taskLinks[_curr];

		return link.next;
	} else {
		HFSM2_ASSERT(_curr == INVALID_LONG);

		return INVALID_LONG;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
PlanBaseT<TArgs>::CIterator::CIterator(const PlanBaseT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
PlanBaseT<TArgs>::CIterator::operator bool() const noexcept {
	HFSM2_ASSERT(_curr < PlanBaseT::TASK_CAPACITY || _curr == INVALID_LONG);

	return _curr < PlanBaseT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanBaseT<TArgs>::CIterator::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
Long
PlanBaseT<TArgs>::CIterator::next() const noexcept {
	if (_curr < PlanBaseT::TASK_CAPACITY) {
		const TaskLink& link = _plan._planData.taskLinks[_curr];

		return link.next;
	} else {
		HFSM2_ASSERT(_curr == INVALID_LONG);

		return INVALID_LONG;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
PlanBaseT<TArgs>::PlanBaseT(PlanData& planData,
							const RegionID regionId) noexcept
	: _planData{planData}
	, _regionId{regionId}
	, _bounds{planData.tasksBounds[regionId]}
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
PlanBaseT<TArgs>::operator bool() const noexcept {
	if (_bounds.first < TASK_CAPACITY) {
		HFSM2_ASSERT(_bounds.last < TASK_CAPACITY);
		return true;
	} else {
		HFSM2_ASSERT(_bounds.last == INVALID_LONG);
		return false;
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
bool
PlanBaseT<TArgs>::append(const StateID origin,
						 const StateID destination,
						 const TransitionType transitionType) noexcept
{
	_planData.planExists.set(_regionId);

	return linkTask(_planData.tasks.emplace(origin, destination, transitionType));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
bool
PlanBaseT<TArgs>::linkTask(const Long index) noexcept {
	if (index != Tasks::INVALID) {
		if (_bounds.first == INVALID_LONG) {
			HFSM2_ASSERT(_bounds.last == INVALID_LONG);
			HFSM2_ASSERT(_planData.taskLinks[index].prev == INVALID_LONG);
			HFSM2_ASSERT(_planData.taskLinks[index].next == INVALID_LONG);

			_bounds.first = index;
			_bounds.last  = index;
		} else {
			HFSM2_ASSERT(_bounds.first < TaskLinks::CAPACITY);
			HFSM2_ASSERT(_bounds.last  < TaskLinks::CAPACITY);

			auto& lastLink = _planData.taskLinks[_bounds.last];
			HFSM2_ASSERT(lastLink.next == INVALID_LONG);

			lastLink.next  = index;

			auto& currLink = _planData.taskLinks[index];
			HFSM2_ASSERT(lastLink.prev == INVALID_LONG);

			currLink.prev  = _bounds.last;

			_bounds.last   = index;
		}

		return true;
	} else
		return false;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanBaseT<TArgs>::clear() noexcept {
	if (_bounds.first < TaskLinks::CAPACITY) {
		HFSM2_ASSERT(_bounds.last < TaskLinks::CAPACITY);

		for (Long index = _bounds.first;
			 index != INVALID_LONG;
			 )
		{
			HFSM2_ASSERT(index < TaskLinks::CAPACITY);

			const auto& taskLink = _planData.taskLinks[index];
			HFSM2_ASSERT(index == _bounds.first ?
							 taskLink.prev == INVALID_LONG :
							 taskLink.prev <  TaskLinks::CAPACITY);

			const Long next = taskLink.next;

			_planData.tasks.remove(index);

			index = next;
		}

		_bounds.first = INVALID_LONG;
		_bounds.last  = INVALID_LONG;
	} else {
		HFSM2_ASSERT(_bounds.first == INVALID_LONG);
		HFSM2_ASSERT(_bounds.last  == INVALID_LONG);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanBaseT<TArgs>::remove(const Long task) noexcept {
	HFSM2_ASSERT(_planData.planExists.get(_regionId));
	HFSM2_ASSERT(_bounds.first < TaskLinks::CAPACITY);
	HFSM2_ASSERT(_bounds.last  < TaskLinks::CAPACITY);

	HFSM2_ASSERT(task < TaskLinks::CAPACITY);

	TaskLink& curr = _planData.taskLinks[task];

	if (curr.prev < TaskLinks::CAPACITY) {
		TaskLink& prev = _planData.taskLinks[curr.prev];
		prev.next = curr.next;
	} else {
		HFSM2_ASSERT(_bounds.first == task);
		_bounds.first = curr.next;
	}

	if (curr.next < TaskLinks::CAPACITY) {
		TaskLink& next = _planData.taskLinks[curr.next];
		next.prev = curr.prev;
	} else {
		HFSM2_ASSERT(_bounds.last == task);
		_bounds.last = curr.prev;
	}

	curr.prev = INVALID_LONG;
	curr.next = INVALID_LONG;

	_planData.tasks.remove(task);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL, Long NTC, typename TTP>
bool
PlanT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL, NTC, TTP>>::append(const StateID origin,
																		  const StateID destination,
																		  const TransitionType transitionType,
																		  const Payload& payload) noexcept
{
	_planData.planExists.set(_regionId);

	return linkTask(_planData.tasks.emplace(origin, destination, transitionType, payload));
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL, Long NTC, typename TTP>
bool
PlanT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL, NTC, TTP>>::append(const StateID origin,
																		  const StateID destination,
																		  const TransitionType transitionType,
																		  Payload&& payload) noexcept
{
	_planData.planExists.set(_regionId);

	return linkTask(_planData.tasks.emplace(origin, destination, transitionType, std::move(payload)));
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
