namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename TConfig,
		  typename TApex>
class R_ {
public:
	static constexpr FeatureTag FEATURE_TAG = TConfig::FEATURE_TAG;

	using Context				= typename TConfig::Context;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Rank					= typename TConfig::Rank;
	using Utility				= typename TConfig::Utility;
	using RNG					= typename TConfig::RNG;
#endif

	using Payload				= typename TConfig::Payload;

private:
	using Apex					= TApex;

	using Forward				= RF_<TConfig, Apex>;
	using StateList				= typename Forward::StateList;
	using RegionList			= typename Forward::RegionList;
	using Args					= typename Forward::Args;

	static_assert(Args::STATE_COUNT <  (unsigned) -1, "Too many states in the FSM. Change 'Short' type.");
	static_assert(Args::STATE_COUNT == (unsigned) StateList::SIZE, "STATE_COUNT != StateList::SIZE");

	using MaterialApex			= Material<I_<0, 0, 0, 0>, Args, Apex>;

	using Registry				= RegistryT<Args>;
	using CompoForks			= typename Registry::CompoForks;
	using RegistryBackUp		= typename Registry::BackUp;

	using Control				= ControlT	   <Args>;
	using PlanControl			= PlanControlT <Args>;
	using FullControl			= FullControlT <Args>;
	using GuardControl			= GuardControlT<Args>;

	static constexpr Long	SUBSTITUTION_LIMIT	= Forward::SUBSTITUTION_LIMIT;

#ifdef HFSM2_ENABLE_PLANS
	using PlanData				= PlanDataT<Args>;
#endif

#ifdef HFSM2_ENABLE_SERIALIZATION
	using WriteStream			= typename Args::WriteStream;
	using ReadStream			= typename Args::ReadStream;
#endif

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	static constexpr Long NAME_COUNT = MaterialApex::NAME_COUNT;
#endif

#ifdef HFSM2_ENABLE_PLANS
	static constexpr Long TASK_CAPACITY = Forward::TASK_CAPACITY;
#endif

public:
	using Info					= WrapInfo<Apex>;

	/// @brief Transition
	using Transition			= typename Control::Transition;

	/// @brief Array of transitions
	using TransitionSet			= typename Control::TransitionSet;
	using TransitionSets		= typename Control::TransitionSets;

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY
	using TransitionTargets		= typename Control::TransitionTargets;
#endif

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	using Prefix				= StaticArray<wchar_t, Info::REVERSE_DEPTH * 2 + 2>;
	using Prefixes				= StaticArray<Prefix, Info::STATE_COUNT>;

	using StructureStateInfos	= typename Args::StructureStateInfos;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger				= typename TConfig::LoggerInterface;
#endif

public:

	//----------------------------------------------------------------------

	HFSM2_INLINE explicit R_(Context& context
						  HFSM2_IF_UTILITY_THEORY(, RNG& rng)
						  HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept;

	HFSM2_INLINE ~R_() noexcept;

	//----------------------------------------------------------------------

	/// @brief Access context
	/// @return context
	HFSM2_INLINE	   Context& context()							  noexcept	{ return _context;									}

	/// @brief Access context
	/// @return context
	HFSM2_INLINE const Context& context()						const noexcept	{ return _context;									}

	//----------------------------------------------------------------------

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID stateId()								  noexcept	{ return index<StateList, TState>();				}

	/// @brief Get region identifier for a region type
	/// @tparam TState Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static constexpr RegionID regionId()							  noexcept	{ return (RegionID) index<RegionList, TState>();	}

	//----------------------------------------------------------------------
#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION

private:

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4348) // redefinition of default parameter: parameter 2
#endif

	template <typename TState, bool = contains<StateList, TState>()>
	struct Accessor;

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

	template <typename TState>
	struct Accessor<TState, true> {
		HFSM2_INLINE static		  TState& get(		MaterialApex& apex) noexcept	{ return apex.template access<TState>();	}
		HFSM2_INLINE static const TState& get(const MaterialApex& apex) noexcept	{ return apex.template access<TState>();	}
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
	HFSM2_INLINE	   TState& access()								  noexcept	{ return Accessor<TState				   >::get(_apex);	}

	/// @brief Access state instance
	/// @tparam TState State type
	/// @return State instance
	template <typename TState>
	HFSM2_INLINE const TState& access()							const noexcept	{ return Accessor<TState				   >::get(_apex);	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#else

public:

	/// @brief Access state instance
	/// @tparam TState State type
	/// @return State instance
	template <typename TState>
	HFSM2_INLINE	   TState& access()								  noexcept	{ return Accessor<TState,	   MaterialApex>{_apex}.get();	}

	/// @brief Access state instance
	/// @tparam TState State type
	/// @return State instance
	template <typename TState>
	HFSM2_INLINE const TState& access()							const noexcept	{ return Accessor<TState, const MaterialApex>{_apex}.get();	}

#endif

	//----------------------------------------------------------------------

	/// @brief Trigger FSM update cycle (recursively call 'update()' on all active states, then process requested transitions)
	void update() noexcept;

	/// @brief Have FSM react to an event (recursively call matching 'react<>()' on all active states, then process requested transitions)
	/// @tparam TEvent Event type
	/// @param event Event to react to
	template <typename TEvent>
	HFSM2_INLINE void react(const TEvent& event)					  noexcept;

	//----------------------------------------------------------------------

	/// @brief Check if a state is active
	/// @param stateId Destination state identifier
	/// @return State active status
	HFSM2_INLINE bool isActive		 (const StateID stateId)	const noexcept	{ return _registry.isActive   (stateId);		}

	/// @brief Check if a state is active
	/// @tparam TState Destination state type
	/// @return State active status
	template <typename TState>
	HFSM2_INLINE bool isActive		 ()							const noexcept	{ return isActive	(stateId<TState>());		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @param stateId Destination state identifier
	/// @return State resumable status
	HFSM2_INLINE bool isResumable	 (const StateID stateId)	const noexcept	{ return _registry.isResumable(stateId);		}

	/// @brief Check if a state is resumable (activated then deactivated previously)
	/// @tparam TState Destination state type
	/// @return State resumable status
	template <typename TState>
	HFSM2_INLINE bool isResumable	 ()							const noexcept	{ return isResumable(stateId<TState>());		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @param stateId Destination state identifier
	/// @return State scheduled status
	HFSM2_INLINE bool isScheduled	 (const StateID stateId)	const noexcept	{ return isResumable(stateId);					}

	/// @brief Check if a state is scheduled to activate on the next transition to parent region
	/// @tparam TState Destination state type
	/// @return State scheduled status
	template <typename TState>
	HFSM2_INLINE bool isScheduled	 ()							const noexcept	{ return isResumable<TState>();					}

	//------------------------------------------------------------------------------
	// COMMON

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	HFSM2_INLINE void changeTo		 (const StateID stateId)		  noexcept;

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_INLINE void changeTo		 ()								  noexcept	{ changeTo (stateId<TState>());					}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	HFSM2_INLINE void restart		 (const StateID stateId)		  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_INLINE void restart		 ()								  noexcept	{ restart  (stateId<TState>());					}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	HFSM2_INLINE void resume		 (const StateID stateId)		  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_INLINE void resume		 ()								  noexcept	{ resume   (stateId<TState>());					}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void utilize		 (const StateID stateId)		  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///   with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void utilize		 ()								  noexcept	{ utilize  (stateId<TState>());					}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void randomize		 (const StateID stateId)		  noexcept;

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///   proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void randomize		 ()								  noexcept	{ randomize(stateId<TState>());					}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId Destination state identifier
	HFSM2_INLINE void schedule		 (const StateID stateId)		  noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState Destination state type
	template <typename TState>
	HFSM2_INLINE void schedule		 ()								  noexcept	{ schedule (stateId<TState>());					}

	//------------------------------------------------------------------------------

	/// @brief Check if a state is going to be activated or deactivated
	/// @param stateId Destination state identifier
	/// @return State pending activation/deactivation status
	HFSM2_INLINE bool isPendingChange(const StateID stateId)	const noexcept	{ return _registry.isPendingChange(stateId);	}

	/// @brief Check if a state is going to be activated or deactivated
	/// @tparam TState Destination state type
	/// @return State pending activation/deactivation status
	template <typename TState>
	HFSM2_INLINE bool isPendingChange()								  noexcept	{ return isPendingChange(stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be activated
	/// @param stateId Destination state identifier
	/// @return State pending activation status
	HFSM2_INLINE bool isPendingEnter (const StateID stateId)	const noexcept	{ return _registry.isPendingEnter (stateId);	}

	/// @brief Check if a state is going to be activated
	/// @tparam TState Destination state type
	/// @return State pending activation status
	template <typename TState>
	HFSM2_INLINE bool isPendingEnter ()								  noexcept	{ return isPendingEnter (stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be deactivated
	/// @param stateId Destination state identifier
	/// @return State pending deactivation status
	HFSM2_INLINE bool isPendingExit	 (const StateID stateId)	const noexcept	{ return _registry.isPendingExit  (stateId);	}

	/// @brief Check if a state is going to be deactivated
	/// @tparam TState Destination state type
	/// @return State pending deactivation status
	template <typename TState>
	HFSM2_INLINE bool isPendingExit  ()								  noexcept	{ return isPendingExit  (stateId<TState>());	}

	//------------------------------------------------------------------------------

	/// @brief Reset FSM to initial state (recursively 'exit()' currently active states, 'enter()' initial states)
	void reset() noexcept;

	//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION

	/// @brief Buffer for serialization
	/// @see https://doc.hfsm.dev/user-guide/debugging-and-tools/serialization
	/// @see HFSM2_ENABLE_SERIALIZATION
	using SerialBuffer			= typename Args::SerialBuffer;

	/// @brief Serialize FSM into 'buffer'
	/// @param buffer 'SerialBuffer' to serialize to
	/// @see HFSM2_ENABLE_SERIALIZATION
	void save(		SerialBuffer& buffer)						const noexcept;

	/// @brief De-serialize FSM from 'buffer'
	/// @param buffer 'SerialBuffer' to de-serialize from
	/// @see HFSM2_ENABLE_SERIALIZATION
	void load(const SerialBuffer& buffer)							  noexcept;

#endif

	//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY

	/// @brief Get the list of transitions recorded during last 'update()'
	/// @return Array of last recorded transitions
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	const TransitionSets& previousTransitions()					const noexcept	{ return _previousTransitions;					}

	/// @brief Force process transitions (skips 'guard()' calls)
	///   Can be used to synchronize multiple FSMs
	/// @param transitions Array of 'Transition' to replay
	/// @param count Number of transitions
	/// @return Success status
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	bool replayTransitions(const Transition* const transitions,
						   const uint64_t count)					  noexcept;

	/// @brief Force process transitions (skips 'guard()' calls)
	///   Can be used to synchronize multiple FSMs
	/// @param transitions 'TransitionHistory' to replay
	/// @return Success status
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	template <Long NCount>
	bool replayTransitions(const Array<Transition,
						   NCount>& transitions)					  noexcept;

	/// @brief Force process a transition (skips 'guard()' calls)
	///   Can be used to synchronize multiple FSMs
	/// @param transition 'Transition' to replay
	/// @return Success status
	/// @see HFSM2_ENABLE_TRANSITION_HISTORY
	bool replayTransition (const Transition& transition)			  noexcept	{ return replayTransitions(&transition, 1);		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Get the last transition that caused the state to be activated
	/// @param stateId State identifier
	/// @return Pointer to the last transition that activated the state
	const Transition* lastTransition(const StateID stateId)		const noexcept;

	/// @brief Get the last transition that caused the state to be activated
	/// @tparam TState State type
	/// @return Pointer to the last transition that activated the state
	template <typename TState>
	const Transition* lastTransition()							const noexcept	{ return lastTransition(stateId<TState>());		}

#endif

	//------------------------------------------------------------------------------

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
	const Structure&	   structure()							const noexcept	{ return _structure;							}

	/// @brief Get the array of 'char' representing FSM activation history (negative - 'update()' cycles since deactivated, positive - 'update()' cycles since activated)
	/// @return FSM activation history
	/// @see HFSM2_ENABLE_STRUCTURE_REPORT
	const ActivityHistory& activityHistory()					const noexcept	{ return _activityHistory;						}

#endif

	//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_LOG_INTERFACE

	/// @brief Attach logger
	/// @param logger A logger implementing 'hfsm2::LoggerInterfaceT<>' interface
	/// @see HFSM2_ENABLE_LOG_INTERFACE
	HFSM2_INLINE void attachLogger(Logger* const logger)			  noexcept	{ _logger = logger;								}

#endif

	//----------------------------------------------------------------------

private:
	void initialEnter() noexcept;
	void processTransitions(TransitionSets& currentTransitions) noexcept;

	bool applyRequest (Control& control, const Transition& request, const Short index) noexcept;
	bool applyRequests(Control& control) noexcept;

	bool cancelledByEntryGuards(const TransitionSets& currentTransitions,
								const TransitionSet&  pendingTransitions) noexcept;

	bool cancelledByGuards(const TransitionSets& currentTransitions,
						   const TransitionSet&  pendingTransitions) noexcept;

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY
	bool applyRequests(Control& control,
					   const Transition* const transitions,
					   const uint64_t count) noexcept;

	TransitionTargets _transitionTargets;
	TransitionSets _previousTransitions;
#endif

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	void getStateNames() noexcept;
	void udpateActivity() noexcept;

	Prefixes _prefixes;
	StructureStateInfos _stateInfos;

	Structure _structure;
	ActivityHistory _activityHistory;
#endif

protected:
	Context _context;
	HFSM2_IF_UTILITY_THEORY(RNG& _rng);

	Registry _registry;
	HFSM2_IF_PLANS(PlanData _planData);

	TransitionSet _requests;

	MaterialApex _apex;

	HFSM2_IF_LOG_INTERFACE(Logger* _logger);
};

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig,
		  typename TApex>
class RP_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFeatureTag
		, typename TContext

	#ifdef HFSM2_ENABLE_UTILITY_THEORY
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RP_		   <G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 R_<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = R_<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

	using Transition = TransitionT<TPayload>;

public:
	using Payload	 = typename Base::Payload;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using RNG		 = typename Base::RNG;
#endif

public:
	using Base::Base;

	/// @brief Get state identifier for a state type
	/// @tparam TState State type
	/// @return Numeric state identifier
	template <typename TState>
	static constexpr StateID  stateId()								  noexcept	{ return Base::template stateId<TState>();		}

	/// @brief Get region identifier for a region type
	/// @tparam TState Region head state type
	/// @return Numeric region identifier
	template <typename TState>
	static constexpr RegionID regionId()							  noexcept	{ return Base::template regionId<TState>();		}

	//------------------------------------------------------------------------------
	// COMMON

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void changeWith   (const StateID  stateId,
									const Payload& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void changeWith   (const StateID  stateId,
										 Payload&& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void changeWith   (const Payload& payload)			  noexcept	{ changeWith   (stateId<TState>(),			 payload );	}

	/// @brief Transition into a state (if transitioning into a region, acts depending on the region type)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void changeWith   (	 Payload&& payload)			  noexcept	{ changeWith   (stateId<TState>(), std::move(payload));	}

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void restartWith  (const StateID  stateId,
									const Payload& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void restartWith  (const StateID  stateId,
										 Payload&& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void restartWith  (const Payload& payload)			  noexcept	{ restartWith  (stateId<TState>(),			 payload );	}

	/// @brief Transition into a state (if transitioning into a region, activates the initial state)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void restartWith  (	 Payload&& payload)			  noexcept	{ restartWith  (stateId<TState>(), std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void resumeWith   (const StateID  stateId,
									const Payload& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void resumeWith   (const StateID  stateId,
										 Payload&& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void resumeWith   (const Payload& payload)			  noexcept	{ resumeWith   (stateId<TState>(),			 payload );	}

	/// @brief Transition into a state (if transitioning into a region, activates the state that was active previously)
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void resumeWith   (	 Payload&& payload)			  noexcept	{ resumeWith   (stateId<TState>(), std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void utilizeWith  (const StateID  stateId,
									const Payload& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void utilizeWith  (const StateID  stateId,
										 Payload&& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void utilizeWith  (const Payload& payload)			  noexcept	{ utilizeWith  (stateId<TState>(),			 payload );	}

	/// @brief Transition into a state (if transitioning into a region, activates the state
	///		with the highest 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void utilizeWith  (	 Payload&& payload)			  noexcept	{ utilizeWith  (stateId<TState>(), std::move(payload));	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void randomizeWith(const StateID  stateId,
									const Payload& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @param stateId Destination state identifier
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void randomizeWith(const StateID  stateId,
										 Payload&& payload)			  noexcept;

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void randomizeWith(const Payload& payload)			  noexcept	{ randomizeWith(stateId<TState>(),			 payload );	}

	/// @brief Transition into a state (if transitioning into a region, uses weighted random to activate the state
	///		proportional to 'utility()' among those with the highest 'rank()')
	/// @tparam TState Destination state type
	/// @param payload Payload
	/// @see HFSM2_ENABLE_UTILITY_THEORY
	template <typename TState>
	HFSM2_INLINE void randomizeWith(	 Payload&& payload)			  noexcept	{ randomizeWith(stateId<TState>(), std::move(payload));	}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void scheduleWith (const StateID  stateId,
									const Payload& payload)			  noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @param stateId Destination state identifier
	/// @param payload Payload
	HFSM2_INLINE void scheduleWith (const StateID  stateId,
										 Payload&& payload)			  noexcept;

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void scheduleWith (const Payload& payload)			  noexcept	{ scheduleWith (stateId<TState>(),			 payload );	}

	/// @brief Schedule a state to be activated when its parent region is activated
	/// @tparam TState Destination state type
	/// @param payload Payload
	template <typename TState>
	HFSM2_INLINE void scheduleWith (	 Payload&& payload)			  noexcept	{ scheduleWith (stateId<TState>(), std::move(payload));	}

	//------------------------------------------------------------------------------

protected:
	using Base::_context;

private:
	using Base::_requests;

	HFSM2_IF_LOG_INTERFACE(using Base::_logger);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFeatureTag
		, typename TContext

	#ifdef HFSM2_ENABLE_UTILITY_THEORY
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TApex>
class RP_		   <G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>
	: public	 R_<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>
{
	using Base = R_<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), void>, TApex>;

public:
#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using RNG		= typename Base::RNG;
#endif

public:
	using Base::Base;
};

////////////////////////////////////////////////////////////////////////////////

template <typename, typename>
class RC_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext

template <FeatureTag NFeatureTag
		, typename TContext

	#ifdef HFSM2_ENABLE_UTILITY_THEORY
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RC_			<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RP_<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RP_<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using Context	= TContext;
	using Payload	= typename Base::Payload;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using RNG		= typename Base::RNG;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Base::Logger;
#endif

public:
	HFSM2_INLINE explicit RC_(Context& context
							HFSM2_IF_UTILITY_THEORY(, RNG& rng)
							HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept
		: Base{context
			 HFSM2_IF_UTILITY_THEORY(, rng)
			 HFSM2_IF_LOG_INTERFACE(, logger)}
	{}

	HFSM2_INLINE void setContext(const Context&  context) noexcept { _context =			  context ; }
	HFSM2_INLINE void setContext(	   Context&& context) noexcept { _context = std::move(context); }

private:
	using Base::_context;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext&

template <FeatureTag NFeatureTag
		, typename TContext

	#ifdef HFSM2_ENABLE_UTILITY_THEORY
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RC_			<G_<NFeatureTag, TContext& HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RP_<G_<NFeatureTag, TContext& HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RP_<G_<NFeatureTag, TContext& HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using Context	= typename Base::Context;
	using Payload	= typename Base::Payload;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using RNG		= typename Base::RNG;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Base::Logger;
#endif

public:
	HFSM2_INLINE explicit RC_(Context context
							HFSM2_IF_UTILITY_THEORY(, RNG& rng)
							HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept
		: Base{context
			 HFSM2_IF_UTILITY_THEORY(, rng)
			 HFSM2_IF_LOG_INTERFACE(, logger)}
	{}

	HFSM2_INLINE void setContext(Context context) noexcept { _context = context; }

private:
	using Base::_context;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext*

template <FeatureTag NFeatureTag
		, typename TContext

	#ifdef HFSM2_ENABLE_UTILITY_THEORY
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RC_			<G_<NFeatureTag, TContext* HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RP_<G_<NFeatureTag, TContext* HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
{
	using Base = RP_<G_<NFeatureTag, TContext* HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using Context	= typename Base::Context;
	using Payload	= typename Base::Payload;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using RNG		= typename Base::RNG;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Base::Logger;
#endif

public:
#ifdef HFSM2_ENABLE_UTILITY_THEORY

	HFSM2_INLINE explicit RC_(Context context
							, RNG& rng
							HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept
		: Base{context
			 , rng
			 HFSM2_IF_LOG_INTERFACE(, logger)}
	{}

#else

	HFSM2_INLINE explicit RC_(Context context = nullptr
							HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept
		: Base{context
			 HFSM2_IF_LOG_INTERFACE(, logger)}
	{}

#endif

	HFSM2_INLINE void setContext(Context context) noexcept { _context = context; }

private:
	using Base::_context;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext == EmptyContext

template <FeatureTag NFeatureTag

	#ifdef HFSM2_ENABLE_UTILITY_THEORY
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RC_			<G_<NFeatureTag, EmptyContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	: public	 RP_<G_<NFeatureTag, EmptyContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	, public EmptyContext
{
	using Base = RP_<G_<NFeatureTag, EmptyContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using Context	= typename Base::Context;
	using Payload	= typename Base::Payload;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using RNG		= typename Base::RNG;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Base::Logger;
#endif

public:

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	HFSM2_INLINE explicit RC_(RNG& rng
							HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept
		: Base{static_cast<Context&>(*this)
			 , rng
			 HFSM2_IF_LOG_INTERFACE(, logger)}
	{}

#else

	HFSM2_INLINE explicit RC_(HFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr)) noexcept
		: Base{static_cast<Context&>(*this)
			 HFSM2_IF_LOG_INTERFACE(, logger)}
	{}

#endif
};

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <typename TConfig,
		  typename TApex>
class RR_ final
	: public	 RC_<TConfig, TApex>
{
	using Base = RC_<TConfig, TApex>;

public:
	using Base::Base;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TRNG == RNGT<TUtility>

#ifdef HFSM2_ENABLE_UTILITY_THEORY

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TContext

#ifdef HFSM2_ENABLE_UTILITY_THEORY
		, typename TRank
		, typename TUtility
#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RR_			<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, RNGT<TUtility>), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex> final
	: public	 RC_<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, RNGT<TUtility>), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	HFSM2_IF_UTILITY_THEORY(, public RNGT<TUtility>)
{
	using Base = RC_<G_<NFeatureTag, TContext HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, RNGT<TUtility>), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using Context	= typename Base::Context;
	using Payload	= typename Base::Payload;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using RNG		= typename Base::RNG;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Base::Logger;
#endif

public:
#ifdef HFSM2_ENABLE_UTILITY_THEORY

	HFSM2_INLINE explicit RR_(Context& context
							HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept
		: Base{context
			 , static_cast<RNG&>(*this)
			 HFSM2_IF_LOG_INTERFACE(, logger)}
		, RNG{0}
	{}

#else

	HFSM2_INLINE explicit RR_(Context& context
							HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr)) noexcept
		: Base{context
			 HFSM2_IF_LOG_INTERFACE(, logger)}
	{}

#endif
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext == EmptyContext
// TRNG == RNGT<TUtility>

/// @brief FSM Root
/// @tparam Cfg Type configuration
/// @tparam TApex Root region type
template <FeatureTag NFeatureTag
		, typename TRank
		, typename TUtility
		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload
		, typename TApex>
class RR_			<G_<NFeatureTag, EmptyContext, TRank, TUtility, RNGT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex> final
	: public	 RC_<G_<NFeatureTag, EmptyContext, TRank, TUtility, RNGT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>
	, public RNGT<TUtility>
{
	using Base = RC_<G_<NFeatureTag, EmptyContext, TRank, TUtility, RNGT<TUtility>, NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>, TApex>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using Context	= typename Base::Context;
	using Payload	= typename Base::Payload;
	using RNG		= typename Base::RNG;

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger	= typename Base::Logger;
#endif

public:
	HFSM2_INLINE explicit RR_(HFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr)) noexcept
		: Base{static_cast<RNG&>(*this)
			 HFSM2_IF_LOG_INTERFACE(, logger)}
		, RNG{0}
	{}
};

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#include "root.inl"
