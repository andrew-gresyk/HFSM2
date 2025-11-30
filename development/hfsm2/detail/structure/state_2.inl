namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, EmptyT<TA_>>::deepRegister(Registry& registry,
										const Parent parent) noexcept
{
	registry.stateParents[STATE_ID] = parent;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
Prong
S_<TN_, TA_, EmptyT<TA_>>::wrapSelect(Control& HFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	HFSM2_LOG_STATE_METHOD(&Empty::select,
						   Method::SELECT);

	return INVALID_PRONG;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA_, EmptyT<TA_>>::Rank
S_<TN_, TA_, EmptyT<TA_>>::wrapRank(Control& HFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	HFSM2_LOG_STATE_METHOD(&Empty::rank,
						   Method::RANK);

	return Rank{};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA_, EmptyT<TA_>>::Utility
S_<TN_, TA_, EmptyT<TA_>>::wrapUtility(Control& HFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	HFSM2_LOG_STATE_METHOD(&Empty::utility,
						   Method::UTILITY);

	return Utility{};
}

#endif

//------------------------------------------------------------------------------
// COMMON

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
bool
S_<TN_, TA_, EmptyT<TA_>>::deepEntryGuard(GuardControl& HFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	HFSM2_LOG_STATE_METHOD(&Empty::entryGuard,
						   Method::ENTRY_GUARD);

	return true;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, EmptyT<TA_>>::deepEnter(PlanControl& HFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	HFSM2_LOG_STATE_METHOD(&Empty::enter,
						   Method::ENTER);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, EmptyT<TA_>>::deepReenter(PlanControl& HFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	HFSM2_LOG_STATE_METHOD(&Empty::reenter,
						   Method::REENTER);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
TaskStatus
S_<TN_, TA_, EmptyT<TA_>>::deepPreUpdate(FullControl& HFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	HFSM2_LOG_STATE_METHOD(&Empty::preUpdate,
						   Method::PRE_UPDATE);

	return TaskStatus{};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
TaskStatus
S_<TN_, TA_, EmptyT<TA_>>::deepUpdate(FullControl& HFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	HFSM2_LOG_STATE_METHOD(&Empty::update,
						   Method::UPDATE);

	return TaskStatus{};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
TaskStatus
S_<TN_, TA_, EmptyT<TA_>>::deepPostUpdate(FullControl& HFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	HFSM2_LOG_STATE_METHOD(&Empty::postUpdate,
						   Method::POST_UPDATE);

	return TaskStatus{};
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
S_<TN_, TA_, EmptyT<TA_>>::deepPreReact(EventControl& HFSM2_IF_LOG_STATE_METHOD(control),
										const TEvent& HFSM2_UNUSED(event)) noexcept
{
	HFSM2_LOG_STATE_METHOD(static_cast<void (Empty::*)(const TEvent&, EventControl&)>(&Empty::preReact),
						   Method::PRE_REACT);

	return TaskStatus{};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
S_<TN_, TA_, EmptyT<TA_>>::deepReact(EventControl& HFSM2_IF_LOG_STATE_METHOD(control),
									 const TEvent& HFSM2_UNUSED(event)) noexcept
{
	HFSM2_LOG_STATE_METHOD(static_cast<void (Empty::*)(const TEvent&, EventControl&)>(&Empty::react),
						   Method::REACT);

	return TaskStatus{};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
S_<TN_, TA_, EmptyT<TA_>>::deepPostReact(EventControl& HFSM2_IF_LOG_STATE_METHOD(control),
										 const TEvent& HFSM2_UNUSED(event)) noexcept
{
	HFSM2_LOG_STATE_METHOD(static_cast<void (Empty::*)(const TEvent&, EventControl&)>(&Empty::postReact),
						   Method::POST_REACT);

	return TaskStatus{};
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, EmptyT<TA_>>::deepQuery(ConstControl& HFSM2_IF_LOG_STATE_METHOD(control),
									 TEvent& HFSM2_UNUSED(event)) const noexcept
{
	HFSM2_LOG_STATE_METHOD(static_cast<void (Empty::*)(TEvent&, ConstControl&) const>(&Empty::query),
						   Method::QUERY);
}

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
TaskStatus
S_<TN_, TA_, EmptyT<TA_>>::deepUpdatePlans(FullControl& control) noexcept {
	if (control._core.planData.tasksFailures .get(STATE_ID))
		return TaskStatus{TaskStatus::FAILURE};
	else
	if (control._core.planData.tasksSuccesses.get(STATE_ID))
		return TaskStatus{TaskStatus::SUCCESS};
	else
		return TaskStatus{};
}

#endif

//------------------------------------------------------------------------------

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
bool
S_<TN_, TA_, EmptyT<TA_>>::deepExitGuard(GuardControl& HFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	HFSM2_LOG_STATE_METHOD(&Empty::exitGuard,
						   Method::EXIT_GUARD);

	return true;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, EmptyT<TA_>>::deepExit(PlanControl& HFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	HFSM2_LOG_STATE_METHOD(&Empty::exit,
						   Method::EXIT);
}

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, EmptyT<TA_>>::wrapPlanSucceeded(FullControl& HFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	HFSM2_LOG_STATE_METHOD(&Empty::planSucceeded,
						   Method::PLAN_SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, EmptyT<TA_>>::wrapPlanFailed(FullControl& HFSM2_IF_LOG_STATE_METHOD(control)) noexcept {
	HFSM2_LOG_STATE_METHOD(&Empty::planFailed,
						   Method::PLAN_FAILED);
}

#endif

//------------------------------------------------------------------------------

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, EmptyT<TA_>>::deepForwardRequest(Control& HFSM2_IF_TRANSITION_HISTORY(control),
											  const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, EmptyT<TA_>>::deepRequestChange(Control& HFSM2_IF_TRANSITION_HISTORY(control),
											 const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, EmptyT<TA_>>::deepRequestRestart(Control& HFSM2_IF_TRANSITION_HISTORY(control),
											  const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, EmptyT<TA_>>::deepRequestResume(Control& HFSM2_IF_TRANSITION_HISTORY(control),
											 const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, EmptyT<TA_>>::deepRequestSelect(Control& HFSM2_IF_TRANSITION_HISTORY(control),
											 const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, EmptyT<TA_>>::deepRequestUtilize(Control& HFSM2_IF_TRANSITION_HISTORY(control),
											  const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, EmptyT<TA_>>::deepRequestRandomize(Control& HFSM2_IF_TRANSITION_HISTORY(control),
												const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA_, EmptyT<TA_>>::UP
S_<TN_, TA_, EmptyT<TA_>>::deepReportChange(Control& control) noexcept {
	const Parent parent = stateParent(control);

	return {Utility{}, parent.prong};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA_, EmptyT<TA_>>::UP
S_<TN_, TA_, EmptyT<TA_>>::deepReportUtilize(Control& control) noexcept {
	const Parent parent  = stateParent(control);

	return {Utility{}, parent.prong};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA_, EmptyT<TA_>>::Rank
S_<TN_, TA_, EmptyT<TA_>>::deepReportRank(Control& HFSM2_UNUSED(control)) noexcept {
	return Rank{};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA_, EmptyT<TA_>>::Utility
S_<TN_, TA_, EmptyT<TA_>>::deepReportRandomize(Control& HFSM2_UNUSED(control)) noexcept {
	return Utility{};
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(NO)
const char*
S_<TN_, TA_, EmptyT<TA_>>::name() noexcept {
	return "";
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, EmptyT<TA_>>::deepGetNames(const Long parent,
										const RegionType region,
										const Short depth,
										StructureStateInfos& stateInfos) const noexcept
{
	stateInfos.emplace(StructureStateInfo{parent, region, depth, name()});
}

#endif

///////////////////////////////////////////////////////////////////////////////

}
}
