namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideRegister(Registry& registry,
										  const Parent parent) noexcept
{
	State::deepRegister(registry, Parent{parent.forkId, PRONG_INDEX});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, TL_<T>>::wideForwardEntryGuard(GuardControl& control,
												   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return State::deepForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, TL_<T>>::wideEntryGuard(GuardControl& control,
											const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return State::deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideEnter(PlanControl& control,
									   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	State::deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideReenter(PlanControl& control,
										 const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	State::deepReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<T>>::wideUpdate(FullControl& control,
										const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return State::deepUpdate(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<T>>::wideReverseUpdate(FullControl& control,
											   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return State::deepReverseUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<T>>::wideReact(FullControl& control,
									   const TEvent& event,
									   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return State::deepReact(control, event);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<T>>::wideReverseReact(FullControl& control,
											  const TEvent& event,
											  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return State::deepReverseReact(control, event);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, TL_<T>>::wideForwardExitGuard(GuardControl& control,
												  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return State::deepForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, TL_<T>>::wideExitGuard(GuardControl& control,
										   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return State::deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideExit(PlanControl& control,
									  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	State::deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideForwardActive(Control& control,
											   const Request request,
											   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	State::deepForwardActive(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideForwardRequest(Control& control,
												const Request request,
												const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	State::deepForwardRequest(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideRequestChangeComposite(Control& control,
														const Request request) noexcept
{
	State::deepRequestChange(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideRequestChangeResumable(Control& control,
														const Request request,
														const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	State::deepRequestChange(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideRequestRestart(Control& control,
												const Request request) noexcept
{
	State::deepRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideRequestResume(Control& control,
											   const Request request,
											   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	State::deepRequestResume(control, request);
}

//------------------------------------------------------------------------------

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<T>>::wideReportUtilize(Control& control) noexcept {
	return State::deepReportUtilize(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::Rank
CS_<TN, TA, SG, NI, TL_<T>>::wideReportRank(Control& control,
											Rank* const ranks) noexcept
{
	HFSM2_ASSERT(ranks);

	*ranks = State::deepReportRank(control);

	return *ranks;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::Utility
CS_<TN, TA, SG, NI, TL_<T>>::wideReportRandomize(Control& control,
												 Utility* const options,
												 const Rank* const ranks,
												 const Rank top) noexcept
{
	HFSM2_ASSERT(options && ranks);

	*options = (*ranks == top) ?
		State::deepReportRandomize(control) : Utility{0};

	return *options;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<T>>::wideReportChangeComposite(Control& control) noexcept {
	return State::deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<T>>::wideReportChangeResumable(Control& control,
													   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return State::deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<T>>::wideReportChangeUtilitarian(Control& control) noexcept {
	return State::deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::Utility
CS_<TN, TA, SG, NI, TL_<T>>::wideReportChangeRandom(Control& control,
													Utility* const options,
													const Rank* const ranks,
													const Rank top) noexcept
{
	HFSM2_ASSERT(options && ranks);

	*options = (*ranks == top) ?
		State::deepReportChange(control).utility : Utility{0};

	return *options;
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideChangeToRequested(PlanControl& control,
												   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	State::deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideSaveActive(const Registry& registry,
											WriteStream& stream,
											const Short HFSM2_IF_ASSERT(prong)) const noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	State::deepSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideSaveResumable(const Registry& registry,
											   WriteStream& stream) const noexcept
{
	State::deepSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideLoadRequested(Registry& registry,
											   ReadStream& stream,
											   const Short HFSM2_IF_ASSERT(prong)) const noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	State::deepLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideLoadResumable(Registry& registry,
											   ReadStream& stream) const noexcept
{
	State::deepLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideGetNames(const Long parent,
										  const RegionType /*region*/,
										  const Short depth,
										  StructureStateInfos& _stateInfos) const noexcept
{
	State::deepGetNames(parent, StructureStateInfo::RegionType::COMPOSITE, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
