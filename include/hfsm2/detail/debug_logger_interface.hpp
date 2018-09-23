#pragma once

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG

namespace hfsm2 {

////////////////////////////////////////////////////////////////////////////////

struct LoggerInterface {
	using Method	  = ::hfsm2::Method;
	using StateID	  = ::hfsm2::StateID;
	using RegionID	  = ::hfsm2::RegionID;
	using Transition  = ::hfsm2::Transition;
	using StatusEvent = ::hfsm2::StatusEvent;

	virtual void recordMethod(const StateID /*origin*/,
							  const Method /*method*/)
	{}

	virtual void recordTransition(const StateID /*origin*/,
								  const Transition /*transition*/,
								  const StateID /*target*/)
	{}

	virtual void recordTaskStatus(const RegionID /*region*/,
								  const StateID /*origin*/,
								  const StatusEvent /*event*/)
	{}

	virtual void recordPlanStatus(const RegionID /*region*/,
								  const StatusEvent /*event*/)
	{}
};

////////////////////////////////////////////////////////////////////////////////

}

#else

namespace hfsm2 {

using LoggerInterface = void;

}

#endif
