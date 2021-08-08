namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#if HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
template <typename T>
HFSM2_CONSTEXPR(14)
T&
OS_<TN, TA, NI, TI, TR...>::access() noexcept {
	return contains<InitialStates, T>() ?
		initial  .template access<T>() :
		remaining.template access<T>();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
template <typename T>
HFSM2_CONSTEXPR(11)
const T&
OS_<TN, TA, NI, TI, TR...>::access() const noexcept {
	return contains<InitialStates, T>() ?
		initial  .template access<T>() :
		remaining.template access<T>();
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideRegister(Registry& registry,
										 const ForkID forkId) noexcept
{
	initial  .deepRegister(registry, Parent{forkId, PRONG_INDEX});
	remaining.wideRegister(registry, forkId);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardEntryGuard(GuardControl& control,
												  const ProngCBits prongs) noexcept
{
	const bool i = prongs.get(PRONG_INDEX) ?
				   initial  .deepForwardEntryGuard(control) : false;

	const bool r = remaining.wideForwardEntryGuard(control, prongs);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardEntryGuard(GuardControl& control) noexcept {
	const bool i = initial  .deepForwardEntryGuard(control);
	const bool r = remaining.wideForwardEntryGuard(control);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI, TR...>::wideEntryGuard(GuardControl& control) noexcept {
	const bool i = initial  .deepEntryGuard(control);
	const bool r = remaining.wideEntryGuard(control);

	return i || r;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideEnter(PlanControl& control) noexcept {
	initial  .deepEnter(control);
	remaining.wideEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideReenter(PlanControl& control) noexcept {
	initial  .deepReenter(control);
	remaining.wideReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI, TR...>::wideUpdate(FullControl& control) noexcept {
	const auto status =	   initial	.deepUpdate(control);
	return combine(status, remaining.wideUpdate(control));
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI, TR...>::wideReact(FullControl& control,
									  const TEvent& event) noexcept
{
	const auto status =	   initial	.deepReact(control, event);
	return combine(status, remaining.wideReact(control, event));
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardExitGuard(GuardControl& control,
												 const ProngCBits prongs) noexcept
{
	const bool i = prongs.get(PRONG_INDEX) ?
				   initial  .deepForwardExitGuard(control) : false;

	const bool r = remaining.wideForwardExitGuard(control, prongs);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardExitGuard(GuardControl& control) noexcept {
	const bool i = initial  .deepForwardExitGuard(control);
	const bool r = remaining.wideForwardExitGuard(control);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI, TR...>::wideExitGuard(GuardControl& control) noexcept {
	const bool i = initial  .deepExitGuard(control);
	const bool r = remaining.wideExitGuard(control);

	return i || r;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideExit(PlanControl& control) noexcept {
	initial	 .deepExit(control);
	remaining.wideExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideForwardActive(Control& control,
											  const Request request,
											  const ProngCBits prongs) noexcept
{
	if (prongs.get(PRONG_INDEX))
		initial.deepForwardActive(control, request);

	remaining  .wideForwardActive(control, request, prongs);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideForwardRequest(Control& control,
											   const Request request) noexcept
{
	initial	 .deepForwardRequest(control, request);
	remaining.wideForwardRequest(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideRequestChange(Control& control,
											  const Request request) noexcept
{
	initial  .deepRequestChange(control, request);
	remaining.wideRequestChange(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideRequestRestart(Control& control,
											   const Request request) noexcept
{
	initial	 .deepRequestRestart(control, request);
	remaining.wideRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideRequestResume(Control& control,
											  const Request request) noexcept
{
	initial	 .deepRequestResume(control, request);
	remaining.wideRequestResume(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideRequestUtilize(Control& control,
											   const Request request) noexcept
{
	initial  .deepRequestUtilize(control, request);
	remaining.wideRequestUtilize(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideRequestRandomize(Control& control,
												 const Request request) noexcept
{
	initial  .deepRequestRandomize(control, request);
	remaining.wideRequestRandomize(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
typename TA::Utility
OS_<TN, TA, NI, TI, TR...>::wideReportChange(Control& control) noexcept {
	const UP	  i = initial  .deepReportChange(control);
	const Utility r = remaining.wideReportChange(control);

	return i.utility + r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
typename TA::Utility
OS_<TN, TA, NI, TI, TR...>::wideReportUtilize(Control& control) noexcept {
	const UP	  i = initial  .deepReportUtilize(control);
	const Utility r = remaining.wideReportUtilize(control);

	return i.utility + r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
typename TA::Utility
OS_<TN, TA, NI, TI, TR...>::wideReportRandomize(Control& control) noexcept {
	const Utility i = initial  .deepReportRandomize(control);
	const Utility r = remaining.wideReportRandomize(control);

	return i + r;
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideChangeToRequested(PlanControl& control) noexcept {
	initial	 .deepChangeToRequested(control);
	remaining.wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideSaveActive(const Registry& registry,
										   WriteStream& stream) const noexcept
{
	initial	 .deepSaveActive(registry, stream);
	remaining.wideSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideSaveResumable(const Registry& registry,
											  WriteStream& stream) const noexcept
{
	initial	 .deepSaveResumable(registry, stream);
	remaining.wideSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideLoadRequested(Registry& registry,
											  ReadStream& stream) const noexcept
{
	initial	 .deepLoadRequested(registry, stream);
	remaining.wideLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideLoadResumable(Registry& registry,
											  ReadStream& stream) const noexcept
{
	initial	 .deepLoadResumable(registry, stream);
	remaining.wideLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideGetNames(const Long parent,
										 const Short depth,
										 StructureStateInfos& stateInfos) const noexcept
{
	initial	 .deepGetNames(parent, StructureStateInfo::RegionType::ORTHOGONAL, depth, stateInfos);
	remaining.wideGetNames(parent,											   depth, stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
