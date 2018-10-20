namespace hfsm2 {
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
	using RegionList			 = typename ForwardApex::RegionList;
	using Forward				 = _RF<Context, Config, PayloadList, Apex>;

	static constexpr LongIndex MAX_PLAN_TASKS	 = Config::MAX_PLAN_TASKS;
	static constexpr LongIndex MAX_SUBSTITUTIONS = Config::MAX_SUBSTITUTIONS;

	static constexpr LongIndex TASK_CAPACITY	 = Forward::TASK_CAPACITY;

public:
	static constexpr LongIndex  REVERSE_DEPTH	 = ForwardApex::REVERSE_DEPTH;
	static constexpr LongIndex  DEEP_WIDTH		 = ForwardApex::DEEP_WIDTH;
	static constexpr ShortIndex COMPO_COUNT		 = ForwardApex::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT		 = ForwardApex::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS		 = ForwardApex::ORTHO_UNITS;
	static constexpr LongIndex  PRONG_COUNT		 = ForwardApex::PRONG_COUNT;

	static constexpr LongIndex  STATE_COUNT		 = ForwardApex::STATE_COUNT;
	static constexpr LongIndex  REGION_COUNT	 = ForwardApex::REGION_COUNT;

	static_assert(STATE_COUNT <  (ShortIndex) -1, "Too many states in the hierarchy. Change 'ShortIndex' type.");
	static_assert(STATE_COUNT == (ShortIndex) StateList::SIZE, "STATE_COUNT != StateList::SIZE");

private:
	using Args					 = ArgsT<Context,
										 Config,
										 StateList,
										 RegionList,
										 COMPO_COUNT,
										 ORTHO_COUNT,
										 ORTHO_UNITS,
										 PayloadList,
										 TASK_CAPACITY>;

	using StateData				 = StateDataT  <Args>;
	using PlanData				 = PlanDataT   <Args>;

	using Control				 = ControlT	   <Args>;
	using FullControl			 = FullControlT<Args>;

	using PayloadBox			 = typename PayloadList::Variant;
	using Payloads				 = Array<PayloadBox, STATE_COUNT>;
	using Request				 = typename FullControl::Request;
	using Requests				 = typename FullControl::Requests;

	using MaterialApex			 = typename WrapMaterial<0, 0, 0, Args, Apex>::Type;

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static constexpr LongIndex NAME_COUNT	  = MaterialApex::NAME_COUNT;

	using Prefix				 = StaticArray<wchar_t,			REVERSE_DEPTH * 2 + 2>;
	using Prefixes				 = StaticArray<Prefix,			STATE_COUNT>;

	using StateInfoStorage		 = Array<StructureStateInfo,	STATE_COUNT>;

	using StructureStorage		 = Array<StructureEntry,		NAME_COUNT>;
	using ActivityHistoryStorage = Array<char,					NAME_COUNT>;

	using TransitionInfo		 = TransitionInfoT<PayloadList>;
	using TransitionInfoStorage	 = Array<TransitionInfo,		COMPO_COUNT * 2>;
#endif

public:
	_R(Context& context
	   HFSM_IF_LOGGER(, LoggerInterface* const logger = nullptr));

	~_R();

	template <typename T>
	static constexpr LongIndex
	stateId()													{ return StateList::template index<T>();			}

	void update();

	template <typename TEvent>
	HSFM_INLINE void react(const TEvent& event);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HSFM_INLINE bool isActive   (const StateID stateId) const	{ return _stateData.isActive   (stateId);			}
	HSFM_INLINE bool isResumable(const StateID stateId) const	{ return _stateData.isResumable(stateId);			}

	HSFM_INLINE bool isScheduled(const StateID stateId) const	{ return isResumable(stateId);						}

	template <typename TState>
	HSFM_INLINE bool isActive   () const						{ return isActive	(stateId<TState>());			}

	template <typename TState>
	HSFM_INLINE bool isResumable() const						{ return isResumable(stateId<TState>());			}

	template <typename TState>
	HSFM_INLINE bool isScheduled() const						{ return isResumable<TState>();						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HSFM_INLINE void changeTo(const StateID stateId);
	HSFM_INLINE void resume	 (const StateID stateId);
	HSFM_INLINE void schedule(const StateID stateId);

	template <typename TState>
	HSFM_INLINE void changeTo()									{ changeTo(stateId<TState>());						}

	template <typename TState>
	HSFM_INLINE void resume	()									{ resume  (stateId<TState>());						}

	template <typename TState>
	HSFM_INLINE void schedule()									{ schedule(stateId<TState>());						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TPayload>
	HSFM_INLINE void changeTo(const StateID stateId, TPayload* const payload);

	template <typename TPayload>
	HSFM_INLINE void resume  (const StateID stateId, TPayload* const payload);

	template <typename TPayload>
	HSFM_INLINE void schedule(const StateID stateId, TPayload* const payload);

	template <typename TState, typename TPayload>
	HSFM_INLINE void changeTo(TPayload* const payload)			{ changeTo(stateId<TState>(), payload);				}

	template <typename TState, typename TPayload>
	HSFM_INLINE void resume	(TPayload* const payload)			{ resume  (stateId<TState>(), payload);				}

	template <typename TState, typename TPayload>
	HSFM_INLINE void schedule(TPayload* const payload)			{ schedule(stateId<TState>(), payload);				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HSFM_INLINE void resetStateData(const StateID stateId);

	template <typename TPayload>
	HSFM_INLINE void setStateData  (const StateID stateId, TPayload* const payload);

	HSFM_INLINE bool isStateDataSet(const StateID stateId) const;

	template <typename TPayload>
	HSFM_INLINE TPayload* getStateData(const StateID stateId) const;

	template <typename TState>
	HSFM_INLINE void resetStateData()							{ resetStateData(stateId<TState>());				}

	template <typename TState, typename TPayload>
	HSFM_INLINE void setStateData  (TPayload* const payload)	{ setStateData  (stateId<TState>(), payload);		}

	template <typename TState>
	HSFM_INLINE bool isStateDataSet() const						{ return isStateDataSet(stateId<TState>());			}

	template <typename TState, typename TPayload>
	HSFM_INLINE TPayload* getStateData() const					{ return getStateData<TPayload>(stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	const MachineStructure& structure() const					{ return _structure;								}
	const MachineActivity&  activity()  const					{ return _activityHistory;							}
#endif

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	void attachLogger(LoggerInterface* const logger)			{ _logger = logger;									}
#endif

protected:
	void processTransitions();

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	void getStateNames();
	void udpateActivity();
#endif

private:
	Context& _context;

	StateData _stateData;
	PlanData _planData;

	Payloads _requestPayloads;
	Requests _requests;

	MaterialApex _apex;

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
