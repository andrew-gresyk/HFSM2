namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
_O<NS, NC, NO, TA, TH, TS...>::_O(Registry& registry,
								  const Parent parent)
	: _headState{registry, parent}
	, _subStates{registry, ORTHO_ID}
{
	Fork& fork = orthoFork(registry);

	fork.active		= (ShortIndex) STATE_COUNT;
	fork.resumable	= (ShortIndex) STATE_COUNT;
	HSFM_IF_DEBUG(fork.TYPE = _headState.isBare() ? typeid(void) : typeid(Head));

	registry.forkParent(ORTHO_ID) = parent;
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepForwardGuard(FullControl& control) {
	Fork& fork = orthoFork(control);

	assert(fork.active    == (ShortIndex) STATE_COUNT &&
		   fork.resumable == (ShortIndex) STATE_COUNT);

	if (fork.requested != INVALID_SHORT_INDEX)
		_subStates.wideForwardGuard(fork.requested, control);
	else
		_subStates.wideForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepGuard(FullControl& control) {
	HSFM_IF_ASSERT(Fork& fork = orthoFork(control));

	assert(fork.active    == (ShortIndex) STATE_COUNT &&
		   fork.resumable == (ShortIndex) STATE_COUNT);

	if (!_headState.deepGuard(control))
		 _subStates.wideGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepEnterInitial(PlanControl& control) {
	HSFM_IF_ASSERT(Fork& fork = orthoFork(control));

	assert(fork.active    == (ShortIndex) STATE_COUNT &&
		   fork.resumable == (ShortIndex) STATE_COUNT &&
		   fork.requested == INVALID_SHORT_INDEX);

	_headState.deepEnter	   (control);
	_subStates.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepEnter(PlanControl& control) {
	HSFM_IF_ASSERT(Fork& fork = orthoFork(control));

	assert(fork.active    == (ShortIndex) STATE_COUNT &&
		   fork.resumable == (ShortIndex) STATE_COUNT);

	_headState.deepEnter(control);
	_subStates.wideEnter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
Status
_O<NS, NC, NO, TA, TH, TS...>::deepUpdate(FullControl& control) {
	HSFM_IF_ASSERT(Fork& fork = orthoFork(control));

	assert(fork.active    == (ShortIndex) STATE_COUNT &&
		   fork.resumable == (ShortIndex) STATE_COUNT);

	ControlRegion region{control, HEAD_ID, SubStateList::SIZE};

	if (const auto status = _headState.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates.wideUpdate(control);

		return status;
	} else
		return _subStates.wideUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
template <typename TEvent>
void
_O<NS, NC, NO, TA, TH, TS...>::deepReact(const TEvent& event,
										 FullControl& control)
{
	HSFM_IF_ASSERT(Fork& fork = orthoFork(control));

	assert(fork.active    == (ShortIndex) STATE_COUNT &&
		   fork.resumable == (ShortIndex) STATE_COUNT);

	_headState.deepReact(event, control);
	_subStates.wideReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepExit(PlanControl& control) {
	HSFM_IF_ASSERT(Fork& fork = orthoFork(control));

	assert(fork.active    == (ShortIndex) STATE_COUNT &&
		   fork.resumable == (ShortIndex) STATE_COUNT);

	_subStates.wideExit(control);
	_headState.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepForwardRequest(Registry& registry,
												  const TransitionType transition)
{
	Fork& fork = orthoFork(registry);

	assert(fork.active    == (ShortIndex) STATE_COUNT &&
		   fork.resumable == (ShortIndex) STATE_COUNT);

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
_O<NS, NC, NO, TA, TH, TS...>::deepRequestRemain(Registry& registry) {
	HSFM_IF_ASSERT(Fork& fork = orthoFork(registry));

	assert(fork.active    == (ShortIndex) STATE_COUNT &&
		   fork.resumable == (ShortIndex) STATE_COUNT);

	_subStates.wideRequestRemain(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestRestart(Registry& registry) {
	HSFM_IF_ASSERT(Fork& fork = orthoFork(registry));

	assert(fork.active    == (ShortIndex) STATE_COUNT &&
		   fork.resumable == (ShortIndex) STATE_COUNT);

	_subStates.wideRequestRestart(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestResume(Registry& registry) {
	HSFM_IF_ASSERT(Fork& fork = orthoFork(registry));

	assert(fork.active    == (ShortIndex) STATE_COUNT &&
		   fork.resumable == (ShortIndex) STATE_COUNT);

	_subStates.wideRequestResume(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepChangeToRequested(Registry& registry,
													 PlanControl& control)
{
	HSFM_IF_ASSERT(Fork& fork = orthoFork(registry));

	assert(fork.active    == (ShortIndex) STATE_COUNT &&
		   fork.resumable == (ShortIndex) STATE_COUNT);

	_subStates.wideChangeToRequested(registry, control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepGetNames(const LongIndex parent,
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
