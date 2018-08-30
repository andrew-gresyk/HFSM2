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
	friend struct _Q;

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

	using Plan			= PlanT<StateList, RegionList::SIZE, Args::TASK_CAPACITY>;
	using TaskLinks		= typename Plan::TaskLinks;
	using TasksBounds	= typename Plan::TasksBounds;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Origin {
		inline Origin(ControlT& control_,
					  const StateID id);

		inline ~Origin();

		ControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		inline Region(ControlT& control,
					  const RegionID id,
					  const StateID index,
					  const LongIndex size);

		inline ~Region();

		ControlT& control;
		const RegionID prevId;
		const LongIndex prevIndex;
		const LongIndex prevSize;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	inline ControlT(Context& context,
					StateData& stateData,
					TaskLinks& taskLinks,
					TasksBounds& tasksBounds,
					LoggerInterface* const HFSM_IF_LOGGER(logger))
		: _context(context)
		, _stateData(stateData)
		, _taskLinks(taskLinks)
		, _tasksBounds(tasksBounds)
		HFSM_IF_LOGGER(, _logger(logger))
	{}

	inline void setOrigin  (const StateID id);
	inline void resetOrigin(const StateID id);

	inline void setRegion  (const RegionID id, const StateID index, const LongIndex size);
	inline void resetRegion(const RegionID id, const StateID index, const LongIndex size);

	template <typename T>
	static constexpr LongIndex stateId()					{ return StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()					{ return RegionList::template index<T>();	}

public:
	inline Context& _()										{ return _context;							}
	inline Context& context()								{ return _context;							}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	inline bool isActive   (const StateID stateId) const	{ return _stateData.isActive	(stateId);		}
	inline bool isResumable(const StateID stateId) const	{ return _stateData.isResumable(stateId);	}

	inline bool isScheduled(const StateID stateId) const	{ return isResumable(stateId);				}

	template <typename TState>
	inline bool isActive() const							{ return isActive	(stateId<TState>());	}

	template <typename TState>
	inline bool isResumable() const							{ return isResumable(stateId<TState>());	}

	template <typename TState>
	inline bool isScheduled() const							{ return isResumable(stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	inline Plan plan()									{ return Plan{_taskLinks, _tasksBounds, _regionId};	}
	inline Plan plan() const							{ return Plan{_taskLinks, _tasksBounds, _regionId};	}

	inline Plan plan(const RegionID regionId)			{ return Plan{_taskLinks, _tasksBounds,  regionId};	}
	inline Plan plan(const RegionID regionId) const		{ return Plan{_taskLinks, _tasksBounds,  regionId};	}

	template <typename TRegion>
	inline Plan plan()						{ return Plan{_taskLinks, _tasksBounds, regionId<TRegion>()};	}

	template <typename TRegion>
	inline Plan plan() const				{ return Plan{_taskLinks, _tasksBounds, regionId<TRegion>()};	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	inline		 StateData& stateData()						{ return _stateData;							}
	inline const StateData& stateData() const					{ return _stateData;							}

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	inline LoggerInterface* logger()						{ return _logger;							}
#endif

protected:
	Context& _context;
	StateData& _stateData;
	TaskLinks& _taskLinks;
	TasksBounds& _tasksBounds;
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

	template <typename, typename, typename, typename>
	friend class _R;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using PayloadList	= typename Args::PayloadList;

	using Control		= ControlT  <Args>;
	using StateData		= StateDataT<Args>;

	using Plan			= typename Control::Plan;
	using TaskLinks		= typename Plan::TaskLinks;
	using TasksBounds	= typename Plan::TasksBounds;

public:
	using Request		= RequestT <PayloadList>;
	using Requests		= RequestsT<PayloadList, Args::COMPO_COUNT>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Lock {
		inline Lock(FullControlT& control_);
		inline ~Lock();

		FullControlT* const control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	inline FullControlT(Context& context,
						StateData& stateData,
						TaskLinks& tasks,
						TasksBounds& firstTasks,
						Requests& requests,
						LoggerInterface* const logger)
		: Control{context, stateData, tasks, firstTasks, logger}
		, _requests(requests)
	{}

	template <typename T>
	static constexpr LongIndex stateId()					{ return StateList ::template index<T>();	}

	template <typename T>
	static constexpr LongIndex regionId()					{ return RegionList::template index<T>();	}

public:
	using Control::isActive;
	using Control::isResumable;
	using Control::isScheduled;
	using Control::plan;

	inline void changeTo(const StateID stateId);
	inline void resume	(const StateID stateId);
	inline void schedule(const StateID stateId);

	template <typename TState>
	inline void changeTo()									{ changeTo(stateId<TState>());				}

	template <typename TState>
	inline void resume()									{ resume  (stateId<TState>());				}

	template <typename TState>
	inline void schedule()									{ schedule(stateId<TState>());				}

	inline void succeed()									{ _status.success = true;					}
	inline void fail()										{ _status.failure = true;					}

private:
	using Control::_regionId;
	using Control::_regionIndex;
	using Control::_regionSize;
	using Control::_status;

	Requests& _requests;
	bool _locked = false;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_control.inl"
