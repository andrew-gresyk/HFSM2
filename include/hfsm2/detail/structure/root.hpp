namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TConfig,
		  typename TPayload,
		  typename TApex>
class _R {
	using Context				 = TContext;
	using Config				 = TConfig;
	using Payload				 = TPayload;
	using Apex					 = TApex;

	using ForwardApex			 = Wrap<Apex>;
	using AllForward			 = _RF<Context, Config, Payload, Apex>;
	using StateList				 = typename AllForward::StateList;
	using RegionList			 = typename AllForward::RegionList;

	static constexpr LongIndex MAX_PLAN_TASKS	 = Config::MAX_PLAN_TASKS;
	static constexpr LongIndex MAX_SUBSTITUTIONS = Config::MAX_SUBSTITUTIONS;

	static constexpr LongIndex TASK_CAPACITY	 = AllForward::TASK_CAPACITY;

public:
	static constexpr LongIndex  REVERSE_DEPTH	 = ForwardApex::REVERSE_DEPTH;
	static constexpr ShortIndex COMPO_COUNT		 = ForwardApex::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT		 = ForwardApex::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS		 = ForwardApex::ORTHO_UNITS;
	static constexpr LongIndex  PRONG_COUNT		 = ForwardApex::PRONG_COUNT;

	static constexpr LongIndex  STATE_COUNT		 = ForwardApex::STATE_COUNT;
	static constexpr LongIndex  REGION_COUNT	 = ForwardApex::REGION_COUNT;

	static_assert(STATE_COUNT <  (ShortIndex) -1, "Too many states in the hierarchy. Change 'ShortIndex' type.");
	static_assert(STATE_COUNT == (ShortIndex) StateList::SIZE, "STATE_COUNT != StateList::SIZE");

private:
	using Args					 = typename AllForward::Args;

	using StateRegistry			 = StateRegistryT<Args>;
	using AllForks				 = typename StateRegistry::AllForks;

	using Control				 = ControlT<Args>;

	using PlanControl			 = PlanControlT<Args>;
	using PlanData				 = PlanDataT   <Args>;

	using FullControl			 = FullControlT<Args>;
	using Request				 = typename FullControl::Request;
	using Requests				 = typename FullControl::Requests;

	using GuardControl			 = GuardControlT<Args>;

	using Payloads				 = Array<Payload, STATE_COUNT>;
	using PayloadsSet			 = BitArrayStorageT<LongIndex, STATE_COUNT>;

	using MaterialApex			 = Material<0, 0, 0, Args, Apex>;

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static constexpr LongIndex NAME_COUNT	  = MaterialApex::NAME_COUNT;

	using Prefix				 = StaticArray<wchar_t,			REVERSE_DEPTH * 2 + 2>;
	using Prefixes				 = StaticArray<Prefix,			STATE_COUNT>;

	using StateInfoStorage		 = Array<StructureStateInfo,	STATE_COUNT>;

	using StructureStorage		 = Array<StructureEntry,		NAME_COUNT>;
	using ActivityHistoryStorage = Array<char,					NAME_COUNT>;

	using TransitionInfo		 = TransitionInfoT<Payload>;
	using TransitionInfoStorage	 = Array<TransitionInfo,		COMPO_COUNT * 4>;
#endif

public:
	explicit _R(Context& context
				HFSM_IF_LOGGER(, LoggerInterface* const logger = nullptr));

	~_R();

	template <typename T>
	static constexpr StateID  stateId()						{ return			StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()					{ return (RegionID) RegionList::template index<T>();	}

	void update();

	template <typename TEvent>
	HFSM_INLINE void react(const TEvent& event);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool isActive   (const StateID stateId) const	{ return _stateRegistry.isActive   (stateId);		}
	HFSM_INLINE bool isResumable(const StateID stateId) const	{ return _stateRegistry.isResumable(stateId);		}

	HFSM_INLINE bool isScheduled(const StateID stateId) const	{ return isResumable(stateId);						}

	template <typename TState>
	HFSM_INLINE bool isActive   () const						{ return isActive	(stateId<TState>());			}

	template <typename TState>
	HFSM_INLINE bool isResumable() const						{ return isResumable(stateId<TState>());			}

	template <typename TState>
	HFSM_INLINE bool isScheduled() const						{ return isResumable<TState>();						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void changeTo(const StateID stateId);
	HFSM_INLINE void restart (const StateID stateId);
	HFSM_INLINE void resume	 (const StateID stateId);
	HFSM_INLINE void utilize (const StateID stateId);
	HFSM_INLINE void schedule(const StateID stateId);

	template <typename TState>
	HFSM_INLINE void changeTo()									{ changeTo(stateId<TState>());						}

	template <typename TState>
	HFSM_INLINE void restart ()									{ restart (stateId<TState>());						}

	template <typename TState>
	HFSM_INLINE void resume	 ()									{ resume  (stateId<TState>());						}

	template <typename TState>
	HFSM_INLINE void utilize ()									{ utilize (stateId<TState>());						}

	template <typename TState>
	HFSM_INLINE void schedule()									{ schedule(stateId<TState>());						}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void changeTo(const StateID stateId, const Payload& payload);
	HFSM_INLINE void restart (const StateID stateId, const Payload& payload);
	HFSM_INLINE void resume  (const StateID stateId, const Payload& payload);
	HFSM_INLINE void utilize (const StateID stateId, const Payload& payload);
	HFSM_INLINE void schedule(const StateID stateId, const Payload& payload);

	template <typename TState>
	HFSM_INLINE void changeTo(const Payload& payload)			{ changeTo(stateId<TState>(), payload);				}

	template <typename TState>
	HFSM_INLINE void restart (const Payload& payload)			{ restart (stateId<TState>(), payload);				}

	template <typename TState>
	HFSM_INLINE void resume	 (const Payload& payload)			{ resume  (stateId<TState>(), payload);				}

	template <typename TState>
	HFSM_INLINE void utilize (const Payload& payload)			{ utilize (stateId<TState>(), payload);				}

	template <typename TState>
	HFSM_INLINE void schedule(const Payload& payload)			{ schedule(stateId<TState>(), payload);				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void resetStateData(const StateID stateId);
	HFSM_INLINE void setStateData  (const StateID stateId, const Payload& payload);
	HFSM_INLINE bool isStateDataSet(const StateID stateId) const;
	HFSM_INLINE const Payload* getStateData(const StateID stateId) const;

	template <typename TState>
	HFSM_INLINE void resetStateData()							{ resetStateData(stateId<TState>());				}

	template <typename TState>
	HFSM_INLINE void setStateData  (const Payload& payload)		{ setStateData  (stateId<TState>(), payload);		}

	template <typename TState>
	HFSM_INLINE bool isStateDataSet() const						{ return isStateDataSet(stateId<TState>());			}

	template <typename TState>
	HFSM_INLINE const Payload* getStateData() const				{ return getStateData(stateId<TState>());			}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	const MachineStructure& structure() const					{ return _structure;								}
	const MachineActivity&  activity()  const					{ return _activityHistory;							}
#endif

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	void attachLogger(LoggerInterface* const logger)			{ _logger = logger;									}
#endif

private:
	void initialEnter();
	void processTransitions();

	bool applyRequests(Control& control);

	bool cancelledByEntryGuards(const Requests& pendingChanges);
	bool cancelledByGuards(const Requests& pendingChanges);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	void getStateNames();
	void udpateActivity();
	void recordRequestsAs(const Method method);
#endif

private:
	Context& _context;

	StateRegistry _stateRegistry;
	PlanData _planData;

	Payloads _payloads;
	PayloadsSet _payloadsSet;
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

template <typename TContext,
		  typename TConfig,
		  typename TPayload,
		  typename TApex>
class _RW final
	: public _R<TContext, TConfig, TPayload, TApex>
{
public:
	using _R<TContext, TConfig, TPayload, TApex>::_R;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TConfig,
		  typename TPayload,
		  typename TApex>
class _RW<EmptyContext, TConfig, TPayload, TApex> final
	: public _R<EmptyContext, TConfig, TPayload, TApex>
	, EmptyContext
{
	using R = _R<EmptyContext, TConfig, TPayload, TApex>;

public:

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	_RW(LoggerInterface* const logger = nullptr)
		: R{static_cast<EmptyContext&>(*this), logger}
	{}
#else
	_RW()
		: R{static_cast<EmptyContext&>(*this)}
	{}
#endif
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "root.inl"
