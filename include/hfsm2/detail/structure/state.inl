namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
struct RegisterT {
	using StateParents	= StaticArrayT<Parent, TA::STATE_COUNT>;
	using StateList		= typename TA::StateList;

	static constexpr StateID STATE_ID = NS;

	static
	HFSM2_CONSTEXPR(14)
	void
	execute(StateParents& stateParents,
			const Parent parent) noexcept
	{
		constexpr auto HEAD_ID = index<StateList, TH>();
		static_assert(STATE_ID == HEAD_ID, "");

		stateParents[STATE_ID] = parent;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, typename TA>
struct RegisterT<NS, TA, EmptyT<TA>> {
	using StateParents = StaticArrayT<Parent, TA::STATE_COUNT>;

	static
	HFSM2_CONSTEXPR(14)
	void
	execute(StateParents&, const Parent) noexcept {}
};

////////////////////////////////////////////////////////////////////////////////

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepRegister(Registry& registry,
							  const Parent parent) noexcept
{
	using Register = RegisterT<STATE_ID, TA, Head>;
	Register::execute(registry.stateParents, parent);
}

//------------------------------------------------------------------------------

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA, TH>::Rank
S_<TN_, TA, TH>::wrapRank(Control& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::rank,
						   Method::RANK);

	return Head::rank(static_cast<const Control&>(control));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA, TH>::Utility
S_<TN_, TA, TH>::wrapUtility(Control& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::utility,
						   Method::UTILITY);

	return Head::utility(static_cast<const Control&>(control));
}

#endif

//------------------------------------------------------------------------------
// COMMON

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
bool
S_<TN_, TA, TH>::deepEntryGuard(GuardControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::entryGuard,
						   Method::ENTRY_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	Head::widePreEntryGuard(control.context());
	Head::		 entryGuard(control);

	return !cancelledBefore && control._cancelled;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepEnter(PlanControl& control) noexcept {
	HFSM2_IF_PLANS(control._planData.verifyEmptyStatus(STATE_ID));

	HFSM2_LOG_STATE_METHOD(&Head::enter,
						   Method::ENTER);

	ScopedOrigin origin{control, STATE_ID};

	Head::widePreEnter(control.context());
	Head::		 enter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepReenter(PlanControl& control) noexcept {
	HFSM2_IF_PLANS(control._planData.verifyEmptyStatus(STATE_ID));

	HFSM2_LOG_STATE_METHOD(&Head::reenter,
						   Method::REENTER);

	ScopedOrigin origin{control, STATE_ID};

	Head::widePreReenter(control.context());
	Head::		 reenter(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
Status
S_<TN_, TA, TH>::deepUpdate(FullControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::update,
						   Method::UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	Head::widePreUpdate(control.context());
	Head::		 update(control);

	return control._status;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
S_<TN_, TA, TH>::deepReact(FullControl& control,
						   const TEvent& event) noexcept
{
	auto reaction = static_cast<void (Head::*)(const TEvent&, FullControl&)>(&Head::react);

	HFSM2_LOG_STATE_METHOD(reaction,
						   Method::REACT);

	ScopedOrigin origin{control, STATE_ID};

	Head::widePreReact(event, control.context());
	(this->*reaction)(event, control);

	return control._status;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
bool
S_<TN_, TA, TH>::deepExitGuard(GuardControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::exitGuard,
						   Method::EXIT_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	Head::widePreExitGuard(control.context());
	Head::		 exitGuard(control);

	return !cancelledBefore && control._cancelled;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepExit(PlanControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::exit,
						   Method::EXIT);

	ScopedOrigin origin{control, STATE_ID};

	// if you see..
	// VS	 - error C2039:  'exit': is not a member of 'Blah'
	// Clang - error : no member named 'exit' in 'Blah'
	//
	// .. inherit state 'Blah' from hfsm2::Machine::Instance::State
	Head::		  exit(control);
	Head::widePostExit(control.context());

	HFSM2_IF_PLANS(control._planData.clearTaskStatus(STATE_ID));
}

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::wrapPlanSucceeded(FullControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::planSucceeded,
						   Method::PLAN_SUCCEEDED);

	ScopedOrigin origin{control, STATE_ID};

	Head::planSucceeded(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::wrapPlanFailed(FullControl& control) noexcept {
	HFSM2_LOG_STATE_METHOD(&Head::planFailed,
						   Method::PLAN_FAILED);

	ScopedOrigin origin{control, STATE_ID};

	Head::planFailed(control);
}

#endif

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepForwardRequest(Control& HFSM2_IF_TRANSITION_HISTORY(control),
									const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepRequestChange(Control& HFSM2_IF_TRANSITION_HISTORY(control),
								   const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepRequestRestart(Control& HFSM2_IF_TRANSITION_HISTORY(control),
									const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepRequestResume(Control& HFSM2_IF_TRANSITION_HISTORY(control),
								   const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepRequestUtilize(Control& HFSM2_IF_TRANSITION_HISTORY(control),
									const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
void
S_<TN_, TA, TH>::deepRequestRandomize(Control& HFSM2_IF_TRANSITION_HISTORY(control),
									  const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(STATE_ID, request.index));
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA, TH>::UP
S_<TN_, TA, TH>::deepReportChange(Control& control) noexcept {
	const Utility utility = wrapUtility(control);

	const Parent parent = stateParent(control);

	return {utility, parent.prong};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA, TH>::UP
S_<TN_, TA, TH>::deepReportUtilize(Control& control) noexcept {
	const Utility utility = wrapUtility(control);
	const Parent  parent  = stateParent(control);

	return {utility, parent.prong};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA, TH>::Rank
S_<TN_, TA, TH>::deepReportRank(Control& control) noexcept {
	return wrapRank(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(14)
typename S_<TN_, TA, TH>::Utility
S_<TN_, TA, TH>::deepReportRandomize(Control& control) noexcept {
	return wrapUtility(control);
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(NO)
const char*
S_<TN_, TA, TH>::name() noexcept {
	if (isBare())
		return "";
	else {
		const std::type_index type = typeid(Head);
		const char* const raw = type.name();

	#if defined(_MSC_VER)

		auto first =
			raw[0] == 's' ? 7 : // Struct
			raw[0] == 'c' ? 6 : // Class
							0;

		for (auto c = first; raw[c]; ++c)
			if (raw[c] == ':')
				first = c + 1;

		return raw + first;

	#elif defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)

		return raw;

	#else

		return raw;

	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
HFSM2_CONSTEXPR(11)
void
S_<TN_, TA, TH>::deepGetNames(const Long parent,
							  const RegionType region,
							  const Short depth,
							  StructureStateInfos& _stateInfos) const noexcept
{
	_stateInfos.emplace(StructureStateInfo{parent, region, depth, name()});
}

#endif

///////////////////////////////////////////////////////////////////////////////

}
}
