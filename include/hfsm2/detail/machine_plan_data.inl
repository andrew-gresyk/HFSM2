namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM_ENABLE_ASSERT

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::verifyPlans() const {
	LongIndex planCount = 0;
	for (RegionID id = 0; id < REGION_COUNT; ++id)
		planCount += verifyPlan(id);

	HFSM_ASSERT(taskLinks.count() == planCount);
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
LongIndex
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::verifyPlan(const RegionID regionId) const {
	LongIndex length = 0;
	const Bounds& bounds = tasksBounds[regionId];

	if (bounds.first != INVALID_LONG_INDEX) {
		HFSM_ASSERT(bounds.last != INVALID_LONG_INDEX);

		for (auto slow = bounds.first, fast = slow; ; ) {
			++length;
			const TaskLink& task = taskLinks[slow];

			if (slow != bounds.last) {
				HFSM_ASSERT(task.next != INVALID_LONG_INDEX);
				slow = task.next;

				// loop check
				if (fast != INVALID_LONG_INDEX) {
					fast = taskLinks[fast].next;

					if (fast != INVALID_LONG_INDEX) {
						fast = taskLinks[fast].next;
					}

					if (fast != INVALID_LONG_INDEX)
						HFSM_ASSERT(slow != fast);
				}
			} else {
				HFSM_ASSERT(task.next == INVALID_LONG_INDEX);

				break;
			}
		};
	} else
		HFSM_ASSERT(bounds.last == INVALID_LONG_INDEX);

	return length;
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
