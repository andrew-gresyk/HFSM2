namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
_C<TID, TC, TSL, TPL, TH, TS...>::_C(StateRegistry& stateRegistry,
									 const Parent parent,
									 Parents& forkParents,
									 ForkPointers& forkPointers)
	: _fork(static_cast<ShortIndex>(forkPointers << &_fork),
			parent,
			forkParents)
	, _state(stateRegistry,
			 parent,
			 forkParents,
			 forkPointers)
	, _subStates(stateRegistry,
				 _fork.self,
				 forkParents,
				 forkPointers)
{}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepForwardGuard(TransitionControl& control) {
	assert(_fork.requested != INVALID_SHORT_INDEX);

	if (_fork.requested == _fork.active)
		_subStates.wideForwardGuard(_fork.requested, control);
	else
		_subStates.wideGuard	   (_fork.requested, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepGuard(TransitionControl& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.requested != INVALID_SHORT_INDEX);

	if (!_state	  .deepGuard(				  control))
		_subStates.wideGuard(_fork.requested, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepEnterInitial(Control& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX &&
		   _fork.requested == INVALID_SHORT_INDEX);

	_fork.active = 0;

	_state	  .deepEnter	   (control);
	_subStates.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepEnter(Control& control) {
	assert(_fork.active	   == INVALID_SHORT_INDEX &&
		   _fork.requested != INVALID_SHORT_INDEX);

	_fork.active	= _fork.requested;
	_fork.requested	= INVALID_SHORT_INDEX;

	_state	  .deepEnter(control);
	_subStates.wideEnter(_fork.active, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
bool
_C<TID, TC, TSL, TPL, TH, TS...>::deepUpdate(TransitionControl& control) {
	assert(_fork.active != INVALID_SHORT_INDEX);

	if (_state	  .deepUpdate(control)) {
		ControlLock lock(control);
		_subStates.wideUpdate(_fork.active, control);

		return true;
	} else
		return _subStates.wideUpdate(_fork.active, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
template <typename TEvent>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepReact(const TEvent& event,
											TransitionControl& control)
{
	assert(_fork.active != INVALID_SHORT_INDEX);

	_state	  .deepReact(			   event, control);
	_subStates.wideReact(_fork.active, event, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepExit(Control& control) {
	assert(_fork.active != INVALID_SHORT_INDEX);

	_subStates.wideExit(_fork.active, control);
	_state	  .deepExit(control);

	_fork.resumable	= _fork.active;
	_fork.active	= INVALID_SHORT_INDEX;
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepForwardRequest(const TransitionType transition) {
	if (_fork.requested != INVALID_SHORT_INDEX)
		_subStates.wideForwardRequest(_fork.requested, transition);
	else
		switch (transition) {
		case Transition::REMAIN:
			deepRequestRemain();
			break;

		case Transition::RESTART:
			deepRequestRestart();
			break;

		case Transition::RESUME:
			deepRequestResume();
			break;

		default:
			assert(false);
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepRequestRemain() {
	if (_fork.active == INVALID_SHORT_INDEX)
		_fork.requested = 0;

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepRequestRestart() {
	_fork.requested = 0;

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepRequestResume() {
	_fork.requested = _fork.resumable != INVALID_SHORT_INDEX ?
		_fork.resumable : 0;

	_subStates.wideRequestResume(_fork.requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepChangeToRequested(Control& control) {
	assert(_fork.active != INVALID_SHORT_INDEX);

	if (_fork.requested == _fork.active)
		_subStates.wideChangeToRequested(_fork.requested, control);
	else if (_fork.requested != INVALID_SHORT_INDEX) {
		_subStates.wideExit(_fork.active, control);

		_fork.resumable	= _fork.active;
		_fork.active	= _fork.requested;
		_fork.requested	= INVALID_SHORT_INDEX;

		_subStates.wideEnter(_fork.active, control);
	}
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_C<TID, TC, TSL, TPL, TH, TS...>::deepGetNames(const LongIndex parent,
											   const RegionType region,
											   const ShortIndex depth,
											   StructureStateInfos& _stateInfos) const
{
	_state	  .deepGetNames(parent, region,			 depth,		_stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, depth + 1, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
