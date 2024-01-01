namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideRegister(Registry& registry,
												   const Parent parent) noexcept
{
	LHalf::wideRegister(registry, Parent{parent.forkId, L_PRONG});
	RHalf::wideRegister(registry, Parent{parent.forkId, R_PRONG});
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
bool
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideForwardEntryGuard(GuardControl& control,
															const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return LHalf::wideForwardEntryGuard(control, prong);
	else
		return RHalf::wideForwardEntryGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
bool
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideEntryGuard(GuardControl& control,
													 const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		return LHalf::wideEntryGuard(control, prong);
	else
		return RHalf::wideEntryGuard(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideEnter(PlanControl& control,
												const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		LHalf::wideEnter(control, prong);
	else
		RHalf::wideEnter(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideReenter(PlanControl& control,
												  const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		LHalf::wideReenter(control, prong);
	else
		RHalf::wideReenter(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
TaskStatus
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::widePreUpdate(FullControl& control,
													const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::widePreUpdate(control, prong) :
		RHalf::widePreUpdate(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
TaskStatus
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideUpdate(FullControl& control,
												 const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::wideUpdate(control, prong) :
		RHalf::wideUpdate(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
TaskStatus
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::widePostUpdate(FullControl& control,
													 const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::widePostUpdate(control, prong) :
		RHalf::widePostUpdate(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::widePreReact(EventControl& control,
												   const TEvent& event,
												   const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::widePreReact(control, event, prong) :
		RHalf::widePreReact(control, event, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideReact(EventControl& control,
												const TEvent& event,
												const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::wideReact(control, event, prong) :
		RHalf::wideReact(control, event, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::widePostReact(EventControl& control,
													const TEvent& event,
													const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::widePostReact(control, event, prong) :
		RHalf::widePostReact(control, event, prong);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideQuery(ConstControl& control,
												TEvent& event,
												const Prong prong) const noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::wideQuery(control, event, prong) :
		RHalf::wideQuery(control, event, prong);
}

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
TaskStatus
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideUpdatePlans(FullControl& control,
													  const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	return prong < R_PRONG ?
		LHalf::wideUpdatePlans(control, prong) :
		RHalf::wideUpdatePlans(control, prong);
}

#endif

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
bool
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideForwardExitGuard(GuardControl& control,
														   const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		return LHalf::wideForwardExitGuard(control, prong);
	else
		return RHalf::wideForwardExitGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
bool
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideExitGuard(GuardControl& control,
													const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		return LHalf::wideExitGuard(control, prong);
	else
		return RHalf::wideExitGuard(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideExit(PlanControl& control,
											   const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		LHalf::wideExit(control, prong);
	else
		RHalf::wideExit(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideForwardActive(Control& control,
														const Request request,
														const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		LHalf::wideForwardActive(control, request, prong);
	else
		RHalf::wideForwardActive(control, request, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideForwardRequest(Control& control,
														 const Request request,
														 const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		LHalf::wideForwardRequest(control, request, prong);
	else
		RHalf::wideForwardRequest(control, request, prong);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideRequestChangeComposite(Control& control,
																 const Request request) noexcept
{
	LHalf::wideRequestChangeComposite(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideRequestChangeResumable(Control& control,
																 const Request request,
																 const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		LHalf::wideRequestChangeResumable(control, request, prong);
	else
		RHalf::wideRequestChangeResumable(control, request, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideRequestChangeSelectable(Control& control,
																  const Request request,
																  const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		LHalf::wideRequestChangeSelectable(control, request, prong);
	else
		RHalf::wideRequestChangeSelectable(control, request, prong);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideRequestRestart(Control& control,
														 const Request request) noexcept
{
	LHalf::wideRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideRequestResume(Control& control,
														const Request request,
														const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		LHalf::wideRequestResume(control, request, prong);
	else
		RHalf::wideRequestResume(control, request, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideRequestSelect(Control& control,
														const Request request,
														const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		LHalf::wideRequestSelect(control, request, prong);
	else
		RHalf::wideRequestSelect(control, request, prong);
}

//------------------------------------------------------------------------------

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::UP
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideReportUtilize(Control& control) noexcept {
	const UP l = LHalf::wideReportUtilize(control);
	const UP r = RHalf::wideReportUtilize(control);

	return l.utility >= r.utility ?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::Rank
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideReportRank(Control& control,
													 Rank* const ranks) noexcept
{
	HFSM2_ASSERT(ranks);

	const Rank l = LHalf::wideReportRank(control, ranks);
	const Rank r = RHalf::wideReportRank(control, ranks + LStateList::SIZE);

	return l >= r ?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::Utility
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideReportRandomize(Control& control,
														  Utility* const utilities,
														  const Rank* const ranks,
														  const Rank top) noexcept
{
	HFSM2_ASSERT(utilities && ranks);

	const Utility l = LHalf::wideReportRandomize(control, utilities,					ranks,					  top);
	const Utility r = RHalf::wideReportRandomize(control, utilities + LStateList::SIZE, ranks + LStateList::SIZE, top);

	return { l + r };
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::UP
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideReportChangeComposite(Control& control) noexcept {
	return LHalf::wideReportChangeComposite(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::UP
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideReportChangeResumable(Control& control,
																const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		return LHalf::wideReportChangeResumable(control, prong);
	else
		return RHalf::wideReportChangeResumable(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::UP
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideReportChangeSelectable(Control& control,
																 const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		return LHalf::wideReportChangeSelectable(control, prong);
	else
		return RHalf::wideReportChangeSelectable(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::UP
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideReportChangeUtilitarian(Control& control) noexcept {
	const UP l = LHalf::wideReportChangeUtilitarian(control);
	const UP r = RHalf::wideReportChangeUtilitarian(control);

	return l.utility >= r.utility ?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
typename TA_::Utility
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideReportChangeRandom(Control& control,
															 Utility* const utilities,
															 const Rank* const ranks,
															 const Rank top) noexcept
{
	HFSM2_ASSERT(utilities && ranks);

	const Utility l = LHalf::wideReportChangeRandom(control, utilities,					   ranks,					 top);
	const Utility r = RHalf::wideReportChangeRandom(control, utilities + LStateList::SIZE, ranks + LStateList::SIZE, top);

	return { l + r };
}

#endif

//------------------------------------------------------------------------------
// COMMON

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideChangeToRequested(PlanControl& control,
															const Prong prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG)
		LHalf::wideChangeToRequested(control, prong);
	else
		RHalf::wideChangeToRequested(control, prong);
}

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideSaveActive(const Registry& registry,
													 WriteStream& stream,
													 const Prong prong) const noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG) {
		LHalf::wideSaveActive   (registry, stream, prong);
		RHalf::wideSaveResumable(registry, stream);
	} else {
		LHalf::wideSaveResumable(registry, stream);
		RHalf::wideSaveActive   (registry, stream, prong);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideSaveResumable(const Registry& registry,
														WriteStream& stream) const noexcept
{
	LHalf::wideSaveResumable(registry, stream);
	RHalf::wideSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideLoadRequested(Registry& registry,
														ReadStream& stream,
														const Prong prong) const noexcept
{
	HFSM2_ASSERT(prong != INVALID_PRONG);

	if (prong < R_PRONG) {
		LHalf::wideLoadRequested(registry, stream, prong);
		RHalf::wideLoadResumable(registry, stream);
	} else {
		LHalf::wideLoadResumable(registry, stream);
		RHalf::wideLoadRequested(registry, stream, prong);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideLoadResumable(Registry& registry,
														ReadStream& stream) const noexcept
{
	LHalf::wideLoadResumable(registry, stream);
	RHalf::wideLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename... TS_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<TS_...>>::wideGetNames(const Long parent,
												   const RegionType /*region*/,
												   const Short depth,
												   StructureStateInfos& stateInfos) const noexcept
{
	LHalf::wideGetNames(parent, RegionType::COMPOSITE, depth, stateInfos);
	RHalf::wideGetNames(parent, RegionType::COMPOSITE, depth, stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
