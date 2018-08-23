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
				  const bool outerTransition_ = false);

	inline explicit operator bool() const { return success || failure || innerTransition || outerTransition; }

	inline void clear();
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

	// TODO: add payload
	StateID origin		= INVALID_STATE_ID;
	StateID destination	= INVALID_STATE_ID;

	LongIndex prev		= INVALID_LONG_INDEX;
	LongIndex next		= INVALID_LONG_INDEX;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

template <typename, typename, LongIndex, LongIndex>
class PlanControlT;

template <typename, typename, LongIndex, typename, LongIndex>
class FullControlT;

template <typename TStateList, LongIndex NPlanCapacity>
class PlanT {
	template <typename, typename, LongIndex, LongIndex>
	friend class PlanControlT;

	template <typename, typename, LongIndex, typename, LongIndex>
	friend class FullControlT;

	using StateList			= TStateList;

	static constexpr LongIndex CAPACITY		= NPlanCapacity;
	static constexpr LongIndex STATE_COUNT	= StateList::SIZE;

	using Tasks				= List<TaskTransition, CAPACITY>;
	using StateTasks		= Array<TaskIndices,   STATE_COUNT>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	struct Iterator {
		inline Iterator(PlanT& plan);

		inline explicit operator bool() const;

		inline void operator ++();

		inline		 TaskTransition& operator  *()		 { return  _plan._tasks[_curr]; }
		inline const TaskTransition& operator  *() const { return  _plan._tasks[_curr]; }

		inline		 TaskTransition* operator ->()		 { return &_plan._tasks[_curr]; }
		inline const TaskTransition* operator ->() const { return &_plan._tasks[_curr]; }

		inline void remove();

		inline LongIndex next() const;

		PlanT& _plan;
		LongIndex _curr;
		LongIndex _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	inline PlanT(Tasks& tasks,
				 StateTasks& stateTasks,
				 const StateID plannerId);

	template <typename T>
	static constexpr LongIndex
	stateId()	{ return StateList::template index<T>();	}

public:
	inline explicit operator bool() const;

	inline void clear();

	void add(const StateID origin, const StateID destination);

	template <typename TOrigin, typename TDestination>
	inline void add();

	void remove(const LongIndex task);

	inline Iterator begin()	{ return Iterator{*this}; }

private:
	Tasks& _tasks;
	TaskIndices& _taskIndices;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_plan.inl"
