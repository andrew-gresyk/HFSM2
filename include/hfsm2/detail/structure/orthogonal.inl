namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRegister(Registry& registry,
										const Parent parent)
{
	registry.orthoParents[ORTHO_INDEX] = parent;
	registry.orthoUnits[ORTHO_INDEX] = Units{ORTHO_UNIT, WIDTH};

	_headState.deepRegister(registry, parent);
	_subStates.wideRegister(registry, ORTHO_ID);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
bool
O_<TN, TA, TH, TS...>::deepForwardEntryGuard(GuardControl& control) {
	const ProngConstBits requested = orthoRequested(static_cast<const GuardControl&>(control));

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested)
		return _subStates.wideForwardEntryGuard(control, requested);
	else
		return _subStates.wideForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
bool
O_<TN, TA, TH, TS...>::deepEntryGuard(GuardControl& control) {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepEntryGuard(control) ||
		   _subStates.wideEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepConstruct(PlanControl& control) {
	ProngBits requested = orthoRequested(control);
	requested.clear();

	_headState.deepConstruct(control);
	_subStates.wideConstruct(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepEnter(PlanControl& control) {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter(control);
	_subStates.wideEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepReenter(PlanControl& control) {
	ProngBits requested = orthoRequested(control);
	requested.clear();

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepReenter(control);
	_subStates.wideReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
Status
O_<TN, TA, TH, TS...>::deepUpdate(FullControl& control) {
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

	#ifdef HFSM2_ENABLE_PLANS
		return subStatus && control._planData.planExists.template get<REGION_ID>() ?
			control.updatePlan(_headState, subStatus) : subStatus;
	#else
		return subStatus;
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
template <typename TEvent>
Status
O_<TN, TA, TH, TS...>::deepReact(FullControl& control,
								 const TEvent& event)
{
	ScopedRegion outer{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const auto headStatus = _headState.deepReact(control, event)) {
		ControlLock lock{control};
		_subStates.wideReact(control, event);

		return headStatus;
	} else {
		const Status subStatus = _subStates.wideReact(control, event);

		if (subStatus.outerTransition)
			return subStatus;

		ScopedRegion inner{control, REGION_ID, HEAD_ID, REGION_SIZE};

	#ifdef HFSM2_ENABLE_PLANS
		return subStatus && control._planData.planExists.template get<REGION_ID>() ?
			control.updatePlan(_headState, subStatus) : subStatus;
	#else
		return subStatus;
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
bool
O_<TN, TA, TH, TS...>::deepForwardExitGuard(GuardControl& control) {
	const ProngConstBits requested = orthoRequested(static_cast<const GuardControl&>(control));

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested)
		return _subStates.wideForwardExitGuard(control, requested);
	else
		return _subStates.wideForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
bool
O_<TN, TA, TH, TS...>::deepExitGuard(GuardControl& control) {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepExitGuard(control) ||
		   _subStates.wideExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepExit(PlanControl& control) {
	_subStates.wideExit(control);
	_headState.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepDestruct(PlanControl& control) {
	_subStates.wideDestruct(control);
	_headState.deepDestruct(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepForwardActive(Control& control,
										 const Request request)
{
	HFSM2_ASSERT(control._registry.isActive(HEAD_ID));

	const ProngConstBits requested = orthoRequested(static_cast<const Control&>(control));
	HFSM2_ASSERT(!!requested);

	_subStates.wideForwardActive(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepForwardRequest(Control& control,
										  const Request request)
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const ProngConstBits requested = orthoRequested(static_cast<const Control&>(control));

	if (requested)
		_subStates.wideForwardRequest(control, request);
	else
		deepRequest					 (control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRequest(Control& control,
								   const Request request)
{
	switch (request.type) {
	case TransitionType::CHANGE:
		deepRequestChange	(control, request);
		break;

	case TransitionType::RESTART:
		deepRequestRestart	(control, request);
		break;

	case TransitionType::RESUME:
		deepRequestResume	(control, request);
		break;

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	case TransitionType::UTILIZE:
		deepRequestUtilize	(control, request);
		break;

	case TransitionType::RANDOMIZE:
		deepRequestRandomize(control, request);
		break;

#endif

	default:
		HFSM2_BREAK();
	}
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRequestChange(Control& control,
										 const Request request)
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	_subStates.wideRequestChange(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRequestRestart(Control& control,
										  const Request request)
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	_subStates.wideRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRequestResume(Control& control,
										 const Request request)
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	_subStates.wideRequestResume(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRequestUtilize(Control& control,
										  const Request request)
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	_subStates.wideRequestUtilize(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRequestRandomize(Control& control,
											const Request request)
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	_subStates.wideRequestRandomize(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
typename TA::UP
O_<TN, TA, TH, TS...>::deepReportChange(Control& control) {
	const UP	  h = _headState.deepReportChange(control);
	const Utility s = _subStates.wideReportChange(control);

	const Utility sub = s / WIDTH;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, INVALID_STATE_ID, sub);

	return {
		h.utility * sub,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
typename TA::UP
O_<TN, TA, TH, TS...>::deepReportUtilize(Control& control) {
	const UP	  h = _headState.deepReportUtilize(control);
	const Utility s = _subStates.wideReportUtilize(control);

	const Utility sub = s / WIDTH;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, INVALID_STATE_ID, sub);

	return {
		h.utility * sub,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
typename TA::Rank
O_<TN, TA, TH, TS...>::deepReportRank(Control& control) {
	return _headState.wrapRank(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
typename TA::Utility
O_<TN, TA, TH, TS...>::deepReportRandomize(Control& control) {
	const Utility h = _headState.wrapUtility(control);
	const Utility s = _subStates.wideReportRandomize(control);

	const Utility sub = s / WIDTH;

	HFSM2_LOG_RANDOM_RESOLUTION(control.context(), HEAD_ID, INVALID_STATE_ID, sub);

	return h * sub;
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepChangeToRequested(PlanControl& control) {
	_subStates.wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepSaveActive(const Registry& registry,
									  WriteStream& stream) const
{
	_subStates.wideSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepSaveResumable(const Registry& registry,
										 WriteStream& stream) const
{
	_subStates.wideSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepLoadRequested(Registry& registry,
										 ReadStream& stream) const
{
	_subStates.wideLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepLoadResumable(Registry& registry,
										 ReadStream& stream) const
{
	_subStates.wideLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepGetNames(const Long parent,
									const RegionType region,
									const Short depth,
									StructureStateInfos& stateInfos) const
{
	_headState.deepGetNames(parent, region,			depth,	   stateInfos);
	_subStates.wideGetNames(stateInfos.count() - 1, depth + 1, stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
