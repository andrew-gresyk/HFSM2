namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideRegister(StateRegistry& stateRegistry,
												 const ForkID forkId)
{
	initial  .deepRegister(stateRegistry, Parent{forkId, PRONG_INDEX});
	remaining.wideRegister(stateRegistry, forkId);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardEntryGuard(GuardControl& control,
														  const OrthoFork& prongs)
{
	const bool i = prongs[PRONG_INDEX] ?
				   initial  .deepForwardEntryGuard(control) : false;

	const bool r = remaining.wideForwardEntryGuard(control, prongs);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardEntryGuard(GuardControl& control) {
	const bool i = initial  .deepForwardEntryGuard(control);
	const bool r = remaining.wideForwardEntryGuard(control);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideEntryGuard(GuardControl& control) {
	const bool i = initial  .deepEntryGuard(control);
	const bool r = remaining.wideEntryGuard(control);

	return i || r;
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideEnter(PlanControl& control) {
	initial  .deepEnter(control);
	remaining.wideEnter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
Status
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideUpdate(FullControl& control) {
	const auto status =	   initial	.deepUpdate(control);
	return combine(status, remaining.wideUpdate(control));
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
template <typename TEvent>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideReact(const TEvent& event,
											  FullControl& control)
{
	initial  .deepReact(control, event);
	remaining.wideReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardExitGuard(GuardControl& control,
														 const OrthoFork& prongs)
{
	const bool i = prongs[PRONG_INDEX] ?
				   initial  .deepForwardExitGuard(control) : false;

	const bool r = remaining.wideForwardExitGuard(control, prongs);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardExitGuard(GuardControl& control) {
	const bool i = initial  .deepForwardExitGuard(control);
	const bool r = remaining.wideForwardExitGuard(control);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideExitGuard(GuardControl& control) {
	const bool i = initial  .deepExitGuard(control);
	const bool r = remaining.wideExitGuard(control);

	return i || r;
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideExit(PlanControl& control) {
	initial	 .deepExit(control);
	remaining.wideExit(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardActive(Control& control,
													  const RequestType request,
													  const OrthoFork& prongs)
{
	const RequestType local = prongs[PRONG_INDEX] ?
		request : Request::REMAIN;

	initial	 .deepForwardActive(control, local);
	remaining.wideForwardActive(control, request, prongs);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardRequest(Control& control,
													   const RequestType request,
													   const OrthoFork& prongs)
{
	const RequestType local = prongs[PRONG_INDEX] ?
		request : Request::REMAIN;

	initial	 .deepForwardRequest(control, local);
	remaining.wideForwardRequest(control, request, prongs);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
UProng
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestChange(Control& control) {
	initial  .deepRequestChange(control);
	remaining.wideRequestChange(control);

	return {};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
UProng
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideReportChange(Control& control) {
	const UProng i = initial  .deepReportChange(control);
	const UProng r = remaining.wideReportChange(control);

	return { i.utilityCompliment * r.utilityCompliment };
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestRemain(StateRegistry& stateRegistry) {
	initial	 .deepRequestRemain(stateRegistry);
	remaining.wideRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestRestart(StateRegistry& stateRegistry) {
	initial	 .deepRequestRestart(stateRegistry);
	remaining.wideRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestResume(StateRegistry& stateRegistry) {
	initial	 .deepRequestResume(stateRegistry);
	remaining.wideRequestResume(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestUtilize(Control& control) {
	initial  .deepRequestUtilize(control);
	remaining.wideRequestUtilize(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
UProng
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideReportUtilize(Control& control) {
	const UProng i = initial  .deepReportUtilize(control);
	const UProng r = remaining.wideReportUtilize(control);

	return { i.utilityCompliment * r.utilityCompliment };
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideEnterRequested(PlanControl& control) {
	initial	 .deepEnterRequested(control);
	remaining.wideEnterRequested(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideChangeToRequested(PlanControl& control) {
	initial	 .deepChangeToRequested(control);
	remaining.wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideGetNames(const LongIndex parent,
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
