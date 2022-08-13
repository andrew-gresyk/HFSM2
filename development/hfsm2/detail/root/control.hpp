namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class ConstControlT {
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

	using Core				= CoreT<TArgs>;

	using Registry			= RegistryT<TArgs>;

	using Payload			= typename TArgs::Payload;
	using Transition		= TransitionT<Payload>;
	using TransitionSet		= ArrayT<Transition, TArgs::COMPO_REGIONS>;
	using TransitionSets	= ArrayT<Transition, TArgs::COMPO_REGIONS * TArgs::SUBSTITUTION_LIMIT>;

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
		HFSM2_CONSTEXPR(14)	Origin(ConstControlT& control_,
								   const StateID stateId_)					noexcept;

		HFSM2_CONSTEXPR(20)	~Origin()										noexcept;

		ConstControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		HFSM2_CONSTEXPR(14)	Region(ConstControlT& control,
								   const RegionID regionId_)				noexcept;

		HFSM2_CONSTEXPR(20)	~Region()										noexcept;

		ConstControlT& control;
		const RegionID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(11)	ConstControlT(const Core& core)						noexcept
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

	/// @brief Get current state's identifier
	/// @return Numeric state identifier
	HFSM2_CONSTEXPR(11) StateID stateId()							  const noexcept	{ return _originId;												}

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()										noexcept	{ return					   index<StateList , TState>() ;	}

	/// @brief Get region identifier for a region type
	/// @tparam TState Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static constexpr RegionID regionId()									noexcept	{ return static_cast<RegionID>(index<RegionList, TState>());	}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	HFSM2_CONSTEXPR(11)	const Context& _()							  const noexcept	{ return _core.context;											}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	HFSM2_CONSTEXPR(11)	const Context& context()					  const noexcept	{ return _core.context;											}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Inspect current transition requests
	/// @return Array of transition requests
	HFSM2_CONSTEXPR(11)	const TransitionSet& requests()				  const noexcept	{ return _core.requests;										}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get active sub-state's index for the current region
	/// @return Region's active sub-state index
	HFSM2_CONSTEXPR(14)	Short activeSubState()						  const noexcept	{ return _core.registry.activeSubState(_originId);				}

	/// @brief Get region's active sub-state's index
	/// @param stateId Region's head state ID
	/// @return Region's active sub-state index
	HFSM2_CONSTEXPR(14)	Short activeSubState(const StateID stateId_)  const noexcept	{ return _core.registry.activeSubState(stateId_);				}

	/// @brief Get region's active sub-state's index
	/// @tparam TState Region's head state type
	/// @return Region's active sub-state index
	template <typename TState>
	HFSM2_CONSTEXPR(14)	Short activeSubState()						  const noexcept	{ return _core.registry.activeSubState(stateId<TState>());		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is active
	/// @param stateId State identifier
	/// @return State active status
	HFSM2_CONSTEXPR(11)	bool isActive   (const StateID stateId_)	  const noexcept	{ return _core.registry.isActive(stateId_);						}

	/// @brief Check if a state is active
	/// @tparam TState State type
	/// @return State active status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isActive	()							  const noexcept	{ return isActive(stateId<TState>());							}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @param stateId State identifier
	/// @return State resumable status
	HFSM2_CONSTEXPR(11)	bool isResumable(const StateID stateId_)	  const noexcept	{ return _core.registry.isResumable(stateId_);					}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @tparam TState State type
	/// @return State resumable status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isResumable()							  const noexcept	{ return isResumable(stateId<TState>());						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @param stateId State identifier
	/// @return State scheduled status
	HFSM2_CONSTEXPR(11)	bool isScheduled(const StateID stateId_)	  const noexcept	{ return isResumable(stateId_);									}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @tparam TState State type
	/// @return State scheduled status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isScheduled()							  const noexcept	{ return isResumable(stateId<TState>());						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_PLANS_AVAILABLE()

	/// @brief Access read-only plan for the current region
	/// @return Plan for the current region
	HFSM2_CONSTEXPR(11)	CPlan plan()								  const noexcept	{ return CPlan{_core.registry, _core.planData, _regionId};						}

	/// @brief Access read-only plan for a region
	/// @param regionId Region identifier
	/// @return Read-only plan for the region
	HFSM2_CONSTEXPR(14)	CPlan plan(const RegionID regionId_)		  const noexcept	{ return CPlan{_core.registry, _core.planData, regionId_};						}

	/// @brief Access read-only plan for a region
	/// @tparam TRegion Region head state type
	/// @return Read-only Plan for the region
	template <typename TRegion>
	HFSM2_CONSTEXPR(14)	CPlan plan()								  const noexcept	{ return CPlan{_core.registry, _core.planData, regionId<TRegion>()};			}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get transitions processed during last 'update()', 'react()' or 'replayTransitions()'
	/// @return Array of last transition requests
	HFSM2_CONSTEXPR(11)	const TransitionSets& previousTransitions()					  const noexcept	{ return _core.previousTransitions;				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get the last transition that caused the state to be activated
	/// @param stateId State identifier
	/// @return Pointer to the last transition that activated the state
	HFSM2_CONSTEXPR(14)	const Transition* lastTransitionTo(const StateID stateId_)	  const noexcept;

	/// @brief Get the last transition that caused the state to be activated
	/// @tparam TState State type
	/// @return Pointer to the last transition that activated the state
	template <typename TState>
	HFSM2_CONSTEXPR(11)	const Transition* lastTransitionTo()						  const noexcept	{ return lastTransitionTo(stateId<TState>());	}

	/// @brief Get the last transition that caused the current state to be activated
	/// @return Pointer to the last transition that activated the current state
	HFSM2_CONSTEXPR(14)	const Transition* lastTransition()							  const noexcept;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	const Core& _core;
	StateID _originId = INVALID_STATE_ID;
	RegionID _regionId = 0;
};

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

	using Core				= CoreT<TArgs>;

	using Registry			= RegistryT<TArgs>;

	using Payload			= typename TArgs::Payload;
	using Transition		= TransitionT<Payload>;
	using TransitionSet		= ArrayT<Transition, TArgs::COMPO_REGIONS>;
	using TransitionSets	= ArrayT<Transition, TArgs::COMPO_REGIONS * TArgs::SUBSTITUTION_LIMIT>;

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
		HFSM2_CONSTEXPR(14)	Origin(ControlT& control_,
								   const StateID stateId_)					noexcept;

		HFSM2_CONSTEXPR(20)	~Origin()										noexcept;

		ControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		HFSM2_CONSTEXPR(14)	Region(ControlT& control,
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

	/// @brief Get current state's identifier
	/// @return Numeric state identifier
	constexpr StateID stateId()										  const noexcept	{ return _originId;												}

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()										noexcept	{ return					   index<StateList , TState>() ;	}

	/// @brief Get region identifier for a region type
	/// @tparam TState Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static constexpr RegionID regionId()									noexcept	{ return static_cast<RegionID>(index<RegionList, TState>());	}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	HFSM2_CONSTEXPR(14)		  Context& _()									noexcept	{ return _core.context;											}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	HFSM2_CONSTEXPR(11)	const Context& _()							  const noexcept	{ return _core.context;											}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	HFSM2_CONSTEXPR(14)		  Context& context()							noexcept	{ return _core.context;											}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	HFSM2_CONSTEXPR(11)	const Context& context()					  const noexcept	{ return _core.context;											}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Inspect current transition requests
	/// @return Array of transition requests
	HFSM2_CONSTEXPR(11)	const TransitionSet& requests()				  const noexcept	{ return _core.requests;										}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get active sub-state's index for the current region
	/// @return Region's active sub-state index
	HFSM2_CONSTEXPR(14)	Short activeSubState()						  const noexcept	{ return _core.registry.activeSubState(_originId);				}

	/// @brief Get region's active sub-state's index
	/// @param stateId Region's head state ID
	/// @return Region's active sub-state index
	HFSM2_CONSTEXPR(14)	Short activeSubState(const StateID stateId_)  const noexcept	{ return _core.registry.activeSubState(stateId_);				}

	/// @brief Get region's active sub-state's index
	/// @tparam TState Region's head state type
	/// @return Region's active sub-state index
	template <typename TState>
	HFSM2_CONSTEXPR(14)	Short activeSubState()						  const noexcept	{ return _core.registry.activeSubState(stateId<TState>());		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is active
	/// @param stateId State identifier
	/// @return State active status
	HFSM2_CONSTEXPR(11)	bool isActive   (const StateID stateId_)	  const noexcept	{ return _core.registry.isActive(stateId_);						}

	/// @brief Check if a state is active
	/// @tparam TState State type
	/// @return State active status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isActive	()							  const noexcept	{ return isActive(stateId<TState>());							}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @param stateId State identifier
	/// @return State resumable status
	HFSM2_CONSTEXPR(11)	bool isResumable(const StateID stateId_)	  const noexcept	{ return _core.registry.isResumable(stateId_);					}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @tparam TState State type
	/// @return State resumable status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isResumable()							  const noexcept	{ return isResumable(stateId<TState>());						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @param stateId State identifier
	/// @return State scheduled status
	HFSM2_CONSTEXPR(11)	bool isScheduled(const StateID stateId_)	  const noexcept	{ return isResumable(stateId_);									}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @tparam TState State type
	/// @return State scheduled status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isScheduled()							  const noexcept	{ return isResumable(stateId<TState>());						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_PLANS_AVAILABLE()

	/// @brief Access read-only plan for the current region
	/// @return Plan for the current region
	HFSM2_CONSTEXPR(11)	CPlan plan()								  const noexcept	{ return CPlan{_core.registry, _core.planData, _regionId};						}

	/// @brief Access read-only plan for a region
	/// @param regionId Region identifier
	/// @return Read-only plan for the region
	HFSM2_CONSTEXPR(14)	CPlan plan(const RegionID regionId_)		  const noexcept	{ return CPlan{_core.registry, _core.planData, regionId_};						}

	/// @brief Access read-only plan for a region
	/// @tparam TRegion Region head state type
	/// @return Read-only plan for the region
	template <typename TRegion>
	HFSM2_CONSTEXPR(14)	CPlan plan()										noexcept	{ return CPlan{_core.registry, _core.planData, regionId<TRegion>()};			}

	/// @brief Access read-only plan for a region
	/// @tparam TRegion Region head state type
	/// @return Read-only Plan for the region
	template <typename TRegion>
	HFSM2_CONSTEXPR(14)	CPlan plan()								  const noexcept	{ return CPlan{_core.registry, _core.planData, regionId<TRegion>()};			}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get transitions processed during last 'update()', 'react()' or 'replayTransitions()'
	/// @return Array of last transition requests
	HFSM2_CONSTEXPR(11)	const TransitionSets& previousTransitions()					  const noexcept	{ return _core.previousTransitions;				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get the last transition that caused the state to be activated
	/// @param stateId State identifier
	/// @return Pointer to the last transition that activated the state
	HFSM2_CONSTEXPR(14)	const Transition* lastTransitionTo(const StateID stateId_)	  const noexcept;

	/// @brief Get the last transition that caused the state to be activated
	/// @tparam TState State type
	/// @return Pointer to the last transition that activated the state
	template <typename TState>
	HFSM2_CONSTEXPR(11)	const Transition* lastTransitionTo()						  const noexcept	{ return lastTransitionTo(stateId<TState>());	}

	/// @brief Get the last transition that caused the current state to be activated
	/// @return Pointer to the last transition that activated the current state
	HFSM2_CONSTEXPR(14)	const Transition* lastTransition()							  const noexcept;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	Core& _core;
	StateID _originId = INVALID_STATE_ID;
	RegionID _regionId = 0;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class PlanControlT
	: public ControlT<TArgs>
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename, Strategy, typename, typename...>
	friend struct C_;

	template <typename, typename, typename, typename...>
	friend struct O_;

	template <typename, typename>
	friend class R_;

	template <typename, typename>
	friend class RV_;

protected:
	using Control		= ControlT<TArgs>;

	using typename Control::StateList;
	using typename Control::RegionList;

#if HFSM2_PLANS_AVAILABLE()
	using typename Control::PlanData;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		HFSM2_CONSTEXPR(14)	Region(PlanControlT& control,
								   const RegionID regionId_,
								   const StateID index,
								   const Long size)					noexcept;

		HFSM2_CONSTEXPR(20)	~Region()								noexcept;

		PlanControlT& control;
		const RegionID prevId;
		const Long prevIndex;
		const Long prevSize;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Control::Control;

	HFSM2_CONSTEXPR(14)	void   setRegion(const RegionID regionId_,
										 const StateID index,
										 const Long size)			noexcept;

	HFSM2_CONSTEXPR(14)	void resetRegion(const RegionID regionId_,
										 const StateID index,
										 const Long size)			noexcept;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_PLANS_AVAILABLE()
	using typename Control::CPlan;

	using Plan			= PlanT<TArgs>;

	/// @brief Access plan for the current region
	/// @return Plan for the current region
	HFSM2_CONSTEXPR(14)  Plan plan()								noexcept	{ return  Plan{_core.registry, _core.planData, _regionId							};	}

// COMMON

	/// @brief Access plan for a region
	/// @param regionId
	/// @return Plan for the region
	HFSM2_CONSTEXPR(14)  Plan plan(const RegionID regionId_)		noexcept	{ return  Plan{_core.registry, _core.planData,  regionId_							};	}

	/// @brief Access plan for a region
	/// @tparam TRegion Region head state type
	/// @return Plan for the region
	template <typename TRegion>
	HFSM2_CONSTEXPR(14)  Plan plan()								noexcept	{ return  Plan{_core.registry, _core.planData, Control::template regionId<TRegion>()};	}

// COMMON

	/// @brief Access plan for the current region
	/// @return Plan for the current region
	HFSM2_CONSTEXPR(11)	CPlan plan()						  const noexcept	{ return CPlan{_core.registry, _core.planData, _regionId							};	}

// COMMON

	/// @brief Access plan for a region
	/// @param regionId
	/// @return Plan for the region
	HFSM2_CONSTEXPR(11)	CPlan plan(const RegionID regionId_)  const noexcept	{ return CPlan{_core.registry, _core.planData,  regionId_							};	}

	/// @brief Access plan for a region
	/// @tparam TRegion Region head state type
	/// @return Plan for the region
	template <typename TRegion>
	HFSM2_CONSTEXPR(11)	CPlan plan()						  const noexcept	{ return CPlan{_core.registry, _core.planData, Control::template regionId<TRegion>()};	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using Control::_core;
	using Control::_regionId;

	StateID _regionStateId = 0;
	Long _regionSize = StateList::SIZE;
	Status _status;
};

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
		HFSM2_CONSTEXPR(14)	Lock(FullControlBaseT& control_)		noexcept;
		HFSM2_CONSTEXPR(20)	~Lock()									noexcept;

		FullControlBaseT* const control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using PlanControl::PlanControl;

#if HFSM2_PLANS_AVAILABLE()

	template <typename TState>
	HFSM2_CONSTEXPR(14)	Status buildPlanStatus() noexcept;

#endif

public:
	using PlanControl::context;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Transition into a state
	///   (if transitioning into a region, acts depending on the region type)
	/// @param stateId State identifier
	HFSM2_CONSTEXPR(14)	void changeTo (const StateID stateId_)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void changeTo ()							noexcept	{ changeTo (PlanControl::template stateId<TState>());	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the initial state)
	/// @param stateId State identifier
	HFSM2_CONSTEXPR(14)	void restart  (const StateID stateId_)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the initial state)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void restart  ()							noexcept	{ restart  (PlanControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param stateId State identifier
	HFSM2_CONSTEXPR(14)	void resume   (const StateID stateId_)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void resume   ()							noexcept	{ resume   (PlanControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @param stateId State identifier
	HFSM2_CONSTEXPR(14)	void select   (const StateID stateId_)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void select   ()							noexcept	{ select   (PlanControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId State identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void utilize  (const StateID stateId_)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState State type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void utilize  ()							noexcept	{ utilize  (PlanControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId State identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void randomize(const StateID stateId_)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState State type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void randomize()							noexcept	{ randomize(PlanControl::template stateId<TState>());	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId State identifier
	HFSM2_CONSTEXPR(14)	void schedule (const StateID stateId_)		noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void schedule ()							noexcept	{ schedule (PlanControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_PLANS_AVAILABLE()

	/// @brief Succeed a plan task for the current state
	HFSM2_CONSTEXPR(14)	void succeed()								noexcept	{ succeed  (_originId							   );	}

	/// @brief Succeed a plan task for a state
	/// @param stateId State ID
	HFSM2_CONSTEXPR(14) void succeed(const StateID stateId_)		noexcept;

	/// @brief Succeed a plan task for a state
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14) void succeed()								noexcept	{ succeed  (PlanControl::template stateId<TState>());	}

	/// @brief Fail a plan task for the current state
	HFSM2_CONSTEXPR(14)	void fail	()								noexcept	{ fail	   (_originId							   );	}

	/// @brief Fail a plan task for a state
	/// @param stateId State ID
	HFSM2_CONSTEXPR(14) void fail	(const StateID stateId_)		noexcept;

	/// @brief Fail a plan task for a state
	/// @tparam TState State type
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
	using PlanControl::_status;

	bool _locked = false;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
class FullControlT;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NCompoCount
		, Long NOrthoCount
		, Long NOrthoUnits
		HFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
class FullControlT<ArgsT<TContext
					   , TConfig
					   , TStateList
					   , TRegionList
					   , NCompoCount
					   , NOrthoCount
					   , NOrthoUnits
					   HFSM2_IF_SERIALIZATION(, NSerialBits)
					   , NSubstitutionLimit
					   HFSM2_IF_PLANS(, NTaskCapacity)
					   , TPayload>>
	: public FullControlBaseT<ArgsT<TContext
								  , TConfig
								  , TStateList
								  , TRegionList
								  , NCompoCount
								  , NOrthoCount
								  , NOrthoUnits
								  HFSM2_IF_SERIALIZATION(, NSerialBits)
								  , NSubstitutionLimit
								  HFSM2_IF_PLANS(, NTaskCapacity)
								  , TPayload>>
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename, Strategy, typename, typename...>
	friend struct C_;

	template <typename, typename, typename, typename...>
	friend struct O_;

	template <typename, typename>
	friend class R_;

	using Args = ArgsT<TContext
					 , TConfig
					 , TStateList
					 , TRegionList
					 , NCompoCount
					 , NOrthoCount
					 , NOrthoUnits
					 HFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 HFSM2_IF_PLANS(, NTaskCapacity)
					 , TPayload>;

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
	HFSM2_CONSTEXPR(14)	Status updatePlan(TState& headState,
										  const Status subStatus)		noexcept;

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
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void changeWith   (const StateID  stateId_,
										   const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void changeWith   (const Payload& payload)		noexcept	{ changeWith   (FullControlBase::template stateId<TState>(),	  payload );	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void restartWith  (const StateID  stateId_,
										   const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void restartWith  (const Payload& payload)		noexcept	{ restartWith  (FullControlBase::template stateId<TState>(),	  payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void resumeWith   (const StateID  stateId_,
										   const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void resumeWith   (const Payload& payload)		noexcept	{ resumeWith   (FullControlBase::template stateId<TState>(),	  payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void selectWith   (const StateID  stateId_,
										   const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void selectWith   (const Payload& payload)		noexcept	{ selectWith   (FullControlBase::template stateId<TState>(),	  payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void utilizeWith  (const StateID  stateId_,
										   const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void utilizeWith  (const Payload& payload)		noexcept	{ utilizeWith  (FullControlBase::template stateId<TState>(),	  payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///   proportional to 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void randomizeWith(const StateID  stateId_,
										   const Payload& payload)		noexcept;

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///   proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void randomizeWith(const Payload& payload)		noexcept	{ randomizeWith(FullControlBase::template stateId<TState>(),	  payload );	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void scheduleWith (const StateID  stateId_,
										   const Payload& payload)		noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void scheduleWith (const Payload& payload)		noexcept	{ scheduleWith (FullControlBase::template stateId<TState>(),	  payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using FullControlBase::_core;
	using FullControlBase::_originId;
	using FullControlBase::_regionId;

	using FullControlBase::_regionStateId;
	using FullControlBase::_regionSize;
	using FullControlBase::_status;

	using FullControlBase::_locked;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NCompoCount
		, Long NOrthoCount
		, Long NOrthoUnits
		HFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)>
class FullControlT<ArgsT<TContext
					   , TConfig
					   , TStateList
					   , TRegionList
					   , NCompoCount
					   , NOrthoCount
					   , NOrthoUnits
					   HFSM2_IF_SERIALIZATION(, NSerialBits)
					   , NSubstitutionLimit
					   HFSM2_IF_PLANS(, NTaskCapacity)
					   , void>>
	: public FullControlBaseT<ArgsT<TContext
								  , TConfig
								  , TStateList
								  , TRegionList
								  , NCompoCount
								  , NOrthoCount
								  , NOrthoUnits
								  HFSM2_IF_SERIALIZATION(, NSerialBits)
								  , NSubstitutionLimit
								  HFSM2_IF_PLANS(, NTaskCapacity)
								  , void>>
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename, Strategy, typename, typename...>
	friend struct C_;

	template <typename, typename, typename, typename...>
	friend struct O_;

	template <typename, typename>
	friend class R_;

	using Args = ArgsT<TContext
					 , TConfig
					 , TStateList
					 , TRegionList
					 , NCompoCount
					 , NOrthoCount
					 , NOrthoUnits
					 HFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 HFSM2_IF_PLANS(, NTaskCapacity)
					 , void>;

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
	HFSM2_CONSTEXPR(14)	Status updatePlan(TState& headState,
										  const Status subStatus)		noexcept;

#endif

public:
	using FullControlBase::isActive;
	using FullControlBase::changeTo;

	HFSM2_IF_PLANS(using FullControlBase::plan);

protected:
	using FullControlBase::_core;
	using FullControlBase::_regionId;

	using FullControlBase::_status;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class GuardControlT final
	: public FullControlT<TArgs>
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename>
	friend class R_;

	using FullControl	= FullControlT<TArgs>;

	using typename FullControl::Core;

	using typename FullControl::Context;

	using typename FullControl::Registry;
	using typename FullControl::TransitionSet;
	using typename FullControl::TransitionSets;

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	using typename FullControl::TransitionTargets;
#endif

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename FullControl::RNG;
#endif

#if HFSM2_PLANS_AVAILABLE()
	using typename FullControl::PlanData;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename FullControl::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(11)	GuardControlT(Core& core,
									  const TransitionSets& currentTransitions,
									  const TransitionSet& pendingTransitions) noexcept
		: FullControl{core}
		, _currentTransitions{currentTransitions}
		, _pendingTransitions{pendingTransitions}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	using FullControl::context;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be activated or deactivated
	/// @param stateId State identifier
	/// @return State pending activation/deactivation status
	HFSM2_CONSTEXPR(11)	bool isPendingChange(const StateID stateId_)  const noexcept	{ return _core.registry.isPendingChange(stateId_);					}

	/// @brief Check if a state is going to be activated or deactivated
	/// @tparam TState State type
	/// @return State pending activation/deactivation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingChange()						  const noexcept	{ return isPendingChange(FullControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be activated
	/// @param stateId State identifier
	/// @return State pending activation status
	HFSM2_CONSTEXPR(11)	bool isPendingEnter (const StateID stateId_)  const noexcept	{ return _core.registry.isPendingEnter (stateId_);					}

	/// @brief Check if a state is going to be activated
	/// @tparam TState State type
	/// @return State pending activation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingEnter ()						  const noexcept	{ return isPendingEnter (FullControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be deactivated
	/// @param stateId State identifier
	/// @return State pending deactivation status
	HFSM2_CONSTEXPR(11)	bool isPendingExit	(const StateID stateId_)  const noexcept	{ return _core.registry.isPendingExit  (stateId_);					}

	/// @brief Check if a state is going to be deactivated
	/// @tparam TState State type
	/// @return State pending deactivation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingExit  ()						  const noexcept	{ return isPendingExit  (FullControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Get current transition requests
	/// @return ArrayT of pending transition requests
	HFSM2_CONSTEXPR(11)	const TransitionSets& currentTransitions()	  const noexcept	{ return _currentTransitions;										}

	/// @brief Get pending transition requests
	/// @return Array of pending transition requests
	HFSM2_CONSTEXPR(11)	const TransitionSet&  pendingTransitions()	  const noexcept	{ return _pendingTransitions;										}

	/// @brief Cancel pending transition requests
	///   (can be used to substitute a transition into the current state with a different one)
	HFSM2_CONSTEXPR(14)	void cancelPendingTransitions()						noexcept;

private:
	using FullControl::_core;
	using FullControl::_originId;

	const TransitionSets& _currentTransitions;
	const TransitionSet&  _pendingTransitions;
	bool _cancelled = false;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "control.inl"
