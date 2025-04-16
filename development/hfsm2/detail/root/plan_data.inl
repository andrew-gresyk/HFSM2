namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

HFSM2_CONSTEXPR(11)
TaskStatus::TaskStatus(const Result result_,
					   const bool outerTransition_) noexcept
	: result{result_}
	, outerTransition{outerTransition_}
{}

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(11)
TaskStatus::operator bool() const noexcept	{
	return result != Result::NONE || outerTransition;
}

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(14)
void
TaskStatus::clear() noexcept {
	result = Result::NONE;
	outerTransition = false;
}

////////////////////////////////////////////////////////////////////////////////

HFSM2_CONSTEXPR(14)
TaskStatus
operator | (TaskStatus& lhs,
			const TaskStatus rhs) noexcept
{
	const TaskStatus::Result result = lhs.result > rhs.result ?
		lhs.result : rhs.result;

	return TaskStatus{result, lhs.outerTransition || rhs.outerTransition};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

HFSM2_CONSTEXPR(14)
TaskStatus&
operator |= (TaskStatus& lhs,
			 const TaskStatus rhs) noexcept
{
	const TaskStatus::Result result = lhs.result > rhs.result ?
										  lhs.result : rhs.result;

	lhs = TaskStatus{result, lhs.outerTransition || rhs.outerTransition};

	return lhs;
}

////////////////////////////////////////////////////////////////////////////////

#if HFSM2_PLANS_AVAILABLE()

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
	taskLinks	 .clear();
	taskPayloads .clear();
	payloadExists.clear();

	taskBounds	 .clear();
	planExists	 .clear();

	clearStatuses();
}

//------------------------------------------------------------------------------

#if HFSM2_ASSERT_AVAILABLE()

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::verifyPlans() const noexcept {
	Long planCount = 0;

	for (RegionID regionId = 0; regionId < REGION_COUNT; ++regionId)
		planCount += verifyPlan(regionId);

	HFSM2_ASSERT(tasks.count() == planCount);
}

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
HFSM2_CONSTEXPR(14)
Long
PlanDataT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::verifyPlan(const RegionID regionId) const noexcept {
	Long length = 0;
	const Bounds& bounds = taskBounds[regionId];

	if (bounds.first != INVALID_LONG) {
		HFSM2_ASSERT(bounds.last != INVALID_LONG);

		for (Long slow = bounds.first, fast = slow; ; ) {
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
	taskLinks .clear();

	taskBounds.clear();
	planExists.clear();

	clearStatuses();
}

//------------------------------------------------------------------------------

#if HFSM2_ASSERT_AVAILABLE()

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::verifyPlans() const noexcept {
	Long planCount = 0;

	for (RegionID regionId = 0; regionId < REGION_COUNT; ++regionId)
		planCount += verifyPlan(regionId);

	HFSM2_ASSERT(tasks.count() == planCount);
}

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
HFSM2_CONSTEXPR(14)
Long
PlanDataT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::verifyPlan(const RegionID regionId) const noexcept {
	Long length = 0;
	const Bounds& bounds = taskBounds[regionId];

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
#endif

////////////////////////////////////////////////////////////////////////////////

}
}
