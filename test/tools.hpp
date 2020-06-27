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
	enum Enum {

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

template <hfsm2::FeatureTag NFeatureTag = hfsm2::FEATURE_TAG
		  IF_UTILITY_THEORY(, typename TUtilty = float)
		  , typename TContext = hfsm2::EmptyContext>
struct LoggerT
	: hfsm2::LoggerInterfaceT<TContext IF_UTILITY_THEORY(, TUtilty), NFeatureTag>
{
	static constexpr hfsm2::FeatureTag FEATURE_TAG = NFeatureTag;

	using Context		  = TContext;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Utilty		  = TUtilty;
#endif

	using Interface		  = hfsm2::LoggerInterfaceT<Context IF_UTILITY_THEORY(, Utilty), FEATURE_TAG>;

	using Method		  = typename Interface::Method;
	using StateID		  = typename Interface::StateID;
	using RegionID		  = typename Interface::RegionID;
	using TransitionType  = typename Interface::TransitionType;

#ifdef HFSM2_ENABLE_PLANS
	using StatusEvent	  = typename Interface::StatusEvent;
#endif

	void recordMethod(Context& context,
					  const StateID origin,
					  const Method method) override;

	void recordTransition(Context& context,
						  const StateID origin,
						  const TransitionType transitionType,
						  const StateID target) override;

#ifdef HFSM2_ENABLE_PLANS

	void recordTaskStatus(Context& context,
						  const RegionID region,
						  const StateID origin,
						  const StatusEvent event) override;

	void recordPlanStatus(Context& context,
						  const RegionID region,
						  const StatusEvent event) override;

#endif

	void recordCancelledPending(Context& context,
								const StateID origin) override;

#ifdef HFSM2_ENABLE_UTILITY_THEORY

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

#include "tools.inl"

#undef IF_UTILITY_THEORY
