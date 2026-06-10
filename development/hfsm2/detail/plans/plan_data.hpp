#if HFSM2_PLANS_AVAILABLE()

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

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
  , typename TReactOrder
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
			 , TReactOrder
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

	using Task				= TaskT<Payload>;
	using Tasks				= TaskListT<Task, TASK_CAPACITY, REGION_COUNT>;
	using TaskIndex			= typename Tasks::Index;
	using Bounds			= typename Tasks::Bounds;

	using TasksBits			= BitFlatSetT <	            STATE_COUNT>;
	using RegionBits		= BitFlatSetT <	           REGION_COUNT>;
	using RegionStatuses	= StaticArrayT<TaskStatus, REGION_COUNT>;

	Tasks tasks;
	TasksBits payloadExists;

	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	RegionBits planExists;
	RegionStatuses headStatuses;
	RegionStatuses  subStatuses;

	HFSM2_CONSTEXPR(14)	void clearTaskStatus  (const StateID stateId)		noexcept;
	HFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID stateId) const noexcept;

	HFSM2_CONSTEXPR(14)	void clearStatuses()								noexcept;
	HFSM2_CONSTEXPR(14)	void clear()										noexcept;

#if HFSM2_ASSERT_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void verifyPlans()							  const noexcept;
	HFSM2_CONSTEXPR(14)	TaskIndex verifyPlan(const RegionID regionId) const noexcept;
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
  , typename TReactOrder
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
			 , TReactOrder
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

	using Task				= TaskT<void>;
	using Tasks				= TaskListT<Task, TASK_CAPACITY, REGION_COUNT>;
	using TaskIndex			= typename Tasks::Index;
	using Bounds			= typename Tasks::Bounds;

	using TasksBits			= BitFlatSetT <	            STATE_COUNT>;
	using RegionBits		= BitFlatSetT <	           REGION_COUNT>;
	using RegionStatuses	= StaticArrayT<TaskStatus, REGION_COUNT>;

	Tasks tasks;

	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	RegionBits planExists;
	RegionStatuses headStatuses;
	RegionStatuses  subStatuses;

	HFSM2_CONSTEXPR(14)	void clearTaskStatus  (const StateID stateId)		noexcept;
	HFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID stateId) const noexcept;

	HFSM2_CONSTEXPR(14)	void clearStatuses()								noexcept;
	HFSM2_CONSTEXPR(14)	void clear()										noexcept;

#if HFSM2_ASSERT_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void verifyPlans()							  const noexcept;
	HFSM2_CONSTEXPR(14)	TaskIndex verifyPlan(const RegionID regionId) const noexcept;
#endif
};

//------------------------------------------------------------------------------

template <
	typename TConfig
  , typename TStateList
  , typename TRegionList
  , Long NOrthoCount
  , Long NOrthoUnits
  , typename TReactOrder
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
			 , TReactOrder
			 HFSM2_IF_SERIALIZATION(, 0)
			 , NTaskCapacity
			 , TPayload
		   >
	   > final
{
	HFSM2_CONSTEXPR(14)	void clearTaskStatus  (const StateID)				noexcept	{}
	HFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID)		  const noexcept	{}

	HFSM2_CONSTEXPR(14)	void clearStatuses()								noexcept	{}
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
  , typename TReactOrder
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
			 , TReactOrder
			 HFSM2_IF_SERIALIZATION(, 0)
			 , NTaskCapacity
			 , void
		   >
	   > final
{
	HFSM2_CONSTEXPR(14)	void clearTaskStatus  (const StateID)				noexcept	{}
	HFSM2_CONSTEXPR(14)	void verifyEmptyStatus(const StateID)		  const noexcept	{}

	HFSM2_CONSTEXPR(14)	void clearStatuses()								noexcept	{}
	HFSM2_CONSTEXPR(14)	void clear()										noexcept	{}

#if HFSM2_ASSERT_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void verifyPlans()							  const noexcept	{}
#endif
};

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#include "plan_data.inl"
