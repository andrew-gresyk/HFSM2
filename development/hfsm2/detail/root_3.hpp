namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <
	typename
  , typename
>
class RC_;

//------------------------------------------------------------------------------
// TContext

template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
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
class HFSM2_EMPTY_BASES RC_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
	: public			RP_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
{
	using Base =		RP_<
							G_<
								NFeatureTag
							  , TContext
							  , TActivation
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;
	using typename Base::PureContext;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	HFSM2_CONSTEXPR(14)	explicit RC_(Context& context
								   HFSM2_IF_UTILITY_THEORY(, RNG& rng)
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	HFSM2_CONSTEXPR(14)	explicit RC_(PureContext&& context
								   HFSM2_IF_UTILITY_THEORY(, RNG& rng)
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

	HFSM2_CONSTEXPR(NO) RC_(const RC_& )														noexcept = default;
	HFSM2_CONSTEXPR(NO) RC_(	  RC_&&)														noexcept = default;

private:
	using Base::_core;
};

//------------------------------------------------------------------------------
// TContext&

template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
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
class HFSM2_EMPTY_BASES RC_<
							G_<
								NFeatureTag
							  , TContext&
							  , TActivation
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
	: public			RP_<
							G_<
								NFeatureTag
							  , TContext&
							  , TActivation
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
{
	using Base =		RP_<
							G_<
								NFeatureTag
							  , TContext&
							  , TActivation
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
	using Base::Base;

private:
	using Base::_core;
};

//------------------------------------------------------------------------------
// TContext*

template <
	FeatureTag NFeatureTag
  , typename TContext
  , typename TActivation
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
class HFSM2_EMPTY_BASES RC_<
							G_<
								NFeatureTag
							  , TContext*
							  , TActivation
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
	: public			RP_<
							G_<
								NFeatureTag
							  , TContext*
							  , TActivation
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
{
	using Base =		RP_<
							G_<
								NFeatureTag
							  , TContext*
							  , TActivation
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

	using typename Base::Context;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:
#if HFSM2_UTILITY_THEORY_AVAILABLE()

	HFSM2_CONSTEXPR(14)	explicit RC_(Context context
								   , RNG& rng
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

#else

	HFSM2_CONSTEXPR(14)	explicit RC_(Context context = nullptr
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

#endif

	HFSM2_CONSTEXPR(NO)	RC_(const RC_& )														noexcept = default;
	HFSM2_CONSTEXPR(NO)	RC_(	  RC_&&)														noexcept = default;

	HFSM2_CONSTEXPR(14)	void setContext(Context context)										noexcept	{ _core.context = context; }

private:
	using Base::_core;
};

//------------------------------------------------------------------------------
// TContext == EmptyContext

template <
	FeatureTag NFeatureTag
  , typename TActivation
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
class HFSM2_EMPTY_BASES RC_<
							G_<
								NFeatureTag
							  , EmptyContext
							  , TActivation
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
	: public			RP_<
							G_<
								NFeatureTag
							  , EmptyContext
							  , TActivation
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>
	, public	 EmptyContext
{
	using Base =		RP_<
							G_<
								NFeatureTag
							  , EmptyContext
							  , TActivation
							  , TReactOrder
							  HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG)
							  , NSubstitutionLimit
							  HFSM2_IF_PLANS(, NTaskCapacity)
							  , TPayload
							>
						  , TApex
						>;

public:
	static constexpr FeatureTag FEATURE_TAG = Base::FEATURE_TAG;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename Base::RNG;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename Base::Logger;
#endif

public:

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	HFSM2_CONSTEXPR(14)	explicit RC_(RNG& rng
								   HFSM2_IF_LOG_INTERFACE(, Logger* const logger = nullptr))	noexcept;

#else

	HFSM2_CONSTEXPR(14)	explicit RC_(HFSM2_IF_LOG_INTERFACE(Logger* const logger = nullptr))	noexcept;

#endif

	HFSM2_CONSTEXPR(NO)	RC_(const RC_& )														noexcept = default;
	HFSM2_CONSTEXPR(NO)	RC_(	  RC_&&)														noexcept = default;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "root_3.inl"
