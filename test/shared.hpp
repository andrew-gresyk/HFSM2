#pragma once

#define HFSM_ENABLE_LOG_INTERFACE
#define HFSM_ENABLE_ASSERT
#include <hfsm2/machine.hpp>

#include <catch2/catch.hpp>

//#include <vector> // already included in catch.hpp

//------------------------------------------------------------------------------

struct Event {
	enum Enum {
		RANK,
		UTILITY,
		ENTRY_GUARD,
		ENTER,
		REENTER,
		UPDATE,
		REACT_REQUEST,
		REACT,
		EXIT_GUARD,
		EXIT,

		UTILITY_RESOLUTION,
		RANDOM_RESOLUTION,

		PLAN_SUCCEEDED,
		PLAN_FAILED,

		TASK_SUCCESS,
		TASK_FAILURE,
		PLAN_SUCCESS,
		PLAN_FAILURE,

		CHANGE,
		RESTART,
		RESUME,
		UTILIZE,
		RANDOMIZE,
		SCHEDULE,
		CANCELLED_PENDING,

		COUNT
	};

	Event(const hfsm2::StateID origin_,
		  const Enum type_,
		  const hfsm2::StateID target_ = hfsm2::INVALID_STATE_ID,
		  const float utility_ = 0.0f)
		: origin{origin_}
		, type{type_}
		, target{target_}
		, utility{utility_}
	{}

	Event(const Enum type_,
		  const hfsm2::StateID target_,
		  const float utility_ = 0.0f)
		: origin{hfsm2::INVALID_STATE_ID}
		, type{type_}
		, target{target_}
		, utility{utility_}
	{}

	hfsm2::StateID origin;
	Enum type;
	hfsm2::StateID target;
	float utility;
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

	void recordCancelledPending(const StateID origin) override;

	void recordUtilityResolution(const StateID head,
								 const StateID prong,
								 const Utilty utilty) override;

	void recordRandomResolution(const StateID head,
								const StateID prong,
								const Utilty utilty) override;

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
			REQUIRE( machine.isActive(type)); //-V521
		else
			REQUIRE(!machine.isActive(type)); //-V521
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
			REQUIRE( machine.isResumable(type)); //-V521
		else
			REQUIRE(!machine.isResumable(type)); //-V521
	}
}

////////////////////////////////////////////////////////////////////////////////
