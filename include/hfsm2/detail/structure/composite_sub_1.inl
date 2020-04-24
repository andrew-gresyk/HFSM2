namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM_EXPLICIT_MEMBER_SPECIALIZATION

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
template <typename T>
T&
CS_<TN_, TA_, TG_, NI_, TS_...>::access() {
	return LHalfInfo::StateList::template contains<T>() ?
		lHalf.template access<T>() :
		rHalf.template access<T>();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
template <typename T>
const T&
CS_<TN_, TA_, TG_, NI_, TS_...>::access() const {
	return LHalfInfo::StateList::template contains<T>() ?
		lHalf.template access<T>() :
		rHalf.template access<T>();
}

#endif

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
void
CS_<TN_, TA_, TG_, NI_, TS_...>::wideRegister(StateRegistry& stateRegistry,
											  const Parent parent)
{
	lHalf.wideRegister(stateRegistry, Parent{parent.forkId, L_PRONG});
	rHalf.wideRegister(stateRegistry, Parent{parent.forkId, R_PRONG});
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
bool
CS_<TN_, TA_, TG_, NI_, TS_...>::wideForwardEntryGuard(GuardControl& control,
													   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.wideForwardEntryGuard(control, prong);
	else
		return rHalf.wideForwardEntryGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
bool
CS_<TN_, TA_, TG_, NI_, TS_...>::wideEntryGuard(GuardControl& control,
												const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.wideEntryGuard(control, prong);
	else
		return rHalf.wideEntryGuard(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
void
CS_<TN_, TA_, TG_, NI_, TS_...>::wideConstruct(PlanControl& control,
											   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideConstruct(control, prong);
	else
		rHalf.wideConstruct(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
void
CS_<TN_, TA_, TG_, NI_, TS_...>::wideEnter(PlanControl& control,
										   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideEnter(control, prong);
	else
		rHalf.wideEnter(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
void
CS_<TN_, TA_, TG_, NI_, TS_...>::wideReenter(PlanControl& control,
											 const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideReenter(control, prong);
	else
		rHalf.wideReenter(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
Status
CS_<TN_, TA_, TG_, NI_, TS_...>::wideUpdate(FullControl& control,
											const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	return prong < R_PRONG ?
		lHalf.wideUpdate(control, prong) :
		rHalf.wideUpdate(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
template <typename TEvent>
Status
CS_<TN_, TA_, TG_, NI_, TS_...>::wideReact(FullControl& control,
										   const TEvent& event,
										   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	return prong < R_PRONG ?
		lHalf.wideReact(control, event, prong) :
		rHalf.wideReact(control, event, prong);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
bool
CS_<TN_, TA_, TG_, NI_, TS_...>::wideForwardExitGuard(GuardControl& control,
													  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.wideForwardExitGuard(control, prong);
	else
		return rHalf.wideForwardExitGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
bool
CS_<TN_, TA_, TG_, NI_, TS_...>::wideExitGuard(GuardControl& control,
											   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.wideExitGuard(control, prong);
	else
		return rHalf.wideExitGuard(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
void
CS_<TN_, TA_, TG_, NI_, TS_...>::wideExit(PlanControl& control,
										  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideExit(control, prong);
	else
		rHalf.wideExit(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
void
CS_<TN_, TA_, TG_, NI_, TS_...>::wideDestruct(PlanControl& control,
											  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideDestruct(control, prong);
	else
		rHalf.wideDestruct(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
void
CS_<TN_, TA_, TG_, NI_, TS_...>::wideForwardActive(Control& control,
												   const Request::Type request,
												   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideForwardActive(control, request, prong);
	else
		rHalf.wideForwardActive(control, request, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
void
CS_<TN_, TA_, TG_, NI_, TS_...>::wideForwardRequest(Control& control,
													const Request::Type request,
													const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideForwardRequest(control, request, prong);
	else
		rHalf.wideForwardRequest(control, request, prong);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
void
CS_<TN_, TA_, TG_, NI_, TS_...>::wideRequestChangeComposite(Control& control) {
	lHalf.wideRequestChangeComposite(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
void
CS_<TN_, TA_, TG_, NI_, TS_...>::wideRequestChangeResumable(Control& control,
															const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideRequestChangeResumable(control, prong);
	else
		rHalf.wideRequestChangeResumable(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
void
CS_<TN_, TA_, TG_, NI_, TS_...>::wideRequestRemain(StateRegistry& stateRegistry) {
	lHalf.wideRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
void
CS_<TN_, TA_, TG_, NI_, TS_...>::wideRequestRestart(StateRegistry& stateRegistry) {
	lHalf.wideRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
void
CS_<TN_, TA_, TG_, NI_, TS_...>::wideRequestResume(StateRegistry& stateRegistry,
												   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideRequestResume(stateRegistry, prong);
	else
		rHalf.wideRequestResume(stateRegistry, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
typename TA_::UP
CS_<TN_, TA_, TG_, NI_, TS_...>::wideReportUtilize(Control& control) {
	const UP l = lHalf.wideReportUtilize(control);
	const UP r = rHalf.wideReportUtilize(control);

	return l.utility >= r.utility ?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
typename TA_::Rank
CS_<TN_, TA_, TG_, NI_, TS_...>::wideReportRank(Control& control,
												Rank* const ranks)
{
	HFSM_ASSERT(ranks);

	const Rank l = lHalf.wideReportRank(control, ranks);
	const Rank r = rHalf.wideReportRank(control, ranks + LStates::SIZE);

	return l > r?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
typename TA_::Utility
CS_<TN_, TA_, TG_, NI_, TS_...>::wideReportRandomize(Control& control,
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

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
typename TA_::UP
CS_<TN_, TA_, TG_, NI_, TS_...>::wideReportChangeComposite(Control& control) {
	return lHalf.wideReportChangeComposite(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
typename TA_::UP
CS_<TN_, TA_, TG_, NI_, TS_...>::wideReportChangeResumable(Control& control,
														   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.wideReportChangeResumable(control, prong);
	else
		return rHalf.wideReportChangeResumable(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
typename TA_::UP
CS_<TN_, TA_, TG_, NI_, TS_...>::wideReportChangeUtilitarian(Control& control) {
	const UP l = lHalf.wideReportChangeUtilitarian(control);
	const UP r = rHalf.wideReportChangeUtilitarian(control);

	return l.utility >= r.utility ?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
typename TA_::Utility
CS_<TN_, TA_, TG_, NI_, TS_...>::wideReportChangeRandom(Control& control,
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

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
void
CS_<TN_, TA_, TG_, NI_, TS_...>::wideChangeToRequested(PlanControl& control,
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

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename... TS_>
void
CS_<TN_, TA_, TG_, NI_, TS_...>::wideGetNames(const LongIndex parent,
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
