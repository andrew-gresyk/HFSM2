namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class ControlT {
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename, Strategy, typename, typename...>
	friend struct C_;

	template <typename, typename, typename, typename...>
	friend struct O_;

	template <typename, typename>
	friend class R_;

protected:
	using Context			= typename TArgs::Context;

	using StateList			= typename TArgs::StateList;
	using RegionList		= typename TArgs::RegionList;

	using Core				= CoreT	   <TArgs>;
	using Registry			= RegistryT<TArgs>;

	using Payload			= typename TArgs::Payload;
	using Transition		= TransitionT<Payload>;
	using TransitionSet		= DynamicArrayT<Transition, TArgs::COMPO_COUNT>;
	using TransitionSets	= DynamicArrayT<Transition, TArgs::COMPO_COUNT * TArgs::SUBSTITUTION_LIMIT>;

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	using TransitionTargets	= StaticArrayT<Short, TArgs::STATE_COUNT>;
#endif

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using RNG				= typename TArgs::RNG;
#endif

#if HFSM2_PLANS_AVAILABLE()
	using PlanData			= PlanDataT<TArgs>;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger			= typename TArgs::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Origin final {
		HFSM2_CONSTEXPR(14)	 Origin(ControlT& control_,
									const StateID stateId_)					noexcept;

		HFSM2_CONSTEXPR(20)	~Origin()										noexcept;

		ControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		HFSM2_CONSTEXPR(14)	 Region(ControlT& control,
									const RegionID regionId_)				noexcept;

		HFSM2_CONSTEXPR(20)	~Region()										noexcept;

		ControlT& control;
		const RegionID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(11)	ControlT(Core& core)								noexcept
		: _core{core}
	{}

	HFSM2_CONSTEXPR(14)	void   setRegion(const RegionID regionId_)			noexcept;
	HFSM2_CONSTEXPR(14)	void resetRegion(const RegionID regionId_)			noexcept;

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void pinLastTransition(const StateID stateId_,
											   const Short index)			noexcept;
#endif

public:

#if HFSM2_PLANS_AVAILABLE()
	using CPlan				= CPlanT<TArgs>;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get current state's identifier
	/// @return Numeric state identifier
	HFSM2_CONSTEXPR(11) StateID  stateId()							  const noexcept	{ return _originId;												}

	/// @brief Get state identifier for a state type
	/// @tparam `TState` State type
	/// @return Numeric state identifier
	template <typename TState>
	static
	HFSM2_CONSTEXPR(11) StateID  stateId()									noexcept	{ return					   index<StateList , TState>() ;	}

	/// @brief Get region identifier for a region type
	/// @tparam `TState` Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static
	HFSM2_CONSTEXPR(11) RegionID regionId()									noexcept	{ return static_cast<RegionID>(index<RegionList, TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see `Control::context()`
	HFSM2_CONSTEXPR(14)		  Context& _()									noexcept	{ return _core.context;											}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see `Control::context()`
	HFSM2_CONSTEXPR(11)	const Context& _()							  const noexcept	{ return _core.context;											}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see `Control::_()`
	HFSM2_CONSTEXPR(14)		  Context& context()							noexcept	{ return _core.context;											}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see `Control::_()`
	HFSM2_CONSTEXPR(11)	const Context& context()					  const noexcept	{ return _core.context;											}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Inspect current transition requests
	/// @return Array of transition requests
	HFSM2_CONSTEXPR(11)	const TransitionSet& requests()				  const noexcept	{ return _core.requests;										}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get active sub-state's index for the current region
	/// @return Region's active sub-state index
	HFSM2_CONSTEXPR(14)	Prong activeSubState()						  const noexcept	{ return _core.registry.activeSubState(_originId);				}

	/// @brief Get region's active sub-state's index
	/// @param `stateId` Region's head state ID
	/// @return Region's active sub-state index
	HFSM2_CONSTEXPR(14)	Prong activeSubState(const StateID stateId_)  const noexcept	{ return _core.registry.activeSubState(stateId_);				}

	/// @brief Get region's active sub-state's index
	/// @tparam `TState` Region's head state type
	/// @return Region's active sub-state index
	template <typename TState>
	HFSM2_CONSTEXPR(14)	Prong activeSubState()						  const noexcept	{ return _core.registry.activeSubState(stateId<TState>());		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is active
	/// @param `stateId` State identifier
	/// @return State active status
	HFSM2_CONSTEXPR(11)	bool isActive   (const StateID stateId_)	  const noexcept	{ return _core.registry.isActive(stateId_);						}

	/// @brief Check if a state is active
	/// @tparam `TState` State type
	/// @return State active status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isActive	()							  const noexcept	{ return isActive(stateId<TState>());							}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @param `stateId` State identifier
	/// @return State resumable status
	HFSM2_CONSTEXPR(11)	bool isResumable(const StateID stateId_)	  const noexcept	{ return _core.registry.isResumable(stateId_);					}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @tparam `TState` State type
	/// @return State resumable status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isResumable()							  const noexcept	{ return isResumable(stateId<TState>());						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @param `stateId` State identifier
	/// @return State scheduled status
	HFSM2_CONSTEXPR(11)	bool isScheduled(const StateID stateId_)	  const noexcept	{ return isResumable(stateId_);									}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @tparam `TState` State type
	/// @return State scheduled status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isScheduled()							  const noexcept	{ return isResumable(stateId<TState>());						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_PLANS_AVAILABLE()

	/// @brief Access read-only plan for the current region
	/// @return Plan for the current region
	/// @see `HFSM2_ENABLE_PLANS`
	HFSM2_CONSTEXPR(11)	CPlan plan()								  const noexcept	{ return CPlan{_core.registry, _core.planData, _regionId};				}

	/// @brief Access read-only plan for a region
	/// @param `regionId` Region identifier
	/// @return Read-only plan for the region
	/// @see `HFSM2_ENABLE_PLANS`
	HFSM2_CONSTEXPR(14)	CPlan plan(const RegionID regionId_)		  const noexcept	{ return CPlan{_core.registry, _core.planData, regionId_};				}

	/// @brief Access read-only plan for a region
	/// @tparam `TRegion` Region head state type
	/// @return Read-only plan for the region
	/// @see `HFSM2_ENABLE_PLANS`
	template <typename TRegion>
	HFSM2_CONSTEXPR(14)	CPlan plan()										noexcept	{ return CPlan{_core.registry, _core.planData, regionId<TRegion>()};	}

	/// @brief Access read-only plan for a region
	/// @tparam `TRegion` Region head state type
	/// @return Read-only Plan for the region
	/// @see `HFSM2_ENABLE_PLANS`
	template <typename TRegion>
	HFSM2_CONSTEXPR(14)	CPlan plan()								  const noexcept	{ return CPlan{_core.registry, _core.planData, regionId<TRegion>()};	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get transitions processed during last `update()`, `react()` or `replayTransitions()`
	/// @return Array of last transition requests
	/// @see `HFSM2_ENABLE_TRANSITION_HISTORY`
	HFSM2_CONSTEXPR(11)	const TransitionSets& previousTransitions()					  const noexcept	{ return _core.previousTransitions;				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get the last transition that caused the state to be activated
	/// @param `stateId` State identifier
	/// @return Pointer to the last transition that activated the state
	/// @see `HFSM2_ENABLE_TRANSITION_HISTORY`
	HFSM2_CONSTEXPR(14)	const Transition* lastTransitionTo(const StateID stateId_)	  const noexcept;

	/// @brief Get the last transition that caused the state to be activated
	/// @tparam `TState` State type
	/// @return Pointer to the last transition that activated the state
	/// @see `HFSM2_ENABLE_TRANSITION_HISTORY`
	template <typename TState>
	HFSM2_CONSTEXPR(11)	const Transition* lastTransitionTo()						  const noexcept	{ return lastTransitionTo(stateId<TState>());	}

	/// @brief Get the last transition that caused the current state to be activated
	/// @return Pointer to the last transition that activated the current state
	/// @see `HFSM2_ENABLE_TRANSITION_HISTORY`
	HFSM2_CONSTEXPR(14)	const Transition* lastTransition()							  const noexcept;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	Core& _core;
	StateID _originId = INVALID_STATE_ID;
	RegionID _regionId = 0;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "control_1.inl"
