namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename TConfig,
		  typename TApex>
class R_ {
	using Context				= typename TConfig::Context;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Rank					= typename TConfig::Rank;
	using Utility				= typename TConfig::Utility;
	using RNG					= typename TConfig::RNG;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger				= typename TConfig::LoggerInterface;
#endif

	using Apex					= TApex;
	using ApexInfo				= WrapInfo<Apex>;

	using Info					= RF_<TConfig, Apex>;
	using StateList				= typename Info::StateList;
	using RegionList			= typename Info::RegionList;

	static constexpr LongIndex	SUBSTITUTION_LIMIT	= Info::SUBSTITUTION_LIMIT;

#ifdef HFSM2_ENABLE_PLANS
	static constexpr LongIndex	TASK_CAPACITY		= Info::TASK_CAPACITY;
#endif

public:
	static constexpr LongIndex  REVERSE_DEPTH		= ApexInfo::REVERSE_DEPTH;
	static constexpr ShortIndex COMPO_REGIONS		= ApexInfo::COMPO_REGIONS;
	static constexpr LongIndex  COMPO_PRONGS		= ApexInfo::COMPO_PRONGS;
	static constexpr ShortIndex ORTHO_REGIONS		= ApexInfo::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS			= ApexInfo::ORTHO_UNITS;

	static constexpr LongIndex  ACTIVE_BITS			= ApexInfo::ACTIVE_BITS;
	static constexpr LongIndex  RESUMABLE_BITS		= ApexInfo::RESUMABLE_BITS;

	static constexpr LongIndex  STATE_COUNT			= ApexInfo::STATE_COUNT;
	static constexpr LongIndex  REGION_COUNT		= ApexInfo::REGION_COUNT;

	static_assert(STATE_COUNT <  (ShortIndex) -1, "Too many states in the hierarchy. Change 'ShortIndex' type.");
	static_assert(STATE_COUNT == (ShortIndex) StateList::SIZE, "STATE_COUNT != StateList::SIZE");

private:
	using Args					= typename Info::Args;

	using Registry				= RegistryT<Args>;
	using CompoForks			= typename Registry::CompoForks;
	using RegistryBackUp		= typename Registry::BackUp;

	using MaterialApex			= Material<I_<0, 0, 0, 0>, Args, Apex>;

	using Control				= ControlT<Args>;
	using PlanControl			= PlanControlT<Args>;

#ifdef HFSM2_ENABLE_PLANS
	using PlanData				= PlanDataT   <Args>;
#endif

	using FullControl			= FullControlT<Args>;
	using Requests				= typename FullControl::Requests;

	using GuardControl			= GuardControlT<Args>;

#ifdef HFSM2_ENABLE_SERIALIZATION
	using WriteStream			= typename Args::WriteStream;
	using ReadStream			= typename Args::ReadStream;
#endif

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	static constexpr LongIndex NAME_COUNT	  = MaterialApex::NAME_COUNT;

	using Prefix				= StaticArray<wchar_t, REVERSE_DEPTH * 2 + 2>;
	using Prefixes				= StaticArray<Prefix, STATE_COUNT>;

	using StructureStateInfos	= typename Args::StructureStateInfos;
#endif

	//----------------------------------------------------------------------
public:

	explicit R_(Context& context
				HFSM2_IF_UTILITY_THEORY(, RNG& rng)
				HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr));

	~R_();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID  stateId()					{ return			StateList ::template index<TState>();	}

	/// @brief Get region identifier for a region type
	/// @tparam TState Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static constexpr RegionID regionId()				{ return (RegionID) RegionList::template index<TState>();	}

	//----------------------------------------------------------------------
#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION

private:

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4348) // redefinition of default parameter: parameter 2
#endif

	template <typename TState, bool = StateList::template contains<TState>()>
	struct Accessor;

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

	template <typename TState>
	struct Accessor<TState, true> {
		HFSM2_INLINE static		  TState& get(		MaterialApex& apex)	{ return apex.template access<TState>();	}
		HFSM2_INLINE static const TState& get(const MaterialApex& apex)	{ return apex.template access<TState>();	}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
public:

	// if you see..
	// VS	 - error C2027: use of undefined type 'hfsm2::detail::R_<..>::Accessor<TState,false>'
	// Clang - error : implicit instantiation of undefined template 'hfsm2::detail::R_<..>::Accessor<*, false>'
	//
	// .. you're trying to access() a type that is not present in the state machine hierarchy

	/// @brief Access state instance
	/// @tparam TState State type
	/// @return State instance
	template <typename TState>
	HFSM2_INLINE	   TState& access()										{ return Accessor<TState>::get(_apex);	}

	/// @brief Access state instance
	/// @tparam TState State type
	/// @return State instance
	template <typename TState>
	HFSM2_INLINE const TState& access() const								{ return Accessor<TState>::get(_apex);	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#else

public:

	/// @brief Access state instance
	/// @tparam TState State type
	/// @return State instance
	template <typename TState>
	HFSM2_INLINE	   TState& access()					{ return Accessor<TState,	   MaterialApex>{_apex}.get();	}

	/// @brief Access state instance
	/// @tparam TState State type
	/// @return State instance
	template <typename TState>
	HFSM2_INLINE const TState& access() const			{ return Accessor<TState, const MaterialApex>{_apex}.get();	}

#endif

	//----------------------------------------------------------------------

	/// @brief Trigger FSM update cycle (recursively call 'update()' on all active states, then process requested transitions)
	void update();

	/// @brief Have FSM react to an event (recursively call matching 'react<>()' on all active states, then process requested transitions)
	/// @tparam TEvent Event type
	/// @param event Event to react to
	template <typename TEvent>
	HFSM2_INLINE void react(const TEvent& event);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is active
	/// @param stateId State identifier
	/// @return State active status
	HFSM2_INLINE bool isActive   (const StateID stateId) const		{ return _registry.isActive   (stateId);		}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @param stateId State identifier
	/// @return State resumable status
	HFSM2_INLINE bool isResumable(const StateID stateId) const		{ return _registry.isResumable(stateId);		}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @param stateId State identifier
	/// @return State scheduled status
	HFSM2_INLINE bool isScheduled(const StateID stateId) const		{ return isResumable(stateId);					}

	/// @brief Check if a state is active
	/// @tparam TState State type
	/// @return State active status
	template <typename TState>
	HFSM2_INLINE bool isActive   () const							{ return isActive	(stateId<TState>());		}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @tparam TState State type
	/// @return State resumable status
	template <typename TState>
	HFSM2_INLINE bool isResumable() const							{ return isResumable(stateId<TState>());		}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @tparam TState State type
	/// @return State scheduled status
	template <typename TState>
	HFSM2_INLINE bool isScheduled() const							{ return isResumable<TState>();					}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId State identifier
	HFSM2_INLINE void changeTo	 (const StateID stateId);

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param stateId State identifier
	HFSM2_INLINE void restart	 (const StateID stateId);

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @param stateId State identifier
	HFSM2_INLINE void resume	 (const StateID stateId);

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @param stateId State identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void utilize	 (const StateID stateId);

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @param stateId State identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void randomize	 (const StateID stateId);

#endif

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId State identifier
	HFSM2_INLINE void schedule	 (const StateID stateId);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_INLINE void changeTo	 ()									{ changeTo (stateId<TState>());					}

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_INLINE void restart	 ()									{ restart  (stateId<TState>());					}

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState State type
	template <typename TState>
	HFSM2_INLINE void resume	 ()									{ resume   (stateId<TState>());					}

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///   with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TState State type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void utilize	()									{ utilize  (stateId<TState>());					}

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///   proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TState State type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void randomize	()									{ randomize(stateId<TState>());					}

#endif

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState State type
	template <typename TState>
	HFSM2_INLINE void schedule	()									{ schedule (stateId<TState>());					}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be activated or deactivated
	/// @param stateId State identifier
	/// @return State pending activation/deactivation status
	HFSM2_INLINE bool isPendingChange(const StateID stateId) const	{ return _registry.isPendingChange(stateId);	}

	/// @brief Check if a state is going to be activated
	/// @param stateId State identifier
	/// @return State pending activation status
	HFSM2_INLINE bool isPendingEnter (const StateID stateId) const	{ return _registry.isPendingEnter (stateId);	}

	/// @brief Check if a state is going to be deactivated
	/// @param stateId State identifier
	/// @return State pending deactivation status
	HFSM2_INLINE bool isPendingExit	 (const StateID stateId) const	{ return _registry.isPendingExit  (stateId);	}

	/// @brief Check if a state is going to be activated or deactivated
	/// @tparam TState State type
	/// @return State pending activation/deactivation status
	template <typename TState>
	HFSM2_INLINE bool isPendingChange()								{ return isPendingChange(stateId<TState>());	}

	/// @brief Check if a state is going to be activated
	/// @tparam TState State type
	/// @return State pending activation status
	template <typename TState>
	HFSM2_INLINE bool isPendingEnter ()								{ return isPendingEnter (stateId<TState>());	}

	/// @brief Check if a state is going to be deactivated
	/// @tparam TState State type
	/// @return State pending deactivation status
	template <typename TState>
	HFSM2_INLINE bool isPendingExit  ()								{ return isPendingExit  (stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Reset FSM to initial state (recursively 'exit()' currently active states, 'enter()' initial states)
	void reset();

#ifdef HFSM2_ENABLE_SERIALIZATION

	/// @brief Buffer for serialization
	/// @see https://doc.hfsm.dev/user-guide/debugging-and-tools/serialization
	/// @see HFSM2_ENABLE_SERIALIZATION
	using SerialBuffer			= typename Args::SerialBuffer;

	/// @brief Serialize FSM into 'buffer'
	/// @param buffer 'SerialBuffer' to serialize to
	/// @see HFSM2_ENABLE_SERIALIZATION
	void save(		SerialBuffer& buffer) const;

	/// @brief De-serialize FSM from 'buffer'
	/// @param buffer 'SerialBuffer' to de-serialize from
	/// @see HFSM2_ENABLE_SERIALIZATION
	void load(const SerialBuffer& buffer);

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY

	/// @brief Array of last recorded transitions
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	using TransitionHistory		= Array<Transition, COMPO_REGIONS * 4>;

	/// @brief Get the list of transitions recorded during last 'update()'
	/// @return Array of last recorded transitions
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	const TransitionHistory& transitionHistory() const				{ return _transitionHistory;					}

	/// @brief Force process transitions (skips 'guard()' calls)
	///   Can be used to synchronize multiple FSMs
	/// @param transitions Array of 'Transition' to replay
	/// @param count Number of transitions
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	void replayTransitions(const Transition* const transitions, const uint64_t count);

	/// @brief Force process transitions (skips 'guard()' calls)
	///   Can be used to synchronize multiple FSMs
	/// @param transitions Array of 'Transition' to replay
	/// @param count Number of transitions
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	void replayTransitions(const TransitionHistory& transitions)	{ replayTransitions(transitions, 1);			}

	/// @brief Force process a transition (skips 'guard()' calls)
	///   Can be used to synchronize multiple FSMs
	/// @param transition 'Transition' to replay
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	void replayTransition (const Transition& transition)			{ replayTransitions(&transition, 1);			}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

	/// @brief Array of 'StructureEntry' representing FSM structure
	/// @see HFSM2_ENABLE_STRUCTURE_REPORT
	using Structure				= Array<StructureEntry, NAME_COUNT>;

	/// @brief Array of 'char' representing FSM activation history (negative - 'update()' cycles since deactivated, positive - 'update()' cycles since activated)
	/// @see HFSM2_ENABLE_STRUCTURE_REPORT
	using ActivityHistory		= Array<char,			NAME_COUNT>;

	/// @brief Get the array of 'StructureEntry' representing FSM structure
	/// @return FSM structure
	/// @see HFSM2_ENABLE_STRUCTURE_REPORT
	const Structure&	   structure()		 const					{ return _structure;							}

	/// @brief Get the array of 'char' representing FSM activation history (negative - 'update()' cycles since deactivated, positive - 'update()' cycles since activated)
	/// @return FSM activation history
	/// @see HFSM2_ENABLE_STRUCTURE_REPORT
	const ActivityHistory& activityHistory() const					{ return _activityHistory;						}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_LOG_INTERFACE

	/// @brief Attach logger
	/// @param logger A logger implementing 'hfsm2::LoggerInterfaceT<TContext>' interface
	/// @see HFSM2_ENABLE_LOG_INTERFACE
	void attachLogger(Logger* const logger)							{ _logger = logger;								}

#endif

	//----------------------------------------------------------------------

private:
	void initialEnter();
	void processTransitions();

	bool applyRequest (Control& control, const Request& request);
	bool applyRequests(Control& control);

	bool cancelledByEntryGuards(const Requests& pendingChanges);
	bool cancelledByGuards(const Requests& pendingChanges);

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY
	bool applyRequests(Control& control,
					   const Transition* const transitions,
					   const uint64_t count);

	void recordRequestsAs(const Method method);

	TransitionHistory _transitionHistory;
#endif

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	void getStateNames();
	void udpateActivity();

	Prefixes _prefixes;
	StructureStateInfos _stateInfos;

	Structure _structure;
	ActivityHistory _activityHistory;
#endif

private:
	Context& _context;
	HFSM2_IF_UTILITY_THEORY(RNG& _rng);

	Registry _registry;
	HFSM2_IF_PLANS(PlanData _planData);

	Requests _requests;

	MaterialApex _apex;

	HFSM2_IF_LOG_INTERFACE(Logger* _logger);
};

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename TConfig,
		  typename TApex>
class RW_ final
	: public R_<TConfig, TApex>
{
public:
	using R_<TConfig, TApex>::R_;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <
#ifdef HFSM2_ENABLE_UTILITY_THEORY
		  typename TRank,
		  typename TUtility,
		  typename TRNG,
#endif
		  LongIndex NSubstitutionLimit,

#ifdef HFSM2_ENABLE_PLANS
		  LongIndex NTaskCapacity,
#endif

		  typename TApex>
class RW_	   <::hfsm2::ConfigT<::hfsm2::EmptyContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity)>, TApex> final
	: public R_<::hfsm2::ConfigT<::hfsm2::EmptyContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity)>, TApex>
	, ::hfsm2::EmptyContext
{
#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using RNG		= TRNG;
#endif

	using Context	= ::hfsm2::EmptyContext;
	using Cfg		= ::hfsm2::ConfigT<Context HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, RNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity)>;
	using R			= R_<Cfg, TApex>;

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Cfg::LoggerInterface;
#endif

public:

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	explicit HFSM2_INLINE RW_(RNG& rng
							  HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))
		: R{static_cast<Context&>(*this),
			rng
			HFSM2_IF_LOG_INTERFACE(, logger)}
	{}

#else

	explicit HFSM2_INLINE RW_(HFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr))
		: R{static_cast<Context&>(*this)
			HFSM2_IF_LOG_INTERFACE(, logger)}
	{}

#endif
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TContext,
#ifdef HFSM2_ENABLE_UTILITY_THEORY
		  typename TRank,
		  typename TUtility,
#endif
		  LongIndex NSubstitutionLimit,

#ifdef HFSM2_ENABLE_PLANS
		  LongIndex NTaskCapacity,
#endif

		  typename TApex>
class RW_	   <::hfsm2::ConfigT<TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, ::hfsm2::RandomT<TUtility>), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity)>, TApex> final
	: public R_<::hfsm2::ConfigT<TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, ::hfsm2::RandomT<TUtility>), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity)>, TApex>
	HFSM2_IF_UTILITY_THEORY(, ::hfsm2::RandomT<TUtility>)
{
#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using RNG		= ::hfsm2::RandomT<TUtility>;
#endif

	using Context	= TContext;
	using Cfg		= ::hfsm2::ConfigT<Context HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, ::hfsm2::RandomT<TUtility>), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity)>;
	using R			= R_<Cfg, TApex>;

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Cfg::LoggerInterface;
#endif


public:
#ifdef HFSM2_ENABLE_UTILITY_THEORY

	explicit HFSM2_INLINE RW_(Context& context
							  HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))
		: R{context,
			static_cast<RNG&>(*this)
			HFSM2_IF_LOG_INTERFACE(, logger)}
		, RNG{0}
	{}

#else

	explicit HFSM2_INLINE RW_(Context& context
							  HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))
		: R{context
			HFSM2_IF_LOG_INTERFACE(, logger)}
	{}

#endif
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TRank,
		  typename TUtility,
		  LongIndex NSubstitutionLimit,

#ifdef HFSM2_ENABLE_PLANS
		  LongIndex NTaskCapacity,
#endif

		  typename TApex>
class RW_	   <::hfsm2::ConfigT<::hfsm2::EmptyContext, TRank, TUtility, ::hfsm2::RandomT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity)>, TApex> final
	: public R_<::hfsm2::ConfigT<::hfsm2::EmptyContext, TRank, TUtility, ::hfsm2::RandomT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity)>, TApex>
	, ::hfsm2::EmptyContext
	, ::hfsm2::RandomT<TUtility>
{
	using Context	= ::hfsm2::EmptyContext;
	using RNG		= ::hfsm2::RandomT<TUtility>;
	using Cfg		= ::hfsm2::ConfigT<Context, TRank, TUtility, RNG, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity)>;
	using R			= R_<Cfg, TApex>;

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Cfg::LoggerInterface;
#endif

public:
	explicit HFSM2_INLINE RW_(HFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr))
		: R{static_cast<Context&>(*this),
			static_cast<RNG&>(*this)
			HFSM2_IF_LOG_INTERFACE(, logger)}
		, RNG{0}
	{}
};

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#include "root.inl"
