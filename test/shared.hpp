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
		CONSTRUCT,
		ENTER,
		REENTER,
		UPDATE,
		REACT,
		EXIT_GUARD,
		EXIT,
		DESTRUCT,

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
		CANCEL_PENDING,

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

template <typename TContext = hfsm2::EmptyContext>
struct LoggerT
	: hfsm2::LoggerInterfaceT<TContext>
{
	using Context	  = TContext;
	using Interface	  = hfsm2::LoggerInterfaceT<Context>;

	using Utilty	  = typename Interface::Utilty;
	using Method	  = typename Interface::Method;
	using StateID	  = typename Interface::StateID;
	using RegionID	  = typename Interface::RegionID;
	using Transition  = typename Interface::Transition;
	using StatusEvent = typename Interface::StatusEvent;

	void recordMethod(Context& context,
					  const StateID origin,
					  const Method method) override;

	void recordTransition(Context& context,
						  const StateID origin,
						  const Transition transition,
						  const StateID target) override;

	void recordTaskStatus(Context& context,
						  const RegionID region,
						  const StateID origin,
						  const StatusEvent event) override;

	void recordPlanStatus(Context& context,
						  const RegionID region,
						  const StatusEvent event) override;

	void recordCancelledPending(Context& context,
								const StateID origin) override;

	void recordUtilityResolution(Context& context,
								 const StateID head,
								 const StateID prong,
								 const Utilty utilty) override;

	void recordRandomResolution(Context& context,
								const StateID head,
								const StateID prong,
								const Utilty utilty) override;

	void assertSequence(const Events& reference);

	Events history;
};
using Logger = LoggerT<>;

//------------------------------------------------------------------------------

using Types = std::vector<hfsm2::StateID>;

template <typename TMachine>
void assertActive(TMachine& machine,
				  const Types& all,
				  const Types& toCheck);

template <typename TMachine>
void assertResumable(TMachine& machine,
					 const Types& all,
					 const Types& toCheck);

////////////////////////////////////////////////////////////////////////////////

#include "shared.inl"
