namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
ControlT<TA>::Origin::Origin(ControlT& control_,
							 const StateID id)
	: control{control_}
	, prevId(control._originId)
{
	control.setOrigin(id);
}

//------------------------------------------------------------------------------

template <typename TA>
ControlT<TA>::Origin::~Origin() {
	control.resetOrigin(prevId);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
ControlT<TA>::Region::Region(ControlT& control_,
							 const RegionID id,
							 const StateID index,
							 const LongIndex size)
	: control{control_}
	, prevId(control._regionId)
	, prevIndex(control._regionIndex)
	, prevSize(control._regionSize)
{
	control.setRegion(id, index, size);
}

//------------------------------------------------------------------------------

template <typename TA>
ControlT<TA>::Region::~Region() {
	control.resetRegion(prevId, prevIndex, prevSize);

	control._status.clear();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
void
ControlT<TA>::setOrigin(const StateID id) {
	//HFSM_ASSERT(_regionId != INVALID_STATE_ID && _regionSize != INVALID_LONG_INDEX);
	//HFSM_ASSERT(_regionId < StateList::SIZE && _regionId + _regionSize <= StateList::SIZE);

	HFSM_ASSERT(id != INVALID_STATE_ID);

	_originId = id;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
ControlT<TA>::resetOrigin(const StateID id) {
	//HFSM_ASSERT(_regionId != INVALID_STATE_ID && _regionSize != INVALID_LONG_INDEX);
	//HFSM_ASSERT(_regionId < StateList::SIZE && _regionId + _regionSize <= StateList::SIZE);

	HFSM_ASSERT(_originId != INVALID_STATE_ID);

	_originId = id;
}

//------------------------------------------------------------------------------

template <typename TA>
void
ControlT<TA>::setRegion(const RegionID id,
						const StateID index,
						const LongIndex size)
{
	HFSM_ASSERT(index != INVALID_STATE_ID && size != INVALID_LONG_INDEX);

	if (_regionId == INVALID_REGION_ID) {
		HFSM_ASSERT(_regionIndex == INVALID_STATE_ID);
		HFSM_ASSERT(_regionSize  == INVALID_LONG_INDEX);
		HFSM_ASSERT(index < StateList::SIZE && index + size <= StateList::SIZE);
	} else {
		HFSM_ASSERT(_regionIndex != INVALID_STATE_ID);
		HFSM_ASSERT(_regionSize  != INVALID_LONG_INDEX);
		HFSM_ASSERT(_regionIndex <= index && index + size <= _regionIndex + _regionSize);
	}

	HFSM_ASSERT(_originId == INVALID_STATE_ID);

	_regionId	 = id;
	_regionIndex = index;
	_regionSize	 = size;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
ControlT<TA>::resetRegion(const RegionID id,
						  const StateID index,
						  const LongIndex size)
{
	HFSM_ASSERT(_regionId	 != INVALID_REGION_ID);
	HFSM_ASSERT(_regionIndex != INVALID_STATE_ID);
	HFSM_ASSERT(_regionSize  != INVALID_LONG_INDEX);

	if (index == INVALID_STATE_ID)
		HFSM_ASSERT(size == INVALID_LONG_INDEX);
	else
		HFSM_ASSERT(size != INVALID_LONG_INDEX);

	_regionId	 = id;
	_regionIndex = index;
	_regionSize	 = size;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
FullControlT<TA>::Lock::Lock(FullControlT& control_)
	: control(!control_._locked ? &control_ : nullptr)
{
	if (control)
		control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TA>
FullControlT<TA>::Lock::~Lock() {
	if (control)
		control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
template <typename TState>
Status
FullControlT<TA>::updatePlan(TState& headState,
							 const Status subStatus)
{
	using State = TState;
	static constexpr StateID STATE_ID = State::STATE_ID;

	HFSM_ASSERT(subStatus);

	if (subStatus.failure) {
		headState.wrapPlanFailed(*this);

		if (_status.failure) {
			_planData.tasksFailures[STATE_ID] = true;

			return {false, _status.failure, subStatus.outerTransition};
		} else
			return {_status.success, false, subStatus.outerTransition};
	} else if (subStatus.success) {
		if (Plan p = plan(_regionId)) {
			for (auto it = p.begin(); it; ++it) {
				if (isActive(it->origin) &&
					_planData.tasksSuccesses[it->origin])
				{
					Origin origin{*this, STATE_ID};

					changeTo(it->destination);

					it.remove();
				} else
					break;
			}

			return {false, false, subStatus.outerTransition};
		} else {
			headState.wrapPlanSucceeded(*this);

			if (_status.success)
				_planData.tasksSuccesses[STATE_ID] = true;

			return {_status.success, false, subStatus.outerTransition};
		}
	} else
		return {false, false, subStatus.outerTransition};
}

//------------------------------------------------------------------------------

template <typename TA>
void
FullControlT<TA>::changeTo(const StateID stateId) {
	if (!_locked) {
		const Request request{Request::Type::RESTART, stateId};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

	#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
		if (_logger)
			_logger->recordTransition(_originId, Transition::RESTART, stateId);
	#endif
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::resume(const StateID stateId) {
	if (!_locked) {
		const Request request{Request::Type::RESUME, stateId};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

	#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
		if (_logger)
			_logger->recordTransition(_originId, Transition::RESUME, stateId);
	#endif
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::schedule(const StateID stateId) {
	const Request transition{Request::Type::SCHEDULE, stateId};
	_requests << transition;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(_originId, Transition::SCHEDULE, stateId);
#endif
}

//------------------------------------------------------------------------------

template <typename TA>
void
FullControlT<TA>::succeed() {
	_status.success = true;

	_planData.tasksSuccesses[_originId] = true;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	if (_logger)
		_logger->recordTaskStatus(_regionId, _originId, StatusEvent::SUCCEEDED);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::fail() {
	_status.failure = true;

	_planData.tasksFailures [_originId] = true;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	if (_logger)
		_logger->recordTaskStatus(_regionId, _originId, StatusEvent::FAILED);
#endif
}

////////////////////////////////////////////////////////////////////////////////

}
}
