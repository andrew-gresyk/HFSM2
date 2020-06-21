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
	HFSM_ASSERT(_regionId <= id && id < RegionList::SIZE);

	_regionId = id;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
ControlT<TArgs>::resetRegion(const RegionID id) { //-V524
	HFSM_ASSERT(id <= _regionId && _regionId < RegionList::SIZE);

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
	HFSM_ASSERT(_regionId + _regionSize <= StateList::SIZE);
	HFSM_ASSERT(_originId <= id && id < StateList::SIZE);

	_originId = id;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
void
PlanControlT<TArgs>::resetOrigin(const StateID id) { //-V524
	HFSM_ASSERT(_regionId + _regionSize <= StateList::SIZE);
	HFSM_ASSERT(id <= _originId && _originId < StateList::SIZE);

	_originId = id;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanControlT<TArgs>::setRegion(const RegionID id,
							   const StateID index,
							   const LongIndex size)
{
	HFSM_ASSERT(_regionId <= id && id <  RegionList::SIZE);
	HFSM_ASSERT(_regionIndex <= index && index + size <= _regionIndex + _regionSize);

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
	HFSM_ASSERT(id <= _regionId && _regionId < RegionList::SIZE);
	HFSM_ASSERT(index <= _regionIndex && _regionIndex + _regionSize <= index + size);

	_regionId	 = id;
	_regionIndex = index;
	_regionSize	 = size;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
FullControlT<TArgs>::Lock::Lock(FullControlT& control_)
	: control{!control_._locked ? &control_ : nullptr}
{
	if (control)
		control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TArgs>
FullControlT<TArgs>::Lock::~Lock() {
	if (control)
		control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
template <typename TState>
Status
FullControlT<TArgs>::updatePlan(TState& headState,
								const Status subStatus)
{
	using State = TState;
	static constexpr StateID STATE_ID = State::STATE_ID;

	HFSM_ASSERT(subStatus);

	if (subStatus.result == Status::FAILURE) {
		_status.result = Status::FAILURE;
		headState.wrapPlanFailed(*this);

		if (Plan p = plan(_regionId))
			p.clear();

		return buildPlanStatus<State>();
	} else if (subStatus.result == Status::SUCCESS) {
		if (Plan p = plan(_regionId)) {
			for (auto it = p.first(); it; ++it) {
				if (isActive(it->origin) &&
					_planData.tasksSuccesses.get(it->origin))
				{
					Origin origin{*this, STATE_ID};

					changeTo(it->destination);

					it.remove();
				} else
					break;
			}

			return Status{};
		} else {
			_status.result = Status::SUCCESS;
			headState.wrapPlanSucceeded(*this);

			return buildPlanStatus<State>();
		}
	} else
		return Status{};
}

//------------------------------------------------------------------------------

template <typename TArgs>
template <typename TState>
Status
FullControlT<TArgs>::buildPlanStatus() {
	using State = TState;
	static constexpr StateID STATE_ID = State::STATE_ID;

	switch (_status.result) {
	case Status::NONE:
		HFSM_BREAK();
		break;

	case Status::SUCCESS:
		_planData.tasksSuccesses.template set<STATE_ID>();

		HFSM_LOG_PLAN_STATUS(context(), _regionId, StatusEvent::SUCCEEDED);
		break;

	case Status::FAILURE:
		_planData.tasksFailures.template set<STATE_ID>();

		HFSM_LOG_PLAN_STATUS(context(), _regionId, StatusEvent::FAILED);
		break;

	default:
		HFSM_BREAK();
	}

	return {_status.result};
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
FullControlT<TArgs>::changeTo(const StateID stateId) {
	if (!_locked) {
		_requests.append(Request{Request::Type::CHANGE, stateId});

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(context(), _originId, TransitionType::CHANGE, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
FullControlT<TArgs>::restart(const StateID stateId) {
	if (!_locked) {
		_requests.append(Request{Request::Type::RESTART, stateId});

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(context(), _originId, TransitionType::RESTART, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
FullControlT<TArgs>::resume(const StateID stateId) {
	if (!_locked) {
		_requests.append(Request{Request::Type::RESUME, stateId});

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(context(), _originId, TransitionType::RESUME, stateId);
	}
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_UTILITY_THEORY

template <typename TArgs>
void
FullControlT<TArgs>::utilize(const StateID stateId) {
	if (!_locked) {
		_requests.append(Request{Request::Type::UTILIZE, stateId});

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(context(), _originId, TransitionType::UTILIZE, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
FullControlT<TArgs>::randomize(const StateID stateId) {
	if (!_locked) {
		_requests.append(Request{Request::Type::RANDOMIZE, stateId});

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(context(), _originId, TransitionType::RANDOMIZE, stateId);
	}
}

#endif

//------------------------------------------------------------------------------

template <typename TArgs>
void
FullControlT<TArgs>::schedule(const StateID stateId) {
	_requests.append(Request{Request::Type::SCHEDULE, stateId});

	HFSM_LOG_TRANSITION(context(), _originId, TransitionType::SCHEDULE, stateId);
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
FullControlT<TArgs>::succeed() {
	_status.result = Status::SUCCESS;

	_planData.tasksSuccesses.set(_originId);

	// TODO: promote taskSuccess all the way up for all regions without plans
	if (_regionId < RegionList::SIZE && !_planData.planExists.get(_regionId)) {
		HFSM_ASSERT(_regionIndex < StateList::SIZE);

		_planData.tasksSuccesses.set(_regionIndex);
	}

	HFSM_LOG_TASK_STATUS(context(), _regionId, _originId, StatusEvent::SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
void
FullControlT<TArgs>::fail() {
	_status.result = Status::FAILURE;

	_planData.tasksFailures.set(_originId);

	// TODO: promote taskFailure all the way up for all regions without plans
	if (_regionId < RegionList::SIZE && !_planData.planExists.get(_regionId)) {
		HFSM_ASSERT(_regionIndex < StateList::SIZE);

		_planData.tasksFailures.set(_regionIndex);
	}

	HFSM_LOG_TASK_STATUS(context(), _regionId, _originId, StatusEvent::FAILED);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
void
GuardControlT<TArgs>::cancelPendingTransitions() {
	_cancelled = true;

	HFSM_LOG_CANCELLED_PENDING(context(), _originId);
}

////////////////////////////////////////////////////////////////////////////////

}
}
