namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <typename TArgs>
ControlT<TArgs>::Origin::Origin(ControlT& control_,
								const StateID stateId) noexcept
	: control{control_}
	, prevId{control._originId}
{
	control.setOrigin(stateId);
}

//------------------------------------------------------------------------------

template <typename TArgs>
ControlT<TArgs>::Origin::~Origin() noexcept {
	control.resetOrigin(prevId);
}

// COMMON
////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
ControlT<TArgs>::Region::Region(ControlT& control_,
								const RegionID regionId) noexcept
	: control{control_}
	, prevId{control._regionId}
{
	control.setRegion(regionId);
}

//------------------------------------------------------------------------------

template <typename TArgs>
ControlT<TArgs>::Region::~Region() noexcept {
	control.resetRegion(prevId);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
void
ControlT<TArgs>::setOrigin(const StateID stateId) noexcept {
	HFSM2_ASSERT(stateId < StateList::SIZE);

	_originId = stateId;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
void
ControlT<TArgs>::resetOrigin(const StateID stateId) noexcept { //-V524
	_originId = stateId;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
ControlT<TArgs>::setRegion(const RegionID regionId) noexcept {
	HFSM2_ASSERT(_regionId <= regionId && regionId < RegionList::SIZE);

	_regionId = regionId;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
void
ControlT<TArgs>::resetRegion(const RegionID regionId) noexcept { //-V524
	HFSM2_ASSERT(regionId <= _regionId && _regionId < RegionList::SIZE);

	_regionId = regionId;
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY

template <typename TArgs>
void
ControlT<TArgs>::pinLastTransition(const StateID stateId,
								   const Short index) noexcept
{
	if (index != INVALID_SHORT) {
		HFSM2_ASSERT(index < TransitionSets::CAPACITY);

		if (!_registry.isActive(stateId))
			_transitionTargets[stateId] = index;
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
const typename ControlT<TArgs>::Transition*
ControlT<TArgs>::lastTransition(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < StateList::SIZE)) {
		const Short index = _transitionTargets[stateId];

		if (index < _previousTransitions.count())
			return &_previousTransitions[index];
	}

	return nullptr;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
const typename ControlT<TArgs>::Transition*
ControlT<TArgs>::lastTransition() const noexcept {
	HFSM2_ASSERT(_originId < _transitionTargets.CAPACITY);

	return lastTransition(_originId);
}

#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
PlanControlT<TArgs>::Region::Region(PlanControlT& control_,
									const RegionID regionId,
									const StateID index,
									const Long size) noexcept
	: control  {control_}
	, prevId   {control._regionId}
	, prevIndex{control._regionStateId}
	, prevSize {control._regionSize}
{
	control.setRegion(regionId, index, size);
}

//------------------------------------------------------------------------------

template <typename TArgs>
PlanControlT<TArgs>::Region::~Region() noexcept {
	control.resetRegion(prevId, prevIndex, prevSize);

	control._status.clear();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
void
PlanControlT<TArgs>::setRegion(const RegionID regionId,
							   const StateID index,
							   const Long size) noexcept
{
	HFSM2_ASSERT(_regionId <= regionId && regionId <  RegionList::SIZE);
	HFSM2_ASSERT(_regionStateId <= index && index + size <= _regionStateId + _regionSize);

	_regionId	   = regionId;
	_regionStateId = index;
	_regionSize	   = size;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
void
PlanControlT<TArgs>::resetRegion(const RegionID regionId, //-V524
								 const StateID index,
								 const Long size) noexcept
{
	HFSM2_ASSERT(regionId <= _regionId && _regionId < RegionList::SIZE);
	HFSM2_ASSERT(index <= _regionStateId && _regionStateId + _regionSize <= index + size);

	_regionId	   = regionId;
	_regionStateId = index;
	_regionSize	   = size;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FullControlBaseT<TArgs>::Lock::Lock(FullControlBaseT& control_) noexcept
	: control{!control_._locked ? &control_ : nullptr}
{
	if (control)
		control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FullControlBaseT<TArgs>::Lock::~Lock() noexcept {
	if (control)
		control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_PLANS

template <typename TArgs>
template <typename TState>
Status
FullControlBaseT<TArgs>::buildPlanStatus() noexcept {
	using State = TState;
	static constexpr StateID STATE_ID = State::STATE_ID;

	switch (_status.result) {
	case Status::Result::NONE:
		HFSM2_BREAK();
		break;

	case Status::Result::SUCCESS:
		_planData.tasksSuccesses.template set<STATE_ID>();

		HFSM2_LOG_PLAN_STATUS(context(), _regionId, StatusEvent::SUCCEEDED);
		break;

	case Status::Result::FAILURE:
		_planData.tasksFailures .template set<STATE_ID>();

		HFSM2_LOG_PLAN_STATUS(context(), _regionId, StatusEvent::FAILED);
		break;

	default:
		HFSM2_BREAK();
	}

	return {_status.result};
}

#endif

//------------------------------------------------------------------------------
// COMMON

template <typename TArgs>
void
FullControlBaseT<TArgs>::changeTo(const StateID stateId) noexcept {
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::CHANGE});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::CHANGE, stateId);
	}
}

// COMMON
//------------------------------------------------------------------------------

template <typename TArgs>
void
FullControlBaseT<TArgs>::restart(const StateID stateId) noexcept {
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RESTART});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESTART, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
FullControlBaseT<TArgs>::resume(const StateID stateId) noexcept {
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RESUME});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESUME, stateId);
	}
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TArgs>
void
FullControlBaseT<TArgs>::utilize(const StateID stateId) noexcept {
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::UTILIZE});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::UTILIZE, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
FullControlBaseT<TArgs>::randomize(const StateID stateId) noexcept {
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RANDOMIZE});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RANDOMIZE, stateId);
	}
}

#endif

//------------------------------------------------------------------------------

template <typename TArgs>
void
FullControlBaseT<TArgs>::schedule(const StateID stateId) noexcept {
	_requests.append(Transition{_originId, stateId, TransitionType::SCHEDULE});

	HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SCHEDULE, stateId);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_PLANS

template <typename TArgs>
void
FullControlBaseT<TArgs>::succeed() noexcept {
	_status.result = Status::Result::SUCCESS;

	_planData.tasksSuccesses.set(_originId);

	// TODO: promote taskSuccess all the way up for all regions without plans
	if (_regionId < RegionList::SIZE && !_planData.planExists.get(_regionId)) {
		HFSM2_ASSERT(_regionStateId < StateList::SIZE);

		_planData.tasksSuccesses.set(_regionStateId);
	}

	HFSM2_LOG_TASK_STATUS(context(), _regionId, _originId, StatusEvent::SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
void
FullControlBaseT<TArgs>::fail() noexcept {
	_status.result = Status::Result::FAILURE;

	_planData.tasksFailures.set(_originId);

	// TODO: promote taskFailure all the way up for all regions without plans
	if (_regionId < RegionList::SIZE && !_planData.planExists.get(_regionId)) {
		HFSM2_ASSERT(_regionStateId < StateList::SIZE);

		_planData.tasksFailures.set(_regionStateId);
	}

	HFSM2_LOG_TASK_STATUS(context(), _regionId, _originId, StatusEvent::FAILED);
}

#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_PLANS

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL, Long NTC, typename TTP>
template <typename TState>
Status
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL, NTC, TTP>>::updatePlan(TState& headState,
																					 const Status subStatus) noexcept
{
	using State = TState;
	static constexpr StateID STATE_ID = State::STATE_ID;

	HFSM2_ASSERT(subStatus);

	if (subStatus.result == Status::Result::FAILURE) {
		_status.result = Status::Result::FAILURE;
		headState.wrapPlanFailed(*this);

		if (Plan p = plan(_regionId))
			p.clear();

		return FullControlBase::template buildPlanStatus<State>();
	} else if (subStatus.result == Status::Result::SUCCESS) {
		if (Plan p = plan(_regionId)) {
			TasksBits processed;

			for (auto it = p.first(); it; ++it) {
				if (isActive(it->origin) &&
					_planData.tasksSuccesses.get(it->origin) &&
					!processed.get(it->origin))
				{
					Origin origin{*this, STATE_ID};
					changeTo(it->destination);

					_planData.tasksSuccesses.clear(it->origin);
					processed.set(it->origin);
					it.remove();
				}
			}

			return Status{};
		} else {
			_status.result = Status::Result::SUCCESS;
			headState.wrapPlanSucceeded(*this);

			return FullControlBase::template buildPlanStatus<State>();
		}
	} else
		return Status{};
}

#endif

//------------------------------------------------------------------------------
// COMMON

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::changeWith(const StateID stateId,
																									  const Payload& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::CHANGE, payload});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::CHANGE, stateId);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::changeWith(const StateID stateId,
																									  Payload&& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::CHANGE, std::move(payload)});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::CHANGE, stateId);
	}
}

// COMMON
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::restartWith(const StateID stateId,
																									   const Payload& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RESTART, payload});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESTART, stateId);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::restartWith(const StateID stateId,
																									   Payload&& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RESTART, std::move(payload)});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESTART, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::resumeWith(const StateID stateId,
																									  const Payload& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RESUME, payload});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESUME, stateId);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::resumeWith(const StateID stateId,
																									  Payload&& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RESUME, std::move(payload)});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESUME, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::utilizeWith(const StateID stateId,
																									   const Payload& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::UTILIZE, payload});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::UTILIZE, stateId);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::utilizeWith(const StateID stateId,
																									   Payload&& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::UTILIZE, std::move(payload)});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::UTILIZE, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::randomizeWith(const StateID stateId,
																										 const Payload& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RANDOMIZE, payload});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RANDOMIZE, stateId);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::randomizeWith(const StateID stateId,
																										 Payload&& payload) noexcept
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RANDOMIZE, std::move(payload)});

		if (stateId < _regionStateId || _regionStateId + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RANDOMIZE, stateId);
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::scheduleWith(const StateID stateId,
																										const Payload& payload) noexcept
{
	_requests.append(Transition{_originId, stateId, TransitionType::SCHEDULE, payload});

	HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SCHEDULE, stateId);
}

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::scheduleWith(const StateID stateId,
																										Payload&& payload) noexcept
{
	_requests.append(Transition{_originId, stateId, TransitionType::SCHEDULE, std::move(payload)});

	HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SCHEDULE, stateId);
}

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_PLANS

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL, Long NTC>
template <typename TState>
Status
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL, NTC, void>>::updatePlan(TState& headState,
																					  const Status subStatus) noexcept
{
	using State = TState;
	static constexpr StateID STATE_ID = State::STATE_ID;

	HFSM2_ASSERT(subStatus);

	if (subStatus.result == Status::Result::FAILURE) {
		_status.result = Status::Result::FAILURE;
		headState.wrapPlanFailed(*this);

		if (Plan p = plan(_regionId))
			p.clear();

		return FullControlBase::template buildPlanStatus<State>();
	} else if (subStatus.result == Status::Result::SUCCESS) {
		if (Plan p = plan(_regionId)) {
			TasksBits processed;

			for (auto it = p.first(); it; ++it) {
				if (isActive(it->origin) &&
					_planData.tasksSuccesses.get(it->origin) &&
					!processed.get(it->origin))
				{
					Origin origin{*this, STATE_ID};
					changeTo(it->destination);

					_planData.tasksSuccesses.clear(it->origin);
					processed.set(it->origin);
					it.remove();
				}
			}

			return Status{};
		} else {
			_status.result = Status::Result::SUCCESS;
			headState.wrapPlanSucceeded(*this);

			return FullControlBase::template buildPlanStatus<State>();
		}
	} else
		return Status{};
}

#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
void
GuardControlT<TArgs>::cancelPendingTransitions() noexcept {
	_cancelled = true;

	HFSM2_LOG_CANCELLED_PENDING(context(), _originId);
}

////////////////////////////////////////////////////////////////////////////////

}
}
