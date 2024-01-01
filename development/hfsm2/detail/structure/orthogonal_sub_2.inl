namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_>::wideRegister(Registry& registry,
									  const ForkID forkId) noexcept
{
	Initial::deepRegister(registry, Parent{forkId, PRONG_INDEX});
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
bool
OS_<TN_, TA_, NP_, TI_>::wideForwardEntryGuard(GuardControl& control,
											   const ProngCBits prongs) noexcept
{
	return prongs.get(PRONG_INDEX) ?
		Initial::deepForwardEntryGuard(control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
bool
OS_<TN_, TA_, NP_, TI_>::wideForwardEntryGuard(GuardControl& control) noexcept {
	return Initial::deepForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
bool
OS_<TN_, TA_, NP_, TI_>::wideEntryGuard(GuardControl& control) noexcept {
	return Initial::deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_>::wideEnter(PlanControl& control) noexcept {
	Initial::deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_>::wideReenter(PlanControl& control) noexcept {
	Initial::deepReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN_, TA_, NP_, TI_>::widePreUpdate(FullControl& control) noexcept {
	return Initial::deepPreUpdate(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN_, TA_, NP_, TI_>::wideUpdate(FullControl& control) noexcept {
	return Initial::deepUpdate(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN_, TA_, NP_, TI_>::widePostUpdate(FullControl& control) noexcept {
	return Initial::deepPostUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN_, TA_, NP_, TI_>::widePreReact(EventControl& control,
									  const TEvent& event) noexcept
{
	return Initial::deepPreReact(control, event);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN_, TA_, NP_, TI_>::wideReact(EventControl& control,
								   const TEvent& event) noexcept
{
	return Initial::deepReact(control, event);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN_, TA_, NP_, TI_>::widePostReact(EventControl& control,
									   const TEvent& event) noexcept
{
	return Initial::deepPostReact(control, event);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_>::wideQuery(ConstControl& control,
								   TEvent& event) const noexcept
{
	Initial::deepQuery(control, event);
}

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN_, TA_, NP_, TI_>::wideUpdatePlans(FullControl& control) noexcept {
	return Initial::deepUpdatePlans(control);
}

#endif

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
bool
OS_<TN_, TA_, NP_, TI_>::wideForwardExitGuard(GuardControl& control,
											  const ProngCBits prongs) noexcept
{
	return prongs.get(PRONG_INDEX) ?
		Initial::deepForwardExitGuard(control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
bool
OS_<TN_, TA_, NP_, TI_>::wideForwardExitGuard(GuardControl& control) noexcept {
	return Initial::deepForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
bool
OS_<TN_, TA_, NP_, TI_>::wideExitGuard(GuardControl& control) noexcept {
	return Initial::deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_>::wideExit(PlanControl& control) noexcept {
	Initial::deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_>::wideForwardActive(Control& control,
										   const Request request,
										   const ProngCBits prongs) noexcept
{
	if (prongs.get(PRONG_INDEX))
		Initial::deepForwardActive(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_>::wideForwardRequest(Control& control,
											const Request request) noexcept
{
	Initial::deepForwardRequest(control, request);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_>::wideRequestChange(Control& control,
										   const Request request) noexcept
{
	Initial::deepRequestChange(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_>::wideRequestRestart(Control& control,
											const Request request) noexcept
{
	Initial::deepRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_>::wideRequestResume(Control& control,
										   const Request request) noexcept
{
	Initial::deepRequestResume(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_>::wideRequestSelect(Control& control,
										   const Request request) noexcept
{
	Initial::deepRequestSelect(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_>::wideRequestUtilize(Control& control,
											const Request request) noexcept
{
	Initial::deepRequestUtilize(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_>::wideRequestRandomize(Control& control,
											  const Request request) noexcept
{
	Initial::deepRequestRandomize(control, request);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
typename TA_::Utility
OS_<TN_, TA_, NP_, TI_>::wideReportChange(Control& control) noexcept {
	const UP i = Initial::deepReportChange(control);

	return i.utility;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
typename TA_::Utility
OS_<TN_, TA_, NP_, TI_>::wideReportUtilize(Control& control) noexcept {
	const UP i = Initial::deepReportUtilize(control);

	return i.utility;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
typename TA_::Utility
OS_<TN_, TA_, NP_, TI_>::wideReportRandomize(Control& control) noexcept {
	return Initial::deepReportRandomize(control);
}

#endif

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_>::wideChangeToRequested(PlanControl& control) noexcept {
	Initial::deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_>::wideSaveActive(const Registry& registry,
										WriteStream& stream) const noexcept
{
	Initial::deepSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_>::wideSaveResumable(const Registry& registry,
										   WriteStream& stream) const noexcept
{
	Initial::deepSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_>::wideLoadRequested(Registry& registry,
										   ReadStream& stream) const noexcept
{
	Initial::deepLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_>::wideLoadResumable(Registry& registry,
										   ReadStream& stream) const noexcept
{
	Initial::deepLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN_, typename TA_, Prong NP_, typename TI_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_>::wideGetNames(const Long parent,
									  const Short depth,
									  StructureStateInfos& stateInfos) const noexcept
{
	Initial::deepGetNames(parent, RegionType::ORTHOGONAL, depth, stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
