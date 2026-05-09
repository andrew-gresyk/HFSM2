#if HFSM2_PLANS_AVAILABLE()

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
PlanT<TArgs>::CIterator::CIterator(const PlanT& plan) noexcept
	: _plan{plan}
	, _curr{plan._planData.tasks.bounds(plan._regionId).first}
	, _next{next()}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
PlanT<TArgs>::CIterator::operator bool() const noexcept {
	HFSM2_ASSERT(_curr  < PlanT::TASK_CAPACITY ||
				 _curr == Tasks::invalid());

	return _curr < PlanT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
PlanT<TArgs>::CIterator::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
typename PlanT<TArgs>::TaskIndex
PlanT<TArgs>::CIterator::next() const noexcept {
	return _curr < PlanT::TASK_CAPACITY ?
		_plan._planData.tasks.next(_curr) :
		Tasks::invalid();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
PlanT<TArgs>::Iterator::Iterator(PlanT& plan) noexcept
	: _plan{plan}
	, _curr{plan._planData.tasks.bounds(plan._regionId).first}
	, _next{next()}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
PlanT<TArgs>::Iterator::operator bool() const noexcept {
	HFSM2_ASSERT(_curr < PlanT::TASK_CAPACITY ||
				 _curr == Tasks::invalid());

	return _curr < PlanT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
PlanT<TArgs>::Iterator::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
typename PlanT<TArgs>::TaskIndex
PlanT<TArgs>::Iterator::next() const noexcept {
	return _curr < PlanT::TASK_CAPACITY ?
		_plan._planData.tasks.next(_curr) :
		Tasks::invalid();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(11)
PlanT<TArgs>::PlanT(Registry& registry,
					PlanData& planData,
					const RegionID regionId_) noexcept
	: _registry{registry}
	, _planData{planData}
	, _regionId{regionId_}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
bool
PlanT<TArgs>::append(const StateID origin,
					 const StateID destination,
					 const TransitionType type) noexcept
{
	if (_planData.tasks.count() < TASK_CAPACITY) {
		_planData.planExists.set(_regionId);

		return _planData.tasks.emplace(_regionId, origin, destination, type) != Tasks::invalid();
	}
	else
		return false;
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
PlanT<TArgs>::clearTasks() noexcept {
	_planData.tasks.clearRegion(_regionId);
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
PlanT<TArgs>::clearStatuses() noexcept {
	TasksBits bitsToClear;
	bitsToClear.set();

	const StateID begin = _registry.regionHeads[_regionId];

	const StateID end   = _registry.regionHeads[_regionId] +
						  _registry.regionSizes[_regionId];

	for (StateID i = begin; i < end; ++i)
		bitsToClear.clear(i);

	_planData.tasksSuccesses &= bitsToClear;
	_planData.tasksFailures  &= bitsToClear;

	_planData.headStatuses[_regionId].clear();
	_planData. subStatuses[_regionId].clear();
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
PlanT<TArgs>::operator bool() const noexcept {
	const Bounds& bounds = _planData.tasks.bounds(_regionId);

	HFSM2_ASSERT(bounds.first < TASK_CAPACITY &&
				 bounds.last  < TASK_CAPACITY ||
				 bounds.last == Tasks::invalid());

	return bounds.first < TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
PlanT<TArgs>::clear() noexcept {
	clearTasks();
	clearStatuses();
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
PlanT<TArgs>::remove(const TaskIndex index) noexcept {
	HFSM2_ASSERT(_planData.planExists.get(_regionId));
	HFSM2_ASSERT(index < TASK_CAPACITY);

	_planData.tasks.remove(_regionId, index);
}

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
