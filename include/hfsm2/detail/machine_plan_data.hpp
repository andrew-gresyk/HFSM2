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

	HSFM_INLINE TaskLink(const StateID origin_,
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

template <typename TArgs>
struct PlanDataT {
	using Args			 = TArgs;
	using StateList		 = typename Args::StateList;
	using RegionList	 = typename Args::RegionList;

	static constexpr ShortIndex REGION_COUNT = RegionList::SIZE;

	using TaskLinks		 = List<TaskLink, Args::TASK_CAPACITY>;
	using TasksBounds	 = Array<Bounds, RegionList::SIZE>;
	using TasksBits		 = BitArrayStorageT<StateID, StateList::SIZE>;
	using RegionBit		 = BitT<RegionID>;
	using RegionBits	 = BitArrayStorageT<RegionID, RegionList::SIZE>;

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

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_plan_data.inl"
