namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct TaskStatus final {
	enum Result {
		NONE,
		SUCCESS,
		FAILURE
	};

	Result result = Result::NONE;
	bool outerTransition = false;

	HFSM2_CONSTEXPR(11)	TaskStatus(const Result result_ = Result::NONE,
								   const bool outerTransition_ = false)	noexcept;

	HFSM2_CONSTEXPR(11)	explicit operator bool()				  const noexcept;

	HFSM2_CONSTEXPR(14)	void clear()									noexcept;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(14) TaskStatus  operator |  (TaskStatus& l, const TaskStatus r)	noexcept;
HFSM2_CONSTEXPR(14) TaskStatus& operator |= (TaskStatus& l, const TaskStatus r)	noexcept;

////////////////////////////////////////////////////////////////////////////////

#if HFSM2_PLANS_AVAILABLE()

struct TaskLink final {
	Long prev		= INVALID_LONG;
	Long next		= INVALID_LONG;
};

//------------------------------------------------------------------------------

struct Bounds final {
	Long first		= INVALID_LONG;
	Long last		= INVALID_LONG;
};

////////////////////////////////////////////////////////////////////////////////

template <
	typename
  , typename
  , typename
  , Long
  , Long
  , Long
  HFSM2_IF_SERIALIZATION(, Long)
  , Long
  , typename
>
struct ArgsT;

template <typename>
struct PlanDataT;

//------------------------------------------------------------------------------

template <
	typename TConfig
  , typename TStateList
  , typename TRegionList
  , Long NCompoCount
  , Long NOrthoCount
  , Long NOrthoUnits
  HFSM2_IF_SERIALIZATION(, Long NSerialBits)
  , Long NTaskCapacity
  , typename TPayload
>
struct PlanDataT<
		   ArgsT<
			   TConfig
			 , TStateList
			 , TRegionList
			 , NCompoCount
			 , NOrthoCount
			 , NOrthoUnits
			 HFSM2_IF_SERIALIZATION(, NSerialBits)
			 , NTaskCapacity
			 , TPayload
		   >
	   > final
{
	using StateList			= TStateList;
	using RegionList		= TRegionList;
	using Payload			= TPayload;

	static constexpr Long STATE_COUNT	= StateList ::SIZE;
	static constexpr Long REGION_COUNT	= RegionList::SIZE;
	static constexpr Long TASK_CAPACITY	= NTaskCapacity;

	using Task				= TaskT		  <Payload>;
	using Tasks				= TaskListT   <Payload,  TASK_CAPACITY>;
	using TaskLinks			= StaticArrayT<TaskLink, TASK_CAPACITY>;
	using Payloads			= StaticArrayT<Payload,  TASK_CAPACITY>;

	using TasksBounds		= StaticArrayT<Bounds,	   REGION_COUNT>;
	using TasksBits			= BitArrayT   <				STATE_COUNT>;
	using RegionBits		= BitArrayT   <			   REGION_COUNT>;
	using RegionStatuses	= StaticArrayT<TaskStatus, REGION_COUNT>;

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

	HFSM2_CONSTEXPR(14)	void clearTaskStatus  (const StateID stateId)		noexcept;
	HFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID stateId) const noexcept;

	HFSM2_CONSTEXPR(14)	void clearRegionStatuses()							noexcept;
	HFSM2_CONSTEXPR(14)	void clear()										noexcept;

#if HFSM2_ASSERT_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void verifyPlans()							  const noexcept;
	HFSM2_CONSTEXPR(14)	Long verifyPlan(const RegionID stateId)		  const noexcept;
#endif
};

//------------------------------------------------------------------------------

template <
	typename TConfig
  , typename TStateList
  , typename TRegionList
  , Long NCompoCount
  , Long NOrthoCount
  , Long NOrthoUnits
  HFSM2_IF_SERIALIZATION(, Long NSerialBits)
  , Long NTaskCapacity
>
struct PlanDataT<
		   ArgsT<
			   TConfig
			 , TStateList
			 , TRegionList
			 , NCompoCount
			 , NOrthoCount
			 , NOrthoUnits
			 HFSM2_IF_SERIALIZATION(, NSerialBits)
			 , NTaskCapacity
			 , void
		   >
	   > final
{
	using StateList			= TStateList;
	using RegionList		= TRegionList;

	static constexpr Long  STATE_COUNT	= StateList ::SIZE;
	static constexpr Long REGION_COUNT	= RegionList::SIZE;
	static constexpr Long TASK_CAPACITY	= NTaskCapacity;

	using Task				= TaskT		  <void>;
	using Tasks				= TaskListT	  <void,	 TASK_CAPACITY>;
	using TaskLinks			= StaticArrayT<TaskLink, TASK_CAPACITY>;

	using TasksBounds		= StaticArrayT<Bounds,	   REGION_COUNT>;
	using TasksBits			= BitArrayT	  <				STATE_COUNT>;
	using RegionBits		= BitArrayT	  <			   REGION_COUNT>;
	using RegionStatuses	= StaticArrayT<TaskStatus, REGION_COUNT>;

	Tasks tasks;
	TaskLinks taskLinks;

	TasksBounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	RegionBits planExists;
	RegionStatuses headStatuses;
	RegionStatuses subStatuses;

	HFSM2_CONSTEXPR(14)	void clearTaskStatus  (const StateID stateId)		noexcept;
	HFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID stateId) const noexcept;

	HFSM2_CONSTEXPR(14)	void clearRegionStatuses()							noexcept;
	HFSM2_CONSTEXPR(14)	void clear()										noexcept;

#if HFSM2_ASSERT_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void verifyPlans()							  const noexcept;
	HFSM2_CONSTEXPR(14)	Long verifyPlan(const RegionID stateId)		  const noexcept;
#endif
};

//------------------------------------------------------------------------------

template <
	typename TConfig
  , typename TStateList
  , typename TRegionList
  , Long NOrthoCount
  , Long NOrthoUnits
  , Long NTaskCapacity
  , typename TPayload
>
struct PlanDataT<
		   ArgsT<
			   TConfig
			 , TStateList
			 , TRegionList
			 , 0
			 , NOrthoCount
			 , NOrthoUnits
			 HFSM2_IF_SERIALIZATION(, 0)
			 , NTaskCapacity
			 , TPayload
		   >
	   > final
{
	HFSM2_CONSTEXPR(14)	void clearTaskStatus  (const StateID)				noexcept	{}
	HFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID)		  const noexcept	{}

	HFSM2_CONSTEXPR(14)	void clearRegionStatuses()							noexcept	{}
	HFSM2_CONSTEXPR(14)	void clear()										noexcept	{}

#if HFSM2_ASSERT_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void verifyPlans()							  const noexcept	{}
#endif
};

//------------------------------------------------------------------------------

template <
	typename TConfig
  , typename TStateList
  , typename TRegionList
  , Long NOrthoCount
  , Long NOrthoUnits
  , Long NTaskCapacity
>
struct PlanDataT<
		   ArgsT<
			   TConfig
			 , TStateList
			 , TRegionList
			 , 0
			 , NOrthoCount
			 , NOrthoUnits
			 HFSM2_IF_SERIALIZATION(, 0)
			 , NTaskCapacity
			 , void
		   >
	   > final
{
	HFSM2_CONSTEXPR(14)	void clearTaskStatus  (const StateID)				noexcept	{}
	HFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID)		  const noexcept	{}

	HFSM2_CONSTEXPR(14)	void clearRegionStatuses()							noexcept	{}
	HFSM2_CONSTEXPR(14)	void clear()										noexcept	{}

#if HFSM2_ASSERT_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void verifyPlans()							  const noexcept	{}
#endif
};

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#include "plan_data.inl"
