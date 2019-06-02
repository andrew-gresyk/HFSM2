namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepRegister(StateRegistry& stateRegistry,
												const Parent parent)
{
	stateRegistry.compoParents[COMPO_INDEX] = parent;

	_headState.deepRegister(stateRegistry, parent);
	_subStates.deepRegister(stateRegistry, Parent{COMPO_ID});
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
bool
_C<NS, NC, NO, TA, TG, TH, TS...>::deepForwardEntryGuard(GuardControl& control,
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

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
bool
_C<NS, NC, NO, TA, TG, TH, TS...>::deepEntryGuard(GuardControl& control,
												  const ShortIndex /*prong*/)
{
	const ShortIndex requested = compoRequested(control);
	HFSM_ASSERT(requested != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepEntryGuard(control) ||
		   _subStates.deepEntryGuard(control, requested);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepEnter(PlanControl& control,
											 const ShortIndex /*prong*/)
{
	ShortIndex& active	  = compoActive   (control);
	ShortIndex& resumable = compoResumable(control);
	ShortIndex& requested = compoRequested(control);

	HFSM_ASSERT(active	  == INVALID_SHORT_INDEX &&
				requested != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter(control);

	active	  = requested;

	if (requested == resumable)
		resumable = INVALID_SHORT_INDEX;

	requested = INVALID_SHORT_INDEX;

	_subStates.deepEnter(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepReenter(PlanControl& control,
											   const ShortIndex /*prong*/)
{
	ShortIndex& active	  = compoActive   (control);
	ShortIndex& resumable = compoResumable(control);
	ShortIndex& requested = compoRequested(control);

	HFSM_ASSERT(active	  != INVALID_SHORT_INDEX &&
				requested != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepReenter(control);

	if (active == requested)
		_subStates.deepReenter(control, active);
	else {
		_subStates.deepExit	  (control, active);

		active	  = requested;

		if (requested == resumable)
			resumable = INVALID_SHORT_INDEX;

		_subStates.deepEnter  (control, active);
	}

	requested = INVALID_SHORT_INDEX;
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
Status
_C<NS, NC, NO, TA, TG, TH, TS...>::deepUpdate(FullControl& control,
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

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
template <typename TEvent>
Status
_C<NS, NC, NO, TA, TG, TH, TS...>::deepReact(FullControl& control,
											 const TEvent& event,
											 const ShortIndex /*prong*/)
{
	const ShortIndex active = compoActive(control);
	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion outer{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const Status headStatus = _headState.deepReact(control, event)) {
		ControlLock lock{control};
		_subStates.deepReact(control, event, active);

		return headStatus;
	} else {
		const Status subStatus = _subStates.deepReact(control, event, active);

		if (subStatus.outerTransition)
			return subStatus;

		ScopedRegion inner{control, REGION_ID, HEAD_ID, REGION_SIZE};

		return subStatus && control.planData().planExists[REGION_ID] ?
			control.updatePlan(_headState, subStatus) : subStatus;
	}
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
bool
_C<NS, NC, NO, TA, TG, TH, TS...>::deepForwardExitGuard(GuardControl& control,
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

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
bool
_C<NS, NC, NO, TA, TG, TH, TS...>::deepExitGuard(GuardControl& control,
												 const ShortIndex /*prong*/)
{
	const ShortIndex active = compoActive(control);
	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepExitGuard(control) ||
		   _subStates.deepExitGuard(control, active);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepExit(PlanControl& control,
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

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepForwardActive(Control& control,
													 const RequestType request,
													 const ShortIndex /*prong*/)
{
	HFSM_ASSERT(control._stateRegistry.isActive(HEAD_ID));

	const ShortIndex requested = compoRequested(control);

	if (requested == INVALID_SHORT_INDEX) {
		const ShortIndex active = compoActive(control);

		_subStates.deepForwardActive (control, request, active);
	} else
		_subStates.deepForwardRequest(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepForwardRequest(Control& control,
													  const RequestType request,
													  const ShortIndex /*prong*/)
{
	const ShortIndex requested = compoRequested(control);

	if (requested == INVALID_SHORT_INDEX)
		deepRequest					 (control, request);
	else
		_subStates.deepForwardRequest(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepRequest(Control& control,
											   const RequestType request)
{
	switch (request) {
	case Request::REMAIN:
		deepRequestRemain (control._stateRegistry);
		break;

	case Request::CHANGE:
		deepRequestChange (control);
		break;

	case Request::RESTART:
		deepRequestRestart(control._stateRegistry);
		break;

	case Request::RESUME:
		deepRequestResume (control._stateRegistry);
		break;

	case Request::UTILIZE:
		deepRequestUtilize(control);
		break;

	default:
		HFSM_BREAK();
	}
}

//------------------------------------------------------------------------------

#if !HFSM_EXPLICIT_MEMBER_SPECIALIZATION

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepRequestChange(Control& control,
													 const ShortIndex /*prong*/)
{
	switch (STRATEGY) {
	case RegionStrategy::Composite:
		deepRequestChangeComposite  (control);

	case RegionStrategy::Resumable:
		deepRequestChangeResumable  (control);

	case RegionStrategy::Utilitarian:
		deepRequestChangeUtilitarian(control);

	default:
		HFSM_BREAK();
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepRequestChangeComposite(Control& control) {
	ShortIndex& requested = compoRequested(control);

	requested = 0;

	_subStates.deepRequestChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepRequestChangeResumable(Control& control) {
	const ShortIndex  resumable = compoResumable(control);
		  ShortIndex& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT_INDEX) ?
		resumable : 0;

	_subStates.deepRequestChange(control, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepRequestChangeUtilitarian(Control& control) {
	const UP s = _subStates.deepReportChange(control);
	HFSM_ASSERT(s.prong != INVALID_SHORT_INDEX);

	ShortIndex& requested = compoRequested(control);
	requested = s.prong;

	HFSM_LOG_UTILITY_RESOLUTION(HEAD_ID, s.prong, s.utility);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepRequestRemain(StateRegistry& stateRegistry) {
	const ShortIndex  active	= compoActive   (stateRegistry);
		  ShortIndex& requested = compoRequested(stateRegistry);

	if (active == INVALID_SHORT_INDEX)
		requested = 0;

	_subStates.deepRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepRequestRestart(StateRegistry& stateRegistry) {
	ShortIndex& requested = compoRequested(stateRegistry);

	requested = 0;

	_subStates.deepRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepRequestResume(StateRegistry& stateRegistry,
													 const ShortIndex /*prong*/)
{
	const ShortIndex  resumable = compoResumable(stateRegistry);
		  ShortIndex& requested = compoRequested(stateRegistry);

	requested = (resumable != INVALID_SHORT_INDEX) ?
		resumable : 0;

	_subStates.deepRequestResume(stateRegistry, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepRequestUtilize(Control& control) {
	const UP s = _subStates.deepReportUtilize(control);

	ShortIndex& requested = compoRequested(control);
	requested = s.prong;

	HFSM_LOG_UTILITY_RESOLUTION(HEAD_ID, s.prong, s.utility);
}

//------------------------------------------------------------------------------

#if !HFSM_EXPLICIT_MEMBER_SPECIALIZATION

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
typename TA::UP
_C<NS, NC, NO, TA, TG, TH, TS...>::deepReportChange(Control& control,
													const ShortIndex /*prong*/)
{
	switch (STRATEGY) {
	case RegionStrategy::Composite:
		return deepReportChangeComposite  (control);

	case RegionStrategy::Resumable:
		return deepReportChangeResumable  (control);

	case RegionStrategy::Utilitarian:
		return deepReportChangeUtilitarian(control);

	default:
		HFSM_BREAK();
		return {};
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
typename TA::UP
_C<NS, NC, NO, TA, TG, TH, TS...>::deepReportChangeComposite(Control& control) {
	ShortIndex& requested = compoRequested(control);
	requested = 0;

	const UP h = _headState.deepReportChange(control);
	const UP s = _subStates.deepReportChange(control);

	return { h.utility * s.utility, h.prong };
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
typename TA::UP
_C<NS, NC, NO, TA, TG, TH, TS...>::deepReportChangeResumable(Control& control) {
	const ShortIndex  resumable = compoResumable(control);
		  ShortIndex& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT_INDEX) ?
		resumable : 0;

	const UP h = _headState.deepReportChange(control);
	const UP s = _subStates.deepReportChange(control, requested);

	return { h.utility * s.utility, h.prong };
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
typename TA::UP
_C<NS, NC, NO, TA, TG, TH, TS...>::deepReportChangeUtilitarian(Control& control) {
	const UP h = _headState.deepReportChange(control);
	const UP s = _subStates.deepReportChange(control);

	ShortIndex& requested = compoRequested(control);
	requested = s.prong;

	HFSM_LOG_UTILITY_RESOLUTION(HEAD_ID, s.prong, s.utility);

	return { h.utility * s.utility, h.prong };
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
typename TA::UP
_C<NS, NC, NO, TA, TG, TH, TS...>::deepReportUtilize(Control& control) {
	const UP h = _headState.deepReportUtilize(control);
	const UP s = _subStates.deepReportUtilize(control);

	ShortIndex& requested = compoRequested(control);
	requested = s.prong;

	HFSM_LOG_UTILITY_RESOLUTION(HEAD_ID, s.prong, s.utility);

	return { h.utility * s.utility, h.prong };
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepEnterRequested(PlanControl& control,
													  const ShortIndex /*prong*/)
{
	ShortIndex& active	  = compoActive	  (control);
	ShortIndex& requested = compoRequested(control);

	HFSM_ASSERT(active	  == INVALID_SHORT_INDEX);
	HFSM_ASSERT(requested != INVALID_SHORT_INDEX);

	active	  = requested;
	requested = INVALID_SHORT_INDEX;

	_headState.deepEnter(control);
	_subStates.deepEnter(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepChangeToRequested(PlanControl& control,
														 const ShortIndex /*prong*/)
{
	ShortIndex& active	  = compoActive	  (control);
	ShortIndex& resumable = compoResumable(control);
	ShortIndex& requested = compoRequested(control);

	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	if (requested == INVALID_SHORT_INDEX)
		_subStates.deepChangeToRequested(control, active);
	else if (requested != active) {
		_subStates.deepExit	  (control, active);

		resumable = active;
		active	  = requested;
		requested = INVALID_SHORT_INDEX;

		_subStates.deepEnter  (control, active);
	} else if (compoRemain(control)) {
		_subStates.deepExit	  (control, active);

		requested = INVALID_SHORT_INDEX;

		_subStates.deepEnter  (control, active);
	} else {
		requested = INVALID_SHORT_INDEX;

		_subStates.deepReenter(control, active);
	}
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepGetNames(const LongIndex parent,
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
