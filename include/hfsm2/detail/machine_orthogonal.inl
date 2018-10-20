namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
_O<NS, NC, NO, TA, TH, TS...>::_O(StateData& stateData,
								  const Parent parent)
	: _headState{stateData, parent}
	, _subStates{stateData, ORTHO_ID}
{
	stateData.orthoParents[ORTHO_INDEX] = parent;

	HSFM_IF_ASSERT(const ShortIndex requestedIndex =)
	stateData.orthoRequested.template emplace<OrthoForkT<Forward::WIDTH>>();
	HFSM_ASSERT(requestedIndex == ORTHO_INDEX);

	HSFM_IF_ASSERT(const ShortIndex resumableIndex =)
	stateData.orthoResumable.template emplace<OrthoForkT<Forward::WIDTH>>();
	HFSM_ASSERT(resumableIndex == ORTHO_INDEX);

#ifdef _DEBUG
	OrthoFork& requested = orthoRequested(stateData);
	requested.TYPE = _headState.TYPE;

	OrthoFork& resumable = stateData.orthoResumable[ORTHO_INDEX];
	resumable.TYPE = _headState.TYPE;
#endif
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepForwardGuard(FullControl& control) {
	const OrthoFork& requested = orthoRequested(control);

	ControlRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested.prongs)
		_subStates.wideForwardGuard(requested.prongs, control);
	else
		_subStates.wideForwardGuard(				  control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepGuard(FullControl& control) {
	ControlRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (!_headState.deepGuard(control))
		 _subStates.wideGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepEnterInitial(Control& control) {
	HSFM_IF_ASSERT(const OrthoFork& requested = orthoRequested(control));

	HFSM_ASSERT(!requested.prongs);

	ControlRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter	   (control);
	_subStates.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepEnter(Control& control) {
	OrthoFork& requested = orthoRequested(control);
	requested.prongs.clear();

	ControlRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter(control);
	_subStates.wideEnter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
Status
_O<NS, NC, NO, TA, TH, TS...>::deepUpdate(FullControl& control) {
	ControlRegion outer{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const auto headStatus = _headState.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates.wideUpdate(control);

		return headStatus;
	} else {
		const Status subStatus = _subStates.wideUpdate(control);

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
_O<NS, NC, NO, TA, TH, TS...>::deepReact(const TEvent& event,
										 FullControl& control)
{
	ControlRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepReact(event, control);
	_subStates.wideReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepExit(Control& control) {
	_subStates.wideExit(control);
	_headState.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepForwardRequest(StateData& stateData,
												  const RequestType request)
{
	const OrthoFork& requested = orthoRequested(stateData);

	if (requested.prongs)
		_subStates.wideForwardRequest(stateData, requested.prongs, request);
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
			HSFM_BREAK();
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestRemain(StateData& stateData) {
	_subStates.wideRequestRemain(stateData);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestRestart(StateData& stateData) {
	_subStates.wideRequestRestart(stateData);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestResume(StateData& stateData) {
	_subStates.wideRequestResume(stateData);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepChangeToRequested(StateData& stateData,
													 Control& control)
{
	_subStates.wideChangeToRequested(stateData, control);
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
