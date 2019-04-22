namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRegister(StateRegistry& stateRegistry,
											const Parent parent)
{
	stateRegistry.orthoParents[ORTHO_INDEX] = parent;

	HFSM_IF_ASSERT(const ShortIndex requestedIndex =)
	stateRegistry.requested.ortho.template emplace<OrthoForkT<AllForward::WIDTH>>();
	HFSM_ASSERT(requestedIndex == ORTHO_INDEX);

	HFSM_IF_ASSERT(const ShortIndex resumableIndex =)
	stateRegistry.resumable.ortho.template emplace<OrthoForkT<AllForward::WIDTH>>();
	HFSM_ASSERT(resumableIndex == ORTHO_INDEX);

	_headState.deepRegister(stateRegistry, parent);
	_subStates.wideRegister(stateRegistry, ORTHO_ID);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
bool
_O<NS, NC, NO, TA, TH, TS...>::deepForwardEntryGuard(GuardControl& control,
													 const ShortIndex /*prong*/)
{
	const OrthoFork& requested = orthoRequested(control);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested)
		return _subStates.wideForwardEntryGuard(control, requested);
	else
		return _subStates.wideForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
bool
_O<NS, NC, NO, TA, TH, TS...>::deepEntryGuard(GuardControl& control,
											  const ShortIndex /*prong*/)
{
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepEntryGuard(control) ||
		   _subStates.wideEntryGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepEnter(PlanControl& control,
										 const ShortIndex /*prong*/)
{
	OrthoFork& requested = orthoRequested(control);
	requested.clear();

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter(control);
	_subStates.wideEnter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
Status
_O<NS, NC, NO, TA, TH, TS...>::deepUpdate(FullControl& control,
										  const ShortIndex /*prong*/)
{
	ScopedRegion outer{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const auto headStatus = _headState.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates.wideUpdate(control);

		return headStatus;
	} else {
		const Status subStatus = _subStates.wideUpdate(control);

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
_O<NS, NC, NO, TA, TH, TS...>::deepReact(FullControl& control,
										 const TEvent& event,
										 const ShortIndex /*prong*/)
{
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepReact(control, event);
	_subStates.wideReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
bool
_O<NS, NC, NO, TA, TH, TS...>::deepForwardExitGuard(GuardControl& control,
													const ShortIndex /*prong*/)
{
	const OrthoFork& requested = orthoRequested(control);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested)
		return _subStates.wideForwardExitGuard(control, requested);
	else
		return _subStates.wideForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
bool
_O<NS, NC, NO, TA, TH, TS...>::deepExitGuard(GuardControl& control,
											 const ShortIndex /*prong*/)
{
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepExitGuard(control) ||
		   _subStates.wideExitGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepExit(PlanControl& control,
										const ShortIndex /*prong*/)
{
	_subStates.wideExit(control);
	_headState.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepForwardActive(Control& control,
												 const RequestType request,
												 const ShortIndex /*prong*/)
{
	HFSM_ASSERT(control._stateRegistry.isActive(HEAD_ID));

	const OrthoFork& requested = orthoRequested(control);
	HFSM_ASSERT(requested);

	_subStates.wideForwardActive(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepForwardRequest(Control& control,
												  const RequestType request,
												  const ShortIndex /*prong*/)
{
	const OrthoFork& requested = orthoRequested(control);

	if (requested)
		_subStates.wideForwardRequest(control, request, requested);
	else
		deepRequest					 (control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequest(Control& control,
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

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
UProng
_O<NS, NC, NO, TA, TH, TS...>::deepRequestChange(Control& control,
												 const ShortIndex /*prong*/)
{
	_subStates.wideRequestChange(control);

	return {};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
UProng
_O<NS, NC, NO, TA, TH, TS...>::deepReportChange(Control& control,
												  const ShortIndex /*prong*/)
{
	_subStates.wideReportChange(control);

	return {};
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestRemain(StateRegistry& stateRegistry) {
	_subStates.wideRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestRestart(StateRegistry& stateRegistry) {
	_subStates.wideRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestResume(StateRegistry& stateRegistry,
												 const ShortIndex /*prong*/)
{
	_subStates.wideRequestResume(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestUtilize(Control& control) {
	_subStates.wideRequestUtilize(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
UProng
_O<NS, NC, NO, TA, TH, TS...>::deepReportUtilize(Control& control) {
	const UProng headResult = _headState.deepReportUtilize(control);
	const UProng subResults = _subStates.wideReportUtilize(control);

	return {
		headResult.utilityCompliment * subResults.utilityCompliment,
		subResults.prong
	};
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepEnterRequested(PlanControl& control,
												  const ShortIndex /*prong*/)
{
	// TODO: pass requested orthoFork
	_headState.deepEnter		 (control);
	_subStates.wideEnterRequested(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepChangeToRequested(PlanControl& control,
													 const ShortIndex /*prong*/)
{
	// TODO: pass requested orthoFork
	_subStates.wideChangeToRequested(control);
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
