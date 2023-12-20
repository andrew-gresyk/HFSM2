namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

/// @brief FSM Root
/// @tparam `TConfig` Type configuration
/// @tparam `TApex` Root region type
template <
	typename TConfig
  , typename TApex
>
class HFSM2_EMPTY_BASES InstanceT final
	: public	 RC_<TConfig, TApex>
{
	using Base = RC_<TConfig, TApex>;

public:
	using Base::Base;
};

#if HFSM2_UTILITY_THEORY_AVAILABLE()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TRNG == RNGT<TUtility>

/// @brief FSM Root
/// @tparam `TConfig` Type configuration
/// @tparam `TApex` Root region type
template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
  , typename TReactOrder
  , typename TRank
  , typename TUtility
  , Short NSubstitutionLimit
  HFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
  , typename TApex
>
class HFSM2_EMPTY_BASES InstanceT<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , TReactOrder
							  , TRank
							  , TUtility
							  , RNGT<TUtility>
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						> final
	: public			RC_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , TReactOrder
							  , TRank
							  , TUtility
							  , RNGT<TUtility>
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
	, public			RNGT<TUtility>
{
	using Base =		RC_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , TReactOrder
							  , TRank
							  , TUtility
							  , RNGT<TUtility>
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	HFSM2_CONSTEXPR(14)	explicit InstanceT(Context& context
										 HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept
		: Base{context
			 , static_cast<RNGT<TUtility>&>(*this)
			 HFSM2_IF_LOG_INTERFACE(, logger)}
		, RNGT<TUtility>{0}
	{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TContext == EmptyContext
// TRNG == RNGT<TUtility>

/// @brief FSM Root
/// @tparam `TConfig` Type configuration
/// @tparam `TApex` Root region type
template <
	FeatureTag NFeatureTag
  , typename TActivation
  , typename TReactOrder
  , typename TRank
  , typename TUtility
  , Short NSubstitutionLimit
  HFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
  , typename TApex
>
class HFSM2_EMPTY_BASES InstanceT<
							G_<
								NFeatureTag
							  , EmptyContext
							  , TActivation
							  , TReactOrder
							  , TRank
							  , TUtility
							  , RNGT<TUtility>
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						> final
	: public			RC_<
							G_<
								NFeatureTag
							  , EmptyContext
							  , TActivation
							  , TReactOrder
							  , TRank
							  , TUtility
							  , RNGT<TUtility>
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
	, public RNGT<TUtility>
{
	using Base =		RC_<
							G_<
								NFeatureTag
							  , EmptyContext
							  , TActivation
							  , TReactOrder
							  , TRank
							  , TUtility
							  , RNGT<TUtility>
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	HFSM2_CONSTEXPR(14)	explicit InstanceT(HFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr))	noexcept
		: Base{static_cast<RNGT<TUtility>&>(*this)
			 HFSM2_IF_LOG_INTERFACE(, logger)}
		, RNGT<TUtility>{0}
	{}
};

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
