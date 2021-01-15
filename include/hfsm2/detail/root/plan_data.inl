#ifdef HFSM2_ENABLE_PLANS

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL, Long NTC, typename TTP>
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL, NTC, TTP>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL, Long NTC, typename TTP>
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL, NTC, TTP>>::verifyEmptyStatus(const StateID HFSM2_IF_ASSERT(stateId)) const noexcept {
#ifdef HFSM2_ENABLE_ASSERT

	if (stateId != INVALID_STATE_ID) {
		HFSM2_ASSERT(!tasksSuccesses.get(stateId));
		HFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_ASSERT

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL, Long NTC, typename TTP>
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL, NTC, TTP>>::verifyPlans() const noexcept {
	Long planCount = 0;

	for (RegionID regionId = 0; regionId < REGION_COUNT; ++regionId)
		planCount += verifyPlan(regionId);

	HFSM2_ASSERT(tasks.count() == planCount);
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL, Long NTC, typename TTP>
Long
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL, NTC, TTP>>::verifyPlan(const RegionID regionId) const noexcept {
	Long length = 0;
	const Bounds& bounds = tasksBounds[regionId];

	if (bounds.first != INVALID_LONG) {
		HFSM2_ASSERT(bounds.last != INVALID_LONG);

		for (auto slow = bounds.first, fast = slow; ; ) {
			++length;
			const TaskLink& task = taskLinks[slow];

			if (slow != bounds.last) {
				HFSM2_ASSERT(task.next != INVALID_LONG);
				slow = task.next;

				// loop check
				if (fast != INVALID_LONG) {
					fast = taskLinks[fast].next;

					if (fast != INVALID_LONG)
						fast = taskLinks[fast].next;

					HFSM2_ASSERT(fast == INVALID_LONG || slow != fast);
				}
			} else {
				HFSM2_ASSERT(task.next == INVALID_LONG);

				break;
			}
		};
	} else
		HFSM2_ASSERT(bounds.last == INVALID_LONG);

	return length;
}

#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL, Long NTC>
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL, NTC, void>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL, Long NTC>
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL, NTC, void>>::verifyEmptyStatus(const StateID HFSM2_IF_ASSERT(stateId)) const noexcept {
#ifdef HFSM2_ENABLE_ASSERT

	if (stateId != INVALID_STATE_ID) {
		HFSM2_ASSERT(!tasksSuccesses.get(stateId));
		HFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_ASSERT

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL, Long NTC>
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL, NTC, void>>::verifyPlans() const noexcept {
	Long planCount = 0;

	for (RegionID regionId = 0; regionId < REGION_COUNT; ++regionId)
		planCount += verifyPlan(regionId);

	HFSM2_ASSERT(tasks.count() == planCount);
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL, Long NTC>
Long
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL, NTC, void>>::verifyPlan(const RegionID regionId) const noexcept {
	Long length = 0;
	const Bounds& bounds = tasksBounds[regionId];

	if (bounds.first != INVALID_LONG) {
		HFSM2_ASSERT(bounds.last != INVALID_LONG);

		for (auto slow = bounds.first, fast = slow; ; ) {
			++length;
			const TaskLink& task = taskLinks[slow];

			if (slow != bounds.last) {
				HFSM2_ASSERT(task.next != INVALID_LONG);
				slow = task.next;

				// loop check
				if (fast != INVALID_LONG) {
					fast = taskLinks[fast].next;

					if (fast != INVALID_LONG)
						fast = taskLinks[fast].next;

					HFSM2_ASSERT(fast == INVALID_LONG || slow != fast);
				}
			} else {
				HFSM2_ASSERT(task.next == INVALID_LONG);

				break;
			}
		};
	} else
		HFSM2_ASSERT(bounds.last == INVALID_LONG);

	return length;
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
