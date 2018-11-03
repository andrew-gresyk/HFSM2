namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// TODO: add TaskLink::payload

#pragma pack(push, 2)

//template <typename TPayloadList>
//struct TaskLinkT {

struct TaskLink {
	//using PayloadList	= TPayloadList;
	//using PayloadBox		= typename PayloadList::Variant;

	HFSM_INLINE TaskLink(const StateID origin_,
						 const StateID destination_)
		: origin(origin_)
		, destination(destination_)
		, next(INVALID_LONG_INDEX)
	{}

	//PayloadBox payload;

	StateID origin		= INVALID_STATE_ID;
	StateID destination	= INVALID_STATE_ID;

	LongIndex prev		= INVALID_LONG_INDEX;
	LongIndex next		= INVALID_LONG_INDEX;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Bounds {
	LongIndex first		= INVALID_LONG_INDEX;
	LongIndex last		= INVALID_LONG_INDEX;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

template <typename,
		  typename,
		  typename,
		  typename,
		  LongIndex,
		  LongIndex,
		  LongIndex,
		  typename,
		  LongIndex>
struct ArgsT;

template <typename>
struct PlanDataT;

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NCompoCount,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  typename TPayloadList,
		  LongIndex NTaskCapacity>
struct PlanDataT<ArgsT<TContext,
					   TConfig,
					   TStateList,
					   TRegionList,
					   NCompoCount,
					   NOrthoCount,
					   NOrthoUnits,
					   TPayloadList,
					   NTaskCapacity>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;

	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
	static constexpr ShortIndex TASK_CAPACITY = NTaskCapacity;

	using TaskLinks		= List<TaskLink, TASK_CAPACITY>;
	using TasksBounds	= Array<Bounds, RegionList::SIZE>;
	using TasksBits		= BitArrayStorageT<StateID, StateList::SIZE>;
	using RegionBit		= BitT<RegionID>;
	using RegionBits	= BitArrayStorageT<RegionID, RegionList::SIZE>;

	HFSM_INLINE void setSuccessful(const StateID stateId, const bool state) { tasksSuccesses[stateId]  = state;	}
	HFSM_INLINE void setFailed	  (const StateID stateId, const bool state) { tasksFailures [stateId]  = state;	}

	HFSM_INLINE bool hasSucceeded (const StateID stateId) const	   { return !!tasksSuccesses[stateId];			}
	HFSM_INLINE bool hasFailed	  (const StateID stateId) const	   { return !!tasksFailures [stateId];			}

	TaskLinks taskLinks;
	TasksBounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	RegionBits planExists;

#ifdef HFSM_ENABLE_ASSERT
	void verifyPlans() const;
	LongIndex verifyPlan(const RegionID stateId) const;
#endif
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  typename TPayloadList,
		  LongIndex NTaskCapacity>
struct PlanDataT<ArgsT<TContext,
					   TConfig,
					   TStateList,
					   TRegionList,
					   0,
					   NOrthoCount,
					   NOrthoUnits,
					   TPayloadList,
					   NTaskCapacity>>
{
	static constexpr void setSuccessful(const StateID, const bool)				{}
	static constexpr void setFailed	   (const StateID, const bool)				{}

	static constexpr bool hasSucceeded (const StateID)			{ return false;	}
	static constexpr bool hasFailed	   (const StateID)			{ return false;	}

#ifdef HFSM_ENABLE_ASSERT
	void verifyPlans() const													{}
	LongIndex verifyPlan(const RegionID) const									{}
#endif
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_plan_data.inl"
