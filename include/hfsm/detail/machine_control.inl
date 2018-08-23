namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TSL, LongIndex NFC>
void
ControlT<TC, TSL, NFC>::setOrigin(const StateID id) {
	//assert(_regionId != INVALID_STATE_ID && _regionSize != INVALID_LONG_INDEX);
	//assert(_regionId < StateList::SIZE && _regionId + _regionSize <= StateList::SIZE);

	assert(id != INVALID_STATE_ID);

	_originId = id;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TSL, LongIndex NFC>
void
ControlT<TC, TSL, NFC>::resetOrigin(const StateID id) {
	//assert(_regionId != INVALID_STATE_ID && _regionSize != INVALID_LONG_INDEX);
	//assert(_regionId < StateList::SIZE && _regionId + _regionSize <= StateList::SIZE);

	assert(_originId != INVALID_STATE_ID);

	_originId = id;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TSL, LongIndex NFC>
ControlOriginT<TC, TSL, NFC>::ControlOriginT(Control& control_,
											 const StateID id)
	: control{control_}
	, prevId(control._originId)
{
	control.setOrigin(id);
}

//------------------------------------------------------------------------------

template <typename TC, typename TSL, LongIndex NFC>
ControlOriginT<TC, TSL, NFC>::~ControlOriginT() {
	control.resetOrigin(prevId);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TSL, LongIndex NFC, typename TPL>
void
TransitionControlT<TC, TSL, NFC, TPL>::changeTo(const StateID stateId) {
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

template <typename TC, typename TSL, LongIndex NFC, typename TPL>
void
TransitionControlT<TC, TSL, NFC, TPL>::resume(const StateID stateId) {
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

template <typename TC, typename TSL, LongIndex NFC, typename TPL>
void
TransitionControlT<TC, TSL, NFC, TPL>::schedule(const StateID stateId) {
	const Transition transition{TransitionType::SCHEDULE, stateId};
	_requests << transition;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	if (Control::_logger)
		Control::_logger->recordTransition(Control::_originId, ::hfsm::Transition::SCHEDULE, stateId);
#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TSL, LongIndex NFC, typename TPL>
void
TransitionControlT<TC, TSL, NFC, TPL>::setRegion(const StateID id,
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

template <typename TC, typename TSL, LongIndex NFC, typename TPL>
void
TransitionControlT<TC, TSL, NFC, TPL>::resetRegion(const StateID id,
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

template <typename TC, typename TSL, LongIndex NFC, typename TPL>
ControlLockT<TC, TSL, NFC, TPL>::ControlLockT(TransitionControl& control_)
	: control(!control_._locked ? &control_ : nullptr)
{
	if (control)
		control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TC, typename TSL, LongIndex NFC, typename TPL>
ControlLockT<TC, TSL, NFC, TPL>::~ControlLockT() {
	if (control)
		control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TSL, LongIndex NFC, typename TPL>
ControlRegionT<TC, TSL, NFC, TPL>::ControlRegionT(TransitionControl& control_,
												  const StateID id,
												  const LongIndex size)
	: control{control_}
	, prevId(control._regionId)
	, prevSize(control._regionSize)
{
	control.setRegion(id, size);
}

//------------------------------------------------------------------------------

template <typename TC, typename TSL, LongIndex NFC, typename TPL>
ControlRegionT<TC, TSL, NFC, TPL>::~ControlRegionT() {
	control.resetRegion(prevId, prevSize);

	control._status.clear();
}

////////////////////////////////////////////////////////////////////////////////

}
}
