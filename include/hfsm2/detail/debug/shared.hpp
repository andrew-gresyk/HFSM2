#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace hfsm2 {

enum class Method : ShortIndex {
	ENTRY_GUARD,
	ENTER,
	UPDATE,
	REACT,
	EXIT_GUARD,
	EXIT,
	PLAN_SUCCEEDED,
	PLAN_FAILED,

	COUNT
};

enum class Transition : ShortIndex {
	CHANGE,
	RESTART,
	RESUME,
	SCHEDULE,

	COUNT
};

enum class StatusEvent : ShortIndex {
	SUCCEEDED,
	FAILED,

	COUNT
};

//------------------------------------------------------------------------------

static inline
const char*
stateName(const std::type_index stateType) {
	const char* const raw = stateType.name();

	#if defined(_MSC_VER)

		auto first =
			raw[0] == 's' ? 7 : // Struct
			raw[0] == 'c' ? 6 : // Class
							0;
		return raw + first;

	#elif defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)

		return raw;

	#else

		return raw;

	#endif
}

//------------------------------------------------------------------------------

static inline
const char*
methodName(const Method method) {
	switch (method) {
		case Method::ENTRY_GUARD:		return "entryGuard";
		case Method::ENTER:				return "enter";
		case Method::UPDATE:			return "update";
		case Method::REACT:				return "react";
		case Method::EXIT_GUARD:		return "exitGuard";
		case Method::EXIT:				return "exit";
		case Method::PLAN_SUCCEEDED:	return "planSucceeded";
		case Method::PLAN_FAILED:		return "planFailed";

		default:
			HFSM_BREAK();
			return nullptr;
	}
}

//------------------------------------------------------------------------------

static inline
const char*
transitionName(const Transition transition) {
	switch (transition) {
		case Transition::CHANGE:		return "changeTo";
		case Transition::RESTART:		return "restart";
		case Transition::RESUME:		return "resume";
		case Transition::SCHEDULE:		return "schedule";

		default:
			HFSM_BREAK();
			return nullptr;
	}
}

////////////////////////////////////////////////////////////////////////////////

}
