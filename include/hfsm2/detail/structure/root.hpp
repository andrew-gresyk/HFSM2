namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig,
		  typename TApex>
class R_ {
	using Config_				= TConfig;
	using Context				= typename Config_::Context;
	using Rank					= typename Config_::Rank;
	using Utility				= typename Config_::Utility;
	using RNG					= typename Config_::RNG;
	using Logger				= typename Config_::Logger;
	using Payload				= typename Config_::Payload;

	using Apex					= TApex;

	using ApexInfo				= Wrap<Apex>;
	using Info					= RF_<Config_, Apex>;
	using StateList				= typename Info::StateList;
	using RegionList			= typename Info::RegionList;

	static constexpr LongIndex SUBSTITUTION_LIMIT = Info::SUBSTITUTION_LIMIT;
	static constexpr LongIndex TASK_CAPACITY	  = Info::TASK_CAPACITY;

public:
	static constexpr LongIndex  REVERSE_DEPTH	  = ApexInfo::REVERSE_DEPTH;
	static constexpr ShortIndex COMPO_REGIONS	  = ApexInfo::COMPO_REGIONS;
	static constexpr LongIndex  COMPO_PRONGS	  = ApexInfo::COMPO_PRONGS;
	static constexpr ShortIndex ORTHO_REGIONS	  = ApexInfo::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS		  = ApexInfo::ORTHO_UNITS;

	static constexpr LongIndex  STATE_COUNT		  = ApexInfo::STATE_COUNT;
	static constexpr LongIndex  REGION_COUNT	  = ApexInfo::REGION_COUNT;

	static_assert(STATE_COUNT <  (ShortIndex) -1, "Too many states in the hierarchy. Change 'ShortIndex' type.");
	static_assert(STATE_COUNT == (ShortIndex) StateList::SIZE, "STATE_COUNT != StateList::SIZE");

private:
	using Args					= typename Info::Args;

	using StateRegistry			= StateRegistryT<Args>;
	using AllForks				= typename StateRegistry::AllForks;

	using Control				= ControlT<Args>;

	using PlanControl			= PlanControlT<Args>;
	using PlanData				= PlanDataT   <Args>;

	using FullControl			= FullControlT<Args>;
	using Request				= typename FullControl::Request;
	using Requests				= typename FullControl::Requests;

	using GuardControl			= GuardControlT<Args>;

	using Payloads				= Array<Payload, STATE_COUNT>;
	using PayloadsSet			= BitArray<LongIndex, STATE_COUNT>;

	using MaterialApex			= Material<I_<0, 0, 0, 0>, Args, Apex>;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos	= typename Args::StructureStateInfos;

	static constexpr LongIndex NAME_COUNT	  = MaterialApex::NAME_COUNT;

	using Prefix				= StaticArray<wchar_t, REVERSE_DEPTH * 2 + 2>;
	using Prefixes				= StaticArray<Prefix, STATE_COUNT>;

	using Structure				= Array<StructureEntry, NAME_COUNT>;
	using ActivityHistory		= Array<char,			NAME_COUNT>;

	using TransitionInfo		= TransitionInfoT<Payload>;
	using TransitionInfoStorage	= Array<TransitionInfo, COMPO_REGIONS * 4>;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	explicit R_(Context& context,
				RNG& rng
				HFSM_IF_LOGGER(, Logger* const logger = nullptr));

	~R_();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TState>
	static constexpr StateID  stateId()				{ return			StateList ::template index<TState>();	}

	template <typename TState>
	static constexpr RegionID regionId()			{ return (RegionID) RegionList::template index<TState>();	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename T>
	HFSM_INLINE		  T& access()								{ return detail::access<T>(_apex);				}

	template <typename T>
	HFSM_INLINE const T& access() const							{ return detail::access<T>(_apex);				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	void update();

	template <typename TEvent>
	HFSM_INLINE void react(const TEvent& event);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool isActive   (const StateID stateId) const	{ return _stateRegistry.isActive   (stateId);	}
	HFSM_INLINE bool isResumable(const StateID stateId) const	{ return _stateRegistry.isResumable(stateId);	}

	HFSM_INLINE bool isScheduled(const StateID stateId) const	{ return isResumable(stateId);					}

	template <typename TState>
	HFSM_INLINE bool isActive   () const						{ return isActive	(stateId<TState>());		}

	template <typename TState>
	HFSM_INLINE bool isResumable() const						{ return isResumable(stateId<TState>());		}

	template <typename TState>
	HFSM_INLINE bool isScheduled() const						{ return isResumable<TState>();					}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void changeTo (const StateID stateId);
	HFSM_INLINE void changeTo (const StateID stateId, const Payload& payload);

	HFSM_INLINE void restart  (const StateID stateId);
	HFSM_INLINE void restart  (const StateID stateId, const Payload& payload);

	HFSM_INLINE void resume	  (const StateID stateId);
	HFSM_INLINE void resume   (const StateID stateId, const Payload& payload);

	HFSM_INLINE void utilize  (const StateID stateId);
	HFSM_INLINE void utilize  (const StateID stateId, const Payload& payload);

	HFSM_INLINE void randomize(const StateID stateId);
	HFSM_INLINE void randomize(const StateID stateId, const Payload& payload);

	HFSM_INLINE void schedule (const StateID stateId);
	HFSM_INLINE void schedule (const StateID stateId, const Payload& payload);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TState>
	HFSM_INLINE void changeTo ()								{ changeTo (stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void changeTo (const Payload& payload)			{ changeTo (stateId<TState>(), payload);		}

	template <typename TState>
	HFSM_INLINE void restart  ()								{ restart  (stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void restart  (const Payload& payload)			{ restart  (stateId<TState>(), payload);		}

	template <typename TState>
	HFSM_INLINE void resume	  ()								{ resume   (stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void resume	  (const Payload& payload)			{ resume   (stateId<TState>(), payload);		}

	template <typename TState>
	HFSM_INLINE void utilize  ()								{ utilize  (stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void utilize  (const Payload& payload)			{ utilize  (stateId<TState>(), payload);		}

	template <typename TState>
	HFSM_INLINE void randomize()								{ randomize(stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void randomize(const Payload& payload)			{ randomize(stateId<TState>(), payload);		}

	template <typename TState>
	HFSM_INLINE void schedule ()								{ schedule (stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void schedule (const Payload& payload)			{ schedule (stateId<TState>(), payload);		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// TODO: deprecate

	HFSM_INLINE void resetStateData(const StateID stateId);
	HFSM_INLINE void setStateData  (const StateID stateId, const Payload& payload);
	HFSM_INLINE bool isStateDataSet(const StateID stateId) const;

	HFSM_INLINE const Payload* getStateData(const StateID stateId) const;

	template <typename TState>
	HFSM_INLINE void resetStateData()							{ resetStateData(stateId<TState>());			}

	template <typename TState>
	HFSM_INLINE void setStateData  (const Payload& payload)		{ setStateData  (stateId<TState>(), payload);	}

	template <typename TState>
	HFSM_INLINE bool isStateDataSet() const						{ return isStateDataSet(stateId<TState>());		}

	template <typename TState>
	HFSM_INLINE const Payload* getStateData() const				{ return getStateData(stateId<TState>());		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	const Structure&	   structure()		 const				{ return _structure;							}
	const ActivityHistory& activityHistory() const				{ return _activityHistory;						}
#endif

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_ENABLE_VERBOSE_DEBUG_LOG
	void attachLogger(Logger* const logger)						{ _logger = logger;								}
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
	RNG& _rng;

	StateRegistry _stateRegistry;
	PlanData _planData;

	Payloads _payloads;
	PayloadsSet _payloadsSet;

	Requests _requests;

	MaterialApex _apex;

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	Prefixes _prefixes;
	StructureStateInfos _stateInfos;

	Structure _structure;
	ActivityHistory _activityHistory;

	TransitionInfoStorage _lastTransitions;
#endif

	HFSM_IF_LOGGER(Logger* _logger);
};

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig,
		  typename TApex>
class RW_ final
	: public R_<TConfig, TApex>
{
public:
	using R_<TConfig, TApex>::R_;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_,
		  typename TU_,
		  typename TR_,
		  typename TP_,
		  LongIndex NS,
		  LongIndex NT,
		  typename TApex>
class RW_	   <::hfsm2::ConfigT<::hfsm2::EmptyContext, TN_, TU_, TR_, TP_, NS, NT>, TApex> final
	: public R_<::hfsm2::ConfigT<::hfsm2::EmptyContext, TN_, TU_, TR_, TP_, NS, NT>, TApex>
	, ::hfsm2::EmptyContext
{
	using Config_	= ::hfsm2::ConfigT<::hfsm2::EmptyContext, TN_, TU_, TR_, TP_, NS, NT>;
	using Context	= typename Config_::Context;
	using RNG		= typename Config_::RNG;
	using Logger	= typename Config_::Logger;

	using R			= R_<Config_, TApex>;

public:
	explicit HFSM_INLINE RW_(RNG& rng
							 HFSM_IF_LOGGER(, Logger* const logger = nullptr))
		: R{static_cast<Context&>(*this),
			rng
			HFSM_IF_LOGGER(, logger)}
	{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC_,
		  typename TN_,
		  typename TU_,
		  typename TP_,
		  LongIndex NS,
		  LongIndex NT,
		  typename TApex>
class RW_	   <::hfsm2::ConfigT<TC_, TN_, TU_, ::hfsm2::RandomT<TU_>, TP_, NS, NT>, TApex> final
	: public R_<::hfsm2::ConfigT<TC_, TN_, TU_, ::hfsm2::RandomT<TU_>, TP_, NS, NT>, TApex>
	, ::hfsm2::RandomT<TU_>
{
	using Config_	= ::hfsm2::ConfigT<TC_, TN_, TU_, ::hfsm2::RandomT<TU_>, TP_, NS, NT>;
	using Context	= typename Config_::Context;
	using RNG		= typename Config_::RNG;
	using Logger	= typename Config_::Logger;

	using R			= R_<Config_, TApex>;

public:
	explicit HFSM_INLINE RW_(Context& context
							 HFSM_IF_LOGGER(, Logger* const logger = nullptr))
		: R{context,
			static_cast<RNG&>(*this)
			HFSM_IF_LOGGER(, logger)}
		, RNG{0}
	{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_,
		  typename TU_,
		  typename TP_,
		  LongIndex NS,
		  LongIndex NT,
		  typename TApex>
class RW_	   <::hfsm2::ConfigT<::hfsm2::EmptyContext, TN_, TU_, ::hfsm2::RandomT<TU_>, TP_, NS, NT>, TApex> final
	: public R_<::hfsm2::ConfigT<::hfsm2::EmptyContext, TN_, TU_, ::hfsm2::RandomT<TU_>, TP_, NS, NT>, TApex>
	, ::hfsm2::EmptyContext
	, ::hfsm2::RandomT<TU_>
{
	using Config_	= ::hfsm2::ConfigT<::hfsm2::EmptyContext, TN_, TU_, ::hfsm2::RandomT<TU_>, TP_, NS, NT>;
	using Context	= typename Config_::Context;
	using RNG		= typename Config_::RNG;
	using Logger	= typename Config_::Logger;

	using R			= R_<Config_, TApex>;

public:
	explicit HFSM_INLINE RW_(HFSM_IF_LOGGER(Logger* const logger = nullptr))
		: R{static_cast<Context&>(*this),
			static_cast<RNG&>(*this)
			HFSM_IF_LOGGER(, logger)}
		, RNG{0}
	{}
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "root.inl"
