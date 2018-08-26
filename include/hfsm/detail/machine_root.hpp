namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TConfig,
		  typename TPayloadList,
		  typename TApex>
class _R final {
	using Context				 = TContext;
	using Config				 = TConfig;
	using PayloadList			 = TPayloadList;
	using Apex					 = TApex;

	using ForwardApex			 = typename WrapForward<Apex>::Type;
	using StateList				 = typename ForwardApex::StateList;
	using Forward				 = _RF<Context, Config, PayloadList, Apex>;

	static constexpr LongIndex MAX_PLAN_TASKS	 = Config::MAX_PLAN_TASKS;
	static constexpr LongIndex MAX_SUBSTITUTIONS = Config::MAX_SUBSTITUTIONS;

	static constexpr LongIndex PLAN_CAPACITY	 = Forward::PLAN_CAPACITY;

public:
	static constexpr LongIndex REVERSE_DEPTH	 = ForwardApex::REVERSE_DEPTH;
	static constexpr LongIndex DEEP_WIDTH		 = ForwardApex::DEEP_WIDTH;
	static constexpr LongIndex STATE_COUNT		 = ForwardApex::STATE_COUNT;
	static constexpr LongIndex COMPOSITE_COUNT	 = ForwardApex::COMPOSITE_COUNT;
	static constexpr LongIndex ORTHOGONAL_COUNT	 = ForwardApex::ORTHOGONAL_COUNT;
	static constexpr LongIndex PRONG_COUNT		 = ForwardApex::PRONG_COUNT;
	static constexpr LongIndex WIDTH			 = ForwardApex::WIDTH;

	static constexpr LongIndex FORK_COUNT		 = COMPOSITE_COUNT + ORTHOGONAL_COUNT;

private:
	using Args					 = ArgsT<Context, Config, StateList, COMPOSITE_COUNT, ORTHOGONAL_COUNT, PayloadList, PLAN_CAPACITY>;
	using PlanControl			 = typename Forward::PlanControl;
	using Payload				 = typename PayloadList::Container;
	using Transition			 = TransitionT<PayloadList>;
	using TransitionControl		 = typename Forward::TransitionControl;
	using FullControl			 = typename Forward::FullControl;

	static_assert(STATE_COUNT <  (ShortIndex) -1, "Too many states in the hierarchy. Change 'ShortIndex' type.");
	static_assert(STATE_COUNT == (ShortIndex) StateList::SIZE, "STATE_COUNT != StateList::SIZE");

private:
	using Registry				 = RegistryT<Args>;

	using Payloads				 = Array<Payload,		STATE_COUNT>;
	using TransitionQueueStorage = Array<Transition,	FORK_COUNT>;

	using MaterialApex			 = typename WrapMaterial<0, 0, 0, Args, Apex>::Type;
	using Tasks					 = typename FullControl::Tasks;
	using StateTasks			 = Array<TaskIndices,	STATE_COUNT>;			// TODO: change to ForkTasks

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static constexpr LongIndex NAME_COUNT	  = MaterialApex::NAME_COUNT;		// TODO: move to ForwardApex

	using Prefix				 = StaticArray<wchar_t,			REVERSE_DEPTH * 2 + 2>;
	using Prefixes				 = StaticArray<Prefix,			STATE_COUNT>;

	using StateInfoStorage		 = Array<StructureStateInfo,	STATE_COUNT>;

	using StructureStorage		 = Array<StructureEntry,		NAME_COUNT>;
	using ActivityHistoryStorage = Array<char,					NAME_COUNT>;

	using TransitionInfo		 = TransitionInfoT<PayloadList>;
	using TransitionInfoStorage	 = Array<TransitionInfo,		FORK_COUNT * 2>;
#endif

public:
	_R(Context& context
	   HFSM_IF_LOGGER(, LoggerInterface* const logger = nullptr));

	~_R();

	template <typename T>
	static constexpr LongIndex
	stateId()												{ return StateList::template index<T>();			}

	void update();

	template <typename TEvent>
	inline void react(const TEvent& event);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	inline void changeTo(const StateID stateId);
	inline void resume	(const StateID stateId);
	inline void schedule(const StateID stateId);

	template <typename TPayload>
	inline void changeTo(const StateID stateId, TPayload* const payload);

	template <typename TPayload>
	inline void resume  (const StateID stateId, TPayload* const payload);

	template <typename TPayload>
	inline void schedule(const StateID stateId, TPayload* const payload);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TState>
	inline void changeTo()									{ changeTo(stateId<TState>());						}

	template <typename TState>
	inline void resume	()									{ resume  (stateId<TState>());						}

	template <typename TState>
	inline void schedule()									{ schedule(stateId<TState>());						}

	template <typename TState, typename TPayload>
	inline void changeTo(TPayload* const payload)			{ changeTo(stateId<TState>(), payload);				}

	template <typename TState, typename TPayload>
	inline void resume	(TPayload* const payload)			{ resume  (stateId<TState>(), payload);				}

	template <typename TState, typename TPayload>
	inline void schedule(TPayload* const payload)			{ schedule(stateId<TState>(), payload);				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	inline void resetStateData(const StateID stateId);

	template <typename TPayload>
	inline void setStateData(const StateID stateId, TPayload* const payload);

	inline bool isStateDataSet(const StateID stateId) const;

	template <typename TPayload>
	inline TPayload* getStateData(const StateID stateId) const;

	template <typename TState>
	inline void resetStateData()							{ resetStateData(stateId<TState>());				}

	template <typename TState, typename TPayload>
	inline void setStateData(TPayload* const payload)		{ setStateData(stateId<TState>(), payload);			}

	template <typename TState>
	inline bool isStateDataSet() const						{ return isStateDataSet(stateId<TState>());			}

	template <typename TState, typename TPayload>
	inline TPayload* getStateData() const					{ return getStateData<TPayload>(stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	inline bool isActive   (const StateID stateId) const	{ return ::hfsm::detail::isActive	(_registry, stateId);	}
	inline bool isResumable(const StateID stateId) const	{ return ::hfsm::detail::isResumable(_registry, stateId);	}

	inline bool isScheduled(const StateID stateId) const	{ return isResumable(stateId);						}

	template <typename TState>
	inline bool isActive   () const							{ return isActive	(stateId<TState>());			}

	template <typename TState>
	inline bool isResumable() const							{ return isResumable(stateId<TState>());			}

	template <typename TState>
	inline bool isScheduled() const							{ return isResumable<TState>();						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	const MachineStructure& structure() const				{ return _structure;								}
	const MachineActivity&  activity()  const				{ return _activityHistory;							}
#endif

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	void attachLogger(LoggerInterface* const logger)		{ _logger = logger;									}
#endif

protected:
	void processTransitions();
	void requestImmediate(const Transition request);
	void requestScheduled(const Transition request);

#ifdef _DEBUG
	void verifyPlans() const;
	LongIndex verifyPlan(const StateID stateId) const;
#endif

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	void getStateNames();
	void udpateActivity();
#endif

private:
	Context& _context;

	Registry _registry;
	Tasks _tasks;
	StateTasks _stateTasks;

	Payloads _transitionPayloads;
	TransitionQueueStorage _requests;

	MaterialApex _apex;

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	Prefixes _prefixes;
	StateInfoStorage _stateInfos;

	StructureStorage _structure;
	ActivityHistoryStorage _activityHistory;

	TransitionInfoStorage _lastTransitions;
#endif

	HFSM_IF_LOGGER(LoggerInterface* _logger);
	HSFM_IF_DEBUG(StateList stateList);
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_root.inl"
