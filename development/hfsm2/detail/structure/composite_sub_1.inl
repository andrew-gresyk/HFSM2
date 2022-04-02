namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideRegister(Registry& registry,
											  const Parent parent) noexcept
{
	LHalf::wideRegister(registry, Parent{parent.forkId, L_PRONG});
	RHalf::wideRegister(registry, Parent{parent.forkId, R_PRONG});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, TL_<TS...>>::wideForwardEntryGuard(GuardControl& control,
													   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return LHalf::wideForwardEntryGuard(control, prong);
	else
		return RHalf::wideForwardEntryGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, TL_<TS...>>::wideEntryGuard(GuardControl& control,
												const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return LHalf::wideEntryGuard(control, prong);
	else
		return RHalf::wideEntryGuard(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideEnter(PlanControl& control,
										   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideEnter(control, prong);
	else
		RHalf::wideEnter(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReenter(PlanControl& control,
											 const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideReenter(control, prong);
	else
		RHalf::wideReenter(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<TS...>>::widePreUpdate(FullControl& control,
											   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::widePreUpdate(control, prong) :
		RHalf::widePreUpdate(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<TS...>>::wideUpdate(FullControl& control,
											const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::wideUpdate(control, prong) :
		RHalf::wideUpdate(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<TS...>>::widePostUpdate(FullControl& control,
												const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::widePostUpdate(control, prong) :
		RHalf::widePostUpdate(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<TS...>>::widePreReact(FullControl& control,
											  const TEvent& event,
											  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::widePreReact(control, event, prong) :
		RHalf::widePreReact(control, event, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReact(FullControl& control,
										   const TEvent& event,
										   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::wideReact(control, event, prong) :
		RHalf::wideReact(control, event, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<TS...>>::widePostReact(FullControl& control,
											   const TEvent& event,
											   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::widePostReact(control, event, prong) :
		RHalf::widePostReact(control, event, prong);
}

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<TS...>>::wideUpdatePlans(FullControl& control,
												 const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		LHalf::wideUpdatePlans(control, prong) :
		RHalf::wideUpdatePlans(control, prong);
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, TL_<TS...>>::wideForwardExitGuard(GuardControl& control,
													  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return LHalf::wideForwardExitGuard(control, prong);
	else
		return RHalf::wideForwardExitGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, TL_<TS...>>::wideExitGuard(GuardControl& control,
											   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return LHalf::wideExitGuard(control, prong);
	else
		return RHalf::wideExitGuard(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideExit(PlanControl& control,
										  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideExit(control, prong);
	else
		RHalf::wideExit(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideForwardActive(Control& control,
												   const Request request,
												   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideForwardActive(control, request, prong);
	else
		RHalf::wideForwardActive(control, request, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideForwardRequest(Control& control,
													const Request request,
													const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideForwardRequest(control, request, prong);
	else
		RHalf::wideForwardRequest(control, request, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideRequestChangeComposite(Control& control,
															const Request request) noexcept
{
	LHalf::wideRequestChangeComposite(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideRequestChangeResumable(Control& control,
															const Request request,
															const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideRequestChangeResumable(control, request, prong);
	else
		RHalf::wideRequestChangeResumable(control, request, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideRequestChangeSelectable(Control& control,
															 const Request request,
															 const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideRequestChangeSelectable(control, request, prong);
	else
		RHalf::wideRequestChangeSelectable(control, request, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideRequestRestart(Control& control,
													const Request request) noexcept
{
	LHalf::wideRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideRequestResume(Control& control,
												   const Request request,
												   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideRequestResume(control, request, prong);
	else
		RHalf::wideRequestResume(control, request, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideRequestSelect(Control& control,
												   const Request request,
												   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideRequestSelect(control, request, prong);
	else
		RHalf::wideRequestSelect(control, request, prong);
}

//------------------------------------------------------------------------------

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReportUtilize(Control& control) noexcept {
	const UP l = LHalf::wideReportUtilize(control);
	const UP r = RHalf::wideReportUtilize(control);

	return l.utility >= r.utility ?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::Rank
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReportRank(Control& control,
												Rank* const ranks) noexcept
{
	HFSM2_ASSERT(ranks);

	const Rank l = LHalf::wideReportRank(control, ranks);
	const Rank r = RHalf::wideReportRank(control, ranks + LStateList::SIZE);

	return l >= r?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::Utility
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReportRandomize(Control& control,
													 Utility* const options,
													 const Rank* const ranks,
													 const Rank top) noexcept
{
	HFSM2_ASSERT(options && ranks);

	const Utility l = LHalf::wideReportRandomize(control, options,					  ranks,					top);
	const Utility r = RHalf::wideReportRandomize(control, options + LStateList::SIZE, ranks + LStateList::SIZE, top);

	return { l + r };
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReportChangeComposite(Control& control) noexcept {
	return LHalf::wideReportChangeComposite(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReportChangeResumable(Control& control,
														   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return LHalf::wideReportChangeResumable(control, prong);
	else
		return RHalf::wideReportChangeResumable(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReportChangeSelectable(Control& control,
															const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return LHalf::wideReportChangeSelectable(control, prong);
	else
		return RHalf::wideReportChangeSelectable(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReportChangeUtilitarian(Control& control) noexcept {
	const UP l = LHalf::wideReportChangeUtilitarian(control);
	const UP r = RHalf::wideReportChangeUtilitarian(control);

	return l.utility >= r.utility ?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
typename TA::Utility
CS_<TN, TA, SG, NI, TL_<TS...>>::wideReportChangeRandom(Control& control,
														Utility* const options,
														const Rank* const ranks,
														const Rank top) noexcept
{
	HFSM2_ASSERT(options && ranks);

	const Utility l = LHalf::wideReportChangeRandom(control, options,					 ranks,					   top);
	const Utility r = RHalf::wideReportChangeRandom(control, options + LStateList::SIZE, ranks + LStateList::SIZE, top);

	return { l + r };
}

#endif

//------------------------------------------------------------------------------
// COMMON

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideChangeToRequested(PlanControl& control,
													   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		LHalf::wideChangeToRequested(control, prong);
	else
		RHalf::wideChangeToRequested(control, prong);
}

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideSaveActive(const Registry& registry,
												WriteStream& stream,
												const Short prong) const noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG) {
		LHalf::wideSaveActive   (registry, stream, prong);
		RHalf::wideSaveResumable(registry, stream);
	} else {
		LHalf::wideSaveResumable(registry, stream);
		RHalf::wideSaveActive   (registry, stream, prong);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideSaveResumable(const Registry& registry,
												   WriteStream& stream) const noexcept
{
	LHalf::wideSaveResumable(registry, stream);
	RHalf::wideSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideLoadRequested(Registry& registry,
												   ReadStream& stream,
												   const Short prong) const noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG) {
		LHalf::wideLoadRequested(registry, stream, prong);
		RHalf::wideLoadResumable(registry, stream);
	} else {
		LHalf::wideLoadResumable(registry, stream);
		RHalf::wideLoadRequested(registry, stream, prong);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideLoadResumable(Registry& registry,
												   ReadStream& stream) const noexcept
{
	LHalf::wideLoadResumable(registry, stream);
	RHalf::wideLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<TS...>>::wideGetNames(const Long parent,
											  const RegionType /*region*/,
											  const Short depth,
											  StructureStateInfos& _stateInfos) const noexcept
{
	LHalf::wideGetNames(parent, StructureStateInfo::RegionType::COMPOSITE, depth, _stateInfos);
	RHalf::wideGetNames(parent, StructureStateInfo::RegionType::COMPOSITE, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
