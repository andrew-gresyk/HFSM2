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

//------------------------------------------------------------------------------

template <typename>
class ControlT;

template <typename>
class FullControlT;

template <typename TArgs>
class PlanT {
	template <typename>
	friend class ControlT;

	template <typename>
	friend class FullControlT;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using PayloadList	= typename Args::PayloadList;

	static constexpr LongIndex  TASK_CAPACITY	= Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using TaskLinks		= typename PlanData::TaskLinks;
	using RegionBit		= typename PlanData::RegionBit;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Iterator {
		HSFM_INLINE Iterator(PlanT& plan);

		HSFM_INLINE explicit operator bool() const;

		HSFM_INLINE void operator ++();

		HSFM_INLINE		  TaskLink& operator  *()	    { return  _plan._planData.taskLinks[_curr];	}
		HSFM_INLINE const TaskLink& operator  *() const { return  _plan._planData.taskLinks[_curr];	}

		HSFM_INLINE		  TaskLink* operator ->()	    { return &_plan._planData.taskLinks[_curr];	}
		HSFM_INLINE const TaskLink* operator ->() const { return &_plan._planData.taskLinks[_curr];	}

		HSFM_INLINE void remove();

		HSFM_INLINE LongIndex next() const;

		PlanT& _plan;
		LongIndex _curr;
		LongIndex _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	HSFM_INLINE PlanT(PlanData& planData,
					  const RegionID regionId);

	template <typename T>
	static constexpr LongIndex stateId()	{ return StateList ::template index<T>();	}

	template <typename T>
	static constexpr LongIndex regionId()	{ return RegionList::template index<T>();	}

public:
	HSFM_INLINE explicit operator bool() const;

	HSFM_INLINE void clear();

	void add(const StateID origin,
			 const StateID destination);

	template <typename TOrigin, typename TDestination>
	HSFM_INLINE void add()			{ add(stateId<TOrigin>(), stateId<TDestination>());	}

	void remove(const LongIndex task);

	HSFM_INLINE Iterator begin()			{ return Iterator{*this};					}

private:
	PlanData& _planData;
	RegionBit _planExists;
	Bounds& _bounds;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_plan.inl"
