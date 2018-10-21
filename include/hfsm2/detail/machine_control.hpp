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
	using StateData		= StateDataT<Args>;
	using PlanData		= PlanDataT <Args>;

	using Plan			= PlanT		<Args>;
	using TaskLinks		= typename PlanData::TaskLinks;
	using TasksBounds	= typename PlanData::TasksBounds;

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
						 StateData& stateData,
						 PlanData& planData,
						 LoggerInterface* const HFSM_IF_LOGGER(logger))
		: _context(context)
		, _stateData(stateData)
		, _planData(planData)
		HFSM_IF_LOGGER(, _logger(logger))
	{}

	HFSM_INLINE void setOrigin  (const StateID id);
	HFSM_INLINE void resetOrigin(const StateID id);

	HFSM_INLINE void setRegion  (const RegionID id, const StateID index, const LongIndex size);
	HFSM_INLINE void resetRegion(const RegionID id, const StateID index, const LongIndex size);

	template <typename T>
	static constexpr LongIndex stateId()						{ return StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()						{ return RegionList::template index<T>();	}

public:
	HFSM_INLINE Context& _()									{ return _context;							}
	HFSM_INLINE Context& context()								{ return _context;							}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool isActive   (const StateID stateId) const	{ return _stateData.isActive   (stateId);	}
	HFSM_INLINE bool isResumable(const StateID stateId) const	{ return _stateData.isResumable(stateId);	}

	HFSM_INLINE bool isScheduled(const StateID stateId) const	{ return isResumable(stateId);				}

	template <typename TState>
	HFSM_INLINE bool isActive() const							{ return isActive	(stateId<TState>());	}

	template <typename TState>
	HFSM_INLINE bool isResumable() const						{ return isResumable(stateId<TState>());	}

	template <typename TState>
	HFSM_INLINE bool isScheduled() const						{ return isResumable(stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE Plan plan()										{ return Plan{_planData, _regionId};		}
	HFSM_INLINE Plan plan() const								{ return Plan{_planData, _regionId};		}

	HFSM_INLINE Plan plan(const RegionID regionId)				{ return Plan{_planData,  regionId};		}
	HFSM_INLINE Plan plan(const RegionID regionId) const		{ return Plan{_planData,  regionId};		}

	template <typename TRegion>
	HFSM_INLINE Plan plan()									{ return Plan{_planData, regionId<TRegion>()};	}

	template <typename TRegion>
	HFSM_INLINE Plan plan() const							{ return Plan{_planData, regionId<TRegion>()};	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	HFSM_INLINE		 StateData&	stateData()						{ return _stateData;						}
	HFSM_INLINE const StateData&	stateData() const			{ return _stateData;						}

	HFSM_INLINE		 PlanData&	planData()						{ return _planData;							}
	HFSM_INLINE const PlanData&	planData() const				{ return _planData;							}

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	HFSM_INLINE LoggerInterface* logger()						{ return _logger;							}
#endif

protected:
	Context& _context;
	StateData& _stateData;
	PlanData& _planData;
	StateID _originId = INVALID_STATE_ID;
	RegionID _regionId = INVALID_REGION_ID;
	StateID _regionIndex = INVALID_STATE_ID;
	LongIndex _regionSize = INVALID_LONG_INDEX;
	Status _status;
	HFSM_IF_LOGGER(LoggerInterface* _logger);
};

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class FullControlT final
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

	using StateData		= StateDataT<Args>;
	using Plan			= PlanT		<Args>;
	using PlanData		= PlanDataT <Args>;
	using TaskLinks		= typename PlanData::TaskLinks;
	using TasksBounds	= typename PlanData::TasksBounds;

public:
	using Request		= RequestT <PayloadList>;
	using Requests		= RequestsT<PayloadList, Args::COMPO_COUNT>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Lock {
		HFSM_INLINE Lock(FullControlT& control_);
		HFSM_INLINE ~Lock();

		FullControlT* const control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	HFSM_INLINE FullControlT(Context& context,
							 StateData& stateData,
							 PlanData& planData,
							 Requests& requests,
							 LoggerInterface* const logger)
		: Control{context, stateData, planData, logger}
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

public:
	using Control::isActive;
	using Control::isResumable;
	using Control::isScheduled;
	using Control::plan;

	using Control::_planData;
	using Control::_originId;
	using Control::_regionId;
	using Control::_regionIndex;
	using Control::_regionSize;
	using Control::_status;
	HFSM_IF_LOGGER(using Control::_logger);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void changeTo(const StateID stateId);
	HFSM_INLINE void resume	 (const StateID stateId);
	HFSM_INLINE void schedule(const StateID stateId);

	template <typename TState>
	HFSM_INLINE void changeTo()				{ changeTo(stateId<TState>());				}

	template <typename TState>
	HFSM_INLINE void resume()				{ resume  (stateId<TState>());				}

	template <typename TState>
	HFSM_INLINE void schedule()				{ schedule(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void succeed();
	HFSM_INLINE void fail();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	Requests& _requests;
	bool _locked = false;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_control.inl"
