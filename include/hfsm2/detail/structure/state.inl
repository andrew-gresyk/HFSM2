namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

template <StateID NS, typename TA_, typename TH_>
struct RegisterT {
	using StateParents	= StaticArray<Parent, TA_::STATE_COUNT>;
	using StateList		= typename TA_::StateList;

	static constexpr StateID STATE_ID = NS;

	static HFSM_INLINE
	void
	execute(StateParents& stateParents,
			const Parent parent)
	{
		static constexpr auto HEAD_ID = StateList::template index<TH_>();
		StaticAssertEquality<STATE_ID, HEAD_ID>();

		stateParents[STATE_ID] = parent;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, typename TA_>
struct RegisterT<NS, TA_, StaticEmptyT<TA_>> {
	using StateParents	= StaticArray<Parent, TA_::STATE_COUNT>;

	static HFSM_INLINE
	void
	execute(StateParents&, const Parent) {}
};

////////////////////////////////////////////////////////////////////////////////

template <typename TN_, typename TA_, typename TH_>
void
S_<TN_, TA_, TH_>::deepRegister(StateRegistry& stateRegistry,
								const Parent parent)
{
	using Register = RegisterT<STATE_ID, TA_, Head>;
	Register::execute(stateRegistry.stateParents, parent);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_>
bool
S_<TN_, TA_, TH_>::deepEntryGuard(GuardControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::entryGuard,
						  control.context(),
						  Method::ENTRY_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	_headBox.guard(control);

	return !cancelledBefore && control._cancelled;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_>
void
S_<TN_, TA_, TH_>::deepEnter(PlanControl& control) {
	HFSM_ASSERT(!control._planData.tasksSuccesses.template get<STATE_ID>());
	HFSM_ASSERT(!control._planData.tasksFailures .template get<STATE_ID>());

	HFSM_LOG_STATE_METHOD(&Head::enter,
						  control.context(),
						  Method::ENTER);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.construct();

	_headBox.get().widePreEnter(control.context());
	_headBox.get().	   enter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
void
S_<TN_, TA_, TH_>::deepReenter(PlanControl& control) {
	HFSM_ASSERT(!control._planData.tasksSuccesses.template get<STATE_ID>());
	HFSM_ASSERT(!control._planData.tasksFailures .template get<STATE_ID>());

	HFSM_LOG_STATE_METHOD(&Head::reenter,
						  control.context(),
						  Method::REENTER);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.destruct ();
	_headBox.construct();

	_headBox.get().widePreReenter(control.context());
	_headBox.get().	   reenter(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_>
Status
S_<TN_, TA_, TH_>::deepUpdate(FullControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::update,
						  control.context(),
						  Method::UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().widePreUpdate(control.context());
	_headBox.get().	   update(control);

	return control._status;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_>
template <typename TEvent>
Status
S_<TN_, TA_, TH_>::deepReact(FullControl& control,
							 const TEvent& event)
{
	auto reaction = static_cast<void(Head::*)(const TEvent&, FullControl&)>(&Head::react);
	HFSM_LOG_STATE_METHOD(reaction,
						  control.context(),
						  Method::REACT);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().widePreReact(event, control.context());
	(_headBox.get().*reaction)(event, control);				//_headBox.get().react(event, control);

	return control._status;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_>
bool
S_<TN_, TA_, TH_>::deepExitGuard(GuardControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::exitGuard,
						  control.context(),
						  Method::EXIT_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	_headBox.get().widePreExitGuard(control.context());
	_headBox.get().	   exitGuard(control);

	return !cancelledBefore && control._cancelled;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
void
S_<TN_, TA_, TH_>::deepExit(PlanControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::exit,
						  control.context(),
						  Method::EXIT);

	ScopedOrigin origin{control, STATE_ID};

	// if you see..
	// VS	 - error C2039:  'exit': is not a member of 'Blah'
	// Clang - error : no member named 'exit' in 'Blah'
	//
	// .. inherit state 'Blah' from hfsm2::Machine::Instance::State
	_headBox.get().	    exit(control);
	_headBox.get().widePostExit(control.context());

	_headBox.destruct();

	control._planData.tasksSuccesses.template reset<STATE_ID>();
	control._planData.tasksFailures .template reset<STATE_ID>();
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_>
void
S_<TN_, TA_, TH_>::wrapPlanSucceeded(FullControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::planSucceeded,
						  control.context(),
						  Method::PLAN_SUCCEEDED);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().planSucceeded(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
void
S_<TN_, TA_, TH_>::wrapPlanFailed(FullControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::planFailed,
						  control.context(),
						  Method::PLAN_FAILED);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().planFailed(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_>
typename S_<TN_, TA_, TH_>::Rank
S_<TN_, TA_, TH_>::wrapRank(Control& control) {
	HFSM_LOG_STATE_METHOD(&Head::rank,
						  control.context(),
						  Method::RANK);

	return _headBox.get().rank(static_cast<const Control&>(control));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
typename S_<TN_, TA_, TH_>::Utility
S_<TN_, TA_, TH_>::wrapUtility(Control& control) {
	HFSM_LOG_STATE_METHOD(&Head::utility,
						  control.context(),
						  Method::UTILITY);

	return _headBox.get().utility(static_cast<const Control&>(control));
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, typename TH_>
typename S_<TN_, TA_, TH_>::UP
S_<TN_, TA_, TH_>::deepReportChange(Control& control) {
	const Utility utility = wrapUtility(control);

	const Parent parent = stateParent(control);

	return {utility, parent.prong};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
typename S_<TN_, TA_, TH_>::UP
S_<TN_, TA_, TH_>::deepReportUtilize(Control& control) {
	const Utility utility = wrapUtility(control);
	const Parent  parent  = stateParent(control);

	return {utility, parent.prong};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
typename S_<TN_, TA_, TH_>::Rank
S_<TN_, TA_, TH_>::deepReportRank(Control& control) {
	return wrapRank(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, typename TH_>
typename S_<TN_, TA_, TH_>::Utility
S_<TN_, TA_, TH_>::deepReportRandomize(Control& control) {
	return wrapUtility(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TN_, typename TA_, typename TH_>
const char*
S_<TN_, TA_, TH_>::name() {
	if (HeadBox::isBare())
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

template <typename TN_, typename TA_, typename TH_>
void
S_<TN_, TA_, TH_>::deepGetNames(const LongIndex parent,
								const RegionType region,
								const ShortIndex depth,
								StructureStateInfos& _stateInfos) const
{
	_stateInfos.append(StructureStateInfo{parent, region, depth, name()});
}

#endif

///////////////////////////////////////////////////////////////////////////////

}
}
