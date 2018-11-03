namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRegister(StateData& stateData,
											const Parent parent)
{
	// TODO: add parent/forks type arrays to StateRegistry
	//HFSM_IF_DEBUG(CompoFork0& fork = compoFork(stateData));
	//HFSM_IF_DEBUG(fork.TYPE = _headState.TYPE);

	stateData.compoParents[COMPO_INDEX] = parent;

	_headState.deepRegister(stateData, parent);
	_subStates.wideRegister(stateData, COMPO_ID);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
bool
_C<NS, NC, NO, TA, TH, TS...>::deepForwardGuard(GuardControl& control) {
	const ShortIndex active	   = compoActive   (control);
	const ShortIndex requested = compoRequested(control);

	HFSM_ASSERT(requested != INVALID_SHORT_INDEX);

	ControlRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested == active)
		return _subStates.wideForwardGuard(requested, control);
	else
		return _subStates.wideGuard		  (requested, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
bool
_C<NS, NC, NO, TA, TH, TS...>::deepGuard(GuardControl& control) {
	HFSM_IF_ASSERT(const ShortIndex active = compoActive(control));
	const ShortIndex requested = compoRequested(control);

	HFSM_ASSERT(active    == INVALID_SHORT_INDEX &&
				requested != INVALID_SHORT_INDEX);

	ControlRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepGuard(		   control)
		|| _subStates.wideGuard(requested, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepEnterInitial(Control& control) {
	ShortIndex& active = compoActive(control);
	HFSM_IF_ASSERT(const ShortIndex resumable = compoResumable(control));
	HFSM_IF_ASSERT(const ShortIndex requested = compoRequested(control));

	HFSM_ASSERT(active    == INVALID_SHORT_INDEX &&
				resumable == INVALID_SHORT_INDEX &&
				requested == INVALID_SHORT_INDEX);

	active = 0;

	ControlRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter	   (control);
	_subStates.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepEnter(Control& control) {
	ShortIndex& active	  = compoActive   (control);
	ShortIndex& requested = compoRequested(control);

	HFSM_ASSERT(active	  == INVALID_SHORT_INDEX &&
				requested != INVALID_SHORT_INDEX);

	active	  = requested;
	requested = INVALID_SHORT_INDEX;

	ControlRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter(		 control);
	_subStates.wideEnter(active, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
Status
_C<NS, NC, NO, TA, TH, TS...>::deepUpdate(FullControl& control) {
	const ShortIndex active = compoActive(control);

	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ControlRegion outer{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const Status headStatus = _headState.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates.wideUpdate(active, control);

		return headStatus;
	} else {
		const Status subStatus = _subStates.wideUpdate(active, control);

		if (subStatus.outerTransition)
			return subStatus;

		ControlRegion inner{control, REGION_ID, HEAD_ID, REGION_SIZE};

		return subStatus && control.planData().planExists[REGION_ID] ?
			control.updatePlan(_headState, subStatus) : subStatus;
	}
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
template <typename TEvent>
void
_C<NS, NC, NO, TA, TH, TS...>::deepReact(const TEvent& event,
										 FullControl& control)
{
	const ShortIndex active = compoActive(control);

	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ControlRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepReact(		 event, control);
	_subStates.wideReact(active, event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepExit(Control& control) {
	ShortIndex& active	  = compoActive   (control);
	ShortIndex& resumable = compoResumable(control);

	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	_subStates.wideExit(active, control);
	_headState.deepExit(		control);

	resumable = active;
	active	  = INVALID_SHORT_INDEX;

	auto plan = control.plan(REGION_ID);
	plan.clear();
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepForwardRequest(StateData& stateData,
												  const RequestType request)
{
	const ShortIndex compoRequested = stateData.requested.compo[COMPO_INDEX];

	if (compoRequested != INVALID_SHORT_INDEX)
		_subStates.wideForwardRequest(stateData, compoRequested, request);
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
			HFSM_BREAK();
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestRemain(StateData& stateData) {
	const ShortIndex  active	= stateData.compoActive	   [COMPO_INDEX];
		  ShortIndex& requested = stateData.requested.compo[COMPO_INDEX];

	if (active == INVALID_SHORT_INDEX)
		requested = 0;

	_subStates.wideRequestRemain(stateData);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestRestart(StateData& stateData) {
	ShortIndex& requested = stateData.requested.compo[COMPO_INDEX];

	requested = 0;

	_subStates.wideRequestRestart(stateData);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepRequestResume(StateData& stateData) {
	const ShortIndex  resumable = stateData.resumable.compo[COMPO_INDEX];
		  ShortIndex& requested = stateData.requested.compo[COMPO_INDEX];

	requested = (resumable != INVALID_SHORT_INDEX) ?
		resumable : 0;

	_subStates.wideRequestResume(stateData, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_C<NS, NC, NO, TA, TH, TS...>::deepChangeToRequested(StateData& stateData,
													 Control& control)
{
	ShortIndex& active	  = stateData.compoActive	 [COMPO_INDEX];
	ShortIndex& resumable = stateData.resumable.compo[COMPO_INDEX];
	ShortIndex& requested = stateData.requested.compo[COMPO_INDEX];

	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	if (requested == active)
		_subStates.wideChangeToRequested(stateData, requested, control);
	else if (requested != INVALID_SHORT_INDEX) {
		_subStates.wideExit(active, control);

		resumable = active;
		active	  = requested;
		requested = INVALID_SHORT_INDEX;

		_subStates.wideEnter(active, control);
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
