#pragma once

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class ControlT {
	template <StateID, typename, typename>
	friend struct _S;

	template <StateID, ShortIndex, ShortIndex, typename, typename, typename...>
	friend struct _C;

	template <StateID, ShortIndex, ShortIndex, typename, typename, typename...>
	friend struct _O;

	template <typename, typename, typename, typename>
	friend class _R;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

public:
	using StateRegistry	= StateRegistryT<Args>;
	using PlanData		= PlanDataT <Args>;
	using Plan			= PlanT		<Args>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Origin {
		HFSM_INLINE Origin(ControlT& control_,
						   const StateID id);

		HFSM_INLINE ~Origin();

		ControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		HFSM_INLINE Region(ControlT& control,
						   const RegionID id,
						   const StateID index,
						   const LongIndex size);

		HFSM_INLINE ~Region();

		ControlT& control;
		const RegionID prevId;
		const LongIndex prevIndex;
		const LongIndex prevSize;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	HFSM_INLINE ControlT(Context& context,
						 StateRegistry& stateRegistry,
						 PlanData& planData,
						 LoggerInterface* const HFSM_IF_LOGGER(logger))
		: _context(context)
		, _stateRegistry(stateRegistry)
		, _planData(planData)
		HFSM_IF_LOGGER(, _logger(logger))
	{}

	HFSM_INLINE void setOrigin  (const StateID id);
	HFSM_INLINE void resetOrigin(const StateID id);

	HFSM_INLINE void setRegion  (const RegionID id, const StateID index, const LongIndex size);
	HFSM_INLINE void resetRegion(const RegionID id, const StateID index, const LongIndex size);

	template <typename T>
	static constexpr LongIndex stateId()						{ return StateList ::template index<T>();		}

	template <typename T>
	static constexpr RegionID regionId()						{ return RegionList::template index<T>();		}

public:
	HFSM_INLINE Context& _()									{ return _context;								}
	HFSM_INLINE Context& context()								{ return _context;								}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool isActive   (const StateID stateId) const	{ return _stateRegistry.isActive   (stateId);	}
	HFSM_INLINE bool isResumable(const StateID stateId) const	{ return _stateRegistry.isResumable(stateId);	}

	HFSM_INLINE bool isScheduled(const StateID stateId) const	{ return isResumable(stateId);					}

	template <typename TState>
	HFSM_INLINE bool isActive() const							{ return isActive	(stateId<TState>());		}

	template <typename TState>
	HFSM_INLINE bool isResumable() const						{ return isResumable(stateId<TState>());		}

	template <typename TState>
	HFSM_INLINE bool isScheduled() const						{ return isResumable(stateId<TState>());		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE Plan plan()										{ return Plan{_planData, _regionId};			}
	HFSM_INLINE Plan plan() const								{ return Plan{_planData, _regionId};			}

	HFSM_INLINE Plan plan(const RegionID regionId)				{ return Plan{_planData,  regionId};			}
	HFSM_INLINE Plan plan(const RegionID regionId) const		{ return Plan{_planData,  regionId};			}

	template <typename TRegion>
	HFSM_INLINE Plan plan()										{ return Plan{_planData, regionId<TRegion>()};	}

	template <typename TRegion>
	HFSM_INLINE Plan plan() const								{ return Plan{_planData, regionId<TRegion>()};	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	HFSM_INLINE		  StateRegistry&	stateRegistry()			{ return _stateRegistry;						}
	HFSM_INLINE const StateRegistry&	stateRegistry() const	{ return _stateRegistry;						}

	HFSM_INLINE		  PlanData&	planData()						{ return _planData;								}
	HFSM_INLINE const PlanData&	planData() const				{ return _planData;								}

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	HFSM_INLINE LoggerInterface* logger()						{ return _logger;								}
#endif

protected:
	Context& _context;
	StateRegistry& _stateRegistry;
	PlanData& _planData;
	StateID _originId = INVALID_STATE_ID;
	RegionID _regionId = INVALID_REGION_ID;
	StateID _regionIndex = INVALID_STATE_ID;
	LongIndex _regionSize = INVALID_LONG_INDEX;
	Status _status;
	HFSM_IF_LOGGER(LoggerInterface* _logger);
};

//------------------------------------------------------------------------------

template <typename TArgs>
class FullControlT
	: public ControlT<TArgs>
{
	template <StateID, typename, typename>
	friend struct _S;

	template <StateID, ShortIndex, ShortIndex, typename, typename, typename...>
	friend struct _C;

	template <StateID, ShortIndex, ShortIndex, typename, typename, typename...>
	friend struct _O;

	template <typename, typename, typename, typename>
	friend class _R;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using PayloadList	= typename Args::PayloadList;

	using Control		= ControlT<Args>;
	using Origin		= typename Control::Origin;

	using StateRegistry	= StateRegistryT<Args>;
	using PlanData		= PlanDataT		<Args>;
	using Plan			= PlanT			<Args>;

	using Request		= RequestT <PayloadList>;
	using Requests		= RequestsT<PayloadList, Args::COMPO_COUNT>;

protected:
	using Control::_planData;
	using Control::_originId;
	using Control::_regionId;
	using Control::_regionIndex;
	using Control::_regionSize;
	using Control::_status;
	HFSM_IF_LOGGER(using Control::_logger);

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
							 LoggerInterface* const logger)
		: Control{context, stateRegistry, planData, logger}
		, _requests(requests)
	{}

	template <typename T>
	static constexpr LongIndex stateId()	{ return StateList ::template index<T>();	}

	template <typename T>
	static constexpr LongIndex regionId()	{ return RegionList::template index<T>();	}

	template <typename TState>
	Status updatePlan(TState& headState, const Status subStatus);

	template <typename TState>
	Status buildPlanStatus(const bool outerTransition);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	using Control::isActive;
	using Control::isResumable;
	using Control::isScheduled;
	using Control::plan;

	HFSM_INLINE void changeTo(const StateID stateId);
	HFSM_INLINE void resume	 (const StateID stateId);
	HFSM_INLINE void schedule(const StateID stateId);

	template <typename TState>
	HFSM_INLINE void changeTo()					{ changeTo(stateId<TState>());	}

	template <typename TState>
	HFSM_INLINE void resume()					{ resume  (stateId<TState>());	}

	template <typename TState>
	HFSM_INLINE void schedule()					{ schedule(stateId<TState>());	}

	HFSM_INLINE void succeed();
	HFSM_INLINE void fail();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	Requests& _requests;
	bool _locked = false;
};

//------------------------------------------------------------------------------

template <typename TArgs>
class GuardControlT
	: public FullControlT<TArgs>
{
	template <StateID, typename, typename>
	friend struct _S;

	template <typename, typename, typename, typename>
	friend class _R;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using PayloadList	= typename Args::PayloadList;

	using Control		= ControlT	  <Args>;
	using FullControl	= FullControlT<Args>;

	using StateRegistry	= StateRegistryT<Args>;
	using PlanData		= PlanDataT		<Args>;

public:
	using Requests		= RequestsT<PayloadList, Args::COMPO_COUNT>;

private:
	using FullControl::_stateRegistry;
	using FullControl::_originId;
	HFSM_IF_LOGGER(using Control::_logger);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	HFSM_INLINE GuardControlT(Context& context,
							  StateRegistry& stateRegistry,
							  PlanData& planData,
							  Requests& requests,
							  const Requests& pendingChanges,
							  LoggerInterface* const logger)
		: FullControl{context, stateRegistry, planData, requests, logger}
		, _pending(pendingChanges)
	{}

	template <typename T>
	static constexpr LongIndex stateId()							{ return StateList::template index<T>();			}

	HFSM_INLINE bool cancelled() const								{ return _cancelled;								}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	using Control::isActive;
	using Control::isResumable;
	using Control::isScheduled;
	using Control::plan;

	HFSM_INLINE bool isPendingChange(const StateID stateId) const	{ return _stateRegistry.isPendingChange(stateId);	}
	HFSM_INLINE bool isPendingEnter	(const StateID stateId) const	{ return _stateRegistry.isPendingEnter (stateId);	}
	HFSM_INLINE bool isPendingExit	(const StateID stateId) const	{ return _stateRegistry.isPendingExit  (stateId);	}

	template <typename TState>
	HFSM_INLINE bool isPendingChange()								{ return isPendingChange(stateId<TState>());		}

	template <typename TState>
	HFSM_INLINE bool isPendingEnter()								{ return isPendingEnter (stateId<TState>());		}

	template <typename TState>
	HFSM_INLINE bool isPendingExit()								{ return isPendingExit  (stateId<TState>());		}

	HFSM_INLINE void cancelPendingChanges();

	HFSM_INLINE const Requests& pendingChanges() const				{ return _pending;									}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	bool _cancelled = false;
	const Requests& _pending;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "control.inl"
