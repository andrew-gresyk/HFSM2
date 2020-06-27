namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
struct RegisterT {
	using StateParents	= StaticArray<Parent, TA::STATE_COUNT>;
	using StateList		= typename TA::StateList;

	static constexpr StateID STATE_ID = NS;

	static HFSM2_INLINE
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
struct RegisterT<NS, TA, StaticEmptyT<TA>> {
	using StateParents	= StaticArray<Parent, TA::STATE_COUNT>;

	static HFSM2_INLINE
	void
	execute(StateParents&, const Parent) {}
};

////////////////////////////////////////////////////////////////////////////////

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepRegister(Registry& registry,
							  const Parent parent)
{
	using Register = RegisterT<STATE_ID, TA, Head>;
	Register::execute(registry.stateParents, parent);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TN_, typename TA, typename TH>
typename S_<TN_, TA, TH>::Rank
S_<TN_, TA, TH>::wrapRank(Control& control) {
	HFSM2_LOG_STATE_METHOD(&Head::rank,
						   control.context(),
						   Method::RANK);

	return _headBox.get().rank(static_cast<const Control&>(control));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
typename S_<TN_, TA, TH>::Utility
S_<TN_, TA, TH>::wrapUtility(Control& control) {
	HFSM2_LOG_STATE_METHOD(&Head::utility,
						   control.context(),
						   Method::UTILITY);

	return _headBox.get().utility(static_cast<const Control&>(control));
}

#endif

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
bool
S_<TN_, TA, TH>::deepEntryGuard(GuardControl& control) {
	HFSM2_LOG_STATE_METHOD(&Head::entryGuard,
						   control.context(),
						   Method::ENTRY_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	_headBox.guard(control);

	return !cancelledBefore && control._cancelled;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepConstruct(PlanControl& HFSM2_IF_LOG_INTERFACE(control)) {
#ifdef HFSM2_ENABLE_PLANS
	HFSM2_ASSERT(!control._planData.tasksSuccesses.template get<STATE_ID>());
	HFSM2_ASSERT(!control._planData.tasksFailures .template get<STATE_ID>());
#endif

	HFSM2_LOG_STATE_METHOD(&Head::enter,
						   control.context(),
						   Method::CONSTRUCT);

	_headBox.construct();
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepEnter(PlanControl& control) {
	HFSM2_LOG_STATE_METHOD(&Head::enter,
						   control.context(),
						   Method::ENTER);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().widePreEnter(control.context());
	_headBox.get().		  enter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepReenter(PlanControl& control) {
#ifdef HFSM2_ENABLE_PLANS
	HFSM2_ASSERT(!control._planData.tasksSuccesses.template get<STATE_ID>());
	HFSM2_ASSERT(!control._planData.tasksFailures .template get<STATE_ID>());
#endif

	HFSM2_LOG_STATE_METHOD(&Head::reenter,
						   control.context(),
						   Method::REENTER);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.destruct ();
	_headBox.construct();

	_headBox.get().widePreReenter(control.context());
	_headBox.get().		  reenter(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
Status
S_<TN_, TA, TH>::deepUpdate(FullControl& control) {
	HFSM2_LOG_STATE_METHOD(&Head::update,
						   control.context(),
						   Method::UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().widePreUpdate(control.context());
	_headBox.get().		  update(control);

	return control._status;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
template <typename TEvent>
Status
S_<TN_, TA, TH>::deepReact(FullControl& control,
						   const TEvent& event)
{
	auto reaction = static_cast<void(Head::*)(const TEvent&, FullControl&)>(&Head::react);
	HFSM2_LOG_STATE_METHOD(reaction,
						   control.context(),
						   Method::REACT);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().widePreReact(event, control.context());
	(_headBox.get().*reaction) (event, control);				//_headBox.get().react(event, control);

	return control._status;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
bool
S_<TN_, TA, TH>::deepExitGuard(GuardControl& control) {
	HFSM2_LOG_STATE_METHOD(&Head::exitGuard,
						   control.context(),
						   Method::EXIT_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control._cancelled;

	_headBox.get().widePreExitGuard(control.context());
	_headBox.get().		  exitGuard(control);

	return !cancelledBefore && control._cancelled;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepExit(PlanControl& control) {
	HFSM2_LOG_STATE_METHOD(&Head::exit,
						   control.context(),
						   Method::EXIT);

	ScopedOrigin origin{control, STATE_ID};

	// if you see..
	// VS	 - error C2039:  'exit': is not a member of 'Blah'
	// Clang - error : no member named 'exit' in 'Blah'
	//
	// .. inherit state 'Blah' from hfsm2::Machine::Instance::State
	_headBox.get().		   exit(control);
	_headBox.get().widePostExit(control.context());
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepDestruct(PlanControl&
						  #if defined HFSM2_ENABLE_LOG_INTERFACE || defined HFSM2_ENABLE_PLANS
							  control
						  #endif
							  )
{
	HFSM2_LOG_STATE_METHOD(&Head::exit,
						   control.context(),
						   Method::DESTRUCT);

	_headBox.destruct();

#ifdef HFSM2_ENABLE_PLANS
	control._planData.tasksSuccesses.template reset<STATE_ID>();
	control._planData.tasksFailures .template reset<STATE_ID>();
#endif
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_PLANS

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::wrapPlanSucceeded(FullControl& control) {
	HFSM2_LOG_STATE_METHOD(&Head::planSucceeded,
						   control.context(),
						   Method::PLAN_SUCCEEDED);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().planSucceeded(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::wrapPlanFailed(FullControl& control) {
	HFSM2_LOG_STATE_METHOD(&Head::planFailed,
						   control.context(),
						   Method::PLAN_FAILED);

	ScopedOrigin origin{control, STATE_ID};

	_headBox.get().planFailed(control);
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TN_, typename TA, typename TH>
typename S_<TN_, TA, TH>::UP
S_<TN_, TA, TH>::deepReportChange(Control& control) {
	const Utility utility = wrapUtility(control);

	const Parent parent = stateParent(control);

	return {utility, parent.prong};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
typename S_<TN_, TA, TH>::UP
S_<TN_, TA, TH>::deepReportUtilize(Control& control) {
	const Utility utility = wrapUtility(control);
	const Parent  parent  = stateParent(control);

	return {utility, parent.prong};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
typename S_<TN_, TA, TH>::Rank
S_<TN_, TA, TH>::deepReportRank(Control& control) {
	return wrapRank(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA, typename TH>
typename S_<TN_, TA, TH>::Utility
S_<TN_, TA, TH>::deepReportRandomize(Control& control) {
	return wrapUtility(control);
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

template <typename TN_, typename TA, typename TH>
const char*
S_<TN_, TA, TH>::name() {
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

template <typename TN_, typename TA, typename TH>
void
S_<TN_, TA, TH>::deepGetNames(const LongIndex parent,
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
