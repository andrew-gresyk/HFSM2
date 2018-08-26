namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
_C<NS, NC, NO, TA, TH, TS...>::_C(Registry& registry,
								  const Parent parent)
	: _headState{registry, parent}
	, _subStates{registry, COMPO_ID}
{
	HSFM_IF_DEBUG(Fork& fork = compoFork(registry));

	HSFM_IF_DEBUG(fork.TYPE = _headState.isBare() ? typeid(void) : typeid(Head));

	registry.forkParent(COMPO_ID) = parent;
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepForwardGuard(FullControl& control) {
	Fork& fork = compoFork(control);

	assert( fork.requested != INVALID_SHORT_INDEX);

	ControlRegion region{control, HEAD_ID, SubStateList::SIZE};

	if (fork.requested == fork.active)
		_subStates.wideForwardGuard(fork.requested, control);
	else
		_subStates.wideGuard	   (fork.requested, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepGuard(FullControl& control) {
	Fork& fork = compoFork(control);

	assert(fork.active    == INVALID_SHORT_INDEX &&
		   fork.requested != INVALID_SHORT_INDEX);

	if (!_headState.deepGuard(				  control))
		 _subStates.wideGuard(fork.requested, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepEnterInitial(PlanControl& control) {
	Fork& fork = compoFork(control);

	assert(fork.active    == INVALID_SHORT_INDEX &&
		   fork.resumable == INVALID_SHORT_INDEX &&
		   fork.requested == INVALID_SHORT_INDEX);

	fork.active = 0;

	_headState.deepEnter	   (control);
	_subStates.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepEnter(PlanControl& control) {
	Fork& fork = compoFork(control);

	assert(fork.active	   == INVALID_SHORT_INDEX &&
		   fork.requested != INVALID_SHORT_INDEX);

	fork.active	= fork.requested;
	fork.requested	= INVALID_SHORT_INDEX;

	_headState.deepEnter(			  control);
	_subStates.wideEnter(fork.active, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
Status
_C<NS, NC, NO, TA, TH, TS...>::deepUpdate(FullControl& control) {
	Fork& fork = compoFork(control);

	assert(fork.active != INVALID_SHORT_INDEX);

	ControlRegion region{control, HEAD_ID, SubStateList::SIZE};

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
	Fork& fork = compoFork(control);

	assert(fork.active != INVALID_SHORT_INDEX);

	ControlRegion region{control, HEAD_ID, SubStateList::SIZE};

	_headState.deepReact(			  event, control);
	_subStates.wideReact(fork.active, event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepExit(PlanControl& control) {
	Fork& fork = compoFork(control);

	assert(fork.active != INVALID_SHORT_INDEX);

	_subStates.wideExit(fork.active, control);
	_headState.deepExit(			 control);

	fork.resumable	= fork.active;
	fork.active		= INVALID_SHORT_INDEX;
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepForwardRequest(Registry& registry,
												  const TransitionType transition)
{
	Fork& fork = compoFork(registry);

	if (fork.requested != INVALID_SHORT_INDEX)
		_subStates.wideForwardRequest(registry, fork.requested, transition);
	else
		switch (transition) {
		case Transition::REMAIN:
			deepRequestRemain(registry);
			break;

		case Transition::RESTART:
			deepRequestRestart(registry);
			break;

		case Transition::RESUME:
			deepRequestResume(registry);
			break;

		default:
			assert(false);
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestRemain(Registry& registry) {
	Fork& fork = compoFork(registry);

	if (fork.active == INVALID_SHORT_INDEX)
		fork.requested = 0;

	_subStates.wideRequestRemain(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestRestart(Registry& registry) {
	Fork& fork = compoFork(registry);

	fork.requested = 0;

	_subStates.wideRequestRestart(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestResume(Registry& registry) {
	Fork& fork = compoFork(registry);

	fork.requested = fork.resumable != INVALID_SHORT_INDEX ?
		fork.resumable : 0;

	_subStates.wideRequestResume(registry, fork.requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepChangeToRequested(Registry& registry,
													 PlanControl& control)
{
	Fork& fork = compoFork(registry);

	assert(fork.active != INVALID_SHORT_INDEX);

	if (fork.requested == fork.active)
		_subStates.wideChangeToRequested(registry, fork.requested, control);
	else if (fork.requested != INVALID_SHORT_INDEX) {
		_subStates.wideExit(fork.active, control);

		fork.resumable	= fork.active;
		fork.active	= fork.requested;
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
