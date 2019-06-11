namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
_OS<TN, TA, NI, TI>::wideRegister(StateRegistry& stateRegistry,
								  const ForkID forkId)
{
	initial.deepRegister(stateRegistry, Parent{forkId, PRONG_INDEX});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
bool
_OS<TN, TA, NI, TI>::wideForwardEntryGuard(GuardControl& control,
										   const ProngConstBits prongs)
{
	return prongs.get(PRONG_INDEX) ?
		initial.deepForwardEntryGuard(control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
bool
_OS<TN, TA, NI, TI>::wideForwardEntryGuard(GuardControl& control) {
	return initial.deepForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
bool
_OS<TN, TA, NI, TI>::wideEntryGuard(GuardControl& control) {
	return initial.deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
_OS<TN, TA, NI, TI>::wideEnter(PlanControl& control) {
	initial.deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
_OS<TN, TA, NI, TI>::wideReenter(PlanControl& control) {
	initial.deepReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
Status
_OS<TN, TA, NI, TI>::wideUpdate(FullControl& control) {
	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
template <typename TEvent>
Status
_OS<TN, TA, NI, TI>::wideReact(FullControl& control,
									   const TEvent& event)
{
	return initial.deepReact(control, event);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
bool
_OS<TN, TA, NI, TI>::wideForwardExitGuard(GuardControl& control,
										  const ProngConstBits prongs)
{
	return prongs.get(PRONG_INDEX) ?
		initial.deepForwardExitGuard(control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
bool
_OS<TN, TA, NI, TI>::wideForwardExitGuard(GuardControl& control) {
	return initial.deepForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
bool
_OS<TN, TA, NI, TI>::wideExitGuard(GuardControl& control) {
	return initial.deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
_OS<TN, TA, NI, TI>::wideExit(PlanControl& control) {
	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
_OS<TN, TA, NI, TI>::wideForwardActive(Control& control,
									   const RequestType request,
									   const ProngConstBits prongs)
{
	const RequestType local = prongs.get(PRONG_INDEX) ?
		request : Request::REMAIN;

	initial.deepForwardActive(control, local);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
_OS<TN, TA, NI, TI>::wideForwardRequest(Control& control,
										const RequestType request,
										const ProngConstBits prongs)
{
	const RequestType local = prongs.get(PRONG_INDEX) ?
		request : Request::REMAIN;

	initial.deepForwardRequest(control, local);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
_OS<TN, TA, NI, TI>::wideRequestChange(Control& control) {
	initial.deepRequestChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
_OS<TN, TA, NI, TI>::wideRequestRemain(StateRegistry& stateRegistry) {
	initial.deepRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
_OS<TN, TA, NI, TI>::wideRequestRestart(StateRegistry& stateRegistry) {
	initial.deepRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
_OS<TN, TA, NI, TI>::wideRequestResume(StateRegistry& stateRegistry) {
	initial.deepRequestResume(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
_OS<TN, TA, NI, TI>::wideRequestUtilize(Control& control) {
	initial.deepRequestUtilize(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
typename TA::Utility
_OS<TN, TA, NI, TI>::wideReportChange(Control& control) {
	const UP i = initial.deepReportChange(control);

	return i.utility;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
typename TA::Utility
_OS<TN, TA, NI, TI>::wideReportUtilize(Control& control) {
	const UP i = initial.deepReportUtilize(control);

	return i.utility;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
_OS<TN, TA, NI, TI>::wideEnterRequested(PlanControl& control) {
	initial.deepEnterRequested(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
_OS<TN, TA, NI, TI>::wideChangeToRequested(PlanControl& control) {
	initial.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
_OS<TN, TA, NI, TI>::wideGetNames(const LongIndex parent,
								  const ShortIndex depth,
								  StructureStateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StructureStateInfo::ORTHOGONAL, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
