namespace hfsm2 {

//------------------------------------------------------------------------------

struct HFSM2_EMPTY_BASES EmptyContext {};

#if HFSM2_LOG_INTERFACE_AVAILABLE()

////////////////////////////////////////////////////////////////////////////////

template <
	FeatureTag NFeatureTag = HFSM2_FEATURE_TAG
  , typename TContext = EmptyContext
  HFSM2_IF_UTILITY_THEORY(, typename TUtilty = float)
>
struct LoggerInterfaceT {
	using Context		 = TContext;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Utilty		 = TUtilty;
#endif

	using Method		 = ::hfsm2::Method;
	using Prong			 = ::hfsm2::Prong;
	using StateID		 = ::hfsm2::StateID;
	using TransitionType = ::hfsm2::TransitionType;

#if HFSM2_PLANS_AVAILABLE()
	using StatusEvent	 = ::hfsm2::StatusEvent;
#endif

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordMethod(const Context& HFSM2_UNUSED(context),
				 const StateID HFSM2_UNUSED(origin),
				 const Method HFSM2_UNUSED(method))
	{}

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordTransition(const Context& HFSM2_UNUSED(context),
					 const StateID HFSM2_UNUSED(origin),
					 const TransitionType HFSM2_UNUSED(transitionType),
					 const StateID HFSM2_UNUSED(target))
	{}

#if HFSM2_PLANS_AVAILABLE()

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordTaskStatus(const Context& HFSM2_UNUSED(context),
					 const StateID HFSM2_UNUSED(region),
					 const StateID HFSM2_UNUSED(origin),
					 const StatusEvent HFSM2_UNUSED(event))
	{}

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordPlanStatus(const Context& HFSM2_UNUSED(context),
					 const StateID HFSM2_UNUSED(region),
					 const StatusEvent HFSM2_UNUSED(event))
	{}

#endif

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordCancelledPending(const Context& HFSM2_UNUSED(context),
						   const StateID HFSM2_UNUSED(origin))
	{}

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordSelectResolution(const Context& HFSM2_UNUSED(context),
						   const StateID HFSM2_UNUSED(head),
						   const Prong HFSM2_UNUSED(prong))
	{}

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordUtilityResolution(const Context& HFSM2_UNUSED(context),
							const StateID HFSM2_UNUSED(head),
							const Prong HFSM2_UNUSED(prong),
							const Utilty HFSM2_UNUSED(utilty))
	{}

	HFSM2_CONSTEXPR(NO)
	virtual
	void
	recordRandomResolution(const Context& HFSM2_UNUSED(context),
						   const StateID HFSM2_UNUSED(head),
						   const Prong HFSM2_UNUSED(prong),
						   const Utilty HFSM2_UNUSED(utilty))
	{}

#endif
};

////////////////////////////////////////////////////////////////////////////////

#else

template <
	FeatureTag NFeatureTag = HFSM2_FEATURE_TAG
  , typename TContext = EmptyContext
  HFSM2_IF_UTILITY_THEORY(, typename TUtilty = float)
>
using LoggerInterfaceT = void;

#endif

using LoggerInterface = LoggerInterfaceT<>;

}
