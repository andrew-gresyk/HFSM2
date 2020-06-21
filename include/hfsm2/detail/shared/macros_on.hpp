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

#define HFSM_INLINE														//inline

//------------------------------------------------------------------------------

#if INTPTR_MAX == INT64_MAX
	#define HFSM_64BIT_OR_32BIT(p64, p32)		p64
#else
	#define HFSM_64BIT_OR_32BIT(p64, p32)		p32
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _DEBUG && defined _MSC_VER
	#define HFSM_BREAK()						__debugbreak()
#else
	#define HFSM_BREAK()						((void) 0)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef _DEBUG
	#define HFSM_IF_DEBUG(...)					__VA_ARGS__
	#define HFSM_UNLESS_DEBUG(...)
	#define HFSM_DEBUG_OR(y, n)					y
#else
	#define HFSM_IF_DEBUG(...)
	#define HFSM_UNLESS_DEBUG(...)				__VA_ARGS__
	#define HFSM_DEBUG_OR(y, n)					n
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_ASSERT
	#define HFSM_IF_ASSERT(...)					__VA_ARGS__
	#define HFSM_CHECKED(x)						(!!(x) || (HFSM_BREAK(), 0))
	#define HFSM_ASSERT(x)						(!!(x) || (HFSM_BREAK(), 0))
	#define HFSM_ASSERT_OR(y, n)				y
#else
	#define HFSM_IF_ASSERT(...)
	#define HFSM_CHECKED(x)						x
	#define HFSM_ASSERT(x)						((void) 0)
	#define HFSM_ASSERT_OR(y, n)				n
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _MSC_VER || defined __clang_major__ && __clang_major__ >= 7
	#define HFSM_EXPLICIT_MEMBER_SPECIALIZATION
#endif

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_UTILITY_THEORY
	#define HFSM_IF_UTILITY_THEORY(...)								 __VA_ARGS__
#else
	#define HFSM_IF_UTILITY_THEORY(...)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_SERIALIZATION
	#define HFSM_IF_SERIALIZATION(...)								 __VA_ARGS__
#else
	#define HFSM_IF_SERIALIZATION(...)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_TRANSITION_HISTORY
	#define HFSM_IF_TRANSITION_HISTORY(...)							 __VA_ARGS__
#else
	#define HFSM_IF_TRANSITION_HISTORY(...)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	#define HFSM_IF_STRUCTURE(...)									 __VA_ARGS__
#else
	#define HFSM_IF_STRUCTURE(...)
#endif

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_VERBOSE_DEBUG_LOG
	#define HFSM_ENABLE_LOG_INTERFACE
#endif

#ifdef HFSM_ENABLE_LOG_INTERFACE

	#define HFSM_IF_LOG_INTERFACE(...)								 __VA_ARGS__

	#define HFSM_LOG_TRANSITION(CONTEXT, ORIGIN, TYPE, DESTINATION)				\
		if (_logger)															\
			_logger->recordTransition(CONTEXT, ORIGIN, TYPE, DESTINATION)

	#define HFSM_LOG_TASK_STATUS(CONTEXT, REGION, ORIGIN, STATUS)				\
		if (_logger)															\
			_logger->recordTaskStatus(CONTEXT, REGION, ORIGIN, STATUS)

	#define HFSM_LOG_PLAN_STATUS(CONTEXT, REGION, STATUS)						\
		if (_logger)															\
			_logger->recordPlanStatus(CONTEXT, REGION, STATUS)

	#define HFSM_LOG_CANCELLED_PENDING(CONTEXT, ORIGIN)							\
		if (_logger)															\
			_logger->recordCancelledPending(CONTEXT, ORIGIN)

#ifdef HFSM_ENABLE_UTILITY_THEORY

	#define HFSM_LOG_UTILITY_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)			\
		if (auto* const logger = control.logger())								\
			logger->recordUtilityResolution(CONTEXT, HEAD, PRONG, UTILITY)

	#define HFSM_LOG_RANDOM_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)			\
		if (auto* const logger = control.logger())								\
			logger->recordRandomResolution(CONTEXT, HEAD, PRONG, UTILITY)

#endif

#else

	#define HFSM_IF_LOG_INTERFACE(...)

	#define HFSM_LOG_TRANSITION(CONTEXT, ORIGIN, TYPE, DESTINATION)
	#define HFSM_LOG_TASK_STATUS(CONTEXT, REGION, ORIGIN, STATUS)
	#define HFSM_LOG_PLAN_STATUS(CONTEXT, REGION, STATUS)
	#define HFSM_LOG_CANCELLED_PENDING(CONTEXT, ORIGIN)

#ifdef HFSM_ENABLE_UTILITY_THEORY
	#define HFSM_LOG_UTILITY_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)
	#define HFSM_LOG_RANDOM_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)
#endif

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_VERBOSE_DEBUG_LOG

	#define HFSM_LOG_STATE_METHOD(METHOD, CONTEXT, METHOD_ID)					\
		if (auto* const logger = control.logger())								\
			logger->recordMethod(CONTEXT, STATE_ID, METHOD_ID)

#elif defined HFSM_ENABLE_LOG_INTERFACE

	#define HFSM_LOG_STATE_METHOD(METHOD, CONTEXT, METHOD_ID)					\
		if (auto* const logger = control.logger())								\
			log<decltype(METHOD)>(*logger, CONTEXT, METHOD_ID)

#else
	#define HFSM_LOG_STATE_METHOD(METHOD, CONTEXT, METHOD_ID)
#endif

////////////////////////////////////////////////////////////////////////////////
