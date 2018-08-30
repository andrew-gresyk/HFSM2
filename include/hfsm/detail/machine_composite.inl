namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
_C<NS, NC, NO, TA, TH, TS...>::_C(StateData& stateData,
								  const Parent parent)
	: _headState{stateData, parent}
	, _subStates{stateData, COMPO_ID}
{
	stateData.compoParents[COMPO_INDEX] = parent;

	HSFM_IF_DEBUG(CompoFork& fork = compoFork(stateData));
	HSFM_IF_DEBUG(fork.TYPE = _headState.TYPE);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepForwardGuard(FullControl& control) {
	const CompoFork& fork = compoFork(control);

	assert( fork.requested != INVALID_SHORT_INDEX);

	ControlRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (fork.requested == fork.active)
		_subStates.wideForwardGuard(fork.requested, control);
	else
		_subStates.wideGuard	   (fork.requested, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepGuard(FullControl& control) {
	const CompoFork& fork = compoFork(control);

	assert(fork.active    == INVALID_SHORT_INDEX &&
		   fork.requested != INVALID_SHORT_INDEX);

	ControlRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (!_headState.deepGuard(				  control))
		 _subStates.wideGuard(fork.requested, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepEnterInitial(Control& control) {
	CompoFork& fork = compoFork(control);

	assert(fork.active    == INVALID_SHORT_INDEX &&
		   fork.resumable == INVALID_SHORT_INDEX &&
		   fork.requested == INVALID_SHORT_INDEX);

	fork.active = 0;

	ControlRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter	   (control);
	_subStates.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepEnter(Control& control) {
	CompoFork& fork = compoFork(control);

	assert(fork.active	  == INVALID_SHORT_INDEX &&
		   fork.requested != INVALID_SHORT_INDEX);

	fork.active	   = fork.requested;
	fork.requested = INVALID_SHORT_INDEX;

	ControlRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter(			  control);
	_subStates.wideEnter(fork.active, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
Status
_C<NS, NC, NO, TA, TH, TS...>::deepUpdate(FullControl& control) {
	CompoFork& fork = compoFork(control);

	assert(fork.active != INVALID_SHORT_INDEX);

	ControlRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const auto status = _headState.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates.wideUpdate(fork.active, control);

		return status;
	} else
		return _subStates.wideUpdate(fork.active, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
template <typename TEvent>
void
_C<NS, NC, NO, TA, TH, TS...>::deepReact(const TEvent& event,
										 FullControl& control)
{
	CompoFork& fork = compoFork(control);

	assert(fork.active != INVALID_SHORT_INDEX);

	ControlRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepReact(			  event, control);
	_subStates.wideReact(fork.active, event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepExit(Control& control) {
	CompoFork& fork = compoFork(control);

	assert(fork.active != INVALID_SHORT_INDEX);

	_subStates.wideExit(fork.active, control);
	_headState.deepExit(			 control);

	fork.resumable	= fork.active;
	fork.active		= INVALID_SHORT_INDEX;
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepForwardRequest(StateData& stateData,
												  const RequestType request)
{
	const CompoFork& fork = compoFork(stateData);

	if (fork.requested != INVALID_SHORT_INDEX)
		_subStates.wideForwardRequest(stateData, fork.requested, request);
	else
		switch (request) {
		case Request::REMAIN:
			deepRequestRemain(stateData);
			break;

		case Request::RESTART:
			deepRequestRestart(stateData);
			break;

		case Request::RESUME:
			deepRequestResume(stateData);
			break;

		default:
			assert(false);
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestRemain(StateData& stateData) {
	CompoFork& fork = compoFork(stateData);

	if (fork.active == INVALID_SHORT_INDEX)
		fork.requested = 0;

	_subStates.wideRequestRemain(stateData);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestRestart(StateData& stateData) {
	CompoFork& fork = compoFork(stateData);

	fork.requested = 0;

	_subStates.wideRequestRestart(stateData);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestResume(StateData& stateData) {
	CompoFork& fork = compoFork(stateData);

	fork.requested = (fork.resumable != INVALID_SHORT_INDEX) ?
		fork.resumable : 0;

	_subStates.wideRequestResume(stateData, fork.requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepChangeToRequested(StateData& stateData,
													 Control& control)
{
	CompoFork& fork = compoFork(stateData);

	assert(fork.active != INVALID_SHORT_INDEX);

	if (fork.requested == fork.active)
		_subStates.wideChangeToRequested(stateData, fork.requested, control);
	else if (fork.requested != INVALID_SHORT_INDEX) {
		_subStates.wideExit(fork.active, control);

		fork.resumable	= fork.active;
		fork.active		= fork.requested;
		fork.requested	= INVALID_SHORT_INDEX;

		_subStates.wideEnter(fork.active, control);
	}
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepGetNames(const LongIndex parent,
											const RegionType region,
											const ShortIndex depth,
											StructureStateInfos& _stateInfos) const
{
	_headState.deepGetNames(parent, region,			 depth,		_stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, depth + 1, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
