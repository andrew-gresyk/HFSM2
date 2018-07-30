namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TPayloadList,
		  ShortIndex TMaxSubstitutions,
		  typename TApex>
class _R final {
	static constexpr ShortIndex MAX_SUBSTITUTIONS = TMaxSubstitutions;

	using Context			= TContext;
	using Control			= ControlT<Context>;
	using PayloadList		= TPayloadList;
	using Payload			= typename PayloadList::Container;
	using Transition		= TransitionT<PayloadList>;

	using ForwardApex		= typename WrapForward<TApex>::Type;

	using StateList			= typename ForwardApex::StateList;
	using TransitionControl	= TransitionControlT<Context, StateList, PayloadList>;

	using Apex				= typename WrapMaterial<0, Context, StateList, PayloadList, TApex>::Type;

public:
	static constexpr LongIndex REVERSE_DEPTH  = ForwardApex::REVERSE_DEPTH;
	static constexpr LongIndex DEEP_WIDTH	  = ForwardApex::DEEP_WIDTH;
	static constexpr LongIndex STATE_COUNT	  = ForwardApex::STATE_COUNT;
	static constexpr LongIndex FORK_COUNT	  = ForwardApex::FORK_COUNT;
	static constexpr LongIndex PRONG_COUNT	  = ForwardApex::PRONG_COUNT;
	static constexpr LongIndex WIDTH		  = ForwardApex::WIDTH;

	static_assert(STATE_COUNT <  (ShortIndex) -1, "Too many states in the hierarchy. Change 'ShortIndex' type.");
	static_assert(STATE_COUNT == (ShortIndex) StateList::SIZE, "STATE_COUNT != StateList::SIZE");

private:
	using StateRegistry			 = Array<Parent,	 STATE_COUNT>;
	using TransitionPayloads	 = Array<Payload,	 STATE_COUNT>;

	using ForkParents			 = Array<Parent,	 FORK_COUNT>;
	using ForkPointerStorage	 = Array<Fork*,		 FORK_COUNT>;
	using TransitionQueueStorage = Array<Transition, FORK_COUNT>;

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static constexpr LongIndex NAME_COUNT	  = Apex::NAME_COUNT;

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

	inline bool isActive(const StateID stateId) const;

	inline bool isResumable(const StateID stateId) const;

	inline bool isScheduled(const StateID stateId) const	{ return isResumable(stateId);						}

	template <typename T>
	inline bool isActive() const;

	template <typename T>
	inline bool isResumable() const;

	template <typename T>
	inline bool isScheduled() const							{ return isResumable<T>();							}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	const MachineStructure& structure() const				{ return _structure;								}
	const MachineActivity&  activity()  const				{ return _activityHistory;							}
#endif

#ifdef HFSM_ENABLE_LOG_INTERFACE
	void attachLogger(LoggerInterface* const logger)		{ _logger = logger;									}
#endif

protected:
	void processTransitions();
	void requestImmediate(const Transition request);
	void requestScheduled(const Transition request);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	void getStateNames();
	void udpateActivity();
#endif

private:
	Context& _context;

	StateRegistry _stateRegistry;
	TransitionPayloads _transitionPayloads;

	ForkParents  _forkParents;
	ForkPointerStorage _forkPointers;

	TransitionQueueStorage _requests;

	Apex _apex;

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
