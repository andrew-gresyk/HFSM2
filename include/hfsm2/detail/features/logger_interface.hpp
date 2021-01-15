namespace hfsm2 {

#ifdef HFSM2_ENABLE_LOG_INTERFACE

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFeatureTag = HFSM2_FEATURE_TAG
		, typename TContext = EmptyContext
		HFSM2_IF_UTILITY_THEORY(, typename TUtilty = float)>
struct LoggerInterfaceT {
	using Context		 = TContext;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Utilty		 = TUtilty;
#endif

	using Method		 = ::hfsm2::Method;
	using StateID		 = ::hfsm2::StateID;
	using RegionID		 = ::hfsm2::RegionID;
	using TransitionType = ::hfsm2::TransitionType;

#ifdef HFSM2_ENABLE_PLANS
	using StatusEvent	 = ::hfsm2::StatusEvent;
#endif

	virtual void recordMethod(Context& /*context*/,
							  const StateID /*origin*/,
							  const Method /*method*/) noexcept
	{}

	virtual void recordTransition(Context& /*context*/,
								  const StateID /*origin*/,
								  const TransitionType /*transitionType*/,
								  const StateID /*target*/) noexcept
	{}

#ifdef HFSM2_ENABLE_PLANS

	virtual void recordTaskStatus(Context& /*context*/,
								  const RegionID /*region*/,
								  const StateID /*origin*/,
								  const StatusEvent /*event*/) noexcept
	{}

	virtual void recordPlanStatus(Context& /*context*/,
								  const RegionID /*region*/,
								  const StatusEvent /*event*/) noexcept
	{}

#endif

	virtual void recordCancelledPending(Context& /*context*/,
										const StateID /*origin*/) noexcept
	{}

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	virtual void recordUtilityResolution(Context& /*context*/,
										 const StateID /*head*/,
										 const StateID /*prong*/,
										 const Utilty /*utilty*/) noexcept
	{}

	virtual void recordRandomResolution(Context& /*context*/,
										const StateID /*head*/,
										const StateID /*prong*/,
										const Utilty /*utilty*/) noexcept
	{}

#endif
};

////////////////////////////////////////////////////////////////////////////////

#else

template <FeatureTag NFeatureTag = HFSM2_FEATURE_TAG
		, typename TContext = EmptyContext
		HFSM2_IF_UTILITY_THEORY(, typename TUtilty = float)>
using LoggerInterfaceT = void;

#endif

using LoggerInterface = LoggerInterfaceT<>;

}
