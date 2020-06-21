////////////////////////////////////////////////////////////////////////////////

#undef HFSM_INLINE

#undef HFSM_64BIT_OR_32BIT

//#undef HFSM_BREAK

#undef HFSM_IF_DEBUG
#undef HFSM_UNLESS_DEBUG
#undef HFSM_DEBUG_OR

#undef HFSM_IF_ASSERT
#undef HFSM_CHECKED
#undef HFSM_ASSERT
#undef HFSM_ASSERT_OR

#undef HFSM_EXPLICIT_MEMBER_SPECIALIZATION

#undef HFSM_IF_UTILITY_THEORY
#undef HFSM_IF_SERIALIZATION
#undef HFSM_IF_TRANSITION_HISTORY
#undef HFSM_IF_STRUCTURE

#undef HFSM_IF_LOG_INTERFACE
#undef HFSM_LOG_TRANSITION
#undef HFSM_LOG_TASK_STATUS
#undef HFSM_LOG_PLAN_STATUS
#undef HFSM_LOG_CANCELLED_PENDING

#undef HFSM_LOG_STATE_METHOD

////////////////////////////////////////////////////////////////////////////////

#if defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
#endif

#ifdef __clang__
	#pragma clang diagnostic pop
#endif

//------------------------------------------------------------------------------
