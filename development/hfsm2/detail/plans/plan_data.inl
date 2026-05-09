#if HFSM2_PLANS_AVAILABLE()

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::verifyEmptyStatus(const StateID HFSM2_IF_ASSERT(stateId)) const noexcept {
#if HFSM2_ASSERT_AVAILABLE()

	if (stateId != INVALID_STATE_ID) {
		HFSM2_ASSERT(!tasksSuccesses.get(stateId));
		HFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::clearStatuses() noexcept {
	tasksSuccesses.clear();
	tasksFailures .clear();

	headStatuses.clear();
	 subStatuses.clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::clear() noexcept {
	tasks		 .clear();
	payloadExists.clear();

	planExists	 .clear();

	clearStatuses();
}

//------------------------------------------------------------------------------

#if HFSM2_ASSERT_AVAILABLE()

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::verifyPlans() const noexcept {
	TaskIndex planCount = 0;

	for (RegionID regionId = 0; regionId < REGION_COUNT; ++regionId)
		planCount += verifyPlan(regionId);

	HFSM2_ASSERT(tasks.count() == planCount);
}

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
HFSM2_CONSTEXPR(14)
typename PlanDataT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::TaskIndex
PlanDataT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::verifyPlan(const RegionID regionId) const noexcept {
	TaskIndex length = 0;
	const Bounds& regionBounds = tasks.bounds(regionId);

	if (regionBounds) {
		HFSM2_ASSERT(regionBounds.last != Tasks::invalid());

		for (TaskIndex slow = regionBounds.first, fast = slow; ; ) {
			++length;

			if (slow != regionBounds.last) {
				const TaskIndex slowNext = tasks.next(slow);
				HFSM2_ASSERT(slowNext != Tasks::invalid());
				slow = slowNext;

				// loop check
				if (fast != Tasks::invalid()) {
					fast = tasks.next(fast);

					if (fast != Tasks::invalid())
						fast = tasks.next(fast);

					HFSM2_ASSERT(fast == Tasks::invalid() || slow != fast);
				}
			}
			else {
				HFSM2_ASSERT(tasks.next(slow) == Tasks::invalid());

				break;
			}
		}
	}
	else
		HFSM2_ASSERT(regionBounds.last == Tasks::invalid());

	return length;
}

#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::verifyEmptyStatus(const StateID HFSM2_IF_ASSERT(stateId)) const noexcept {
#if HFSM2_ASSERT_AVAILABLE()

	if (stateId != INVALID_STATE_ID) {
		HFSM2_ASSERT(!tasksSuccesses.get(stateId));
		HFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::clearStatuses() noexcept {
	tasksSuccesses.clear();
	tasksFailures .clear();

	headStatuses.clear();
	 subStatuses.clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::clear() noexcept {
	tasks	  .clear();

	planExists.clear();

	clearStatuses();
}

//------------------------------------------------------------------------------

#if HFSM2_ASSERT_AVAILABLE()

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::verifyPlans() const noexcept {
	TaskIndex planCount = 0;

	for (RegionID regionId = 0; regionId < REGION_COUNT; ++regionId)
		planCount += verifyPlan(regionId);

	HFSM2_ASSERT(tasks.count() == planCount);
}

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
HFSM2_CONSTEXPR(14)
typename PlanDataT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::TaskIndex
PlanDataT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::verifyPlan(const RegionID regionId) const noexcept {
	TaskIndex length = 0;
	const Bounds& regionBounds = tasks.bounds(regionId);

	if (regionBounds) {
		HFSM2_ASSERT(regionBounds.last != Tasks::invalid());

		for (TaskIndex slow = regionBounds.first, fast = slow; ; ) {
			++length;

			if (slow != regionBounds.last) {
				const TaskIndex slowNext = tasks.next(slow);
				HFSM2_ASSERT(slowNext != Tasks::invalid());
				slow = slowNext;

				// loop check
				if (fast != Tasks::invalid()) {
					fast = tasks.next(fast);

					if (fast != Tasks::invalid())
						fast = tasks.next(fast);

					HFSM2_ASSERT(fast == Tasks::invalid() || slow != fast);
				}
			}
			else {
				HFSM2_ASSERT(tasks.next(slow) == Tasks::invalid());

				break;
			}
		}
	}
	else
		HFSM2_ASSERT(regionBounds.last == Tasks::invalid());

	return length;
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
