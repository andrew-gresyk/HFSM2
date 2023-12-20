#if HFSM2_PLANS_AVAILABLE()

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class PayloadPlanT;

//------------------------------------------------------------------------------

template <
	typename TConfig
  , typename TStateList
  , typename TRegionList
  , Long NCompoCount
  , Long NOrthoCount
  , Long NOrthoUnits
  HFSM2_IF_SERIALIZATION(, Long NSerialBits)
  , Long NTaskCapacity
  , typename TPayload
>
class PayloadPlanT<
		  ArgsT<
			  TConfig
			, TStateList
			, TRegionList
			, NCompoCount
			, NOrthoCount
			, NOrthoUnits
			HFSM2_IF_SERIALIZATION(, NSerialBits)
			, NTaskCapacity
			, TPayload
		  >
	  > final
	: public PlanT<
				 ArgsT<
					 TConfig
				   , TStateList
				   , TRegionList
				   , NCompoCount
				   , NOrthoCount
				   , NOrthoUnits
				   HFSM2_IF_SERIALIZATION(, NSerialBits)
				   , NTaskCapacity
				   , TPayload
				 >
			 >
{
	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	template <typename, typename>
	friend class R_;

	using Args = ArgsT<
					 TConfig
				   , TStateList
				   , TRegionList
				   , NCompoCount
				   , NOrthoCount
				   , NOrthoUnits
				   HFSM2_IF_SERIALIZATION(, NSerialBits)
				   , NTaskCapacity
				   , TPayload
				 >;

	using Payload		= typename Args::Payload;

	static constexpr Long  TASK_CAPACITY	= Args::TASK_CAPACITY;

	using PlanBase		= PlanT<Args>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using PlanBase::PlanBase;

	using PlanBase::linkTask;

	HFSM2_CONSTEXPR(14)	bool  append(const StateID origin,
									 const StateID destination,
									 const TransitionType type,
									 const Payload& payload)			noexcept;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, acts depending on the region type)
	/// @param `origin` Origin state identifier
	/// @param `destination` Destination state identifier
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool changeWith   (const StateID origin,
										   const StateID destination,
										   const Payload& payload)		noexcept	{ return append(origin								 , destination								 , TransitionType::CHANGE   ,	   payload );	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool changeWith   (const StateID destination,
										   const Payload& payload)		noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::CHANGE   ,	   payload );	}

	/// @brief Prepend a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam `TOrigin` Origin state type
	/// @tparam `TDestination` Destination state type
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool changeWith   (const Payload& payload)		noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::CHANGE   ,	   payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the initial state)
	/// @param `origin` Origin state identifier
	/// @param `destination` Destination state identifier
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool restartWith  (const StateID origin,
										   const StateID destination,
										   const Payload& payload)		noexcept	{ return append(origin								 , destination								 , TransitionType::RESTART  ,	   payload );	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the initial state)
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool restartWith  (const StateID destination,
										   const Payload& payload)		noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::RESTART  ,	   payload );	}

	/// @brief Prepend a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the initial state)
	/// @tparam `TOrigin` Origin state type
	/// @tparam `TDestination` Destination state type
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool restartWith  (const Payload& payload)		noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::RESTART  ,	   payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param `origin` Origin state identifier
	/// @param `destination` Destination state identifier
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool resumeWith   (const StateID origin,
										   const StateID destination,
										   const Payload& payload)		noexcept	{ return append(origin								 , destination								 , TransitionType::RESUME   ,	   payload );	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool resumeWith   (const StateID destination,
										   const Payload& payload)		noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::RESUME   ,	   payload );	}

	/// @brief Prepend a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam `TOrigin` Origin state type
	/// @tparam `TDestination` Destination state type
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool resumeWith   (const Payload& payload)		noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::RESUME   ,	   payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the sub-state by index returned by the region's `select()` method)
	/// @param `origin` Origin state identifier
	/// @param `destination` Destination state identifier
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool selectWith   (const StateID origin,
										   const StateID destination,
										   const Payload& payload)		noexcept	{ return append(origin								 , destination								 , TransitionType::SELECT   ,	   payload );	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the sub-state by index returned by the region's `select()` method)
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool selectWith   (const StateID destination,
										   const Payload& payload)		noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::SELECT   ,	   payload );	}

	/// @brief Prepend a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the sub-state by index returned by the region's `select()` method)
	/// @tparam `TOrigin` Origin state type
	/// @tparam `TDestination` Destination state type
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool selectWith   (const Payload& payload)		noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::SELECT   ,	   payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the state with the highest `utility()`
	///   among those with the highest `rank()`)
	/// @param `origin` Origin state identifier
	/// @param `destination` Destination state identifier
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	HFSM2_CONSTEXPR(14)	bool utilizeWith  (const StateID origin,
										   const StateID destination,
										   const Payload& payload)		noexcept	{ return append(origin								 , destination								 , TransitionType::UTILIZE  ,	   payload );	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the state with the highest `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool utilizeWith  (const StateID destination,
										   const Payload& payload)		noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::UTILIZE  ,	   payload );	}

	/// @brief Prepend a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the state with the highest `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TOrigin` Origin state type
	/// @tparam `TDestination` Destination state type
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool utilizeWith  (const Payload& payload)		noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::UTILIZE  ,	   payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, uses weighted random to activate the state proportional to `utility()`
	///   among those with the highest `rank()`)
	/// @param `origin` Origin state identifier
	/// @param `destination` Destination state identifier
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	HFSM2_CONSTEXPR(14)	bool randomizeWith(const StateID origin,
										   const StateID destination,
										   const Payload& payload)		noexcept	{ return append(origin								 , destination								 , TransitionType::RANDOMIZE,	   payload );	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, uses weighted random to activate the state proportional to `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool randomizeWith(const StateID destination,
										   const Payload& payload)		noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::RANDOMIZE,	   payload );	}

	/// @brief Prepend a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, uses weighted random to activate the state proportional to `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TOrigin` Origin state type
	/// @tparam `TDestination` Destination state type
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool randomizeWith(const Payload& payload)		noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::RANDOMIZE,	   payload );	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Append a task to schedule a transition to `destination` if `origin` completes with `success()`
	/// @param `origin` Origin state identifier
	/// @param `destination` Destination state identifier
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool scheduleWith (const StateID origin,
										   const StateID destination,
										   const Payload& payload)		noexcept	{ return append(origin								 , destination								 , TransitionType::SCHEDULE ,	   payload );	}

	/// @brief Append a task to schedule a transition to `destination` if `origin` completes with `success()`
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool scheduleWith (const StateID destination,
										   const Payload& payload)		noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::SCHEDULE ,	   payload );	}

	/// @brief Prepend a task to schedule a transition to `destination` if `origin` completes with `success()`
	/// @tparam `TOrigin` Origin state type
	/// @tparam `TDestination` Destination state type
	/// @param `payload` Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool scheduleWith (const Payload& payload)		noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::SCHEDULE ,	   payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	using PlanBase::_planData;
	using PlanBase::_regionId;
};

//------------------------------------------------------------------------------

template <
	typename TConfig
  , typename TStateList
  , typename TRegionList
  , Long NCompoCount
  , Long NOrthoCount
  , Long NOrthoUnits
  HFSM2_IF_SERIALIZATION(, Long NSerialBits)
  , Long NTaskCapacity
>
class PayloadPlanT<
		  ArgsT<
			  TConfig
			, TStateList
			, TRegionList
			, NCompoCount
			, NOrthoCount
			, NOrthoUnits
			HFSM2_IF_SERIALIZATION(, NSerialBits)
			, NTaskCapacity
			, void
		  >
	  > final
	: public PlanT<
				 ArgsT<
					 TConfig
				   , TStateList
				   , TRegionList
				   , NCompoCount
				   , NOrthoCount
				   , NOrthoUnits
				   HFSM2_IF_SERIALIZATION(, NSerialBits)
				   , NTaskCapacity
				   , void
				 >
			 >
{
	template <typename, typename>
	friend class R_;

	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	using Args = ArgsT<
					 TConfig
				   , TStateList
				   , TRegionList
				   , NCompoCount
				   , NOrthoCount
				   , NOrthoUnits
				   HFSM2_IF_SERIALIZATION(, NSerialBits)
				   , NTaskCapacity
				   , void
				 >;

	using PlanBase = PlanT<Args>;

	using PlanBase::PlanBase;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#include "plan_2.inl"
