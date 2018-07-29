namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
_O<TID, TC, TSL, TPL, TH, TS...>::_O(StateRegistry2& stateRegistry,
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
_O<TID, TC, TSL, TPL, TH, TS...>::deepForwardGuard(TransitionControl& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	if (_fork.requested != INVALID_SHORT_INDEX)
		_subStates.wideForwardGuard(_fork.requested, control);
	else
		_subStates.wideForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepGuard(TransitionControl& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	if (!_state	  .deepGuard(control))
		_subStates.wideGuard(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepEnterInitial(Control& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX &&
		   _fork.requested == INVALID_SHORT_INDEX);

	_state	  .deepEnter	   (control);
	_subStates.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepEnter(Control& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_state	  .deepEnter(control);
	_subStates.wideEnter(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
bool
_O<TID, TC, TSL, TPL, TH, TS...>::deepUpdate(TransitionControl& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	if (_state	  .deepUpdate(control)) {
		ControlLock lock(control);
		_subStates.wideUpdate(control);

		return true;
	} else
		return _subStates.wideUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
template <typename TEvent>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepReact(const TEvent& event,
											TransitionControl& control)
{
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_state	  .deepReact(event, control);
	_subStates.wideReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepExit(Control& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_subStates.wideExit(control);
	_state	  .deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepForwardRequest(const TransitionType transition) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

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
_O<TID, TC, TSL, TPL, TH, TS...>::deepRequestRemain() {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepRequestRestart() {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepRequestResume() {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_subStates.wideRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepChangeToRequested(Control& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX);

	_subStates.wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepGetNames(const LongIndex parent,
											   const RegionType region,
											   const ShortIndex depth,
											   StructureStateInfos& _stateInfos) const
{
	_state	  .deepGetNames(parent, region,			 depth,		_stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, depth + 1, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH, typename... TS>
void
_O<TID, TC, TSL, TPL, TH, TS...>::deepIsActive(const bool isActive,
											   LongIndex& index,
											   MachineStructure& structure) const
{
	_state	  .deepIsActive(isActive, index, structure);
	_subStates.wideIsActive(isActive, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
