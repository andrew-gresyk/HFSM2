namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
void
OS_<TN_, TA_, NI_, TI_>::wideRegister(StateRegistry& stateRegistry,
									  const ForkID forkId)
{
	initial.deepRegister(stateRegistry, Parent{forkId, PRONG_INDEX});
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
bool
OS_<TN_, TA_, NI_, TI_>::wideForwardEntryGuard(GuardControl& control,
											   const ProngConstBits prongs)
{
	return prongs.get(PRONG_INDEX) ?
		initial.deepForwardEntryGuard(control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
bool
OS_<TN_, TA_, NI_, TI_>::wideForwardEntryGuard(GuardControl& control) {
	return initial.deepForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
bool
OS_<TN_, TA_, NI_, TI_>::wideEntryGuard(GuardControl& control) {
	return initial.deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
void
OS_<TN_, TA_, NI_, TI_>::wideEnter(PlanControl& control) {
	initial.deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
void
OS_<TN_, TA_, NI_, TI_>::wideReenter(PlanControl& control) {
	initial.deepReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
Status
OS_<TN_, TA_, NI_, TI_>::wideUpdate(FullControl& control) {
	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
template <typename TEvent>
Status
OS_<TN_, TA_, NI_, TI_>::wideReact(FullControl& control,
								   const TEvent& event)
{
	return initial.deepReact(control, event);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
bool
OS_<TN_, TA_, NI_, TI_>::wideForwardExitGuard(GuardControl& control,
											  const ProngConstBits prongs)
{
	return prongs.get(PRONG_INDEX) ?
		initial.deepForwardExitGuard(control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
bool
OS_<TN_, TA_, NI_, TI_>::wideForwardExitGuard(GuardControl& control) {
	return initial.deepForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
bool
OS_<TN_, TA_, NI_, TI_>::wideExitGuard(GuardControl& control) {
	return initial.deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
void
OS_<TN_, TA_, NI_, TI_>::wideExit(PlanControl& control) {
	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
void
OS_<TN_, TA_, NI_, TI_>::wideForwardActive(Control& control,
										   const RequestType request,
										   const ProngConstBits prongs)
{
	const RequestType local = prongs.get(PRONG_INDEX) ?
		request : Request::REMAIN;

	initial.deepForwardActive(control, local);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
void
OS_<TN_, TA_, NI_, TI_>::wideForwardRequest(Control& control,
											const RequestType request,
											const ProngConstBits prongs)
{
	const RequestType local = prongs.get(PRONG_INDEX) ?
		request : Request::REMAIN;

	initial.deepForwardRequest(control, local);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
void
OS_<TN_, TA_, NI_, TI_>::wideRequestChange(Control& control) {
	initial.deepRequestChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
void
OS_<TN_, TA_, NI_, TI_>::wideRequestRemain(StateRegistry& stateRegistry) {
	initial.deepRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
void
OS_<TN_, TA_, NI_, TI_>::wideRequestRestart(StateRegistry& stateRegistry) {
	initial.deepRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
void
OS_<TN_, TA_, NI_, TI_>::wideRequestResume(StateRegistry& stateRegistry) {
	initial.deepRequestResume(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
void
OS_<TN_, TA_, NI_, TI_>::wideRequestUtilize(Control& control) {
	initial.deepRequestUtilize(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
void
OS_<TN_, TA_, NI_, TI_>::wideRequestRandomize(Control& control) {
	initial.deepRequestRandomize(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
typename TA_::Utility
OS_<TN_, TA_, NI_, TI_>::wideReportChange(Control& control) {
	const UP i = initial.deepReportChange(control);

	return i.utility;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
typename TA_::Utility
OS_<TN_, TA_, NI_, TI_>::wideReportUtilize(Control& control) {
	const UP i = initial.deepReportUtilize(control);

	return i.utility;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
typename TA_::Utility
OS_<TN_, TA_, NI_, TI_>::wideReportRandomize(Control& control) {
	const Utility i = initial.deepReportRandomize(control);

	return i;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
void
OS_<TN_, TA_, NI_, TI_>::wideEnterRequested(PlanControl& control) {
	initial.deepEnterRequested(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
void
OS_<TN_, TA_, NI_, TI_>::wideChangeToRequested(PlanControl& control) {
	initial.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TN_, typename TA_, ShortIndex NI_, typename TI_>
void
OS_<TN_, TA_, NI_, TI_>::wideGetNames(const LongIndex parent,
									  const ShortIndex depth,
									  StructureStateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StructureStateInfo::ORTHOGONAL, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
