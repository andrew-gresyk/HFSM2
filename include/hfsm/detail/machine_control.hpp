#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename>
struct ControlOriginT;

template <typename TArgs>
class ControlT {
	template <StateID, typename, typename>
	friend struct _S;

	template <StateID, ShortIndex, ShortIndex, typename, typename, typename...>
	friend struct _C;

	template <StateID, ShortIndex, ShortIndex, typename, typename, typename...>
	friend struct _Q;

	template <StateID, ShortIndex, ShortIndex, typename, typename, typename...>
	friend struct _O;

	template <typename, typename, typename, typename>
	friend class _R;

	template <typename>
	friend struct ControlOriginT;

	using Args				= TArgs;
	using Context			= typename Args::Context;
	using StateList			= typename Args::StateList;

protected:
	using Registry			= RegistryT<Args>;

	inline ControlT(Context& context,
					Registry& registry,
					LoggerInterface* const HFSM_IF_LOGGER(logger))
		: _context(context)
		, _registry(registry)
		HFSM_IF_LOGGER(, _logger(logger))
	{}

	inline void setOrigin  (const StateID id);
	inline void resetOrigin(const StateID id);

	template <typename T>
	static constexpr LongIndex
	stateId()												{ return StateList::template index<T>();			}

public:
	inline Context& _()										{ return _context;									}
	inline Context& context()								{ return _context;									}

	inline bool isActive   (const StateID stateId) const	{ return ::hfsm::detail::isActive	(_registry, stateId);	}
	inline bool isResumable(const StateID stateId) const	{ return ::hfsm::detail::isResumable(_registry, stateId);	}

	inline bool isScheduled(const StateID stateId) const	{ return isResumable(stateId);						}

	template <typename TState>
	inline bool isActive() const							{ return isActive	(stateId<TState>());			}

	template <typename TState>
	inline bool isResumable() const							{ return isResumable(stateId<TState>());			}

	template <typename TState>
	inline bool isScheduled() const							{ return isResumable(stateId<TState>());			}

private:
	inline Registry& registry()								{ return _registry;									}

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	inline LoggerInterface* logger()						{ return _logger;									}
#endif

protected:
	Context& _context;
	Registry& _registry;
	StateID _originId = INVALID_STATE_ID;
	HFSM_IF_LOGGER(LoggerInterface* _logger);
};

//------------------------------------------------------------------------------

template <typename TArgs>
struct ControlOriginT {
	using Args				= TArgs;
	using Control			= ControlT<Args>;

	inline ControlOriginT(Control& control,
						  const StateID id);

	inline ~ControlOriginT();

	Control& control;
	const StateID prevId;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class PlanControlT final
	: public ControlT<TArgs>
{
	using Args				= TArgs;
	using Context			= typename Args::Context;
	using StateList			= typename Args::StateList;

public:
	using Control			= ControlT<Args>;
	using Registry			= typename Control::Registry;

	using Plan				= PlanT<StateList, Args::PLAN_CAPACITY>;
	using Tasks				= typename Plan::Tasks;
	using StateTasks		= typename Plan::StateTasks;

	template <typename, typename, typename, typename>
	friend class _R;

private:
	inline PlanControlT(Context& context,
						Registry& registry,
						Tasks& tasks,
						StateTasks& stateTasks,
						LoggerInterface* const logger)
		: Control(context, registry, logger)
		, _tasks(tasks)
		, _stateTasks(stateTasks)
	{}

	template <typename T>
	static constexpr LongIndex
	stateId()										{ return StateList::template index<T>();					}

public:
	using Control::isActive;
	using Control::isResumable;
	using Control::isScheduled;

	inline Plan plan()								{ return Plan(_tasks, _stateTasks, _originId);				}
	inline Plan plan() const						{ return Plan(_tasks, _stateTasks, _originId);				}

	inline Plan plan(const StateID stateId)			{ return Plan(_tasks, _stateTasks, stateId);				}
	inline Plan plan(const StateID stateId) const	{ return Plan(_tasks, _stateTasks, stateId);				}

	template <typename TPlanner>
	inline Plan plan()								{ return Plan(_tasks, _stateTasks, stateId<TPlanner>());	}

	template <typename TPlanner>
	inline Plan plan() const						{ return Plan(_tasks, _stateTasks, stateId<TPlanner>());	}

private:
	using Control::_originId;

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

template <typename>
struct ControlLockT;

template <typename>
struct ControlRegionT;

template <typename TArgs>
class TransitionControlT
	: public ControlT<TArgs>
{
protected:
	using Args				= TArgs;
	using Context			= typename Args::Context;
	using StateList			= typename Args::StateList;
	using PayloadList		= typename Args::PayloadList;

	using Control			= ControlT <Args>;
	using Registry			= RegistryT<Args>;

	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using TransitionQueue	= TransitionQueueT<PayloadList>;

	template <StateID, typename, typename>
	friend struct _S;

	template <typename, typename, typename, typename>
	friend class _R;

	template <typename>
	friend struct ControlLockT;

	template <typename>
	friend struct ControlRegionT;

protected:
	inline TransitionControlT(Context& context,
							  Registry& registry,
							  TransitionQueue& requests,
							  LoggerInterface* const logger)
		: Control(context, registry, logger)
		, _requests(requests)
	{}

public:
	using Control::isActive;
	using Control::isResumable;
	using Control::isScheduled;

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

protected:
	using Control::_originId;

	TransitionQueue& _requests;
	Status _status;
	bool _locked = false;
	StateID _regionId = INVALID_STATE_ID;
	LongIndex _regionSize = INVALID_LONG_INDEX;
};

//------------------------------------------------------------------------------

template <typename TArgs>
struct ControlLockT {
	using Args				= TArgs;

	using TransitionControl = TransitionControlT<Args>;

	inline ControlLockT(TransitionControl& control);
	inline ~ControlLockT();

	TransitionControl* const control;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
struct ControlRegionT {
	using Args				= TArgs;

	using TransitionControl = TransitionControlT<Args>;

	inline ControlRegionT(TransitionControl& control,
						  const StateID id,
						  const LongIndex size);

	inline ~ControlRegionT();

	TransitionControl& control;
	const StateID prevId;
	const LongIndex prevSize;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class FullControlT final
	: public TransitionControlT<TArgs>
{
	using Args				= TArgs;
	using Context			= typename Args::Context;
	using StateList			= typename Args::StateList;

	using Control			= ControlT<Args>;
	using Registry			= typename Control::Registry;

	using TransitionControl	= TransitionControlT<Args>;
	using TransitionQueue	= typename TransitionControl::TransitionQueue;
	using Plan				= PlanT<StateList, Args::PLAN_CAPACITY>;
	using Tasks				= typename Plan::Tasks;
	using StateTasks		= typename Plan::StateTasks;

	template <typename, typename, typename, typename>
	friend class _R;

private:
	inline FullControlT(Context& context,
						Registry& registry,
						TransitionQueue& requests,
						Tasks& tasks,
						StateTasks& firstTasks,
						LoggerInterface* const logger)
		: TransitionControl{context, registry, requests, logger}
		, _tasks{tasks}
		, _stateTasks{firstTasks}
	{}

	template <typename T>
	static constexpr LongIndex
	stateId()										{ return StateList::template index<T>();					}

public:
	using Control::isActive;
	using Control::isResumable;
	using Control::isScheduled;

	inline Plan plan()								{ return Plan(_tasks, _stateTasks, _originId);				}
	inline Plan plan() const						{ return Plan(_tasks, _stateTasks, _originId);				}

	inline Plan plan(const StateID stateId)			{ return Plan(_tasks, _stateTasks, stateId);				}
	inline Plan plan(const StateID stateId) const	{ return Plan(_tasks, _stateTasks, stateId);				}

	template <typename TPlanner>
	inline Plan plan()								{ return Plan(_tasks, _stateTasks, stateId<TPlanner>());	}

	template <typename TPlanner>
	inline Plan plan() const						{ return Plan(_tasks, _stateTasks, stateId<TPlanner>());	}

private:
	using Control::_originId;

	Tasks& _tasks;
	StateTasks& _stateTasks;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_control.inl"
