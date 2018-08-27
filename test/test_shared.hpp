#pragma once

#define HFSM_ENABLE_STRUCTURE_REPORT
#define HFSM_ENABLE_LOG_INTERFACE
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

	Event(const hfsm::StateID origin_,
		  const Enum type_)
		: origin(origin_)
		, type(type_)
		, target(hfsm::INVALID_STATE_ID)
	{}

	Event(const Enum type_,
		  const hfsm::StateID target_)
		: origin(hfsm::INVALID_STATE_ID)
		, type(type_)
		, target(target_)
	{}

	Event(const hfsm::StateID origin_,
		  const Enum type_,
		  const hfsm::StateID target_)
		: origin(origin_)
		, type(type_)
		, target(target_)
	{}

	hfsm::StateID origin;
	Enum type;
	hfsm::StateID target;
};
using Events = std::vector<Event>;

////////////////////////////////////////////////////////////////////////////////

struct Logger
	: hfsm::LoggerInterface
{

	void recordMethod(const StateID origin,
					  const Method method) override;

	void recordTransition(const StateID origin,
						  const Transition transition,
						  const StateID target) override;

	void assertSequence(const Events& reference);

	Events history;
};

//------------------------------------------------------------------------------

using Types = std::vector<hfsm::StateID>;

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

////////////////////////////////////////////////////////////////////////////////
