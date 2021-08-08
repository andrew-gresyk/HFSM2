namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

HFSM2_CONSTEXPR(14)
void
Status::clear() noexcept {
	result = Result::NONE;
	outerTransition = false;
}

////////////////////////////////////////////////////////////////////////////////

#if HFSM2_PLANS_AVAILABLE()

template <typename TArgs>
HFSM2_CONSTEXPR(14)
CPlanT<TArgs>::IteratorT::IteratorT(const CPlanT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
CPlanT<TArgs>::IteratorT::operator bool() const noexcept {
	HFSM2_ASSERT(_curr < CPlanT::TASK_CAPACITY ||
				 _curr == INVALID_LONG);

	return _curr < CPlanT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
CPlanT<TArgs>::IteratorT::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
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
HFSM2_CONSTEXPR(14)
CPlanT<TArgs>::operator bool() const noexcept {
	HFSM2_ASSERT(_bounds.first < TASK_CAPACITY &&
				 _bounds.last  < TASK_CAPACITY ||
				 _bounds.last == INVALID_LONG);

	return _bounds.first < TASK_CAPACITY;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
PlanBaseT<TArgs>::IteratorT::IteratorT(PlanBaseT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
	, _next{next()}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
PlanBaseT<TArgs>::IteratorT::operator bool() const noexcept {
	HFSM2_ASSERT(_curr < PlanBaseT::TASK_CAPACITY ||
				 _curr == INVALID_LONG);

	return _curr < PlanBaseT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
PlanBaseT<TArgs>::IteratorT::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
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
HFSM2_CONSTEXPR(14)
PlanBaseT<TArgs>::CIterator::CIterator(const PlanBaseT& plan) noexcept
	: _plan{plan}
	, _curr{plan._bounds.first}
	, _next{next()}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
PlanBaseT<TArgs>::CIterator::operator bool() const noexcept {
	HFSM2_ASSERT(_curr < PlanBaseT::TASK_CAPACITY ||
				 _curr == INVALID_LONG);

	return _curr < PlanBaseT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
PlanBaseT<TArgs>::CIterator::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
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
HFSM2_CONSTEXPR(11)
PlanBaseT<TArgs>::PlanBaseT(PlanData& planData,
							const RegionID regionId) noexcept
	: _planData{planData}
	, _regionId{regionId}
	, _bounds{planData.tasksBounds[regionId]}
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
HFSM2_CONSTEXPR(14)
PlanBaseT<TArgs>::operator bool() const noexcept {
	HFSM2_ASSERT(_bounds.first < TASK_CAPACITY &&
				 _bounds.last  < TASK_CAPACITY ||
				 _bounds.last == INVALID_LONG);

	return _bounds.first < TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
bool
PlanBaseT<TArgs>::append(const StateID origin,
						 const StateID destination,
						 const TransitionType transitionType) noexcept
{
	if (_planData.tasks.count() < TASK_CAPACITY) {
		_planData.planExists.set(_regionId);

		return linkTask(_planData.tasks.emplace(origin, destination, transitionType));
	} else
		return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
HFSM2_CONSTEXPR(14)
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
			HFSM2_ASSERT(currLink.prev == INVALID_LONG);

			currLink.prev  = _bounds.last;

			_bounds.last   = index;
		}

		return true;
	} else
		return false;
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
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
HFSM2_CONSTEXPR(14)
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

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
HFSM2_CONSTEXPR(14)
bool
PlanT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::append(const StateID origin,
																								   const StateID destination,
																								   const TransitionType transitionType,
																								   const Payload& payload) noexcept
{
	_planData.planExists.set(_regionId);

	return linkTask(_planData.tasks.emplace(origin, destination, transitionType, payload));
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
HFSM2_CONSTEXPR(14)
bool
PlanT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::append(const StateID origin,
																								   const StateID destination,
																								   const TransitionType transitionType,
																								   Payload&& payload) noexcept
{
	_planData.planExists.set(_regionId);

	return linkTask(_planData.tasks.emplace(origin, destination, transitionType, move(payload)));
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
