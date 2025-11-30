namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, TH_>::deepRegister(Registry& registry,
								const Parent parent) noexcept
{
	constexpr StateID HEAD_ID = index<StateList, TH_>();
	static_assert(STATE_ID == HEAD_ID, "");

	registry.stateParents[STATE_ID] = parent;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
Prong
S_<TN_, TA_, TH_>::wrapSelect(Control& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::select,
						   Method::SELECT);

	return Head::select(static_cast<const Control&>(control));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA_, TH_>::Rank
S_<TN_, TA_, TH_>::wrapRank(Control& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::rank,
						   Method::RANK);

	return Head::rank(static_cast<const Control&>(control));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA_, TH_>::Utility
S_<TN_, TA_, TH_>::wrapUtility(Control& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::utility,
						   Method::UTILITY);

	return Head::utility(static_cast<const Control&>(control));
}

#endif

//------------------------------------------------------------------------------
// COMMON

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
bool
S_<TN_, TA_, TH_>::deepEntryGuard(GuardControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::entryGuard,
						   Method::ENTRY_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	Head::wideEntryGuard(control);
	Head::	  entryGuard(control);

	return cancelledBefore || !control._cancelled;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, TH_>::deepEnter(PlanControl& control) noexcept {
	HFSM2_IF_PLANS(control._core.planData.verifyEmptyStatus(STATE_ID));

	HFSM2_LOG_STATE_METHOD(&Head::enter,
						   Method::ENTER);

	ScopedOrigin origin{control, STATE_ID};

	Head::wideEnter(control);
	Head::	  enter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, TH_>::deepReenter(PlanControl& control) noexcept {
	HFSM2_IF_PLANS(control._core.planData.verifyEmptyStatus(STATE_ID));

	HFSM2_LOG_STATE_METHOD(&Head::reenter,
						   Method::REENTER);

	ScopedOrigin origin{control, STATE_ID};

	Head::wideReenter(control);
	Head::	  reenter(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
TaskStatus
S_<TN_, TA_, TH_>::deepPreUpdate(FullControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::preUpdate,
						   Method::PRE_UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	Head::widePreUpdate(control);
	Head::	  preUpdate(control);

	return control._taskStatus;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
TaskStatus
S_<TN_, TA_, TH_>::deepUpdate(FullControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::update,
						   Method::UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	Head::wideUpdate(control);
	Head::	  update(control);

	return control._taskStatus;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
TaskStatus
S_<TN_, TA_, TH_>::deepPostUpdate(FullControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::postUpdate,
						   Method::POST_UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	Head::	  postUpdate(control);
	Head::widePostUpdate(control);

	return control._taskStatus;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
S_<TN_, TA_, TH_>::deepPreReact(EventControl& control,
								const TEvent& event) noexcept
{
	// If you see `error: 'static_cast': cannot convert from `...` to
	//   `void (__cdecl Class::* )(const TEvent &,_::EventControlT<> &)`
	//   add `using FSM::State::preReact;` within state declaration
	auto method = static_cast<void (Head::*)(const TEvent&, EventControl&)>(&Head::preReact);

	HFSM2_LOG_STATE_METHOD(method,
						   Method::PRE_REACT);

	ScopedOrigin origin{control, STATE_ID};

	Head::widePreReact(event, control);
	(this->*method) (event, control);

	return control._taskStatus;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
S_<TN_, TA_, TH_>::deepReact(EventControl& control,
							 const TEvent& event) noexcept
{
	// If you see `error: 'static_cast': cannot convert from `...` to
	//   `void (__cdecl Class::* )(const TEvent &,_::EventControlT<> &)`
	//   add `using FSM::State::react;` within state declaration
	auto method = static_cast<void (Head::*)(const TEvent&, EventControl&)>(&Head::react);

	HFSM2_LOG_STATE_METHOD(method,
						   Method::REACT);

	ScopedOrigin origin{control, STATE_ID};

	Head::wideReact(event, control);
	(this->*method)(event, control);

	return control._taskStatus;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
S_<TN_, TA_, TH_>::deepPostReact(EventControl& control,
								 const TEvent& event) noexcept
{
	// If you see `error: 'static_cast': cannot convert from `...` to
	//   `void (__cdecl Class::* )(const TEvent &,_::EventControlT<> &)`
	//   add `using FSM::State::postReact;` within state declaration
	auto method = static_cast<void (Head::*)(const TEvent&, EventControl&)>(&Head::postReact);

	HFSM2_LOG_STATE_METHOD(method,
						   Method::POST_REACT);

	ScopedOrigin origin{control, STATE_ID};

	(this->*method)	   (event, control);
	Head::widePostReact(event, control);

	return control._taskStatus;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, TH_>::deepQuery(ConstControl& control,
							 TEvent&  event) const noexcept
{
	// If you see `error: 'static_cast': cannot convert from `...` to
	//   `void (__cdecl Class::* )(const TEvent &,_::ConstControlT<> &)`
	//   add `using FSM::State::query;` within state declaration
	auto method = static_cast<void (Head::*)(TEvent&, ConstControl&) const>(&Head::query);

	HFSM2_LOG_STATE_METHOD(method,
						   Method::QUERY);

	ScopedCOrigin origin{control, STATE_ID};

	(this->*method)(event, control);
	Head::wideQuery(event, control);
}

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
TaskStatus
S_<TN_, TA_, TH_>::deepUpdatePlans(FullControl& control) noexcept {
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

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
bool
S_<TN_, TA_, TH_>::deepExitGuard(GuardControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::exitGuard,
						   Method::EXIT_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	Head::wideExitGuard(control);
	Head::	  exitGuard(control);

	return cancelledBefore || !control._cancelled;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, TH_>::deepExit(PlanControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::exit,
						   Method::EXIT);

	ScopedOrigin origin{control, STATE_ID};

	// if you see..
	// VS	 - error C2039:  'exit': is not a member of 'Blah'
	// Clang - error : no member named 'exit' in 'Blah'
	//
	// .. inherit state 'Blah' from hfsm2::Machine::Instance::State
	Head::	  exit(control);
	Head::wideExit(control);

	HFSM2_IF_PLANS(control._core.planData.clearTaskStatus(STATE_ID));
}

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, TH_>::wrapPlanSucceeded(FullControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::planSucceeded,
						   Method::PLAN_SUCCEEDED);

	ScopedOrigin origin{control, STATE_ID};

	Head::planSucceeded(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, TH_>::wrapPlanFailed(FullControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::planFailed,
						   Method::PLAN_FAILED);

	ScopedOrigin origin{control, STATE_ID};

	Head::planFailed(control);
}

#endif

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, TH_>::deepForwardRequest(Control& HFSM2_IF_TRANSITION_HISTORY(control),
									  const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, TH_>::deepRequestChange(Control& HFSM2_IF_TRANSITION_HISTORY(control),
									 const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, TH_>::deepRequestRestart(Control& HFSM2_IF_TRANSITION_HISTORY(control),
									  const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, TH_>::deepRequestResume(Control& HFSM2_IF_TRANSITION_HISTORY(control),
									 const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, TH_>::deepRequestSelect(Control& HFSM2_IF_TRANSITION_HISTORY(control),
									 const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, TH_>::deepRequestUtilize(Control& HFSM2_IF_TRANSITION_HISTORY(control),
									  const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, TH_>::deepRequestRandomize(Control& HFSM2_IF_TRANSITION_HISTORY(control),
										const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA_, TH_>::UP
S_<TN_, TA_, TH_>::deepReportChange(Control& control) noexcept {
	const Utility utility = wrapUtility(control);

	const Parent parent = stateParent(control);

	return {utility, parent.prong};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA_, TH_>::UP
S_<TN_, TA_, TH_>::deepReportUtilize(Control& control) noexcept {
	const Utility utility = wrapUtility(control);
	const Parent  parent  = stateParent(control);

	return {utility, parent.prong};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA_, TH_>::Rank
S_<TN_, TA_, TH_>::deepReportRank(Control& control) noexcept {
	return wrapRank(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA_, TH_>::Utility
S_<TN_, TA_, TH_>::deepReportRandomize(Control& control) noexcept {
	return wrapUtility(control);
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(NO)
const char*
S_<TN_, TA_, TH_>::name() noexcept {
	const std::type_index type = typeid(Head);
	const char* const raw = type.name();

#if defined(_MSC_VER)

	Short first =
		raw[0] == 's' ? 7 : // Struct
		raw[0] == 'c' ? 6 : // Class
						0;

	for (Short c = first; raw[c]; ++c)
		if (raw[c] == ':')
			first = c + 1;

	return raw + first;

#elif defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)

	return raw;

#else

	return raw;

#endif
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA_, TH_>::deepGetNames(const Long parent,
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
