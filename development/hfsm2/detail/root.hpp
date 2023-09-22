namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename TConfig,
		  typename TApex>
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
	static_assert(Args::COMPO_REGIONS >= 1, "State machine needs at least one composite region");

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

	static constexpr Long	SUBSTITUTION_LIMIT	= Forward::SUBSTITUTION_LIMIT;

#if HFSM2_PLANS_AVAILABLE()
	using PlanData				= PlanDataT<Args>;
#endif

public:
#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream			= typename Args::WriteStream;
	using ReadStream			= typename Args::ReadStream;
#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	static constexpr Long NAME_COUNT = Apex::NAME_COUNT;
#endif

public:
	using Info					= WrapInfo<TApex>;

	/// @brief Transition
	using Transition			= typename Control::Transition;

	/// @brief Array of transitions
	using TransitionSet			= typename Control::TransitionSet;
	using TransitionSets		= typename Control::TransitionSets;

#if HFSM2_PLANS_AVAILABLE()
	using CPlan					= CPlanT<Args>;
	using Plan					=  PlanT<Args>;

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

	/// @brief Access context
	/// @return context
	HFSM2_CONSTEXPR(14)		  Context& context()												noexcept	{ return _core.context;		}

	/// @brief Access context
	/// @return context
	HFSM2_CONSTEXPR(11)	const Context& context()										  const noexcept	{ return _core.context;		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()															noexcept	{ return					   index<StateList , TState>() ;	}

	/// @brief Get region identifier for a region type
	/// @tparam TState Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static constexpr RegionID regionId()														noexcept	{ return static_cast<RegionID>(index<RegionList, TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Access state instance
	/// @tparam TState State type
	/// @return State instance
	template <typename TState>
	HFSM2_CONSTEXPR(14)		  TState& access()													noexcept	{ return static_cast<	   TState&>(_apex);		}

	/// @brief Access state instance
	/// @tparam TState State type
	/// @return State instance
	template <typename TState>
	HFSM2_CONSTEXPR(11)	const TState& access()											  const noexcept	{ return static_cast<const TState&>(_apex);		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Trigger FSM update cycle (recursively call 'update()' from the root down to the leaf states,
	///   on all active states, then process requested transitions)
	HFSM2_CONSTEXPR(14)	void update()															noexcept;

	/// @brief Have FSM react to an event (recursively call matching 'react<>()' from the root down to the leaf states,
	///   on all active states, then process requested transitions)
	/// @tparam TEvent Event type
	/// @param event Event to react to
	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void react(const TEvent& event)											noexcept;

	/// @brief Recursively call 'query()' from the root down to the leaf states, on all active states
	/// @tparam TEvent Event type
	/// @param event Event to react to
	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void query(TEvent& event)										  const noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get region's active sub-state's index
	/// @param stateId Region's head state ID
	/// @return Region's active sub-state index
	HFSM2_CONSTEXPR(14)	Short activeSubState (const StateID stateId_)					  const noexcept	{ return _core.registry.activeSubState(stateId_			);	}

	/// @brief Get region's active sub-state's index
	/// @tparam TState Region's head state type
	/// @return Region's active sub-state index
	template <typename TState>
	HFSM2_CONSTEXPR(14)	Short activeSubState ()											  const noexcept	{ return _core.registry.activeSubState(stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is active
	/// @param stateId Destination state identifier
	/// @return State active status
	HFSM2_CONSTEXPR(11)	bool isActive		 (const StateID stateId_)					  const noexcept	{ return _core.registry.isActive   (stateId_);			}

	/// @brief Check if a state is active
	/// @tparam TState Destination state type
	/// @return State active status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isActive		 ()											  const noexcept	{ return isActive	(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @param stateId Destination state identifier
	/// @return State resumable status
	HFSM2_CONSTEXPR(11)	bool isResumable	 (const StateID stateId_)					  const noexcept	{ return _core.registry.isResumable(stateId_);			}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @tparam TState Destination state type
	/// @return State resumable status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isResumable	 ()											  const noexcept	{ return isResumable(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @param stateId Destination state identifier
	/// @return State scheduled status
	HFSM2_CONSTEXPR(11)	bool isScheduled	 (const StateID stateId_)					  const noexcept	{ return isResumable(stateId_);							}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @tparam TState Destination state type
	/// @return State scheduled status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isScheduled	 ()											  const noexcept	{ return isResumable<TState>();							}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_PLANS_AVAILABLE()

	/// @brief Access read-only plan for the root region
	/// @return Plan for the root region
	HFSM2_CONSTEXPR(14)  Plan plan()															noexcept	{ return  Plan{_core.registry, _core.planData, 0				  };	}

	/// @brief Access plan for a region
	/// @param regionId Region ID
	/// @return Plan for the region
	HFSM2_CONSTEXPR(14)  Plan plan(const RegionID regionId_)									noexcept	{ return  Plan{_core.registry, _core.planData, regionId_		  };	}

	/// @brief Access plan for a region
	/// @tparam TRegion Region head state type
	/// @return Plan for the region
	template <typename TRegion>
	HFSM2_CONSTEXPR(14)  Plan plan()															noexcept	{ return  Plan{_core.registry, _core.planData, regionId<TRegion>()};	}

	/// @brief Access plan for the root region
	/// @return Plan for the root region
	HFSM2_CONSTEXPR(11)	CPlan plan()													  const noexcept	{ return CPlan{_core.registry, _core.planData, 0				  };	}

	/// @brief Access plan for a region
	/// @param regionId Region ID
	/// @return Plan for the region
	HFSM2_CONSTEXPR(11)	CPlan plan(const RegionID regionId_)							  const noexcept	{ return CPlan{_core.registry, _core.planData, regionId_		  };	}

	/// @brief Access plan for a region
	/// @tparam TRegion Region head state type
	/// @return Plan for the region
	template <typename TRegion>
	HFSM2_CONSTEXPR(11)	CPlan plan()													  const noexcept	{ return CPlan{_core.registry, _core.planData, regionId<TRegion>()};	}

	/// @brief Succeed a plan task for a state
	/// @param stateId State ID
	HFSM2_CONSTEXPR(14) void succeed(const StateID stateId_)									noexcept;

	/// @brief Succeed a plan task for a state
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14) void succeed()															noexcept	{ succeed(stateId<TState>());							}

	/// @brief Fail a plan task for a state
	/// @param stateId State ID
	HFSM2_CONSTEXPR(14) void fail	(const StateID stateId_)									noexcept;

	/// @brief Fail a plan task for a state
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14) void fail	()															noexcept	{ fail	 (stateId<TState>());							}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	HFSM2_CONSTEXPR(14)	void changeTo			(const StateID stateId_)						noexcept;

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void changeTo			()												noexcept	{ changeTo			(stateId<TState>());				}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	HFSM2_CONSTEXPR(14)	void restart			(const StateID stateId_)						noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void restart			()												noexcept	{ restart			(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	HFSM2_CONSTEXPR(14)	void resume				(const StateID stateId_)						noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void resume				()												noexcept	{ resume			(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @param stateId Destination state identifier
	HFSM2_CONSTEXPR(14)	void select				(const StateID stateId_)						noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void select				()												noexcept	{ select			(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void utilize			(const StateID stateId_)						noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void utilize			()												noexcept	{ utilize			(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void randomize			(const StateID stateId_)						noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void randomize			()												noexcept	{ randomize			(stateId<TState>());				}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId Destination state identifier
	HFSM2_CONSTEXPR(14)	void schedule			(const StateID stateId_)						noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void schedule			()												noexcept	{ schedule			(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Transition into a state
	///   (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	HFSM2_CONSTEXPR(14)	void immediateChangeTo	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateChangeTo	()												noexcept	{ immediateChangeTo	(stateId<TState>());				}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	HFSM2_CONSTEXPR(14)	void immediateRestart	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateRestart	()												noexcept	{ immediateRestart	(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	HFSM2_CONSTEXPR(14)	void immediateResume	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateResume	()												noexcept	{ immediateResume	(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @param stateId Destination state identifier
	HFSM2_CONSTEXPR(14)	void immediateSelect	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateSelect	()												noexcept	{ immediateSelect	(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void immediateUtilize	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateUtilize	()												noexcept	{ immediateUtilize	(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void immediateRandomize	(const StateID stateId_)						noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateRandomize	()												noexcept	{ immediateRandomize(stateId<TState>());				}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be activated or deactivated
	/// @param stateId Destination state identifier
	/// @return State pending activation/deactivation status
	HFSM2_CONSTEXPR(11)	bool isPendingChange	(const StateID stateId_)				  const noexcept	{ return _core.registry.isPendingChange(stateId_);		}

	/// @brief Check if a state is going to be activated or deactivated
	/// @tparam TState Destination state type
	/// @return State pending activation/deactivation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingChange	()										  const noexcept	{ return isPendingChange(stateId<TState>());			}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be activated
	/// @param stateId Destination state identifier
	/// @return State pending activation status
	HFSM2_CONSTEXPR(11)	bool isPendingEnter		(const StateID stateId_)				  const noexcept	{ return _core.registry.isPendingEnter (stateId_);		}

	/// @brief Check if a state is going to be activated
	/// @tparam TState Destination state type
	/// @return State pending activation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingEnter		()										  const noexcept	{ return isPendingEnter (stateId<TState>());			}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be deactivated
	/// @param stateId Destination state identifier
	/// @return State pending deactivation status
	HFSM2_CONSTEXPR(11)	bool isPendingExit		(const StateID stateId_)				  const noexcept	{ return _core.registry.isPendingExit  (stateId_);		}

	/// @brief Check if a state is going to be deactivated
	/// @tparam TState Destination state type
	/// @return State pending deactivation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingExit		()										  const noexcept	{ return isPendingExit  (stateId<TState>());			}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Reset FSM to initial state (recursively 'exit()' currently active states, 'enter()' initial states)
	HFSM2_CONSTEXPR(14)	void reset()															noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get the list of transitions recorded during last 'update()'
	/// @return Array of last recorded transitions
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	HFSM2_CONSTEXPR(11)	const TransitionSets& previousTransitions()						  const noexcept	{ return _core.previousTransitions;						}

	/// @brief Force process transitions (skips 'guard()' calls)
	///  Can be used to synchronize multiple FSMs
	/// @param transitions 'TransitionHistory' to replay
	/// @param count Number of transitions
	/// @return Success status
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	HFSM2_CONSTEXPR(14)	bool replayTransitions(const Transition* const transitions,
											   const Short count)								noexcept;

	/// @brief Force process transitions (skips 'guard()' calls)
	///  Can be used to synchronize multiple FSMs
	/// @param transitions Array of 'Transition' to replay
	/// @return Success status
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	template <Long NCount>
	HFSM2_CONSTEXPR(14)	bool replayTransitions(const ArrayT<Transition, NCount>& transitions)	noexcept;

	/// @brief Force process a transition (skips 'guard()' calls)
	///  Can be used to synchronize multiple FSMs
	/// @param transition 'Transition' to replay
	/// @return Success status
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	HFSM2_CONSTEXPR(14)	bool replayTransition (const Transition& transition)					noexcept	{ return replayTransitions(&transition, 1);				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get the last transition that caused the state to be activated
	/// @param stateId State identifier
	/// @return Pointer to the last transition that activated the state
	HFSM2_CONSTEXPR(14)	const Transition* lastTransitionTo(const StateID stateId_)		  const noexcept;

	/// @brief Get the last transition that caused the state to be activated
	/// @tparam TState State type
	/// @return Pointer to the last transition that activated the state
	template <typename TState>
	HFSM2_CONSTEXPR(14)	const Transition* lastTransitionTo()							  const noexcept	{ return lastTransitionTo(stateId<TState>());			}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

	/// @brief Array of 'StructureEntry' representing FSM structure
	/// @see HFSM2_ENABLE_STRUCTURE_REPORT
	using Structure				= ArrayT<StructureEntry, NAME_COUNT>;

	/// @brief Array of 'int8_t' representing FSM activation history (negative - 'update()' cycles since deactivated, positive - 'update()' cycles since activated)
	/// @see HFSM2_ENABLE_STRUCTURE_REPORT
	using ActivityHistory		= ArrayT<int8_t,		 NAME_COUNT>;

	/// @brief Get the array of 'StructureEntry' representing FSM structure
	/// @return FSM structure
	/// @see HFSM2_ENABLE_STRUCTURE_REPORT
	HFSM2_CONSTEXPR(11)	const Structure&	   structure()								  const noexcept	{ return _structure;									}

	/// @brief Get the array of 'int8_t' representing FSM activation history (negative - 'update()' cycles since deactivated, positive - 'update()' cycles since activated)
	/// @return FSM activation history
	/// @see HFSM2_ENABLE_STRUCTURE_REPORT
	HFSM2_CONSTEXPR(11)	const ActivityHistory& activityHistory()						  const noexcept	{ return _activityHistory;								}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_LOG_INTERFACE_AVAILABLE()

	/// @brief Attach logger
	/// @param logger A logger implementing 'hfsm2::LoggerInterfaceT<>' interface
	/// @see HFSM2_ENABLE_LOG_INTERFACE
	HFSM2_CONSTEXPR(14)	void attachLogger(Logger* const logger)									noexcept	{ _core.logger = logger;								}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	HFSM2_CONSTEXPR(14)	void initialEnter()														noexcept;
	HFSM2_CONSTEXPR(14)	void finalExit()														noexcept;

	HFSM2_CONSTEXPR(14)	void processRequest()													noexcept;
	HFSM2_CONSTEXPR(14)	void processTransitions(TransitionSets& currentTransitions)				noexcept;

	HFSM2_CONSTEXPR(14)	bool applyRequest(const TransitionSets& currentTransitions,
										  Control& control,
										  const Transition& request,
										  const Short index)									noexcept;

	HFSM2_CONSTEXPR(14)	bool applyRequests(const TransitionSets& currentTransitions,
										   Control& control)									noexcept;

	HFSM2_CONSTEXPR(14)	bool cancelledByEntryGuards(const TransitionSets& currentTransitions,
													const TransitionSet&  pendingTransitions)	noexcept;

	HFSM2_CONSTEXPR(14)	bool cancelledByGuards(const TransitionSets& currentTransitions,
											   const TransitionSet&  pendingTransitions)		noexcept;

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
	NamedStates _namedStates;

	Structure _structure;
	ActivityHistory _activityHistory;
#endif

protected:
	Core _core;
	Apex _apex;
};

////////////////////////////////////////////////////////////////////////////////

// Automatic / manual [de]activation

template <typename, typename>
class RV_;

//------------------------------------------------------------------------------
// Automatic enter() / exit()

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RV_		   <G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 R_<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = R_<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

protected:
	using typename Base::Context;
	using typename Base::PureContext;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

#if HFSM2_SERIALIZATION_AVAILABLE()
	using typename Base::Args;
	using typename Base::WriteStream;
	using typename Base::ReadStream;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	HFSM2_CONSTEXPR(14)	explicit RV_(Context& context
								   HFSM2_IF_UTILITY_THEORY(, RNG& rng)
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	HFSM2_CONSTEXPR(14)	explicit RV_(PureContext&& context
								   HFSM2_IF_UTILITY_THEORY(, RNG& rng)
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	HFSM2_CONSTEXPR(14)	RV_(const RV_&  other)													noexcept;
	HFSM2_CONSTEXPR(14)	RV_(	  RV_&& other)													noexcept;

	HFSM2_CONSTEXPR(20)	~RV_()																	noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_SERIALIZATION_AVAILABLE()

	/// @brief Buffer for serialization
	/// @see `HFSM2_ENABLE_SERIALIZATION`
	using SerialBuffer			= typename Args::SerialBuffer;

	/// @brief Serialize FSM into 'buffer'
	/// @param buffer `SerialBuffer` to serialize to
	/// @see `HFSM2_ENABLE_SERIALIZATION`
	HFSM2_CONSTEXPR(14)	void save(		SerialBuffer& buffer)							  const noexcept;

	/// @brief De-serialize FSM from 'buffer'
	/// @param buffer `SerialBuffer` to de-serialize from
	/// @see `HFSM2_ENABLE_SERIALIZATION`
	HFSM2_CONSTEXPR(14)	void load(const SerialBuffer& buffer)									noexcept;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
#if HFSM2_SERIALIZATION_AVAILABLE()
	using Base::save;
	using Base::load;
#endif

private:
	using Base::initialEnter;
	using Base::finalExit;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Manual enter() / exit()

template <FeatureTag NFeatureTag
		, typename TContext

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RV_		   <G_<NFeatureTag, TContext, Manual HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 R_<G_<NFeatureTag, TContext, Manual HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = R_<G_<NFeatureTag, TContext, Manual HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	using typename Base::Transition;

protected:
	HFSM2_IF_UTILITY_THEORY(using typename Base::RNG);

#if HFSM2_SERIALIZATION_AVAILABLE()
	using typename Base::Args;
	using typename Base::WriteStream;
	using typename Base::ReadStream;
#endif

#if HFSM2_SERIALIZATION_AVAILABLE() || HFSM2_TRANSITION_HISTORY_AVAILABLE()
	using typename Base::PlanControl;
	using typename Base::TransitionSets;
#endif

public:
	using Base::Base;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if FSM is active
	/// @return FSM active status
	HFSM2_CONSTEXPR(11)	bool isActive()												  const noexcept	{ return _core.registry.isActive();	}

	using Base::isActive;

	/// @brief Manually start the FSM
	///  Can be used with UE4 to start / stop the FSM in BeginPlay() / EndPlay()
	HFSM2_CONSTEXPR(14)	void enter()														noexcept	{ initialEnter();	}

	/// @brief Manually stop the FSM
	///  Can be used with UE4 to start / stop the FSM in BeginPlay() / EndPlay()
	HFSM2_CONSTEXPR(14)	void exit()															noexcept	{ finalExit();		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_SERIALIZATION_AVAILABLE()

	/// @brief Buffer for serialization
	/// @see `HFSM2_ENABLE_SERIALIZATION`
	using SerialBuffer			= typename Args::SerialBuffer;

	/// @brief Serialize FSM into 'buffer'
	/// @param buffer `SerialBuffer` to serialize to
	/// @see `HFSM2_ENABLE_SERIALIZATION`
	HFSM2_CONSTEXPR(14)	void save(		SerialBuffer& buffer)							  const noexcept;

	/// @brief De-serialize FSM from 'buffer'
	/// @param buffer `SerialBuffer` to de-serialize from
	/// @see `HFSM2_ENABLE_SERIALIZATION`
	HFSM2_CONSTEXPR(14)	void load(const SerialBuffer& buffer)									noexcept;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Start the FSM from a specific state
	///  Can be used to synchronize multiple FSMs
	/// @param transitions 'TransitionHistory' to replay
	/// @param count Number of transitions
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	HFSM2_CONSTEXPR(14)	bool replayEnter(const Transition* const transitions,
										 const Short count)									noexcept;

	/// @brief Start the FSM from a specific state
	///  Can be used to synchronize multiple FSMs
	/// @param transitions Array of 'Transition' to replay
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	template <Long NCount>
	HFSM2_CONSTEXPR(14)	bool replayEnter(const ArrayT<Transition, NCount>& transitions)		noexcept;

	/// @brief Start the FSM from a specific state
	///  Can be used to synchronize multiple FSMs
	/// @param transition 'Transition' to replay
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	HFSM2_CONSTEXPR(14)	bool replayEnter(const Transition& transition)						noexcept	{ return replayEnter(&transition, 1);	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
#if HFSM2_SERIALIZATION_AVAILABLE()
	using Base::save;
	using Base::load;

	HFSM2_CONSTEXPR(14)	void loadEnter(ReadStream& stream)										noexcept;
#endif

protected:
	using Base::initialEnter;
	using Base::finalExit;

	using Base::_core;

#if HFSM2_SERIALIZATION_AVAILABLE() || HFSM2_TRANSITION_HISTORY_AVAILABLE()
	using Base::_apex;
#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	using Base::applyRequests;
	HFSM2_IF_STRUCTURE_REPORT(using Base::udpateActivity);
#endif
};

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig,
		  typename TApex>
class RP_;

//------------------------------------------------------------------------------
// Non-'void' payloads

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RP_			<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RV_<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RV_<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

	using Transition = TransitionT<TPayload>;

public:
	using typename Base::Payload;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

public:
	using Base::Base;
	using Base::processRequest;

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID  stateId()												noexcept	{ return Base::template stateId<TState>();				}

	/// @brief Get region identifier for a region type
	/// @tparam TState Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static constexpr RegionID regionId()											noexcept	{ return Base::template regionId<TState>();				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void changeWith				(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void changeWith				(const Payload& payload)		noexcept	{ changeWith			(stateId<TState>(), payload );	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void restartWith			(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void restartWith			(const Payload& payload)		noexcept	{ restartWith			(stateId<TState>(), payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void resumeWith				(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void resumeWith				(const Payload& payload)		noexcept	{ resumeWith			(stateId<TState>(), payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void selectWith				(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void selectWith				(const Payload& payload)		noexcept	{ selectWith			(stateId<TState>(), payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void utilizeWith			(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void utilizeWith			(const Payload& payload)		noexcept	{ utilizeWith			(stateId<TState>(), payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void randomizeWith			(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Queue a transition into a state (takes effect during immediate*(), update() or react())
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void randomizeWith			(const Payload& payload)		noexcept	{ randomizeWith			(stateId<TState>(), payload );	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void scheduleWith			(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void scheduleWith			(const Payload& payload)		noexcept	{ scheduleWith			(stateId<TState>(), payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void immediateChangeWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateChangeWith	(const Payload& payload)		noexcept	{ immediateChangeWith   (stateId<TState>(), payload );	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void immediateRestartWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateRestartWith	(const Payload& payload)		noexcept	{ immediateRestartWith	(stateId<TState>(), payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void immediateResumeWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateResumeWith	(const Payload& payload)		noexcept	{ immediateResumeWith	(stateId<TState>(), payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void immediateSelectWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateSelectWith	(const Payload& payload)		noexcept	{ immediateSelectWith	(stateId<TState>(), payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void immediateUtilizeWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateUtilizeWith	(const Payload& payload)		noexcept	{ immediateUtilizeWith	(stateId<TState>(), payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void immediateRandomizeWith	(const StateID  stateId_,
													 const Payload& payload)		noexcept;

	/// @brief Transition into a state
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void immediateRandomizeWith	(const Payload& payload)		noexcept	{ immediateRandomizeWith(stateId<TState>(), payload );	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using Base::_core;
};

//------------------------------------------------------------------------------

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TApex>
class RP_			<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>
	: public	 RV_<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>
{
	using Base = RV_<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>;

public:
#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

public:
	using Base::Base;
};

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename, typename>
class RC_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RC_			<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RP_<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RP_<G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;
	using typename Base::PureContext;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	HFSM2_CONSTEXPR(14)	explicit RC_(Context& context
								   HFSM2_IF_UTILITY_THEORY(, RNG& rng)
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	HFSM2_CONSTEXPR(14)	explicit RC_(PureContext&& context
								   HFSM2_IF_UTILITY_THEORY(, RNG& rng)
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	HFSM2_CONSTEXPR(NO) RC_(const RC_& )														noexcept = default;
	HFSM2_CONSTEXPR(NO) RC_(	  RC_&&)														noexcept = default;

private:
	using Base::_core;
};

//------------------------------------------------------------------------------
// TContext&

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RC_			<G_<NFeatureTag, TContext&, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RP_<G_<NFeatureTag, TContext&, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RP_<G_<NFeatureTag, TContext&, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	using Base::Base;

private:
	using Base::_core;
};

//------------------------------------------------------------------------------
// TContext*

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RC_			<G_<NFeatureTag, TContext*, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RP_<G_<NFeatureTag, TContext*, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RP_<G_<NFeatureTag, TContext*, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
#if HFSM2_UTILITY_THEORY_AVAILABLE()

	HFSM2_CONSTEXPR(14)	explicit RC_(Context context
								   , RNG& rng
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

#else

	HFSM2_CONSTEXPR(14)	explicit RC_(Context context = nullptr
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

#endif

	HFSM2_CONSTEXPR(NO)	RC_(const RC_& )														noexcept = default;
	HFSM2_CONSTEXPR(NO)	RC_(	  RC_&&)														noexcept = default;

	HFSM2_CONSTEXPR(14)	void setContext(Context context)										noexcept	{ _core.context = context; }

private:
	using Base::_core;
};

//------------------------------------------------------------------------------
// TContext == EmptyContext

template <FeatureTag NFeatureTag
		, typename TActivation

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RC_			<G_<NFeatureTag, EmptyContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RP_<G_<NFeatureTag, EmptyContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	, public EmptyContext
{
	using Base = RP_<G_<NFeatureTag, EmptyContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	HFSM2_CONSTEXPR(14)	explicit RC_(RNG& rng
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

#else

	HFSM2_CONSTEXPR(14)	explicit RC_(HFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr))	noexcept;

#endif

	HFSM2_CONSTEXPR(NO)	RC_(const RC_& )														noexcept = default;
	HFSM2_CONSTEXPR(NO)	RC_(	  RC_&&)														noexcept = default;
};

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename TConfig,
		  typename TApex>
class InstanceT final
	: public	 RC_<TConfig, TApex>
{
	using Base = RC_<TConfig, TApex>;

public:
	using Base::Base;
};

#if HFSM2_UTILITY_THEORY_AVAILABLE()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TRNG == RNGT<TUtility>

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation
		, typename TRank
		, typename TUtility
		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class InstanceT	    <G_<NFeatureTag, TContext, TActivation, TRank, TUtility, RNGT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex> final
	: public	 RC_<G_<NFeatureTag, TContext, TActivation, TRank, TUtility, RNGT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	, public RNGT<TUtility>
{
	using Base = RC_<G_<NFeatureTag, TContext, TActivation, TRank, TUtility, RNGT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	HFSM2_CONSTEXPR(14)	explicit InstanceT(Context& context
										 HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept
		: Base{context
			 , static_cast<RNGT<TUtility>&>(*this)
			 HFSM2_IF_LOG_INTERFACE(, logger)}
		, RNGT<TUtility>{0}
	{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext == EmptyContext
// TRNG == RNGT<TUtility>

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TActivation
		, typename TRank
		, typename TUtility
		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class InstanceT		<G_<NFeatureTag, EmptyContext, TActivation, TRank, TUtility, RNGT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex> final
	: public	 RC_<G_<NFeatureTag, EmptyContext, TActivation, TRank, TUtility, RNGT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	, public RNGT<TUtility>
{
	using Base = RC_<G_<NFeatureTag, EmptyContext, TActivation, TRank, TUtility, RNGT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	HFSM2_CONSTEXPR(14)	explicit InstanceT(HFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr))	noexcept
		: Base{static_cast<RNGT<TUtility>&>(*this)
			 HFSM2_IF_LOG_INTERFACE(, logger)}
		, RNGT<TUtility>{0}
	{}
};

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#include "root.inl"
