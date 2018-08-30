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

//------------------------------------------------------------------------------

template <typename TA>
void
ControlT<TA>::setRegion(const RegionID id,
						const StateID index,
						const LongIndex size)
{
	assert(index != INVALID_STATE_ID && size != INVALID_LONG_INDEX);

	if (_regionId == INVALID_REGION_ID) {
		assert(_regionIndex == INVALID_STATE_ID);
		assert(_regionSize  == INVALID_LONG_INDEX);
		assert(index < StateList::SIZE && index + size <= StateList::SIZE);
	} else {
		assert(_regionIndex != INVALID_STATE_ID);
		assert(_regionSize  != INVALID_LONG_INDEX);
		assert(_regionIndex < index && index + size <= _regionIndex + _regionSize);
	}

	assert(_originId == INVALID_STATE_ID);

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
	assert(_regionId	!= INVALID_REGION_ID);
	assert(_regionIndex != INVALID_STATE_ID);
	assert(_regionSize  != INVALID_LONG_INDEX);

	if (index == INVALID_STATE_ID)
		assert(size == INVALID_LONG_INDEX);
	else
		assert(size != INVALID_LONG_INDEX);

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
void
FullControlT<TA>::changeTo(const StateID stateId) {
	if (!_locked) {
		const Request request{Request::Type::RESTART, stateId};
		_requests << request;

		if (_regionIndex <= stateId && stateId < _regionIndex + _regionSize)
			_status.innerTransition = true;
		else
			_status.outerTransition = true;

	#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
		if (Control::_logger)
			Control::_logger->recordTransition(Control::_originId, Transition::RESTART, stateId);
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

		if (_regionIndex <= stateId && stateId < _regionIndex + _regionSize)
			_status.innerTransition = true;
		else
			_status.outerTransition = true;

	#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
		if (Control::_logger)
			Control::_logger->recordTransition(Control::_originId, Transition::RESUME, stateId);
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
	if (Control::_logger)
		Control::_logger->recordTransition(Control::_originId, Transition::SCHEDULE, stateId);
#endif
}

////////////////////////////////////////////////////////////////////////////////

}
}
