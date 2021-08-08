namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, T>::wideRegister(Registry& registry,
									 const Parent parent) noexcept
{
	state.deepRegister(registry, Parent{parent.forkId, PRONG_INDEX});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, T>::wideForwardEntryGuard(GuardControl& control,
											  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, T>::wideEntryGuard(GuardControl& control,
									   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, T>::wideEnter(PlanControl& control,
								  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, T>::wideReenter(PlanControl& control,
									const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, T>::wideUpdate(FullControl& control,
								   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, T>::wideReact(FullControl& control,
								  const TEvent& event,
								  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepReact(control, event);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, T>::wideForwardExitGuard(GuardControl& control,
											 const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, T>::wideExitGuard(GuardControl& control,
									  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, T>::wideExit(PlanControl& control,
								 const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, T>::wideForwardActive(Control& control,
										  const Request request,
										  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepForwardActive(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, T>::wideForwardRequest(Control& control,
										   const Request request,
										   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepForwardRequest(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, T>::wideRequestChangeComposite(Control& control,
												   const Request request) noexcept
{
	state.deepRequestChange(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, T>::wideRequestChangeResumable(Control& control,
												   const Request request,
												   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepRequestChange(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, T>::wideRequestRestart(Control& control,
										   const Request request) noexcept
{
	state.deepRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, T>::wideRequestResume(Control& control,
										  const Request request,
										  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepRequestResume(control, request);
}

//------------------------------------------------------------------------------

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, T>::wideReportUtilize(Control& control) noexcept {
	return state.deepReportUtilize(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::Rank
CS_<TN, TA, SG, NI, T>::wideReportRank(Control& control,
									   Rank* const ranks) noexcept
{
	HFSM2_ASSERT(ranks);

	*ranks = state.deepReportRank(control);

	return *ranks;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::Utility
CS_<TN, TA, SG, NI, T>::wideReportRandomize(Control& control,
											Utility* const options,
											const Rank* const ranks,
											const Rank top) noexcept
{
	HFSM2_ASSERT(options && ranks);

	*options = (*ranks == top) ?
		state.deepReportRandomize(control) : Utility{0};

	return *options;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, T>::wideReportChangeComposite(Control& control) noexcept {
	return state.deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, T>::wideReportChangeResumable(Control& control,
												  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, T>::wideReportChangeUtilitarian(Control& control) noexcept {
	return state.deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::Utility
CS_<TN, TA, SG, NI, T>::wideReportChangeRandom(Control& control,
											   Utility* const options,
											   const Rank* const ranks,
											   const Rank top) noexcept
{
	HFSM2_ASSERT(options && ranks);

	*options = (*ranks == top) ?
		state.deepReportChange(control).utility : Utility{0};

	return *options;
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, T>::wideChangeToRequested(PlanControl& control,
											  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(11)
void
CS_<TN, TA, SG, NI, T>::wideSaveActive(const Registry& registry,
									   WriteStream& stream,
									   const Short HFSM2_IF_ASSERT(prong)) const noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(11)
void
CS_<TN, TA, SG, NI, T>::wideSaveResumable(const Registry& registry,
										  WriteStream& stream) const noexcept
{
	state.deepSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(11)
void
CS_<TN, TA, SG, NI, T>::wideLoadRequested(Registry& registry,
										  ReadStream& stream,
										  const Short HFSM2_IF_ASSERT(prong)) const noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(11)
void
CS_<TN, TA, SG, NI, T>::wideLoadResumable(Registry& registry,
										  ReadStream& stream) const noexcept
{
	state.deepLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(11)
void
CS_<TN, TA, SG, NI, T>::wideGetNames(const Long parent,
									 const RegionType /*region*/,
									 const Short depth,
									 StructureStateInfos& _stateInfos) const noexcept
{
	state.deepGetNames(parent, StructureStateInfo::RegionType::COMPOSITE, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
