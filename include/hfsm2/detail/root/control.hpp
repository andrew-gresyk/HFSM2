#pragma once

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
	using RNG			= typename Args::RNG;
	using Logger		= typename Args::Logger;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using StateRegistry	= StateRegistryT<Args>;
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

	HFSM_INLINE ControlT(Context& context,
						 RNG& rng,
						 StateRegistry& stateRegistry,
						 PlanData& planData,
						 Logger* const HFSM_IF_LOGGER(logger))
		: _context{context}
		, _rng{rng}
		, _stateRegistry{stateRegistry}
		, _planData{planData}
		HFSM_IF_LOGGER(, _logger{logger})
	{}


	HFSM_INLINE void setRegion(const RegionID id);
	HFSM_INLINE void resetRegion(const RegionID id);

public:
	template <typename T>
	static constexpr StateID  stateId()					{ return			StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()				{ return (RegionID) RegionList::template index<T>();	}

	HFSM_INLINE Context& _()								{ return _context;									}
	HFSM_INLINE Context& context()							{ return _context;									}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool isActive   (const StateID id) const	{ return _stateRegistry.isActive   (id);			}
	HFSM_INLINE bool isResumable(const StateID id) const	{ return _stateRegistry.isResumable(id);			}

	HFSM_INLINE bool isScheduled(const StateID id) const	{ return isResumable(id);							}

	template <typename TState>
	HFSM_INLINE bool isActive() const						{ return isActive	(stateId<TState>());			}

	template <typename TState>
	HFSM_INLINE bool isResumable() const					{ return isResumable(stateId<TState>());			}

	template <typename TState>
	HFSM_INLINE bool isScheduled() const					{ return isResumable(stateId<TState>());			}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE ConstPlan plan() const						{ return ConstPlan{_planData, _regionId};			}
	HFSM_INLINE ConstPlan plan(const RegionID id) const		{ return ConstPlan{_planData, id};					}

	template <typename TRegion>
	HFSM_INLINE ConstPlan plan()							{ return ConstPlan{_planData, regionId<TRegion>()};	}

	template <typename TRegion>
	HFSM_INLINE ConstPlan plan() const						{ return ConstPlan{_planData, regionId<TRegion>()};	}

protected:
#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_ENABLE_VERBOSE_DEBUG_LOG
	HFSM_INLINE Logger* logger()							{ return _logger;									}
#endif

protected:
	Context& _context;
	RNG& _rng;
	StateRegistry& _stateRegistry;
	PlanData& _planData;
	RegionID _regionId = 0;
	HFSM_IF_LOGGER(Logger* _logger);
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

	HFSM_INLINE		 Plan plan()							{ return	  Plan{_planData, _regionId};			}
	HFSM_INLINE ConstPlan plan() const						{ return ConstPlan{_planData, _regionId};			}

	HFSM_INLINE		 Plan plan(const RegionID id)			{ return	  Plan{_planData, id};					}
	HFSM_INLINE ConstPlan plan(const RegionID id) const		{ return ConstPlan{_planData, id};					}

	template <typename TRegion>
	HFSM_INLINE		 Plan plan()		{ return	  Plan{_planData, Control::template regionId<TRegion>()};	}

	template <typename TRegion>
	HFSM_INLINE ConstPlan plan() const	{ return ConstPlan{_planData, Control::template regionId<TRegion>()};	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using Control::_planData;
	using Control::_regionId;
	HFSM_IF_LOGGER(using Control::_logger);

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
	using typename PlanControl::RNG;
	using typename PlanControl::Logger;
	using typename PlanControl::StateList;
	using typename PlanControl::RegionList;
	using typename PlanControl::PlanData;

	using typename PlanControl::Plan;
	using typename PlanControl::Origin;

	using StateRegistry	= StateRegistryT<Args>;
	using Requests		= RequestsT<Args::COMPO_REGIONS>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Lock {
		HFSM_INLINE Lock(FullControlT& control_);
		HFSM_INLINE ~Lock();

		FullControlT* const control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE FullControlT(Context& context,
							 RNG& rng,
							 StateRegistry& stateRegistry,
							 PlanData& planData,
							 Requests& requests,
							 Logger* const logger)
		: PlanControl{context, rng, stateRegistry, planData, logger}
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
	// TODO: remove payload versions

	HFSM_INLINE void changeTo (const StateID id);
	HFSM_INLINE void restart  (const StateID id);
	HFSM_INLINE void resume	  (const StateID id);
	HFSM_INLINE void utilize  (const StateID id);
	HFSM_INLINE void randomize(const StateID id);
	HFSM_INLINE void schedule (const StateID id);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clang trips over 'stateId<>()', so give it a hint it comes from ControlT<>

	template <typename T>
	HFSM_INLINE void changeTo ()							{ changeTo (PlanControl::template stateId<T>());	}

	template <typename T>
	HFSM_INLINE void restart  ()							{ restart  (PlanControl::template stateId<T>());	}

	template <typename T>
	HFSM_INLINE void resume   ()							{ resume   (PlanControl::template stateId<T>());	}

	template <typename T>
	HFSM_INLINE void utilize  ()							{ utilize  (PlanControl::template stateId<T>());	}

	template <typename T>
	HFSM_INLINE void randomize()							{ randomize(PlanControl::template stateId<T>());	}

	template <typename T>
	HFSM_INLINE void schedule ()							{ schedule (PlanControl::template stateId<T>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void succeed();
	HFSM_INLINE void fail();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using PlanControl::_planData;
	using PlanControl::_regionId;
	HFSM_IF_LOGGER(using PlanControl::_logger);

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
	using typename FullControl::RNG;
	using typename FullControl::Logger;
	using typename FullControl::StateList;
	using typename FullControl::RegionList;
	using typename FullControl::PlanData;

	using typename FullControl::StateRegistry;

protected:
	using Requests		= RequestsT<Args::COMPO_REGIONS>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	HFSM_INLINE GuardControlT(Context& context,
							  RNG& rng,
							  StateRegistry& stateRegistry,
							  PlanData& planData,
							  Requests& requests,
							  const Requests& pendingChanges,
							  Logger* const logger)
		: FullControl{context, rng, stateRegistry, planData, requests, logger}
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
	using FullControl::utilize;
	using FullControl::randomize;
	using FullControl::schedule;
	using FullControl::succeed;
	using FullControl::fail;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool isPendingChange(const StateID id) const	{ return _stateRegistry.isPendingChange(id);	}
	HFSM_INLINE bool isPendingEnter	(const StateID id) const	{ return _stateRegistry.isPendingEnter (id);	}
	HFSM_INLINE bool isPendingExit	(const StateID id) const	{ return _stateRegistry.isPendingExit  (id);	}

	template <typename TState>
	HFSM_INLINE bool isPendingChange()		{ return isPendingChange(FullControl::template stateId<TState>());	}

	template <typename TState>
	HFSM_INLINE bool isPendingEnter()		{ return isPendingEnter (FullControl::template stateId<TState>());	}

	template <typename TState>
	HFSM_INLINE bool isPendingExit()		{ return isPendingExit  (FullControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void cancelPendingTransitions();

	HFSM_INLINE const Requests& pendingTransitions() const		{ return _pending;								}

private:
	HFSM_IF_LOGGER(using FullControl::_logger);

	using FullControl::_stateRegistry;
	using FullControl::_originId;

	bool _cancelled = false;
	const Requests& _pending;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "control.inl"
