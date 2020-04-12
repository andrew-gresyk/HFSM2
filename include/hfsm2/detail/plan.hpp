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

	inline explicit operator bool() const { return result || outerTransition; }

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
		HFSM_INLINE Iterator(const ConstPlanT& plan);

		HFSM_INLINE explicit operator bool() const;

		HFSM_INLINE void operator ++();

		HFSM_INLINE const TaskLink& operator  *() const { return  _plan._planData.taskLinks[_curr];	}
		HFSM_INLINE const TaskLink* operator ->() const { return &_plan._planData.taskLinks[_curr];	}

		HFSM_INLINE LongIndex next() const;

		const ConstPlanT& _plan;
		LongIndex _curr;
		LongIndex _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	HFSM_INLINE ConstPlanT(const PlanData& planData,
						   const RegionID regionId);

	template <typename T>
	static constexpr StateID  stateId()		{ return			StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()	{ return (RegionID) RegionList::template index<T>();	}

public:
	HFSM_INLINE explicit operator bool() const;

	HFSM_INLINE Iterator first()			{ return Iterator{*this};								}

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
		HFSM_INLINE Iterator(PlanT& plan);

		HFSM_INLINE explicit operator bool() const;

		HFSM_INLINE void operator ++();

		HFSM_INLINE		  TaskLink& operator  *()	    { return  _plan._planData.taskLinks[_curr];	}
		HFSM_INLINE const TaskLink& operator  *() const { return  _plan._planData.taskLinks[_curr];	}

		HFSM_INLINE		  TaskLink* operator ->()	    { return &_plan._planData.taskLinks[_curr];	}
		HFSM_INLINE const TaskLink* operator ->() const { return &_plan._planData.taskLinks[_curr];	}

		HFSM_INLINE void remove();

		HFSM_INLINE LongIndex next() const;

		PlanT& _plan;
		LongIndex _curr;
		LongIndex _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	HFSM_INLINE PlanT(PlanData& planData,
					  const RegionID regionId);

	template <typename T>
	static constexpr StateID  stateId()		{ return			StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()	{ return (RegionID) RegionList::template index<T>();	}

	bool append(const Transition transition,
				const StateID origin,
				const StateID destination);

public:
	HFSM_INLINE explicit operator bool() const;

	HFSM_INLINE void clear();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool change   (const StateID origin, const StateID destination)	{ return append(Transition::CHANGE,	   origin, destination); }
	HFSM_INLINE bool restart  (const StateID origin, const StateID destination)	{ return append(Transition::RESTART,   origin, destination); }
	HFSM_INLINE bool resume   (const StateID origin, const StateID destination)	{ return append(Transition::RESUME,	   origin, destination); }
	HFSM_INLINE bool utilize  (const StateID origin, const StateID destination)	{ return append(Transition::UTILIZE,   origin, destination); }
	HFSM_INLINE bool randomize(const StateID origin, const StateID destination)	{ return append(Transition::RANDOMIZE, origin, destination); }
	HFSM_INLINE bool schedule (const StateID origin, const StateID destination)	{ return append(Transition::SCHEDULE,  origin, destination); }

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TOrigin>
	HFSM_INLINE bool change   (const StateID destination)					{ return change   (stateId<TOrigin>(), destination);				}

	template <typename TOrigin>
	HFSM_INLINE bool restart  (const StateID destination)					{ return restart  (stateId<TOrigin>(), destination);				}

	template <typename TOrigin>
	HFSM_INLINE bool resume   (const StateID destination)					{ return resume   (stateId<TOrigin>(), destination);				}

	template <typename TOrigin>
	HFSM_INLINE bool utilize  (const StateID destination)					{ return utilize  (stateId<TOrigin>(), destination);				}

	template <typename TOrigin>
	HFSM_INLINE bool randomize(const StateID destination)					{ return randomize(stateId<TOrigin>(), destination);				}

	template <typename TOrigin>
	HFSM_INLINE bool schedule (const StateID destination)					{ return schedule (stateId<TOrigin>(), destination);				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TOrigin, typename TDestination>
	HFSM_INLINE bool change   ()											{ return change   (stateId<TOrigin>(), stateId<TDestination>());	}

	template <typename TOrigin, typename TDestination>
	HFSM_INLINE bool restart  ()											{ return restart  (stateId<TOrigin>(), stateId<TDestination>());	}

	template <typename TOrigin, typename TDestination>
	HFSM_INLINE bool resume   ()											{ return resume   (stateId<TOrigin>(), stateId<TDestination>());	}

	template <typename TOrigin, typename TDestination>
	HFSM_INLINE bool utilize  ()											{ return utilize  (stateId<TOrigin>(), stateId<TDestination>());	}

	template <typename TOrigin, typename TDestination>
	HFSM_INLINE bool randomize()											{ return randomize(stateId<TOrigin>(), stateId<TDestination>());	}

	template <typename TOrigin, typename TDestination>
	HFSM_INLINE bool schedule ()											{ return schedule (stateId<TOrigin>(), stateId<TDestination>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE Iterator first()			{ return Iterator{*this};								}

private:
	void remove(const LongIndex task);

private:
	PlanData& _planData;
	const RegionID _regionId;
	Bounds& _bounds;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "plan.inl"
