namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepRegister(Registry& registry,
										const Parent parent) noexcept
{
	registry.orthoParents[ORTHO_INDEX] = parent;
	registry.orthoUnits	 [ORTHO_INDEX] = Units{ORTHO_UNIT, WIDTH};
	registry.regionHeads [REGION_ID]   = HEAD_ID;
	registry.regionSizes [REGION_ID]   = REGION_SIZE;

	HeadState::deepRegister(registry, parent);
	SubStates::wideRegister(registry, ORTHO_ID);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
bool
O_<TN_, TA_, TH_, TS_...>::deepForwardEntryGuard(GuardControl& control) noexcept {
	const ProngCBits requested = orthoRequested(static_cast<const GuardControl&>(control));

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested)
		return SubStates::wideForwardEntryGuard(control, requested);
	else
		return SubStates::wideForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
bool
O_<TN_, TA_, TH_, TS_...>::deepEntryGuard(GuardControl& control) noexcept {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return HeadState::deepEntryGuard(control) &&
		   SubStates::wideEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepEnter(PlanControl& control) noexcept {
	ProngBits requested = orthoRequested(control);
	requested.clear();

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HeadState::deepEnter(control);
	SubStates::wideEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepReenter(PlanControl& control) noexcept {
	ProngBits requested = orthoRequested(control);
	requested.clear();

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HeadState::deepReenter(control);
	SubStates::wideReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
TaskStatus
O_<TN_, TA_, TH_, TS_...>::deepPreUpdate(FullControl& control) noexcept {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const TaskStatus h =
		HeadState::deepPreUpdate(control);
	HFSM2_IF_PLANS(headStatus	(control) |= h);

	HFSM2_IF_PLANS(subStatus	(control) |=)
		SubStates::widePreUpdate(control);

	return h;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
TaskStatus
O_<TN_, TA_, TH_, TS_...>::deepUpdate(FullControl& control) noexcept {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const TaskStatus h =
		HeadState::deepUpdate(control);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS(subStatus (control) |=)
		SubStates::wideUpdate(control);

	return h;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
TaskStatus
O_<TN_, TA_, TH_, TS_...>::deepPostUpdate(FullControl& control) noexcept {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HFSM2_IF_PLANS(subStatus	 (control) |=)
		SubStates::widePostUpdate(control);

	const TaskStatus h =
		HeadState::deepPostUpdate(control);
	HFSM2_IF_PLANS(headStatus	 (control) |= h);

	return h;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_, typename... TS_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
O_<TN_, TA_, TH_, TS_...>::deepPreReact(EventControl& control,
										const TEvent& event) noexcept
{
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return PreReactWrapper::execute(*this, control, event);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_, typename... TS_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
O_<TN_, TA_, TH_, TS_...>::deepReact(EventControl& control,
									 const TEvent& event) noexcept
{
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return ReactWrapper::execute(*this, control, event);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_, typename... TS_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
O_<TN_, TA_, TH_, TS_...>::deepPostReact(EventControl& control,
										 const TEvent& event) noexcept
{
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return PostReactWrapper::execute(*this, control, event);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_, typename... TS_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepQuery(ConstControl& control,
									 TEvent& event) const noexcept
{
	ScopedCRegion region{control, REGION_ID};

	QueryWrapper::execute(*this, control, event);
}

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
TaskStatus
O_<TN_, TA_, TH_, TS_...>::deepUpdatePlans(FullControl& control) noexcept {
	const TaskStatus h = headStatus(control) |
		HeadState::deepUpdatePlans (control);

	const TaskStatus s =  subStatus(control) |
		SubStates::wideUpdatePlans (control);

	if (h)
		return h;
	else {
		if (s.outerTransition)
			return TaskStatus{TaskStatus::NONE, true};

		ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

		const bool planExists = control._core.planData.planExists.template get<REGION_ID>();

		return s && planExists ?
			control.updatePlan(static_cast<HeadState&>(*this), s) : s;
	}
}

#endif

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
bool
O_<TN_, TA_, TH_, TS_...>::deepForwardExitGuard(GuardControl& control) noexcept {
	const ProngCBits requested = orthoRequested(static_cast<const GuardControl&>(control));

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested)
		return SubStates::wideForwardExitGuard(control, requested);
	else
		return SubStates::wideForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
bool
O_<TN_, TA_, TH_, TS_...>::deepExitGuard(GuardControl& control) noexcept {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return SubStates::wideExitGuard(control) &&
		   HeadState::deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepExit(PlanControl& control) noexcept {
	SubStates::wideExit(control);
	HeadState::deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepForwardActive(Control& control,
											 const Request request) noexcept
{
	HFSM2_ASSERT(control._core.registry.isActive(HEAD_ID));

	const ProngCBits requested = orthoRequested(static_cast<const Control&>(control));
	HFSM2_ASSERT(!!requested);

	SubStates::wideForwardActive(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepForwardRequest(Control& control,
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

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepRequest(Control& control,
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

	case TransitionType::SELECT:
		deepRequestSelect	(control, request);
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

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepRequestChange(Control& control,
											 const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	SubStates::wideRequestChange(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepRequestRestart(Control& control,
											  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	SubStates::wideRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepRequestResume(Control& control,
											 const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	SubStates::wideRequestResume(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepRequestSelect(Control& control,
											 const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	SubStates::wideRequestSelect(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepRequestUtilize(Control& control,
											  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	SubStates::wideRequestUtilize(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepRequestRandomize(Control& control,
												const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	SubStates::wideRequestRandomize(control, request);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::UP
O_<TN_, TA_, TH_, TS_...>::deepReportChange(Control& control) noexcept {
	const UP	  h = HeadState::deepReportChange(control);
	const Utility s = SubStates::wideReportChange(control);

	const Utility sub = s / WIDTH;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, INVALID_SHORT, sub);

	return {
		h.utility * sub,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::UP
O_<TN_, TA_, TH_, TS_...>::deepReportUtilize(Control& control) noexcept {
	const UP	  h = HeadState::deepReportUtilize(control);
	const Utility s = SubStates::wideReportUtilize(control);

	const Utility sub = s / WIDTH;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, INVALID_SHORT, sub);

	return {
		h.utility * sub,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::Rank
O_<TN_, TA_, TH_, TS_...>::deepReportRank(Control& control) noexcept {
	return HeadState::wrapRank(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::Utility
O_<TN_, TA_, TH_, TS_...>::deepReportRandomize(Control& control) noexcept {
	const Utility h = HeadState::wrapUtility(control);
	const Utility s = SubStates::wideReportRandomize(control);

	const Utility sub = s / WIDTH;

	HFSM2_LOG_RANDOM_RESOLUTION(control.context(), HEAD_ID, INVALID_SHORT, sub);

	return h * sub;
}

#endif

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepChangeToRequested(PlanControl& control) noexcept {
	SubStates::wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepSaveActive(const Registry& registry,
										  WriteStream& stream) const noexcept
{
	SubStates::wideSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepSaveResumable(const Registry& registry,
											 WriteStream& stream) const noexcept
{
	SubStates::wideSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepLoadRequested(Registry& registry,
											 ReadStream& stream) const noexcept
{
	SubStates::wideLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepLoadResumable(Registry& registry,
											 ReadStream& stream) const noexcept
{
	SubStates::wideLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN_, typename TA_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
O_<TN_, TA_, TH_, TS_...>::deepGetNames(const Long parent,
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
