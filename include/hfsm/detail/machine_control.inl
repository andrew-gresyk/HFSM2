namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
void
ControlT<TA>::setOrigin(const StateID id) {
	//assert(_regionId != INVALID_STATE_ID && _regionSize != INVALID_LONG_INDEX);
	//assert(_regionId < StateList::SIZE && _regionId + _regionSize <= StateList::SIZE);

	assert(id != INVALID_STATE_ID);

	_originId = id;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
ControlT<TA>::resetOrigin(const StateID id) {
	//assert(_regionId != INVALID_STATE_ID && _regionSize != INVALID_LONG_INDEX);
	//assert(_regionId < StateList::SIZE && _regionId + _regionSize <= StateList::SIZE);

	assert(_originId != INVALID_STATE_ID);

	_originId = id;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
ControlOriginT<TA>::ControlOriginT(Control& control_,
								   const StateID id)
	: control{control_}
	, prevId(control._originId)
{
	control.setOrigin(id);
}

//------------------------------------------------------------------------------

template <typename TA>
ControlOriginT<TA>::~ControlOriginT() {
	control.resetOrigin(prevId);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
void
TransitionControlT<TA>::changeTo(const StateID stateId) {
	if (!_locked) {
		const Transition transition{TransitionType::RESTART, stateId};
		_requests << transition;

		if (_regionId <= stateId && stateId < _regionId + _regionSize)
			_status.innerTransition = true;
		else
			_status.outerTransition = true;

	#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
		if (Control::_logger)
			Control::_logger->recordTransition(Control::_originId, ::hfsm::Transition::RESTART, stateId);
	#endif
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
TransitionControlT<TA>::resume(const StateID stateId) {
	if (!_locked) {
		const Transition transition{TransitionType::RESUME, stateId};
		_requests << transition;

		if (_regionId <= stateId && stateId < _regionId + _regionSize)
			_status.innerTransition = true;
		else
			_status.outerTransition = true;

	#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
		if (Control::_logger)
			Control::_logger->recordTransition(Control::_originId, ::hfsm::Transition::RESUME, stateId);
	#endif
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
TransitionControlT<TA>::schedule(const StateID stateId) {
	const Transition transition{TransitionType::SCHEDULE, stateId};
	_requests << transition;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	if (Control::_logger)
		Control::_logger->recordTransition(Control::_originId, ::hfsm::Transition::SCHEDULE, stateId);
#endif
}

//------------------------------------------------------------------------------

template <typename TA>
void
TransitionControlT<TA>::setRegion(const StateID id,
								  const LongIndex size)
{
	if (_regionId == INVALID_STATE_ID) {
		assert(_regionSize == INVALID_LONG_INDEX);
		assert(id < StateList::SIZE && id + size <= StateList::SIZE);
	} else {
		assert(_regionSize != INVALID_LONG_INDEX);
		assert(_regionId < id && id + size <= _regionId + _regionSize);
	}

	assert(id != INVALID_STATE_ID && size != INVALID_LONG_INDEX);

	assert(Control::_originId == INVALID_STATE_ID);

	_regionId	= id;
	_regionSize	= size;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
TransitionControlT<TA>::resetRegion(const StateID id,
									const LongIndex size)
{
	assert(_regionId != INVALID_STATE_ID && _regionSize != INVALID_LONG_INDEX);

	if (id == INVALID_STATE_ID)
		assert(size == INVALID_LONG_INDEX);
	else
		assert(size != INVALID_LONG_INDEX);

	_regionId	= id;
	_regionSize	= size;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
ControlLockT<TA>::ControlLockT(TransitionControl& control_)
	: control(!control_._locked ? &control_ : nullptr)
{
	if (control)
		control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TA>
ControlLockT<TA>::~ControlLockT() {
	if (control)
		control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
ControlRegionT<TA>::ControlRegionT(TransitionControl& control_,
								   const StateID id,
								   const LongIndex size)
	: control{control_}
	, prevId(control._regionId)
	, prevSize(control._regionSize)
{
	control.setRegion(id, size);
}

//------------------------------------------------------------------------------

template <typename TA>
ControlRegionT<TA>::~ControlRegionT() {
	control.resetRegion(prevId, prevSize);

	control._status.clear();
}

////////////////////////////////////////////////////////////////////////////////

}
}
