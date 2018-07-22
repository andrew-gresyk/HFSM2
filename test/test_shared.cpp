#include "test_shared.hpp"

////////////////////////////////////////////////////////////////////////////////

void Logger::recordMethod(const std::type_index origin,
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
		default:
			assert(false);
	}
}

//------------------------------------------------------------------------------

void Logger::recordTransition(const Transition transition,
							  const std::type_index target)
{
	switch (transition) {
		case Transition::RESTART:
			history.emplace_back(typeid(Event), Event::RESTART,  target);
			break;
		case Transition::RESUME:
			history.emplace_back(typeid(Event), Event::RESUME,   target);
			break;
		case Transition::SCHEDULE:
			history.emplace_back(typeid(Event), Event::SCHEDULE, target);
			break;
		default:
			assert(false);
	}
}

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
