namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<TN, TA, NI, TI, TR...>::wideRegister(StateRegistry& stateRegistry,
										 const ForkID forkId)
{
	initial  .deepRegister(stateRegistry, Parent{forkId, PRONG_INDEX});
	remaining.wideRegister(stateRegistry, forkId);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_OS<TN, TA, NI, TI, TR...>::wideForwardEntryGuard(GuardControl& control,
												  const ProngConstBits prongs)
{
	const bool i = prongs.get(PRONG_INDEX) ?
				   initial  .deepForwardEntryGuard(control) : false;

	const bool r = remaining.wideForwardEntryGuard(control, prongs);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_OS<TN, TA, NI, TI, TR...>::wideForwardEntryGuard(GuardControl& control) {
	const bool i = initial  .deepForwardEntryGuard(control);
	const bool r = remaining.wideForwardEntryGuard(control);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_OS<TN, TA, NI, TI, TR...>::wideEntryGuard(GuardControl& control) {
	const bool i = initial  .deepEntryGuard(control);
	const bool r = remaining.wideEntryGuard(control);

	return i || r;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<TN, TA, NI, TI, TR...>::wideEnter(PlanControl& control) {
	initial  .deepEnter(control);
	remaining.wideEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<TN, TA, NI, TI, TR...>::wideReenter(PlanControl& control) {
	initial  .deepReenter(control);
	remaining.wideReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
Status
_OS<TN, TA, NI, TI, TR...>::wideUpdate(FullControl& control) {
	const auto status =	   initial	.deepUpdate(control);
	return combine(status, remaining.wideUpdate(control));
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
template <typename TEvent>
Status
_OS<TN, TA, NI, TI, TR...>::wideReact(FullControl& control,
									  const TEvent& event)
{
	const auto status =	   initial	.deepReact(control, event);
	return combine(status, remaining.wideReact(control, event));
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_OS<TN, TA, NI, TI, TR...>::wideForwardExitGuard(GuardControl& control,
												 const ProngConstBits prongs)
{
	const bool i = prongs.get(PRONG_INDEX) ?
				   initial  .deepForwardExitGuard(control) : false;

	const bool r = remaining.wideForwardExitGuard(control, prongs);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_OS<TN, TA, NI, TI, TR...>::wideForwardExitGuard(GuardControl& control) {
	const bool i = initial  .deepForwardExitGuard(control);
	const bool r = remaining.wideForwardExitGuard(control);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_OS<TN, TA, NI, TI, TR...>::wideExitGuard(GuardControl& control) {
	const bool i = initial  .deepExitGuard(control);
	const bool r = remaining.wideExitGuard(control);

	return i || r;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<TN, TA, NI, TI, TR...>::wideExit(PlanControl& control) {
	initial	 .deepExit(control);
	remaining.wideExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<TN, TA, NI, TI, TR...>::wideForwardActive(Control& control,
											  const RequestType request,
											  const ProngConstBits prongs)
{
	const RequestType local = prongs.get(PRONG_INDEX) ?
		request : Request::REMAIN;

	initial	 .deepForwardActive(control, local);
	remaining.wideForwardActive(control, request, prongs);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<TN, TA, NI, TI, TR...>::wideForwardRequest(Control& control,
											   const RequestType request,
											   const ProngConstBits prongs)
{
	const RequestType local = prongs.get(PRONG_INDEX) ?
		request : Request::REMAIN;

	initial	 .deepForwardRequest(control, local);
	remaining.wideForwardRequest(control, request, prongs);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<TN, TA, NI, TI, TR...>::wideRequestChange(Control& control) {
	initial  .deepRequestChange(control);
	remaining.wideRequestChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<TN, TA, NI, TI, TR...>::wideRequestRemain(StateRegistry& stateRegistry) {
	initial	 .deepRequestRemain(stateRegistry);
	remaining.wideRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<TN, TA, NI, TI, TR...>::wideRequestRestart(StateRegistry& stateRegistry) {
	initial	 .deepRequestRestart(stateRegistry);
	remaining.wideRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<TN, TA, NI, TI, TR...>::wideRequestResume(StateRegistry& stateRegistry) {
	initial	 .deepRequestResume(stateRegistry);
	remaining.wideRequestResume(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<TN, TA, NI, TI, TR...>::wideRequestUtilize(Control& control) {
	initial  .deepRequestUtilize(control);
	remaining.wideRequestUtilize(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
typename TA::Utility
_OS<TN, TA, NI, TI, TR...>::wideReportChange(Control& control) {
	const UP	  i = initial  .deepReportChange(control);
	const Utility r = remaining.wideReportChange(control);

	return i.utility + r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
typename TA::Utility
_OS<TN, TA, NI, TI, TR...>::wideReportUtilize(Control& control) {
	const UP	  i = initial  .deepReportUtilize(control);
	const Utility r = remaining.wideReportUtilize(control);

	return i.utility + r;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<TN, TA, NI, TI, TR...>::wideEnterRequested(PlanControl& control) {
	initial	 .deepEnterRequested(control);
	remaining.wideEnterRequested(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<TN, TA, NI, TI, TR...>::wideChangeToRequested(PlanControl& control) {
	initial	 .deepChangeToRequested(control);
	remaining.wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<TN, TA, NI, TI, TR...>::wideGetNames(const LongIndex parent,
										 const ShortIndex depth,
										 StructureStateInfos& _stateInfos) const
{
	initial	 .deepGetNames(parent, StructureStateInfo::ORTHOGONAL, depth, _stateInfos);
	remaining.wideGetNames(parent,								   depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
