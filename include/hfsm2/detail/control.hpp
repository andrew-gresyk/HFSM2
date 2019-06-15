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
	friend class _R;

	using Args			= TArgs;
	using Logger		= typename Args::Logger;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

public:
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

protected:
	HFSM_INLINE ControlT(Context& context,
						 StateRegistry& stateRegistry,
						 PlanData& planData,
						 Logger* const HFSM_IF_LOGGER(logger))
		: _context{context}
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

	HFSM_INLINE Context& _()										{ return _context;							}
	HFSM_INLINE Context& context()									{ return _context;							}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool isActive   (const StateID id) const			{ return _stateRegistry.isActive   (id);	}
	HFSM_INLINE bool isResumable(const StateID id) const			{ return _stateRegistry.isResumable(id);	}

	HFSM_INLINE bool isScheduled(const StateID id) const			{ return isResumable(id);					}

	template <typename TState>
	HFSM_INLINE bool isActive() const								{ return isActive	(stateId<TState>());	}

	template <typename TState>
	HFSM_INLINE bool isResumable() const							{ return isResumable(stateId<TState>());	}

	template <typename TState>
	HFSM_INLINE bool isScheduled() const							{ return isResumable(stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE const ConstPlan plan() const						{ return ConstPlan{_planData, _regionId};	}
	HFSM_INLINE const ConstPlan plan(const RegionID id) const		{ return ConstPlan{_planData, id};			}

protected:
#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_ENABLE_VERBOSE_DEBUG_LOG
	HFSM_INLINE Logger* logger()									{ return _logger;							}
#endif

protected:
	Context& _context;
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
	friend class _R;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using Control		= ControlT<Args>;
	using StateRegistry	= StateRegistryT<Args>;

public:
	using PlanData		= PlanDataT<Args>;
	using Plan			= PlanT<Args>;
	using ConstPlan		= ConstPlanT<Args>;

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

protected:
	using Control::Control;

	HFSM_INLINE void setOrigin  (const StateID id);
	HFSM_INLINE void resetOrigin(const StateID id);

	HFSM_INLINE void setRegion  (const RegionID id, const StateID index, const LongIndex size);
	HFSM_INLINE void resetRegion(const RegionID id, const StateID index, const LongIndex size);

public:
	using Control::isActive;
	using Control::isResumable;
	using Control::isScheduled;

	using Control::plan;

	using Control::_;
	using Control::context;

	template <typename T>
	static constexpr StateID  stateId()					{ return			StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()				{ return (RegionID) RegionList::template index<T>();	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE Plan plan()										{ return Plan{_planData, _regionId};			}
	HFSM_INLINE Plan plan(const RegionID id)					{ return Plan{_planData, id};					}

	template <typename TRegion>
	HFSM_INLINE Plan plan()										{ return Plan{_planData, regionId<TRegion>()};	}

	template <typename TRegion>
	HFSM_INLINE Plan plan() const								{ return Plan{_planData, regionId<TRegion>()};	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	HFSM_INLINE		  PlanData&	planData()						{ return _planData;								}
	HFSM_INLINE const PlanData&	planData() const				{ return _planData;								}

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
	friend class _R;

	using Args			= TArgs;
	using Logger		= typename Args::Logger;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using Payload		= typename Args::Payload;

	using Control		= ControlT<Args>;
	using StateRegistry	= StateRegistryT<Args>;

	using PlanControl	= PlanControlT<Args>;
	using Origin		= typename PlanControl::Origin;
	using Plan			= typename PlanControl::Plan;
	using PlanData		= PlanDataT<Args>;

	using Request		= RequestT <Payload>;
	using Requests		= RequestsT<Payload, Args::COMPO_REGIONS>;

protected:

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Lock {
		HFSM_INLINE Lock(FullControlT& control_);
		HFSM_INLINE ~Lock();

		FullControlT* const control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	HFSM_INLINE FullControlT(Context& context,
							 StateRegistry& stateRegistry,
							 PlanData& planData,
							 Requests& requests,
							 Logger* const logger)
		: PlanControl{context, stateRegistry, planData, logger}
		, _requests{requests}
	{}

	template <typename TState>
	Status updatePlan(TState& headState, const Status subStatus);

	template <typename TState>
	Status buildPlanStatus(const bool outerTransition);

public:
	using Control::isActive;
	using Control::isResumable;
	using Control::isScheduled;

	using PlanControl::plan;

	using Control::_;
	using Control::context;

	template <typename T>
	static constexpr StateID  stateId()					{ return			StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()				{ return (RegionID) RegionList::template index<T>();	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void changeTo(const StateID id);
	HFSM_INLINE void changeTo(const StateID stateId, const Payload& payload);

	HFSM_INLINE void restart (const StateID id);
	HFSM_INLINE void restart (const StateID stateId, const Payload& payload);

	HFSM_INLINE void resume	 (const StateID id);
	HFSM_INLINE void resume  (const StateID stateId, const Payload& payload);

	HFSM_INLINE void utilize (const StateID id);
	HFSM_INLINE void utilize (const StateID stateId, const Payload& payload);

	HFSM_INLINE void schedule(const StateID id);
	HFSM_INLINE void schedule(const StateID stateId, const Payload& payload);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TState>
	HFSM_INLINE void changeTo()									{ changeTo(stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void changeTo(const Payload& payload)			{ changeTo(stateId<TState>(), payload);			}

	template <typename TState>
	HFSM_INLINE void restart()									{ restart (stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void restart (const Payload& payload)			{ restart (stateId<TState>(), payload);			}

	template <typename TState>
	HFSM_INLINE void resume()									{ resume  (stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void resume	 (const Payload& payload)			{ resume  (stateId<TState>(), payload);			}

	template <typename TState>
	HFSM_INLINE void utilize()									{ utilize (stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void utilize (const Payload& payload)			{ utilize (stateId<TState>(), payload);			}

	template <typename TState>
	HFSM_INLINE void schedule()									{ schedule(stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void schedule(const Payload& payload)			{ schedule(stateId<TState>(), payload);			}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void succeed();
	HFSM_INLINE void fail();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using PlanControl::_planData;
	using PlanControl::_originId;
	using PlanControl::_regionId;
	using PlanControl::_regionIndex;
	using PlanControl::_regionSize;
	using PlanControl::_status;
	HFSM_IF_LOGGER(using Control::_logger);

	Requests& _requests;
	bool _locked = false;
};

//------------------------------------------------------------------------------

template <typename TArgs>
class GuardControlT
	: public FullControlT<TArgs>
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename>
	friend class _R;

	using Args			= TArgs;
	using Logger		= typename Args::Logger;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using Payload		= typename Args::Payload;

	using Control		= ControlT<Args>;
	using StateRegistry	= StateRegistryT<Args>;

	using PlanControl	= PlanControlT<Args>;
	using PlanData		= PlanDataT<Args>;

	using FullControl	= FullControlT<Args>;

public:
	using Requests		= RequestsT<Payload, Args::COMPO_REGIONS>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	HFSM_INLINE GuardControlT(Context& context,
							  StateRegistry& stateRegistry,
							  PlanData& planData,
							  Requests& requests,
							  const Requests& pendingChanges,
							  Logger* const logger)
		: FullControl{context, stateRegistry, planData, requests, logger}
		, _pending{pendingChanges}
	{}

	HFSM_INLINE bool cancelled() const							{ return _cancelled;							}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	using Control::isActive;
	using Control::isResumable;
	using Control::isScheduled;

	using PlanControl::plan;

	using Control::_;
	using Control::context;

	template <typename T>
	static constexpr StateID  stateId()					{ return			StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()				{ return (RegionID) RegionList::template index<T>();	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool isPendingChange(const StateID id) const	{ return _stateRegistry.isPendingChange(id);	}
	HFSM_INLINE bool isPendingEnter	(const StateID id) const	{ return _stateRegistry.isPendingEnter (id);	}
	HFSM_INLINE bool isPendingExit	(const StateID id) const	{ return _stateRegistry.isPendingExit  (id);	}

	template <typename TState>
	HFSM_INLINE bool isPendingChange()							{ return isPendingChange(stateId<TState>());	}

	template <typename TState>
	HFSM_INLINE bool isPendingEnter()							{ return isPendingEnter (stateId<TState>());	}

	template <typename TState>
	HFSM_INLINE bool isPendingExit()							{ return isPendingExit  (stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void cancelPendingTransitions();

	HFSM_INLINE const Requests& pendingTransitions() const		{ return _pending;								}

private:
	using FullControl::_stateRegistry;
	using FullControl::_originId;
	HFSM_IF_LOGGER(using FullControl::_logger);

	bool _cancelled = false;
	const Requests& _pending;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "control.inl"
