#include "shared.hpp"

////////////////////////////////////////////////////////////////////////////////

void
Logger::recordMethod(const StateID origin,
					 const Method method)
{
	switch (method) {
		case Method::UTILITY:
			history.emplace_back(origin, Event::UTILITY);
			break;
		case Method::ENTRY_GUARD:
			history.emplace_back(origin, Event::ENTRY_GUARD);
			break;
		case Method::ENTER:
			history.emplace_back(origin, Event::ENTER);
			break;
		case Method::REENTER:
			history.emplace_back(origin, Event::REENTER);
			break;
		case Method::UPDATE:
			history.emplace_back(origin, Event::UPDATE);
			break;
		case Method::REACT:
			history.emplace_back(origin, Event::REACT);
			break;
		case Method::EXIT_GUARD:
			history.emplace_back(origin, Event::EXIT_GUARD);
			break;
		case Method::EXIT:
			history.emplace_back(origin, Event::EXIT);
			break;
		case Method::PLAN_SUCCEEDED:
			history.emplace_back(origin, Event::PLAN_SUCCEEDED);
			break;
		case Method::PLAN_FAILED:
			history.emplace_back(origin, Event::PLAN_FAILED);
			break;
		default:
			HFSM_BREAK();
	}
}

//------------------------------------------------------------------------------

void
Logger::recordTransition(const StateID origin,
						 const Transition transition,
						 const StateID target)
{
	switch (transition) {
		case Transition::CHANGE:
			history.emplace_back(origin, Event::CHANGE,   target);
			break;

		case Transition::RESTART:
			history.emplace_back(origin, Event::RESTART,  target);
			break;

		case Transition::RESUME:
			history.emplace_back(origin, Event::RESUME,   target);
			break;

		case Transition::UTILIZE:
			history.emplace_back(origin, Event::UTILIZE,  target);
			break;

		case Transition::SCHEDULE:
			history.emplace_back(origin, Event::SCHEDULE, target);
			break;

		default:
			HFSM_BREAK();
	}
}

//------------------------------------------------------------------------------

void
Logger::recordTaskStatus(const RegionID region,
						 const StateID origin,
						 const StatusEvent event)
{
	switch (event) {
		case StatusEvent::SUCCEEDED:
			history.emplace_back(region, Event::TASK_SUCCESS, origin);
			break;
		case StatusEvent::FAILED:
			history.emplace_back(region, Event::TASK_FAILURE, origin);
			break;
		default:
			HFSM_BREAK();
	}
}

//------------------------------------------------------------------------------

void
Logger::recordPlanStatus(const RegionID region,
						 const StatusEvent event)
{
	switch (event) {
		case StatusEvent::SUCCEEDED:
			history.emplace_back(region, Event::PLAN_SUCCESS, hfsm2::INVALID_STATE_ID);
			break;
		case StatusEvent::FAILED:
			history.emplace_back(region, Event::PLAN_FAILURE, hfsm2::INVALID_STATE_ID);
			break;
		default:
			HFSM_BREAK();
	}
}

//------------------------------------------------------------------------------

void
Logger::recordUtilityResolution(const StateID head,
								const StateID prong,
								const Utilty utilty)
{
	history.emplace_back(head, Event::UTILITY_RESOLUTION, prong, utilty);
}

//------------------------------------------------------------------------------

void
Logger::recordCancelledPending(const StateID origin) {
	history.emplace_back(origin, Event::CANCELLED_PENDING);
}

//------------------------------------------------------------------------------

void
Logger::assertSequence(const Events& reference) {
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

////////////////////////////////////////////////////////////////////////////////
