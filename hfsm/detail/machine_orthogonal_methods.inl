namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
M<TC, TMS>::_O<T, TS...>::_O(StateRegistry& stateRegistry,
							 const Parent parent,
							 Parents& stateParents,
							 Parents& forkParents,
							 ForkPointers& forkPointers)
	: ForkT<T>(static_cast<Index>(forkPointers << this), parent, forkParents)
	, _state(stateRegistry, parent, stateParents, forkParents, forkPointers)
	, _subStates(stateRegistry, Fork::self, stateParents, forkParents, forkPointers)
{}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepForwardSubstitute(Control& control,
												Context& context)
{
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	if (Fork::requested != INVALID_INDEX)
		_subStates.wideForwardSubstitute(Fork::requested, control, context);
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
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	if (!_state.deepSubstitute(control, context))
		_subStates.wideSubstitute(control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepEnterInitial(Context& context) {
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX &&
		   Fork::requested == INVALID_INDEX);

	_state.deepEnter(context);
	_subStates.wideEnterInitial(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepEnter(Context& context) {
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

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
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

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
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

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
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	_state.deepReact(event, control, context);
	_subStates.wideReact(event, control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepLeave(Context& context) {
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	_subStates.wideLeave(context);
	_state.deepLeave(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepForwardRequest(const enum Transition::Type transition) {
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	if (Fork::requested != INVALID_INDEX)
		_subStates.wideForwardRequest(Fork::requested, transition);
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
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepRequestRestart() {
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepRequestResume() {
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	_subStates.wideRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepChangeToRequested(Context& context) {
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	_subStates.wideChangeToRequested(context);
}

////////////////////////////////////////////////////////////////////////////////

}
