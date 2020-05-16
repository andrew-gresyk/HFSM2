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

	using Apex					= TApex;
	using ApexInfo				= WrapInfo<Apex>;

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

	static constexpr LongIndex  ACTIVE_BITS		  = ApexInfo::ACTIVE_BITS;
	static constexpr LongIndex  RESUMABLE_BITS	  = ApexInfo::RESUMABLE_BITS;

	static constexpr LongIndex  STATE_COUNT		  = ApexInfo::STATE_COUNT;
	static constexpr LongIndex  REGION_COUNT	  = ApexInfo::REGION_COUNT;

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
	using PlanData				= PlanDataT   <Args>;

	using FullControl			= FullControlT<Args>;
	using Requests				= typename FullControl::Requests;

	using GuardControl			= GuardControlT<Args>;

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static constexpr LongIndex NAME_COUNT	  = MaterialApex::NAME_COUNT;

	using Prefix				= StaticArray<wchar_t, REVERSE_DEPTH * 2 + 2>;
	using Prefixes				= StaticArray<Prefix, STATE_COUNT>;

	using StructureStateInfos	= typename Args::StructureStateInfos;
#endif

#ifdef HFSM_ENABLE_SERIALIZATION
	using WriteStream			= typename Args::WriteStream;
	using ReadStream			= typename Args::ReadStream;
#endif

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using Structure				= Array<StructureEntry, NAME_COUNT>;
	using ActivityHistory		= Array<char,			NAME_COUNT>;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	explicit R_(Context& context,
				RNG& rng
				HFSM_IF_LOGGER(, Logger* const logger = nullptr));

	~R_();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename T>
	static constexpr StateID  stateId()				{ return			StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()			{ return (RegionID) RegionList::template index<T>();	}

	//----------------------------------------------------------------------

#ifdef HFSM_EXPLICIT_MEMBER_SPECIALIZATION

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4348) // redefinition of default parameter: parameter 2
#endif

	template <typename T, bool = StateList::template contains<T>()>
	struct Accessor;

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

	template <typename T>
	struct Accessor<T, true> {
		HFSM_INLINE static		 T& get(	  MaterialApex& apex)	{ return apex.template access<T>();		}
		HFSM_INLINE static const T& get(const MaterialApex& apex)	{ return apex.template access<T>();		}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// if you see..
	// VS	 - error C2027: use of undefined type 'hfsm2::detail::R_<..>::Accessor<T,false>'
	// Clang - error : implicit instantiation of undefined template 'hfsm2::detail::R_<..>::Accessor<*, false>'
	//
	// .. you're trying to access() a type that is not present in the state machine hierarchy

	template <typename T>
	HFSM_INLINE		  T& access()									{ return Accessor<T>::get(_apex);		}

	template <typename T>
	HFSM_INLINE const T& access() const								{ return Accessor<T>::get(_apex);		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#else

	template <typename T>
	HFSM_INLINE		  T& access()					{ return Accessor<T,	   MaterialApex>{_apex}.get();	}

	template <typename T>
	HFSM_INLINE const T& access() const				{ return Accessor<T, const MaterialApex>{_apex}.get();	}

#endif

	//----------------------------------------------------------------------

	void update();

	template <typename TEvent>
	HFSM_INLINE void react(const TEvent& event);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool isActive   (const StateID stateId) const	{ return _registry.isActive   (stateId);	}
	HFSM_INLINE bool isResumable(const StateID stateId) const	{ return _registry.isResumable(stateId);	}

	HFSM_INLINE bool isScheduled(const StateID stateId) const	{ return isResumable(stateId);				}

	template <typename TState>
	HFSM_INLINE bool isActive   () const						{ return isActive	(stateId<TState>());	}

	template <typename TState>
	HFSM_INLINE bool isResumable() const						{ return isResumable(stateId<TState>());	}

	template <typename TState>
	HFSM_INLINE bool isScheduled() const						{ return isResumable<TState>();				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void changeTo (const StateID stateId);
	HFSM_INLINE void restart  (const StateID stateId);
	HFSM_INLINE void resume	  (const StateID stateId);
	HFSM_INLINE void utilize  (const StateID stateId);
	HFSM_INLINE void randomize(const StateID stateId);
	HFSM_INLINE void schedule (const StateID stateId);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TState>
	HFSM_INLINE void changeTo ()								{ changeTo (stateId<TState>());				}

	template <typename TState>
	HFSM_INLINE void restart  ()								{ restart  (stateId<TState>());				}

	template <typename TState>
	HFSM_INLINE void resume	  ()								{ resume   (stateId<TState>());				}

	template <typename TState>
	HFSM_INLINE void utilize  ()								{ utilize  (stateId<TState>());				}

	template <typename TState>
	HFSM_INLINE void randomize()								{ randomize(stateId<TState>());				}

	template <typename TState>
	HFSM_INLINE void schedule ()								{ schedule (stateId<TState>());				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	const Structure&	   structure()		 const				{ return _structure;						}
	const ActivityHistory& activityHistory() const				{ return _activityHistory;					}
#endif

#ifdef HFSM_ENABLE_TRANSITION_HISTORY
	using TransitionHistory		= Array<Transition, COMPO_REGIONS * 4>;

	const TransitionHistory& transitionHistory() const			{ return _transitionHistory;				}

	void replayTransition (const Transition& transition)		{ replayTransitions(&transition, 1);		}
	void replayTransitions(const Transition* const transitions, const uint64_t count);
#endif

	void reset();

#ifdef HFSM_ENABLE_SERIALIZATION
	// Buffer for serialization
	//  Members:
	//   bitSize - Number of payload bits used
	//   payload - Serialized data
	//  See https://doc.hfsm.dev/user-guide/debugging-and-tools/serialization
	using SerialBuffer			= typename Args::SerialBuffer;

	// Serialize the structural configuration into 'buffer'
	void save(		SerialBuffer& buffer) const;

	// De-serialize the configuration and initialize the instance
	void load(const SerialBuffer& buffer);
#endif

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_ENABLE_VERBOSE_DEBUG_LOG
	void attachLogger(Logger* const logger)						{ _logger = logger;							}
#endif

private:
	void initialEnter();
	void processTransitions();

	bool applyRequest (Control& control, const Request& request);
	bool applyRequests(Control& control);

#ifdef HFSM_ENABLE_TRANSITION_HISTORY
	bool applyRequests(Control& control,
					   const Transition* const transitions,
					   const uint64_t count);
#endif

	bool cancelledByEntryGuards(const Requests& pendingChanges);
	bool cancelledByGuards(const Requests& pendingChanges);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	void getStateNames();
	void udpateActivity();
#endif

	HFSM_IF_TRANSITION_HISTORY(void recordRequestsAs(const Method method));

private:
	Context& _context;
	RNG& _rng;

	Registry _registry;
	PlanData _planData;

	Requests _requests;

	MaterialApex _apex;

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	Prefixes _prefixes;
	StructureStateInfos _stateInfos;

	Structure _structure;
	ActivityHistory _activityHistory;
#endif

	HFSM_IF_TRANSITION_HISTORY(TransitionHistory _transitionHistory);

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
		  LongIndex NS,
		  LongIndex NT,
		  typename TApex>
class RW_	   <::hfsm2::ConfigT<::hfsm2::EmptyContext, TN_, TU_, TR_, NS, NT>, TApex> final
	: public R_<::hfsm2::ConfigT<::hfsm2::EmptyContext, TN_, TU_, TR_, NS, NT>, TApex>
	, ::hfsm2::EmptyContext
{
	using Config_	= ::hfsm2::ConfigT<::hfsm2::EmptyContext, TN_, TU_, TR_, NS, NT>;
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
		  LongIndex NS,
		  LongIndex NT,
		  typename TApex>
class RW_	   <::hfsm2::ConfigT<TC_, TN_, TU_, ::hfsm2::RandomT<TU_>, NS, NT>, TApex> final
	: public R_<::hfsm2::ConfigT<TC_, TN_, TU_, ::hfsm2::RandomT<TU_>, NS, NT>, TApex>
	, ::hfsm2::RandomT<TU_>
{
	using Config_	= ::hfsm2::ConfigT<TC_, TN_, TU_, ::hfsm2::RandomT<TU_>, NS, NT>;
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
		  LongIndex NS,
		  LongIndex NT,
		  typename TApex>
class RW_	   <::hfsm2::ConfigT<::hfsm2::EmptyContext, TN_, TU_, ::hfsm2::RandomT<TU_>, NS, NT>, TApex> final
	: public R_<::hfsm2::ConfigT<::hfsm2::EmptyContext, TN_, TU_, ::hfsm2::RandomT<TU_>, NS, NT>, TApex>
	, ::hfsm2::EmptyContext
	, ::hfsm2::RandomT<TU_>
{
	using Config_	= ::hfsm2::ConfigT<::hfsm2::EmptyContext, TN_, TU_, ::hfsm2::RandomT<TU_>, NS, NT>;
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
