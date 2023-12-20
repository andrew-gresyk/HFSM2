namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFT_, typename TC_, typename TV_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
RC_<G_<NFT_, TC_, TV_, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::RC_(Context& context
																												 HFSM2_IF_UTILITY_THEORY(, RNG& rng)
																												 HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
		 HFSM2_IF_UTILITY_THEORY(, rng)
		 HFSM2_IF_LOG_INTERFACE(, logger)}
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, typename TV_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
RC_<G_<NFT_, TC_, TV_, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::RC_(PureContext&& context
																												 HFSM2_IF_UTILITY_THEORY(, RNG& rng)
																												 HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{move(context)
		 HFSM2_IF_UTILITY_THEORY(, rng)
		 HFSM2_IF_LOG_INTERFACE(, logger)}
{}

////////////////////////////////////////////////////////////////////////////////

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <FeatureTag NFT_, typename TC_, typename TV_, typename TRO_, typename TR_, typename TU_, typename TG_, Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
RC_<G_<NFT_, TC_*, TV_, TRO_, TR_, TU_, TG_, NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::RC_(Context context
																						, RNG& rng
																						HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
		 , rng
		 HFSM2_IF_LOG_INTERFACE(, logger)}
{}

#else

template <FeatureTag NFT_, typename TC_, typename TV_, typename TRO_, Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
RC_<G_<NFT_, TC_*, TV_, TRO_, NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::RC_(Context context
																		 HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
		 HFSM2_IF_LOG_INTERFACE(, logger)}
{}

#endif

////////////////////////////////////////////////////////////////////////////////

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <FeatureTag NFT_, typename TV_, typename TRO_, typename TR_, typename TU_, typename TG_, Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
RC_<G_<NFT_, EmptyContext, TV_, TRO_, TR_, TU_, TG_, NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::RC_(RNG& rng
																								HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{static_cast<EmptyContext&>(*this)
		 , rng
		 HFSM2_IF_LOG_INTERFACE(, logger)}
{}

#else

template <FeatureTag NFT_, typename TV_, typename TRO_, Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
RC_<G_<NFT_, EmptyContext, TV_, TRO_, NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::RC_(HFSM2_IF_LOG_INTERFACE(Logger* const logger)) noexcept
	: Base{static_cast<EmptyContext&>(*this)
		 HFSM2_IF_LOG_INTERFACE(, logger)}
{}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
