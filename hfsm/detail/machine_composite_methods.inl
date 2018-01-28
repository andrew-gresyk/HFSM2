namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
M<TC, TMS>::_C<T, TS...>::_C(StateRegistry& stateRegistry,
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
M<TC, TMS>::_C<T, TS...>::deepForwardSubstitute(Control& control,
												Context& context)
{
	assert(Fork::requested != INVALID_INDEX);

	if (Fork::requested == Fork::active)
		_subStates.wideForwardSubstitute(Fork::requested, control, context);
	else
		_subStates.wideSubstitute(Fork::requested, control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepSubstitute(Control& control,
										 Context& context)
{
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::requested != INVALID_INDEX);

	if (!_state.deepSubstitute(control, context))
		_subStates.wideSubstitute(Fork::requested, control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepEnterInitial(Context& context) {
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX &&
		   Fork::requested == INVALID_INDEX);

	HSFM_DEBUG_ONLY(Fork::activeType = TypeInfo::get<typename SubStates::Initial::Client>());
	Fork::active = 0;

	_state.deepEnter(context);
	_subStates.wideEnterInitial(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepEnter(Context& context) {
	assert(Fork::active	   == INVALID_INDEX &&
		   Fork::requested != INVALID_INDEX);

	HSFM_DEBUG_ONLY(Fork::activeType = Fork::requestedType);
	Fork::active = Fork::requested;

	HSFM_DEBUG_ONLY(Fork::requestedType.clear());
	Fork::requested = INVALID_INDEX;

	_state.deepEnter(context);
	_subStates.wideEnter(Fork::active, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
bool
M<TC, TMS>::_C<T, TS...>::deepUpdateAndTransition(Control& control,
												  Context& context)
{
	assert(Fork::active != INVALID_INDEX);

	if (_state.deepUpdateAndTransition(control, context)) {
		_subStates.wideUpdate(Fork::active, context);

		return true;
	} else
		return _subStates.wideUpdateAndTransition(Fork::active, control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepUpdate(Context& context) {
	assert(Fork::active != INVALID_INDEX);

	_state.deepUpdate(context);
	_subStates.wideUpdate(Fork::active, context);
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
	assert(Fork::active != INVALID_INDEX);

	_state.deepReact(event, control, context);
	_subStates.wideReact(Fork::active, event, control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepLeave(Context& context) {
	assert(Fork::active != INVALID_INDEX);

	_subStates.wideLeave(Fork::active, context);
	_state.deepLeave(context);

	HSFM_DEBUG_ONLY(Fork::resumableType = Fork::activeType);
	Fork::resumable = Fork::active;

	HSFM_DEBUG_ONLY(Fork::activeType.clear());
	Fork::active = INVALID_INDEX;
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepForwardRequest(const enum Transition::Type transition) {
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
M<TC, TMS>::_C<T, TS...>::deepRequestRemain() {
	if (Fork::active == INVALID_INDEX) {
		HSFM_DEBUG_ONLY(Fork::requestedType = TypeInfo::get<typename SubStates::Initial::Client>());
		Fork::requested = 0;
	}

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepRequestRestart() {
	HSFM_DEBUG_ONLY(Fork::requestedType = TypeInfo::get<typename SubStates::Initial::Client>());
	Fork::requested = 0;

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepRequestResume() {
	if (Fork::resumable != INVALID_INDEX) {
		HSFM_DEBUG_ONLY(Fork::requestedType = Fork::resumableType);
		Fork::requested = Fork::resumable;
	} else {
		HSFM_DEBUG_ONLY(Fork::requestedType = TypeInfo::get<typename SubStates::Initial::Client>());
		Fork::requested = 0;
	}

	_subStates.wideRequestResume(Fork::requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepChangeToRequested(Context& context) {
	assert(Fork::active != INVALID_INDEX);

	if (Fork::requested != INVALID_INDEX) {
		if (Fork::requested == Fork::active)
			_subStates.wideChangeToRequested(Fork::requested, context);
		else {
			_subStates.wideLeave(Fork::active, context);

			HSFM_DEBUG_ONLY(Fork::resumableType = Fork::activeType);
			Fork::resumable = Fork::active;

			HSFM_DEBUG_ONLY(Fork::activeType = Fork::requestedType);
			Fork::active = Fork::requested;

			HSFM_DEBUG_ONLY(Fork::requestedType.clear());
			Fork::requested = INVALID_INDEX;

			_subStates.wideEnter(Fork::active, context);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

}
