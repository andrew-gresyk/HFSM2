#pragma once

#define HFSM_ENABLE_LOG_INTERFACE
#define HFSM_ENABLE_ASSERT
#include <hfsm2/machine.hpp>

#include <catch2/catch.hpp>

//#include <vector> // already included in catch.hpp

//------------------------------------------------------------------------------

struct Event {
	enum Enum {

	#ifdef HFSM_ENABLE_UTILITY_THEORY
		RANK,
		UTILITY,
	#endif

		ENTRY_GUARD,
		CONSTRUCT,
		ENTER,
		REENTER,
		UPDATE,
		REACT,
		EXIT_GUARD,
		EXIT,
		DESTRUCT,

	#ifdef HFSM_ENABLE_UTILITY_THEORY
		UTILITY_RESOLUTION,
		RANDOM_RESOLUTION,
	#endif

		PLAN_SUCCEEDED,
		PLAN_FAILED,

		TASK_SUCCESS,
		TASK_FAILURE,
		PLAN_SUCCESS,
		PLAN_FAILURE,

		CHANGE,
		RESTART,
		RESUME,

	#ifdef HFSM_ENABLE_UTILITY_THEORY
		UTILIZE,
		RANDOMIZE,
	#endif

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
	using Context		  = TContext;
	using Interface		  = hfsm2::LoggerInterfaceT<Context>;

#ifdef HFSM_ENABLE_UTILITY_THEORY
	using Utilty		  = typename Interface::Utilty;
#endif

	using Method		  = typename Interface::Method;
	using StateID		  = typename Interface::StateID;
	using RegionID		  = typename Interface::RegionID;
	using TransitionType  = typename Interface::TransitionType;
	using StatusEvent	  = typename Interface::StatusEvent;

	void recordMethod(Context& context,
					  const StateID origin,
					  const Method method) override;

	void recordTransition(Context& context,
						  const StateID origin,
						  const TransitionType transitionType,
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

#ifdef HFSM_ENABLE_UTILITY_THEORY

	void recordUtilityResolution(Context& context,
								 const StateID head,
								 const StateID prong,
								 const Utilty utilty) override;

	void recordRandomResolution(Context& context,
								const StateID head,
								const StateID prong,
								const Utilty utilty) override;

#endif

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
