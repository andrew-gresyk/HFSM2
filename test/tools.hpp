#pragma once

#define HFSM2_ENABLE_LOG_INTERFACE
#define HFSM2_ENABLE_ASSERT
#include <hfsm2/machine.hpp>

#include <catch2/catch.hpp>

//#include <vector> // already included in catch.hpp

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	#define IF_UTILITY_THEORY(...)									 __VA_ARGS__
#else
	#define IF_UTILITY_THEORY(...)
#endif

////////////////////////////////////////////////////////////////////////////////

struct Event {
	enum class Type {

	//#ifdef HFSM2_ENABLE_UTILITY_THEORY
		RANK,
		UTILITY,
	//#endif

		ENTRY_GUARD,
		CONSTRUCT,
		ENTER,
		REENTER,
		UPDATE,
		REACT,
		EXIT_GUARD,
		EXIT,
		DESTRUCT,

	//#ifdef HFSM2_ENABLE_UTILITY_THEORY
		UTILITY_RESOLUTION,
		RANDOM_RESOLUTION,
	//#endif

	//#ifdef HFSM2_ENABLE_PLANS
		PLAN_SUCCEEDED,
		PLAN_FAILED,

		TASK_SUCCESS,
		TASK_FAILURE,
		PLAN_SUCCESS,
		PLAN_FAILURE,
	//#endif

		CHANGE,
		RESTART,
		RESUME,

	//#ifdef HFSM2_ENABLE_UTILITY_THEORY
		UTILIZE,
		RANDOMIZE,
	//#endif

		SCHEDULE,
		CANCEL_PENDING,

		COUNT
	};

	Event(const hfsm2::StateID origin_,
		  const Type type_,
		  const hfsm2::StateID target_ = hfsm2::INVALID_STATE_ID,
		  const float utility_ = 0.0f)
		: origin{origin_}
		, type{type_}
		, target{target_}
		, utility{utility_}
	{}

	Event(const Type type_,
		  const hfsm2::StateID target_,
		  const float utility_ = 0.0f)
		: origin{hfsm2::INVALID_STATE_ID}
		, type{type_}
		, target{target_}
		, utility{utility_}
	{}

	hfsm2::StateID origin;
	Type type;
	hfsm2::StateID target;
	float utility;
};
using Events = std::vector<Event>;

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig>
struct LoggerT
	:								 TConfig::LoggerInterface
{
	using Interface		  = typename TConfig::LoggerInterface;

	using typename Interface::Context;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using typename Interface::Utilty;
#endif

	using typename Interface::Method;
	using typename Interface::StateID;
	using typename Interface::RegionID;
	using typename Interface::TransitionType;

#ifdef HFSM2_ENABLE_PLANS
	using StatusEvent	  = typename Interface::StatusEvent;
#endif

	void recordMethod(Context& context,
					  const StateID origin,
					  const Method method) noexcept override;

	void recordTransition(Context& context,
						  const StateID origin,
						  const TransitionType transitionType,
						  const StateID target) noexcept override;

#ifdef HFSM2_ENABLE_PLANS

	void recordTaskStatus(Context& context,
						  const RegionID region,
						  const StateID origin,
						  const StatusEvent event) noexcept override;

	void recordPlanStatus(Context& context,
						  const RegionID region,
						  const StatusEvent event) noexcept override;

#endif

	void recordCancelledPending(Context& context,
								const StateID origin) noexcept override;

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	void recordUtilityResolution(Context& context,
								 const StateID head,
								 const StateID prong,
								 const Utilty utilty) noexcept override;

	void recordRandomResolution(Context& context,
								const StateID head,
								const StateID prong,
								const Utilty utilty) noexcept override;

#endif

	void assertSequence(const Events& reference) noexcept;

	Events history;
};
using Logger = LoggerT<::hfsm2::Config>;

//------------------------------------------------------------------------------

using Types = std::vector<hfsm2::StateID>;

template <typename TMachine>
void assertActive(TMachine& machine,
				  const Types& all,
				  const Types& toCheck) noexcept;

template <typename TMachine>
void assertResumable(TMachine& machine,
					 const Types& all,
					 const Types& toCheck) noexcept;

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY

template <typename TMachine>
void assertLastTransitions(TMachine& machine,
							 const Types& all,
							 const Types& toCheck) noexcept;

#endif

////////////////////////////////////////////////////////////////////////////////

#include "tools.inl"

#undef IF_UTILITY_THEORY
