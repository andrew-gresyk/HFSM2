namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
ControlT<TArgs>::Region::Region(ControlT& control_,
								const RegionID id)
	: control{control_}
	, prevId{control._regionId}
{
	control.setRegion(id);
}

//------------------------------------------------------------------------------

template <typename TArgs>
ControlT<TArgs>::Region::~Region() {
	control.resetRegion(prevId);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
void
ControlT<TArgs>::setRegion(const RegionID id) {
	HFSM2_ASSERT(_regionId <= id && id < RegionList::SIZE);

	_regionId = id;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
ControlT<TArgs>::resetRegion(const RegionID id) { //-V524
	HFSM2_ASSERT(id <= _regionId && _regionId < RegionList::SIZE);

	_regionId = id;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
PlanControlT<TArgs>::Origin::Origin(PlanControlT& control_,
									const StateID id)
	: control{control_}
	, prevId{control._originId}
{
	control.setOrigin(id);
}

//------------------------------------------------------------------------------

template <typename TArgs>
PlanControlT<TArgs>::Origin::~Origin() {
	control.resetOrigin(prevId);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
PlanControlT<TArgs>::Region::Region(PlanControlT& control_,
									const RegionID id,
									const StateID index,
									const LongIndex size)
	: control{control_}
	, prevId{control._regionId}
	, prevIndex{control._regionIndex}
	, prevSize{control._regionSize}
{
	control.setRegion(id, index, size);
}

//------------------------------------------------------------------------------

template <typename TArgs>
PlanControlT<TArgs>::Region::~Region() {
	control.resetRegion(prevId, prevIndex, prevSize);

	control._status.clear();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
void
PlanControlT<TArgs>::setOrigin(const StateID id) {
	HFSM2_ASSERT(_regionId + _regionSize <= StateList::SIZE);
	HFSM2_ASSERT(_originId <= id && id < StateList::SIZE);

	_originId = id;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
void
PlanControlT<TArgs>::resetOrigin(const StateID id) { //-V524
	HFSM2_ASSERT(_regionId + _regionSize <= StateList::SIZE);
	HFSM2_ASSERT(id <= _originId && _originId < StateList::SIZE);

	_originId = id;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanControlT<TArgs>::setRegion(const RegionID id,
							   const StateID index,
							   const LongIndex size)
{
	HFSM2_ASSERT(_regionId <= id && id <  RegionList::SIZE);
	HFSM2_ASSERT(_regionIndex <= index && index + size <= _regionIndex + _regionSize);

	_regionId	 = id;
	_regionIndex = index;
	_regionSize	 = size;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
void
PlanControlT<TArgs>::resetRegion(const RegionID id, //-V524
								 const StateID index,
								 const LongIndex size)
{
	HFSM2_ASSERT(id <= _regionId && _regionId < RegionList::SIZE);
	HFSM2_ASSERT(index <= _regionIndex && _regionIndex + _regionSize <= index + size);

	_regionId	 = id;
	_regionIndex = index;
	_regionSize	 = size;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FullControlBaseT<TArgs>::Lock::Lock(FullControlBaseT& control_)
	: control{!control_._locked ? &control_ : nullptr}
{
	if (control)
		control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FullControlBaseT<TArgs>::Lock::~Lock() {
	if (control)
		control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_PLANS

template <typename TArgs>
template <typename TState>
Status
FullControlBaseT<TArgs>::buildPlanStatus() {
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

template <typename TArgs>
void
FullControlBaseT<TArgs>::changeTo(const StateID stateId) {
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::CHANGE});

		if (stateId < _regionIndex || _regionIndex + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::CHANGE, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
FullControlBaseT<TArgs>::restart(const StateID stateId) {
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RESTART});

		if (stateId < _regionIndex || _regionIndex + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESTART, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
FullControlBaseT<TArgs>::resume(const StateID stateId) {
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RESUME});

		if (stateId < _regionIndex || _regionIndex + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESUME, stateId);
	}
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TArgs>
void
FullControlBaseT<TArgs>::utilize(const StateID stateId) {
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::UTILIZE});

		if (stateId < _regionIndex || _regionIndex + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::UTILIZE, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
FullControlBaseT<TArgs>::randomize(const StateID stateId) {
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RANDOMIZE});

		if (stateId < _regionIndex || _regionIndex + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RANDOMIZE, stateId);
	}
}

#endif

//------------------------------------------------------------------------------

template <typename TArgs>
void
FullControlBaseT<TArgs>::schedule(const StateID stateId) {
	_requests.append(Transition{_originId, stateId, TransitionType::SCHEDULE});

	HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SCHEDULE, stateId);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_PLANS

template <typename TArgs>
void
FullControlBaseT<TArgs>::succeed() {
	_status.result = Status::Result::SUCCESS;

	_planData.tasksSuccesses.set(_originId);

	// TODO: promote taskSuccess all the way up for all regions without plans
	if (_regionId < RegionList::SIZE && !_planData.planExists.get(_regionId)) {
		HFSM2_ASSERT(_regionIndex < StateList::SIZE);

		_planData.tasksSuccesses.set(_regionIndex);
	}

	HFSM2_LOG_TASK_STATUS(context(), _regionId, _originId, StatusEvent::SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
void
FullControlBaseT<TArgs>::fail() {
	_status.result = Status::Result::FAILURE;

	_planData.tasksFailures.set(_originId);

	// TODO: promote taskFailure all the way up for all regions without plans
	if (_regionId < RegionList::SIZE && !_planData.planExists.get(_regionId)) {
		HFSM2_ASSERT(_regionIndex < StateList::SIZE);

		_planData.tasksFailures.set(_regionIndex);
	}

	HFSM2_LOG_TASK_STATUS(context(), _regionId, _originId, StatusEvent::FAILED);
}

#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_PLANS

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NSL HFSM2_IF_PLANS(, LongIndex NTC), typename TTP>
template <typename TState>
Status
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::updatePlan(TState& headState,
																									  const Status subStatus)
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

					_planData.tasksSuccesses.reset(it->origin);
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

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NSL HFSM2_IF_PLANS(, LongIndex NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::changeWith(const StateID stateId,
																									  const Payload& payload)
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::CHANGE, payload});

		if (stateId < _regionIndex || _regionIndex + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::CHANGE, stateId);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NSL HFSM2_IF_PLANS(, LongIndex NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::changeWith(const StateID stateId,
																									  Payload&& payload)
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::CHANGE, std::move(payload)});

		if (stateId < _regionIndex || _regionIndex + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::CHANGE, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NSL HFSM2_IF_PLANS(, LongIndex NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::restartWith(const StateID stateId,
																									   const Payload& payload)
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RESTART, payload});

		if (stateId < _regionIndex || _regionIndex + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESTART, stateId);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NSL HFSM2_IF_PLANS(, LongIndex NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::restartWith(const StateID stateId,
																									   Payload&& payload)
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RESTART, std::move(payload)});

		if (stateId < _regionIndex || _regionIndex + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESTART, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NSL HFSM2_IF_PLANS(, LongIndex NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::resumeWith(const StateID stateId,
																									  const Payload& payload)
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RESUME, payload});

		if (stateId < _regionIndex || _regionIndex + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESUME, stateId);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NSL HFSM2_IF_PLANS(, LongIndex NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::resumeWith(const StateID stateId,
																									  Payload&& payload)
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RESUME, std::move(payload)});

		if (stateId < _regionIndex || _regionIndex + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESUME, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NSL HFSM2_IF_PLANS(, LongIndex NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::utilizeWith(const StateID stateId,
																									   const Payload& payload)
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::UTILIZE, payload});

		if (stateId < _regionIndex || _regionIndex + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::UTILIZE, stateId);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NSL HFSM2_IF_PLANS(, LongIndex NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::utilizeWith(const StateID stateId,
																									   Payload&& payload)
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::UTILIZE, std::move(payload)});

		if (stateId < _regionIndex || _regionIndex + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::UTILIZE, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NSL HFSM2_IF_PLANS(, LongIndex NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::randomizeWith(const StateID stateId,
																										 const Payload& payload)
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RANDOMIZE, payload});

		if (stateId < _regionIndex || _regionIndex + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RANDOMIZE, stateId);
	}
}

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NSL HFSM2_IF_PLANS(, LongIndex NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::randomizeWith(const StateID stateId,
																										 Payload&& payload)
{
	if (!_locked) {
		_requests.append(Transition{_originId, stateId, TransitionType::RANDOMIZE, std::move(payload)});

		if (stateId < _regionIndex || _regionIndex + _regionSize <= stateId)
			_status.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RANDOMIZE, stateId);
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NSL HFSM2_IF_PLANS(, LongIndex NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::scheduleWith(const StateID stateId,
																										const Payload& payload)
{
	_requests.append(Transition{_originId, stateId, TransitionType::SCHEDULE, payload});

	HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SCHEDULE, stateId);
}

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NSL HFSM2_IF_PLANS(, LongIndex NTC), typename TTP>
void
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::scheduleWith(const StateID stateId,
																										Payload&& payload)
{
	_requests.append(Transition{_originId, stateId, TransitionType::SCHEDULE, std::move(payload)});

	HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SCHEDULE, stateId);
}

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_PLANS

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NSL HFSM2_IF_PLANS(, LongIndex NTC)>
template <typename TState>
Status
FullControlT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), void>>::updatePlan(TState& headState,
																									   const Status subStatus)
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

					_planData.tasksSuccesses.reset(it->origin);
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
GuardControlT<TArgs>::cancelPendingTransitions() {
	_cancelled = true;

	HFSM2_LOG_CANCELLED_PENDING(context(), _originId);
}

////////////////////////////////////////////////////////////////////////////////

}
}
