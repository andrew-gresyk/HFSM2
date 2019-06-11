namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// TODO: add TaskLink::payload

#pragma pack(push, 2)

struct TaskLink {
	HFSM_INLINE TaskLink(const Transition transition_,
						 const StateID origin_,
						 const StateID destination_)
		: transition{transition_}
		, origin(origin_)
		, destination(destination_)
		, next(INVALID_LONG_INDEX)
	{}

	Transition transition;
	StateID origin		= INVALID_STATE_ID;
	StateID destination	= INVALID_STATE_ID;
	// TODO: add paylaods

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
		  typename TPayload,
		  LongIndex NTaskCapacity>
struct PlanDataT<ArgsT<TContext,
					   TConfig,
					   TStateList,
					   TRegionList,
					   NCompoCount,
					   NOrthoCount,
					   NOrthoUnits,
					   TPayload,
					   NTaskCapacity>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;

	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
	static constexpr ShortIndex TASK_CAPACITY = NTaskCapacity;

	using TaskLinks		= List<TaskLink, TASK_CAPACITY>;
	using TasksBounds	= Array<Bounds, RegionList::SIZE>;
	using TasksBits		= BitArray<StateID, StateList::SIZE>;
	using RegionBits	= BitArray<RegionID, RegionList::SIZE>;

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
		  typename TPayload,
		  LongIndex NTaskCapacity>
struct PlanDataT<ArgsT<TContext,
					   TConfig,
					   TStateList,
					   TRegionList,
					   0,
					   NOrthoCount,
					   NOrthoUnits,
					   TPayload,
					   NTaskCapacity>>
{
#ifdef HFSM_ENABLE_ASSERT
	void verifyPlans() const													{}
	LongIndex verifyPlan(const RegionID) const					{ return 0;		}
#endif
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "plan_data.inl"
