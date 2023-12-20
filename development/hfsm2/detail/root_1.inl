namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFT_, typename TC_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
RV_<G_<NFT_, TC_, Automatic, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::RV_(Context& context
																													   HFSM2_IF_UTILITY_THEORY(, RNG& rng)
																													   HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{context
		 HFSM2_IF_UTILITY_THEORY(, rng)
		 HFSM2_IF_LOG_INTERFACE(, logger)}
{
	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
RV_<G_<NFT_, TC_, Automatic, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::RV_(PureContext&& context
																													   HFSM2_IF_UTILITY_THEORY(, RNG& rng)
																													   HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: Base{move(context)
		 HFSM2_IF_UTILITY_THEORY(, rng)
		 HFSM2_IF_LOG_INTERFACE(, logger)}
{
	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
RV_<G_<NFT_, TC_, Automatic, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::RV_(const RV_& other) noexcept
	: Base{other}
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
RV_<G_<NFT_, TC_, Automatic, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::RV_(RV_&& other) noexcept
	: Base{move(other)}
{}

//------------------------------------------------------------------------------

template <FeatureTag NFT_, typename TC_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(20)
RV_<G_<NFT_, TC_, Automatic, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::~RV_() noexcept {
	finalExit();
}

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <FeatureTag NFT_, typename TC_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
void
RV_<G_<NFT_, TC_, Automatic, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::save(SerialBuffer& buffer) const noexcept {
	WriteStream stream{buffer};

	stream.template write<1>(1);
	save(stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
void
RV_<G_<NFT_, TC_, Automatic, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::load(const SerialBuffer& buffer) noexcept {
	ReadStream stream{buffer};

	if (HFSM2_CHECKED(stream.template read<1>()))
		Base::load(stream);
}

#endif

////////////////////////////////////////////////////////////////////////////////

#if HFSM2_SERIALIZATION_AVAILABLE()

template <FeatureTag NFT_, typename TC_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
void
RV_<G_<NFT_, TC_, Manual, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::save(SerialBuffer& buffer) const noexcept {
	WriteStream stream{buffer};

	if (isActive()) {
		stream.template write<1>(1);

		save(stream);
	}
	else
		stream.template write<1>(0);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
void
RV_<G_<NFT_, TC_, Manual, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::load(const SerialBuffer& buffer) noexcept {
	ReadStream stream{buffer};

	if (stream.template read<1>()) {
		if (isActive())
			Base::load(stream);
		else
			loadEnter (stream);
	}
	else
		if (isActive())
			finalExit();
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

template <FeatureTag NFT_, typename TC_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
bool
RV_<G_<NFT_, TC_, Manual, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::replayEnter(const Transition* const transitions,
																															  const Short count) noexcept
{
	HFSM2_ASSERT(!_core.registry.isActive());
	HFSM2_ASSERT(_core.requests.count() == 0);

	_core.transitionTargets.clear();
	HFSM2_ASSERT(_core.previousTransitions.count() == 0);

	if (HFSM2_CHECKED(transitions && count)) {
		TransitionSets emptyTransitions;
		PlanControl control{_core, emptyTransitions};

		if (HFSM2_CHECKED(applyRequests(control, transitions, count))) {
			for (Short i = 0; i < count; ++i)
				_core.previousTransitions.emplace(transitions[i]);

			_apex.deepEnter(control);

			_core.registry.clearRequests();

			HFSM2_IF_ASSERT(HFSM2_IF_PLANS(_core.planData.verifyPlans()));
			HFSM2_IF_STRUCTURE_REPORT(udpateActivity());

			return true;
		}
	}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT_, typename TC_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
template <Long NCount>
HFSM2_CONSTEXPR(14)
bool
RV_<G_<NFT_, TC_, Manual, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::replayEnter(const DynamicArrayT<Transition, NCount>& transitions)  noexcept {
	if (transitions.count())
		return replayEnter(&transitions[0],
						   transitions.count());
	else
		return false;
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <FeatureTag NFT_, typename TC_, typename TRO_ HFSM2_IF_UTILITY_THEORY(, typename TR_, typename TU_, typename TG_), Short NSL_ HFSM2_IF_PLANS(, Long NTC_), typename TP_, typename TA_>
HFSM2_CONSTEXPR(14)
void
RV_<G_<NFT_, TC_, Manual, TRO_ HFSM2_IF_UTILITY_THEORY(, TR_, TU_, TG_), NSL_ HFSM2_IF_PLANS(, NTC_), TP_>, TA_>::loadEnter(ReadStream& stream) noexcept {
	HFSM2_ASSERT(_core.registry.empty());
	_apex.deepLoadRequested(_core.registry, stream);

	HFSM2_ASSERT(_core.requests.empty());

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_ASSERT(_core.planData.empty() == 0);
#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	HFSM2_ASSERT(_core.transitionTargets  .empty());
	HFSM2_ASSERT(_core.previousTransitions.empty());
#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	//HFSM2_ASSERT(_activityHistory.empty());
#endif

	TransitionSets emptyTransitions;
	PlanControl control{_core, emptyTransitions};

	_apex.deepEnter(control);

	HFSM2_IF_STRUCTURE_REPORT(udpateActivity());
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
