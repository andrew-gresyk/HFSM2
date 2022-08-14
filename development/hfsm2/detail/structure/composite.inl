namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
Short
C_<TN, TA, SG, TH, TS...>::resolveRandom(Control& control,
										 const Utility(& options)[WIDTH],
										 const Utility sum,
										 const Rank(& ranks)[WIDTH],
										 const Rank top) const noexcept
{
	const Utility random = control._core.rng.next();
	HFSM2_ASSERT(0.0f <= random && random < 1.0f);

	Utility cursor = random * sum;

	for (Short i = 0; i < count<Short>(ranks); ++i)
		if (ranks[i] == top) {
			HFSM2_ASSERT(options[i] >= 0.0f);

			if (cursor >= options[i])
				cursor -= options[i];
			else {
				HFSM2_LOG_RANDOM_RESOLUTION(control.context(), HEAD_ID, i, random);

				return i;
			}
		}

	HFSM2_BREAK();
	return INVALID_SHORT;
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRegister(Registry& registry,
										const Parent parent) noexcept
{
	registry.compoParents[COMPO_INDEX] = parent;
	registry.regionSizes [REGION_ID]   = REGION_SIZE;

	HeadState::deepRegister(registry, parent);
	SubStates::wideRegister(registry, Parent{COMPO_ID});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
bool
C_<TN, TA, SG, TH, TS...>::deepForwardEntryGuard(GuardControl& control) noexcept {
	const Short requested = compoRequested(control);

	const Short active	  = compoActive   (control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested == INVALID_SHORT)
		return SubStates::wideForwardEntryGuard(control, active);
	else
		return SubStates::wideEntryGuard	   (control, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
bool
C_<TN, TA, SG, TH, TS...>::deepEntryGuard(GuardControl& control) noexcept {
	const Short requested = compoRequested(control);
	HFSM2_ASSERT(requested < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return HeadState::deepEntryGuard(control) ||
		   SubStates::wideEntryGuard(control, requested);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepEnter(PlanControl& control) noexcept {
	Short& requested = compoRequested(control);
	Short& active	 = compoActive	 (control);
	Short& resumable = compoResumable(control);

	HFSM2_ASSERT(requested < WIDTH);
	HFSM2_ASSERT(active	  == INVALID_SHORT);

	active	  = requested;

	if (requested == resumable)
		resumable = INVALID_SHORT;

	requested = INVALID_SHORT;

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HeadState::deepEnter(control);
	SubStates::wideEnter(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepReenter(PlanControl& control) noexcept {
	Short& active	 = compoActive   (control);
	Short& resumable = compoResumable(control);
	Short& requested = compoRequested(control);

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
			resumable = INVALID_SHORT;

		SubStates::wideEnter  (control, active);
	}

	requested = INVALID_SHORT;
}

//------------------------------------------------------------------------------
// COMMON

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
Status
C_<TN, TA, SG, TH, TS...>::deepPreUpdate(FullControl& control) noexcept {
	HFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const Status h =
		HeadState::deepPreUpdate(control);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePreUpdate(control, active);

	return h;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
Status
C_<TN, TA, SG, TH, TS...>::deepUpdate(FullControl& control) noexcept {
	HFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const Status h =
		HeadState::deepUpdate(control);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::wideUpdate(control, active);

	return h;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
Status
C_<TN, TA, SG, TH, TS...>::deepPostUpdate(FullControl& control) noexcept {
	HFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePostUpdate(control, active);

	const Status h =
		HeadState::deepPostUpdate(control);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	return h;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
C_<TN, TA, SG, TH, TS...>::deepPreReact(FullControl& control,
										const TEvent& event) noexcept
{
	HFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const Status h =
		HeadState::deepPreReact(control, event);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePreReact(control, event, active);

	return h;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
C_<TN, TA, SG, TH, TS...>::deepReact(FullControl& control,
									 const TEvent& event) noexcept
{
	HFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	const Status h =
		HeadState::deepReact(control, event);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::wideReact(control, event, active);

	return h;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
C_<TN, TA, SG, TH, TS...>::deepPostReact(FullControl& control,
										 const TEvent& event) noexcept
{
	HFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	HFSM2_IF_PLANS(subStatus(control) |=)
		SubStates::widePostReact(control, event, active);

	const Status h =
		HeadState::deepPostReact(control, event);
	HFSM2_IF_PLANS(headStatus(control) |= h);

	return h;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepQuery(ConstControl& control,
									 TEvent& event) const noexcept
{
	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	HeadState::deepQuery(control, event);
	SubStates::wideQuery(control, event, active);
}

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
Status
C_<TN, TA, SG, TH, TS...>::deepUpdatePlans(FullControl& control) noexcept {
	HFSM2_ASSERT(compoRequested(control) == INVALID_SHORT);

	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	const Status h =	headStatus(control) |
		HeadState::deepUpdatePlans(control);

	const Status s =	 subStatus(control) |
		SubStates::wideUpdatePlans(control, active);

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

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
bool
C_<TN, TA, SG, TH, TS...>::deepForwardExitGuard(GuardControl& control) noexcept {
	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (compoRequested(control) == INVALID_SHORT)
		return SubStates::wideForwardExitGuard(control, active);
	else
		return SubStates::wideExitGuard		  (control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
bool
C_<TN, TA, SG, TH, TS...>::deepExitGuard(GuardControl& control) noexcept {
	const Short  active = compoActive(control);
	HFSM2_ASSERT(active < WIDTH);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return HeadState::deepExitGuard(control) ||
		   SubStates::wideExitGuard(control, active);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepExit(PlanControl& control) noexcept {
	Short& active	 = compoActive   (control);
	HFSM2_ASSERT(active < WIDTH);

	Short& resumable = compoResumable(control);

	SubStates::wideExit(control, active);
	HeadState::deepExit(control);

	resumable = active;
	active	  = INVALID_SHORT;

#if HFSM2_PLANS_AVAILABLE()
	Plan plan = control.plan(REGION_ID);
	plan.clear();
#endif
}

// COMMON
//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepForwardActive(Control& control,
											 const Request request) noexcept
{
	HFSM2_ASSERT(control._core.registry.isActive(HEAD_ID));

	const Short requested = compoRequested(control);

	if (requested == INVALID_SHORT) {
		const Short active = compoActive(control);

		SubStates::wideForwardActive (control, request, active);
	} else
		SubStates::wideForwardRequest(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepForwardRequest(Control& control,
											  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const Short requested = compoRequested(control);

	if (requested != INVALID_SHORT)
		SubStates::wideForwardRequest(control, request, requested);
	else
		deepRequest					 (control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
HFSM2_CONSTEXPR(14)
C_<TN, TA, SG, TH, TS...>::deepRequest(Control& control,
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

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestChange(Control& control,
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

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestChangeComposite(Control& control,
													  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Short& requested = compoRequested(control);

	requested = 0;

	SubStates::wideRequestChangeComposite(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestChangeResumable(Control& control,
													  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const Short  resumable = compoResumable(control);
		  Short& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT) ?
		resumable : 0;

	HFSM2_ASSERT(requested < WIDTH);

	SubStates::wideRequestChangeResumable(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestChangeSelectable(Control& control,
													   const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Short& requested = compoRequested(control);
	requested = HeadState::wrapSelect(control);

	HFSM2_ASSERT(requested < WIDTH);

	HFSM2_LOG_SELECT_RESOLUTION(control.context(), HEAD_ID, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestChangeUtilitarian(Control& control,
														const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const UP s = SubStates::wideReportChangeUtilitarian(control);
	HFSM2_ASSERT(s.prong < WIDTH);

	Short& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestChangeRandom(Control& control,
												   const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Rank ranks[WIDTH] = { Rank{} };
	Rank top = SubStates::wideReportRank(control, ranks);

	Utility options[WIDTH] = { Utility{} };
	const UP sum = SubStates::wideReportChangeRandom(control, options, ranks, top);

	Short& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum.utility, ranks, top);
	HFSM2_ASSERT(requested < WIDTH);
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestRestart(Control& control,
											  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Short& requested = compoRequested(control);

	requested = 0;

	SubStates::wideRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestResume(Control& control,
											 const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const Short  resumable = compoResumable(control);
		  Short& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT) ?
		resumable : 0;

	HFSM2_ASSERT(requested < WIDTH);

	SubStates::wideRequestResume(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestSelect(Control& control,
											 const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Short& requested = compoRequested(control);
	requested = HeadState::wrapSelect(control);

	HFSM2_ASSERT(requested < WIDTH);

	HFSM2_LOG_SELECT_RESOLUTION(control.context(), HEAD_ID, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestUtilize(Control& control,
											  const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const UP s = SubStates::wideReportUtilize(control);

	Short& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_ASSERT(s.prong < WIDTH);

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepRequestRandomize(Control& control,
												const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Rank ranks[WIDTH] = { Rank{} };
	Rank top = SubStates::wideReportRank(control, ranks);

	Utility options[WIDTH] = { Utility{} };
	const Utility sum = SubStates::wideReportRandomize(control, options, ranks, top);

	Short& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum, ranks, top);
	HFSM2_ASSERT(requested < WIDTH);
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_UTILITY_THEORY_AVAILABLE()
#if !HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChange(Control& control) noexcept {
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

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChangeComposite(Control& control) noexcept {
	Short& requested = compoRequested(control);
	requested = 0;

	const UP h = HeadState::deepReportChange(control);
	const UP s = SubStates::wideReportChangeComposite(control);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChangeResumable(Control& control) noexcept {
	const Short  resumable = compoResumable(control);
		  Short& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT) ?
		resumable : 0;

	const UP h = HeadState::deepReportChange(control);
	const UP s = SubStates::wideReportChangeResumable(control, requested);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChangeSelectable(Control& control) noexcept {
	const Short  resumable = compoResumable(control);
		  Short& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT) ?
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

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChangeUtilitarian(Control& control) noexcept {
	const UP h = HeadState::deepReportChange(control);
	const UP s = SubStates::wideReportChangeUtilitarian(control);

	Short& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChangeRandom(Control& control) noexcept {
	const UP h = HeadState::deepReportChange(control);

	Rank ranks[WIDTH] = { Rank{} };
	Rank top = SubStates::wideReportRank(control, ranks);

	Utility options[WIDTH] = { Utility{} };
	const UP sum = SubStates::wideReportChangeRandom(control, options, ranks, top);

	Short& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum.utility, ranks, top);
	HFSM2_ASSERT(requested < WIDTH);

	return {
		h.utility * options[requested],
		h.prong
	};
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportUtilize(Control& control) noexcept {
	const UP h = HeadState::deepReportUtilize(control);
	const UP s = SubStates::wideReportUtilize(control);

	Short& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::Rank
C_<TN, TA, SG, TH, TS...>::deepReportRank(Control& control) noexcept {
	return HeadState::wrapRank(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::Utility
C_<TN, TA, SG, TH, TS...>::deepReportRandomize(Control& control) noexcept {
	const Utility h = HeadState::wrapUtility(control);

	Rank ranks[WIDTH] = { Rank{} };
	Rank top = SubStates::wideReportRank(control, ranks);

	Utility options[WIDTH] = { Utility{} };
	const Utility sum = SubStates::wideReportRandomize(control, options, ranks, top);

	Short& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum, ranks, top);
	HFSM2_ASSERT(requested < WIDTH);

	return h * options[requested];
}

#endif

//------------------------------------------------------------------------------
// COMMON

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepChangeToRequested(PlanControl& control) noexcept {
	Short& requested = compoRequested(control);
	Short& active	 = compoActive	 (control);
	Short& resumable = compoResumable(control);

	HFSM2_ASSERT(active < WIDTH);

	if (requested == INVALID_SHORT)
		SubStates::wideChangeToRequested(control, active);
	else if (requested != active) {
		SubStates::wideExit   (control, active);

		HFSM2_ASSERT(requested < WIDTH);

		resumable = active;
		active	  = requested;
		requested = INVALID_SHORT;

		SubStates::wideEnter  (control, active);
	} else if (compoRemain(control)) {
		SubStates::wideExit   (control, active);

		requested = INVALID_SHORT;

		SubStates::wideEnter  (control, active);
	} else {
		requested = INVALID_SHORT;

		SubStates::wideReenter(control, active);
	}
}

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepSaveActive(const Registry& registry,
										  WriteStream& stream) const noexcept
{
	const Short active	  = compoActive   (registry);
	const Short resumable = compoResumable(registry);

	stream.template write<WIDTH_BITS>(active);

	if (resumable != INVALID_SHORT) {
		stream.template write<1>(1);
		stream.template write<WIDTH_BITS>(resumable);
	} else
		stream.template write<1>(0);

	SubStates::wideSaveActive(registry,stream, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepSaveResumable(const Registry& registry,
											 WriteStream& stream) const noexcept
{
	const Short resumable = compoResumable(registry);

	if (resumable != INVALID_SHORT) {
		stream.template write<1>(1);
		stream.template write<WIDTH_BITS>(resumable);
	} else
		stream.template write<1>(0);

	SubStates::wideSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepLoadRequested(Registry& registry,
											 ReadStream& stream) const noexcept
{
	Short& resumable = compoResumable(registry);
	Short& requested = compoRequested(registry);

	requested = stream.template read<WIDTH_BITS>();
	HFSM2_ASSERT(requested < WIDTH);

	if (stream.template read<1>()) {
		resumable = stream.template read<WIDTH_BITS>();
		HFSM2_ASSERT(resumable < WIDTH);
	} else
		resumable = INVALID_SHORT;

	SubStates::wideLoadRequested(registry, stream, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepLoadResumable(Registry& registry,
											 ReadStream& stream) const noexcept
{
	Short& resumable = compoResumable(registry);

	if (stream.template read<1>()) {
		resumable = stream.template read<WIDTH_BITS>();
		HFSM2_ASSERT(resumable < WIDTH);
	} else
		resumable = INVALID_SHORT;

	SubStates::wideLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
HFSM2_CONSTEXPR(14)
void
C_<TN, TA, SG, TH, TS...>::deepGetNames(const Long parent,
										const RegionType regionType,
										const Short depth,
										StructureStateInfos& stateInfos) const noexcept
{
	HeadState::deepGetNames(parent,					regionType,								   depth,	  stateInfos);
	SubStates::wideGetNames(stateInfos.count() - 1, StructureStateInfo::RegionType::COMPOSITE, depth + 1, stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
