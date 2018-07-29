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

	Event(const hfsm::StateID origin_,
		  const Enum type_)
		: origin(origin_)
		, target(hfsm::INVALID_STATE_ID)
		, type(type_)
	{}

	Event(const hfsm::StateID origin_,
		  const Enum type_,
		  const hfsm::StateID target_)
		: origin(origin_)
		, target(target_)
		, type(type_)
	{}

	hfsm::StateID origin;
	hfsm::StateID target;
	Enum type;
};
using Events = std::vector<Event>;

////////////////////////////////////////////////////////////////////////////////

struct Logger
	: hfsm::LoggerInterface
{

	void recordMethod(const hfsm::StateID origin,
					  const Method method) override;

	void recordTransition(const Transition transition,
						  const hfsm::StateID target) override;

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
