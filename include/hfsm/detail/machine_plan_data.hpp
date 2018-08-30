namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
struct PlanDataT {
	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using Plan			= PlanT<StateList, RegionList::SIZE, Args::TASK_CAPACITY>;
	using TaskLinks		= typename Plan::TaskLinks;
	using TasksBounds	= typename Plan::TasksBounds;

	TaskLinks taskLinks;
	TasksBounds tasksBounds;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_plan_data.inl"
