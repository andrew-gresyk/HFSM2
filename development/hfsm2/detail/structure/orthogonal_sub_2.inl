namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRegister(Registry& registry,
								  const ForkID forkId) noexcept
{
	initial.deepRegister(registry, Parent{forkId, PRONG_INDEX});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI>::wideForwardEntryGuard(GuardControl& control,
										   const ProngCBits prongs) noexcept
{
	return prongs.get(PRONG_INDEX) ?
		initial.deepForwardEntryGuard(control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI>::wideForwardEntryGuard(GuardControl& control) noexcept {
	return initial.deepForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI>::wideEntryGuard(GuardControl& control) noexcept {
	return initial.deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideEnter(PlanControl& control) noexcept {
	initial.deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideReenter(PlanControl& control) noexcept {
	initial.deepReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI>::wideUpdate(FullControl& control) noexcept {
	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI>::wideReact(FullControl& control,
							   const TEvent& event) noexcept
{
	return initial.deepReact(control, event);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI>::wideForwardExitGuard(GuardControl& control,
										  const ProngCBits prongs) noexcept
{
	return prongs.get(PRONG_INDEX) ?
		initial.deepForwardExitGuard(control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI>::wideForwardExitGuard(GuardControl& control) noexcept {
	return initial.deepForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI>::wideExitGuard(GuardControl& control) noexcept {
	return initial.deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideExit(PlanControl& control) noexcept {
	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideForwardActive(Control& control,
									   const Request request,
									   const ProngCBits prongs) noexcept
{
	if (prongs.get(PRONG_INDEX))
		initial.deepForwardActive(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideForwardRequest(Control& control,
										const Request request) noexcept
{
	initial.deepForwardRequest(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRequestChange(Control& control,
									   const Request request) noexcept
{
	initial.deepRequestChange(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRequestRestart(Control& control,
										const Request request) noexcept
{
	initial.deepRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRequestResume(Control& control,
									   const Request request) noexcept
{
	initial.deepRequestResume(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRequestUtilize(Control& control,
										const Request request) noexcept
{
	initial.deepRequestUtilize(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRequestRandomize(Control& control,
										  const Request request) noexcept
{
	initial.deepRequestRandomize(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
typename TA::Utility
OS_<TN, TA, NI, TI>::wideReportChange(Control& control) noexcept {
	const UP i = initial.deepReportChange(control);

	return i.utility;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
typename TA::Utility
OS_<TN, TA, NI, TI>::wideReportUtilize(Control& control) noexcept {
	const UP i = initial.deepReportUtilize(control);

	return i.utility;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
typename TA::Utility
OS_<TN, TA, NI, TI>::wideReportRandomize(Control& control) noexcept {
	return initial.deepReportRandomize(control);
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideChangeToRequested(PlanControl& control) noexcept {
	initial.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideSaveActive(const Registry& registry,
									WriteStream& stream) const noexcept
{
	initial.deepSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideSaveResumable(const Registry& registry,
									   WriteStream& stream) const noexcept
{
	initial.deepSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideLoadRequested(Registry& registry,
									   ReadStream& stream) const noexcept
{
	initial.deepLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideLoadResumable(Registry& registry,
									   ReadStream& stream) const noexcept
{
	initial.deepLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideGetNames(const Long parent,
								  const Short depth,
								  StructureStateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StructureStateInfo::RegionType::ORTHOGONAL, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
