namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct Status {
	enum Result {
		NONE,
		SUCCESS,
		FAILURE
	};

	Result result = NONE;
	bool outerTransition = false;

	inline Status(const Result result_ = NONE,
				  const bool outerTransition_ = false);

	inline explicit operator bool() const											{ return result || outerTransition;									}

	inline void clear();
};

#pragma pack(pop)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Status
combine(const Status lhs, const Status rhs) {
	const Status::Result result = lhs.result > rhs.result ?
		lhs.result : rhs.result;

	return {result, lhs.outerTransition || rhs.outerTransition};
}

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_PLANS

template <typename TArgs>
class ConstPlanT {
	template <typename>
	friend class ControlT;

	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	static constexpr LongIndex TASK_CAPACITY = Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using TaskLinks		= typename PlanData::TaskLinks;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Iterator {
		HFSM2_INLINE Iterator(const ConstPlanT& plan);

		HFSM2_INLINE explicit operator bool() const;

		HFSM2_INLINE void operator ++();

		HFSM2_INLINE const TaskLink& operator  *() const							{ return  _plan._planData.taskLinks[_curr];							}
		HFSM2_INLINE const TaskLink* operator ->() const							{ return &_plan._planData.taskLinks[_curr];							}

		HFSM2_INLINE LongIndex next() const;

		const ConstPlanT& _plan;
		LongIndex _curr;
		LongIndex _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	HFSM2_INLINE ConstPlanT(const PlanData& planData,
							const RegionID regionId);

	template <typename T>
	static constexpr StateID  stateId()												{ return			StateList ::template index<T>();				}

	template <typename T>
	static constexpr RegionID regionId()											{ return (RegionID) RegionList::template index<T>();				}

public:
	HFSM2_INLINE explicit operator bool() const;

	HFSM2_INLINE Iterator first()													{ return Iterator{*this};											}

private:
	const PlanData& _planData;
	const Bounds& _bounds;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class PlanT {
	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	static constexpr LongIndex  TASK_CAPACITY	= Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using TaskLinks		= typename PlanData::TaskLinks;
	using TaskIndex		= typename TaskLinks::Index;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Iterator {
		HFSM2_INLINE Iterator(PlanT& plan);

		HFSM2_INLINE explicit operator bool() const;

		HFSM2_INLINE void operator ++();

		HFSM2_INLINE	   TaskLink& operator  *()									{ return  _plan._planData.taskLinks[_curr];							}
		HFSM2_INLINE const TaskLink& operator  *() const							{ return  _plan._planData.taskLinks[_curr];							}

		HFSM2_INLINE	   TaskLink* operator ->()									{ return &_plan._planData.taskLinks[_curr];							}
		HFSM2_INLINE const TaskLink* operator ->() const							{ return &_plan._planData.taskLinks[_curr];							}

		HFSM2_INLINE void remove();

		HFSM2_INLINE LongIndex next() const;

		PlanT& _plan;
		LongIndex _curr;
		LongIndex _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	HFSM2_INLINE PlanT(PlanData& planData,
					   const RegionID regionId);

	template <typename T>
	static constexpr StateID  stateId()												{ return			StateList ::template index<T>();				}

	template <typename T>
	static constexpr RegionID regionId()											{ return (RegionID) RegionList::template index<T>();				}

	bool append(const TransitionType transitionType,
				const StateID origin,
				const StateID destination);

public:
	HFSM2_INLINE explicit operator bool() const;

	/// @brief Clear all tasks from the plan
	HFSM2_INLINE void clear();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Append a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, acts depending on the region type)
	/// @param origin Transition origin state identifier
	/// @param destination Transition destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_INLINE bool change   (const StateID origin, const StateID destination)	{ return append(TransitionType::CHANGE,	   origin, destination);	}

	/// @brief Append a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the initial state)
	/// @param origin Transition origin state identifier
	/// @param destination Transition destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_INLINE bool restart  (const StateID origin, const StateID destination)	{ return append(TransitionType::RESTART,   origin, destination);	}

	/// @brief Append a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state that was active previously)
	/// @param origin Transition origin state identifier
	/// @param destination Transition destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_INLINE bool resume   (const StateID origin, const StateID destination)	{ return append(TransitionType::RESUME,	   origin, destination);	}

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	/// @brief Append a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state with the highest 'utility()' among those with the highest 'rank()')
	/// @param origin Transition origin state identifier
	/// @param destination Transition destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE bool utilize  (const StateID origin, const StateID destination)	{ return append(TransitionType::UTILIZE,   origin, destination);	}

	/// @brief Append a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, uses weighted random to activate the state proportional to 'utility()' among those with the highest 'rank()')
	/// @param origin Transition origin state identifier
	/// @param destination Transition destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE bool randomize(const StateID origin, const StateID destination)	{ return append(TransitionType::RANDOMIZE, origin, destination);	}

#endif

	/// @brief Append a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @param origin Transition origin state identifier
	/// @param destination Transition destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_INLINE bool schedule (const StateID origin, const StateID destination)	{ return append(TransitionType::SCHEDULE,  origin, destination);	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Append a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, acts depending on the region type)
	/// @tparam TOrigin Transition origin state type
	/// @param destination Transition destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_INLINE bool change   (const StateID destination)							{ return change   (stateId<TOrigin>(), destination);				}

	/// @brief Append a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the initial state)
	/// @tparam TOrigin Transition origin state type
	/// @param destination Transition destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_INLINE bool restart  (const StateID destination)							{ return restart  (stateId<TOrigin>(), destination);				}

	/// @brief Append a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state that was active previously)
	/// @tparam TOrigin Transition origin state type
	/// @param destination Transition destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_INLINE bool resume   (const StateID destination)							{ return resume   (stateId<TOrigin>(), destination);				}

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	/// @brief Append a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TOrigin Transition origin state type
	/// @param destination Transition destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin>
	HFSM2_INLINE bool utilize  (const StateID destination)							{ return utilize  (stateId<TOrigin>(), destination);				}

	/// @brief Append a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, uses weighted random to activate the state proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TOrigin Transition origin state type
	/// @param destination Transition destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin>
	HFSM2_INLINE bool randomize(const StateID destination)							{ return randomize(stateId<TOrigin>(), destination);				}

#endif

	/// @brief Append a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Transition origin state type
	/// @param destination Transition destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_INLINE bool schedule (const StateID destination)							{ return schedule (stateId<TOrigin>(), destination);				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Append a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, acts depending on the region type)
	/// @tparam TOrigin Transition origin state type
	/// @tparam TDestination Transition destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool change   ()													{ return change   (stateId<TOrigin>(), stateId<TDestination>());	}

	/// @brief Append a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the initial state)
	/// @tparam TOrigin Transition origin state type
	/// @tparam TDestination Transition destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool restart  ()													{ return restart  (stateId<TOrigin>(), stateId<TDestination>());	}

	/// @brief Append a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state that was active previously)
	/// @tparam TOrigin Transition origin state type
	/// @tparam TDestination Transition destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool resume   ()													{ return resume   (stateId<TOrigin>(), stateId<TDestination>());	}

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	/// @brief Append a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, activates the state with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TOrigin Transition origin state type
	/// @tparam TDestination Transition destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool utilize  ()													{ return utilize  (stateId<TOrigin>(), stateId<TDestination>());	}

	/// @brief Append a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///		(if transitioning into a region, uses weighted random to activate the state proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TOrigin Transition origin state type
	/// @tparam TDestination Transition destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool randomize()													{ return randomize(stateId<TOrigin>(), stateId<TDestination>());	}

#endif

	/// @brief Append a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Transition origin state type
	/// @tparam TDestination Transition destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_INLINE bool schedule ()													{ return schedule (stateId<TOrigin>(), stateId<TDestination>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Begin iteration over plan tasks for the current region
	/// @return
	HFSM2_INLINE Iterator first()													{ return Iterator{*this};											}

private:
	void remove(const LongIndex task);

private:
	PlanData& _planData;
	const RegionID _regionId;
	Bounds& _bounds;
};

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#include "plan.inl"
