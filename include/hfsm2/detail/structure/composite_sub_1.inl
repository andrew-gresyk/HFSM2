namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideRegister(StateRegistry& stateRegistry,
										 const Parent parent)
{
	lHalf.wideRegister(stateRegistry, Parent{parent.forkId, L_PRONG});
	rHalf.wideRegister(stateRegistry, Parent{parent.forkId, R_PRONG});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
bool
CS_<TN, TA, TG, NI, TS...>::wideForwardEntryGuard(GuardControl& control,
												  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.wideForwardEntryGuard(control, prong);
	else
		return rHalf.wideForwardEntryGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
bool
CS_<TN, TA, TG, NI, TS...>::wideEntryGuard(GuardControl& control,
										   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.wideEntryGuard(control, prong);
	else
		return rHalf.wideEntryGuard(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideEnter(PlanControl& control,
									  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideEnter(control, prong);
	else
		rHalf.wideEnter(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideReenter(PlanControl& control,
										const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideReenter(control, prong);
	else
		rHalf.wideReenter(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
Status
CS_<TN, TA, TG, NI, TS...>::wideUpdate(FullControl& control,
									   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	return prong < R_PRONG ?
		lHalf.wideUpdate(control, prong) :
		rHalf.wideUpdate(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
template <typename TEvent>
Status
CS_<TN, TA, TG, NI, TS...>::wideReact(FullControl& control,
									  const TEvent& event,
									  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	return prong < R_PRONG ?
		lHalf.wideReact(control, event, prong) :
		rHalf.wideReact(control, event, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
bool
CS_<TN, TA, TG, NI, TS...>::wideForwardExitGuard(GuardControl& control,
												 const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.wideForwardExitGuard(control, prong);
	else
		return rHalf.wideForwardExitGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
bool
CS_<TN, TA, TG, NI, TS...>::wideExitGuard(GuardControl& control,
										  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.wideExitGuard(control, prong);
	else
		return rHalf.wideExitGuard(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideExit(PlanControl& control,
									 const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideExit(control, prong);
	else
		rHalf.wideExit(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideForwardActive(Control& control,
											  const RequestType request,
											  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideForwardActive(control, request, prong);
	else
		rHalf.wideForwardActive(control, request, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideForwardRequest(Control& control,
											   const RequestType request,
											   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideForwardRequest(control, request, prong);
	else
		rHalf.wideForwardRequest(control, request, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideRequestChangeComposite(Control& control) {
	lHalf.wideRequestChangeComposite(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideRequestChangeResumable(Control& control,
													   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideRequestChangeResumable(control, prong);
	else
		rHalf.wideRequestChangeResumable(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideRequestRemain(StateRegistry& stateRegistry) {
	lHalf.wideRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideRequestRestart(StateRegistry& stateRegistry) {
	lHalf.wideRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideRequestResume(StateRegistry& stateRegistry,
											  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideRequestResume(stateRegistry, prong);
	else
		rHalf.wideRequestResume(stateRegistry, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
typename TA::UP
CS_<TN, TA, TG, NI, TS...>::wideReportUtilize(Control& control) {
	const UP l = lHalf.wideReportUtilize(control);
	const UP r = rHalf.wideReportUtilize(control);

	return l.utility >= r.utility ?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
typename TA::Rank
CS_<TN, TA, TG, NI, TS...>::wideReportRank(Control& control,
										   Rank* const ranks)
{
	HFSM_ASSERT(ranks);

	const Rank l = lHalf.wideReportRank(control, ranks);
	const Rank r = rHalf.wideReportRank(control, ranks + LStates::SIZE);

	return l > r?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
typename TA::Utility
CS_<TN, TA, TG, NI, TS...>::wideReportRandomize(Control& control,
												Utility* const options,
												const Rank* const ranks,
												const Rank top)
{
	HFSM_ASSERT(options && ranks);

	const Utility l = lHalf.wideReportRandomize(control, options,				  ranks,				 top);
	const Utility r = rHalf.wideReportRandomize(control, options + LStates::SIZE, ranks + LStates::SIZE, top);

	return { l + r };
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
typename TA::UP
CS_<TN, TA, TG, NI, TS...>::wideReportChangeComposite(Control& control) {
	return lHalf.wideReportChangeComposite(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
typename TA::UP
CS_<TN, TA, TG, NI, TS...>::wideReportChangeResumable(Control& control,
													  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.wideReportChangeResumable(control, prong);
	else
		return rHalf.wideReportChangeResumable(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
typename TA::UP
CS_<TN, TA, TG, NI, TS...>::wideReportChangeUtilitarian(Control& control) {
	const UP l = lHalf.wideReportChangeUtilitarian(control);
	const UP r = rHalf.wideReportChangeUtilitarian(control);

	return l.utility >= r.utility ?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
typename TA::Utility
CS_<TN, TA, TG, NI, TS...>::wideReportChangeRandom(Control& control,
												   Utility* const options,
												   const Rank* const ranks,
												   const Rank top)
{
	HFSM_ASSERT(options && ranks);

	const Utility l = lHalf.wideReportChangeRandom(control, options,				 ranks,					top);
	const Utility r = rHalf.wideReportChangeRandom(control, options + LStates::SIZE, ranks + LStates::SIZE, top);

	return { l + r };
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideEnterRequested(PlanControl& control,
											   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideEnterRequested(control, prong);
	else
		rHalf.wideEnterRequested(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideChangeToRequested(PlanControl& control,
												  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideChangeToRequested(control, prong);
	else
		rHalf.wideChangeToRequested(control, prong);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideGetNames(const LongIndex parent,
										 const RegionType /*region*/,
										 const ShortIndex depth,
										 StructureStateInfos& _stateInfos) const
{
	lHalf.wideGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
	rHalf.wideGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
