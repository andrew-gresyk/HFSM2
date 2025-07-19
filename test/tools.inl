// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

namespace test_tools {

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig>
void
LoggerT<TConfig>::recordMethod(const Context& /*context*/,
							   const StateID origin,
							   const Method method)
{
	REQUIRE(hfsm2::methodName(method));

	switch (method) {

		case Method::SELECT:
			history.emplace_back(origin, Event::Type::REPORT_SELECT);
			break;

	//#ifdef HFSM2_ENABLE_UTILITY_THEORY

		case Method::RANK:
			history.emplace_back(origin, Event::Type::REPORT_RANK);
			break;

		case Method::UTILITY:
			history.emplace_back(origin, Event::Type::UTILITY);
			break;

	//#endif

		case Method::ENTRY_GUARD:
			history.emplace_back(origin, Event::Type::ENTRY_GUARD);
			break;

		case Method::ENTER:
			history.emplace_back(origin, Event::Type::ENTER);
			break;

	//#ifdef HFSM2_ENABLE_REENTER

		case Method::REENTER:
			history.emplace_back(origin, Event::Type::REENTER);
			break;

	//#endif

		case Method::PRE_UPDATE:
			history.emplace_back(origin, Event::Type::PRE_UPDATE);
			break;

		case Method::UPDATE:
			history.emplace_back(origin, Event::Type::UPDATE);
			break;

		case Method::POST_UPDATE:
			history.emplace_back(origin, Event::Type::POST_UPDATE);
			break;

		case Method::PRE_REACT:
			history.emplace_back(origin, Event::Type::PRE_REACT);
			break;

		case Method::REACT:
			history.emplace_back(origin, Event::Type::REACT);
			break;

		case Method::POST_REACT:
			history.emplace_back(origin, Event::Type::POST_REACT);
			break;

		case Method::EXIT_GUARD:
			history.emplace_back(origin, Event::Type::EXIT_GUARD);
			break;

		case Method::EXIT:
			history.emplace_back(origin, Event::Type::EXIT);
			break;

	//#ifdef HFSM2_ENABLE_PLANS

		case Method::PLAN_SUCCEEDED:
			history.emplace_back(origin, Event::Type::PLAN_SUCCEEDED);
			break;

		case Method::PLAN_FAILED:
			history.emplace_back(origin, Event::Type::PLAN_FAILED);
			break;

	//#endif

		default:
			assert(false);
	}
}

//------------------------------------------------------------------------------

template <typename TConfig>
void
LoggerT<TConfig>::recordTransition(const Context& UNUSED(context),
								   const StateID origin,
								   const TransitionType transitionType,
								   const StateID target)
{
	REQUIRE(hfsm2::transitionName(transitionType));

	switch (transitionType) {
		case TransitionType::CHANGE:
			history.emplace_back(origin, Event::Type::CHANGE   , target);
			break;

		case TransitionType::RESTART:
			history.emplace_back(origin, Event::Type::RESTART  , target);
			break;

		case TransitionType::RESUME:
			history.emplace_back(origin, Event::Type::RESUME   , target);
			break;

		case TransitionType::SELECT:
			history.emplace_back(origin, Event::Type::SELECT   , target);
			break;

	//#ifdef HFSM2_ENABLE_UTILITY_THEORY

		case TransitionType::UTILIZE:
			history.emplace_back(origin, Event::Type::UTILIZE  , target);
			break;

		case TransitionType::RANDOMIZE:
			history.emplace_back(origin, Event::Type::RANDOMIZE, target);
			break;

	//#endif

		case TransitionType::SCHEDULE:
			history.emplace_back(origin, Event::Type::SCHEDULE , target);
			break;

		default:
			assert(false);
	}
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_PLANS

template <typename TConfig>
void
LoggerT<TConfig>::recordTaskStatus(const Context& UNUSED(context),
								   const StateID region,
								   const StateID origin,
								   const StatusEvent event)
{
	switch (event) {
		case StatusEvent::SUCCEEDED:
			history.emplace_back(region, Event::Type::TASK_SUCCESS, origin);
			break;

		case StatusEvent::FAILED:
			history.emplace_back(region, Event::Type::TASK_FAILURE, origin);
			break;

		default:
			assert(false);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TConfig>
void
LoggerT<TConfig>::recordPlanStatus(const Context& UNUSED(context),
								   const StateID region,
								   const StatusEvent event)
{
	switch (event) {
		case StatusEvent::SUCCEEDED:
			history.emplace_back(region, Event::Type::PLAN_SUCCESS, hfsm2::INVALID_STATE_ID);
			break;

		case StatusEvent::FAILED:
			history.emplace_back(region, Event::Type::PLAN_FAILURE, hfsm2::INVALID_STATE_ID);
			break;

		default:
			assert(false);
	}
}

#endif

//------------------------------------------------------------------------------

template <typename TConfig>
void
LoggerT<TConfig>::recordCancelledPending(const Context& UNUSED(context),
										 const StateID origin)
{
	history.emplace_back(origin, Event::Type::CANCEL_PENDING);
}

//------------------------------------------------------------------------------

template <typename TConfig>
void
LoggerT<TConfig>::recordSelectResolution(const Context& UNUSED(context),
										 const StateID head,
										 const Prong prong)
{
	history.emplace_back(head, Event::Type::SELECT_RESOLUTION, prong);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TConfig>
void
LoggerT<TConfig>::recordUtilityResolution(const Context& UNUSED(context),
										  const StateID head,
										  const Prong prong,
										  const Utilty utilty)
{
	history.emplace_back(head, Event::Type::UTILITY_RESOLUTION, prong, utilty);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TConfig>
void
LoggerT<TConfig>::recordRandomResolution(const Context& UNUSED(context),
										 const StateID head,
										 const Prong prong,
										 const Utilty utilty)
{
	history.emplace_back(head, Event::Type::RANDOM_RESOLUTION, prong, utilty);
}

#endif

//------------------------------------------------------------------------------

template <typename TConfig>
void
LoggerT<TConfig>::assertSequence(const Events& reference) {
	const auto count = std::max(history.size(), reference.size());

	for (unsigned i = 0; i < count; ++i) {
		REQUIRE(i < history  .size());
		REQUIRE(i < reference.size());

		if (i < history	 .size() &&
			i < reference.size())
		{
			const Event h = history  [i];
			const Event r = reference[i];

			REQUIRE(h.type	 == r.type	);
			REQUIRE(h.origin == r.origin);
			REQUIRE(h.target == r.target);
		}
	}

	history.clear();
}

//------------------------------------------------------------------------------

using Types = std::vector<hfsm2::StateID>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TMachine>
void
assertActive(TMachine& machine,
			 const Types& reference)
{
	Types actual;
	actual.reserve(reference.size());

	for (hfsm2::StateID id = 0; id < TMachine::Info::STATE_COUNT; ++id)
		if (machine.isActive(id))
			actual.push_back(id);

	REQUIRE(actual == reference);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TMachine>
void
assertResumable(TMachine& machine,
				const Types& reference)
{
	Types actual;
	actual.reserve(reference.size());

	for (hfsm2::StateID id = 0; id < TMachine::Info::STATE_COUNT; ++id)
		if (machine.isResumable(id))
			actual.push_back(id);

	REQUIRE(actual == reference);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

template <typename TStructure>
void
assertStructure(const TStructure& structure,
				const StructureReference& reference)
{
	const std::size_t count = std::max(static_cast<std::size_t>(structure.count()), reference.size());

	for (std::size_t i = 0; i < count; ++i) {
		REQUIRE(i < structure.count());
		REQUIRE(i < reference.size());

		if (i < structure.count() &&
			i < reference.size())
		{
			REQUIRE(structure[i].isActive == reference[i].isActive);
			REQUIRE(wcscmp(structure[i].prefix, reference[i].prefix) == 0);

		#ifdef _MSC_VER
			REQUIRE(strcmp(structure[i].name,   reference[i].name)	 == 0);
		#endif
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TActivityHistory>
void
assertActivity(const TActivityHistory& activity,
			   const ActivityReference& reference)
{
	REQUIRE(static_cast<size_t>(activity.count()) == reference.size());
	const size_t count = std::max(static_cast<size_t>(activity.count()), reference.size());

	for (size_t i = 0; i < count; ++i) {
		REQUIRE(i < activity.count());
		REQUIRE(i < reference.size());

		if (i < activity.count() &&
			i < reference.size())
		{
			REQUIRE(activity[i] == reference[i]);
		}
	}
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY

template <typename TMachine>
void
assertLastTransitions(TMachine& machine,
					  const Types& reference)
{
	Types actual;
	actual.reserve(reference.size());

	for (hfsm2::StateID id = 0; id < TMachine::Info::STATE_COUNT; ++id)
		if (machine.lastTransitionTo(id))
			actual.push_back(id);

	REQUIRE(actual == reference);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
