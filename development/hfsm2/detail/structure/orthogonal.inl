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
	registry.orthoUnits	 [ORTHO_INDEX] = Units{ORTHO_UNIT, WIDTH};
	registry.regionSizes [REGION_ID]   = REGION_SIZE;

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
O_<TN, TA, TH, TS...>::deepPreUpdate(FullControl& control) noexcept {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const Status h =
		HeadState::deepPreUpdate(control);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePreUpdate(control);

	return h;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
Status
O_<TN, TA, TH, TS...>::deepUpdate(FullControl& control) noexcept {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const Status h =
		HeadState::deepUpdate(control);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::wideUpdate(control);

	return h;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
Status
O_<TN, TA, TH, TS...>::deepPostUpdate(FullControl& control) noexcept {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePostUpdate(control);

	const Status h =
		HeadState::deepPostUpdate(control);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	return h;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
O_<TN, TA, TH, TS...>::deepPreReact(FullControl& control,
									const TEvent& event) noexcept
{
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const Status h =
		HeadState::deepPreReact(control, event);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePreReact(control, event);

	return h;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
O_<TN, TA, TH, TS...>::deepReact(FullControl& control,
								 const TEvent& event) noexcept
{
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const Status h =
		HeadState::deepReact(control, event);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::wideReact(control, event);

	return h;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
O_<TN, TA, TH, TS...>::deepPostReact(FullControl& control,
									 const TEvent& event) noexcept
{
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePostReact(control, event);

	const Status h =
		HeadState::deepPostReact(control, event);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	return h;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepQuery(ConstControl& control,
								 TEvent& event) const noexcept
{
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	SubStates::wideQuery(control, event);
}

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
Status
O_<TN, TA, TH, TS...>::deepUpdatePlans(FullControl& control) noexcept {
	const Status h =	headStatus(control) |
		HeadState::deepUpdatePlans(control);

	const Status s =	 subStatus(control) |
		SubStates::wideUpdatePlans(control);

	if (h)
		return h;
	else {
		if (s.outerTransition)
			return Status{Status::Result::NONE, true};

		ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

		const bool planExists = control._core.planData.planExists.template get<REGION_ID>();

		return s && planExists ?
			control.updatePlan(static_cast<HeadState&>(*this), s) : s;
	}
}

#endif

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
	HFSM2_ASSERT(control._core.registry.isActive(HEAD_ID));

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

template <typename TN, typename TA, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
O_<TN, TA, TH, TS...>::deepRequestSelect(Control& control,
										 const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	SubStates::wideRequestSelect(control, request);
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
