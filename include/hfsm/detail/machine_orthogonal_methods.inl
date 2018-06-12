namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
M<TC, TMS>::_O<T, TS...>::_O(StateRegistry& stateRegistry,
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
M<TC, TMS>::_O<T, TS...>::deepForwardSubstitute(Control& control,
												Context& context)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	if (_fork.requested != INVALID_INDEX)
		_subStates.wideForwardSubstitute(_fork.requested, control, context);
	else
		_subStates.wideForwardSubstitute(control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepSubstitute(Control& control,
										 Context& context)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	if (!_state.deepSubstitute(control, context))
		_subStates.wideSubstitute(control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepEnterInitial(Context& context) {
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX &&
		   _fork.requested == INVALID_INDEX);

	_state.deepEnter(context);
	_subStates.wideEnterInitial(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepEnter(Context& context) {
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_state.deepEnter(context);
	_subStates.wideEnter(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
bool
M<TC, TMS>::_O<T, TS...>::deepUpdateAndTransition(Control& control,
												  Context& context)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	if (_state.deepUpdateAndTransition(control, context)) {
		_subStates.wideUpdate(context);

		return true;
	} else
		return _subStates.wideUpdateAndTransition(control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepUpdate(Context& context) {
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_state.deepUpdate(context);
	_subStates.wideUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <typename TEvent>
void
M<TC, TMS>::_O<T, TS...>::deepReact(const TEvent& event,
									Control& control,
									Context& context)
{
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_state.deepReact(event, control, context);
	_subStates.wideReact(event, control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepLeave(Context& context) {
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_subStates.wideLeave(context);
	_state.deepLeave(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepForwardRequest(const enum Transition::Type transition) {
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
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepRequestRemain() {
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepRequestRestart() {
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepRequestResume() {
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_subStates.wideRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepChangeToRequested(Context& context) {
	assert(_fork.active    == INVALID_INDEX &&
		   _fork.resumable == INVALID_INDEX);

	_subStates.wideChangeToRequested(context);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepGetNames(const unsigned parent,
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
M<TC, TMS>::_O<T, TS...>::deepIsActive(const bool isActive,
									   unsigned& index,
									   MachineStructure& structure) const
{
	_state.deepIsActive(isActive, index, structure);
	_subStates.wideIsActive(isActive, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
