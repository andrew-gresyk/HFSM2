namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TPayloadList,
		  ShortIndex TMaxSubstitutions,
		  typename TApex>
class _R final {
	enum : ShortIndex { MAX_SUBSTITUTIONS = TMaxSubstitutions };

	using Context			= TContext;
	using Control			= ControlT<Context>;
	using PayloadList		= TPayloadList;
	using StateRegistryBase	= StateRegistryBaseT<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionControl	= TransitionControlT<Context, PayloadList>;
	using Apex				= typename WrapState<Context, PayloadList, TApex>::Type;

public:
	using StateList			= typename Apex::StateList;
	HSFM_IF_DEBUG(StateList stateList);

public:
	enum : LongIndex {
		REVERSE_DEPTH			 = Apex::REVERSE_DEPTH,
		DEEP_WIDTH				 = Apex::DEEP_WIDTH,
		STATE_COUNT				 = Apex::STATE_COUNT,
		FORK_COUNT				 = Apex::FORK_COUNT,
		PRONG_COUNT				 = Apex::PRONG_COUNT,
		WIDTH					 = Apex::WIDTH,
	};
	static_assert(STATE_COUNT <  (ShortIndex) -1, "Too many states in the hierarchy. Change 'ShortIndex' type.");
	static_assert(STATE_COUNT == (ShortIndex) StateList::SIZE, "STATE_COUNT != StateList::SIZE");

private:
	enum : LongIndex {
		STATE_CAPACITY = (LongIndex) 1.3 * Apex::STATE_COUNT,
	};

	using StateRegistryImpl		 = StateRegistryT<PayloadList, STATE_CAPACITY>;
	using ForkParentStorage		 = Array<Parent,	 FORK_COUNT>;
	using ForkPointerStorage	 = Array<Fork*,		 FORK_COUNT>;
	using TransitionQueueStorage = Array<Transition, FORK_COUNT>;

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	enum : LongIndex {
		NAME_COUNT	  = Apex::NAME_COUNT,
	};

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

	void update();

	template <typename TEvent>
	inline void react(const TEvent& event);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	inline void changeTo(const std::type_index state);
	inline void resume	(const std::type_index state);
	inline void schedule(const std::type_index state);

	template <typename TState>
	inline typename std::enable_if<StateList::template contains<TState>()>::type
	changeTo()												{ changeTo(typeid(TState));		}

	template <typename TState>
	inline typename std::enable_if<StateList::template contains<TState>()>::type
	resume	()												{ resume  (typeid(TState));		}

	template <typename TState>
	inline typename std::enable_if<StateList::template contains<TState>()>::type
	schedule()												{ schedule(typeid(TState));		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TPayload>
	inline typename std::enable_if<TPayloadList::template contains<TPayload>()>::type
	changeTo(const std::type_index state, TPayload* const payload);

	template <typename TPayload>
	inline typename std::enable_if<TPayloadList::template contains<TPayload>()>::type
	resume(const std::type_index state, TPayload* const payload);

	template <typename TPayload>
	inline typename std::enable_if<TPayloadList::template contains<TPayload>()>::type
	schedule(const std::type_index state, TPayload* const payload);

	template <typename TState, typename TPayload>
	inline typename std::enable_if<StateList::template contains<TState>() &&
								 PayloadList::template contains<TPayload>()>::type
	changeTo(TPayload* const payload)				{ changeTo(typeid(TState), payload);	}

	template <typename TState, typename TPayload>
	inline typename std::enable_if<StateList::template contains<TState>() &&
								 PayloadList::template contains<TPayload>()>::type
	resume	(TPayload* const payload)				{ resume  (typeid(TState), payload);	}

	template <typename TState, typename TPayload>
	inline typename std::enable_if<StateList::template contains<TState>() &&
								 PayloadList::template contains<TPayload>()>::type
	schedule(TPayload* const payload)				{ schedule(typeid(TState), payload);	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	inline void resetStateData(const std::type_index state);

	inline bool isStateDataSet(const std::type_index state);

	template <typename TPayload>
	inline typename std::enable_if<TPayloadList::template contains<TPayload>()>::type
	setStateData(const std::type_index state, TPayload* const payload);

	template <typename TPayload>
	inline typename std::enable_if<TPayloadList::template contains<TPayload>(), TPayload>::type*
	getStateData(const std::type_index state);

	template <typename TState>
	inline typename std::enable_if<StateList::template contains<TState>()>::type
	resetStateData()							{ resetStateData(typeid(TState));			}

	template <typename TState>
	inline typename std::enable_if<StateList::template contains<TState>(), bool>::type
	isStateDataSet()							{ return isStateDataSet(typeid(TState));	}

	template <typename TState, typename TPayload>
	inline typename std::enable_if<StateList::template contains<TState>() &&
								 PayloadList::template contains<TPayload>()>::type
	setStateData(TPayload* const payload)		{ setStateData(typeid(TState), payload);	}

	template <typename TState, typename TPayload>
	inline typename std::enable_if<StateList::template contains<TState>() &&
								 PayloadList::template contains<TPayload>(), TPayload>::type*
	getStateData()						{ return getStateData<TPayload>(typeid(TState));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	inline bool isActive   (const std::type_index state);
	inline bool isResumable(const std::type_index state);
	inline bool isScheduled(const std::type_index state)	{ return isResumable(state);	}

	template <typename TState>
	inline typename std::enable_if<StateList::template contains<TState>(), bool>::type
	isActive()										{ return isActive	(typeid(TState));	}

	template <typename TState>
	inline typename std::enable_if<StateList::template contains<TState>(), bool>::type
	isResumable()									{ return isResumable(typeid(TState));	}

	template <typename TState>
	inline typename std::enable_if<StateList::template contains<TState>(), bool>::type
	isScheduled()									{ return isScheduled(typeid(TState));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	const MachineStructure& structure() const					{ return _structure;		}
	const MachineActivity&  activity()  const					{ return _activityHistory;	}
#endif

#ifdef HFSM_ENABLE_LOG_INTERFACE
	void attachLogger(LoggerInterface* const logger)					{ _logger = logger;	}
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

	StateRegistryImpl _stateRegistry;

	ForkParentStorage  _forkParents;
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
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_root.inl"
