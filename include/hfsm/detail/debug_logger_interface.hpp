#pragma once

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG

namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

struct LoggerInterface {
	using Method	 = Method;
	using StateID	 = ::hfsm::StateID;
	using Transition = Transition;

	virtual
	void
	recordMethod(const StateID origin,
				 const Method method) = 0;

	virtual
	void
	recordTransition(const StateID origin,
					 const Transition transition,
					 const StateID target) = 0;
};

////////////////////////////////////////////////////////////////////////////////

}

#else

namespace hfsm {

using LoggerInterface = void;

}

#endif
