namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
M<TC, TMS>::_C<T, TS...>::_C(StateRegistry& stateRegistry,
							 const Parent parent,
							 Parents& stateParents,
							 Parents& forkParents,
							 ForkPointers& forkPointers)
	: _fork(static_cast<Index>(forkPointers << &_fork), parent, forkParents)
	, _state(stateRegistry, parent, stateParents, forkParents, forkPointers)
	, _subStates(stateRegistry, _fork.self, stateParents, forkParents, forkPointers)
{}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepForwardSubstitute(Control& control,
												Context& context)
{
	assert(_fork.requested != INVALID_INDEX);

	if (_fork.requested == _fork.active)
		_subStates.wideForwardSubstitute(_fork.requested, control, context);
	else
		_subStates.wideSubstitute(_fork.requested, control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepSubstitute(Control& control,
										 Context& context)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.requested != INVALID_INDEX);

	if (!_state.deepSubstitute(control, context))
		_subStates.wideSubstitute(_fork.requested, control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepEnterInitial(Context& context) {
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX &&
		   _fork.requested == INVALID_INDEX);

	HSFM_DEBUG_ONLY(_fork.activeType = TypeInfo::get<typename SubStates::Initial::Client>());
	_fork.active = 0;

	_state.deepEnter(context);
	_subStates.wideEnterInitial(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepEnter(Context& context) {
	assert(_fork.active	   == INVALID_INDEX &&
		   _fork.requested != INVALID_INDEX);

	HSFM_DEBUG_ONLY(_fork.activeType = _fork.requestedType);
	_fork.active = _fork.requested;

	HSFM_DEBUG_ONLY(_fork.requestedType.clear());
	_fork.requested = INVALID_INDEX;

	_state.deepEnter(context);
	_subStates.wideEnter(_fork.active, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
bool
M<TC, TMS>::_C<T, TS...>::deepUpdateAndTransition(Control& control,
												  Context& context)
{
	assert(_fork.active != INVALID_INDEX);

	if (_state.deepUpdateAndTransition(control, context)) {
		_subStates.wideUpdate(_fork.active, context);

		return true;
	} else
		return _subStates.wideUpdateAndTransition(_fork.active, control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepUpdate(Context& context) {
	assert(_fork.active != INVALID_INDEX);

	_state.deepUpdate(context);
	_subStates.wideUpdate(_fork.active, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <typename TEvent>
void
M<TC, TMS>::_C<T, TS...>::deepReact(const TEvent& event,
									Control& control,
									Context& context)
{
	assert(_fork.active != INVALID_INDEX);

	_state.deepReact(event, control, context);
	_subStates.wideReact(_fork.active, event, control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepLeave(Context& context) {
	assert(_fork.active != INVALID_INDEX);

	_subStates.wideLeave(_fork.active, context);
	_state.deepLeave(context);

	HSFM_DEBUG_ONLY(_fork.resumableType = _fork.activeType);
	_fork.resumable = _fork.active;

	HSFM_DEBUG_ONLY(_fork.activeType.clear());
	_fork.active = INVALID_INDEX;
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepForwardRequest(const enum Transition::Type transition) {
	if (_fork.requested != INVALID_INDEX)
		_subStates.wideForwardRequest(_fork.requested, transition);
	else
		switch (transition) {
		case Transition::Remain:
			deepRequestRemain();
			break;

		case Transition::Restart:
			deepRequestRestart();
			break;

		case Transition::Resume:
			deepRequestResume();
			break;

		default:
			assert(false);
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepRequestRemain() {
	if (_fork.active == INVALID_INDEX) {
		HSFM_DEBUG_ONLY(_fork.requestedType = TypeInfo::get<typename SubStates::Initial::Client>());
		_fork.requested = 0;
	}

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepRequestRestart() {
	HSFM_DEBUG_ONLY(_fork.requestedType = TypeInfo::get<typename SubStates::Initial::Client>());
	_fork.requested = 0;

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepRequestResume() {
	if (_fork.resumable != INVALID_INDEX) {
		HSFM_DEBUG_ONLY(_fork.requestedType = _fork.resumableType);
		_fork.requested = _fork.resumable;
	} else {
		HSFM_DEBUG_ONLY(_fork.requestedType = TypeInfo::get<typename SubStates::Initial::Client>());
		_fork.requested = 0;
	}

	_subStates.wideRequestResume(_fork.requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepChangeToRequested(Context& context) {
	assert(_fork.active != INVALID_INDEX);

	if (_fork.requested == _fork.active)
		_subStates.wideChangeToRequested(_fork.requested, context);
	else if (_fork.requested != INVALID_INDEX) {
		_subStates.wideLeave(_fork.active, context);

		HSFM_DEBUG_ONLY(_fork.resumableType = _fork.activeType);
		_fork.resumable = _fork.active;

		HSFM_DEBUG_ONLY(_fork.activeType = _fork.requestedType);
		_fork.active = _fork.requested;

		HSFM_DEBUG_ONLY(_fork.requestedType.clear());
		_fork.requested = INVALID_INDEX;

		_subStates.wideEnter(_fork.active, context);
	}
}

//------------------------------------------------------------------------------

#ifdef HFSM_MACHINE_ENABLE_STRUCTURE_REPORT

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepGetNames(const unsigned parent,
									   const enum StateInfo::RegionType region,
									   const unsigned depth,
									   StateInfos& _stateInfos) const
{
	_state.deepGetNames(parent, region, depth, _stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, depth + 1, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepIsActive(const bool isActive,
									   unsigned& index,
									   MachineStructure& structure) const
{
	_state.deepIsActive(isActive, index, structure);
	_subStates.wideIsActive(isActive ? _fork.active : INVALID_INDEX, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
