#if HFSM2_PLANS_AVAILABLE()

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

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

	struct Iterator final {
		HFSM2_CONSTEXPR(14)	Iterator(const CPlanT& plan)				noexcept;

		HFSM2_CONSTEXPR(14)	explicit operator bool()			  const noexcept;

		HFSM2_CONSTEXPR(14)	void operator ++()							noexcept;

		HFSM2_CONSTEXPR(11)	bool operator != (const Iterator)	  const noexcept	{ return operator bool();					}

		HFSM2_CONSTEXPR(11)	const Task& operator  *()			  const noexcept	{ return  _plan._planData.tasks[_curr];		}
		HFSM2_CONSTEXPR(11)	const Task* operator ->()			  const noexcept	{ return &_plan._planData.tasks[_curr];		}

		HFSM2_CONSTEXPR(14)	Long next()							  const noexcept;

		const CPlanT& _plan;
		Long _curr;
		Long _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	HFSM2_CONSTEXPR(11)	CPlanT(const PlanData& planData,
							   const RegionID regionId_)				noexcept
		: _planData{planData}
		, _bounds{planData.tasksBounds[regionId_]}
	{}

	template <typename TState>
	static
	HFSM2_CONSTEXPR(11)  StateID  stateId()								noexcept	{ return					   index<StateList , TState>();		}

	template <typename TState>
	static
	HFSM2_CONSTEXPR(11)	RegionID regionId()								noexcept	{ return static_cast<RegionID>(index<RegionList, TState>());	}

public:
	HFSM2_CONSTEXPR(14)	explicit operator bool()				  const noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Begin iteration over plan tasks
	/// @return CIterator to the first task
	HFSM2_CONSTEXPR(14)	Iterator begin()								noexcept	{ return Iterator{*this};					}

	/// @brief Iteration terminator
	/// @return Dummy Iterator
	HFSM2_CONSTEXPR(14)	Iterator end  ()								noexcept	{ return Iterator{*this};					}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief First task
	/// @return First task
	HFSM2_CONSTEXPR(14) const Task& first()						  const noexcept;

	/// @brief Last task
	/// @return Last task
	HFSM2_CONSTEXPR(14) const Task&  last()						  const noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	const PlanData& _planData;
	const Bounds& _bounds;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

#include "plan_0.inl"
