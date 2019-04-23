namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepRegister(StateRegistry& stateRegistry,
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

	active	  = requested;
	if (requested == resumable)
		resumable = INVALID_SHORT_INDEX;
	requested = INVALID_SHORT_INDEX;

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter(control);
	_subStates.deepEnter(control, active);
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
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepReact(FullControl& control,
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

	if (requested != INVALID_SHORT_INDEX)
		_subStates.deepForwardRequest(control, request, requested);
	else
		deepRequest					 (control, request);
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

#if !defined _MSC_VER && (!defined __clang_major__ || __clang_major__ < 7)

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
UProng
_C<NS, NC, NO, TA, TG, TH, TS...>::deepRequestChange(Control& control,
													 const ShortIndex /*prong*/)
{
	switch (STRATEGY) {
	case RegionStrategy::Composite:
		return deepRequestChangeComposite  (control);

	case RegionStrategy::Resumable:
		return deepRequestChangeResumable  (control);

	case RegionStrategy::Utilitarian:
		return deepRequestChangeUtilitarian(control);

	default:
		HFSM_BREAK();
		return {};
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
UProng
_C<NS, NC, NO, TA, TG, TH, TS...>::deepRequestChangeComposite(Control& control) {
	ShortIndex& requested = compoRequested(control);

	requested = 0;

	_subStates.deepRequestChange(control);

	return {};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
UProng
_C<NS, NC, NO, TA, TG, TH, TS...>::deepRequestChangeResumable(Control& control) {
	const ShortIndex  resumable = compoResumable(control);
		  ShortIndex& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT_INDEX) ?
		resumable : 0;

	_subStates.deepRequestChange(control, requested);

	return {};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
UProng
_C<NS, NC, NO, TA, TG, TH, TS...>::deepRequestChangeUtilitarian(Control& control) {
	const UProng result = _subStates.deepReportChange(control);

	ShortIndex& requested = compoRequested(control);
	requested = result.prong;

	return {};
}

//------------------------------------------------------------------------------

#if !defined _MSC_VER && (!defined __clang_major__ || __clang_major__ < 7)

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
UProng
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
UProng
_C<NS, NC, NO, TA, TG, TH, TS...>::deepReportChangeComposite(Control& control) {
	ShortIndex& requested = compoRequested(control);

	requested = 0;

	const UProng h = _headState.wrapUtility(control);
	const UProng s = _subStates.deepReportChange(control);

	return { h.utilityCompliment * s.utilityCompliment };
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
UProng
_C<NS, NC, NO, TA, TG, TH, TS...>::deepReportChangeResumable(Control& control) {
	const ShortIndex  resumable = compoResumable(control);
		  ShortIndex& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT_INDEX) ?
		resumable : 0;

	const UProng h = _headState.wrapUtility(control);
	const UProng s = _subStates.deepReportChange(control, requested);

	return { h.utilityCompliment * s.utilityCompliment };
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
UProng
_C<NS, NC, NO, TA, TG, TH, TS...>::deepReportChangeUtilitarian(Control& control) {
	const UProng h = _headState.wrapUtility(control);
	const UProng s = _subStates.deepReportChange(control);

	ShortIndex& requested = compoRequested(control);
	requested = s.prong;

	return { h.utilityCompliment * s.utilityCompliment };
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepRequestRemain(StateRegistry& stateRegistry) {
	const ShortIndex  active	= stateRegistry.compoActive	   [COMPO_INDEX];
		  ShortIndex& requested = stateRegistry.requested.compo[COMPO_INDEX];

	if (active == INVALID_SHORT_INDEX)
		requested = 0;

	_subStates.deepRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepRequestRestart(StateRegistry& stateRegistry) {
	ShortIndex& requested = stateRegistry.requested.compo[COMPO_INDEX];

	requested = 0;

	_subStates.deepRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepRequestResume(StateRegistry& stateRegistry,
													 const ShortIndex /*prong*/)
{
	const ShortIndex  resumable = stateRegistry.resumable.compo[COMPO_INDEX];
		  ShortIndex& requested = stateRegistry.requested.compo[COMPO_INDEX];

	requested = (resumable != INVALID_SHORT_INDEX) ?
		resumable : 0;

	_subStates.deepRequestResume(stateRegistry, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TG, TH, TS...>::deepRequestUtilize(Control& control) {
	const UProng result = _subStates.deepReportUtilize(control);

	ShortIndex& requested = compoRequested(control);
	requested = result.prong;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, RegionStrategy TG, typename TH, typename... TS>
UProng
_C<NS, NC, NO, TA, TG, TH, TS...>::deepReportUtilize(Control& control) {
	const UProng headResult = _headState.deepReportUtilize(control);
	const UProng subResults = _subStates.deepReportUtilize(control);

	UProng result{
		headResult.utilityCompliment * subResults.utilityCompliment,
		subResults.prong
	};

	ShortIndex& requested = compoRequested(control);
	requested = result.prong;

	return result;
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
	else if (requested == active) {
		// TODO: replace exit() + enter() => reenter()
		_subStates.deepExit				(control, active);

		if (requested != active) {
			resumable = active;
			active	  = requested;
		}
		requested = INVALID_SHORT_INDEX;

		_subStates.deepEnter(control, active);
	} else {
		_subStates.deepExit				(control, active);

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
