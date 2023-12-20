namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_, TR_...>::wideRegister(Registry& registry,
											  const ForkID forkId) noexcept
{
	Initial	 ::deepRegister(registry, Parent{forkId, PRONG_INDEX});
	Remaining::wideRegister(registry,		 forkId				 );
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
bool
OS_<TN_, TA_, NP_, TI_, TR_...>::wideForwardEntryGuard(GuardControl& control,
													   const ProngCBits prongs) noexcept
{
	const bool i = prongs.get(PRONG_INDEX) ?
				   Initial	::deepForwardEntryGuard(control		   ) : false;

	const bool r = Remaining::wideForwardEntryGuard(control, prongs);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
bool
OS_<TN_, TA_, NP_, TI_, TR_...>::wideForwardEntryGuard(GuardControl& control) noexcept {
	const bool i = Initial	::deepForwardEntryGuard(control);
	const bool r = Remaining::wideForwardEntryGuard(control);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
bool
OS_<TN_, TA_, NP_, TI_, TR_...>::wideEntryGuard(GuardControl& control) noexcept {
	const bool i = Initial	::deepEntryGuard(control);
	const bool r = Remaining::wideEntryGuard(control);

	return i || r;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_, TR_...>::wideEnter(PlanControl& control) noexcept {
	Initial	 ::deepEnter(control);
	Remaining::wideEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_, TR_...>::wideReenter(PlanControl& control) noexcept {
	Initial	 ::deepReenter(control);
	Remaining::wideReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN_, TA_, NP_, TI_, TR_...>::widePreUpdate(FullControl& control) noexcept {
	TaskStatus status;
	status |= Initial  ::deepPreUpdate(control);
	status |= Remaining::widePreUpdate(control);

	return status;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN_, TA_, NP_, TI_, TR_...>::wideUpdate(FullControl& control) noexcept {
	TaskStatus status;
	status |= Initial  ::deepUpdate(control);
	status |= Remaining::wideUpdate(control);

	return status;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN_, TA_, NP_, TI_, TR_...>::widePostUpdate(FullControl& control) noexcept {
	TaskStatus status;
	status |= Initial  ::deepPostUpdate(control);
	status |= Remaining::widePostUpdate(control);

	return status;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN_, TA_, NP_, TI_, TR_...>::widePreReact(FullControl& control,
											  const TEvent& event) noexcept
{
	TaskStatus status;
	status |= Initial  ::deepPreReact(control, event);
	status |= Remaining::widePreReact(control, event);

	return status;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN_, TA_, NP_, TI_, TR_...>::wideReact(FullControl& control,
										   const TEvent& event) noexcept
{
	TaskStatus status;
	status |= Initial  ::deepReact(control, event);
	status |= Remaining::wideReact(control, event);

	return status;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN_, TA_, NP_, TI_, TR_...>::widePostReact(FullControl& control,
											   const TEvent& event) noexcept
{
	TaskStatus status;
	status |= Initial  ::deepPostReact(control, event);
	status |= Remaining::widePostReact(control, event);

	return status;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_, TR_...>::wideQuery(ConstControl& control,
										   TEvent& event) const noexcept
{
	Initial  ::deepQuery(control, event);
	Remaining::wideQuery(control, event);
}

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN_, TA_, NP_, TI_, TR_...>::wideUpdatePlans(FullControl& control) noexcept {
	TaskStatus status;
	status |= Initial  ::deepUpdatePlans(control);
	status |= Remaining::wideUpdatePlans(control);

	return status;
}

#endif

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
bool
OS_<TN_, TA_, NP_, TI_, TR_...>::wideForwardExitGuard(GuardControl& control,
													  const ProngCBits prongs) noexcept
{
	const bool i = prongs.get(PRONG_INDEX) ?
				   Initial	::deepForwardExitGuard(control		  ) : false;

	const bool r = Remaining::wideForwardExitGuard(control, prongs);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
bool
OS_<TN_, TA_, NP_, TI_, TR_...>::wideForwardExitGuard(GuardControl& control) noexcept {
	const bool i = Initial	::deepForwardExitGuard(control);
	const bool r = Remaining::wideForwardExitGuard(control);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
bool
OS_<TN_, TA_, NP_, TI_, TR_...>::wideExitGuard(GuardControl& control) noexcept {
	const bool i = Initial	::deepExitGuard(control);
	const bool r = Remaining::wideExitGuard(control);

	return i || r;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_, TR_...>::wideExit(PlanControl& control) noexcept {
	Initial	 ::deepExit(control);
	Remaining::wideExit(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_, TR_...>::wideForwardActive(Control& control,
												   const Request request,
												   const ProngCBits prongs) noexcept
{
	if (prongs.get(PRONG_INDEX))
		Initial::deepForwardActive(control, request		   );

	Remaining  ::wideForwardActive(control, request, prongs);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_, TR_...>::wideForwardRequest(Control& control,
													const Request request) noexcept
{
	Initial	 ::deepForwardRequest(control, request);
	Remaining::wideForwardRequest(control, request);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_, TR_...>::wideRequestChange(Control& control,
												   const Request request) noexcept
{
	Initial	 ::deepRequestChange(control, request);
	Remaining::wideRequestChange(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_, TR_...>::wideRequestRestart(Control& control,
													const Request request) noexcept
{
	Initial	 ::deepRequestRestart(control, request);
	Remaining::wideRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_, TR_...>::wideRequestResume(Control& control,
												   const Request request) noexcept
{
	Initial	 ::deepRequestResume(control, request);
	Remaining::wideRequestResume(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_, TR_...>::wideRequestSelect(Control& control,
												   const Request request) noexcept
{
	Initial	 ::deepRequestSelect(control, request);
	Remaining::wideRequestSelect(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_, TR_...>::wideRequestUtilize(Control& control,
													const Request request) noexcept
{
	Initial	 ::deepRequestUtilize(control, request);
	Remaining::wideRequestUtilize(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_, TR_...>::wideRequestRandomize(Control& control,
													  const Request request) noexcept
{
	Initial	 ::deepRequestRandomize(control, request);
	Remaining::wideRequestRandomize(control, request);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
typename TA_::Utility
OS_<TN_, TA_, NP_, TI_, TR_...>::wideReportChange(Control& control) noexcept {
	const UP	  i = Initial  ::deepReportChange(control);
	const Utility r = Remaining::wideReportChange(control);

	return i.utility + r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
typename TA_::Utility
OS_<TN_, TA_, NP_, TI_, TR_...>::wideReportUtilize(Control& control) noexcept {
	const UP	  i = Initial  ::deepReportUtilize(control);
	const Utility r = Remaining::wideReportUtilize(control);

	return i.utility + r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
typename TA_::Utility
OS_<TN_, TA_, NP_, TI_, TR_...>::wideReportRandomize(Control& control) noexcept {
	const Utility i = Initial  ::deepReportRandomize(control);
	const Utility r = Remaining::wideReportRandomize(control);

	return i + r;
}

#endif

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_, TR_...>::wideChangeToRequested(PlanControl& control) noexcept {
	Initial	 ::deepChangeToRequested(control);
	Remaining::wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_, TR_...>::wideSaveActive(const Registry& registry,
												WriteStream& stream) const noexcept
{
	Initial	 ::deepSaveActive(registry, stream);
	Remaining::wideSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_, TR_...>::wideSaveResumable(const Registry& registry,
												   WriteStream& stream) const noexcept
{
	Initial	 ::deepSaveResumable(registry, stream);
	Remaining::wideSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_, TR_...>::wideLoadRequested(Registry& registry,
												   ReadStream& stream) const noexcept
{
	Initial	 ::deepLoadRequested(registry, stream);
	Remaining::wideLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_, TR_...>::wideLoadResumable(Registry& registry,
												   ReadStream& stream) const noexcept
{
	Initial	 ::deepLoadResumable(registry, stream);
	Remaining::wideLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN_, typename TA_, Prong NP_, typename TI_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
OS_<TN_, TA_, NP_, TI_, TR_...>::wideGetNames(const Long parent,
											  const Short depth,
											  StructureStateInfos& stateInfos) const noexcept
{
	Initial	 ::deepGetNames(parent, RegionType::ORTHOGONAL, depth, stateInfos);
	Remaining::wideGetNames(parent						  , depth, stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
