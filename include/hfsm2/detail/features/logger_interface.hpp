namespace hfsm2 {

#if HFSM2_LOG_INTERFACE_AVAILABLE()

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFeatureTag = HFSM2_FEATURE_TAG
		, typename TContext = EmptyContext
		HFSM2_IF_UTILITY_THEORY(, typename TUtilty = float)>
struct LoggerInterfaceT {
	using Context		 = TContext;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Utilty		 = TUtilty;
#endif

	using Method		 = ::hfsm2::Method;
	using StateID		 = ::hfsm2::StateID;
	using RegionID		 = ::hfsm2::RegionID;
	using TransitionType = ::hfsm2::TransitionType;

#if HFSM2_PLANS_AVAILABLE()
	using StatusEvent	 = ::hfsm2::StatusEvent;
#endif

	HFSM2_CONSTEXPR(NO)
	virtual
	void recordMethod(Context& HFSM2_UNUSED(context),
					  const StateID HFSM2_UNUSED(origin),
					  const Method HFSM2_UNUSED(method))			  noexcept
	{}

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordTransition(Context& HFSM2_UNUSED(context),
					 const StateID HFSM2_UNUSED(origin),
					 const TransitionType HFSM2_UNUSED(transitionType),
					 const StateID HFSM2_UNUSED(target))			  noexcept
	{}

#if HFSM2_PLANS_AVAILABLE()

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordTaskStatus(Context& HFSM2_UNUSED(context),
					 const RegionID HFSM2_UNUSED(region),
					 const StateID HFSM2_UNUSED(origin),
					 const StatusEvent HFSM2_UNUSED(event))			  noexcept
	{}

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordPlanStatus(Context& HFSM2_UNUSED(context),
					 const RegionID HFSM2_UNUSED(region),
					 const StatusEvent HFSM2_UNUSED(event))			  noexcept
	{}

#endif

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordCancelledPending(Context& HFSM2_UNUSED(context),
						   const StateID HFSM2_UNUSED(origin))		  noexcept
	{}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordUtilityResolution(Context& HFSM2_UNUSED(context),
							const StateID HFSM2_UNUSED(head),
							const StateID HFSM2_UNUSED(prong),
							const Utilty HFSM2_UNUSED(utilty))		  noexcept
	{}

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordRandomResolution(Context& HFSM2_UNUSED(context),
						   const StateID HFSM2_UNUSED(head),
						   const StateID HFSM2_UNUSED(prong),
						   const Utilty HFSM2_UNUSED(utilty))		  noexcept
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
