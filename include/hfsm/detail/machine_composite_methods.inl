namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
M<TC, TMS>::_C<TH, TS...>::_C(StateRegistry& stateRegistry,
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
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepForwardSubstitute(Control& control,
												 Context& context,
												 LoggerInterface* const logger)
{
	assert(_fork.requested != INVALID_INDEX);

	if (_fork.requested == _fork.active)
		_subStates.wideForwardSubstitute(_fork.requested, control, context, logger);
	else
		_subStates.wideSubstitute		(_fork.requested, control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepSubstitute(Control& control,
										  Context& context,
										  LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.requested != INVALID_INDEX);

	if (!_state	  .deepSubstitute(				   control, context, logger))
		_subStates.wideSubstitute(_fork.requested, control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepEnterInitial(Context& context,
											LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX &&
		   _fork.requested == INVALID_INDEX);

	HSFM_IF_DEBUG(_fork.activeType = TypeInfo::get<typename SubStates::Initial::Head>());
	_fork.active = 0;

	_state	  .deepEnter	   (context, logger);
	_subStates.wideEnterInitial(context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepEnter(Context& context,
									 LoggerInterface* const logger)
{
	assert(_fork.active	   == INVALID_INDEX &&
		   _fork.requested != INVALID_INDEX);

	HSFM_IF_DEBUG(_fork.activeType = _fork.requestedType);
	_fork.active = _fork.requested;

	HSFM_IF_DEBUG(_fork.requestedType.clear());
	_fork.requested = INVALID_INDEX;

	_state	  .deepEnter(			   context, logger);
	_subStates.wideEnter(_fork.active, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
bool
M<TC, TMS>::_C<TH, TS...>::deepUpdateAndTransition(Control& control,
												   Context& context,
												   LoggerInterface* const logger)
{
	assert(_fork.active != INVALID_INDEX);

	if (_state.deepUpdateAndTransition(control, context, logger)) {
		_subStates.wideUpdate(_fork.active, context, logger);

		return true;
	} else
		return _subStates.wideUpdateAndTransition(_fork.active, control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepUpdate(Context& context,
									  LoggerInterface* const logger)
{
	assert(_fork.active != INVALID_INDEX);

	_state	  .deepUpdate(				context, logger);
	_subStates.wideUpdate(_fork.active, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
template <typename TEvent>
void
M<TC, TMS>::_C<TH, TS...>::deepReact(const TEvent& event,
									 Control& control,
									 Context& context,
									 LoggerInterface* const logger)
{
	assert(_fork.active != INVALID_INDEX);

	_state	  .deepReact(			   event, control, context, logger);
	_subStates.wideReact(_fork.active, event, control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepLeave(Context& context,
									 LoggerInterface* const logger)
{
	assert(_fork.active != INVALID_INDEX);

	_subStates.wideLeave(_fork.active, context, logger);
	_state	  .deepLeave(			   context, logger);

	HSFM_IF_DEBUG(_fork.resumableType = _fork.activeType);
	_fork.resumable = _fork.active;

	HSFM_IF_DEBUG(_fork.activeType.clear());
	_fork.active = INVALID_INDEX;
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepForwardRequest(const enum Transition::Type transition) {
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
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepRequestRemain() {
	if (_fork.active == INVALID_INDEX) {
		HSFM_IF_DEBUG(_fork.requestedType = TypeInfo::get<typename SubStates::Initial::Head>());
		_fork.requested = 0;
	}

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepRequestRestart() {
	HSFM_IF_DEBUG(_fork.requestedType = TypeInfo::get<typename SubStates::Initial::Head>());
	_fork.requested = 0;

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepRequestResume() {
	if (_fork.resumable != INVALID_INDEX) {
		HSFM_IF_DEBUG(_fork.requestedType = _fork.resumableType);
		_fork.requested = _fork.resumable;
	} else {
		HSFM_IF_DEBUG(_fork.requestedType = TypeInfo::get<typename SubStates::Initial::Head>());
		_fork.requested = 0;
	}

	_subStates.wideRequestResume(_fork.requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepChangeToRequested(Context& context,
												 LoggerInterface* const logger)
{
	assert(_fork.active != INVALID_INDEX);

	if (_fork.requested == _fork.active)
		_subStates.wideChangeToRequested(_fork.requested, context, logger);
	else if (_fork.requested != INVALID_INDEX) {
		_subStates.wideLeave(_fork.active, context, logger);

		HSFM_IF_DEBUG(_fork.resumableType = _fork.activeType);
		_fork.resumable = _fork.active;

		HSFM_IF_DEBUG(_fork.activeType = _fork.requestedType);
		_fork.active = _fork.requested;

		HSFM_IF_DEBUG(_fork.requestedType.clear());
		_fork.requested = INVALID_INDEX;

		_subStates.wideEnter(_fork.active, context, logger);
	}
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepGetNames(const unsigned parent,
										const enum StateInfo::RegionType region,
										const unsigned depth,
										StateInfos& _stateInfos) const
{
	_state.deepGetNames(parent, region, depth, _stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, depth + 1, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_C<TH, TS...>::deepIsActive(const bool isActive,
										unsigned& index,
										MachineStructure& structure) const
{
	_state.deepIsActive(isActive, index, structure);
	_subStates.wideIsActive(isActive ? _fork.active : INVALID_INDEX, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
