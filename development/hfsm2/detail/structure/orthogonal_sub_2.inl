namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRegister(Registry& registry,
								  const ForkID forkId) noexcept
{
	Initial		   ::deepRegister(			registry, Parent{forkId, PRONG_INDEX});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI>::wideForwardEntryGuard(GuardControl& control,
										   const ProngCBits prongs) noexcept
{
	return prongs.get(PRONG_INDEX) ?
		Initial	   ::deepForwardEntryGuard(				 control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI>::wideForwardEntryGuard(GuardControl& control) noexcept {
	return Initial ::deepForwardEntryGuard(				 control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI>::wideEntryGuard(GuardControl& control) noexcept {
	return Initial ::deepEntryGuard(			  control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideEnter(PlanControl& control) noexcept {
	Initial		   ::deepEnter(				control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideReenter(PlanControl& control) noexcept {
	Initial		   ::deepReenter(			  control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN, TA, NI, TI>::widePreUpdate(FullControl& control) noexcept {
	return Initial ::deepPreUpdate(				control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN, TA, NI, TI>::wideUpdate(FullControl& control) noexcept {
	return Initial ::deepUpdate(			 control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN, TA, NI, TI>::widePostUpdate(FullControl& control) noexcept {
	return Initial ::deepPostUpdate(			 control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN, TA, NI, TI>::widePreReact(FullControl& control,
								  const TEvent& event) noexcept
{
	return Initial ::deepPreReact(			   control, event);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN, TA, NI, TI>::wideReact(FullControl& control,
							   const TEvent& event) noexcept
{
	return Initial ::deepReact(				control, event);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN, TA, NI, TI>::widePostReact(FullControl& control,
								   const TEvent& event) noexcept
{
	return Initial ::deepPostReact(				control, event);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideQuery(ConstControl& control,
							   TEvent& event) const noexcept
{
	Initial		   ::wideQuery(				 control, event);
}

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
TaskStatus
OS_<TN, TA, NI, TI>::wideUpdatePlans(FullControl& control) noexcept {
	return Initial ::deepUpdatePlans(			  control);
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI>::wideForwardExitGuard(GuardControl& control,
										  const ProngCBits prongs) noexcept
{
	return prongs.get(PRONG_INDEX) ?
		Initial	   ::deepForwardExitGuard(				control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI>::wideForwardExitGuard(GuardControl& control) noexcept {
	return Initial ::deepForwardExitGuard(				control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
bool
OS_<TN, TA, NI, TI>::wideExitGuard(GuardControl& control) noexcept {
	return Initial ::deepExitGuard(				 control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideExit(PlanControl& control) noexcept {
	Initial		   ::deepExit(			   control);
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
		Initial	   ::deepForwardActive(			control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideForwardRequest(Control& control,
										const Request request) noexcept
{
	Initial		   ::deepForwardRequest(		 control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRequestChange(Control& control,
									   const Request request) noexcept
{
	Initial		   ::deepRequestChange(			control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRequestRestart(Control& control,
										const Request request) noexcept
{
	Initial		   ::deepRequestRestart(		 control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRequestResume(Control& control,
									   const Request request) noexcept
{
	Initial		   ::deepRequestResume(			control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRequestSelect(Control& control,
									   const Request request) noexcept
{
	Initial		   ::deepRequestSelect(			control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRequestUtilize(Control& control,
										const Request request) noexcept
{
	Initial		   ::deepRequestUtilize(		 control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideRequestRandomize(Control& control,
										  const Request request) noexcept
{
	Initial		   ::deepRequestRandomize(		   control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
typename TA::Utility
OS_<TN, TA, NI, TI>::wideReportChange(Control& control) noexcept {
	const UP i = Initial::deepReportChange(control);

	return i.utility;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
typename TA::Utility
OS_<TN, TA, NI, TI>::wideReportUtilize(Control& control) noexcept {
	const UP i = Initial::deepReportUtilize(control);

	return i.utility;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
typename TA::Utility
OS_<TN, TA, NI, TI>::wideReportRandomize(Control& control) noexcept {
	return Initial::deepReportRandomize(control);
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideChangeToRequested(PlanControl& control) noexcept {
	Initial		   ::deepChangeToRequested(				control);
}

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideSaveActive(const Registry& registry,
									WriteStream& stream) const noexcept
{
	Initial		   ::deepSaveActive(				registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideSaveResumable(const Registry& registry,
									   WriteStream& stream) const noexcept
{
	Initial		   ::deepSaveResumable(				   registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideLoadRequested(Registry& registry,
									   ReadStream& stream) const noexcept
{
	Initial		   ::deepLoadRequested(			 registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideLoadResumable(Registry& registry,
									   ReadStream& stream) const noexcept
{
	Initial		   ::deepLoadResumable(			 registry, stream);
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN, typename TA, Short NI, typename TI>
HFSM2_CONSTEXPR(14)
void
OS_<TN, TA, NI, TI>::wideGetNames(const Long parent,
								  const Short depth,
								  StructureStateInfos& stateInfos) const noexcept
{
	Initial		   ::deepGetNames(parent, StructureStateInfo::RegionType::ORTHOGONAL, depth, stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
