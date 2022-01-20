////////////////////////////////////////////////////////////////////////////////

#define HFSM2_UNUSED(x)

//------------------------------------------------------------------------------

#define HFSM2_ATTRIBUTE(A)								   HFSM2_ATTRIBUTE_##A()

#ifndef __has_cpp_attribute
	#define __has_cpp_attribute(x)											   0
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if __has_cpp_attribute(fallthrough)
	#define HFSM2_ATTRIBUTE_FALLTHROUGH()						 [[fallthrough]]
#elif __has_cpp_attribute(clang::fallthrough)
	#define HFSM2_ATTRIBUTE_FALLTHROUGH()				  [[clang::fallthrough]]
#elif __has_cpp_attribute(gnu::fallthrough)
	#define HFSM2_ATTRIBUTE_FALLTHROUGH()					[[gnu::fallthrough]]
#else
	#define HFSM2_ATTRIBUTE_FALLTHROUGH()
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if __has_cpp_attribute(no_unique_address)
	#define HFSM2_ATTRIBUTE_NO_UNIQUE_ADDRESS()			   [[no_unique_address]]
#else
	#define HFSM2_ATTRIBUTE_NO_UNIQUE_ADDRESS()
#endif

//------------------------------------------------------------------------------

#define HFSM2_CONSTEXPR(A)								   HFSM2_CONSTEXPR_##A()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define HFSM2_CONSTEXPR_NO()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define HFSM2_CONSTEXPR_11()										   constexpr

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if __cplusplus >= 201402L
	#define HFSM2_CONSTEXPR_14()									   constexpr
#else
	#define HFSM2_CONSTEXPR_14()										  inline
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if __cplusplus >= 201703L
	#define HFSM2_CONSTEXPR_17()									   constexpr
#else
	#define HFSM2_CONSTEXPR_17()										  inline
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if __cplusplus >= 202002L
	#define HFSM2_CONSTEXPR_20()									   constexpr
#else
	#define HFSM2_CONSTEXPR_20()										  inline
#endif

//------------------------------------------------------------------------------

#if defined _MSC_VER
	#define HFSM2_EMPTY_BASES							 __declspec(empty_bases)
#else
	#define HFSM2_EMPTY_BASES
#endif

//------------------------------------------------------------------------------

#define HFSM2_ARCHITECTURE(A)							HFSM2_ARCHITECTURE_##A()

#if INTPTR_MAX == INT64_MAX
	#define HFSM2_ARCHITECTURE_64()											true
	#define HFSM2_ARCHITECTURE_32()										   false

	#define HFSM2_64BIT_OR_32BIT(p64, p32)									 p64
#elif INTPTR_MAX == INT32_MAX
	#define HFSM2_ARCHITECTURE_64()										   false
	#define HFSM2_ARCHITECTURE_32()											true

	#define HFSM2_64BIT_OR_32BIT(p64, p32)									 p32
#else
	#error
#endif

//------------------------------------------------------------------------------

#if defined _DEBUG && defined _MSC_VER
	#define HFSM2_BREAK()										  __debugbreak()
	#define HFSM2_BREAK_AVAILABLE()											true
#else
	#define HFSM2_BREAK()											  ((void) 0)
	#define HFSM2_BREAK_AVAILABLE()										   false
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

#if HFSM2_BREAK_AVAILABLE() && defined HFSM2_ENABLE_ASSERT
	#define HFSM2_ASSERT_AVAILABLE()										true
	#define HFSM2_IF_ASSERT(...)									 __VA_ARGS__
	#define HFSM2_CHECKED(x)					   (!!(x) || (HFSM2_BREAK(), 0))
	#define HFSM2_ASSERT(x)						   (!!(x) || (HFSM2_BREAK(), 0))
	#define HFSM2_ASSERT_OR(y, n)											   y
#else
	#define HFSM2_ASSERT_AVAILABLE()									   false
	#define HFSM2_IF_ASSERT(...)
	#define HFSM2_CHECKED(x)												   x
	#define HFSM2_ASSERT(x)											  ((void) 0)
	#define HFSM2_ASSERT_OR(y, n)											   n
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _MSC_VER || defined __clang_major__ && __clang_major__ >= 7
	#define HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()				true
#else
	#define HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()			   false
#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_ALL
	#define HFSM2_ENABLE_DEBUG_STATE_TYPE
	#define HFSM2_ENABLE_PLANS
	#define HFSM2_ENABLE_SERIALIZATION
	#define HFSM2_ENABLE_STRUCTURE_REPORT
	#define HFSM2_ENABLE_TRANSITION_HISTORY
	#define HFSM2_ENABLE_UTILITY_THEORY
#endif

//------------------------------------------------------------------------------

#ifndef HFSM2_DISABLE_TYPEINDEX
	#define HFSM2_TYPEINDEX_AVAILABLE()										true
	#define HFSM2_IF_TYPEINDEX(...)									 __VA_ARGS__
	#define HFSM2_TYPEINDEX_MASK										(1 << 0)
#else
	#define HFSM2_TYPEINDEX_AVAILABLE()									   false
	#define HFSM2_IF_TYPEINDEX(...)
	#define HFSM2_TYPEINDEX_MASK										(0 << 0)
#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_DEBUG_STATE_TYPE
	#define HFSM2_DEBUG_STATE_TYPE_AVAILABLE()								true
	#define HFSM2_DEBUG_STATE_TYPE_MASK									(1 << 1)
#else
	#define HFSM2_DEBUG_STATE_TYPE_AVAILABLE()							   false
	#define HFSM2_DEBUG_STATE_TYPE_MASK									(0 << 1)
#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_PLANS
	#define HFSM2_PLANS_AVAILABLE()											true
	#define HFSM2_IF_PLANS(...)										 __VA_ARGS__
	#define HFSM2_PLANS_MASK											(1 << 2)
#else
	#define HFSM2_PLANS_AVAILABLE()										   false
	#define HFSM2_IF_PLANS(...)
	#define HFSM2_PLANS_MASK											(0 << 2)
#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION
	#define HFSM2_SERIALIZATION_AVAILABLE()									true
	#define HFSM2_IF_SERIALIZATION(...)								 __VA_ARGS__
	#define HFSM2_SERIALIZATION_MASK									(1 << 3)
#else
	#define HFSM2_SERIALIZATION_AVAILABLE()								   false
	#define HFSM2_IF_SERIALIZATION(...)
	#define HFSM2_SERIALIZATION_MASK									(0 << 3)
#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	#define HFSM2_STRUCTURE_REPORT_AVAILABLE()								true
	#define HFSM2_IF_STRUCTURE_REPORT(...)							 __VA_ARGS__
	#define HFSM2_STRUCTURE_REPORT_MASK									(1 << 4)
#else
	#define HFSM2_STRUCTURE_REPORT_AVAILABLE()							   false
	#define HFSM2_IF_STRUCTURE_REPORT(...)
	#define HFSM2_STRUCTURE_REPORT_MASK									(0 << 4)
#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY
	#define HFSM2_TRANSITION_HISTORY_AVAILABLE()							true
	#define HFSM2_IF_TRANSITION_HISTORY(...)						 __VA_ARGS__
	#define HFSM2_TRANSITION_HISTORY_MASK								(1 << 5)
#else
	#define HFSM2_TRANSITION_HISTORY_AVAILABLE()						   false
	#define HFSM2_IF_TRANSITION_HISTORY(...)
	#define HFSM2_TRANSITION_HISTORY_MASK								(0 << 5)
#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	#define HFSM2_UTILITY_THEORY_AVAILABLE()								true
	#define HFSM2_IF_UTILITY_THEORY(...)							 __VA_ARGS__
	#define HFSM2_UTILITY_THEORY_MASK									(1 << 6)
#else
	#define HFSM2_UTILITY_THEORY_AVAILABLE()							   false
	#define HFSM2_IF_UTILITY_THEORY(...)
	#define HFSM2_UTILITY_THEORY_MASK									(0 << 6)
#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_VERBOSE_DEBUG_LOG
	#define HFSM2_ENABLE_LOG_INTERFACE

	#define HFSM2_VERBOSE_DEBUG_LOG_AVAILABLE()								true
	#define HFSM2_VERBOSE_DEBUG_LOG_MASK								(1 << 7)
#else
	#define HFSM2_VERBOSE_DEBUG_LOG_AVAILABLE()							   false
	#define HFSM2_VERBOSE_DEBUG_LOG_MASK								(0 << 7)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	#define HFSM2_LOG_INTERFACE_AVAILABLE()									true
	#define HFSM2_IF_LOG_INTERFACE(...)								 __VA_ARGS__
	#define HFSM2_LOG_INTERFACE_MASK									(1 << 8)

	#define HFSM2_LOG_TRANSITION(CONTEXT, ORIGIN, TYPE, DESTINATION)		   \
		if (_logger)														   \
			_logger->recordTransition(CONTEXT, ORIGIN, TYPE, DESTINATION)

#if HFSM2_PLANS_AVAILABLE()
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

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	#define HFSM2_LOG_UTILITY_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)		   \
		if (auto* const logger = control._logger)							   \
			logger->recordUtilityResolution(CONTEXT, HEAD, PRONG, UTILITY)

	#define HFSM2_LOG_RANDOM_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)		   \
		if (auto* const logger = control._logger)							   \
			logger->recordRandomResolution(CONTEXT, HEAD, PRONG, UTILITY)
#endif

#else

	#define HFSM2_LOG_INTERFACE_AVAILABLE()								   false
	#define HFSM2_IF_LOG_INTERFACE(...)
	#define HFSM2_LOG_INTERFACE_MASK									(0 << 8)

	#define HFSM2_LOG_TRANSITION(CONTEXT, ORIGIN, TYPE, DESTINATION)

#if HFSM2_PLANS_AVAILABLE()
	#define HFSM2_LOG_TASK_STATUS(CONTEXT, REGION, ORIGIN, STATUS)
	#define HFSM2_LOG_PLAN_STATUS(CONTEXT, REGION, STATUS)
#endif

	#define HFSM2_LOG_CANCELLED_PENDING(CONTEXT, ORIGIN)

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	#define HFSM2_LOG_UTILITY_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)
	#define HFSM2_LOG_RANDOM_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)
#endif

#endif

////////////////////////////////////////////////////////////////////////////////

#if HFSM2_VERBOSE_DEBUG_LOG_AVAILABLE()

	#define HFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)						   \
		if (auto* const logger = control._logger)							   \
			logger->recordMethod(control.context(), STATE_ID, METHOD_ID)

#elif HFSM2_LOG_INTERFACE_AVAILABLE()

	#define HFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)						   \
		if (auto* const logger = control._logger)							   \
			log(METHOD, *logger, control.context(), METHOD_ID)

#else

	#define HFSM2_LOG_STATE_METHOD(METHOD, METHOD_ID)

#endif

////////////////////////////////////////////////////////////////////////////////

namespace hfsm2 {

using FeatureTag = uint16_t;

constexpr FeatureTag HFSM2_FEATURE_TAG = HFSM2_TYPEINDEX_MASK
									   | HFSM2_DEBUG_STATE_TYPE_MASK
									   | HFSM2_PLANS_MASK
									   | HFSM2_SERIALIZATION_MASK
									   | HFSM2_STRUCTURE_REPORT_MASK
									   | HFSM2_TRANSITION_HISTORY_MASK
									   | HFSM2_UTILITY_THEORY_MASK
									   | HFSM2_VERBOSE_DEBUG_LOG_MASK
									   | HFSM2_LOG_INTERFACE_MASK;

}

//------------------------------------------------------------------------------

#undef HFSM2_TYPEINDEX_MASK
#undef HFSM2_DEBUG_STATE_TYPE_MASK
#undef HFSM2_PLANS_MASK
#undef HFSM2_SERIALIZATION_MASK
#undef HFSM2_STRUCTURE_REPORT_MASK
#undef HFSM2_TRANSITION_HISTORY_MASK
#undef HFSM2_UTILITY_THEORY_MASK
#undef HFSM2_VERBOSE_DEBUG_LOG_MASK
#undef HFSM2_LOG_INTERFACE_MASK

////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER == 1900
	#pragma warning(push)
	#pragma warning(disable: 4814) // in C++14 'constexpr' will not imply 'const'; consider explicitly specifying 'const'
#endif

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wextra-semi" // error : extra ';' inside a class
	#pragma clang diagnostic ignored "-Wconstexpr-not-const" // error: 'constexpr' non-static member function will not be implicitly 'const' in C++14; add 'const' to avoid a change in behavior
#endif

#if defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic" // error : extra ';'
#endif

//------------------------------------------------------------------------------
