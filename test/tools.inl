// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig>
void
LoggerT<TConfig>::recordMethod(Context& /*context*/,
							   const StateID origin,
							   const Method method) noexcept
{
	REQUIRE(hfsm2::methodName(method));

	switch (method) {

	#ifdef HFSM2_ENABLE_UTILITY_THEORY

		case Method::RANK:
			history.emplace_back(origin, Event::Type::RANK);
			break;

		case Method::UTILITY:
			history.emplace_back(origin, Event::Type::UTILITY);
			break;

	#endif

		case Method::ENTRY_GUARD:
			history.emplace_back(origin, Event::Type::ENTRY_GUARD);
			break;

		case Method::CONSTRUCT:
			history.emplace_back(origin, Event::Type::CONSTRUCT);
			break;

		case Method::ENTER:
			history.emplace_back(origin, Event::Type::ENTER);
			break;

		case Method::REENTER:
			history.emplace_back(origin, Event::Type::REENTER);
			break;

		case Method::UPDATE:
			history.emplace_back(origin, Event::Type::UPDATE);
			break;

		case Method::REACT:
			history.emplace_back(origin, Event::Type::REACT);
			break;

		case Method::EXIT_GUARD:
			history.emplace_back(origin, Event::Type::EXIT_GUARD);
			break;

		case Method::EXIT:
			history.emplace_back(origin, Event::Type::EXIT);
			break;

		case Method::DESTRUCT:
			history.emplace_back(origin, Event::Type::DESTRUCT);
			break;

	#ifdef HFSM2_ENABLE_PLANS

		case Method::PLAN_SUCCEEDED:
			history.emplace_back(origin, Event::Type::PLAN_SUCCEEDED);
			break;

		case Method::PLAN_FAILED:
			history.emplace_back(origin, Event::Type::PLAN_FAILED);
			break;

	#endif

		default:
			HFSM2_BREAK();
	}
}

//------------------------------------------------------------------------------

template <typename TConfig>
void
LoggerT<TConfig>::recordTransition(Context& /*context*/,
								   const StateID origin,
								   const TransitionType transitionType,
								   const StateID target) noexcept
{
	REQUIRE(hfsm2::transitionName(transitionType));

	switch (transitionType) {
		case TransitionType::CHANGE:
			history.emplace_back(origin, Event::Type::CHANGE,    target);
			break;

		case TransitionType::RESTART:
			history.emplace_back(origin, Event::Type::RESTART,   target);
			break;

		case TransitionType::RESUME:
			history.emplace_back(origin, Event::Type::RESUME,    target);
			break;

	#ifdef HFSM2_ENABLE_UTILITY_THEORY

		case TransitionType::UTILIZE:
			history.emplace_back(origin, Event::Type::UTILIZE,   target);
			break;

		case TransitionType::RANDOMIZE:
			history.emplace_back(origin, Event::Type::RANDOMIZE, target);
			break;

	#endif

		case TransitionType::SCHEDULE:
			history.emplace_back(origin, Event::Type::SCHEDULE,  target);
			break;

		default:
			HFSM2_BREAK();
	}
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_PLANS

template <typename TConfig>
void
LoggerT<TConfig>::recordTaskStatus(Context& /*context*/,
								   const RegionID region,
								   const StateID origin,
								   const StatusEvent event) noexcept
{
	switch (event) {
		case StatusEvent::SUCCEEDED:
			history.emplace_back(region, Event::Type::TASK_SUCCESS, origin);
			break;

		case StatusEvent::FAILED:
			history.emplace_back(region, Event::Type::TASK_FAILURE, origin);
			break;

		default:
			HFSM2_BREAK();
	}
}

//------------------------------------------------------------------------------

template <typename TConfig>
void
LoggerT<TConfig>::recordPlanStatus(Context& /*context*/,
								   const RegionID region,
								   const StatusEvent event) noexcept
{
	switch (event) {
		case StatusEvent::SUCCEEDED:
			history.emplace_back(region, Event::Type::PLAN_SUCCESS, hfsm2::INVALID_STATE_ID);
			break;

		case StatusEvent::FAILED:
			history.emplace_back(region, Event::Type::PLAN_FAILURE, hfsm2::INVALID_STATE_ID);
			break;

		default:
			HFSM2_BREAK();
	}
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TConfig>
void
LoggerT<TConfig>::recordUtilityResolution(Context& /*context*/,
										  const StateID head,
										  const StateID prong,
										  const Utilty utilty) noexcept
{
	history.emplace_back(head, Event::Type::UTILITY_RESOLUTION, prong, utilty);
}

//------------------------------------------------------------------------------

template <typename TConfig>
void
LoggerT<TConfig>::recordRandomResolution(Context& /*context*/,
										 const StateID head,
										 const StateID prong,
										 const Utilty utilty) noexcept
{
	history.emplace_back(head, Event::Type::RANDOM_RESOLUTION, prong, utilty);
}

#endif

//------------------------------------------------------------------------------

template <typename TConfig>
void
LoggerT<TConfig>::recordCancelledPending(Context& /*context*/,
										 const StateID origin) noexcept
{
	history.emplace_back(origin, Event::Type::CANCEL_PENDING);
}

//------------------------------------------------------------------------------

template <typename TConfig>
void
LoggerT<TConfig>::assertSequence(const Events& reference) noexcept {
	const auto count = std::max(history.size(), reference.size());

	for (unsigned i = 0; i < count; ++i) {
		REQUIRE(i < history.size()); //-V521
		REQUIRE(i < reference.size()); //-V521

		if (i < history.size() &&
			i < reference.size())
		{
			REQUIRE(history[i].type	  == reference[i].type); //-V521
			REQUIRE(history[i].origin == reference[i].origin); //-V521
			REQUIRE(history[i].target == reference[i].target); //-V521
		}
	}

	history.clear();
}

//------------------------------------------------------------------------------

template <typename TMachine>
void
assertActive(TMachine& machine,
			 const Types& all,
			 const Types& toCheck) noexcept
{
	for (const auto& type : all) {
		if (std::find(toCheck.begin(), toCheck.end(), type) != toCheck.end())
			REQUIRE( machine.isActive(type)); //-V521
		else
			REQUIRE(!machine.isActive(type)); //-V521
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TMachine>
void
assertResumable(TMachine& machine,
				const Types& all,
				const Types& toCheck) noexcept
{
	for (const auto& type : all) {
		if (std::find(toCheck.begin(), toCheck.end(), type) != toCheck.end())
			REQUIRE( machine.isResumable(type)); //-V521
		else
			REQUIRE(!machine.isResumable(type)); //-V521
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY

template <typename TMachine>
void
assertLastTransitions(TMachine& machine,
						const Types& all,
						const Types& toCheck) noexcept
{
	for (const auto& type : all) {
		if (std::find(toCheck.begin(), toCheck.end(), type) != toCheck.end())
			REQUIRE( machine.lastTransition(type)); //-V521
		else
			REQUIRE(!machine.lastTransition(type)); //-V521
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////
