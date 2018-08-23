namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
_C<NS, NC, NO, TA, TH, TS...>::_C(StateParents& stateParents,
								  const Parent parent,
								  Parents& forkParents,
								  ForkPointers& forkPointers)
	: _fork(static_cast<ShortIndex>(forkPointers << &_fork),
			parent,
			forkParents,
			INVALID_SHORT_INDEX
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
_C<NS, NC, NO, TA, TH, TS...>::deepForwardGuard(FullControl& control) {
	assert(_fork.requested != INVALID_SHORT_INDEX);

	ControlRegion region{control, HEAD_ID, SubStateList::SIZE};

	if (_fork.requested == _fork.active)
		_subStates.wideForwardGuard(_fork.requested, control);
	else
		_subStates.wideGuard	   (_fork.requested, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepGuard(FullControl& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.requested != INVALID_SHORT_INDEX);

	if (!_state	  .deepGuard(				  control))
		_subStates.wideGuard(_fork.requested, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepEnterInitial(PlanControl& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX &&
		   _fork.requested == INVALID_SHORT_INDEX);

	_fork.active = 0;

	_state	  .deepEnter	   (control);
	_subStates.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepEnter(PlanControl& control) {
	assert(_fork.active	   == INVALID_SHORT_INDEX &&
		   _fork.requested != INVALID_SHORT_INDEX);

	_fork.active	= _fork.requested;
	_fork.requested	= INVALID_SHORT_INDEX;

	_state	  .deepEnter(control);
	_subStates.wideEnter(_fork.active, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
Status
_C<NS, NC, NO, TA, TH, TS...>::deepUpdate(FullControl& control) {
	assert(_fork.active != INVALID_SHORT_INDEX);

	ControlRegion region{control, HEAD_ID, SubStateList::SIZE};

	if (const auto status = _state.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates.wideUpdate(_fork.active, control);

		return status;
	} else
		return _subStates.wideUpdate(_fork.active, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
template <typename TEvent>
void
_C<NS, NC, NO, TA, TH, TS...>::deepReact(const TEvent& event,
										 FullControl& control)
{
	assert(_fork.active != INVALID_SHORT_INDEX);

	ControlRegion region{control, HEAD_ID, SubStateList::SIZE};

	_state	  .deepReact(			   event, control);
	_subStates.wideReact(_fork.active, event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepExit(PlanControl& control) {
	assert(_fork.active != INVALID_SHORT_INDEX);

	_subStates.wideExit(_fork.active, control);
	_state	  .deepExit(control);

	_fork.resumable	= _fork.active;
	_fork.active	= INVALID_SHORT_INDEX;
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepForwardRequest(const TransitionType transition) {
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
_C<NS, NC, NO, TA, TH, TS...>::deepRequestRemain() {
	if (_fork.active == INVALID_SHORT_INDEX)
		_fork.requested = 0;

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestRestart() {
	_fork.requested = 0;

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestResume() {
	_fork.requested = _fork.resumable != INVALID_SHORT_INDEX ?
		_fork.resumable : 0;

	_subStates.wideRequestResume(_fork.requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepChangeToRequested(PlanControl& control) {
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

template <StateID NS, ForkID NC, ForkID NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepGetNames(const LongIndex parent,
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
