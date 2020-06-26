namespace hfsm2 {

#ifdef HFSM2_ENABLE_LOG_INTERFACE

////////////////////////////////////////////////////////////////////////////////

template <typename TContext = EmptyContext

#ifdef HFSM2_ENABLE_UTILITY_THEORY
		, typename TUtilty = float
#endif

		  >
struct LoggerInterfaceT {
	using Context		 = TContext;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Utilty		 = TUtilty;
#endif

	using Method		 = ::hfsm2::Method;
	using StateID		 = ::hfsm2::StateID;
	using RegionID		 = ::hfsm2::RegionID;
	using TransitionType = ::hfsm2::TransitionType;
	using StatusEvent	 = ::hfsm2::StatusEvent;

	virtual void recordMethod(Context& /*context*/,
							  const StateID /*origin*/,
							  const Method /*method*/)
	{}

	virtual void recordTransition(Context& /*context*/,
								  const StateID /*origin*/,
								  const TransitionType /*transitionType*/,
								  const StateID /*target*/)
	{}

	virtual void recordTaskStatus(Context& /*context*/,
								  const RegionID /*region*/,
								  const StateID /*origin*/,
								  const StatusEvent /*event*/)
	{}

	virtual void recordPlanStatus(Context& /*context*/,
								  const RegionID /*region*/,
								  const StatusEvent /*event*/)
	{}

	virtual void recordCancelledPending(Context& /*context*/,
										const StateID /*origin*/)
	{}

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	virtual void recordUtilityResolution(Context& /*context*/,
										 const StateID /*head*/,
										 const StateID /*prong*/,
										 const Utilty /*utilty*/)
	{}

	virtual void recordRandomResolution(Context& /*context*/,
										const StateID /*head*/,
										const StateID /*prong*/,
										const Utilty /*utilty*/)
	{}

#endif
};

////////////////////////////////////////////////////////////////////////////////

#else

template <typename TContext = EmptyContext,
		  typename TUtilty = float>
using LoggerInterfaceT = void;

#endif

using LoggerInterface = LoggerInterfaceT<>;

}
