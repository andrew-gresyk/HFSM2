namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#if HFSM2_PLANS_AVAILABLE()

template <typename TArgs>
class CPlanT {
	template <typename>
	friend class ControlT;

	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	template <typename, typename>
	friend class R_;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	static constexpr Long TASK_CAPACITY = Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using Task			= typename PlanData::Task;
	using TaskLinks		= typename PlanData::TaskLinks;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct IteratorT final {
		HFSM2_CONSTEXPR(14)	IteratorT(const CPlanT& plan)					noexcept;

		HFSM2_CONSTEXPR(14)	explicit operator bool()				  const noexcept;

		HFSM2_CONSTEXPR(14)	void operator ++()								noexcept;

		HFSM2_CONSTEXPR(11)	const Task& operator  *()				  const noexcept	{ return  _plan._planData.tasks[_curr];		}
		HFSM2_CONSTEXPR(11)	const Task* operator ->()				  const noexcept	{ return &_plan._planData.tasks[_curr];		}

		HFSM2_CONSTEXPR(14)	Long next()								  const noexcept;

		const CPlanT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	HFSM2_CONSTEXPR(11)	CPlanT(const PlanData& planData,
							   const RegionID regionId_)					noexcept
		: _planData{planData}
		, _bounds{planData.tasksBounds[regionId_]}
	{}

	template <typename TState>
	static constexpr StateID  stateId()										noexcept	{ return					   index<StateList , TState>() ;	}

	template <typename TState>
	static constexpr RegionID regionId()									noexcept	{ return static_cast<RegionID>(index<RegionList, TState>());	}

public:
	HFSM2_CONSTEXPR(14)	explicit operator bool()					  const noexcept;

	HFSM2_CONSTEXPR(14)	IteratorT first()									noexcept	{ return IteratorT{*this};					}

private:
	const PlanData& _planData;
	const Bounds& _bounds;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class PlanBaseT {
	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using Registry		= RegistryT<Args>;

	static constexpr Long  TASK_CAPACITY	= Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using Task			= typename PlanData::Task;
	using Tasks			= typename PlanData::Tasks;
	using TaskLinks		= typename PlanData::TaskLinks;
	using TaskIndex		= typename TaskLinks::Index;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct CIteratorT final {
		HFSM2_CONSTEXPR(14)	CIteratorT(const PlanBaseT& plan)				noexcept;

		HFSM2_CONSTEXPR(14)	explicit operator bool()				  const noexcept;

		HFSM2_CONSTEXPR(14)	void operator ++()								noexcept;

		HFSM2_CONSTEXPR(14)	const Task& operator  *()				  const noexcept	{ return  _plan._planData.tasks[_curr];		}
		HFSM2_CONSTEXPR(11)	const Task* operator ->()				  const noexcept	{ return &_plan._planData.tasks[_curr];		}

		HFSM2_CONSTEXPR(14)	Long next()								  const noexcept;

		const PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct IteratorT final {
		HFSM2_CONSTEXPR(14)	IteratorT(PlanBaseT& plan)						noexcept;

		HFSM2_CONSTEXPR(14)	explicit operator bool()				  const noexcept;

		HFSM2_CONSTEXPR(14)	void operator ++()								noexcept;

		HFSM2_CONSTEXPR(14)	Task& operator  *()								noexcept	{ return  _plan._planData.tasks[_curr];		}
		HFSM2_CONSTEXPR(14)	Task* operator ->()								noexcept	{ return &_plan._planData.tasks[_curr];		}

		HFSM2_CONSTEXPR(14)	void remove()									noexcept	{ _plan.remove(_curr);						}

		HFSM2_CONSTEXPR(14)	Long next()								  const noexcept;

		PlanBaseT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	HFSM2_CONSTEXPR(11)	PlanBaseT(Registry& registry,
								  PlanData& planData,
								  const RegionID regionId_)					noexcept;

	template <typename TState>
	static constexpr StateID  stateId()										noexcept	{ return					   index<StateList , TState>() ;	}

	template <typename TState>
	static constexpr RegionID regionId()									noexcept	{ return static_cast<RegionID>(index<RegionList, TState>());	}

	HFSM2_CONSTEXPR(14)	bool append(const StateID origin,
									const StateID destination,
									const TransitionType transitionType)	noexcept;

	HFSM2_CONSTEXPR(14)	bool linkTask(const Long index)						noexcept;

	HFSM2_CONSTEXPR(14)	void clearTasks()									noexcept;

public:
	HFSM2_CONSTEXPR(14)	explicit operator bool()					  const noexcept;

	/// @brief Clear all tasks from the plan
	HFSM2_CONSTEXPR(14)	void clear()										noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, acts depending on the region type)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool change   (const StateID origin, const StateID destination)		noexcept	{ return append	  (origin, destination, TransitionType::CHANGE	 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool change   (const StateID destination)							noexcept	{ return change	  (stateId<TOrigin>() , destination				 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool change   ()													noexcept	{ return change	  (stateId<TOrigin>() , stateId<TDestination>()	 );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the initial state)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool restart  (const StateID origin, const StateID destination)		noexcept	{ return append	  (origin, destination, TransitionType::RESTART	 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the initial state)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool restart  (const StateID destination)							noexcept	{ return restart  (stateId<TOrigin>() , destination				 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the initial state)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool restart  ()													noexcept	{ return restart  (stateId<TOrigin>() , stateId<TDestination>()	 );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool resume   (const StateID origin, const StateID destination)		noexcept	{ return append	  (origin, destination, TransitionType::RESUME	 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool resume   (const StateID destination)							noexcept	{ return resume	  (stateId<TOrigin>() , destination				 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool resume   ()													noexcept	{ return resume	  (stateId<TOrigin>() , stateId<TDestination>()	 );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool select   (const StateID origin, const StateID destination)		noexcept	{ return append	  (origin, destination, TransitionType::SELECT	 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool select   (const StateID destination)							noexcept	{ return select	  (stateId<TOrigin>() , destination				 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool select   ()													noexcept	{ return select	  (stateId<TOrigin>() , stateId<TDestination>()	 );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	bool utilize  (const StateID origin, const StateID destination) 	noexcept	{ return append	  (origin, destination, TransitionType::UTILIZE	 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool utilize  (const StateID destination)							noexcept	{ return utilize  (stateId<TOrigin>() , destination				 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool utilize  ()													noexcept	{ return utilize  (stateId<TOrigin>() , stateId<TDestination>()	 );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	bool randomize(const StateID origin, const StateID destination)		noexcept	{ return append	  (origin, destination, TransitionType::RANDOMIZE);	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool randomize(const StateID destination)							noexcept	{ return randomize(stateId<TOrigin>() , destination				 );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool randomize()													noexcept	{ return randomize(stateId<TOrigin>() , stateId<TDestination>()	 );	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool schedule (const StateID origin, const StateID destination)		noexcept	{ return append	  (origin, destination, TransitionType::SCHEDULE );	}

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool schedule (const StateID destination)							noexcept	{ return schedule (stateId<TOrigin>() , destination				 );	}

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool schedule ()													noexcept	{ return schedule (stateId<TOrigin>() , stateId<TDestination>()	 );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Begin iteration over plan tasks for the current region
	/// @return IteratorT to the first task
	HFSM2_CONSTEXPR(14)	 IteratorT first()													noexcept	{ return  IteratorT{*this};											}

	/// @brief Begin iteration over plan tasks
	/// @return CIteratorT to the first task
	HFSM2_CONSTEXPR(11)	CIteratorT first()											  const noexcept	{ return CIteratorT{*this};											}

private:
	HFSM2_CONSTEXPR(14)	void remove(const Long task)										noexcept;

protected:
	Registry& _registry;
	PlanData& _planData;
	const RegionID _regionId;
	Bounds& _bounds;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class PlanT;

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
class PlanT<ArgsT<TContext
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
	: public PlanBaseT<ArgsT<TContext
						   , TConfig
						   , TStateList
						   , TRegionList
						   , NCompoCount
						   , NOrthoCount
						   , NOrthoUnits
						   HFSM2_IF_SERIALIZATION(, NSerialBits)
						   , NSubstitutionLimit
						   , NTaskCapacity
						   , TPayload>>
{
	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	template <typename, typename>
	friend class R_;

	using Args = ArgsT<TContext
					 , TConfig
					 , TStateList
					 , TRegionList
					 , NCompoCount
					 , NOrthoCount
					 , NOrthoUnits
					 HFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , TPayload>;

	using Payload		= typename Args::Payload;

	using PlanBase		= PlanBaseT<Args>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using PlanBase::PlanBase;

	using PlanBase::linkTask;

	HFSM2_CONSTEXPR(14)	bool append(const StateID origin,
									const StateID destination,
									const TransitionType transitionType,
									const Payload& payload)										noexcept;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, acts depending on the region type)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool changeWith   (const StateID origin, const StateID destination, const Payload& payload) noexcept	{ return append(origin								 , destination								 , TransitionType::CHANGE   ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool changeWith   (						 const StateID destination, const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::CHANGE   ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, acts depending on the region type)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool changeWith   (													const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::CHANGE   ,	   payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the initial state)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool restartWith  (const StateID origin, const StateID destination, const Payload& payload) noexcept	{ return append(origin								 , destination								 , TransitionType::RESTART  ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the initial state)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool restartWith  (						 const StateID destination, const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::RESTART  ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the initial state)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool restartWith  (													const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::RESTART  ,	   payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state that was active previously)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool resumeWith   (const StateID origin, const StateID destination, const Payload& payload) noexcept	{ return append(origin								 , destination								 , TransitionType::RESUME   ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool resumeWith   (						 const StateID destination, const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::RESUME   ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state that was active previously)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool resumeWith   (													const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::RESUME   ,	   payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool selectWith   (const StateID origin, const StateID destination, const Payload& payload) noexcept	{ return append(origin								 , destination								 , TransitionType::SELECT   ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool selectWith   (						 const StateID destination, const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::SELECT   ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the sub-state by index returned by the region's 'select()' method)
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool selectWith   (													const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::SELECT   ,	   payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	bool utilizeWith  (const StateID origin, const StateID destination, const Payload& payload) noexcept	{ return append(origin								 , destination								 , TransitionType::UTILIZE  ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool utilizeWith  (						 const StateID destination, const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::UTILIZE  ,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, activates the state with the highest 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool utilizeWith  (													const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::UTILIZE  ,	   payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_CONSTEXPR(14)	bool randomizeWith(const StateID origin, const StateID destination, const Payload& payload) noexcept	{ return append(origin								 , destination								 , TransitionType::RANDOMIZE,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool randomizeWith(						 const StateID destination, const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::RANDOMIZE,	   payload );	}

	/// @brief Add a task to transition from 'origin' to 'destination' if 'origin' completes with 'success()'
	///   (if transitioning into a region, uses weighted random to activate the state proportional to 'utility()'
	///   among those with the highest 'rank()')
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool randomizeWith(													const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::RANDOMIZE,	   payload );	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @param origin Origin state identifier
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	HFSM2_CONSTEXPR(14)	bool scheduleWith (const StateID origin, const StateID destination, const Payload& payload) noexcept	{ return append(origin								 , destination								 , TransitionType::SCHEDULE ,	   payload );	}

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @param destination Destination state identifier
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin>
	HFSM2_CONSTEXPR(14)	bool scheduleWith (						 const StateID destination, const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), destination								 , TransitionType::SCHEDULE ,	   payload );	}

	/// @brief Add a task to schedule a transition to 'destination' if 'origin' completes with 'success()'
	/// @tparam TOrigin Origin state type
	/// @tparam TDestination Destination state type
	/// @param payload Payload
	/// @return Seccess if FSM total number of tasks is below task capacity
	/// @note use 'Config::TaskCapacityN<>' to increase task capacity if necessary
	template <typename TOrigin, typename TDestination>
	HFSM2_CONSTEXPR(14)	bool scheduleWith (													const Payload& payload) noexcept	{ return append(PlanBase::template stateId<TOrigin>(), PlanBase::template stateId<TDestination>(), TransitionType::SCHEDULE ,	   payload );	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	using PlanBase::_planData;
	using PlanBase::_regionId;
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
class PlanT<ArgsT<TContext
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
	: public PlanBaseT<ArgsT<TContext
						   , TConfig
						   , TStateList
						   , TRegionList
						   , NCompoCount
						   , NOrthoCount
						   , NOrthoUnits
						   HFSM2_IF_SERIALIZATION(, NSerialBits)
						   , NSubstitutionLimit
						   , NTaskCapacity
						   , void>>
{
	template <typename, typename>
	friend class R_;

	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	using Args = ArgsT<TContext
					 , TConfig
					 , TStateList
					 , TRegionList
					 , NCompoCount
					 , NOrthoCount
					 , NOrthoUnits
					 HFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 , NTaskCapacity
					 , void>;

	using PlanBase = PlanBaseT<Args>;

	using PlanBase::PlanBase;
};

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#include "plan.inl"
