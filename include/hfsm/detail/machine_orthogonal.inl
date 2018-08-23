namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
_O<NS, NC, NO, TA, TH, TS...>::_O(StateParents& stateParents,
								  const Parent parent,
								  Parents& forkParents,
								  ForkPointers& forkPointers)
	: _fork(static_cast<ShortIndex>(forkPointers << &_fork),
			parent,
			forkParents,
			STATE_COUNT
			HSFM_IF_DEBUG(, _state.isBare() ? typeid(void) : typeid(Head)))
	, _state(stateParents,
			 parent,
			 forkParents,
			 forkPointers)
	, _subStates(stateParents,
				 _fork.self,
				 forkParents,
				 forkPointers)
{}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepForwardGuard(FullControl& control) {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	if (_fork.requested != INVALID_SHORT_INDEX)
		_subStates.wideForwardGuard(_fork.requested, control);
	else
		_subStates.wideForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepGuard(FullControl& control) {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	if (!_state	  .deepGuard(control))
		_subStates.wideGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepEnterInitial(PlanControl& control) {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT &&
		   _fork.requested == INVALID_SHORT_INDEX);

	_state	  .deepEnter	   (control);
	_subStates.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepEnter(PlanControl& control) {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	_state	  .deepEnter(control);
	_subStates.wideEnter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
Status
_O<NS, NC, NO, TA, TH, TS...>::deepUpdate(FullControl& control) {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	ControlRegion region{control, HEAD_ID, SubStateList::SIZE};

	if (const auto status = _state.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates.wideUpdate(control);

		return status;
	} else
		return _subStates.wideUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
template <typename TEvent>
void
_O<NS, NC, NO, TA, TH, TS...>::deepReact(const TEvent& event,
										 FullControl& control)
{
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	_state	  .deepReact(event, control);
	_subStates.wideReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepExit(PlanControl& control) {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	_subStates.wideExit(control);
	_state	  .deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepForwardRequest(const TransitionType transition) {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

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

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestRemain() {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestRestart() {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestResume() {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	_subStates.wideRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepChangeToRequested(PlanControl& control) {
	assert(_fork.active    == STATE_COUNT &&
		   _fork.resumable == STATE_COUNT);

	_subStates.wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepGetNames(const LongIndex parent,
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
