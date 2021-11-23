namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepRegister(Registry& registry,
										const Parent parent) noexcept
{
	registry.orthoParents[ORTHO_INDEX] = parent;
	registry.orthoUnits[ORTHO_INDEX] = Units{ORTHO_UNIT, WIDTH};

	_headState.deepRegister(registry, parent);
	_subStates.wideRegister(registry, ORTHO_ID);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
bool
O_<TN, TA, TH, TS...>::deepForwardEntryGuard(GuardControl& control) noexcept {
	const ProngCBits requested = orthoRequested(static_cast<const GuardControl&>(control));

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested)
		return _subStates.wideForwardEntryGuard(control, requested);
	else
		return _subStates.wideForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
bool
O_<TN, TA, TH, TS...>::deepEntryGuard(GuardControl& control) noexcept {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepEntryGuard(control) ||
		   _subStates.wideEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepEnter(PlanControl& control) noexcept {
	ProngBits requested = orthoRequested(control);
	requested.clear();

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter(control);
	_subStates.wideEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepReenter(PlanControl& control) noexcept {
	ProngBits requested = orthoRequested(control);
	requested.clear();

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepReenter(control);
	_subStates.wideReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
Status
O_<TN, TA, TH, TS...>::deepUpdate(FullControl& control) noexcept {
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

	#if HFSM2_PLANS_AVAILABLE()
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
HFSM2_CONSTEXPR(14)
Status
O_<TN, TA, TH, TS...>::deepReact(FullControl& control,
								 const TEvent& event) noexcept
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

	#if HFSM2_PLANS_AVAILABLE()
		return subStatus && control._planData.planExists.template get<REGION_ID>() ?
			control.updatePlan(_headState, subStatus) : subStatus;
	#else
		return subStatus;
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
bool
O_<TN, TA, TH, TS...>::deepForwardExitGuard(GuardControl& control) noexcept {
	const ProngCBits requested = orthoRequested(static_cast<const GuardControl&>(control));

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested)
		return _subStates.wideForwardExitGuard(control, requested);
	else
		return _subStates.wideForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
bool
O_<TN, TA, TH, TS...>::deepExitGuard(GuardControl& control) noexcept {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepExitGuard(control) ||
		   _subStates.wideExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepExit(PlanControl& control) noexcept {
	_subStates.wideExit(control);
	_headState.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepForwardActive(Control& control,
										 const Request request) noexcept
{
	HFSM2_ASSERT(control._registry.isActive(HEAD_ID));

	const ProngCBits requested = orthoRequested(static_cast<const Control&>(control));
	HFSM2_ASSERT(!!requested);

	_subStates.wideForwardActive(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepForwardRequest(Control& control,
										  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const ProngCBits requested = orthoRequested(static_cast<const Control&>(control));

	if (requested)
		_subStates.wideForwardRequest(control, request);
	else
		deepRequest					 (control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepRequest(Control& control,
								   const Request request) noexcept
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

#if HFSM2_UTILITY_THEORY_AVAILABLE()

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
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepRequestChange(Control& control,
										 const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	_subStates.wideRequestChange(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepRequestRestart(Control& control,
										  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	_subStates.wideRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepRequestResume(Control& control,
										 const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	_subStates.wideRequestResume(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepRequestUtilize(Control& control,
										  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	_subStates.wideRequestUtilize(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepRequestRandomize(Control& control,
											const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	_subStates.wideRequestRandomize(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
O_<TN, TA, TH, TS...>::deepReportChange(Control& control) noexcept {
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
HFSM2_CONSTEXPR(14)
typename TA::UP
O_<TN, TA, TH, TS...>::deepReportUtilize(Control& control) noexcept {
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
HFSM2_CONSTEXPR(14)
typename TA::Rank
O_<TN, TA, TH, TS...>::deepReportRank(Control& control) noexcept {
	return _headState.wrapRank(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::Utility
O_<TN, TA, TH, TS...>::deepReportRandomize(Control& control) noexcept {
	const Utility h = _headState.wrapUtility(control);
	const Utility s = _subStates.wideReportRandomize(control);

	const Utility sub = s / WIDTH;

	HFSM2_LOG_RANDOM_RESOLUTION(control.context(), HEAD_ID, INVALID_STATE_ID, sub);

	return h * sub;
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepChangeToRequested(PlanControl& control) noexcept {
	_subStates.wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepSaveActive(const Registry& registry,
									  WriteStream& stream) const noexcept
{
	_subStates.wideSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepSaveResumable(const Registry& registry,
										 WriteStream& stream) const noexcept
{
	_subStates.wideSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepLoadRequested(Registry& registry,
										 ReadStream& stream) const noexcept
{
	_subStates.wideLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepLoadResumable(Registry& registry,
										 ReadStream& stream) const noexcept
{
	_subStates.wideLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepGetNames(const Long parent,
									const RegionType region,
									const Short depth,
									StructureStateInfos& stateInfos) const noexcept
{
	_headState.deepGetNames(parent, region,			depth,	   stateInfos);
	_subStates.wideGetNames(stateInfos.count() - 1, depth + 1, stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
