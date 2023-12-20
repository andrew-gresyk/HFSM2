namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class FullControlBaseT
	: public PlanControlT<TArgs>
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename, Strategy, typename, typename...>
	friend struct C_;

	template <typename, typename, typename, typename...>
	friend struct O_;

	template <typename, typename>
	friend class R_;

protected:
	using PlanControl	= PlanControlT<TArgs>;

	using typename PlanControl::StateList;
	using typename PlanControl::RegionList;

	using typename PlanControl::Transition;

#if HFSM2_PLANS_AVAILABLE()
	using TasksBits		= BitArrayT<StateList::SIZE>;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Lock final {
		HFSM2_CONSTEXPR(14)	 Lock(FullControlBaseT& control_)		noexcept;
		HFSM2_CONSTEXPR(20)	~Lock()									noexcept;

		FullControlBaseT* const control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using PlanControl::PlanControl;

#if HFSM2_PLANS_AVAILABLE()

	template <typename TState>
	HFSM2_CONSTEXPR(14)	TaskStatus buildPlanStatus()				noexcept;

#endif

public:
	using PlanControl::context;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Transition into a state
	///   (if transitioning into a region, acts depending on the region type)
	/// @param `stateId` State identifier
	HFSM2_CONSTEXPR(14)	void changeTo (const StateID stateId_)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam `TState` State type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void changeTo ()							noexcept	{ changeTo (PlanControl::template stateId<TState>());	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the initial state)
	/// @param `stateId` State identifier
	HFSM2_CONSTEXPR(14)	void restart  (const StateID stateId_)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the initial state)
	/// @tparam `TState` State type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void restart  ()							noexcept	{ restart  (PlanControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param `stateId` State identifier
	HFSM2_CONSTEXPR(14)	void resume   (const StateID stateId_)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam `TState` State type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void resume   ()							noexcept	{ resume   (PlanControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's `select()` method)
	/// @param `stateId` State identifier
	HFSM2_CONSTEXPR(14)	void select   (const StateID stateId_)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's `select()` method)
	/// @tparam `TState` State type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void select   ()							noexcept	{ select   (PlanControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest `utility()`
	///   among those with the highest `rank()`)
	/// @param `stateId` State identifier
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	HFSM2_CONSTEXPR(14)	void utilize  (const StateID stateId_)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TState` State type
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void utilize  ()							noexcept	{ utilize  (PlanControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, uses weighted random to activate the state proportional to `utility()`
	///   among those with the highest `rank()`)
	/// @param `stateId` State identifier
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	HFSM2_CONSTEXPR(14)	void randomize(const StateID stateId_)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, uses weighted random to activate the state proportional to `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TState` State type
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void randomize()							noexcept	{ randomize(PlanControl::template stateId<TState>());	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param `stateId` State identifier
	HFSM2_CONSTEXPR(14)	void schedule (const StateID stateId_)		noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam `TState` State type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void schedule ()							noexcept	{ schedule (PlanControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_PLANS_AVAILABLE()

	/// @brief Succeed a plan task for the current state
	/// @see `HFSM2_ENABLE_PLANS`
	HFSM2_CONSTEXPR(14)	void succeed()								noexcept	{ succeed  (_originId							   );	}

	/// @brief Succeed a plan task for a state
	/// @param `stateId` State identifier
	/// @see `HFSM2_ENABLE_PLANS`
	HFSM2_CONSTEXPR(14) void succeed(const StateID stateId_)		noexcept;

	/// @brief Succeed a plan task for a state
	/// @tparam `TState` State type
	/// @see `HFSM2_ENABLE_PLANS`
	template <typename TState>
	HFSM2_CONSTEXPR(14) void succeed()								noexcept	{ succeed  (PlanControl::template stateId<TState>());	}

	/// @brief Fail a plan task for the current state
	/// @see `HFSM2_ENABLE_PLANS`
	HFSM2_CONSTEXPR(14)	void fail	()								noexcept	{ fail	   (_originId							   );	}

	/// @brief Fail a plan task for a state
	/// @param `stateId` State identifier
	/// @see `HFSM2_ENABLE_PLANS`
	HFSM2_CONSTEXPR(14) void fail	(const StateID stateId_)		noexcept;

	/// @brief Fail a plan task for a state
	/// @tparam `TState` State type
	/// @see `HFSM2_ENABLE_PLANS`
	template <typename TState>
	HFSM2_CONSTEXPR(14) void fail	()								noexcept	{ fail	   (PlanControl::template stateId<TState>());	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using PlanControl::_core;
	using PlanControl::_originId;
	using PlanControl::_regionId;

	using PlanControl::_regionStateId;
	using PlanControl::_regionSize;
	using PlanControl::_taskStatus;

	bool _locked = false;
};

//------------------------------------------------------------------------------

template <typename TArgs>
class FullControlT;

//------------------------------------------------------------------------------

template <
	typename TConfig
  , typename TStateList
  , typename TRegionList
  , Long NCompoCount
  , Long NOrthoCount
  , Long NOrthoUnits
  HFSM2_IF_SERIALIZATION(, Long NSerialBits)
  HFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
>
class FullControlT<
		  ArgsT<
			  TConfig
			, TStateList
			, TRegionList
			, NCompoCount
			, NOrthoCount
			, NOrthoUnits
			HFSM2_IF_SERIALIZATION(, NSerialBits)
			HFSM2_IF_PLANS(, NTaskCapacity)
			, TPayload
		  >
	  >
	: public FullControlBaseT<
				 ArgsT<
					 TConfig
				   , TStateList
				   , TRegionList
				   , NCompoCount
				   , NOrthoCount
				   , NOrthoUnits
				   HFSM2_IF_SERIALIZATION(, NSerialBits)
				   HFSM2_IF_PLANS(, NTaskCapacity)
				   , TPayload
				 >
			 >
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename, Strategy, typename, typename...>
	friend struct C_;

	template <typename, typename, typename, typename...>
	friend struct O_;

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
				   HFSM2_IF_PLANS(, NTaskCapacity)
				   , TPayload
				 >;

protected:
	using FullControlBase = FullControlBaseT<Args>;

	using typename FullControlBase::Payload;
	using typename FullControlBase::Transition;

	using typename FullControlBase::Origin;

#if HFSM2_PLANS_AVAILABLE()
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using FullControlBase::FullControlBase;

#if HFSM2_PLANS_AVAILABLE()

	template <typename TState>
	HFSM2_CONSTEXPR(14)	TaskStatus updatePlan(TState& headState,
											  const TaskStatus subStatus)	noexcept;

#endif

public:
	using FullControlBase::context;

	using FullControlBase::isActive;
	using FullControlBase::changeTo;

	HFSM2_IF_PLANS(using FullControlBase::plan);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Transition into a state
	///   (if transitioning into a region, acts depending on the region type)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	HFSM2_CONSTEXPR(14)	void changeWith   (const StateID  stateId_,
										   const Payload& payload)			noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void changeWith   (const Payload& payload)			noexcept	{ changeWith   (FullControlBase::template stateId<TState>(),	  payload );	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the initial state)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	HFSM2_CONSTEXPR(14)	void restartWith  (const StateID  stateId_,
										   const Payload& payload)			noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the initial state)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void restartWith  (const Payload& payload)			noexcept	{ restartWith  (FullControlBase::template stateId<TState>(),	  payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	HFSM2_CONSTEXPR(14)	void resumeWith   (const StateID  stateId_,
										   const Payload& payload)			noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void resumeWith   (const Payload& payload)			noexcept	{ resumeWith   (FullControlBase::template stateId<TState>(),	  payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's `select()` method)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	HFSM2_CONSTEXPR(14)	void selectWith   (const StateID  stateId_,
										   const Payload& payload)			noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's `select()` method)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void selectWith   (const Payload& payload)			noexcept	{ selectWith   (FullControlBase::template stateId<TState>(),	  payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest `utility()`
	///   among those with the highest `rank()`)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	HFSM2_CONSTEXPR(14)	void utilizeWith  (const StateID  stateId_,
										   const Payload& payload)			noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void utilizeWith  (const Payload& payload)			noexcept	{ utilizeWith  (FullControlBase::template stateId<TState>(),	  payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///   proportional to `utility()` among those with the highest `rank()`)
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	HFSM2_CONSTEXPR(14)	void randomizeWith(const StateID  stateId_,
										   const Payload& payload)			noexcept;

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///   proportional to `utility()` among those with the highest `rank()`)
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void randomizeWith(const Payload& payload)			noexcept	{ randomizeWith(FullControlBase::template stateId<TState>(),	  payload );	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param `stateId` Destination state identifier
	/// @param `payload` Payload
	HFSM2_CONSTEXPR(14)	void scheduleWith (const StateID  stateId_,
										   const Payload& payload)			noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam `TState` Destination state type
	/// @param `payload` Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void scheduleWith (const Payload& payload)			noexcept	{ scheduleWith (FullControlBase::template stateId<TState>(),	  payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using FullControlBase::_core;
	using FullControlBase::_originId;
	using FullControlBase::_regionId;

	using FullControlBase::_regionStateId;
	using FullControlBase::_regionSize;
	using FullControlBase::_taskStatus;

	using FullControlBase::_locked;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <
	typename TConfig
  , typename TStateList
  , typename TRegionList
  , Long NCompoCount
  , Long NOrthoCount
  , Long NOrthoUnits
  HFSM2_IF_SERIALIZATION(, Long NSerialBits)
  HFSM2_IF_PLANS(, Long NTaskCapacity)
>
class FullControlT<
		  ArgsT<
			  TConfig
			, TStateList
			, TRegionList
			, NCompoCount
			, NOrthoCount
			, NOrthoUnits
			HFSM2_IF_SERIALIZATION(, NSerialBits)
			HFSM2_IF_PLANS(, NTaskCapacity)
			, void
		  >
	  >
	: public FullControlBaseT<
				 ArgsT<
					 TConfig
				   , TStateList
				   , TRegionList
				   , NCompoCount
				   , NOrthoCount
				   , NOrthoUnits
				   HFSM2_IF_SERIALIZATION(, NSerialBits)
				   HFSM2_IF_PLANS(, NTaskCapacity)
				   , void
				 >
			 >
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename, Strategy, typename, typename...>
	friend struct C_;

	template <typename, typename, typename, typename...>
	friend struct O_;

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
				   HFSM2_IF_PLANS(, NTaskCapacity)
				   , void
				 >;

protected:
	using FullControlBase = FullControlBaseT<Args>;

	using typename FullControlBase::Origin;

#if HFSM2_PLANS_AVAILABLE()
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using FullControlBase::FullControlBase;

#if HFSM2_PLANS_AVAILABLE()

	template <typename TState>
	HFSM2_CONSTEXPR(14)	TaskStatus updatePlan(TState& headState,
											  const TaskStatus subStatus)	noexcept;

#endif

public:
	using FullControlBase::isActive;
	using FullControlBase::changeTo;

	HFSM2_IF_PLANS(using FullControlBase::plan);

protected:
	using FullControlBase::_core;
	using FullControlBase::_regionId;

	using FullControlBase::_taskStatus;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "control_3.inl"
