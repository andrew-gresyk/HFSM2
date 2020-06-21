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

	using Args			= TArgs;

protected:
	using Context		= typename Args::Context;

#ifdef HFSM_ENABLE_UTILITY_THEORY
	using RNG			= typename Args::RNG;
#endif

#ifdef HFSM_ENABLE_LOG_INTERFACE
	using Logger		= typename Args::Logger;
#endif

	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using Registry		= RegistryT<Args>;
	using PlanData		= PlanDataT<Args>;
	using ConstPlan		= ConstPlanT<Args>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		HFSM_INLINE Region(ControlT& control,
						   const RegionID id);

		HFSM_INLINE ~Region();

		ControlT& control;
		const RegionID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE ControlT(Context& context
						 HFSM_IF_UTILITY_THEORY(, RNG& rng)
						 , Registry& registry
						 , PlanData& planData
						 HFSM_IF_LOG_INTERFACE(, Logger* const logger))
		: _context{context}
		HFSM_IF_UTILITY_THEORY(, _rng{rng})
		, _registry{registry}
		, _planData{planData}
		HFSM_IF_LOG_INTERFACE(, _logger{logger})
	{}


	HFSM_INLINE void setRegion(const RegionID id);
	HFSM_INLINE void resetRegion(const RegionID id);

public:

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID  stateId()					{ return			StateList ::template index<TState>();	}

	/// @brief Get region identifier for a region type
	/// @tparam TState Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static constexpr RegionID regionId()				{ return (RegionID) RegionList::template index<TState>();	}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::context()
	HFSM_INLINE Context& _()									{ return _context;									}

	/// @brief Access FSM context (data shared between states and/or data interface between FSM and external code)
	/// @return context
	/// @see Control::_()
	HFSM_INLINE Context& context()								{ return _context;									}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is active
	/// @param stateId State identifier
	/// @return State active status
	HFSM_INLINE bool isActive   (const StateID id) const		{ return _registry.isActive   (id);					}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @param stateId State identifier
	/// @return State resumable status
	HFSM_INLINE bool isResumable(const StateID id) const		{ return _registry.isResumable(id);					}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @param stateId State identifier
	/// @return State scheduled status
	HFSM_INLINE bool isScheduled(const StateID id) const		{ return isResumable(id);							}

	/// @brief Check if a state is active
	/// @tparam TState State type
	/// @return State active status
	template <typename TState>
	HFSM_INLINE bool isActive() const							{ return isActive	(stateId<TState>());			}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @tparam TState State type
	/// @return State resumable status
	template <typename TState>
	HFSM_INLINE bool isResumable() const						{ return isResumable(stateId<TState>());			}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @tparam TState State type
	/// @return State scheduled status
	template <typename TState>
	HFSM_INLINE bool isScheduled() const						{ return isResumable(stateId<TState>());			}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Access read-only plan for the current region
	/// @return Plan for the current region
	HFSM_INLINE ConstPlan plan() const							{ return ConstPlan{_planData, _regionId};			}

	/// @brief Access read-only plan for a region
	/// @param regionId Region identifier
	/// @return Read-only plan for the region
	HFSM_INLINE ConstPlan plan(const RegionID regionId) const	{ return ConstPlan{_planData, regionId};			}

	/// @brief Access read-only plan for a region
	/// @tparam TRegion Region head state type
	/// @return Read-only plan for the region
	template <typename TRegion>
	HFSM_INLINE ConstPlan plan()								{ return ConstPlan{_planData, regionId<TRegion>()};	}

	/// @brief Access read-only plan for a region
	/// @tparam TRegion Region head state type
	/// @return Read-only Plan for the region
	template <typename TRegion>
	HFSM_INLINE ConstPlan plan() const							{ return ConstPlan{_planData, regionId<TRegion>()};	}

protected:
#ifdef HFSM_ENABLE_LOG_INTERFACE
	HFSM_INLINE Logger* logger()								{ return _logger;									}
#endif

protected:
	Context& _context;
	HFSM_IF_UTILITY_THEORY(RNG& _rng);
	Registry& _registry;
	PlanData& _planData;
	RegionID _regionId = 0;
	HFSM_IF_LOG_INTERFACE(Logger* _logger);
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

	using Args			= TArgs;

protected:
	using Control		= ControlT<Args>;

	using typename Control::StateList;
	using typename Control::RegionList;
	using typename Control::PlanData;
	using typename Control::ConstPlan;

	using Plan			= PlanT<Args>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Origin {
		HFSM_INLINE Origin(PlanControlT& control_,
						   const StateID id);

		HFSM_INLINE ~Origin();

		PlanControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		HFSM_INLINE Region(PlanControlT& control,
						   const RegionID id,
						   const StateID index,
						   const LongIndex size);

		HFSM_INLINE ~Region();

		PlanControlT& control;
		const RegionID prevId;
		const LongIndex prevIndex;
		const LongIndex prevSize;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Control::Control;

	HFSM_INLINE void setOrigin  (const StateID id);
	HFSM_INLINE void resetOrigin(const StateID id);

	HFSM_INLINE void setRegion  (const RegionID id, const StateID index, const LongIndex size);
	HFSM_INLINE void resetRegion(const RegionID id, const StateID index, const LongIndex size);

public:
	using Control::stateId;
	using Control::regionId;

	using Control::_;
	using Control::context;

	using Control::isActive;
	using Control::isResumable;
	using Control::isScheduled;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Access plan for the current region
	/// @return Plan for the current region
	HFSM_INLINE		 Plan plan()								{ return	  Plan{_planData, _regionId};			}

	/// @brief Access plan for the current region
	/// @return Plan for the current region
	HFSM_INLINE ConstPlan plan() const							{ return ConstPlan{_planData, _regionId};			}

	/// @brief Access plan for a region
	/// @param regionId
	/// @return Plan for the region
	HFSM_INLINE		 Plan plan(const RegionID regionId)			{ return	  Plan{_planData, regionId};			}

	/// @brief Access plan for a region
	/// @param regionId
	/// @return Plan for the region
	HFSM_INLINE ConstPlan plan(const RegionID regionId) const	{ return ConstPlan{_planData, regionId};			}

	/// @brief Access plan for a region
	/// @tparam TRegion Region head state type
	/// @return Plan for the region
	template <typename TRegion>
	HFSM_INLINE		 Plan plan()			{ return	  Plan{_planData, Control::template regionId<TRegion>()};	}

	/// @brief Access plan for a region
	/// @tparam TRegion Region head state type
	/// @return Plan for the region
	template <typename TRegion>
	HFSM_INLINE ConstPlan plan() const		{ return ConstPlan{_planData, Control::template regionId<TRegion>()};	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using Control::_planData;
	using Control::_regionId;
	HFSM_IF_LOG_INTERFACE(using Control::_logger);

	StateID _originId = 0;
	StateID _regionIndex = 0;
	LongIndex _regionSize = StateList::SIZE;
	Status _status;
};

//------------------------------------------------------------------------------

template <typename TArgs>
class FullControlT
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

	using Args			= TArgs;

protected:
	using PlanControl	= PlanControlT<Args>;

	using typename PlanControl::Context;

#ifdef HFSM_ENABLE_UTILITY_THEORY
	using typename PlanControl::RNG;
#endif

#ifdef HFSM_ENABLE_LOG_INTERFACE
	using typename PlanControl::Logger;
#endif

	using typename PlanControl::StateList;
	using typename PlanControl::RegionList;
	using typename PlanControl::PlanData;

	using typename PlanControl::Plan;
	using typename PlanControl::Origin;

	using Registry		= RegistryT<Args>;
	using Requests		= RequestsT<Args::COMPO_REGIONS>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Lock {
		HFSM_INLINE Lock(FullControlT& control_);
		HFSM_INLINE ~Lock();

		FullControlT* const control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE FullControlT(Context& context
							 HFSM_IF_UTILITY_THEORY(, RNG& rng)
							 , Registry& registry
							 , PlanData& planData
							 , Requests& requests
							 HFSM_IF_LOG_INTERFACE(, Logger* const logger))
		: PlanControl{context HFSM_IF_UTILITY_THEORY(,rng), registry, planData HFSM_IF_LOG_INTERFACE(, logger)}
		, _requests{requests}
	{}

	template <typename TState>
	Status updatePlan(TState& headState, const Status subStatus);

	template <typename TState>
	Status buildPlanStatus();

public:
	using PlanControl::stateId;
	using PlanControl::regionId;

	using PlanControl::_;
	using PlanControl::context;

	using PlanControl::isActive;
	using PlanControl::isResumable;
	using PlanControl::isScheduled;

	using PlanControl::plan;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId State identifier
	HFSM_INLINE void changeTo (const StateID stateId);

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param stateId State identifier
	HFSM_INLINE void restart  (const StateID stateId);

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @param stateId State identifier
	HFSM_INLINE void resume	  (const StateID stateId);

#ifdef HFSM_ENABLE_UTILITY_THEORY

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @param stateId State identifier
	/// @see HFSM_ENABLE_UTILITY_THEORY
	HFSM_INLINE void utilize  (const StateID stateId);

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @param stateId State identifier
	/// @see HFSM_ENABLE_UTILITY_THEORY
	HFSM_INLINE void randomize(const StateID stateId);

#endif

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId State identifier
	HFSM_INLINE void schedule (const StateID stateId);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clang trips over 'stateId<>()', so give it a hint it comes from PlanControl

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState State type
	template <typename TState>
	HFSM_INLINE void changeTo ()							{ changeTo (PlanControl::template stateId<TState>());	}

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState State type
	template <typename TState>
	HFSM_INLINE void restart  ()							{ restart  (PlanControl::template stateId<TState>());	}

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState State type
	template <typename TState>
	HFSM_INLINE void resume   ()							{ resume   (PlanControl::template stateId<TState>());	}

#ifdef HFSM_ENABLE_UTILITY_THEORY

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///   with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TState State type
	/// @see HFSM_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM_INLINE void utilize  ()							{ utilize  (PlanControl::template stateId<TState>());	}

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///   proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TState State type
	/// @see HFSM_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM_INLINE void randomize()							{ randomize(PlanControl::template stateId<TState>());	}

#endif

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState State type
	template <typename TState>
	HFSM_INLINE void schedule ()							{ schedule (PlanControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Succeed a plan task for the current state
	HFSM_INLINE void succeed();

	/// @brief Fail a plan task for the current state
	HFSM_INLINE void fail();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using PlanControl::_planData;
	using PlanControl::_regionId;
	HFSM_IF_LOG_INTERFACE(using PlanControl::_logger);

	using PlanControl::_originId;
	using PlanControl::_regionIndex;
	using PlanControl::_regionSize;
	using PlanControl::_status;

	Requests& _requests;
	bool _locked = false;
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

	using Args			= TArgs;
	using FullControl	= FullControlT<Args>;

	using typename FullControl::Context;

#ifdef HFSM_ENABLE_UTILITY_THEORY
	using typename FullControl::RNG;
#endif

#ifdef HFSM_ENABLE_LOG_INTERFACE
	using typename FullControl::Logger;
#endif

	using typename FullControl::StateList;
	using typename FullControl::RegionList;
	using typename FullControl::PlanData;

	using typename FullControl::Registry;

protected:
	using Requests		= RequestsT<Args::COMPO_REGIONS>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	HFSM_INLINE GuardControlT(Context& context
							  HFSM_IF_UTILITY_THEORY(, RNG& rng)
							  , Registry& registry
							  , PlanData& planData
							  , Requests& requests
							  , const Requests& pendingChanges
							  HFSM_IF_LOG_INTERFACE(, Logger* const logger))
		: FullControl{context HFSM_IF_UTILITY_THEORY(, rng), registry, planData, requests HFSM_IF_LOG_INTERFACE(, logger)}
		, _pending{pendingChanges}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	using FullControl::stateId;
	using FullControl::regionId;

	using FullControl::_;
	using FullControl::context;

	using FullControl::isActive;
	using FullControl::isResumable;
	using FullControl::isScheduled;

	using FullControl::plan;

	using FullControl::changeTo;
	using FullControl::restart;
	using FullControl::resume;

#ifdef HFSM_ENABLE_UTILITY_THEORY
	using FullControl::utilize;
	using FullControl::randomize;
#endif

	using FullControl::schedule;
	using FullControl::succeed;
	using FullControl::fail;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be activated or deactivated
	/// @param stateId State identifier
	/// @return State pending activation/deactivation status
	HFSM_INLINE bool isPendingChange(const StateID stateId) const	{ return _registry.isPendingChange(stateId);	}

	/// @brief Check if a state is going to be activated
	/// @param stateId State identifier
	/// @return State pending activation status
	HFSM_INLINE bool isPendingEnter	(const StateID stateId) const	{ return _registry.isPendingEnter (stateId);	}

	/// @brief Check if a state is going to be deactivated
	/// @param stateId State identifier
	/// @return State pending deactivation status
	HFSM_INLINE bool isPendingExit	(const StateID stateId) const	{ return _registry.isPendingExit  (stateId);	}

	/// @brief Check if a state is going to be activated or deactivated
	/// @tparam TState State type
	/// @return State pending activation/deactivation status
	template <typename TState>
	HFSM_INLINE bool isPendingChange()			{ return isPendingChange(FullControl::template stateId<TState>());	}

	/// @brief Check if a state is going to be activated
	/// @tparam TState State type
	/// @return State pending activation status
	template <typename TState>
	HFSM_INLINE bool isPendingEnter()			{ return isPendingEnter (FullControl::template stateId<TState>());	}

	/// @brief Check if a state is going to be deactivated
	/// @tparam TState State type
	/// @return State pending deactivation status
	template <typename TState>
	HFSM_INLINE bool isPendingExit()			{ return isPendingExit  (FullControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Cancel pending transition requests
	///		(can be used to substitute a transition into the current state with a different one)
	HFSM_INLINE void cancelPendingTransitions();

	/// @brief Get pending transition requests
	/// @return Array of pending transition requests
	HFSM_INLINE const Requests& pendingTransitions() const			{ return _pending;								}

private:
	HFSM_IF_LOG_INTERFACE(using FullControl::_logger);

	using FullControl::_registry;
	using FullControl::_originId;

	bool _cancelled = false;
	const Requests& _pending;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "control.inl"
