#pragma once

#ifdef HFSM_ENABLE_LOG_INTERFACE

namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

struct LoggerInterface {
	using Method	 = ::hfsm::Method;
	using Transition = ::hfsm::Transition;

	virtual
	void
	recordMethod(const std::type_index state,
				 const Method method) = 0;

	virtual
	void
	recordTransition(const Transition transition,
					 const std::type_index state) = 0;
};

////////////////////////////////////////////////////////////////////////////////

}

#else

namespace hfsm {

using LoggerInterface = void;

}

#endif
