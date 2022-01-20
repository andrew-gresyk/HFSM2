namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideRegister(Registry& registry,
										 const ForkID forkId) noexcept
{
	Initial	 ::deepRegister(registry, Parent{forkId, PRONG_INDEX});
	Remaining::wideRegister(registry, forkId);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardEntryGuard(GuardControl& control,
												  const ProngCBits prongs) noexcept
{
	const bool i = prongs.get(PRONG_INDEX) ?
				   Initial	::deepForwardEntryGuard(control) : false;

	const bool r = Remaining::wideForwardEntryGuard(control, prongs);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardEntryGuard(GuardControl& control) noexcept {
	const bool i = Initial	::deepForwardEntryGuard(control);
	const bool r = Remaining::wideForwardEntryGuard(control);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI, TR...>::wideEntryGuard(GuardControl& control) noexcept {
	const bool i = Initial	::deepEntryGuard(control);
	const bool r = Remaining::wideEntryGuard(control);

	return i || r;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideEnter(PlanControl& control) noexcept {
	Initial	 ::deepEnter(control);
	Remaining::wideEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideReenter(PlanControl& control) noexcept {
	Initial	 ::deepReenter(control);
	Remaining::wideReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI, TR...>::wideUpdate(FullControl& control) noexcept {
	const auto status =	   Initial	::deepUpdate(control);
	return combine(status, Remaining::wideUpdate(control));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI, TR...>::wideReverseUpdate(FullControl& control) noexcept {
	const auto status =	   Initial	::deepReverseUpdate(control);
	return combine(status, Remaining::wideReverseUpdate(control));
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI, TR...>::wideReact(FullControl& control,
									  const TEvent& event) noexcept
{
	const auto status =	   Initial	::deepReact(control, event);
	return combine(status, Remaining::wideReact(control, event));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
OS_<TN, TA, NI, TI, TR...>::wideReverseReact(FullControl& control,
											 const TEvent& event) noexcept
{
	const auto status =	   Initial	::deepReverseReact(control, event);
	return combine(status, Remaining::wideReverseReact(control, event));
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardExitGuard(GuardControl& control,
												 const ProngCBits prongs) noexcept
{
	const bool i = prongs.get(PRONG_INDEX) ?
				   Initial	::deepForwardExitGuard(control) : false;

	const bool r = Remaining::wideForwardExitGuard(control, prongs);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardExitGuard(GuardControl& control) noexcept {
	const bool i = Initial	::deepForwardExitGuard(control);
	const bool r = Remaining::wideForwardExitGuard(control);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI, TR...>::wideExitGuard(GuardControl& control) noexcept {
	const bool i = Initial	::deepExitGuard(control);
	const bool r = Remaining::wideExitGuard(control);

	return i || r;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideExit(PlanControl& control) noexcept {
	Initial	 ::deepExit(control);
	Remaining::wideExit(control);
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
		Initial::deepForwardActive(control, request);

	Remaining  ::wideForwardActive(control, request, prongs);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideForwardRequest(Control& control,
											   const Request request) noexcept
{
	Initial	 ::deepForwardRequest(control, request);
	Remaining::wideForwardRequest(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideRequestChange(Control& control,
											  const Request request) noexcept
{
	Initial	 ::deepRequestChange(control, request);
	Remaining::wideRequestChange(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideRequestRestart(Control& control,
											   const Request request) noexcept
{
	Initial	 ::deepRequestRestart(control, request);
	Remaining::wideRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideRequestResume(Control& control,
											  const Request request) noexcept
{
	Initial	 ::deepRequestResume(control, request);
	Remaining::wideRequestResume(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideRequestUtilize(Control& control,
											   const Request request) noexcept
{
	Initial	 ::deepRequestUtilize(control, request);
	Remaining::wideRequestUtilize(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideRequestRandomize(Control& control,
												 const Request request) noexcept
{
	Initial	 ::deepRequestRandomize(control, request);
	Remaining::wideRequestRandomize(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
typename TA::Utility
OS_<TN, TA, NI, TI, TR...>::wideReportChange(Control& control) noexcept {
	const UP	  i = Initial  ::deepReportChange(control);
	const Utility r = Remaining::wideReportChange(control);

	return i.utility + r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
typename TA::Utility
OS_<TN, TA, NI, TI, TR...>::wideReportUtilize(Control& control) noexcept {
	const UP	  i = Initial  ::deepReportUtilize(control);
	const Utility r = Remaining::wideReportUtilize(control);

	return i.utility + r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
typename TA::Utility
OS_<TN, TA, NI, TI, TR...>::wideReportRandomize(Control& control) noexcept {
	const Utility i = Initial  ::deepReportRandomize(control);
	const Utility r = Remaining::wideReportRandomize(control);

	return i + r;
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideChangeToRequested(PlanControl& control) noexcept {
	Initial	 ::deepChangeToRequested(control);
	Remaining::wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideSaveActive(const Registry& registry,
										   WriteStream& stream) const noexcept
{
	Initial	 ::deepSaveActive(registry, stream);
	Remaining::wideSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideSaveResumable(const Registry& registry,
											  WriteStream& stream) const noexcept
{
	Initial	 ::deepSaveResumable(registry, stream);
	Remaining::wideSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideLoadRequested(Registry& registry,
											  ReadStream& stream) const noexcept
{
	Initial	 ::deepLoadRequested(registry, stream);
	Remaining::wideLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI, typename... TR>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI, TR...>::wideLoadResumable(Registry& registry,
											  ReadStream& stream) const noexcept
{
	Initial	 ::deepLoadResumable(registry, stream);
	Remaining::wideLoadResumable(registry, stream);
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
	Initial	 ::deepGetNames(parent, StructureStateInfo::RegionType::ORTHOGONAL, depth, stateInfos);
	Remaining::wideGetNames(parent,												depth, stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
