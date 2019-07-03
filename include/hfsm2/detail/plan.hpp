namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct Status {
	bool success = false;
	bool failure = false;
	bool outerTransition = false;

	inline Status(const bool success_ = false,
				  const bool failure_ = false,
				  const bool outerTransition_ = false);

	inline explicit operator bool() const { return success || failure || outerTransition; }

	inline void clear();
};

#pragma pack(pop)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Status
combine(const Status lhs, const Status rhs) {
	return Status{lhs.success || rhs.success,
				  lhs.failure || rhs.failure,
				  lhs.outerTransition || rhs.outerTransition};
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
	using Payload		= typename Args::Payload;

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

	HFSM_INLINE Iterator begin()			{ return Iterator{*this};								}

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
	using Payload		= typename Args::Payload;

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

	TaskIndex append(const Transition transition,
					 const StateID origin,
					 const StateID destination);

public:
	HFSM_INLINE explicit operator bool() const;

	HFSM_INLINE void clear();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE TaskIndex change   (const StateID origin, const StateID destination)	{ return append(Transition::CHANGE,	   origin, destination); }
	HFSM_INLINE TaskIndex restart  (const StateID origin, const StateID destination)	{ return append(Transition::RESTART,   origin, destination); }
	HFSM_INLINE TaskIndex resume   (const StateID origin, const StateID destination)	{ return append(Transition::RESUME,	   origin, destination); }
	HFSM_INLINE TaskIndex utilize  (const StateID origin, const StateID destination)	{ return append(Transition::UTILIZE,   origin, destination); }
	HFSM_INLINE TaskIndex randomize(const StateID origin, const StateID destination)	{ return append(Transition::RANDOMIZE, origin, destination); }
	HFSM_INLINE TaskIndex schedule (const StateID origin, const StateID destination)	{ return append(Transition::SCHEDULE,  origin, destination); }

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TOrigin>
	HFSM_INLINE TaskIndex change   (const StateID destination)					{ return change   (stateId<TOrigin>(), destination);				}

	template <typename TOrigin>
	HFSM_INLINE TaskIndex restart  (const StateID destination)					{ return restart  (stateId<TOrigin>(), destination);				}

	template <typename TOrigin>
	HFSM_INLINE TaskIndex resume   (const StateID destination)					{ return resume   (stateId<TOrigin>(), destination);				}

	template <typename TOrigin>
	HFSM_INLINE TaskIndex utilize  (const StateID destination)					{ return utilize  (stateId<TOrigin>(), destination);				}

	template <typename TOrigin>
	HFSM_INLINE TaskIndex randomize(const StateID destination)					{ return randomize(stateId<TOrigin>(), destination);				}

	template <typename TOrigin>
	HFSM_INLINE TaskIndex schedule (const StateID destination)					{ return schedule (stateId<TOrigin>(), destination);				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TOrigin, typename TDestination>
	HFSM_INLINE TaskIndex change   ()											{ return change   (stateId<TOrigin>(), stateId<TDestination>());	}

	template <typename TOrigin, typename TDestination>
	HFSM_INLINE TaskIndex restart  ()											{ return restart  (stateId<TOrigin>(), stateId<TDestination>());	}

	template <typename TOrigin, typename TDestination>
	HFSM_INLINE TaskIndex resume   ()											{ return resume   (stateId<TOrigin>(), stateId<TDestination>());	}

	template <typename TOrigin, typename TDestination>
	HFSM_INLINE TaskIndex utilize  ()											{ return utilize  (stateId<TOrigin>(), stateId<TDestination>());	}

	template <typename TOrigin, typename TDestination>
	HFSM_INLINE TaskIndex randomize()											{ return randomize(stateId<TOrigin>(), stateId<TDestination>());	}

	template <typename TOrigin, typename TDestination>
	HFSM_INLINE TaskIndex schedule ()											{ return schedule (stateId<TOrigin>(), stateId<TDestination>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	void remove(const LongIndex task);

	HFSM_INLINE Iterator begin()			{ return Iterator{*this};								}

private:
	PlanData& _planData;
	const RegionID _regionId;
	Bounds& _bounds;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "plan.inl"
