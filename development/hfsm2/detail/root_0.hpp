namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <
	typename TConfig
  , typename TApex
>
class R_ {
public:
	static constexpr FeatureTag FEATURE_TAG = TConfig::FEATURE_TAG;

	using Context				= typename TConfig::Context;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank					= typename TConfig::Rank;
	using Utility				= typename TConfig::Utility;
	using RNG					= typename TConfig::RNG;
#endif

	using Payload				= typename TConfig::Payload;

protected:
	using Forward				= RF_<TConfig, TApex>;
	using StateList				= typename Forward::StateList;
	using RegionList			= typename Forward::RegionList;

	static constexpr Long STATE_COUNT	= StateList ::SIZE;
	static constexpr Long REGION_COUNT	= RegionList::SIZE;

	using Args					= typename Forward::Args;
	using PureContext			= typename Args::PureContext;

	static_assert(Args::STATE_COUNT	   < static_cast<unsigned>(-1), "Too many states in the FSM. Change 'Short' type.");
	static_assert(Args::STATE_COUNT	  == static_cast<unsigned>(StateList::SIZE), "STATE_COUNT != StateList::SIZE");
	static_assert(Args::COMPO_COUNT >= 1, "State machine needs at least one composite region");

	using Core					= CoreT<Args>;

	using Apex					= MaterialT<I_<0, 0, 0, 0>, Args, TApex>;

	using Registry				= RegistryT<Args>;
	using CompoForks			= typename Registry::CompoForks;
	using RegistryBackUp		= typename Registry::BackUp;

	using ConstControl			= ConstControlT<Args>;
	using Control				= ControlT	   <Args>;
	using PlanControl			= PlanControlT <Args>;
	using FullControl			= FullControlT <Args>;
	using GuardControl			= GuardControlT<Args>;
	using EventControl			= EventControlT<Args>;

	static constexpr Short	SUBSTITUTION_LIMIT	= Forward::SUBSTITUTION_LIMIT;

#if HFSM2_PLANS_AVAILABLE()
	using PlanData				= PlanDataT<Args>;
#endif

public:
#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream			= typename Args::WriteStream;
	using ReadStream			= typename Args::ReadStream;
#endif

public:
	using Info					= WrapInfo<TApex>;

	/// @brief Transition
	using Transition			= typename Core::Transition;

	/// @brief Array of transitions
	using TransitionSet			= typename Control::TransitionSet;
	using TransitionSets		= typename Control::TransitionSets;

#if HFSM2_PLANS_AVAILABLE()
	using CPlan					= CPlanT<Args>;
	using Plan					= PayloadPlanT<Args>;

	static constexpr Long TASK_CAPACITY = Forward::TASK_CAPACITY;
#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	using TransitionTargets		= typename Control::TransitionTargets;
#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using Prefix				= StaticArrayT<wchar_t, Info::REVERSE_DEPTH * 2 + 2>;
	using Prefixes				= StaticArrayT<Prefix, STATE_COUNT>;

	using NamedStates			= BitArrayT<STATE_COUNT>;

	using StructureStateInfos	= typename Args::StructureStateInfos;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger				= typename TConfig::LoggerInterface;
#endif

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	explicit R_(Context& context
								  HFSM2_IF_UTILITY_THEORY(, RNG& rng)
								  HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))		noexcept;

	HFSM2_CONSTEXPR(14)	explicit R_(PureContext&& context
								  HFSM2_IF_UTILITY_THEORY(, RNG& rng)
								  HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))		noexcept;

	HFSM2_CONSTEXPR(NO) R_(const R_& )															noexcept = default;
	HFSM2_CONSTEXPR(NO) R_(		 R_&&)															noexcept = default;

	HFSM2_CONSTEXPR(20)	~R_()																	noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get state identifier for a state type
	/// @tparam `TState` State type
	/// @return Numeric state identifier
	template <typename TState>
	static
	HFSM2_CONSTEXPR(11) StateID  stateId()														noexcept	{ return					   index<StateList , TState>() ;	}

	/// @brief Get region identifier for a region type
	/// @tparam `TState` Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static
	HFSM2_CONSTEXPR(11) RegionID regionId()														noexcept	{ return static_cast<RegionID>(index<RegionList, TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Access context
	/// @return context
	HFSM2_CONSTEXPR(14)		  Context& context()												noexcept	{ return _core.context;							}

	/// @brief Access context
	/// @return context
	HFSM2_CONSTEXPR(11)	const Context& context()										  const noexcept	{ return _core.context;							}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Access state instance
	/// @tparam `TState` State type
	/// @return State instance
	template <typename TState>
	HFSM2_CONSTEXPR(14)		  TState& access()													noexcept	{ return static_cast<	   TState&>(_apex);		}

	/// @brief Access state instance
	/// @tparam `TState` State type
	/// @return State instance
	template <typename TState>
	HFSM2_CONSTEXPR(11)	const TState& access()											  const noexcept	{ return static_cast<const TState&>(_apex);		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Trigger FSM update cycle (recursively call `update()` from the root down to the leaf states
	///   on all active states then process requested transitions)
	HFSM2_CONSTEXPR(14)	void update()															noexcept;

	/// @brief Have FSM react to an event (recursively call matching 'react<>()' from the root down to the leaf states
	///   on all active states then process requested transitions)
	/// @tparam `TEvent` Event type
	/// @param `event` Event to react to
	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void react(const TEvent& event)											noexcept;

	/// @brief Recursively call 'query()' from the root down to the leaf states on all active states
	/// @tparam `TEvent` Event type
	/// @param `event` Event to react to
	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void query(TEvent& event)										  const noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get region's active sub-state's index
	/// @param `stateId` Region's head state identifier
	/// @return Region's active sub-state index
	HFSM2_CONSTEXPR(14)	Prong activeSubState (const StateID stateId_)					  const noexcept	{ return _core.registry.activeSubState(stateId_			);	}

	/// @brief Get region's active sub-state's index
	/// @tparam `TState` Region's head state type
	/// @return Region's active sub-state index
	template <typename TState>
	HFSM2_CONSTEXPR(14)	Prong activeSubState ()											  const noexcept	{ return _core.registry.activeSubState(stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is active
	/// @param `stateId` Destination state identifier
	/// @return State active status
	HFSM2_CONSTEXPR(11)	bool isActive		 (const StateID stateId_)					  const noexcept	{ return _core.registry.isActive   (stateId_);				}

	/// @brief Check if a state is active
	/// @tparam `TState` Destination state type
	/// @return State active status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isActive		 ()											  const noexcept	{ return isActive	(stateId<TState>());					}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @param `stateId` Destination state identifier
	/// @return State resumable status
	HFSM2_CONSTEXPR(11)	bool isResumable	 (const StateID stateId_)					  const noexcept	{ return _core.registry.isResumable(stateId_);				}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @tparam `TState` Destination state type
	/// @return State resumable status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isResumable	 ()											  const noexcept	{ return isResumable(stateId<TState>());					}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @param `stateId` Destination state identifier
	/// @return State scheduled status
	HFSM2_CONSTEXPR(11)	bool isScheduled	 (const StateID stateId_)					  const noexcept	{ return isResumable(stateId_);								}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @tparam `TState` Destination state type
	/// @return State scheduled status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isScheduled	 ()											  const noexcept	{ return isResumable<TState>();								}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_PLANS_AVAILABLE()

	/// @brief Access plan for the root region
	/// @return Plan for the root region
	/// @see `HFSM2_ENABLE_PLANS`
	HFSM2_CONSTEXPR(14)  Plan plan()															noexcept	{ return  Plan{_core.registry, _core.planData, 0				  };	}

	/// @brief Access plan for a region
	/// @param `regionId` Region identifier
	/// @return Plan for the region
	/// @see `HFSM2_ENABLE_PLANS`
	HFSM2_CONSTEXPR(14)  Plan plan(const RegionID regionId_)									noexcept	{ return  Plan{_core.registry, _core.planData, regionId_		  };	}

	/// @brief Access plan for a region
	/// @tparam `TRegion` Region head state type
	/// @return Plan for the region
	/// @see `HFSM2_ENABLE_PLANS`
	template <typename TRegion>
	HFSM2_CONSTEXPR(14)  Plan plan()															noexcept	{ return  Plan{_core.registry, _core.planData, regionId<TRegion>()};	}

	/// @brief Access read-only plan for the root region
	/// @return Read-only plan for the root region
	/// @see `HFSM2_ENABLE_PLANS`
	HFSM2_CONSTEXPR(11)	CPlan plan()													  const noexcept	{ return CPlan{_core.registry, _core.planData, 0				  };	}

	/// @brief Access read-only plan for a region
	/// @param `regionId` Region identifier
	/// @return Read-only plan for the region
	/// @see `HFSM2_ENABLE_PLANS`
	HFSM2_CONSTEXPR(11)	CPlan plan(const RegionID regionId_)							  const noexcept	{ return CPlan{_core.registry, _core.planData, regionId_		  };	}

	/// @brief Access plan for a region
	/// @tparam `TRegion` Region head state type
	/// @return Plan for the region
	/// @see `HFSM2_ENABLE_PLANS`
	template <typename TRegion>
	HFSM2_CONSTEXPR(11)	CPlan plan()													  const noexcept	{ return CPlan{_core.registry, _core.planData, regionId<TRegion>()};	}

	/// @brief Succeed a plan task for a state
	/// @param `stateId` State identifier
	/// @see `HFSM2_ENABLE_PLANS`
	HFSM2_CONSTEXPR(14) void succeed(const StateID stateId_)									noexcept;

	/// @brief Succeed a plan task for a state
	/// @tparam `TState` State type
	/// @see `HFSM2_ENABLE_PLANS`
	template <typename TState>
	HFSM2_CONSTEXPR(14) void succeed()															noexcept	{ succeed(stateId<TState>());							}

	/// @brief Fail a plan task for a state
	/// @param `stateId` State identifier
	/// @see `HFSM2_ENABLE_PLANS`
	HFSM2_CONSTEXPR(14) void fail	(const StateID stateId_)									noexcept;

	/// @brief Fail a plan task for a state
	/// @tparam `TState` State type
	/// @see `HFSM2_ENABLE_PLANS`
	template <typename TState>
	HFSM2_CONSTEXPR(14) void fail	()															noexcept	{ fail	 (stateId<TState>());							}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, acts depending on the region type)
	/// @param `stateId` Destination state identifier
	/// @see `immediateChangeTo()`
	HFSM2_CONSTEXPR(14)	void changeTo			(const StateID stateId_)						noexcept;

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam `TState` Destination state type
	/// @see `immediateChangeTo()`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void changeTo			()												noexcept	{ changeTo			(stateId<TState>());				}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, activates the initial state)
	/// @param `stateId` Destination state identifier
	/// @see `immediateRestart()`
	HFSM2_CONSTEXPR(14)	void restart			(const StateID stateId_)						noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam `TState` Destination state type
	/// @see `immediateRestart()`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void restart			()												noexcept	{ restart			(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param `stateId` Destination state identifier
	/// @see `immediateResume()`
	HFSM2_CONSTEXPR(14)	void resume				(const StateID stateId_)						noexcept;

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam `TState` Destination state type
	/// @see `immediateResume()`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void resume				()												noexcept	{ resume			(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, activates the sub-state by index returned by the region's `select()` method)
	/// @param `stateId` Destination state identifier
	/// @see `immediateSelect()`
	HFSM2_CONSTEXPR(14)	void select				(const StateID stateId_)						noexcept;

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, activates the sub-state by index returned by the region's `select()` method)
	/// @tparam `TState` Destination state type
	/// @see `immediateSelect()`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void select				()												noexcept	{ select			(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, activates the state with the highest `utility()`
	///   among those with the highest `rank()`)
	/// @param `stateId` Destination state identifier
	/// @see `immediateUtilize()`
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	HFSM2_CONSTEXPR(14)	void utilize			(const StateID stateId_)						noexcept;

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, activates the state with the highest `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TState` Destination state type
	/// @see `immediateUtilize()`
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void utilize			()												noexcept	{ utilize			(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, uses weighted random to activate the state proportional to `utility()`
	///   among those with the highest `rank()`)
	/// @param `stateId` Destination state identifier
	/// @see `immediateRandomize()`
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	HFSM2_CONSTEXPR(14)	void randomize			(const StateID stateId_)						noexcept;

	/// @brief Queue a transition into a state (takes effect during `immediate*()`, `update()` or `react()`)
	///   (if transitioning into a region, uses weighted random to activate the state proportional to `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TState` Destination state type
	/// @see `immediateSelect()`
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void randomize			()												noexcept	{ randomize			(stateId<TState>());				}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param `stateId` Destination state identifier
	HFSM2_CONSTEXPR(14)	void schedule			(const StateID stateId_)						noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam `TState` Destination state type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void schedule			()												noexcept	{ schedule			(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Transition into a state
	///   (if transitioning into a region, acts depending on the region type)
	/// @param `stateId` Destination state identifier
	/// @see `changeTo()`
	HFSM2_CONSTEXPR(14)	void immediateChangeTo	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam `TState` Destination state type
	/// @see `changeTo()`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateChangeTo	()												noexcept	{ immediateChangeTo	(stateId<TState>());				}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the initial state)
	/// @param `stateId` Destination state identifier
	/// @see `restart()`
	HFSM2_CONSTEXPR(14)	void immediateRestart	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam `TState` Destination state type
	/// @see `restart()`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateRestart	()												noexcept	{ immediateRestart	(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param `stateId` Destination state identifier
	/// @see `resume()`
	HFSM2_CONSTEXPR(14)	void immediateResume	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam `TState` Destination state type
	/// @see `resume()`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateResume	()												noexcept	{ immediateResume	(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's `select()` method)
	/// @param `stateId` Destination state identifier
	/// @see `select()`
	HFSM2_CONSTEXPR(14)	void immediateSelect	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's `select()` method)
	/// @tparam `TState` Destination state type
	/// @see `select()`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateSelect	()												noexcept	{ immediateSelect	(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest `utility()`
	///   among those with the highest `rank()`)
	/// @param `stateId` Destination state identifier
	/// @see `utilize()`
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	HFSM2_CONSTEXPR(14)	void immediateUtilize	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TState` Destination state type
	/// @see `utilize()`
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateUtilize	()												noexcept	{ immediateUtilize	(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, uses weighted random to activate the state proportional to `utility()`
	///   among those with the highest `rank()`)
	/// @param `stateId` Destination state identifier
	/// @see `randomize()`
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	HFSM2_CONSTEXPR(14)	void immediateRandomize	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, uses weighted random to activate the state proportional to `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TState` Destination state type
	/// @see `randomize()`
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateRandomize	()												noexcept	{ immediateRandomize(stateId<TState>());				}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be activated or deactivated
	/// @param `stateId` Destination state identifier
	/// @return State pending activation/deactivation status
	HFSM2_CONSTEXPR(11)	bool isPendingChange	(const StateID stateId_)				  const noexcept	{ return _core.registry.isPendingChange(stateId_);		}

	/// @brief Check if a state is going to be activated or deactivated
	/// @tparam `TState` Destination state type
	/// @return State pending activation/deactivation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingChange	()										  const noexcept	{ return isPendingChange(stateId<TState>());			}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be activated
	/// @param `stateId` Destination state identifier
	/// @return State pending activation status
	HFSM2_CONSTEXPR(11)	bool isPendingEnter		(const StateID stateId_)				  const noexcept	{ return _core.registry.isPendingEnter (stateId_);		}

	/// @brief Check if a state is going to be activated
	/// @tparam `TState` Destination state type
	/// @return State pending activation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingEnter		()										  const noexcept	{ return isPendingEnter (stateId<TState>());			}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be deactivated
	/// @param `stateId` Destination state identifier
	/// @return State pending deactivation status
	HFSM2_CONSTEXPR(11)	bool isPendingExit		(const StateID stateId_)				  const noexcept	{ return _core.registry.isPendingExit  (stateId_);		}

	/// @brief Check if a state is going to be deactivated
	/// @tparam `TState` Destination state type
	/// @return State pending deactivation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingExit		()										  const noexcept	{ return isPendingExit  (stateId<TState>());			}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Reset FSM to initial state (recursively 'exit()' currently active states 'enter()' initial states)
	HFSM2_CONSTEXPR(14)	void reset()															noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

	/// @brief Array of 'StructureEntry' representing FSM structure
	/// @see HFSM2_ENABLE_STRUCTURE_REPORT
	using Structure				= StaticArrayT<StructureEntry, STATE_COUNT>;

	/// @brief Array of 'int8_t' representing FSM activation history (negative - 'update()' cycles since deactivated, positive - 'update()' cycles since activated)
	/// @see `HFSM2_ENABLE_STRUCTURE_REPORT`
	using ActivityHistory		= StaticArrayT<int8_t,         STATE_COUNT>;

	/// @brief Get the array of `StructureEntry` representing FSM structure
	/// @return FSM structure
	/// @see `HFSM2_ENABLE_STRUCTURE_REPORT`
	HFSM2_CONSTEXPR(11)	const Structure&	   structure()								  const noexcept	{ return _structure;									}

	/// @brief Get the array of `int8_t` representing FSM activation history (negative - `update()` cycles since deactivated, positive - `update()` cycles since activated)
	/// @return FSM activation history
	/// @see `HFSM2_ENABLE_STRUCTURE_REPORT`
	HFSM2_CONSTEXPR(11)	const ActivityHistory& activityHistory()						  const noexcept	{ return _activityHistory;								}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get the transition recorded during last `update()` / `react()`
	/// @return Array of last recorded transitions
	/// @see `HFSM2_ENABLE_TRANSITION_HISTORY`
	HFSM2_CONSTEXPR(11)	const TransitionSets& previousTransitions()						  const noexcept	{ return _core.previousTransitions;						}

	/// @brief Force process transitions (skips `guard()` calls)
	///   Can be used to synchronize multiple FSMs
	/// @param `transitions` Array of 'Transition' to replay
	/// @param `count` Number of transitions
	/// @return Success status
	/// @see `HFSM2_ENABLE_TRANSITION_HISTORY`
	HFSM2_CONSTEXPR(14)	bool replayTransitions(const Transition* const transitions,
											   const Short count)								noexcept;

	/// @brief Force process transitions (skips `guard()` calls)
	///   Can be used to synchronize multiple FSMs
	/// @param `transitions` Array of 'Transition' to replay
	/// @return Success status
	/// @see `HFSM2_ENABLE_TRANSITION_HISTORY`
	template <Long NCount>
	HFSM2_CONSTEXPR(14)	bool replayTransitions(const DynamicArrayT<Transition, NCount>& transitions)	noexcept;

	/// @brief Force process a transition (skips `guard()` calls)
	///   Can be used to synchronize multiple FSMs
	/// @param `transition` 'Transition' to replay
	/// @return Success status
	/// @see `HFSM2_ENABLE_TRANSITION_HISTORY`
	HFSM2_CONSTEXPR(14)	bool replayTransition (const Transition& transition)					noexcept	{ return replayTransitions(&transition, 1);				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get the last transition that caused the state to be activated
	/// @param `stateId` State identifier
	/// @return Pointer to the last transition that activated the state
	HFSM2_CONSTEXPR(14)	const Transition* lastTransitionTo(const StateID stateId_)		  const noexcept;

	/// @brief Get the last transition that caused the state to be activated
	/// @tparam `TState` State type
	/// @return Pointer to the last transition that activated the state
	template <typename TState>
	HFSM2_CONSTEXPR(14)	const Transition* lastTransitionTo()							  const noexcept	{ return lastTransitionTo(stateId<TState>());			}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_LOG_INTERFACE_AVAILABLE()

	/// @brief Attach logger
	/// @param `logger` A logger implementing 'hfsm2::LoggerInterfaceT<>' interface
	/// @see `HFSM2_ENABLE_LOG_INTERFACE`
	HFSM2_CONSTEXPR(14)	void attachLogger(Logger* const logger)									noexcept	{ _core.logger = logger;								}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	HFSM2_CONSTEXPR(14)	void initialEnter()														noexcept;
	HFSM2_CONSTEXPR(14)	void finalExit()														noexcept;

	HFSM2_CONSTEXPR(14)	void processRequest()													noexcept;
	HFSM2_CONSTEXPR(14)	void processTransitions(TransitionSets& currentTransitions)				noexcept;

	HFSM2_CONSTEXPR(14)	void applyRequest (Control& control,
										   const Transition& request,
										   const Short index)									noexcept;

	HFSM2_CONSTEXPR(14)	bool approvedByEntryGuards(const TransitionSets& currentTransitions,
												   const TransitionSet&  pendingTransitions)	noexcept;

	HFSM2_CONSTEXPR(14)	bool approvedByGuards(const TransitionSets& currentTransitions,
											  const TransitionSet&  pendingTransitions)			noexcept;

#if HFSM2_SERIALIZATION_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void save(WriteStream& stream)									  const noexcept;
	HFSM2_CONSTEXPR(14)	void load( ReadStream& stream)											noexcept;
#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	bool applyRequests(Control& control,
										   const Transition* const transitions,
										   const Short count)									noexcept;
#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void getStateNames()													noexcept;
	HFSM2_CONSTEXPR(14)	void udpateActivity()													noexcept;

	Prefixes _prefixes;

	Structure _structure;
	ActivityHistory _activityHistory;
#endif

protected:
	Core _core;
	Apex _apex;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "root_0.inl"
