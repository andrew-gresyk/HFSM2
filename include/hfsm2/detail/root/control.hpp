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
	using CPlan				= CPlanT<TArgs>;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger			= typename TArgs::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Origin final {
		HFSM2_CONSTEXPR(14)	Origin(ControlT& control_,
								   const StateID stateId)				  noexcept;

		HFSM2_CONSTEXPR(20)	~Origin()									  noexcept;

		ControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		HFSM2_CONSTEXPR(14)	Region(ControlT& control,
							const RegionID regionId)					  noexcept;

		HFSM2_CONSTEXPR(20)	~Region()									  noexcept;

		ControlT& control;
		const RegionID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(11)	ControlT(Context& context
							   , Registry& registry
							   , TransitionSet& requests
							   HFSM2_IF_UTILITY_THEORY(, RNG& rng)
							   HFSM2_IF_PLANS(, PlanData& planData)
							   HFSM2_IF_TRANSITION_HISTORY(, TransitionTargets& transitionTargets)
							   HFSM2_IF_TRANSITION_HISTORY(, const TransitionSets& previousTransitions)
							   HFSM2_IF_LOG_INTERFACE(, Logger* const logger))	  noexcept
		: _context{context}
		, _registry{registry}
		, _requests{requests}
		HFSM2_IF_PLANS(, _planData{planData})
		HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets{transitionTargets})
		HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions{previousTransitions})
		HFSM2_IF_UTILITY_THEORY(, _rng{rng})
		HFSM2_IF_LOG_INTERFACE(, _logger{logger})
	{}

	HFSM2_CONSTEXPR(14)	void setOrigin  (const StateID	 stateId)		  noexcept;
	HFSM2_CONSTEXPR(14)	void resetOrigin(const StateID	 stateId)		  noexcept;

	HFSM2_CONSTEXPR(14)	void setRegion	(const RegionID regionId)		  noexcept;
	HFSM2_CONSTEXPR(14)	void resetRegion(const RegionID regionId)		  noexcept;

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void pinLastTransition(const StateID stateId,
											   const Short index)		  noexcept;
#endif

public:

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()									  noexcept	{ return			index<StateList , TState>();	}

	/// @brief Get region identifier for a region type
	/// @tparam TState Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static constexpr RegionID regionId()								  noexcept	{ return (RegionID) index<RegionList, TState>();	}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	HFSM2_CONSTEXPR(14)		  Context& _()								  noexcept	{ return _context;									}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	HFSM2_CONSTEXPR(11)	const Context& _()							const noexcept	{ return _context;									}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	HFSM2_CONSTEXPR(14)		  Context& context()						  noexcept	{ return _context;									}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	HFSM2_CONSTEXPR(11)	const Context& context()					const noexcept	{ return _context;									}

	//----------------------------------------------------------------------

	/// @brief Inspect current transition requests
	/// @return Array of transition requests
	HFSM2_CONSTEXPR(11)	const TransitionSet& requests()				const noexcept	{ return _requests;									}

	//----------------------------------------------------------------------

	/// @brief Check if a state is active
	/// @param stateId State identifier
	/// @return State active status
	HFSM2_CONSTEXPR(11)	bool isActive   (const StateID stateId)		const noexcept	{ return _registry.isActive   (stateId);			}

	/// @brief Check if a state is active
	/// @tparam TState State type
	/// @return State active status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isActive	()							const noexcept	{ return isActive	(stateId<TState>());			}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @param stateId State identifier
	/// @return State resumable status
	HFSM2_CONSTEXPR(11)	bool isResumable(const StateID stateId)		const noexcept	{ return _registry.isResumable(stateId);			}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @tparam TState State type
	/// @return State resumable status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isResumable()							const noexcept	{ return isResumable(stateId<TState>());			}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @param stateId State identifier
	/// @return State scheduled status
	HFSM2_CONSTEXPR(11)	bool isScheduled(const StateID stateId)		const noexcept	{ return isResumable(stateId);						}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @tparam TState State type
	/// @return State scheduled status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isScheduled()							const noexcept	{ return isResumable(stateId<TState>());			}

	//----------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

	/// @brief Access read-only plan for the current region
	/// @return Plan for the current region
	HFSM2_CONSTEXPR(11)	CPlan plan()								const noexcept	{ return CPlan{_planData, _regionId};				}

	/// @brief Access read-only plan for a region
	/// @param regionId Region identifier
	/// @return Read-only plan for the region
	HFSM2_CONSTEXPR(14)	CPlan plan(const RegionID regionId)			const noexcept	{ return CPlan{_planData, regionId};				}

	/// @brief Access read-only plan for a region
	/// @tparam TRegion Region head state type
	/// @return Read-only plan for the region
	template <typename TRegion>
	HFSM2_CONSTEXPR(14)	CPlan plan()									  noexcept	{ return CPlan{_planData, regionId<TRegion>()};		}

	/// @brief Access read-only plan for a region
	/// @tparam TRegion Region head state type
	/// @return Read-only Plan for the region
	template <typename TRegion>
	HFSM2_CONSTEXPR(14)	CPlan plan()								const noexcept	{ return CPlan{_planData, regionId<TRegion>()};		}

#endif

	//----------------------------------------------------------------------

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Get transitions processed during last 'update()', 'react()' or 'replayTransitions()'
	/// @return Array of last transition requests
	HFSM2_CONSTEXPR(11)	const TransitionSets& previousTransitions()					const noexcept	{ return _previousTransitions;					}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get the last transition that caused the state to be activated
	/// @param stateId State identifier
	/// @return Pointer to the last transition that activated the state
	HFSM2_CONSTEXPR(14)	const Transition* lastTransitionTo(const StateID stateId)	const noexcept;

	/// @brief Get the last transition that caused the state to be activated
	/// @tparam TState State type
	/// @return Pointer to the last transition that activated the state
	template <typename TState>
	HFSM2_CONSTEXPR(11)	const Transition* lastTransitionTo()						const noexcept	{ return lastTransitionTo(stateId<TState>());	}

	/// @brief Get the last transition that caused the current state to be activated
	/// @return Pointer to the last transition that activated the current state
	HFSM2_CONSTEXPR(14)	const Transition* lastTransition()							const noexcept;

#endif

	//----------------------------------------------------------------------

protected:
	Context& _context;
	Registry& _registry;
	TransitionSet& _requests;
	StateID _originId = INVALID_STATE_ID;
	RegionID _regionId = 0;
	HFSM2_IF_PLANS(PlanData& _planData);
	HFSM2_IF_TRANSITION_HISTORY(TransitionTargets& _transitionTargets);
	HFSM2_IF_TRANSITION_HISTORY(const TransitionSets& _previousTransitions);
	HFSM2_IF_UTILITY_THEORY(RNG& _rng);
	HFSM2_IF_LOG_INTERFACE(Logger* _logger);
};

//------------------------------------------------------------------------------

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
	using typename Control::CPlan;

	using Plan			= PlanT<TArgs>;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		HFSM2_CONSTEXPR(14)	Region(PlanControlT& control,
							const RegionID regionId,
							const StateID index,
							const Long size) noexcept;

		HFSM2_CONSTEXPR(20)	~Region() noexcept;

		PlanControlT& control;
		const RegionID prevId;
		const Long prevIndex;
		const Long prevSize;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Control::Control;

	HFSM2_CONSTEXPR(14)	void   setRegion(const RegionID regionId, const StateID index, const Long size) noexcept;
	HFSM2_CONSTEXPR(14)	void resetRegion(const RegionID regionId, const StateID index, const Long size) noexcept;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_PLANS_AVAILABLE()
// COMMON

	/// @brief Access plan for the current region
	/// @return Plan for the current region
	HFSM2_CONSTEXPR(14)	  Plan plan()							  noexcept	{ return  Plan{_planData, _regionId};								}

// COMMON

	/// @brief Access plan for a region
	/// @param regionId
	/// @return Plan for the region
	HFSM2_CONSTEXPR(14)	  Plan plan(const RegionID regionId)	  noexcept	{ return  Plan{_planData,  regionId};								}

	/// @brief Access plan for a region
	/// @tparam TRegion Region head state type
	/// @return Plan for the region
	template <typename TRegion>
	HFSM2_CONSTEXPR(14)	  Plan plan()							  noexcept	{ return  Plan{_planData, Control::template regionId<TRegion>()};	}

// COMMON

	/// @brief Access plan for the current region
	/// @return Plan for the current region
	HFSM2_CONSTEXPR(11)	CPlan plan()						const noexcept	{ return CPlan{_planData, _regionId};								}

// COMMON

	/// @brief Access plan for a region
	/// @param regionId
	/// @return Plan for the region
	HFSM2_CONSTEXPR(11)	CPlan plan(const RegionID regionId)	const noexcept	{ return CPlan{_planData,  regionId};								}

	/// @brief Access plan for a region
	/// @tparam TRegion Region head state type
	/// @return Plan for the region
	template <typename TRegion>
	HFSM2_CONSTEXPR(11)	CPlan plan()						const noexcept	{ return CPlan{_planData, Control::template regionId<TRegion>()};	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using Control::_regionId;
	HFSM2_IF_PLANS(using Control::_planData);
	HFSM2_IF_LOG_INTERFACE(using Control::_logger);

	StateID _regionStateId = 0;
	Long _regionSize = StateList::SIZE;
	Status _status;
};

//------------------------------------------------------------------------------

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
		HFSM2_CONSTEXPR(14)	Lock(FullControlBaseT& control_)	  noexcept;
		HFSM2_CONSTEXPR(20)	~Lock()								  noexcept;

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

	//----------------------------------------------------------------------
	// COMMON

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId State identifier
	HFSM2_CONSTEXPR(14)	void changeTo (const StateID stateId)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void changeTo ()						  noexcept	{ changeTo (PlanControl::template stateId<TState>());	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param stateId State identifier
	HFSM2_CONSTEXPR(14)	void restart  (const StateID stateId)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void restart  ()						  noexcept	{ restart  (PlanControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @param stateId State identifier
	HFSM2_CONSTEXPR(14)	void resume   (const StateID stateId)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void resume   ()						  noexcept	{ resume   (PlanControl::template stateId<TState>());	}

	//----------------------------------------------------------------------

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @param stateId State identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void utilize  (const StateID stateId)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///   with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TState State type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void utilize  ()						  noexcept	{ utilize  (PlanControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @param stateId State identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void randomize(const StateID stateId)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///   proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TState State type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void randomize()						  noexcept	{ randomize(PlanControl::template stateId<TState>());	}

#endif

	//----------------------------------------------------------------------

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId State identifier
	HFSM2_CONSTEXPR(14)	void schedule (const StateID stateId)	  noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState State type
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void schedule ()						  noexcept	{ schedule (PlanControl::template stateId<TState>());	}

	//----------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

	/// @brief Succeed a plan task for the current state
	HFSM2_CONSTEXPR(14)	void succeed()							  noexcept;

	/// @brief Fail a plan task for the current state
	HFSM2_CONSTEXPR(14)	void fail()								  noexcept;

#endif

	//----------------------------------------------------------------------

protected:
	using PlanControl::_regionId;
	using PlanControl::_requests;
	HFSM2_IF_PLANS(using PlanControl::_planData);
	HFSM2_IF_LOG_INTERFACE(using PlanControl::_logger);

	using PlanControl::_originId;
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
										  const Status subStatus)	  noexcept;

#endif

public:
	using FullControlBase::context;

	using FullControlBase::isActive;
	using FullControlBase::changeTo;

	HFSM2_IF_PLANS(using FullControlBase::plan);

	//------------------------------------------------------------------------------
	// COMMON

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void changeWith   (const StateID  stateId,
										   const Payload& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void changeWith   (const StateID  stateId,
												Payload&& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void changeWith   (const Payload& payload)	  noexcept { changeWith   (FullControlBase::template stateId<TState>(),		 payload );	}

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void changeWith   (		Payload&& payload)	  noexcept { changeWith   (FullControlBase::template stateId<TState>(), move(payload));	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void restartWith  (const StateID  stateId,
										   const Payload& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void restartWith  (const StateID  stateId,
												Payload&& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void restartWith  (const Payload& payload)	  noexcept { restartWith  (FullControlBase::template stateId<TState>(),		 payload );	}

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void restartWith  (		Payload&& payload)	  noexcept { restartWith  (FullControlBase::template stateId<TState>(), move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void resumeWith   (const StateID  stateId,
										   const Payload& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void resumeWith   (const StateID  stateId,
												Payload&& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void resumeWith   (const Payload& payload)	  noexcept { resumeWith   (FullControlBase::template stateId<TState>(),		 payload );	}

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void resumeWith   (		Payload&& payload)	  noexcept { resumeWith   (FullControlBase::template stateId<TState>(), move(payload));	}

	//------------------------------------------------------------------------------

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void utilizeWith  (const StateID  stateId,
										   const Payload& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void utilizeWith  (const StateID  stateId,
												Payload&& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void utilizeWith  (const Payload& payload)	  noexcept { utilizeWith  (FullControlBase::template stateId<TState>(),		 payload );	}

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void utilizeWith  (		Payload&& payload)	  noexcept { utilizeWith  (FullControlBase::template stateId<TState>(), move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void randomizeWith(const StateID  stateId,
										   const Payload& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	void randomizeWith(const StateID  stateId,
												Payload&& payload)	  noexcept;

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void randomizeWith(const Payload& payload)	  noexcept { randomizeWith(FullControlBase::template stateId<TState>(),		 payload );	}

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void randomizeWith(		Payload&& payload)	  noexcept { randomizeWith(FullControlBase::template stateId<TState>(), move(payload));	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void scheduleWith (const StateID  stateId,
										   const Payload& payload)	  noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_CONSTEXPR(14)	void scheduleWith (const StateID  stateId,
												Payload&& payload)	  noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void scheduleWith (const Payload& payload)	  noexcept { scheduleWith (FullControlBase::template stateId<TState>(),		 payload );	}

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_CONSTEXPR(14)	void scheduleWith (		Payload&& payload)	  noexcept { scheduleWith (FullControlBase::template stateId<TState>(), move(payload));	}

	//------------------------------------------------------------------------------

protected:
	using FullControlBase::_regionId;
	using FullControlBase::_requests;
	HFSM2_IF_PLANS(using FullControlBase::_planData);
	HFSM2_IF_LOG_INTERFACE(using FullControlBase::_logger);

	using FullControlBase::_originId;
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
										  const Status subStatus)	  noexcept;

#endif

public:
	using FullControlBase::isActive;
	using FullControlBase::changeTo;

	HFSM2_IF_PLANS(using FullControlBase::plan);

protected:
	using FullControlBase::_regionId;
	HFSM2_IF_PLANS(using FullControlBase::_planData);

	using FullControlBase::_status;
};

//------------------------------------------------------------------------------

template <typename TArgs>
class GuardControlT final
	: public FullControlT<TArgs>
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename>
	friend class R_;

	using FullControl	= FullControlT<TArgs>;

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

	HFSM2_CONSTEXPR(11)	GuardControlT(Context& context
									, Registry& registry
									, TransitionSet& requests
									, const TransitionSets& currentTransitions
									, const TransitionSet& pendingTransitions
									HFSM2_IF_UTILITY_THEORY(, RNG& rng)
									HFSM2_IF_PLANS(, PlanData& planData)
									HFSM2_IF_TRANSITION_HISTORY(, TransitionTargets& transitionTargets)
									HFSM2_IF_TRANSITION_HISTORY(, const TransitionSets& previousTransitions)
									HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
		: FullControl{context
					, registry
					, requests
					HFSM2_IF_UTILITY_THEORY(, rng)
					HFSM2_IF_PLANS(, planData)
					HFSM2_IF_TRANSITION_HISTORY(, transitionTargets)
					HFSM2_IF_TRANSITION_HISTORY(, previousTransitions)
					HFSM2_IF_LOG_INTERFACE(, logger)}
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
	HFSM2_CONSTEXPR(11)	bool isPendingChange(const StateID stateId)	const noexcept { return _registry.isPendingChange(stateId);							}

	/// @brief Check if a state is going to be activated or deactivated
	/// @tparam TState State type
	/// @return State pending activation/deactivation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingChange()						const noexcept { return isPendingChange(FullControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be activated
	/// @param stateId State identifier
	/// @return State pending activation status
	HFSM2_CONSTEXPR(11)	bool isPendingEnter (const StateID stateId)	const noexcept { return _registry.isPendingEnter (stateId);							}

	/// @brief Check if a state is going to be activated
	/// @tparam TState State type
	/// @return State pending activation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingEnter ()						const noexcept { return isPendingEnter (FullControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be deactivated
	/// @param stateId State identifier
	/// @return State pending deactivation status
	HFSM2_CONSTEXPR(11)	bool isPendingExit	(const StateID stateId)	const noexcept { return _registry.isPendingExit  (stateId);							}

	/// @brief Check if a state is going to be deactivated
	/// @tparam TState State type
	/// @return State pending deactivation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingExit  ()						const noexcept { return isPendingExit  (FullControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Get current transition requests
	/// @return ArrayT of pending transition requests
	HFSM2_CONSTEXPR(11)	const TransitionSets& currentTransitions()	const noexcept { return _currentTransitions;										}

	/// @brief Get pending transition requests
	/// @return Array of pending transition requests
	HFSM2_CONSTEXPR(11)	const TransitionSet&  pendingTransitions()	const noexcept { return _pendingTransitions;										}

	/// @brief Cancel pending transition requests
	///		(can be used to substitute a transition into the current state with a different one)
	HFSM2_CONSTEXPR(14)	void cancelPendingTransitions()					  noexcept;

private:
	using FullControl::_registry;
	using FullControl::_originId;

	HFSM2_IF_LOG_INTERFACE(using FullControl::_logger);

	const TransitionSets& _currentTransitions;
	const TransitionSet&  _pendingTransitions;
	bool _cancelled = false;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "control.inl"
