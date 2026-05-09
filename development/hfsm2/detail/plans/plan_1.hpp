#if HFSM2_PLANS_AVAILABLE()

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class PlanT {
	template <typename, typename, Strategy, typename, typename...>
	friend struct C_;

	template <typename, typename, typename, typename...>
	friend struct O_;

	using Args			= TArgs;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using Registry		= RegistryT<Args>;

	static constexpr Long  TASK_CAPACITY	= Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using Task			= typename PlanData::Task;
	using Tasks			= typename PlanData::Tasks;
	using Bounds		= typename Tasks::Bounds;
	using TaskIndex		= typename Tasks::Index;
	using TasksBits		= typename PlanData::TasksBits;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct CIterator final {
		HFSM2_CONSTEXPR(14)	CIterator(const PlanT& plan)				noexcept;

		HFSM2_CONSTEXPR(14)	explicit operator bool()			  const noexcept;

		HFSM2_CONSTEXPR(14)	void operator ++()							noexcept;

		HFSM2_CONSTEXPR(14)	bool operator != (const CIterator)	  const noexcept	{ return operator bool();					}

		HFSM2_CONSTEXPR(14)	const Task& operator  *()			  const noexcept	{ return  _plan._planData.tasks[_curr];		}
		HFSM2_CONSTEXPR(11)	const Task* operator ->()			  const noexcept	{ return &_plan._planData.tasks[_curr];		}

		HFSM2_CONSTEXPR(14)	TaskIndex next()					  const noexcept;

		const PlanT& _plan;
		TaskIndex _curr;
		TaskIndex _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Iterator final {
		HFSM2_CONSTEXPR(14)	Iterator(PlanT& plan)						noexcept;

		HFSM2_CONSTEXPR(14)	explicit operator bool()			  const noexcept;

		HFSM2_CONSTEXPR(14)	void operator ++()							noexcept;

		HFSM2_CONSTEXPR(14)	bool operator != (const Iterator)	  const noexcept	{ return operator bool();					}

		HFSM2_CONSTEXPR(14)	Task& operator  *()							noexcept	{ return  _plan._planData.tasks[_curr];		}
		HFSM2_CONSTEXPR(14)	Task* operator ->()							noexcept	{ return &_plan._planData.tasks[_curr];		}

		HFSM2_CONSTEXPR(14)	void remove()								noexcept	{ _plan.remove(_curr);						}

		HFSM2_CONSTEXPR(14)	TaskIndex next()					  const noexcept;

		PlanT& _plan;
		TaskIndex _curr;
		TaskIndex _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	HFSM2_CONSTEXPR(11)	PlanT(Registry& registry,
							  PlanData& planData,
							  const RegionID regionId_)					noexcept;

	template <typename TState>
	static
	HFSM2_CONSTEXPR(11)  StateID  stateId()								noexcept	{ return					   index<StateList , TState>();		}

	template <typename TState>
	static
	HFSM2_CONSTEXPR(11) RegionID regionId()								noexcept	{ return static_cast<RegionID>(index<RegionList, TState>());	}

	HFSM2_CONSTEXPR(14)	bool  append(const StateID origin,
									 const StateID destination,
									 const TransitionType type)			noexcept;

	HFSM2_CONSTEXPR(14)	void clearTasks()								noexcept;
	HFSM2_CONSTEXPR(14)	void clearStatuses()							noexcept;

public:
	HFSM2_CONSTEXPR(14)	explicit operator bool()				  const noexcept;

	/// @brief Clear all tasks from the plan
	HFSM2_CONSTEXPR(14)	void clear()									noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, acts depending on the region type)
	/// @param `origin` Origin state identifier
	/// @param `destination` Destination state identifier
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool change   (const StateID origin,
									   const StateID destination)		noexcept	{ return append(origin            , destination            , TransitionType::CHANGE   );	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool change   (const StateID destination)		noexcept	{ return append(stateId<TOrigin>(), destination            , TransitionType::CHANGE   );	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam `TOrigin` Origin state type
	/// @tparam `TDestination` Destination state type
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool change   ()								noexcept	{ return append(stateId<TOrigin>(), stateId<TDestination>(), TransitionType::CHANGE   );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the initial state)
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool restart  (const StateID origin,
									   const StateID destination)		noexcept	{ return append(origin            , destination            , TransitionType::RESTART  );	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the initial state)
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool restart  (const StateID destination)		noexcept	{ return append(stateId<TOrigin>(), destination            , TransitionType::RESTART  );	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the initial state)
	/// @tparam `TOrigin` Origin state type
	/// @tparam `TDestination` Destination state type
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool restart  ()								noexcept	{ return append(stateId<TOrigin>(), stateId<TDestination>(), TransitionType::RESTART  );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool resume   (const StateID origin,
									   const StateID destination)		noexcept	{ return append(origin            , destination            , TransitionType::RESUME   );	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool resume   (const StateID destination)		noexcept	{ return append(stateId<TOrigin>(), destination            , TransitionType::RESUME   );	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam `TOrigin` Origin state type
	/// @tparam `TDestination` Destination state type
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool resume   ()								noexcept	{ return append(stateId<TOrigin>(), stateId<TDestination>(), TransitionType::RESUME   );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the sub-state by index returned by the region's `select()` method)
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool select   (const StateID origin,
									   const StateID destination)		noexcept	{ return append(origin            , destination            , TransitionType::SELECT   );	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the sub-state by index returned by the region's `select()` method)
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool select   (const StateID destination)		noexcept	{ return append(stateId<TOrigin>(), destination            , TransitionType::SELECT   );	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the sub-state by index returned by the region's `select()` method)
	/// @tparam `TOrigin` Origin state type
	/// @tparam `TDestination` Destination state type
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool select   ()								noexcept	{ return append(stateId<TOrigin>(), stateId<TDestination>(), TransitionType::SELECT   );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the state with the highest `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	HFSM2_CONSTEXPR(14)	bool utilize  (const StateID origin,
									   const StateID destination)		noexcept	{ return append(origin            , destination            , TransitionType::UTILIZE  );	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the state with the highest `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool utilize  (const StateID destination)		noexcept	{ return append(stateId<TOrigin>(), destination            , TransitionType::UTILIZE  );	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, activates the state with the highest `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TOrigin` Origin state type
	/// @tparam `TDestination` Destination state type
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool utilize  ()								noexcept	{ return append(stateId<TOrigin>(), stateId<TDestination>(), TransitionType::UTILIZE  );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, uses weighted random to activate the state proportional to `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	HFSM2_CONSTEXPR(14)	bool randomize(const StateID origin,
									   const StateID destination)		noexcept	{ return append(origin            , destination            , TransitionType::RANDOMIZE);	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, uses weighted random to activate the state proportional to `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool randomize(const StateID destination)		noexcept	{ return append(stateId<TOrigin>(), destination            , TransitionType::RANDOMIZE);	}

	/// @brief Append a task to transition from `origin` to `destination` if `origin` completes with `success()`
	///   (if transitioning into a region, uses weighted random to activate the state proportional to `utility()`
	///   among those with the highest `rank()`)
	/// @tparam `TOrigin` Origin state type
	/// @tparam `TDestination` Destination state type
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	/// @see `HFSM2_ENABLE_UTILITY_THEORY`
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool randomize()								noexcept	{ return append(stateId<TOrigin>(), stateId<TDestination>(), TransitionType::RANDOMIZE);	}
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Append a task to schedule a transition to `destination` if `origin` completes with `success()`
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool schedule (const StateID origin,
									   const StateID destination)		noexcept	{ return append(origin            , destination            , TransitionType::SCHEDULE );	}

	/// @brief Append a task to schedule a transition to `destination` if `origin` completes with `success()`
	/// @tparam `TOrigin` Origin state type
	/// @param `destination` Destination state identifier
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool schedule (const StateID destination)		noexcept	{ return append(stateId<TOrigin>(), destination            , TransitionType::SCHEDULE );	}

	/// @brief Append a task to schedule a transition to `destination` if `origin` completes with `success()`
	/// @tparam `TOrigin` Origin state type
	/// @tparam `TDestination` Destination state type
	/// @return Success if FSM total number of tasks is below task capacity
	/// @note use `Config::TaskCapacityN<>` to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool schedule ()								noexcept	{ return append(stateId<TOrigin>(), stateId<TDestination>(), TransitionType::SCHEDULE );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Begin iteration over plan tasks
	/// @return Iterator to the first task
	HFSM2_CONSTEXPR(14)	 Iterator begin()								noexcept	{ return  Iterator{*this};	}

	/// @brief Begin iteration over plan tasks
	/// @return CIterator to the first task
	HFSM2_CONSTEXPR(11)	CIterator begin()						  const noexcept	{ return CIterator{*this};	}

private:
	HFSM2_CONSTEXPR(14)	void remove(const TaskIndex task)				noexcept;

protected:
	Registry& _registry;
	PlanData& _planData;
	const RegionID _regionId;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#include "plan_1.inl"
