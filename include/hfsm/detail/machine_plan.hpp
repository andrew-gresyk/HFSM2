namespace hfsm2 {
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

struct Bounds {
	LongIndex first = INVALID_LONG_INDEX;
	LongIndex last  = INVALID_LONG_INDEX;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct TaskLink {
	//using PayloadList	= TPayloadList;
	//using Payload		= typename PayloadList::Container;

	inline TaskLink(const StateID origin_,
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

template <typename>
class ControlT;

template <typename>
class FullControlT;

template <typename TStateList, ShortIndex NRegionCapacity, LongIndex NTaskCapacity>
class PlanT {
	template <typename>
	friend class ControlT;

	template <typename>
	friend class FullControlT;

	using StateList			= TStateList;

	static constexpr ShortIndex REGION_CAPACITY	= NRegionCapacity;
	static constexpr LongIndex  TASK_CAPACITY	= NTaskCapacity;

public:
	using TaskLinks			= List<TaskLink, TASK_CAPACITY>;
	using TasksBounds		= Array<Bounds, REGION_CAPACITY>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Iterator {
		inline Iterator(PlanT& plan);

		inline explicit operator bool() const;

		inline void operator ++();

		inline		 TaskLink& operator  *()	   { return  _plan._taskLinks[_curr]; }
		inline const TaskLink& operator  *() const { return  _plan._taskLinks[_curr]; }

		inline		 TaskLink* operator ->()	   { return &_plan._taskLinks[_curr]; }
		inline const TaskLink* operator ->() const { return &_plan._taskLinks[_curr]; }

		inline void remove();

		inline LongIndex next() const;

		PlanT& _plan;
		LongIndex _curr;
		LongIndex _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	inline PlanT(TaskLinks& tasks,
				 TasksBounds& tasksBounds,
				 const RegionID regionId);

	template <typename T>
	static constexpr LongIndex stateId()	{ return StateList::template index<T>();	}

public:
	inline explicit operator bool() const;

	inline void clear();

	void add(const StateID origin, const StateID destination);

	template <typename TOrigin, typename TDestination>
	inline void add()				{ add(stateId<TOrigin>(), stateId<TDestination>());	}

	void remove(const LongIndex task);

	inline Iterator begin()	{ return Iterator{*this}; }

private:
	TaskLinks& _taskLinks;
	Bounds& _bounds;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_plan.inl"
