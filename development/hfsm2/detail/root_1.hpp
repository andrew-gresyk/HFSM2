namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

// Automatic / manual [de]activation

template <
	typename
  , typename
>
class RV_;

//------------------------------------------------------------------------------
// Automatic enter() / exit()

template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TReactOrder

#if HFSM2_UTILITY_THEORY_AVAILABLE()
  , typename TRank
  , typename TUtility
  , typename TRNG
#endif

  , Short NSubstitutionLimit
  HFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
  , typename TApex
>
class HFSM2_EMPTY_BASES RV_<
							G_<
								NFeatureTag
							  , TContext
							  , Automatic
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
	: public			R_ <
							G_<
								NFeatureTag
							  , TContext
							  , Automatic
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
{
	using Base =		R_ <
							G_<
								NFeatureTag
							  , TContext
							  , Automatic
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

protected:
	using typename Base::Context;
	using typename Base::PureContext;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

#if HFSM2_SERIALIZATION_AVAILABLE()
	using typename Base::Args;
	using typename Base::WriteStream;
	using typename Base::ReadStream;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	HFSM2_CONSTEXPR(14)	explicit RV_(Context& context
								   HFSM2_IF_UTILITY_THEORY(, RNG& rng)
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	HFSM2_CONSTEXPR(14)	explicit RV_(PureContext&& context
								   HFSM2_IF_UTILITY_THEORY(, RNG& rng)
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	HFSM2_CONSTEXPR(14)	RV_(const RV_&  other)													noexcept;
	HFSM2_CONSTEXPR(14)	RV_(	  RV_&& other)													noexcept;

	HFSM2_CONSTEXPR(20)	~RV_()																	noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_SERIALIZATION_AVAILABLE()

	/// @brief Buffer for serialization
	/// @see `HFSM2_ENABLE_SERIALIZATION`
	using SerialBuffer			= typename Args::SerialBuffer;

	/// @brief Serialize FSM into 'buffer'
	/// @param buffer `SerialBuffer` to serialize to
	/// @see `HFSM2_ENABLE_SERIALIZATION`
	HFSM2_CONSTEXPR(14)	void save(		SerialBuffer& buffer)							  const noexcept;

	/// @brief De-serialize FSM from 'buffer'
	/// @param buffer `SerialBuffer` to de-serialize from
	/// @see `HFSM2_ENABLE_SERIALIZATION`
	HFSM2_CONSTEXPR(14)	void load(const SerialBuffer& buffer)									noexcept;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
#if HFSM2_SERIALIZATION_AVAILABLE()
	using Base::save;
	using Base::load;
#endif

private:
	using Base::initialEnter;
	using Base::finalExit;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Manual enter() / exit()

template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TReactOrder

#if HFSM2_UTILITY_THEORY_AVAILABLE()
  , typename TRank
  , typename TUtility
  , typename TRNG
#endif

  , Short NSubstitutionLimit
  HFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
  , typename TApex
>
class HFSM2_EMPTY_BASES RV_<
							G_<
								NFeatureTag
							  , TContext
							  , Manual
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
	: public			R_<
							G_<
								NFeatureTag
							  , TContext
							  , Manual
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
{
	using Base =		R_<
							G_<
								NFeatureTag
							  , TContext
							  , Manual
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

public:
	using typename Base::Transition;

protected:
#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

#if HFSM2_SERIALIZATION_AVAILABLE()
	using typename Base::Args;
	using typename Base::WriteStream;
	using typename Base::ReadStream;
#endif

#if HFSM2_SERIALIZATION_AVAILABLE() || HFSM2_TRANSITION_HISTORY_AVAILABLE()
	using typename Base::PlanControl;
	using typename Base::TransitionSets;
#endif

public:
	using Base::Base;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if FSM is active
	/// @return FSM active status
	HFSM2_CONSTEXPR(11)	bool isActive()													  const noexcept	{ return _core.registry.isActive();			}

	using Base::isActive;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Manually start the FSM
	///	  Can be used with UE4 to start / stop the FSM in `BeginPlay()` / `EndPlay()`
	HFSM2_CONSTEXPR(14)	void enter()															noexcept	{ initialEnter(					);			}

	/// @brief Manually stop the FSM
	///   Can be used with UE4 to start / stop the FSM in `BeginPlay()` / `EndPlay()`
	HFSM2_CONSTEXPR(14)	void exit()																noexcept	{ finalExit();								}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_SERIALIZATION_AVAILABLE()

	/// @brief Buffer for serialization
	/// @see `HFSM2_ENABLE_SERIALIZATION`
	using SerialBuffer			= typename Args::SerialBuffer;

	/// @brief Serialize FSM into 'buffer'
	/// @param buffer `SerialBuffer` to serialize to
	/// @see `HFSM2_ENABLE_SERIALIZATION`
	HFSM2_CONSTEXPR(14)	void save(		SerialBuffer& buffer)							  const noexcept;

	/// @brief De-serialize FSM from 'buffer'
	/// @param buffer `SerialBuffer` to de-serialize from
	/// @see `HFSM2_ENABLE_SERIALIZATION`
	HFSM2_CONSTEXPR(14)	void load(const SerialBuffer& buffer)									noexcept;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

	/// @brief Start the FSM from a specific state
	///   Can be used to synchronize multiple FSMs
	/// @param `transitions` Array of 'Transition's to replay
	/// @param `count` Number of transitions
	/// @see `HFSM2_ENABLE_TRANSITION_HISTORY`
	HFSM2_CONSTEXPR(14)	bool replayEnter(const Transition* const transitions,
										 const Short count)										noexcept;

	/// @brief Start the FSM from a specific state
	///   Can be used to synchronize multiple FSMs
	/// @param `transitions` Array of 'Transition's to replay
	/// @see `HFSM2_ENABLE_TRANSITION_HISTORY`
	template <Long NCount>
	HFSM2_CONSTEXPR(14)	bool replayEnter(const DynamicArrayT<Transition, NCount>& transitions)	noexcept	{ return replayEnter(&transitions[0],
																																 transitions.count());	}

	/// @brief Start the FSM from a specific state
	///   Can be used to synchronize multiple FSMs
	/// @param `transition` 'Transition' to replay
	/// @see `HFSM2_ENABLE_TRANSITION_HISTORY`
	HFSM2_CONSTEXPR(14)	bool replayEnter(const Transition& transition)							noexcept	{ return replayEnter(&transition, 1);		}

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
#if HFSM2_SERIALIZATION_AVAILABLE()
	using Base::save;
	using Base::load;

	HFSM2_CONSTEXPR(14)	void loadEnter(ReadStream& stream)										noexcept;
#endif

protected:
	using Base::initialEnter;
	using Base::finalExit;

	using Base::_core;

#if HFSM2_SERIALIZATION_AVAILABLE() || HFSM2_TRANSITION_HISTORY_AVAILABLE()
	using Base::_apex;
#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	using Base::applyRequests;
	HFSM2_IF_STRUCTURE_REPORT(using Base::udpateActivity);
#endif
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "root_1.inl"
