#pragma once

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG

namespace hfsm2 {

////////////////////////////////////////////////////////////////////////////////

template <typename TUtilty = float>
struct LoggerInterfaceT {
	using Utilty	  = TUtilty;

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

	virtual void recordCancelledPending(const StateID /*origin*/) {}

	virtual void recordUtilityResolution(const StateID /*head*/,
										 const StateID /*prong*/,
										 const Utilty /*utilty*/)
	{}
};

using LoggerInterface = LoggerInterfaceT<float>;

////////////////////////////////////////////////////////////////////////////////

}

#else

namespace hfsm2 {

template <typename = float>
using LoggerInterfaceT = void;

}

#endif
