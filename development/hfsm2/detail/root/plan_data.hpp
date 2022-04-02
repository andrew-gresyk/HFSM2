namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct Status final {
	enum class Result {
		NONE,
		SUCCESS,
		FAILURE
	};

	Result result = Result::NONE;
	bool outerTransition = false;

	HFSM2_CONSTEXPR(11)	Status(const Result result_ = Result::NONE,
							   const bool outerTransition_ = false)	  noexcept
		: result{result_}
		, outerTransition{outerTransition_}
	{}

	HFSM2_CONSTEXPR(11)	explicit operator bool()				const noexcept	{ return result != Result::NONE || outerTransition;	}

	HFSM2_CONSTEXPR(14)	void clear()								  noexcept;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(14)
Status
operator | (Status& lhs, const Status rhs)							  noexcept	{
	const Status::Result result = lhs.result > rhs.result ?
		lhs.result : rhs.result;

	return Status{result, lhs.outerTransition || rhs.outerTransition};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

HFSM2_CONSTEXPR(14)
Status&
operator |= (Status& lhs, const Status rhs)							  noexcept	{
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

#pragma pack(push, 2)

struct TaskLink final {
	Long prev		= INVALID_LONG;
	Long next		= INVALID_LONG;
};

//------------------------------------------------------------------------------

struct Bounds final {
	Long first		= INVALID_LONG;
	Long last		= INVALID_LONG;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

template <typename
		, typename
		, typename
		, typename
		, Long
		, Long
		, Long
		HFSM2_IF_SERIALIZATION(, Long)
		, Long
		, Long
		, typename>
struct ArgsT;

template <typename>
struct PlanDataT;

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NCompoCount
		, Long NOrthoCount
		, Long NOrthoUnits
		HFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		, Long NTaskCapacity
		, typename TPayload>
struct PlanDataT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 , TRegionList
					 , NCompoCount
					 , NOrthoCount
					 , NOrthoUnits
					 HFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , TPayload>> final
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;
	using Payload		= TPayload;

	static constexpr Short REGION_COUNT		= RegionList::SIZE;
	static constexpr Short TASK_CAPACITY	= NTaskCapacity;

	using Task			 = TaskT<Payload>;
	using Tasks			 = TaskListT	  <Payload,  TASK_CAPACITY>;
	using TaskLinks		 = StaticArrayT<TaskLink, TASK_CAPACITY>;
	using Payloads		 = StaticArrayT<Payload,  TASK_CAPACITY>;

	using TasksBounds	 = ArrayT   <Bounds,   RegionList::SIZE>;
	using TasksBits		 = BitArrayT<StateList::SIZE>;
	using RegionBits	 = BitArrayT<RegionList::SIZE>;
	using RegionStatuses = StaticArrayT<Status, RegionList::SIZE>;

	Tasks tasks;
	TaskLinks taskLinks;
	Payloads taskPayloads;
	TasksBits payloadExists;

	TasksBounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	RegionBits planExists;
	RegionStatuses headStatuses;
	RegionStatuses subStatuses;

	HFSM2_CONSTEXPR(14)	void clearTaskStatus  (const StateID stateId)		  noexcept;
	HFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID stateId)	const noexcept;

	HFSM2_CONSTEXPR(14)	void clearRegionStatuses()							  noexcept;

#if HFSM2_ASSERT_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void verifyPlans()								const noexcept;
	HFSM2_CONSTEXPR(14)	Long verifyPlan(const RegionID stateId)			const noexcept;
#endif
};

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NCompoCount
		, Long NOrthoCount
		, Long NOrthoUnits
		HFSM2_IF_SERIALIZATION(, Long NSerialBits)
		, Long NSubstitutionLimit
		, Long NTaskCapacity>
struct PlanDataT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 , TRegionList
					 , NCompoCount
					 , NOrthoCount
					 , NOrthoUnits
					 HFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , void>> final
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;

	static constexpr Short REGION_COUNT		= RegionList::SIZE;
	static constexpr Short TASK_CAPACITY	= NTaskCapacity;

	using Task			= TaskT<void>;
	using Tasks			= TaskListT<void, TASK_CAPACITY>;
	using TaskLinks		= StaticArrayT<TaskLink, TASK_CAPACITY>;

	using TasksBounds	= ArrayT   <Bounds,   RegionList::SIZE>;
	using TasksBits		= BitArrayT<StateList::SIZE>;
	using RegionBits	= BitArrayT<RegionList::SIZE>;
	using RegionStatuses = StaticArrayT<Status, RegionList::SIZE>;

	Tasks tasks;
	TaskLinks taskLinks;

	TasksBounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	RegionBits planExists;
	RegionStatuses headStatuses;
	RegionStatuses subStatuses;

	HFSM2_CONSTEXPR(14)	void clearTaskStatus  (const StateID stateId)		  noexcept;
	HFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID stateId)	const noexcept;

	HFSM2_CONSTEXPR(14)	void clearRegionStatuses()							  noexcept;

#if HFSM2_ASSERT_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void verifyPlans()								const noexcept;
	HFSM2_CONSTEXPR(14)	Long verifyPlan(const RegionID stateId)			const noexcept;
#endif
};

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NOrthoCount
		, Long NOrthoUnits
		, Long NSubstitutionLimit
		, Long NTaskCapacity
		, typename TPayload>
struct PlanDataT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 , TRegionList
					 , 0
					 , NOrthoCount
					 , NOrthoUnits
					 HFSM2_IF_SERIALIZATION(, 0)
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , TPayload>> final
{
	HFSM2_CONSTEXPR(14)	void clearTaskStatus  (const StateID)				  noexcept	{}
	HFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID)			const noexcept	{}

	HFSM2_CONSTEXPR(14)	void clearRegionStatuses()							  noexcept	{}

#if HFSM2_ASSERT_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void verifyPlans()								const noexcept	{}
#endif
};

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#include "plan_data.inl"
