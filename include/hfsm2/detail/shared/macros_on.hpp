//------------------------------------------------------------------------------

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wextra-semi" // error : extra ';' inside a class
#endif

#if defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic" // error : extra ';'
#endif

////////////////////////////////////////////////////////////////////////////////

#define HFSM2_INLINE													//inline

//------------------------------------------------------------------------------

#if INTPTR_MAX == INT64_MAX
	#define HFSM2_64BIT_OR_32BIT(p64, p32)									 p64
#else
	#define HFSM2_64BIT_OR_32BIT(p64, p32)									 p32
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _DEBUG && defined _MSC_VER
	#define HFSM2_BREAK()										  __debugbreak()
#else
	#define HFSM2_BREAK()											  ((void) 0)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef _DEBUG
	#define HFSM2_IF_DEBUG(...)										 __VA_ARGS__
	#define HFSM2_UNLESS_DEBUG(...)
	#define HFSM2_DEBUG_OR(y, n)											   y
#else
	#define HFSM2_IF_DEBUG(...)
	#define HFSM2_UNLESS_DEBUG(...)									 __VA_ARGS__
	#define HFSM2_DEBUG_OR(y, n)											   n
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_ASSERT
	#define HFSM2_IF_ASSERT(...)									 __VA_ARGS__
	#define HFSM2_CHECKED(x)					   (!!(x) || (HFSM2_BREAK(), 0))
	#define HFSM2_ASSERT(x)						   (!!(x) || (HFSM2_BREAK(), 0))
	#define HFSM2_ASSERT_OR(y, n)											   y
#else
	#define HFSM2_IF_ASSERT(...)
	#define HFSM2_CHECKED(x)												   x
	#define HFSM2_ASSERT(x)											  ((void) 0)
	#define HFSM2_ASSERT_OR(y, n)											   n
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _MSC_VER || defined __clang_major__ && __clang_major__ >= 7
	#define HFSM2_EXPLICIT_MEMBER_SPECIALIZATION
#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_ALL
	#define HFSM2_ENABLE_UTILITY_THEORY
	#define HFSM2_ENABLE_PLANS
	#define HFSM2_ENABLE_SERIALIZATION
	#define HFSM2_ENABLE_TRANSITION_HISTORY
	#define HFSM2_ENABLE_STRUCTURE_REPORT
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	#define HFSM2_IF_UTILITY_THEORY(...)							 __VA_ARGS__
	#define HFSM2_UTILITY_THEORY_MASK									(1 << 0)
#else
	#define HFSM2_IF_UTILITY_THEORY(...)
	#define HFSM2_UTILITY_THEORY_MASK									(0 << 0)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_PLANS
	#define HFSM2_IF_PLANS(...)										 __VA_ARGS__
	#define HFSM2_PLANS_MASK											(1 << 1)
#else
	#define HFSM2_IF_PLANS(...)
	#define HFSM2_PLANS_MASK											(0 << 1)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_SERIALIZATION
	#define HFSM2_IF_SERIALIZATION(...)								 __VA_ARGS__
	#define HFSM2_SERIALIZATION_MASK									(1 << 2)
#else
	#define HFSM2_IF_SERIALIZATION(...)
	#define HFSM2_SERIALIZATION_MASK									(0 << 2)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY
	#define HFSM2_IF_TRANSITION_HISTORY(...)						 __VA_ARGS__
	#define HFSM2_TRANSITION_HISTORY_MASK								(1 << 3)
#else
	#define HFSM2_IF_TRANSITION_HISTORY(...)
	#define HFSM2_TRANSITION_HISTORY_MASK								(0 << 3)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	#define HFSM2_IF_STRUCTURE_REPORT(...)							 __VA_ARGS__
	#define HFSM2_STRUCTURE_REPORT_MASK									(1 << 4)
#else
	#define HFSM2_IF_STRUCTURE_REPORT(...)
	#define HFSM2_STRUCTURE_REPORT_MASK									(0 << 4)
#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_VERBOSE_DEBUG_LOG
	#define HFSM2_ENABLE_LOG_INTERFACE
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE

	#define HFSM2_IF_LOG_INTERFACE(...)								 __VA_ARGS__
	#define HFSM2_LOG_INTERFACE_MASK									(1 << 5)

	#define HFSM2_LOG_TRANSITION(CONTEXT, ORIGIN, TYPE, DESTINATION)		   \
		if (_logger)														   \
			_logger->recordTransition(CONTEXT, ORIGIN, TYPE, DESTINATION)

#ifdef HFSM2_ENABLE_PLANS

	#define HFSM2_LOG_TASK_STATUS(CONTEXT, REGION, ORIGIN, STATUS)			   \
		if (_logger)														   \
			_logger->recordTaskStatus(CONTEXT, REGION, ORIGIN, STATUS)

	#define HFSM2_LOG_PLAN_STATUS(CONTEXT, REGION, STATUS)					   \
		if (_logger)														   \
			_logger->recordPlanStatus(CONTEXT, REGION, STATUS)

#endif

	#define HFSM2_LOG_CANCELLED_PENDING(CONTEXT, ORIGIN)					   \
		if (_logger)														   \
			_logger->recordCancelledPending(CONTEXT, ORIGIN)

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	#define HFSM2_LOG_UTILITY_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)		   \
		if (auto* const logger = control.logger())							   \
			logger->recordUtilityResolution(CONTEXT, HEAD, PRONG, UTILITY)

	#define HFSM2_LOG_RANDOM_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)		   \
		if (auto* const logger = control.logger())							   \
			logger->recordRandomResolution(CONTEXT, HEAD, PRONG, UTILITY)

#endif

#else

	#define HFSM2_IF_LOG_INTERFACE(...)
	#define HFSM2_LOG_INTERFACE_MASK									(0 << 5)

	#define HFSM2_LOG_TRANSITION(CONTEXT, ORIGIN, TYPE, DESTINATION)

#ifdef HFSM2_ENABLE_PLANS
	#define HFSM2_LOG_TASK_STATUS(CONTEXT, REGION, ORIGIN, STATUS)
	#define HFSM2_LOG_PLAN_STATUS(CONTEXT, REGION, STATUS)
#endif

	#define HFSM2_LOG_CANCELLED_PENDING(CONTEXT, ORIGIN)

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	#define HFSM2_LOG_UTILITY_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)
	#define HFSM2_LOG_RANDOM_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)
#endif

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_VERBOSE_DEBUG_LOG

	#define HFSM2_LOG_STATE_METHOD(METHOD, CONTEXT, METHOD_ID)				   \
		if (auto* const logger = control.logger())							   \
			logger->recordMethod(CONTEXT, STATE_ID, METHOD_ID)

#elif defined HFSM2_ENABLE_LOG_INTERFACE

	#define HFSM2_LOG_STATE_METHOD(METHOD, CONTEXT, METHOD_ID)				   \
		if (auto* const logger = control.logger())							   \
			log<decltype(METHOD)>(*logger, CONTEXT, METHOD_ID)

#else

	#define HFSM2_LOG_STATE_METHOD(METHOD, CONTEXT, METHOD_ID)

#endif

//------------------------------------------------------------------------------

namespace hfsm2 {

using FeatureTag = uint8_t;

constexpr FeatureTag FEATURE_TAG = HFSM2_UTILITY_THEORY_MASK	 |
								   HFSM2_PLANS_MASK			     |
								   HFSM2_SERIALIZATION_MASK	     |
								   HFSM2_TRANSITION_HISTORY_MASK |
								   HFSM2_STRUCTURE_REPORT_MASK   |
								   HFSM2_LOG_INTERFACE_MASK;

}

//------------------------------------------------------------------------------

#undef HFSM2_UTILITY_THEORY_MASK
#undef HFSM2_PLANS_MASK
#undef HFSM2_SERIALIZATION_MASK
#undef HFSM2_TRANSITION_HISTORY_MASK
#undef HFSM2_STRUCTURE_REPORT_MASK
#undef HFSM2_LOG_INTERFACE_MASK

////////////////////////////////////////////////////////////////////////////////
