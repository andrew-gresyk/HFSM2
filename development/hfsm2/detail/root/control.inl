namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
ConstControlT<TArgs>::Origin::Origin(ConstControlT& control_,
									 const StateID stateId_) noexcept
	: control{control_}
	, prevId{control._originId}
{
	HFSM2_ASSERT(stateId_ < StateList::SIZE);
	control._originId = stateId_;
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(20)
ConstControlT<TArgs>::Origin::~Origin() noexcept {
	control._originId = prevId;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
ConstControlT<TArgs>::Region::Region(ConstControlT& control_,
									 const RegionID regionId_) noexcept
	: control{control_}
	, prevId{control._regionId}
{
	control.setRegion(regionId_);
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(20)
ConstControlT<TArgs>::Region::~Region() noexcept {
	control.resetRegion(prevId);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
ControlT<TArgs>::Origin::Origin(ControlT& control_,
								const StateID stateId_) noexcept
	: control{control_}
	, prevId{control._originId}
{
	HFSM2_ASSERT(stateId_ < StateList::SIZE);
	control._originId = stateId_;
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(20)
ControlT<TArgs>::Origin::~Origin() noexcept {
	control._originId = prevId;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
ControlT<TArgs>::Region::Region(ControlT& control_,
								const RegionID regionId_) noexcept
	: control{control_}
	, prevId{control._regionId}
{
	control.setRegion(regionId_);
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(20)
ControlT<TArgs>::Region::~Region() noexcept {
	control.resetRegion(prevId);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
ControlT<TArgs>::setRegion(const RegionID regionId_) noexcept {
	HFSM2_ASSERT(_regionId <= regionId_ && regionId_ < RegionList::SIZE);

	_regionId = regionId_;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
ControlT<TArgs>::resetRegion(const RegionID regionId_) noexcept { //-V524
	HFSM2_ASSERT(regionId_ <= _regionId && _regionId < RegionList::SIZE);

	_regionId = regionId_;
}

//------------------------------------------------------------------------------

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
ControlT<TArgs>::pinLastTransition(const StateID stateId_,
								   const Short index) noexcept
{
	if (index != INVALID_SHORT) {
		HFSM2_ASSERT(index < TransitionSets::CAPACITY);

		if (!_core.registry.isActive(stateId_))
			_core.transitionTargets[stateId_] = index;
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
const typename ControlT<TArgs>::Transition*
ControlT<TArgs>::lastTransitionTo(const StateID stateId_) const noexcept {
	if (HFSM2_CHECKED(stateId_ < StateList::SIZE)) {
		const Short index = _core.transitionTargets[stateId_];

		if (index < _core.previousTransitions.count())
			return &_core.previousTransitions[index];
	}

	return nullptr;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
HFSM2_CONSTEXPR(14)
const typename ControlT<TArgs>::Transition*
ControlT<TArgs>::lastTransition() const noexcept {
	HFSM2_ASSERT(_originId < _core.transitionTargets.CAPACITY);

	return lastTransitionTo(_originId);
}

#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
PlanControlT<TArgs>::Region::Region(PlanControlT& control_,
									const RegionID regionId_,
									const StateID index,
									const Long size) noexcept
	: control  {control_}
	, prevId   {control._regionId}
	, prevIndex{control._regionStateId}
	, prevSize {control._regionSize}
{
	control.setRegion(regionId_, index, size);
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(20)
PlanControlT<TArgs>::Region::~Region() noexcept {
	control.resetRegion(prevId, prevIndex, prevSize);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
PlanControlT<TArgs>::setRegion(const RegionID regionId_,
							   const StateID index,
							   const Long size) noexcept
{
	HFSM2_ASSERT(_regionId <= regionId_ && regionId_ <  RegionList::SIZE);
	HFSM2_ASSERT(_regionStateId <= index && index + size <= _regionStateId + _regionSize);

	_regionId	   = regionId_;
	_regionStateId = index;
	_regionSize	   = size;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
PlanControlT<TArgs>::resetRegion(const RegionID regionId_, //-V524
								 const StateID index,
								 const Long size) noexcept
{
	HFSM2_ASSERT(regionId_ <= _regionId && _regionId < RegionList::SIZE);
	HFSM2_ASSERT(index <= _regionStateId && _regionStateId + _regionSize <= index + size);

	_regionId	   = regionId_;
	_regionStateId = index;
	_regionSize	   = size;

	_taskStatus.clear();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
FullControlBaseT<TArgs>::Lock::Lock(FullControlBaseT& control_) noexcept
	: control{!control_._locked ? &control_ : nullptr}
{
	if (control)
		control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(20)
FullControlBaseT<TArgs>::Lock::~Lock() noexcept {
	if (control)
		control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

#if HFSM2_PLANS_AVAILABLE()

template <typename TArgs>
template <typename TState>
HFSM2_CONSTEXPR(14)
TaskStatus
FullControlBaseT<TArgs>::buildPlanStatus() noexcept {
	constexpr StateID STATE_ID = TState::STATE_ID;

	switch (_taskStatus.result) {
	case TaskStatus::NONE:
		HFSM2_BREAK();
		break;

	case TaskStatus::SUCCESS:
		_core.planData.tasksSuccesses.template set<STATE_ID>();

		HFSM2_LOG_PLAN_STATUS(context(), _regionId, StatusEvent::SUCCEEDED);
		break;

	case TaskStatus::FAILURE:
		_core.planData.tasksFailures .template set<STATE_ID>();

		HFSM2_LOG_PLAN_STATUS(context(), _regionId, StatusEvent::FAILED);
		break;

	default:
		HFSM2_BREAK();
	}

	return {_taskStatus.result};
}

#endif

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::changeTo(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::CHANGE});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::CHANGE, stateId_);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::restart(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::RESTART});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESTART, stateId_);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::resume(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::RESUME});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESUME, stateId_);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::select(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::SELECT});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SELECT, stateId_);
	}
}

//------------------------------------------------------------------------------

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::utilize(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::UTILIZE});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::UTILIZE, stateId_);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::randomize(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::RANDOMIZE});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RANDOMIZE, stateId_);
	}
}

#endif

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::schedule(const StateID stateId_) noexcept {
	_core.requests.emplace(Transition{_originId, stateId_, TransitionType::SCHEDULE});

	HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SCHEDULE, stateId_);
}

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::succeed(const StateID stateId_) noexcept {
	_taskStatus.result = TaskStatus::SUCCESS;

	_core.planData.tasksSuccesses.set(stateId_);

	// TODO: promote taskSuccess all the way up for all regions without plans
	if (_regionId < RegionList::SIZE && !_core.planData.planExists.get(_regionId)) {
		HFSM2_ASSERT(_regionStateId < StateList::SIZE);

		_core.planData.tasksSuccesses.set(_regionStateId);
	}

	HFSM2_LOG_TASK_STATUS(context(), _regionId, stateId_, StatusEvent::SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::fail(const StateID stateId_) noexcept {
	_taskStatus.result = TaskStatus::FAILURE;

	_core.planData.tasksFailures.set(stateId_);

	// TODO: promote taskFailure all the way up for all regions without plans
	if (_regionId < RegionList::SIZE && !_core.planData.planExists.get(_regionId)) {
		HFSM2_ASSERT(_regionStateId < StateList::SIZE);

		_core.planData.tasksFailures.set(_regionStateId);
	}

	HFSM2_LOG_TASK_STATUS(context(), _regionId, stateId_, StatusEvent::FAILED);
}

#endif

////////////////////////////////////////////////////////////////////////////////

#if HFSM2_PLANS_AVAILABLE()

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
template <typename TState>
HFSM2_CONSTEXPR(14)
TaskStatus
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::updatePlan(TState& headState,
																											   const TaskStatus subStatus) noexcept
{
	constexpr StateID STATE_ID = TState::STATE_ID;

	HFSM2_ASSERT(subStatus);

	if (subStatus.result == TaskStatus::FAILURE) {
		_taskStatus.result = TaskStatus::FAILURE;
		headState.wrapPlanFailed(*this);

		plan().clear();

		return FullControlBase::template buildPlanStatus<TState>();
	} else if (subStatus.result == TaskStatus::SUCCESS) {
		if (Plan p = plan(_regionId)) {
			TasksBits processed;

			for (auto it = p.first(); it; ++it) {
				if (isActive(it->origin) &&
					_core.planData.tasksSuccesses.get(it->origin) &&
					!processed.get(it->origin))
				{
					Origin origin{*this, STATE_ID};

					if (const Payload* const payload = it->payload())
						changeWith(it->destination, *it->payload());
					else
						changeTo  (it->destination);

					_core.planData.tasksSuccesses.clear(it->origin);
					processed.set(it->origin);
					it.remove();
				}
			}

			return TaskStatus{};
		} else {
			_taskStatus.result = TaskStatus::SUCCESS;
			headState.wrapPlanSucceeded(*this);

			plan().clear();

			return FullControlBase::template buildPlanStatus<TState>();
		}
	} else
		return TaskStatus{};
}

#endif

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::changeWith(const StateID  stateId_,
																																const Payload& payload) noexcept
{
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::CHANGE, payload});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::CHANGE, stateId_);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::restartWith(const StateID  stateId_,
																																 const Payload& payload) noexcept
{
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::RESTART, payload});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESTART, stateId_);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::resumeWith(const StateID  stateId_,
																																const Payload& payload) noexcept
{
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::RESUME, payload});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESUME, stateId_);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::selectWith(const StateID  stateId_,
																																const Payload& payload) noexcept
{
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::SELECT, payload});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SELECT, stateId_);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::utilizeWith(const StateID  stateId_,
																																 const Payload& payload) noexcept
{
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::UTILIZE, payload});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::UTILIZE, stateId_);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::randomizeWith(const StateID  stateId_,
																																   const Payload& payload) noexcept
{
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::RANDOMIZE, payload});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RANDOMIZE, stateId_);
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL HFSM2_IF_PLANS(, NTC), TTP>>::scheduleWith(const StateID  stateId_,
																																  const Payload& payload) noexcept
{
	_core.requests.emplace(Transition{_originId, stateId_, TransitionType::SCHEDULE, payload});

	HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SCHEDULE, stateId_);
}

////////////////////////////////////////////////////////////////////////////////

#if HFSM2_PLANS_AVAILABLE()

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
template <typename TState>
HFSM2_CONSTEXPR(14)
TaskStatus
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::updatePlan(TState& headState,
																												const TaskStatus subStatus) noexcept
{
	constexpr StateID STATE_ID = TState::STATE_ID;

	HFSM2_ASSERT(subStatus);

	if (subStatus.result == TaskStatus::FAILURE) {
		_taskStatus.result = TaskStatus::FAILURE;
		headState.wrapPlanFailed(*this);

		if (Plan p = plan(_regionId))
			p.clear();

		return FullControlBase::template buildPlanStatus<TState>();
	} else if (subStatus.result == TaskStatus::SUCCESS) {
		if (Plan p = plan(_regionId)) {
			TasksBits processed;

			for (auto it = p.first(); it; ++it) {
				if (isActive(it->origin) &&
					_core.planData.tasksSuccesses.get(it->origin) &&
					!processed.get(it->origin))
				{
					Origin origin{*this, STATE_ID};
					changeTo(it->destination);

					_core.planData.tasksSuccesses.clear(it->origin);
					processed.set(it->origin);
					it.remove();
				}
			}

			return TaskStatus{};
		} else {
			_taskStatus.result = TaskStatus::SUCCESS;
			headState.wrapPlanSucceeded(*this);

			return FullControlBase::template buildPlanStatus<TState>();
		}
	} else
		return TaskStatus{};
}

#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
GuardControlT<TArgs>::cancelPendingTransitions() noexcept {
	_cancelled = true;

	HFSM2_LOG_CANCELLED_PENDING(context(), _originId);
}

////////////////////////////////////////////////////////////////////////////////

}
}
