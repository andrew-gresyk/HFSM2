#pragma once

#define HFSM_ENABLE_STRUCTURE_REPORT
#define HFSM_ENABLE_LOG_INTERFACE
#define HFSM_ENABLE_ALIGNMENT_CHEKS
#include <hfsm/machine.hpp>

#include <catch2/catch.hpp>

#include <vector>

//------------------------------------------------------------------------------

struct Event {
	enum Enum {
		GUARD,
		ENTER,
		UPDATE,
		REACT_REQUEST,
		REACT,
		EXIT,

		RESTART,
		RESUME,
		SCHEDULE,

		COUNT
	};

	Event(const std::type_index origin_,
		  const Enum type_)
		: origin(origin_)
		, target(typeid(Event))
		, type(type_)
	{}

	Event(const std::type_index origin_,
		  const Enum type_,
		  const std::type_index target_)
		: origin(origin_)
		, target(target_)
		, type(type_)
	{}

	std::type_index origin;
	std::type_index target;
	Enum type;
};
using Events = std::vector<Event>;

////////////////////////////////////////////////////////////////////////////////

struct Logger
	: hfsm::LoggerInterface
{

	void recordMethod(const std::type_index origin,
					  const Method method) override;

	void recordTransition(const Transition transition,
						  const std::type_index target) override;

	void assertSequence(const Events& reference);

	Events history;
};

//------------------------------------------------------------------------------

using Types = std::vector<std::type_index>;

template <typename TMachine>
void assertActive(TMachine& machine,
				  const Types& all,
				  const Types& toCheck)
{
	for (const auto& type : all) {
		if (std::find(toCheck.begin(), toCheck.end(), type) != toCheck.end())
			REQUIRE( machine.isActive(type));
		else
			REQUIRE(!machine.isActive(type));
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TMachine>
void assertResumable(TMachine& machine,
					 const Types& all,
					 const Types& toCheck)
{
	for (const auto& type : all) {
		if (std::find(toCheck.begin(), toCheck.end(), type) != toCheck.end())
			REQUIRE( machine.isResumable(type));
		else
			REQUIRE(!machine.isResumable(type));
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TMachine>
void assertScheduled(TMachine& machine,
					 const Types& all,
					 const Types& toCheck)
{
	for (const auto& type : all) {
		if (std::find(toCheck.begin(), toCheck.end(), type) != toCheck.end())
			REQUIRE( machine.isScheduled(type));
		else
			REQUIRE(!machine.isScheduled(type));
	}
}

////////////////////////////////////////////////////////////////////////////////
