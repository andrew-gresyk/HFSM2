namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class PlanControlT
	: public ControlT<TArgs>
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename, Strategy, typename, typename...>
	friend struct C_;

	template <typename, typename, typename, typename...>
	friend struct O_;

	template <typename, typename>
	friend class R_;

	template <typename, typename>
	friend class RV_;

protected:
	using Control			= ControlT<TArgs>;

	using typename Control::StateList;
	using typename Control::RegionList;

	using typename Control::Core;

	using TransitionSets	= typename Core::TransitionSets;

#if HFSM2_PLANS_AVAILABLE()
	using typename Control::PlanData;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		HFSM2_CONSTEXPR(14)	Region(PlanControlT& control,
								   const RegionID regionId_,
								   const StateID index,
								   const Long size)								noexcept;

		HFSM2_CONSTEXPR(20)	~Region()											noexcept;

		PlanControlT& control;
		const RegionID prevId;
		const Long prevIndex;
		const Long prevSize;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(11)	PlanControlT(Core& core,
									 const TransitionSets& currentTransitions)	noexcept
		: Control{core}
		, _currentTransitions{currentTransitions}
	{}

	HFSM2_CONSTEXPR(14)	void   setRegion(const RegionID regionId_,
										 const StateID index,
										 const Long size)						noexcept;

	HFSM2_CONSTEXPR(14)	void resetRegion(const RegionID regionId_,
										 const StateID index,
										 const Long size)						noexcept;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_PLANS_AVAILABLE()
	using typename Control::CPlan;

	using Plan				= PayloadPlanT<TArgs>;

	/// @brief Access plan for the current region
	/// @return Plan for the current region
	/// @see `HFSM2_ENABLE_PLANS`
	HFSM2_CONSTEXPR(14)  Plan plan()										noexcept	{ return  Plan{_core.registry, _core.planData, _regionId							};	}

// COMMON

	/// @brief Access plan for a region
	/// @param `regionId` Region identifier
	/// @return Plan for the region
	/// @see `HFSM2_ENABLE_PLANS`
	HFSM2_CONSTEXPR(14)  Plan plan(const RegionID regionId_)				noexcept	{ return  Plan{_core.registry, _core.planData,  regionId_							};	}

	/// @brief Access plan for a region
	/// @tparam `TRegion` Region head state type
	/// @return Plan for the region
	/// @see `HFSM2_ENABLE_PLANS`
	template <typename TRegion>
	HFSM2_CONSTEXPR(14)  Plan plan()										noexcept	{ return  Plan{_core.registry, _core.planData, Control::template regionId<TRegion>()};	}

// COMMON

	/// @brief Access plan for the current region
	/// @return Plan for the current region
	/// @see `HFSM2_ENABLE_PLANS`
	HFSM2_CONSTEXPR(11)	CPlan plan()								  const noexcept	{ return CPlan{_core.registry, _core.planData, _regionId							};	}

// COMMON

	/// @brief Access plan for a region
	/// @param `regionId`
	/// @return Plan for the region
	/// @see `HFSM2_ENABLE_PLANS`
	HFSM2_CONSTEXPR(11)	CPlan plan(const RegionID regionId_)		  const noexcept	{ return CPlan{_core.registry, _core.planData,  regionId_							};	}

	/// @brief Access plan for a region
	/// @tparam `TRegion` Region head state type
	/// @return Plan for the region
	/// @see `HFSM2_ENABLE_PLANS`
	template <typename TRegion>
	HFSM2_CONSTEXPR(11)	CPlan plan()								  const noexcept	{ return CPlan{_core.registry, _core.planData, Control::template regionId<TRegion>()};	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get current transition requests
	/// @return DynamicArrayT of pending transition requests
	HFSM2_CONSTEXPR(11)	const TransitionSets& currentTransitions()	  const noexcept	{ return _currentTransitions;															}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using Control::_core;
	using Control::_regionId;

	const TransitionSets& _currentTransitions;

	StateID _regionStateId = 0;
	Long _regionSize = StateList::SIZE;
	TaskStatus _taskStatus;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "control_2.inl"
