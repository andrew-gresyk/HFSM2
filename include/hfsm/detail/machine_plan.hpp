namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct Status {
	bool success = false;
	bool failure = false;
	bool innerTransition = false;
	bool outerTransition = false;

	inline Status(const bool success_ = false,
				  const bool failure_ = false,
				  const bool innerTransition_ = false,
				  const bool outerTransition_ = false)
		: success(success_)
		, failure(failure_)
		, innerTransition(innerTransition_)
		, outerTransition(outerTransition_)
	{}

	inline explicit operator bool() const { return success || failure || innerTransition || outerTransition; }
};

inline Status
combine(const Status lhs, const Status rhs) {
	return Status{lhs.success || rhs.success,
				  lhs.failure || rhs.failure,
				  lhs.innerTransition || rhs.innerTransition,
				  lhs.outerTransition || rhs.outerTransition};
}

#pragma pack(pop)

//------------------------------------------------------------------------------

#pragma pack(push, 2)

struct TaskIndices {
	LongIndex first = INVALID_LONG_INDEX;
	LongIndex last  = INVALID_LONG_INDEX;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct TaskTransition {
	//using PayloadList	= TPayloadList;
	//using Payload		= typename PayloadList::Container;

	inline TaskTransition(const StateID origin_,
						  const StateID destination_)
		: origin(origin_)
		, destination(destination_)
		, next(INVALID_LONG_INDEX)
	{}

	StateID origin		= INVALID_STATE_ID;
	StateID destination	= INVALID_STATE_ID;
	// TODO: add payload

	LongIndex next		= INVALID_LONG_INDEX;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

template <typename, typename, LongIndex>
class PlanControlT;

template <typename, typename, typename, LongIndex>
class FullControlT;

template <typename TStateList, LongIndex NPlanCapacity>
class PlanT {
	template <typename, typename, LongIndex>
	friend class PlanControlT;

	template <typename, typename, typename, LongIndex>
	friend class FullControlT;

	using StateList			= TStateList;

	static constexpr LongIndex STATE_COUNT	 = StateList::SIZE;
	static constexpr LongIndex PLAN_CAPACITY = NPlanCapacity;

	using Tasks				= List<TaskTransition, PLAN_CAPACITY>;
	using StateTasks		= Array<TaskIndices,   STATE_COUNT>;

private:
	inline PlanT(Tasks& tasks,
				 StateTasks& stateTasks,
				 const StateID plannerId)
		: _tasks(tasks)
		, _taskIndices(stateTasks[plannerId])
	{}

	template <typename T>
	static constexpr LongIndex
	stateId()	{ return StateList::template index<T>();	}

public:
	inline void clear();

	void add(const StateID origin, const StateID destination);

	template <typename TOrigin, typename TDestination>
	inline void add();

	const TaskTransition* next() const;

	void advance();

private:
	Tasks& _tasks;
	TaskIndices& _taskIndices;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_plan.inl"
