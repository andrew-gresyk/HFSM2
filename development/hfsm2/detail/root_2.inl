namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <FeatureTag NFT_, typename TC_, typename TV_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT_, TC_, TV_, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::changeWith(const StateID  stateId_,
																														  const Payload& payload) noexcept
{
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::CHANGE, payload});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::CHANGE, stateId_);
}

// COMMON
//------------------------------------------------------------------------------

template <FeatureTag NFT_, typename TC_, typename TV_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT_, TC_, TV_, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::restartWith(const StateID  stateId_,
																														   const Payload& payload) noexcept
{
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::RESTART, payload});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::RESTART, stateId_);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, typename TV_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT_, TC_, TV_, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::resumeWith(const StateID  stateId_,
																														  const Payload& payload) noexcept
{
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::RESUME, payload});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::RESUME, stateId_);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, typename TV_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT_, TC_, TV_, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::selectWith(const StateID  stateId_,
																														  const Payload& payload) noexcept
{
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::SELECT, payload});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::SELECT, stateId_);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <FeatureTag NFT_, typename TC_, typename TV_, typename TRO_, typename TR_, typename TU_, typename TG_, Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT_, TC_, TV_, TRO_, TR_, TU_, TG_, NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::utilizeWith(const StateID  stateId_,
																								 const Payload& payload) noexcept
{
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::UTILIZE, payload});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::UTILIZE, stateId_);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, typename TV_, typename TRO_, typename TR_, typename TU_, typename TG_, Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT_, TC_, TV_, TRO_, TR_, TU_, TG_, NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::randomizeWith(const StateID  stateId_,
																								   const Payload& payload) noexcept
{
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::RANDOMIZE, payload});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::RANDOMIZE, stateId_);
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, typename TV_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT_, TC_, TV_, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::scheduleWith(const StateID  stateId_,
																															const Payload& payload) noexcept
{
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::SCHEDULE, payload});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::SCHEDULE, stateId_);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, typename TV_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT_, TC_, TV_, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::immediateChangeWith(const StateID  stateId_,
																																   const Payload& payload) noexcept
{
	changeWith(stateId_, payload);

	processRequest();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, typename TV_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT_, TC_, TV_, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::immediateRestartWith(const StateID  stateId_,
																																	const Payload& payload) noexcept
{
	restartWith(stateId_, payload);

	processRequest();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, typename TV_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT_, TC_, TV_, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::immediateResumeWith(const StateID  stateId_,
																																   const Payload& payload) noexcept
{
	resumeWith(stateId_, payload);

	processRequest();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, typename TV_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT_, TC_, TV_, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::immediateSelectWith(const StateID  stateId_,
																																   const Payload& payload) noexcept
{
	selectWith(stateId_, payload);

	processRequest();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <FeatureTag NFT_, typename TC_, typename TV_, typename TRO_, typename TR_, typename TU_, typename TG_, Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT_, TC_, TV_, TRO_, TR_, TU_, TG_, NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::immediateUtilizeWith(const StateID  stateId_,
																										  const Payload& payload) noexcept
{
	utilizeWith(stateId_, payload);

	processRequest();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, typename TV_, typename TRO_, typename TR_, typename TU_, typename TG_, Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT_, TC_, TV_, TRO_, TR_, TU_, TG_, NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::immediateRandomizeWith(const StateID  stateId_,
																											const Payload& payload) noexcept
{
	randomizeWith(stateId_, payload);

	processRequest();
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
