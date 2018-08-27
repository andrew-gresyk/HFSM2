namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
_O<NS, NC, NO, TA, TH, TS...>::_O(Registry& registry,
								  const Parent parent)
	: _headState{registry, parent}
	, _subStates{registry, ORTHO_ID}
{
	registry.orthoParents[ORTHO_INDEX] = parent;

	registry.orthoRequested.template emplace<OrthoForkT<Forward::WIDTH>>();
	registry.orthoResumable.template emplace<OrthoForkT<Forward::WIDTH>>();

	HSFM_IF_DEBUG(OrthoFork& requested = orthoRequested(registry));
	HSFM_IF_DEBUG(requested.TYPE = _headState.TYPE);

	HSFM_IF_DEBUG(OrthoFork& resumable = registry.orthoResumable[ORTHO_INDEX]);
	HSFM_IF_DEBUG(resumable.TYPE = _headState.TYPE);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepForwardGuard(FullControl& control) {
	const OrthoFork& requested = orthoRequested(control);

	if (requested.prongs)
		_subStates.wideForwardGuard(requested.prongs, control);
	else
		_subStates.wideForwardGuard(				  control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepGuard(FullControl& control) {
	if (!_headState.deepGuard(control))
		 _subStates.wideGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepEnterInitial(PlanControl& control) {
	HSFM_IF_ASSERT(const OrthoFork& requested = orthoRequested(control));

	assert(!requested.prongs);

	_headState.deepEnter	   (control);
	_subStates.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepEnter(PlanControl& control) {
	OrthoFork& requested = orthoRequested(control);
	requested.prongs.clear();

	_headState.deepEnter(control);
	_subStates.wideEnter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
Status
_O<NS, NC, NO, TA, TH, TS...>::deepUpdate(FullControl& control) {
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
	_headState.deepReact(event, control);
	_subStates.wideReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepExit(PlanControl& control) {
	_subStates.wideExit(control);
	_headState.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepForwardRequest(Registry& registry,
												  const RequestType request)
{
	const OrthoFork& requested = orthoRequested(registry);

	if (requested.prongs)
		_subStates.wideForwardRequest(registry, requested.prongs, request);
	else
		switch (request) {
		case Request::REMAIN:
			deepRequestRemain(registry);
			break;

		case Request::RESTART:
			deepRequestRestart(registry);
			break;

		case Request::RESUME:
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
	_subStates.wideRequestRemain(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestRestart(Registry& registry) {
	_subStates.wideRequestRestart(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepRequestResume(Registry& registry) {
	_subStates.wideRequestResume(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_O<NS, NC, NO, TA, TH, TS...>::deepChangeToRequested(Registry& registry,
													 PlanControl& control)
{
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
