﻿namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRegister(Registry& registry,
									  const ForkID forkId)
{
	initial.deepRegister(registry, Parent{forkId, PRONG_INDEX});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
bool
OS_<TN, TA, NI, TI>::wideForwardEntryGuard(GuardControl& control,
										   const ProngConstBits prongs)
{
	return prongs.get(PRONG_INDEX) ?
		initial.deepForwardEntryGuard(control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
bool
OS_<TN, TA, NI, TI>::wideForwardEntryGuard(GuardControl& control) {
	return initial.deepForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
bool
OS_<TN, TA, NI, TI>::wideEntryGuard(GuardControl& control) {
	return initial.deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideConstruct(PlanControl& control) {
	initial.deepConstruct(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideEnter(PlanControl& control) {
	initial.deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideReenter(PlanControl& control) {
	initial.deepReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
Status
OS_<TN, TA, NI, TI>::wideUpdate(FullControl& control) {
	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
template <typename TEvent>
Status
OS_<TN, TA, NI, TI>::wideReact(FullControl& control,
							   const TEvent& event)
{
	return initial.deepReact(control, event);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
bool
OS_<TN, TA, NI, TI>::wideForwardExitGuard(GuardControl& control,
										  const ProngConstBits prongs)
{
	return prongs.get(PRONG_INDEX) ?
		initial.deepForwardExitGuard(control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
bool
OS_<TN, TA, NI, TI>::wideForwardExitGuard(GuardControl& control) {
	return initial.deepForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
bool
OS_<TN, TA, NI, TI>::wideExitGuard(GuardControl& control) {
	return initial.deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideExit(PlanControl& control) {
	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideDestruct(PlanControl& control) {
	initial.deepDestruct(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideForwardActive(Control& control,
									   const TransitionType request,
									   const ProngConstBits prongs)
{
	const TransitionType local = prongs.get(PRONG_INDEX) ?
		request : TransitionType::REMAIN;

	initial.deepForwardActive(control, local);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideForwardRequest(Control& control,
										const TransitionType request,
										const ProngConstBits prongs)
{
	const TransitionType local = prongs.get(PRONG_INDEX) ?
		request : TransitionType::REMAIN;

	initial.deepForwardRequest(control, local);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRequestChange(Control& control) {
	initial.deepRequestChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRequestRemain(Registry& registry) {
	initial.deepRequestRemain(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRequestRestart(Registry& registry) {
	initial.deepRequestRestart(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRequestResume(Registry& registry) {
	initial.deepRequestResume(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRequestUtilize(Control& control) {
	initial.deepRequestUtilize(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRequestRandomize(Control& control) {
	initial.deepRequestRandomize(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
typename TA::Utility
OS_<TN, TA, NI, TI>::wideReportChange(Control& control) {
	const UP i = initial.deepReportChange(control);

	return i.utility;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
typename TA::Utility
OS_<TN, TA, NI, TI>::wideReportUtilize(Control& control) {
	const UP i = initial.deepReportUtilize(control);

	return i.utility;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
typename TA::Utility
OS_<TN, TA, NI, TI>::wideReportRandomize(Control& control) {
	const Utility i = initial.deepReportRandomize(control);

	return i;
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideChangeToRequested(PlanControl& control) {
	initial.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideSaveActive(const Registry& registry,
									WriteStream& stream) const
{
	initial.deepSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideSaveResumable(const Registry& registry,
									   WriteStream& stream) const
{
	initial.deepSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideLoadRequested(Registry& registry,
									   ReadStream& stream) const
{
	initial.deepLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideLoadResumable(Registry& registry,
									   ReadStream& stream) const
{
	initial.deepLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideGetNames(const LongIndex parent,
								  const ShortIndex depth,
								  StructureStateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StructureStateInfo::ORTHOGONAL, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
