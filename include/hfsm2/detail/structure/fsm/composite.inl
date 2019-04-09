namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRegister(StateRegistry& stateRegistry,
											const Parent parent)
{
	// TODO: add parent/forks type arrays to StateRegistry
	//HFSM_IF_DEBUG(CompoFork0& fork = compoFork(stateRegistry));
	//HFSM_IF_DEBUG(fork.TYPE = _headState.TYPE);

	stateRegistry.compoParents[COMPO_INDEX] = parent;

	_headState.deepRegister(stateRegistry, parent);
	_subStates.deepRegister(stateRegistry, Parent{COMPO_ID});
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
bool
_C<NS, NC, NO, TA, TH, TS...>::deepForwardEntryGuard(GuardControl& control,
													 const ShortIndex /*prong*/)
{
	const ShortIndex active	   = compoActive   (control);
	const ShortIndex requested = compoRequested(control);

	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested == INVALID_SHORT_INDEX)
		return _subStates.deepForwardEntryGuard(control, active);
	else
		return _subStates.deepEntryGuard	   (control, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
bool
_C<NS, NC, NO, TA, TH, TS...>::deepEntryGuard(GuardControl& control,
											  const ShortIndex /*prong*/)
{
	const ShortIndex requested = compoRequested(control);
	HFSM_ASSERT(requested != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepEntryGuard(control) ||
		   _subStates.deepEntryGuard(control, requested);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepEnterInitial(Control& control) {
	HFSM_ASSERT(compoResumable(control) == INVALID_SHORT_INDEX);
	HFSM_ASSERT(compoRequested(control) == INVALID_SHORT_INDEX);

	ShortIndex& active = compoActive(control);
	HFSM_ASSERT(active == INVALID_SHORT_INDEX);

	active = 0;

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter	   (control);
	_subStates.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepEnter(Control& control,
										 const ShortIndex /*prong*/)
{
	ShortIndex& active	  = compoActive   (control);
	ShortIndex& resumable = compoResumable(control);
	ShortIndex& requested = compoRequested(control);

	HFSM_ASSERT(active	  == INVALID_SHORT_INDEX &&
				requested != INVALID_SHORT_INDEX);

	active	  = requested;
	if (requested == resumable)
		resumable = INVALID_SHORT_INDEX;
	requested = INVALID_SHORT_INDEX;

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter(control);
	_subStates.deepEnter(control, active);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
Status
_C<NS, NC, NO, TA, TH, TS...>::deepUpdate(FullControl& control,
										  const ShortIndex /*prong*/)
{
	const ShortIndex active = compoActive(control);
	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion outer{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const Status headStatus = _headState.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates.deepUpdate(control, active);

		return headStatus;
	} else {
		const Status subStatus = _subStates.deepUpdate(control, active);

		if (subStatus.outerTransition)
			return subStatus;

		ScopedRegion inner{control, REGION_ID, HEAD_ID, REGION_SIZE};

		return subStatus && control.planData().planExists[REGION_ID] ?
			control.updatePlan(_headState, subStatus) : subStatus;
	}
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
template <typename TEvent>
void
_C<NS, NC, NO, TA, TH, TS...>::deepReact(FullControl& control,
										 const TEvent& event,
										 const ShortIndex /*prong*/)
{
	const ShortIndex active = compoActive(control);
	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepReact(control, event);
	_subStates.deepReact(control, event, active);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
bool
_C<NS, NC, NO, TA, TH, TS...>::deepForwardExitGuard(GuardControl& control,
													const ShortIndex /*prong*/)
{
	const ShortIndex active = compoActive(control);
	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (compoRequested(control) == INVALID_SHORT_INDEX)
		return _subStates.deepForwardExitGuard(control, active);
	else
		return _subStates.deepExitGuard		  (control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
bool
_C<NS, NC, NO, TA, TH, TS...>::deepExitGuard(GuardControl& control,
											 const ShortIndex /*prong*/)
{
	const ShortIndex active = compoActive(control);
	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepExitGuard(control) ||
		   _subStates.deepExitGuard(control, active);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepExit(Control& control,
										const ShortIndex /*prong*/)
{
	ShortIndex& active	  = compoActive   (control);
	ShortIndex& resumable = compoResumable(control);

	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	_subStates.deepExit(control, active);
	_headState.deepExit(control);

	resumable = active;
	active	  = INVALID_SHORT_INDEX;

	auto plan = control.plan(REGION_ID);
	plan.clear();
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepForwardActive(StateRegistry& stateRegistry,
												 const RequestType request,
												 const ShortIndex /*prong*/)
{
	HFSM_ASSERT(stateRegistry.isActive(HEAD_ID));

	const ShortIndex requested = stateRegistry.requested.compo[COMPO_INDEX];

	if (requested == INVALID_SHORT_INDEX) {
		const ShortIndex active = stateRegistry.compoActive[COMPO_INDEX];

		_subStates.deepForwardActive (stateRegistry, request, active);
	} else
		_subStates.deepForwardRequest(stateRegistry, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepForwardRequest(StateRegistry& stateRegistry,
												  const RequestType request,
												  const ShortIndex /*prong*/)
{
	const ShortIndex requested = stateRegistry.requested.compo[COMPO_INDEX];

	if (requested != INVALID_SHORT_INDEX)
		_subStates.deepForwardRequest(stateRegistry, request, requested);
	else
		switch (request) {
		case Request::REMAIN:
			deepRequestRemain (stateRegistry);
			break;

		case Request::RESTART:
			deepRequestRestart(stateRegistry);
			break;

		case Request::RESUME:
			deepRequestResume (stateRegistry, INVALID_SHORT_INDEX);
			break;

		default:
			HFSM_BREAK();
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestRemain(StateRegistry& stateRegistry) {
	const ShortIndex  active	= stateRegistry.compoActive	   [COMPO_INDEX];
		  ShortIndex& requested = stateRegistry.requested.compo[COMPO_INDEX];

	if (active == INVALID_SHORT_INDEX)
		requested = 0;

	_subStates.deepRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestRestart(StateRegistry& stateRegistry) {
	ShortIndex& requested = stateRegistry.requested.compo[COMPO_INDEX];

	requested = 0;

	_subStates.deepRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestResume(StateRegistry& stateRegistry,
												 const ShortIndex /*prong*/)
{
	const ShortIndex  resumable = stateRegistry.resumable.compo[COMPO_INDEX];
		  ShortIndex& requested = stateRegistry.requested.compo[COMPO_INDEX];

	requested = (resumable != INVALID_SHORT_INDEX) ?
		resumable : 0;

	_subStates.deepRequestResume(stateRegistry, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepChangeToRequested(StateRegistry& stateRegistry,
													 Control& control,
													 const ShortIndex /*prong*/)
{
	ShortIndex& active	  = stateRegistry.compoActive	 [COMPO_INDEX];
	ShortIndex& resumable = stateRegistry.resumable.compo[COMPO_INDEX];
	ShortIndex& requested = stateRegistry.requested.compo[COMPO_INDEX];

	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	if (requested == INVALID_SHORT_INDEX)
		_subStates.deepChangeToRequested(stateRegistry, control, active);
	else {
		_subStates.deepExit(control, active);

		if (requested != active) {
			resumable = active;
			active	  = requested;
		}
		requested = INVALID_SHORT_INDEX;

		_subStates.deepEnter(control, active);
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
	_headState.deepGetNames(parent,					 region,						depth,	   _stateInfos);
	_subStates.deepGetNames(_stateInfos.count() - 1, StructureStateInfo::COMPOSITE, depth + 1, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
