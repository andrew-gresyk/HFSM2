#include "test_shared.hpp"

////////////////////////////////////////////////////////////////////////////////

void
Logger::recordMethod(const hfsm2::StateID origin,
					 const Method method)
{
	switch (method) {
		case Method::GUARD:
			history.emplace_back(origin, Event::GUARD);
			break;
		case Method::ENTER:
			history.emplace_back(origin, Event::ENTER);
			break;
		case Method::UPDATE:
			history.emplace_back(origin, Event::UPDATE);
			break;
		case Method::REACT:
			history.emplace_back(origin, Event::REACT);
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
			HFSM_ASSERT(false);
	}
}

//------------------------------------------------------------------------------

void
Logger::recordTransition(const hfsm2::StateID origin,
						 const Transition transition,
						 const hfsm2::StateID target)
{
	switch (transition) {
		case Transition::RESTART:
			history.emplace_back(origin, Event::RESTART,  target);
			break;
		case Transition::RESUME:
			history.emplace_back(origin, Event::RESUME,   target);
			break;
		case Transition::SCHEDULE:
			history.emplace_back(origin, Event::SCHEDULE, target);
			break;
		default:
			HFSM_ASSERT(false);
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
			history.emplace_back(region, Event::TASK_SUCCEEDED, origin);
			break;
		case StatusEvent::FAILED:
			history.emplace_back(region, Event::TASK_FAILED,	origin);
			break;
		default:
			HFSM_ASSERT(false);
	}
}

//------------------------------------------------------------------------------

void Logger::assertSequence(const Events& reference) {
	const auto count = std::max(history.size(), reference.size());

	for (unsigned i = 0; i < count; ++i) {
		REQUIRE(i < history.size());
		REQUIRE(i < reference.size());

		if (i < history.size() && i < reference.size())
			REQUIRE(history[i].origin == reference[i].origin);
	}

	history.clear();
}

////////////////////////////////////////////////////////////////////////////////
