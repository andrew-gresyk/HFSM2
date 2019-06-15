namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

namespace {

template <StateID NS, typename TA, typename TH>
struct RegisterT {
	using StateParents	= StaticArray<Parent, TA::STATE_COUNT>;
	using StateList		= typename TA::StateList;

	static constexpr StateID STATE_ID = NS;

	static HFSM_INLINE
	void
	execute(StateParents& stateParents,
			const Parent parent)
	{
		static constexpr auto HEAD_ID = StateList::template index<TH>();
		StaticAssertEquality<STATE_ID, HEAD_ID>();

		stateParents[STATE_ID] = parent;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, typename TA>
struct RegisterT<NS, TA, Empty<TA>> {
	using StateParents	= StaticArray<Parent, TA::STATE_COUNT>;

	static HFSM_INLINE
	void
	execute(StateParents&, const Parent) {}
};

}

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, typename TH>
void
S_<TN, TA, TH>::deepRegister(StateRegistry& stateRegistry,
							 const Parent parent)
{
	using Register = RegisterT<STATE_ID, TA, Head>;
	Register::execute(stateRegistry.stateParents, parent);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH>
bool
S_<TN, TA, TH>::deepEntryGuard(GuardControl& control,
							   const ShortIndex /*prong*/)
{
	HFSM_LOG_STATE_METHOD(&Head::entryGuard, Method::ENTRY_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control.cancelled();

	_head.widePreEntryGuard(control.context());
	_head.entryGuard(control);

	return !cancelledBefore && control.cancelled();
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH>
void
S_<TN, TA, TH>::deepEnter(PlanControl& control,
						  const ShortIndex /*prong*/)
{
	HFSM_ASSERT(!control.planData().tasksSuccesses.template get<STATE_ID>());
	HFSM_ASSERT(!control.planData().tasksFailures .template get<STATE_ID>());

	HFSM_LOG_STATE_METHOD(&Head::enter, Method::ENTER);

	ScopedOrigin origin{control, STATE_ID};

	_head.widePreEnter(control.context());
	_head.enter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH>
void
S_<TN, TA, TH>::deepReenter(PlanControl& control,
							const ShortIndex /*prong*/)
{
	HFSM_ASSERT(!control.planData().tasksSuccesses.template get<STATE_ID>());
	HFSM_ASSERT(!control.planData().tasksFailures .template get<STATE_ID>());

	HFSM_LOG_STATE_METHOD(&Head::reenter, Method::REENTER);

	ScopedOrigin origin{control, STATE_ID};

	_head.widePreReenter(control.context());
	_head.reenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH>
Status
S_<TN, TA, TH>::deepUpdate(FullControl& control,
						   const ShortIndex /*prong*/)
{
	HFSM_LOG_STATE_METHOD(&Head::update, Method::UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	_head.widePreUpdate(control.context());
	_head.update(control);

	return control._status;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH>
template <typename TEvent>
Status
S_<TN, TA, TH>::deepReact(FullControl& control,
						  const TEvent& event,
						  const ShortIndex /*prong*/)
{
	auto reaction = static_cast<void(Head::*)(const TEvent&, FullControl&)>(&Head::react);
	HFSM_LOG_STATE_METHOD(reaction, Method::REACT);

	ScopedOrigin origin{control, STATE_ID};

	_head.widePreReact(event, control.context());
	(_head.*reaction)(event, control);				//_head.react(event, control);

	return control._status;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH>
bool
S_<TN, TA, TH>::deepExitGuard(GuardControl& control,
							  const ShortIndex /*prong*/)
{
	HFSM_LOG_STATE_METHOD(&Head::exitGuard, Method::EXIT_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control.cancelled();

	_head.widePreExitGuard(control.context());
	_head.exitGuard(control);

	return !cancelledBefore && control.cancelled();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH>
void
S_<TN, TA, TH>::deepExit(PlanControl& control,
						 const ShortIndex /*prong*/)
{
	HFSM_LOG_STATE_METHOD(&Head::exit, Method::EXIT);

	ScopedOrigin origin{control, STATE_ID};

	// if you see..
	// VS	 - error C2039:  'exit': is not a member of 'Blah'
	// Clang - error : no member named 'exit' in 'Blah'
	//
	// .. inherit state 'Blah' from hfsm2::Machine::Instance::State
	_head.exit(control);
	_head.widePostExit(control.context());

	control.planData().tasksSuccesses.template reset<STATE_ID>();
	control.planData().tasksFailures .template reset<STATE_ID>();
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH>
void
S_<TN, TA, TH>::wrapPlanSucceeded(FullControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::planSucceeded, Method::PLAN_SUCCEEDED);

	ScopedOrigin origin{control, STATE_ID};

	_head.planSucceeded(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH>
void
S_<TN, TA, TH>::wrapPlanFailed(FullControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::planFailed, Method::PLAN_FAILED);

	ScopedOrigin origin{control, STATE_ID};

	_head.planFailed(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH>
typename TA::Utility
S_<TN, TA, TH>::wrapUtility(Control& control) {
	HFSM_LOG_STATE_METHOD(&Head::utility, Method::UTILITY);

	return _head.utility(static_cast<const Control&>(control));
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH>
typename TA::UP
S_<TN, TA, TH>::deepReportChange(Control& control,
								 const ShortIndex)
{
	const Utility utility = wrapUtility(control);

	const Parent parent = stateParent(control);

	return {utility, parent.prong};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH>
typename TA::UP
S_<TN, TA, TH>::deepReportUtilize(Control& control) {
	const Utility utility = wrapUtility(control);

	const Parent parent = stateParent(control);

	return {utility, parent.prong};
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, typename TH>
const char*
S_<TN, TA, TH>::name() {
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

template <typename TN, typename TA, typename TH>
void
S_<TN, TA, TH>::deepGetNames(const LongIndex parent,
							  const RegionType region,
							  const ShortIndex depth,
							  StructureStateInfos& _stateInfos) const
{
	_stateInfos << StructureStateInfo { parent, region, depth, name() };
}

#endif

///////////////////////////////////////////////////////////////////////////////

}
}
