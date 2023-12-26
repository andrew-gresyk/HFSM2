// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#pragma once

#define HFSM2_ENABLE_LOG_INTERFACE
#define HFSM2_ENABLE_ASSERT
#include <hfsm2/machine.hpp>

#include <doctest/doctest.h>

#include <assert.h>
#include <wchar.h> // wcscmp()

#include <algorithm>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

#define UNUSED(x)

//------------------------------------------------------------------------------

struct Event final {
	enum class Type {

		REPORT_SELECT,

	//#if HFSM2_UTILITY_THEORY_AVAILABLE()
		REPORT_RANK,
		UTILITY,
	//#endif

		ENTRY_GUARD,
		ENTER,
		REENTER,
		PRE_UPDATE,
		UPDATE,
		POST_UPDATE,
		PRE_REACT,
		REACT,
		POST_REACT,
		EXIT_GUARD,
		EXIT,

		SELECT_RESOLUTION,

	//#if HFSM2_UTILITY_THEORY_AVAILABLE()
		UTILITY_RESOLUTION,
		RANDOM_RESOLUTION,
	//#endif

	//#if HFSM2_PLANS_AVAILABLE()
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
		SELECT,

	//#if HFSM2_UTILITY_THEORY_AVAILABLE()
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
struct LoggerT final
	:								 TConfig::LoggerInterface
{
	using Interface		  = typename TConfig::LoggerInterface;

	using typename Interface::Context;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using typename Interface::Utilty;
#endif

	using typename Interface::Method;
	using typename Interface::Prong;
	using typename Interface::StateID;
	using typename Interface::RegionID;
	using typename Interface::TransitionType;

#ifdef HFSM2_ENABLE_PLANS
	using StatusEvent	  = typename Interface::StatusEvent;
#endif

	void recordMethod(const Context& context,
					  const StateID origin,
					  const Method method) override;

	void recordTransition(const Context& context,
						  const StateID origin,
						  const TransitionType transitionType,
						  const StateID target) override;

#ifdef HFSM2_ENABLE_PLANS

	void recordTaskStatus(const Context& context,
						  const RegionID region,
						  const StateID origin,
						  const StatusEvent event) override;

	void recordPlanStatus(const Context& context,
						  const RegionID region,
						  const StatusEvent event) override;

#endif

	void recordCancelledPending(const Context& context,
								const StateID origin) override;

	void recordSelectResolution(const Context& context,
								const StateID head,
								const Prong prong) override;

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	void recordUtilityResolution(const Context& context,
								 const StateID head,
								 const Prong prong,
								 const Utilty utilty) override;

	void recordRandomResolution(const Context& context,
								const StateID head,
								const Prong prong,
								const Utilty utilty) override;

#endif

	void assertSequence(const Events& reference);

	Events history;
};
using Logger = LoggerT<::hfsm2::Config>;

////////////////////////////////////////////////////////////////////////////////

using Types = std::vector<hfsm2::StateID>;

template <typename TMachine>
void assertActive(TMachine& machine,
				  const Types& all,
				  const Types& toCheck);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TMachine>
void assertResumable(TMachine& machine,
					 const Types& all,
					 const Types& toCheck);

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

using StructureReference = std::vector<hfsm2::StructureEntry>;

template <typename TStructure>
void
assertStructure(const TStructure& structure,
				const StructureReference& reference);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

using ActivityReference = std::vector<int8_t>;

template <typename TActivityHistory>
void
assertActivity(const TActivityHistory& activity,
			   const ActivityReference& reference);

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY

template <typename TMachine>
void assertLastTransitions(TMachine& machine,
						   const Types& all,
						   const Types& toCheck);

#endif

////////////////////////////////////////////////////////////////////////////////

#include "tools.inl"
