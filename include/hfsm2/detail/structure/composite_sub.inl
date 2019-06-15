namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::deepRegister(StateRegistry& stateRegistry,
												 const Parent parent)
{
	lHalf.deepRegister(stateRegistry, Parent{parent.forkId, L_PRONG});
	rHalf.deepRegister(stateRegistry, Parent{parent.forkId, R_PRONG});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
bool
CS_<TN, TA, TG, NI, TS...>::deepForwardEntryGuard(GuardControl& control,
												  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.deepForwardEntryGuard(control, prong);
	else
		return rHalf.deepForwardEntryGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
bool
CS_<TN, TA, TG, NI, TS...>::deepEntryGuard(GuardControl& control,
										   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.deepEntryGuard(control, prong);
	else
		return rHalf.deepEntryGuard(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::deepEnter(PlanControl& control,
									  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepEnter(control, prong);
	else
		rHalf.deepEnter(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::deepReenter(PlanControl& control,
										const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepReenter(control, prong);
	else
		rHalf.deepReenter(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
Status
CS_<TN, TA, TG, NI, TS...>::deepUpdate(FullControl& control,
									   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	return prong < R_PRONG ?
		lHalf.deepUpdate(control, prong) :
		rHalf.deepUpdate(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
template <typename TEvent>
Status
CS_<TN, TA, TG, NI, TS...>::deepReact(FullControl& control,
									  const TEvent& event,
									  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	return prong < R_PRONG ?
		lHalf.deepReact(control, event, prong) :
		rHalf.deepReact(control, event, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
bool
CS_<TN, TA, TG, NI, TS...>::deepForwardExitGuard(GuardControl& control,
												 const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.deepForwardExitGuard(control, prong);
	else
		return rHalf.deepForwardExitGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
bool
CS_<TN, TA, TG, NI, TS...>::deepExitGuard(GuardControl& control,
										  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.deepExitGuard(control, prong);
	else
		return rHalf.deepExitGuard(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::deepExit(PlanControl& control,
									 const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepExit(control, prong);
	else
		rHalf.deepExit(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::deepForwardActive(Control& control,
											  const RequestType request,
											  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepForwardActive(control, request, prong);
	else
		rHalf.deepForwardActive(control, request, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::deepForwardRequest(Control& control,
											   const RequestType request,
											   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepForwardRequest(control, request, prong);
	else
		rHalf.deepForwardRequest(control, request, prong);
}

//------------------------------------------------------------------------------

#if !HFSM_EXPLICIT_MEMBER_SPECIALIZATION

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::deepRequestChange(Control& control,
											  const ShortIndex prong)
{
	switch (STRATEGY) {
	case Strategy::Composite:
		deepRequestChangeComposite(control);
		break;

	case Strategy::Resumable:
		deepRequestChangeResumable(control, prong);
		break;

	default:
		HFSM_BREAK();
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::deepRequestChangeComposite(Control& control) {
	lHalf.deepRequestChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::deepRequestChangeResumable(Control& control,
													   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepRequestChange(control, prong);
	else
		rHalf.deepRequestChange(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::deepRequestRemain(StateRegistry& stateRegistry) {
	lHalf.deepRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::deepRequestRestart(StateRegistry& stateRegistry) {
	lHalf.deepRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::deepRequestResume(StateRegistry& stateRegistry,
											  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepRequestResume(stateRegistry, prong);
	else
		rHalf.deepRequestResume(stateRegistry, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
typename TA::UP
CS_<TN, TA, TG, NI, TS...>::deepReportUtilize(Control& control) {
	const UP l = lHalf.deepReportUtilize(control);
	const UP r = rHalf.deepReportUtilize(control);

	return l.utility >= r.utility ?
		l : r;
}

//------------------------------------------------------------------------------

#if !HFSM_EXPLICIT_MEMBER_SPECIALIZATION

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
typename TA::UP
CS_<TN, TA, TG, NI, TS...>::deepReportChange(Control& control,
											 const ShortIndex prong)
{
	switch (STRATEGY) {
	case Strategy::Composite:
		return deepReportChangeComposite  (control);

	case Strategy::Resumable:
		return deepReportChangeResumable  (control, prong);

	case Strategy::Utilitarian:
		return deepReportChangeUtilitarian(control);

	default:
		HFSM_BREAK();
		return {};
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
typename TA::UP
CS_<TN, TA, TG, NI, TS...>::deepReportChangeComposite(Control& control) {
	return lHalf.deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
typename TA::UP
CS_<TN, TA, TG, NI, TS...>::deepReportChangeResumable(Control& control,
													  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.deepReportChange(control, prong);
	else
		return rHalf.deepReportChange(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
typename TA::UP
CS_<TN, TA, TG, NI, TS...>::deepReportChangeUtilitarian(Control& control) {
	const UP l = lHalf.deepReportChange(control);
	const UP r = rHalf.deepReportChange(control);

	return l.utility >= r.utility ?
		l : r;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::deepEnterRequested(PlanControl& control,
											   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepEnterRequested(control, prong);
	else
		rHalf.deepEnterRequested(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::deepChangeToRequested(PlanControl& control,
												  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepChangeToRequested(control, prong);
	else
		rHalf.deepChangeToRequested(control, prong);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::deepGetNames(const LongIndex parent,
										 const RegionType /*region*/,
										 const ShortIndex depth,
										 StructureStateInfos& _stateInfos) const
{
	lHalf.deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
	rHalf.deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
