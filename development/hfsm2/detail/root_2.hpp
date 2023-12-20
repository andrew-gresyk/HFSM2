namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <
	typename
  , typename
>
class RP_;

//------------------------------------------------------------------------------
// Non-'void' payloads

template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
  , typename TReactOrder

#if HFSM2_UTILITY_THEORY_AVAILABLE()
  , typename TRank
  , typename TUtility
  , typename TRNG
#endif

  , Short NSubstitutionLimit
  HFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
  , typename TApex
>
class HFSM2_EMPTY_BASES RP_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
	: public			RV_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
{
	using Base =		RV_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

	using Transition			= TransitionT<TPayload>;

public:
	using typename Base::Payload;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

public:
	using Base::Base;
	using Base::processRequest;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get state identifier for a state type
	/// @tparam `TState` State type
	/// @return Numeric state identifier
	template <typename TState>
	static
	HFSM2_CONSTEXPR(11) StateID  stateId()											noexcept	{ return Base::template stateId<TState>();		}

	/// @brief Get region identifier for a region type
	/// @tparam `TState` Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static
	HFSM2_CONSTEXPR(11) RegionID regionId()											noexcept	{ return Base::template regionId<TState>();		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, acts depending on the region type)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	HFSM2_CONSTEXPR(14)	void changeWith				(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void changeWith				(const Payload& payload)		noexcept	{ changeWith   (stateId<TState>(), payload );	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, activates the initial state)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	HFSM2_CONSTEXPR(14)	void restartWith			(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, activates the initial state)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void restartWith			(const Payload& payload)		noexcept	{ restartWith  (stateId<TState>(), payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	HFSM2_CONSTEXPR(14)	void resumeWith				(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void resumeWith				(const Payload& payload)		noexcept	{ resumeWith   (stateId<TState>(), payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, activates the sub-state by index returned by the region's `select()` method)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	HFSM2_CONSTEXPR(14)	void selectWith				(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, activates the sub-state by index returned by the region's `select()` method)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void selectWith				(const Payload& payload)		noexcept	{ selectWith   (stateId<TState>(), payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, activates the state with the highest `utility()`
	///   among those with the highest `rank()`)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	HFSM2_CONSTEXPR(14)	void utilizeWith			(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, activates the state with the highest `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void utilizeWith			(const Payload& payload)		noexcept	{ utilizeWith  (stateId<TState>(), payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, uses weighted random to activate the state proportional to `utility()`
	///   among those with the highest `rank()`)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	HFSM2_CONSTEXPR(14)	void randomizeWith			(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, uses weighted random to activate the state proportional to `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void randomizeWith			(const Payload& payload)		noexcept	{ randomizeWith(stateId<TState>(), payload );	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	HFSM2_CONSTEXPR(14)	void scheduleWith			(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void scheduleWith			(const Payload& payload)		noexcept	{ scheduleWith (stateId<TState>(), payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Transition into a state
	///   (if transitioning into a region, acts depending on the region type)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	/// @see `changeWith()`
	HFSM2_CONSTEXPR(14)	void immediateChangeWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	/// @see `changeWith()`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateChangeWith	(const Payload& payload)		noexcept	{ immediateChangeWith   (stateId<TState>(), payload );	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	/// @see `restartWith()`
	HFSM2_CONSTEXPR(14)	void immediateRestartWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	/// @see `restartWith()`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateRestartWith	(const Payload& payload)		noexcept	{ immediateRestartWith	(stateId<TState>(), payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	/// @see `resumeWith()`
	HFSM2_CONSTEXPR(14)	void immediateResumeWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	/// @see `resumeWith()`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateResumeWith	(const Payload& payload)		noexcept	{ immediateResumeWith	(stateId<TState>(), payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's `select()` method)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	/// @see `selectWith()`
	HFSM2_CONSTEXPR(14)	void immediateSelectWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's `select()` method)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	/// @see `selectWith()`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateSelectWith	(const Payload& payload)		noexcept	{ immediateSelectWith	(stateId<TState>(), payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest `utility()`
	///   among those with the highest `rank()`)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	/// @see `utilizeWith()`
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	HFSM2_CONSTEXPR(14)	void immediateUtilizeWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	/// @see `utilizeWith()`
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateUtilizeWith	(const Payload& payload)		noexcept	{ immediateUtilizeWith	(stateId<TState>(), payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, uses weighted random to activate the state proportional to `utility()`
	///   among those with the highest `rank()`)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	/// @see `randomizeWith()`
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	HFSM2_CONSTEXPR(14)	void immediateRandomizeWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, uses weighted random to activate the state proportional to `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	/// @see `randomizeWith()`
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateRandomizeWith	(const Payload& payload)		noexcept	{ immediateRandomizeWith(stateId<TState>(), payload );	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using Base::_core;
};

//------------------------------------------------------------------------------

template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
  , typename TReactOrder

#if HFSM2_UTILITY_THEORY_AVAILABLE()
  , typename TRank
  , typename TUtility
  , typename TRNG
#endif

  , Short NSubstitutionLimit
  HFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TApex
>
class HFSM2_EMPTY_BASES RP_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , void
							>
						  , TApex
						>
	: public			RV_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , void
							>
						  , TApex
						>
{
	using Base =		RV_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , void
							>
						  , TApex
						>;

public:
#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

	using Base::Base;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "root_2.inl"
