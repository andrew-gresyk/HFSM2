//------------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
#endif

#ifdef __clang__
	#pragma clang diagnostic pop
#endif

#if _MSC_VER == 1900
	#pragma warning(pop)
#endif

////////////////////////////////////////////////////////////////////////////////

#undef HFSM2_UNUSED

//------------------------------------------------------------------------------

#undef HFSM2_ATTRIBUTE

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#undef HFSM2_ATTRIBUTE_FALLTHROUGH

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#undef HFSM2_ATTRIBUTE_NO_UNIQUE_ADDRESS

//------------------------------------------------------------------------------

#undef HFSM2_CONSTEXPR

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#undef HFSM2_CONSTEXPR_NO

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#undef HFSM2_CONSTEXPR_11

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#undef HFSM2_CONSTEXPR_14

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#undef HFSM2_CONSTEXPR_17

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#undef HFSM2_CONSTEXPR_20

//------------------------------------------------------------------------------

//#undef HFSM2_EMPTY_BASES

//------------------------------------------------------------------------------

//#undef HFSM2_ARCHITECTURE
//#undef HFSM2_ARCHITECTURE_64
//#undef HFSM2_ARCHITECTURE_32
#undef HFSM2_64BIT_OR_32BIT

//------------------------------------------------------------------------------

//#undef HFSM2_BREAK
#undef HFSM2_BREAK_AVAILABLE

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#undef HFSM2_IF_DEBUG
#undef HFSM2_UNLESS_DEBUG
#undef HFSM2_DEBUG_OR

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//#undef HFSM2_ASSERT_AVAILABLE
#undef HFSM2_IF_ASSERT
//#undef HFSM2_CHECKED
#undef HFSM2_ASSERT
#undef HFSM2_ASSERT_OR

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#undef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE

////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------

#undef HFSM2_IF_TYPEINDEX
#undef HFSM2_TYPEINDEX_AVAILABLE
#undef HFSM2_IF_TYPEINDEX

//------------------------------------------------------------------------------

//#undef HFSM2_DEBUG_STATE_TYPE_AVAILABLE

//------------------------------------------------------------------------------

#undef HFSM2_PLANS_AVAILABLE
#undef HFSM2_IF_PLANS

//------------------------------------------------------------------------------

#undef HFSM2_SERIALIZATION_AVAILABLE
#undef HFSM2_IF_SERIALIZATION

//------------------------------------------------------------------------------

#undef HFSM2_STRUCTURE_REPORT_AVAILABLE
#undef HFSM2_IF_STRUCTURE_REPORT

//------------------------------------------------------------------------------

#undef HFSM2_TRANSITION_HISTORY_AVAILABLE
#undef HFSM2_IF_TRANSITION_HISTORY

//------------------------------------------------------------------------------

#undef HFSM2_UTILITY_THEORY_AVAILABLE
#undef HFSM2_IF_UTILITY_THEORY

////////////////////////////////////////////////////////////////////////////////

#undef HFSM2_VERBOSE_DEBUG_LOG_AVAILABLE

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#undef HFSM2_LOG_INTERFACE_AVAILABLE
#undef HFSM2_IF_LOG_INTERFACE

#undef HFSM2_LOG_TRANSITION

#ifdef HFSM2_ENABLE_PLANS
	#undef HFSM2_LOG_TASK_STATUS
	#undef HFSM2_LOG_PLAN_STATUS
#endif

#undef HFSM2_LOG_CANCELLED_PENDING

#ifdef HFSM2_UTILITY_THEORY_AVAILABLE
	#undef HFSM2_LOG_UTILITY_RESOLUTION
	#undef HFSM2_LOG_RANDOM_RESOLUTION
#endif

#undef HFSM2_LOG_STATE_METHOD

////////////////////////////////////////////////////////////////////////////////
