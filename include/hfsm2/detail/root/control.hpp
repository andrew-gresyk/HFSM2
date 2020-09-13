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
	using TransitionSet		= Array<Transition, TArgs::COMPO_REGIONS>;
	using TransitionSets	= Array<Transition, TArgs::COMPO_REGIONS * TArgs::SUBSTITUTION_LIMIT>;

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY
	using TransitionTargets	= StaticArray<Short, TArgs::STATE_COUNT>;
#endif

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using RNG				= typename TArgs::RNG;
#endif

#ifdef HFSM2_ENABLE_PLANS
	using PlanData			= PlanDataT<TArgs>;
	using ConstPlan			= ConstPlanT<TArgs>;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger			= typename TArgs::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Origin {
		HFSM2_INLINE Origin(ControlT& control_,
							const StateID stateId);

		HFSM2_INLINE ~Origin();

		ControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		HFSM2_INLINE Region(ControlT& control,
							const RegionID regionId);

		HFSM2_INLINE ~Region();

		ControlT& control;
		const RegionID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE ControlT(Context& context
						, Registry& registry
						, TransitionSet& requests
						HFSM2_IF_UTILITY_THEORY(, RNG& rng)
						HFSM2_IF_PLANS(, PlanData& planData)
						HFSM2_IF_TRANSITION_HISTORY(, TransitionTargets& transitionTargets)
						HFSM2_IF_TRANSITION_HISTORY(, const TransitionSets& previousTransitions)
						HFSM2_IF_LOG_INTERFACE(, Logger* const logger))
		: _context{context}
		, _registry{registry}
		, _requests{requests}
		HFSM2_IF_UTILITY_THEORY(, _rng{rng})
		HFSM2_IF_PLANS(, _planData{planData})
		HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets{transitionTargets})
		HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions{previousTransitions})
		HFSM2_IF_LOG_INTERFACE(, _logger{logger})
	{}

	HFSM2_INLINE void setOrigin  (const StateID stateId);
	HFSM2_INLINE void resetOrigin(const StateID stateId);

	HFSM2_INLINE void setRegion	 (const RegionID regionId);
	HFSM2_INLINE void resetRegion(const RegionID regionId);

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY
	HFSM2_INLINE void pinLastTransition(const StateID stateId, const Short index);
#endif

public:

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()				{ return			StateList ::template index<TState>();	}

	/// @brief Get region identifier for a region type
	/// @tparam TState Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static constexpr RegionID regionId()			{ return (RegionID) RegionList::template index<TState>();	}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	HFSM2_INLINE Context& _()										{ return _context;							}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	HFSM2_INLINE Context& context()									{ return _context;							}

	//----------------------------------------------------------------------

	/// @brief Inspect current transition requests
	/// @return Array of transition requests
	HFSM2_INLINE const TransitionSet& requests()					{ return _requests;							}

	//----------------------------------------------------------------------

	/// @brief Check if a state is active
	/// @param stateId State identifier
	/// @return State active status
	HFSM2_INLINE bool isActive   (const StateID stateId) const		{ return _registry.isActive   (stateId);	}

	/// @brief Check if a state is active
	/// @tparam TState State type
	/// @return State active status
	template <typename TState>
	HFSM2_INLINE bool isActive	 () const							{ return isActive	(stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @param stateId State identifier
	/// @return State resumable status
	HFSM2_INLINE bool isResumable(const StateID stateId) const		{ return _registry.isResumable(stateId);	}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @tparam TState State type
	/// @return State resumable status
	template <typename TState>
	HFSM2_INLINE bool isResumable() const							{ return isResumable(stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @param stateId State identifier
	/// @return State scheduled status
	HFSM2_INLINE bool isScheduled(const StateID stateId) const		{ return isResumable(stateId);				}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @tparam TState State type
	/// @return State scheduled status
	template <typename TState>
	HFSM2_INLINE bool isScheduled() const							{ return isResumable(stateId<TState>());	}

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_PLANS

	/// @brief Access read-only plan for the current region
	/// @return Plan for the current region
	HFSM2_INLINE ConstPlan plan() const								{ return ConstPlan{_planData, _regionId};	}

	/// @brief Access read-only plan for a region
	/// @param regionId Region identifier
	/// @return Read-only plan for the region
	HFSM2_INLINE ConstPlan plan(const RegionID regionId) const		{ return ConstPlan{_planData, regionId};	}

	/// @brief Access read-only plan for a region
	/// @tparam TRegion Region head state type
	/// @return Read-only plan for the region
	template <typename TRegion>
	HFSM2_INLINE ConstPlan plan()							{ return ConstPlan{_planData, regionId<TRegion>()};	}

	/// @brief Access read-only plan for a region
	/// @tparam TRegion Region head state type
	/// @return Read-only Plan for the region
	template <typename TRegion>
	HFSM2_INLINE ConstPlan plan() const						{ return ConstPlan{_planData, regionId<TRegion>()};	}

#endif

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY

	/// @brief Get transitions processed during last 'update()', 'react()' or 'replayTransitions()'
	/// @return Array of last transition requests
	HFSM2_INLINE const TransitionSets& previousTransitions() const	{ return _previousTransitions;				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	const Transition* lastTransition(const StateID stateId) const;

	template <typename TState>
	const Transition* lastTransition() const						{ return lastTransition(stateId<TState>());	}

	const Transition* lastTransition() const;

#endif

	//----------------------------------------------------------------------

protected:
#ifdef HFSM2_ENABLE_LOG_INTERFACE
	HFSM2_INLINE Logger* logger()									{ return _logger;							}
#endif

protected:
	Context& _context;
	Registry& _registry;
	TransitionSet& _requests;
	StateID _originId = INVALID_STATE_ID;
	RegionID _regionId = 0;
	HFSM2_IF_UTILITY_THEORY(RNG& _rng);
	HFSM2_IF_PLANS(PlanData& _planData);
	HFSM2_IF_TRANSITION_HISTORY(TransitionTargets& _transitionTargets);
	HFSM2_IF_TRANSITION_HISTORY(const TransitionSets& _previousTransitions);
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

protected:
	using Control		= ControlT<TArgs>;

	using typename Control::StateList;
	using typename Control::RegionList;

#ifdef HFSM2_ENABLE_PLANS
	using typename Control::PlanData;
	using typename Control::ConstPlan;

	using Plan			= PlanT<TArgs>;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		HFSM2_INLINE Region(PlanControlT& control,
							const RegionID regionId,
							const StateID index,
							const Long size);

		HFSM2_INLINE ~Region();

		PlanControlT& control;
		const RegionID prevId;
		const Long prevIndex;
		const Long prevSize;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Control::Control;

	HFSM2_INLINE void setRegion  (const RegionID regionId, const StateID index, const Long size);
	HFSM2_INLINE void resetRegion(const RegionID regionId, const StateID index, const Long size);

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_PLANS

	/// @brief Access plan for the current region
	/// @return Plan for the current region
	HFSM2_INLINE	  Plan plan()								{ return	  Plan{_planData, _regionId};								}

	/// @brief Access plan for a region
	/// @param regionId
	/// @return Plan for the region
	HFSM2_INLINE	  Plan plan(const RegionID regionId)		{ return	  Plan{_planData, regionId};								}

	/// @brief Access plan for a region
	/// @tparam TRegion Region head state type
	/// @return Plan for the region
	template <typename TRegion>
	HFSM2_INLINE	  Plan plan()								{ return	  Plan{_planData, Control::template regionId<TRegion>()};	}

	/// @brief Access plan for the current region
	/// @return Plan for the current region
	HFSM2_INLINE ConstPlan plan() const							{ return ConstPlan{_planData, _regionId};								}

	/// @brief Access plan for a region
	/// @param regionId
	/// @return Plan for the region
	HFSM2_INLINE ConstPlan plan(const RegionID regionId) const	{ return ConstPlan{_planData, regionId};								}

	/// @brief Access plan for a region
	/// @tparam TRegion Region head state type
	/// @return Plan for the region
	template <typename TRegion>
	HFSM2_INLINE ConstPlan plan() const							{ return ConstPlan{_planData, Control::template regionId<TRegion>()};	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using Control::_regionId;
	HFSM2_IF_PLANS(using Control::_planData);
	HFSM2_IF_LOG_INTERFACE(using Control::_logger);

	StateID _regionIndex = 0;
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

#ifdef HFSM2_ENABLE_PLANS
	using TasksBits		= BitArray<StateID, StateList::SIZE>;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Lock {
		HFSM2_INLINE Lock(FullControlBaseT& control_);
		HFSM2_INLINE ~Lock();

		FullControlBaseT* const control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using PlanControl::PlanControl;

#ifdef HFSM2_ENABLE_PLANS

	template <typename TState>
	Status buildPlanStatus();

#endif

public:
	using PlanControl::context;

	//----------------------------------------------------------------------
	// Clang trips over 'stateId<>()', so give it a hint it comes from PlanControl

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId State identifier
	HFSM2_INLINE void changeTo (const StateID stateId);

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_INLINE void changeTo ()							{ changeTo (PlanControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param stateId State identifier
	HFSM2_INLINE void restart  (const StateID stateId);

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_INLINE void restart  ()							{ restart  (PlanControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @param stateId State identifier
	HFSM2_INLINE void resume   (const StateID stateId);

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_INLINE void resume   ()							{ resume   (PlanControl::template stateId<TState>());	}

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @param stateId State identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void utilize  (const StateID stateId);

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///   with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TState State type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void utilize  ()							{ utilize  (PlanControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @param stateId State identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void randomize(const StateID stateId);

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///   proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TState State type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void randomize()							{ randomize(PlanControl::template stateId<TState>());	}

#endif

	//----------------------------------------------------------------------

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId State identifier
	HFSM2_INLINE void schedule (const StateID stateId);

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState State type
	template <typename TState>
	HFSM2_INLINE void schedule ()							{ schedule (PlanControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_PLANS

	/// @brief Succeed a plan task for the current state
	HFSM2_INLINE void succeed();

	/// @brief Fail a plan task for the current state
	HFSM2_INLINE void fail();

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using PlanControl::_regionId;
	using PlanControl::_requests;
	HFSM2_IF_PLANS(using PlanControl::_planData);
	HFSM2_IF_LOG_INTERFACE(using PlanControl::_logger);

	using PlanControl::_originId;
	using PlanControl::_regionIndex;
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
		, Long NSerialBits
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
					   , NSerialBits
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
								  , NSerialBits
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
					 , NSerialBits
					 , NSubstitutionLimit
					 HFSM2_IF_PLANS(, NTaskCapacity)
					 , TPayload>;

protected:
	using FullControlBase = FullControlBaseT<Args>;

	using typename FullControlBase::Payload;
	using typename FullControlBase::Transition;

	using typename FullControlBase::Origin;

#ifdef HFSM2_ENABLE_PLANS
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using FullControlBase::FullControlBase;

#ifdef HFSM2_ENABLE_PLANS

	template <typename TState>
	Status updatePlan(TState& headState, const Status subStatus);

#endif

public:
	using FullControlBase::context;

	using FullControlBase::isActive;
	using FullControlBase::changeTo;

	HFSM2_IF_PLANS(using FullControlBase::plan);

	//------------------------------------------------------------------------------

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void changeWith   (const StateID  stateId,
									const Payload& payload);

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void changeWith   (const StateID  stateId,
										 Payload&& payload);

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void changeWith   (const Payload& payload)	{ changeWith   (FullControlBase::template stateId<TState>(),		   payload );	}

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void changeWith   (	 Payload&& payload)	{ changeWith   (FullControlBase::template stateId<TState>(), std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void restartWith  (const StateID  stateId,
									const Payload& payload);

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void restartWith  (const StateID  stateId,
										 Payload&& payload);

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void restartWith  (const Payload& payload)	{ restartWith  (FullControlBase::template stateId<TState>(),		   payload );	}

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void restartWith  (	 Payload&& payload)	{ restartWith  (FullControlBase::template stateId<TState>(), std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void resumeWith   (const StateID  stateId,
									const Payload& payload);

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void resumeWith   (const StateID  stateId,
										 Payload&& payload);

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void resumeWith   (const Payload& payload)	{ resumeWith   (FullControlBase::template stateId<TState>(),		   payload );	}

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void resumeWith   (	 Payload&& payload)	{ resumeWith   (FullControlBase::template stateId<TState>(), std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void utilizeWith  (const StateID  stateId,
									const Payload& payload);

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void utilizeWith  (const StateID  stateId,
										 Payload&& payload);

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void utilizeWith  (const Payload& payload)	{ utilizeWith  (FullControlBase::template stateId<TState>(),		   payload );	}

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void utilizeWith  (	 Payload&& payload)	{ utilizeWith  (FullControlBase::template stateId<TState>(), std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void randomizeWith(const StateID  stateId,
									const Payload& payload);

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void randomizeWith(const StateID  stateId,
										 Payload&& payload);

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void randomizeWith(const Payload& payload)	{ randomizeWith(FullControlBase::template stateId<TState>(),		   payload );	}

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void randomizeWith(	 Payload&& payload)	{ randomizeWith(FullControlBase::template stateId<TState>(), std::move(payload));	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void scheduleWith (const StateID  stateId,
									const Payload& payload);

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void scheduleWith (const StateID  stateId,
										 Payload&& payload);

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void scheduleWith (const Payload& payload)	{ scheduleWith (FullControlBase::template stateId<TState>(),		   payload );	}

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void scheduleWith (	 Payload&& payload)	{ scheduleWith (FullControlBase::template stateId<TState>(), std::move(payload));	}

	//------------------------------------------------------------------------------

protected:
	using FullControlBase::_regionId;
	using FullControlBase::_requests;
	HFSM2_IF_PLANS(using FullControlBase::_planData);
	HFSM2_IF_LOG_INTERFACE(using FullControlBase::_logger);

	using FullControlBase::_originId;
	using FullControlBase::_regionIndex;
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
		, Long NSerialBits
		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)>
class FullControlT<ArgsT<TContext
					   , TConfig
					   , TStateList
					   , TRegionList
					   , NCompoCount
					   , NOrthoCount
					   , NOrthoUnits
					   , NSerialBits
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
								  , NSerialBits
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
					 , NSerialBits
					 , NSubstitutionLimit
					 HFSM2_IF_PLANS(, NTaskCapacity)
					 , void>;

protected:
	using FullControlBase = FullControlBaseT<Args>;

	using typename FullControlBase::Origin;

#ifdef HFSM2_ENABLE_PLANS
	using typename FullControlBase::Plan;
	using typename FullControlBase::TasksBits;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using FullControlBase::FullControlBase;

#ifdef HFSM2_ENABLE_PLANS

	template <typename TState>
	Status updatePlan(TState& headState, const Status subStatus);

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

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY
	using typename FullControl::TransitionTargets;
#endif

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using typename FullControl::RNG;
#endif

#ifdef HFSM2_ENABLE_PLANS
	using typename FullControl::PlanData;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using typename FullControl::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE GuardControlT(Context& context
							 , Registry& registry
							 , TransitionSet& requests
							 , const TransitionSets& currentTransitions
							 , const TransitionSet& pendingTransitions
							 HFSM2_IF_UTILITY_THEORY(, RNG& rng)
							 HFSM2_IF_PLANS(, PlanData& planData)
							 HFSM2_IF_TRANSITION_HISTORY(, TransitionTargets& transitionTargets)
							 HFSM2_IF_TRANSITION_HISTORY(, const TransitionSets& previousTransitions)
							 HFSM2_IF_LOG_INTERFACE(, Logger* const logger))
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
	HFSM2_INLINE bool isPendingChange(const StateID stateId) const	{ return _registry.isPendingChange(stateId);	}

	/// @brief Check if a state is going to be activated or deactivated
	/// @tparam TState State type
	/// @return State pending activation/deactivation status
	template <typename TState>
	HFSM2_INLINE bool isPendingChange()			{ return isPendingChange(FullControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be activated
	/// @param stateId State identifier
	/// @return State pending activation status
	HFSM2_INLINE bool isPendingEnter (const StateID stateId) const	{ return _registry.isPendingEnter (stateId);	}

	/// @brief Check if a state is going to be activated
	/// @tparam TState State type
	/// @return State pending activation status
	template <typename TState>
	HFSM2_INLINE bool isPendingEnter ()			{ return isPendingEnter (FullControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be deactivated
	/// @param stateId State identifier
	/// @return State pending deactivation status
	HFSM2_INLINE bool isPendingExit	 (const StateID stateId) const	{ return _registry.isPendingExit  (stateId);	}

	/// @brief Check if a state is going to be deactivated
	/// @tparam TState State type
	/// @return State pending deactivation status
	template <typename TState>
	HFSM2_INLINE bool isPendingExit  ()			{ return isPendingExit  (FullControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE const TransitionSets& currentTransitions() const	{ return _currentTransitions;					}

	/// @brief Get pending transition requests
	/// @return Array of pending transition requests
	HFSM2_INLINE const TransitionSet&  pendingTransitions() const	{ return _pendingTransitions;					}

	/// @brief Cancel pending transition requests
	///		(can be used to substitute a transition into the current state with a different one)
	HFSM2_INLINE void cancelPendingTransitions();

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
