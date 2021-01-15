namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
template <typename T>
T&
CS_<TN, TA, SG, NI, TS...>::access() noexcept {
	return contains<typename LHalfInfo::StateList, T>() ?
		lHalf.template access<T>() :
		rHalf.template access<T>();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
template <typename T>
const T&
CS_<TN, TA, SG, NI, TS...>::access() const noexcept {
	return contains<typename LHalfInfo::StateList, T>() ?
		lHalf.template access<T>() :
		rHalf.template access<T>();
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideRegister(Registry& registry,
										 const Parent parent) noexcept
{
	lHalf.wideRegister(registry, Parent{parent.forkId, L_PRONG});
	rHalf.wideRegister(registry, Parent{parent.forkId, R_PRONG});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
bool
CS_<TN, TA, SG, NI, TS...>::wideForwardEntryGuard(GuardControl& control,
												  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return lHalf.wideForwardEntryGuard(control, prong);
	else
		return rHalf.wideForwardEntryGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
bool
CS_<TN, TA, SG, NI, TS...>::wideEntryGuard(GuardControl& control,
										   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return lHalf.wideEntryGuard(control, prong);
	else
		return rHalf.wideEntryGuard(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideConstruct(PlanControl& control,
										  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideConstruct(control, prong);
	else
		rHalf.wideConstruct(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideEnter(PlanControl& control,
									  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideEnter(control, prong);
	else
		rHalf.wideEnter(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideReenter(PlanControl& control,
										const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideReenter(control, prong);
	else
		rHalf.wideReenter(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
Status
CS_<TN, TA, SG, NI, TS...>::wideUpdate(FullControl& control,
									   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		lHalf.wideUpdate(control, prong) :
		rHalf.wideUpdate(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
template <typename TEvent>
Status
CS_<TN, TA, SG, NI, TS...>::wideReact(FullControl& control,
									  const TEvent& event,
									  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	return prong < R_PRONG ?
		lHalf.wideReact(control, event, prong) :
		rHalf.wideReact(control, event, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
bool
CS_<TN, TA, SG, NI, TS...>::wideForwardExitGuard(GuardControl& control,
												 const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return lHalf.wideForwardExitGuard(control, prong);
	else
		return rHalf.wideForwardExitGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
bool
CS_<TN, TA, SG, NI, TS...>::wideExitGuard(GuardControl& control,
										  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return lHalf.wideExitGuard(control, prong);
	else
		return rHalf.wideExitGuard(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideExit(PlanControl& control,
									 const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideExit(control, prong);
	else
		rHalf.wideExit(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideDestruct(PlanControl& control,
										 const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideDestruct(control, prong);
	else
		rHalf.wideDestruct(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideForwardActive(Control& control,
											  const Request request,
											  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideForwardActive(control, request, prong);
	else
		rHalf.wideForwardActive(control, request, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideForwardRequest(Control& control,
											   const Request request,
											   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideForwardRequest(control, request, prong);
	else
		rHalf.wideForwardRequest(control, request, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideRequestChangeComposite(Control& control,
													   const Request request) noexcept
{
	lHalf.wideRequestChangeComposite(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideRequestChangeResumable(Control& control,
													   const Request request,
													   const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideRequestChangeResumable(control, request, prong);
	else
		rHalf.wideRequestChangeResumable(control, request, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideRequestRestart(Control& control,
											   const Request request) noexcept
{
	lHalf.wideRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideRequestResume(Control& control,
											  const Request request,
											  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideRequestResume(control, request, prong);
	else
		rHalf.wideRequestResume(control, request, prong);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
typename TA::UP
CS_<TN, TA, SG, NI, TS...>::wideReportUtilize(Control& control) noexcept {
	const UP l = lHalf.wideReportUtilize(control);
	const UP r = rHalf.wideReportUtilize(control);

	return l.utility >= r.utility ?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
typename TA::Rank
CS_<TN, TA, SG, NI, TS...>::wideReportRank(Control& control,
										   Rank* const ranks) noexcept
{
	HFSM2_ASSERT(ranks);

	const Rank l = lHalf.wideReportRank(control, ranks);
	const Rank r = rHalf.wideReportRank(control, ranks + LStateList::SIZE);

	return l > r?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
typename TA::Utility
CS_<TN, TA, SG, NI, TS...>::wideReportRandomize(Control& control,
												Utility* const options,
												const Rank* const ranks,
												const Rank top) noexcept
{
	HFSM2_ASSERT(options && ranks);

	const Utility l = lHalf.wideReportRandomize(control, options,					 ranks,					   top);
	const Utility r = rHalf.wideReportRandomize(control, options + LStateList::SIZE, ranks + LStateList::SIZE, top);

	return { l + r };
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
typename TA::UP
CS_<TN, TA, SG, NI, TS...>::wideReportChangeComposite(Control& control) noexcept {
	return lHalf.wideReportChangeComposite(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
typename TA::UP
CS_<TN, TA, SG, NI, TS...>::wideReportChangeResumable(Control& control,
													  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		return lHalf.wideReportChangeResumable(control, prong);
	else
		return rHalf.wideReportChangeResumable(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
typename TA::UP
CS_<TN, TA, SG, NI, TS...>::wideReportChangeUtilitarian(Control& control) noexcept {
	const UP l = lHalf.wideReportChangeUtilitarian(control);
	const UP r = rHalf.wideReportChangeUtilitarian(control);

	return l.utility >= r.utility ?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
typename TA::Utility
CS_<TN, TA, SG, NI, TS...>::wideReportChangeRandom(Control& control,
												   Utility* const options,
												   const Rank* const ranks,
												   const Rank top) noexcept
{
	HFSM2_ASSERT(options && ranks);

	const Utility l = lHalf.wideReportChangeRandom(control, options,					ranks,					  top);
	const Utility r = rHalf.wideReportChangeRandom(control, options + LStateList::SIZE, ranks + LStateList::SIZE, top);

	return { l + r };
}

#endif

//------------------------------------------------------------------------------
// COMMON

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideChangeToRequested(PlanControl& control,
												  const Short prong) noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG)
		lHalf.wideChangeToRequested(control, prong);
	else
		rHalf.wideChangeToRequested(control, prong);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideSaveActive(const Registry& registry,
										   WriteStream& stream,
										   const Short prong) const noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG) {
		lHalf.wideSaveActive   (registry, stream, prong);
		rHalf.wideSaveResumable(registry, stream);
	} else {
		lHalf.wideSaveResumable(registry, stream);
		rHalf.wideSaveActive   (registry, stream, prong);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideSaveResumable(const Registry& registry,
											  WriteStream& stream) const noexcept
{
	lHalf.wideSaveResumable(registry, stream);
	rHalf.wideSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideLoadRequested(Registry& registry,
											  ReadStream& stream,
											  const Short prong) const noexcept
{
	HFSM2_ASSERT(prong != INVALID_SHORT);

	if (prong < R_PRONG) {
		lHalf.wideLoadRequested(registry, stream, prong);
		rHalf.wideLoadResumable(registry, stream);
	} else {
		lHalf.wideLoadResumable(registry, stream);
		rHalf.wideLoadRequested(registry, stream, prong);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideLoadResumable(Registry& registry,
											  ReadStream& stream) const noexcept
{
	lHalf.wideLoadResumable(registry, stream);
	rHalf.wideLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, Strategy SG, Short NI, typename... TS>
void
CS_<TN, TA, SG, NI, TS...>::wideGetNames(const Long parent,
										 const RegionType /*region*/,
										 const Short depth,
										 StructureStateInfos& _stateInfos) const noexcept
{
	lHalf.wideGetNames(parent, StructureStateInfo::RegionType::COMPOSITE, depth, _stateInfos);
	rHalf.wideGetNames(parent, StructureStateInfo::RegionType::COMPOSITE, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
