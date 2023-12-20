namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepRegister(Registry& registry,
											 const Parent parent) noexcept
{
	registry.compoParents[COMPO_INDEX] = parent;
	registry.regionSizes [REGION_ID]   = REGION_SIZE;

	HeadState::deepRegister(registry, parent);
	SubStates::wideRegister(registry, Parent{COMPO_ID});
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
bool
C_<TN_, TA_, SG_, TH_, TS_...>::deepForwardEntryGuard(GuardControl& control) noexcept {
	const Prong	 requested	= compoRequested(control);

	const Prong	 active		= compoActive   (control);
	HFSM2_ASSERT(active		< WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested == INVALID_PRONG)
		return SubStates::wideForwardEntryGuard(control, active);
	else
		return SubStates::wideEntryGuard	   (control, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
bool
C_<TN_, TA_, SG_, TH_, TS_...>::deepEntryGuard(GuardControl& control) noexcept {
	const Prong  requested = compoRequested(control);
	HFSM2_ASSERT(requested < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return HeadState::deepEntryGuard(control) ||
		   SubStates::wideEntryGuard(control, requested);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepEnter(PlanControl& control) noexcept {
	Prong& requested = compoRequested(control);
	Prong& active	 = compoActive	 (control);
	Prong& resumable = compoResumable(control);

	HFSM2_ASSERT(requested < WIDTH);
	HFSM2_ASSERT(active	  == INVALID_PRONG);

	active	  = requested;

	if (requested == resumable)
		resumable = INVALID_PRONG;

	requested = INVALID_PRONG;

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HeadState::deepEnter(control);
	SubStates::wideEnter(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepReenter(PlanControl& control) noexcept {
	Prong& requested = compoRequested(control);
	Prong& active	 = compoActive   (control);
	Prong& resumable = compoResumable(control);

	HFSM2_ASSERT(active	   < WIDTH &&
				 requested < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HeadState::deepReenter(control);

	if (active == requested)
		SubStates::wideReenter(control, active);
	else {
		SubStates::wideExit	  (control, active);

		active  = requested;

		if (requested == resumable)
			resumable = INVALID_PRONG;

		SubStates::wideEnter  (control, active);
	}

	requested = INVALID_PRONG;
}

//------------------------------------------------------------------------------
// COMMON

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
TaskStatus
C_<TN_, TA_, SG_, TH_, TS_...>::deepPreUpdate(FullControl& control) noexcept {
	HFSM2_ASSERT(compoRequested(control) == INVALID_PRONG);

	const Prong  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const TaskStatus h =
		HeadState::deepPreUpdate(control);
	HFSM2_IF_PLANS(headStatus	(control) |= h);

	HFSM2_IF_PLANS( subStatus	(control) |=)
		SubStates::widePreUpdate(control, active);

	return h;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
TaskStatus
C_<TN_, TA_, SG_, TH_, TS_...>::deepUpdate(FullControl& control) noexcept {
	HFSM2_ASSERT(compoRequested(control) == INVALID_PRONG);

	const Prong  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const TaskStatus h =
		HeadState::deepUpdate(control);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS( subStatus(control) |=)
		SubStates::wideUpdate(control, active);

	return h;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
TaskStatus
C_<TN_, TA_, SG_, TH_, TS_...>::deepPostUpdate(FullControl& control) noexcept {
	HFSM2_ASSERT(compoRequested(control) == INVALID_PRONG);

	const Prong  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HFSM2_IF_PLANS( subStatus	 (control) |=)
		SubStates::widePostUpdate(control, active);

	const TaskStatus h =
		HeadState::deepPostUpdate(control);
	HFSM2_IF_PLANS(headStatus	 (control) |= h);

	return h;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
C_<TN_, TA_, SG_, TH_, TS_...>::deepPreReact(FullControl& control,
											 const TEvent& event) noexcept
{
	HFSM2_ASSERT(compoRequested(control) == INVALID_PRONG);

	const Prong  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const TaskStatus h =
		HeadState::deepPreReact(control, event);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePreReact(control, event, active);

	return h;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
C_<TN_, TA_, SG_, TH_, TS_...>::deepReact(FullControl& control,
										  const TEvent& event) noexcept
{
	HFSM2_ASSERT(compoRequested(control) == INVALID_PRONG);

	const Prong  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const TaskStatus h =
		HeadState::deepReact (control, event);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS( subStatus(control) |=)
		SubStates::wideReact (control, event, active);

	return h;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
C_<TN_, TA_, SG_, TH_, TS_...>::deepPostReact(FullControl& control,
											  const TEvent& event) noexcept
{
	HFSM2_ASSERT(compoRequested(control) == INVALID_PRONG);

	const Prong  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HFSM2_IF_PLANS( subStatus	(control) |=)
		SubStates::widePostReact(control, event, active);

	const TaskStatus h =
		HeadState::deepPostReact(control, event);
	HFSM2_IF_PLANS(headStatus	(control) |= h);

	return h;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepQuery(ConstControl& control,
										  TEvent& event) const noexcept
{
	const Prong  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	HeadState::deepQuery(control, event);
	SubStates::wideQuery(control, event, active);
}

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
TaskStatus
C_<TN_, TA_, SG_, TH_, TS_...>::deepUpdatePlans(FullControl& control) noexcept {
	HFSM2_ASSERT(compoRequested(control) == INVALID_PRONG);

	const Prong  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	const TaskStatus h = headStatus(control) |
		HeadState::deepUpdatePlans (control);

	const TaskStatus s =  subStatus(control) |
		SubStates::wideUpdatePlans (control, active);

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

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
bool
C_<TN_, TA_, SG_, TH_, TS_...>::deepForwardExitGuard(GuardControl& control) noexcept {
	const Prong  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (compoRequested(control) == INVALID_PRONG)
		return SubStates::wideForwardExitGuard(control, active);
	else
		return SubStates::wideExitGuard		  (control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
bool
C_<TN_, TA_, SG_, TH_, TS_...>::deepExitGuard(GuardControl& control) noexcept {
	const Prong  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return HeadState::deepExitGuard(control) ||
		   SubStates::wideExitGuard(control, active);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepExit(PlanControl& control) noexcept {
	Prong& active	 = compoActive   (control);
	HFSM2_ASSERT(active < WIDTH);

	Prong& resumable = compoResumable(control);

	SubStates::wideExit(control, active);
	HeadState::deepExit(control);

	resumable = active;
	active	  = INVALID_PRONG;

#if HFSM2_PLANS_AVAILABLE()
	Plan plan = control.plan(REGION_ID);
	plan.clear();
#endif
}

// COMMON
//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepForwardActive(Control& control,
												  const Request request) noexcept
{
	HFSM2_ASSERT(control._core.registry.isActive(HEAD_ID));

	const Prong requested = compoRequested(control);

	if (requested == INVALID_SHORT) {
		const Prong active = compoActive(control);

		SubStates::wideForwardActive (control, request, active);
	} else
		SubStates::wideForwardRequest(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepForwardRequest(Control& control,
												   const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const Prong requested = compoRequested(control);

	if (requested != INVALID_PRONG)
		SubStates::wideForwardRequest(control, request, requested);
	else
		deepRequest					 (control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
void
HFSM2_CONSTEXPR(14)
C_<TN_, TA_, SG_, TH_, TS_...>::deepRequest(Control& control,
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

#if !HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepRequestChange(Control& control,
												  const Request request) noexcept
{
	switch (STRATEGY) {
	case Strategy::Composite:
		deepRequestChangeComposite  (control, request);
		break;

	case Strategy::Resumable:
		deepRequestChangeResumable  (control, request);
		break;

	case Strategy::Selectable:
		deepRequestChangeSelectable	(control, request);
		break;

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	case Strategy::Utilitarian:
		deepRequestChangeUtilitarian(control, request);
		break;

	case Strategy::RandomUtil:
		deepRequestChangeRandom		(control, request);
		break;

#endif

	default:
		HFSM2_BREAK();
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepRequestChangeComposite(Control& control,
														   const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Prong& requested = compoRequested(control);

	requested = 0;

	SubStates::wideRequestChangeComposite(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepRequestChangeResumable(Control& control,
														   const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

		  Prong& requested = compoRequested(control);
	const Prong  resumable = compoResumable(control);

	requested = (resumable != INVALID_PRONG) ?
		resumable : 0;

	HFSM2_ASSERT(requested < WIDTH);

	SubStates::wideRequestChangeResumable(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepRequestChangeSelectable(Control& control,
															const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Prong& requested = compoRequested(control);
	requested = HeadState::wrapSelect(control);

	HFSM2_ASSERT(requested < WIDTH);

	HFSM2_LOG_SELECT_RESOLUTION(control.context(), HEAD_ID, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepRequestChangeUtilitarian(Control& control,
															 const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const UP s = SubStates::wideReportChangeUtilitarian(control);
	HFSM2_ASSERT(s.prong < WIDTH);

	Prong& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepRequestChangeRandom(Control& control,
														const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Rank ranks[WIDTH] = { Rank{} };
	Rank top = SubStates::wideReportRank(control, ranks);

	Utility utilities[WIDTH] = { Utility{} };
	const UP sum = SubStates::wideReportChangeRandom(control, utilities, ranks, top);

	Prong& requested = compoRequested(control);
	requested = resolveRandom(control, utilities, sum.utility, ranks, top);
	HFSM2_ASSERT(requested < WIDTH);
}

#endif

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepRequestRestart(Control& control,
												   const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Prong& requested = compoRequested(control);

	requested = 0;

	SubStates::wideRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepRequestResume(Control& control,
												  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

		  Prong& requested = compoRequested(control);
	const Prong  resumable = compoResumable(control);

	requested = (resumable != INVALID_PRONG) ?
		resumable : 0;

	HFSM2_ASSERT(requested < WIDTH);

	SubStates::wideRequestResume(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepRequestSelect(Control& control,
												  const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Prong& requested = compoRequested(control);
	requested = HeadState::wrapSelect(control);

	HFSM2_ASSERT(requested < WIDTH);

	HFSM2_LOG_SELECT_RESOLUTION(control.context(), HEAD_ID, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepRequestUtilize(Control& control,
												   const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const UP s = SubStates::wideReportUtilize(control);

	Prong& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_ASSERT(s.prong < WIDTH);

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepRequestRandomize(Control& control,
													 const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Rank ranks[WIDTH] = { Rank{} };
	Rank top = SubStates::wideReportRank(control, ranks);

	Utility utilities[WIDTH] = { Utility{} };
	const Utility sum = SubStates::wideReportRandomize(control, utilities, ranks, top);

	Prong& requested = compoRequested(control);
	requested = resolveRandom(control, utilities, sum, ranks, top);
	HFSM2_ASSERT(requested < WIDTH);
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_UTILITY_THEORY_AVAILABLE()
#if !HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::UP
C_<TN_, TA_, SG_, TH_, TS_...>::deepReportChange(Control& control) noexcept {
	switch (STRATEGY) {
	case Strategy::Composite:
		return deepReportChangeComposite  (control);

	case Strategy::Resumable:
		return deepReportChangeResumable  (control);

	case Strategy::Selectable:
		return deepReportChangeSelectable (control);

	case Strategy::Utilitarian:
		return deepReportChangeUtilitarian(control);

	case Strategy::RandomUtil:
		return deepReportChangeRandom	  (control);

	default:
		HFSM2_BREAK();
		return UP{};
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::UP
C_<TN_, TA_, SG_, TH_, TS_...>::deepReportChangeComposite(Control& control) noexcept {
	Prong& requested = compoRequested(control);
	requested = 0;

	const UP h = HeadState::deepReportChange(control);
	const UP s = SubStates::wideReportChangeComposite(control);

	return {
		h.utility * s.utility,
		h.prong
	};
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
Short
C_<TN_, TA_, SG_, TH_, TS_...>::resolveRandom(Control& control,
											  const Utilities& utilities,
											  const Utility sum,
											  const Ranks& ranks,
											  const Rank top) const noexcept
{
	const Utility random = control._core.rng.next();
	HFSM2_ASSERT(0.0f <= random && random < 1.0f);

	Utility cursor = random * sum;

	for (Prong i = 0; i < count<Prong>(ranks); ++i)
		if (ranks[i] == top) {
			HFSM2_ASSERT(utilities[i] >= 0.0f);

			if (cursor >= utilities[i])
				cursor -= utilities[i];
			else {
				HFSM2_LOG_RANDOM_RESOLUTION(control.context(), HEAD_ID, i, random);

				return i;
			}
		}

	HFSM2_BREAK();
	return INVALID_PRONG;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::UP
C_<TN_, TA_, SG_, TH_, TS_...>::deepReportChangeResumable(Control& control) noexcept {
	const Prong  resumable = compoResumable(control);
		  Prong& requested = compoRequested(control);

	requested = (resumable != INVALID_PRONG) ?
		resumable : 0;

	const UP h = HeadState::deepReportChange(control);
	const UP s = SubStates::wideReportChangeResumable(control, requested);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::UP
C_<TN_, TA_, SG_, TH_, TS_...>::deepReportChangeSelectable(Control& control) noexcept {
	const Prong  resumable = compoResumable(control);
		  Prong& requested = compoRequested(control);

	requested = (resumable != INVALID_PRONG) ?
		resumable : 0;

	HFSM2_ASSERT(requested < WIDTH);

	const UP h = HeadState::deepReportChange		  (control);
	const UP s = SubStates::wideReportChangeSelectable(control, requested);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::UP
C_<TN_, TA_, SG_, TH_, TS_...>::deepReportChangeUtilitarian(Control& control) noexcept {
	const UP h = HeadState::deepReportChange(control);
	const UP s = SubStates::wideReportChangeUtilitarian(control);

	Prong& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::UP
C_<TN_, TA_, SG_, TH_, TS_...>::deepReportChangeRandom(Control& control) noexcept {
	const UP h = HeadState::deepReportChange(control);

	Rank ranks[WIDTH] = { Rank{} };
	Rank top = SubStates::wideReportRank(control, ranks);

	Utility utilities[WIDTH] = { Utility{} };
	const UP sum = SubStates::wideReportChangeRandom(control, utilities, ranks, top);

	Prong& requested = compoRequested(control);
	requested = resolveRandom(control, utilities, sum.utility, ranks, top);
	HFSM2_ASSERT(requested < WIDTH);

	return {
		h.utility * utilities[requested],
		h.prong
	};
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::UP
C_<TN_, TA_, SG_, TH_, TS_...>::deepReportUtilize(Control& control) noexcept {
	const UP h = HeadState::deepReportUtilize(control);
	const UP s = SubStates::wideReportUtilize(control);

	Prong& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::Rank
C_<TN_, TA_, SG_, TH_, TS_...>::deepReportRank(Control& control) noexcept {
	return HeadState::wrapRank(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::Utility
C_<TN_, TA_, SG_, TH_, TS_...>::deepReportRandomize(Control& control) noexcept {
	const Utility h = HeadState::wrapUtility(control);

	Rank ranks[WIDTH] = { Rank{} };
	Rank top = SubStates::wideReportRank(control, ranks);

	Utility utilities[WIDTH] = { Utility{} };
	const Utility sum = SubStates::wideReportRandomize(control, utilities, ranks, top);

	Prong& requested = compoRequested(control);
	requested = resolveRandom(control, utilities, sum, ranks, top);
	HFSM2_ASSERT(requested < WIDTH);

	return h * utilities[requested];
}

#endif

//------------------------------------------------------------------------------
// COMMON

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepChangeToRequested(PlanControl& control) noexcept {
	Prong& requested = compoRequested(control);
	Prong& active	 = compoActive	 (control);
	Prong& resumable = compoResumable(control);

	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested == INVALID_PRONG)
		SubStates::wideChangeToRequested(control, active);
	else if (requested != active) {
		SubStates::wideExit	  (control, active);

		HFSM2_ASSERT(requested < WIDTH);

		resumable = active;
		active	  = requested;
		requested = INVALID_PRONG;

		SubStates::wideEnter  (control, active);
	} else if (compoRemain(control)) {
		SubStates::wideExit   (control, active);

		requested = INVALID_PRONG;

		SubStates::wideEnter  (control, active);
	} else {
		requested = INVALID_PRONG;

		// reconstruction done in S_::reenter()
		SubStates::wideReenter(control, active);
	}
}

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepSaveActive(const Registry& registry,
											   WriteStream& stream) const noexcept
{
	const Prong active	  = compoActive   (registry);
	const Prong resumable = compoResumable(registry);

	stream.template write<WIDTH_BITS>(active);

	if (resumable != INVALID_PRONG) {
		stream.template write<1>(1);
		stream.template write<WIDTH_BITS>(resumable);
	} else
		stream.template write<1>(0);

	SubStates::wideSaveActive(registry,stream, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepSaveResumable(const Registry& registry,
												  WriteStream& stream) const noexcept
{
	const Prong resumable = compoResumable(registry);

	if (resumable != INVALID_PRONG) {
		stream.template write<1>(1);
		stream.template write<WIDTH_BITS>(resumable);
	} else
		stream.template write<1>(0);

	SubStates::wideSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepLoadRequested(Registry& registry,
												  ReadStream& stream) const noexcept
{
	Prong& resumable = compoResumable(registry);
	Prong& requested = compoRequested(registry);

	requested = stream.template read<WIDTH_BITS>();
	HFSM2_ASSERT(requested < WIDTH);

	if (stream.template read<1>()) {
		resumable = stream.template read<WIDTH_BITS>();
		HFSM2_ASSERT(resumable < WIDTH);
	} else
		resumable = INVALID_PRONG;

	SubStates::wideLoadRequested(registry, stream, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepLoadResumable(Registry& registry,
												  ReadStream& stream) const noexcept
{
	Prong& resumable = compoResumable(registry);

	if (stream.template read<1>()) {
		resumable = stream.template read<WIDTH_BITS>();
		HFSM2_ASSERT(resumable < WIDTH);
	} else
		resumable = INVALID_PRONG;

	SubStates::wideLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN_, typename TA_, Strategy SG_, typename TH_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
C_<TN_, TA_, SG_, TH_, TS_...>::deepGetNames(const Long parent,
											 const RegionType regionType,
											 const Short depth,
											 StructureStateInfos& stateInfos) const noexcept
{
	HeadState::deepGetNames(parent,					regionType,			   depth,	  stateInfos);
	SubStates::wideGetNames(stateInfos.count() - 1, RegionType::COMPOSITE, depth + 1, stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
