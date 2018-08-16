#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename>
struct ControlOriginT;

template <typename TContext>
class ControlT {
	template <StateID, typename, typename>
	friend struct _S;

	template <StateID, typename, typename>
	friend struct _P;

	template <StateID, typename, typename, typename...>
	friend struct _C;

	template <StateID, typename, typename, typename...>
	friend struct _Q;

	template <StateID, typename, typename, typename...>
	friend struct _O;

	template <typename, typename, typename, typename>
	friend class _R;

	template <typename>
	friend struct ControlOriginT;

protected:
	using Context = TContext;

	inline ControlT(Context& context,
					LoggerInterface* const HFSM_IF_LOGGER(logger))
		: _context(context)
		HFSM_IF_LOGGER(, _logger(logger))
	{}

	inline void setOrigin  (const StateID id);
	inline void resetOrigin(const StateID id);

public:
	inline Context& _()									{ return _context;		}
	inline Context& context()							{ return _context;		}

private:
#ifdef HFSM_ENABLE_LOG_INTERFACE
	inline LoggerInterface* logger()					{ return _logger;		}
#endif

protected:
	Context& _context;
	StateID _originId = INVALID_STATE_ID;
	HFSM_IF_LOGGER(LoggerInterface* _logger);
};

//------------------------------------------------------------------------------

template <typename TContext>
struct ControlOriginT {
	using Context = TContext;
	using Control = ControlT<Context>;

	inline ControlOriginT(Control& control,
						  const StateID id);

	inline ~ControlOriginT();

	Control& control;
	const StateID prevId;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TContext, typename TStateList, LongIndex NPlanCapacity>
class PlanControlT final
	: public ControlT<TContext>
{
	using Context			= TContext;
	using StateList			= TStateList;

	static constexpr LongIndex STATE_COUNT = StateList::SIZE;
	static constexpr LongIndex PLAN_CAPACITY  = NPlanCapacity;

	using Control			= ControlT<Context>;
	using Plan				= PlanT<StateList, PLAN_CAPACITY>;
	using Tasks				= typename Plan::Tasks;
	using StateTasks		= typename Plan::StateTasks;

	template <typename, typename, typename, typename>
	friend class _R;

private:
	inline PlanControlT(Context& context,
						Tasks& tasks,
						StateTasks& stateTasks,
						LoggerInterface* const logger)
		: Control(context, logger)
		, _tasks(tasks)
		, _stateTasks(stateTasks)
	{}

	template <typename T>
	static constexpr LongIndex
	stateId()										{ return StateList::template index<T>();					}

public:
	inline Plan plan()								{ return Plan(_tasks, _stateTasks, Control::_originId);		}
	inline Plan plan() const						{ return Plan(_tasks, _stateTasks, Control::_originId);		}

	inline Plan plan(const StateID stateId)			{ return Plan(_tasks, _stateTasks, stateId);				}
	inline Plan plan(const StateID stateId) const	{ return Plan(_tasks, _stateTasks, stateId);				}

	template <typename TPlanner>
	inline Plan plan()								{ return Plan(_tasks, _stateTasks, stateId<TPlanner>());	}

	template <typename TPlanner>
	inline Plan plan()						const	{ return Plan(_tasks, _stateTasks, stateId<TPlanner>());	}

private:
	Tasks& _tasks;
	StateTasks& _stateTasks;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TPayloadList>
struct TransitionT {
	using PayloadList = TPayloadList;
	using Payload = typename PayloadList::Container;

	enum Type {
		REMAIN,
		RESTART,
		RESUME,
		SCHEDULE,

		COUNT
	};

	template <typename T>
	static constexpr bool contains() {
		return PayloadList::template contains<T>();
	}

	inline TransitionT() = default;

	inline TransitionT(const Type type_,
					   const StateID stateId_)
		: type(type_)
		, stateId(stateId_)
	{
		assert(type_ < Type::COUNT);
	}

	template <typename T,
			  typename = typename std::enable_if<contains<T>(), T>::type>
	inline TransitionT(const Type type_,
					   const StateID stateId_,
					   T* const payload_)
		: type(type_)
		, stateId(stateId_)
		, payload(payload_)
	{
		assert(type_ < Type::COUNT);
	}

	Type type = RESTART;
	StateID stateId = INVALID_STATE_ID;
	Payload payload;
};

template <typename TPayloadList>
using TransitionQueueT = ArrayView<TransitionT<TPayloadList>>;

//------------------------------------------------------------------------------

template <typename, typename, typename>
struct ControlLockT;

template <typename, typename, typename>
struct ControlRegionT;

template <typename TContext, typename TStateList, typename TPayloadList>
class TransitionControlT
	: public ControlT<TContext>
{
protected:
	using Context			= TContext;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;

	using Control			= ControlT<Context>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using TransitionQueue	= TransitionQueueT<TPayloadList>;

	template <StateID, typename, typename>
	friend struct _S;

	template <StateID, typename, typename>
	friend struct _P;

	template <typename, typename, typename, typename>
	friend class _R;

	template <typename, typename, typename>
	friend struct ControlLockT;

	template <typename, typename, typename>
	friend struct ControlRegionT;

protected:
	inline TransitionControlT(Context& context,
							  TransitionQueue& requests,
							  LoggerInterface* const logger)
		: Control(context, logger)
		, _requests(requests)
	{}

public:
	template <typename T>
	static constexpr LongIndex
	stateId()						{ return StateList::template index<T>();	}

	inline void changeTo(const StateID stateId);
	inline void resume	(const StateID stateId);
	inline void schedule(const StateID stateId);

	template <typename TState>
	inline void changeTo()						{ changeTo(stateId<TState>());	}

	template <typename TState>
	inline void resume()						{ resume  (stateId<TState>());	}

	template <typename TState>
	inline void schedule()						{ schedule(stateId<TState>());	}

	inline void succeed()						{ _status.success = true;		}
	inline void fail()							{ _status.failure = true;		}

private:
	inline void setRegion  (const StateID id, const LongIndex size);
	inline void resetRegion(const StateID id, const LongIndex size);

private:
	TransitionQueue& _requests;
	Status _status;
	bool _locked = false;
	StateID _regionId = INVALID_STATE_ID;
	LongIndex _regionSize = INVALID_LONG_INDEX;
};

//------------------------------------------------------------------------------

template <typename TContext, typename TStateList, typename TPayloadList>
struct ControlLockT {
	using Context			= TContext;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;
	using TransitionControl = TransitionControlT<Context, StateList, PayloadList>;

	inline ControlLockT(TransitionControl& control);
	inline ~ControlLockT();

	TransitionControl* const control;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TContext, typename TStateList, typename TPayloadList>
struct ControlRegionT {
	using Context			= TContext;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;
	using TransitionControl = TransitionControlT<Context, StateList, PayloadList>;

	inline ControlRegionT(TransitionControl& control,
						  const StateID id,
						  const LongIndex size);

	inline ~ControlRegionT();

	TransitionControl& control;
	const StateID prevId;
	const LongIndex prevSize;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TContext, typename TStateList, typename TPayloadList, LongIndex NPlanCapacity>
class FullControlT final
	: public TransitionControlT<TContext, TStateList, TPayloadList>
{
	using Context			= TContext;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;

	static constexpr LongIndex STATE_COUNT = StateList::SIZE;
	static constexpr LongIndex PLAN_CAPACITY  = NPlanCapacity;

	using TransitionControl	= TransitionControlT<Context, StateList, PayloadList>;
	using TransitionQueue	= typename TransitionControl::TransitionQueue;
	using Plan				= PlanT<StateList, PLAN_CAPACITY>;
	using Tasks				= typename Plan::Tasks;
	using StateTasks		= typename Plan::StateTasks;

	template <typename, typename, typename, typename>
	friend class _R;

private:
	inline FullControlT(Context& context,
						TransitionQueue& requests,
						Tasks& tasks,
						StateTasks& firstTasks,
						LoggerInterface* const logger)
		: TransitionControl(context, requests, logger)
		, _tasks(tasks)
		, _stateTasks(firstTasks)
	{}

	template <typename T>
	static constexpr LongIndex
	stateId()									  { return StateList::template index<T>();					}

public:
	inline Plan plan(const StateID stateId)		  { return Plan(_tasks, _stateTasks, stateId);				}

	inline Plan plan(const StateID stateId) const { return Plan(_tasks, _stateTasks, stateId);				}

	template <typename TPlanner>
	inline Plan plan()							  { return Plan(_tasks, _stateTasks, stateId<TPlanner>());	}

	template <typename TPlanner>
	inline Plan plan()						const { return Plan(_tasks, _stateTasks, stateId<TPlanner>());	}

private:
	Tasks& _tasks;
	StateTasks& _stateTasks;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_control.inl"
