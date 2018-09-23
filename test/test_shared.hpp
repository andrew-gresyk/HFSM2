#pragma once

#define HFSM_ENABLE_STRUCTURE_REPORT
#define HFSM_ENABLE_LOG_INTERFACE
#define HFSM_ENABLE_ASSERT
#include <hfsm2/machine.hpp>

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
		PLAN_SUCCEEDED,
		PLAN_FAILED,
		TASK_SUCCESS,
		TASK_FAILURE,
		PLAN_SUCCESS,
		PLAN_FAILURE,

		RESTART,
		RESUME,
		SCHEDULE,

		COUNT
	};

	Event(const hfsm2::StateID origin_,
		  const Enum type_)
		: origin(origin_)
		, type(type_)
		, target(hfsm2::INVALID_STATE_ID)
	{}

	Event(const Enum type_,
		  const hfsm2::StateID target_)
		: origin(hfsm2::INVALID_STATE_ID)
		, type(type_)
		, target(target_)
	{}

	Event(const hfsm2::StateID origin_,
		  const Enum type_,
		  const hfsm2::StateID target_)
		: origin(origin_)
		, type(type_)
		, target(target_)
	{}

	hfsm2::StateID origin;
	Enum type;
	hfsm2::StateID target;
};
using Events = std::vector<Event>;

////////////////////////////////////////////////////////////////////////////////

struct Logger
	: hfsm2::LoggerInterface
{

	void recordMethod(const StateID origin,
					  const Method method) override;

	void recordTransition(const StateID origin,
						  const Transition transition,
						  const StateID target) override;

	void recordTaskStatus(const RegionID region,
						  const StateID origin,
						  const StatusEvent event) override;

	void recordPlanStatus(const RegionID region,
						  const StatusEvent event) override;

	void assertSequence(const Events& reference);

	Events history;
};

//------------------------------------------------------------------------------

using Types = std::vector<hfsm2::StateID>;

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
