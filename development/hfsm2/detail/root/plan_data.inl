namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

HFSM2_CONSTEXPR(11)
Status::Status(const Result result_,
			   const bool outerTransition_) noexcept
	: result{result_}
	, outerTransition{outerTransition_}
{}

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(11)
Status::operator bool() const noexcept	{
	return result != Result::NONE || outerTransition;
}

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(14)
void
Status::clear() noexcept {
	result = Result::NONE;
	outerTransition = false;
}

////////////////////////////////////////////////////////////////////////////////

HFSM2_CONSTEXPR(14)
Status
operator | (Status& lhs,
			const Status rhs) noexcept
{
	const Status::Result result = lhs.result > rhs.result ?
		lhs.result : rhs.result;

	return Status{result, lhs.outerTransition || rhs.outerTransition};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

HFSM2_CONSTEXPR(14)
Status&
operator |= (Status& lhs,
			 const Status rhs) noexcept
{
	const Status::Result result = lhs.result > rhs.result ?
		lhs.result : rhs.result;

	lhs = Status{result, lhs.outerTransition || rhs.outerTransition};

	return lhs;
}

//------------------------------------------------------------------------------

template <>
HFSM2_CONSTEXPR(11)
Status
filler<Status>() noexcept {
	return Status{};
}

////////////////////////////////////////////////////////////////////////////////

#if HFSM2_PLANS_AVAILABLE()

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::verifyEmptyStatus(const StateID HFSM2_IF_ASSERT(stateId)) const noexcept {
#if HFSM2_ASSERT_AVAILABLE()

	if (stateId != INVALID_STATE_ID) {
		HFSM2_ASSERT(!tasksSuccesses.get(stateId));
		HFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::clearRegionStatuses() noexcept {
	headStatuses.clear();
	subStatuses	.clear();
}

//------------------------------------------------------------------------------

#if HFSM2_ASSERT_AVAILABLE()

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::verifyPlans() const noexcept {
	Long planCount = 0;

	for (RegionID regionId = 0; regionId < REGION_COUNT; ++regionId)
		planCount += verifyPlan(regionId);

	HFSM2_ASSERT(tasks.count() == planCount);
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC, typename TTP>
HFSM2_CONSTEXPR(14)
Long
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, TTP>>::verifyPlan(const RegionID regionId) const noexcept {
	Long length = 0;
	const Bounds& bounds = tasksBounds[regionId];

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

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::clearTaskStatus(const StateID stateId) noexcept {
	if (stateId != INVALID_STATE_ID) {
		tasksSuccesses.clear(stateId);
		tasksFailures .clear(stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::verifyEmptyStatus(const StateID HFSM2_IF_ASSERT(stateId)) const noexcept {
#if HFSM2_ASSERT_AVAILABLE()

	if (stateId != INVALID_STATE_ID) {
		HFSM2_ASSERT(!tasksSuccesses.get(stateId));
		HFSM2_ASSERT(!tasksFailures .get(stateId));
	}

#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::clearRegionStatuses() noexcept {
	headStatuses.clear();
	subStatuses	.clear();
}

//------------------------------------------------------------------------------

#if HFSM2_ASSERT_AVAILABLE()

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
HFSM2_CONSTEXPR(14)
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::verifyPlans() const noexcept {
	Long planCount = 0;

	for (RegionID regionId = 0; regionId < REGION_COUNT; ++regionId)
		planCount += verifyPlan(regionId);

	HFSM2_ASSERT(tasks.count() == planCount);
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC_, Long NOC, Long NOU HFSM2_IF_SERIALIZATION(, Long NSB), Long NSL, Long NTC>
HFSM2_CONSTEXPR(14)
Long
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC_, NOC, NOU HFSM2_IF_SERIALIZATION(, NSB), NSL, NTC, void>>::verifyPlan(const RegionID regionId) const noexcept {
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
#endif

////////////////////////////////////////////////////////////////////////////////

}
}
