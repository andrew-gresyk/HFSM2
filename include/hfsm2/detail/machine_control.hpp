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
		HSFM_INLINE Origin(ControlT& control_,
						   const StateID id);

		HSFM_INLINE ~Origin();

		ControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		HSFM_INLINE Region(ControlT& control,
						   const RegionID id,
						   const StateID index,
						   const LongIndex size);

		HSFM_INLINE ~Region();

		ControlT& control;
		const RegionID prevId;
		const LongIndex prevIndex;
		const LongIndex prevSize;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	HSFM_INLINE ControlT(Context& context,
						 StateData& stateData,
						 PlanData& planData,
						 LoggerInterface* const HFSM_IF_LOGGER(logger))
		: _context(context)
		, _stateData(stateData)
		, _planData(planData)
		HFSM_IF_LOGGER(, _logger(logger))
	{}

	HSFM_INLINE void setOrigin  (const StateID id);
	HSFM_INLINE void resetOrigin(const StateID id);

	HSFM_INLINE void setRegion  (const RegionID id, const StateID index, const LongIndex size);
	HSFM_INLINE void resetRegion(const RegionID id, const StateID index, const LongIndex size);

	template <typename T>
	static constexpr LongIndex stateId()						{ return StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()						{ return RegionList::template index<T>();	}

public:
	HSFM_INLINE Context& _()									{ return _context;							}
	HSFM_INLINE Context& context()								{ return _context;							}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HSFM_INLINE bool isActive   (const StateID stateId) const	{ return _stateData.isActive   (stateId);	}
	HSFM_INLINE bool isResumable(const StateID stateId) const	{ return _stateData.isResumable(stateId);	}

	HSFM_INLINE bool isScheduled(const StateID stateId) const	{ return isResumable(stateId);				}

	template <typename TState>
	HSFM_INLINE bool isActive() const							{ return isActive	(stateId<TState>());	}

	template <typename TState>
	HSFM_INLINE bool isResumable() const						{ return isResumable(stateId<TState>());	}

	template <typename TState>
	HSFM_INLINE bool isScheduled() const						{ return isResumable(stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HSFM_INLINE Plan plan()										{ return Plan{_planData, _regionId};		}
	HSFM_INLINE Plan plan() const								{ return Plan{_planData, _regionId};		}

	HSFM_INLINE Plan plan(const RegionID regionId)				{ return Plan{_planData,  regionId};		}
	HSFM_INLINE Plan plan(const RegionID regionId) const		{ return Plan{_planData,  regionId};		}

	template <typename TRegion>
	HSFM_INLINE Plan plan()									{ return Plan{_planData, regionId<TRegion>()};	}

	template <typename TRegion>
	HSFM_INLINE Plan plan() const							{ return Plan{_planData, regionId<TRegion>()};	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	HSFM_INLINE		 StateData&	stateData()						{ return _stateData;						}
	HSFM_INLINE const StateData&	stateData() const			{ return _stateData;						}

	HSFM_INLINE		 PlanData&	planData()						{ return _planData;							}
	HSFM_INLINE const PlanData&	planData() const				{ return _planData;							}

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	HSFM_INLINE LoggerInterface* logger()						{ return _logger;							}
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
		HSFM_INLINE Lock(FullControlT& control_);
		HSFM_INLINE ~Lock();

		FullControlT* const control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	HSFM_INLINE FullControlT(Context& context,
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

	HSFM_INLINE void changeTo(const StateID stateId);
	HSFM_INLINE void resume	 (const StateID stateId);
	HSFM_INLINE void schedule(const StateID stateId);

	template <typename TState>
	HSFM_INLINE void changeTo()				{ changeTo(stateId<TState>());				}

	template <typename TState>
	HSFM_INLINE void resume()				{ resume  (stateId<TState>());				}

	template <typename TState>
	HSFM_INLINE void schedule()				{ schedule(stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HSFM_INLINE void succeed();
	HSFM_INLINE void fail();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	Requests& _requests;
	bool _locked = false;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_control.inl"
