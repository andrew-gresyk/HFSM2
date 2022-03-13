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

	HeadState::deepRegister(registry, parent);
	SubStates::wideRegister(registry, ORTHO_ID);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
bool
O_<TN, TA, TH, TS...>::deepForwardEntryGuard(GuardControl& control) noexcept {
	const ProngCBits requested = orthoRequested(static_cast<const GuardControl&>(control));

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested)
		return SubStates::wideForwardEntryGuard(control, requested);
	else
		return SubStates::wideForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
bool
O_<TN, TA, TH, TS...>::deepEntryGuard(GuardControl& control) noexcept {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return HeadState::deepEntryGuard(control) ||
		   SubStates::wideEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepEnter(PlanControl& control) noexcept {
	ProngBits requested = orthoRequested(control);
	requested.clear();

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HeadState::deepEnter(control);
	SubStates::wideEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepReenter(PlanControl& control) noexcept {
	ProngBits requested = orthoRequested(control);
	requested.clear();

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HeadState::deepReenter(control);
	SubStates::wideReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
Status
O_<TN, TA, TH, TS...>::deepUpdate(FullControl& control) noexcept {
	ScopedRegion outer{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const Status headStatus = HeadState::deepUpdate(control)) {
		ControlLock lock{control};
		SubStates::wideUpdate(control);

		return headStatus;
	} else {
		const Status subStatus = SubStates::wideUpdate(control);

		if (subStatus.outerTransition)
			return Status{Status::Result::NONE, true};

		ScopedRegion inner{control, REGION_ID, HEAD_ID, REGION_SIZE};

	#if HFSM2_PLANS_AVAILABLE()
		return subStatus && control._planData.planExists.template get<REGION_ID>() ?
			control.updatePlan((HeadState&) *this, subStatus) : subStatus;
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

	if (const Status headStatus = HeadState::deepReact(control, event)) {
		ControlLock lock{control};
		SubStates::wideReact(control, event);

		return headStatus;
	} else {
		const Status subStatus = SubStates::wideReact(control, event);

		if (subStatus.outerTransition)
			return Status{Status::Result::NONE, true};

		ScopedRegion inner{control, REGION_ID, HEAD_ID, REGION_SIZE};

	#if HFSM2_PLANS_AVAILABLE()
		return subStatus && control._planData.planExists.template get<REGION_ID>() ?
			control.updatePlan((HeadState&) *this, subStatus) : subStatus;
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
		return SubStates::wideForwardExitGuard(control, requested);
	else
		return SubStates::wideForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
bool
O_<TN, TA, TH, TS...>::deepExitGuard(GuardControl& control) noexcept {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return HeadState::deepExitGuard(control) ||
		   SubStates::wideExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepExit(PlanControl& control) noexcept {
	SubStates::wideExit(control);
	HeadState::deepExit(control);
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

	SubStates::wideForwardActive(control, request, requested);
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
		SubStates::wideForwardRequest(control, request);
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

	SubStates::wideRequestChange(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepRequestRestart(Control& control,
										  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	SubStates::wideRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepRequestResume(Control& control,
										 const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	SubStates::wideRequestResume(control, request);
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

	SubStates::wideRequestUtilize(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepRequestRandomize(Control& control,
											const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	SubStates::wideRequestRandomize(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
O_<TN, TA, TH, TS...>::deepReportChange(Control& control) noexcept {
	const UP	  h = HeadState::deepReportChange(control);
	const Utility s = SubStates::wideReportChange(control);

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
	const UP	  h = HeadState::deepReportUtilize(control);
	const Utility s = SubStates::wideReportUtilize(control);

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
	return HeadState::wrapRank(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::Utility
O_<TN, TA, TH, TS...>::deepReportRandomize(Control& control) noexcept {
	const Utility h = HeadState::wrapUtility(control);
	const Utility s = SubStates::wideReportRandomize(control);

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
	SubStates::wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepSaveActive(const Registry& registry,
									  WriteStream& stream) const noexcept
{
	SubStates::wideSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepSaveResumable(const Registry& registry,
										 WriteStream& stream) const noexcept
{
	SubStates::wideSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepLoadRequested(Registry& registry,
										 ReadStream& stream) const noexcept
{
	SubStates::wideLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepLoadResumable(Registry& registry,
										 ReadStream& stream) const noexcept
{
	SubStates::wideLoadResumable(registry, stream);
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
	HeadState::deepGetNames(parent, region,			depth,	   stateInfos);
	SubStates::wideGetNames(stateInfos.count() - 1, depth + 1, stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
