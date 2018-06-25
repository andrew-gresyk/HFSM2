namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
M<TC, TMS>::_O<TH, TS...>::_O(StateRegistry& stateRegistry,
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
M<TC, TMS>::_O<TH, TS...>::deepForwardSubstitute(Control& control,
												 Context& context,
												 LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	if (_fork.requested != INVALID_INDEX)
		_subStates.wideForwardSubstitute(_fork.requested, control, context, logger);
	else
		_subStates.wideForwardSubstitute(				  control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepSubstitute(Control& control,
										  Context& context,
										  LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	if (!_state	  .deepSubstitute(control, context, logger))
		_subStates.wideSubstitute(control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepEnterInitial(Context& context,
											LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX &&
		   _fork.requested == INVALID_INDEX);

	_state	  .deepEnter	   (context, logger);
	_subStates.wideEnterInitial(context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepEnter(Context& context,
									 LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_state	  .deepEnter(context, logger);
	_subStates.wideEnter(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
bool
M<TC, TMS>::_O<TH, TS...>::deepUpdateAndTransition(Control& control,
												   Context& context,
												   LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	if (_state.deepUpdateAndTransition(control, context, logger)) {
		_subStates.wideUpdate(context, logger);

		return true;
	} else
		return _subStates.wideUpdateAndTransition(control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepUpdate(Context& context,
									  LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_state	  .deepUpdate(context, logger);
	_subStates.wideUpdate(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
template <typename TEvent>
void
M<TC, TMS>::_O<TH, TS...>::deepReact(const TEvent& event,
									 Control& control,
									 Context& context,
									 LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_state	  .deepReact(event, control, context, logger);
	_subStates.wideReact(event, control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepLeave(Context& context,
									 LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_subStates.wideLeave(context, logger);
	_state	  .deepLeave(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepForwardRequest(const enum Transition::Type transition) {
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

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
M<TC, TMS>::_O<TH, TS...>::deepRequestRemain() {
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepRequestRestart() {
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepRequestResume() {
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_subStates.wideRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepChangeToRequested(Context& context,
												 LoggerInterface* const logger)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_subStates.wideChangeToRequested(context, logger);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, unsigned TMS>
template <typename TH, typename... TS>
void
M<TC, TMS>::_O<TH, TS...>::deepGetNames(const unsigned parent,
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
M<TC, TMS>::_O<TH, TS...>::deepIsActive(const bool isActive,
										unsigned& index,
										MachineStructure& structure) const
{
	_state.deepIsActive(isActive, index, structure);
	_subStates.wideIsActive(isActive, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
