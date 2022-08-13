namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideRegister(Registry& registry,
										  const Parent parent) noexcept
{
	Single::deepRegister(registry, Parent{parent.forkId, PRONG_INDEX});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, TL_<T>>::wideForwardEntryGuard(GuardControl& control,
												   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepForwardEntryGuard(control);
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

	return Single::deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideEnter(PlanControl& control,
									   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideReenter(PlanControl& control,
										 const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<T>>::widePreUpdate(FullControl& control,
										   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPreUpdate(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<T>>::wideUpdate(FullControl& control,
										const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepUpdate(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<T>>::widePostUpdate(FullControl& control,
											const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPostUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<T>>::widePreReact(FullControl& control,
										  const TEvent& event,
										  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPreReact(control, event);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<T>>::wideReact(FullControl& control,
									   const TEvent& event,
									   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepReact(control, event);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<T>>::widePostReact(FullControl& control,
										   const TEvent& event,
										   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPostReact(control, event);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideQuery(ConstControl& control,
									   TEvent& event,
									   const Short HFSM2_IF_ASSERT(prong)) const noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepQuery(control, event);
}

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
Status
CS_<TN, TA, SG, NI, TL_<T>>::wideUpdatePlans(FullControl& control,
											 const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepUpdatePlans(control);
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
bool
CS_<TN, TA, SG, NI, TL_<T>>::wideForwardExitGuard(GuardControl& control,
												  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepForwardExitGuard(control);
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

	return Single::deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideExit(PlanControl& control,
									  const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepExit(control);
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

	Single::deepForwardActive(control, request);
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

	Single::deepForwardRequest(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideRequestChangeComposite(Control& control,
														const Request request) noexcept
{
	Single::deepRequestChange(control, request);
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

	Single::deepRequestChange(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideRequestChangeSelectable(Control& control,
														 const Request request,
														 const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepRequestChange(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideRequestRestart(Control& control,
												const Request request) noexcept
{
	Single::deepRequestRestart(control, request);
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

	Single::deepRequestResume(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideRequestSelect(Control& control,
											   const Request request,
											   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepRequestSelect(control, request);
}

//------------------------------------------------------------------------------

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<T>>::wideReportUtilize(Control& control) noexcept {
	return Single::deepReportUtilize(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::Rank
CS_<TN, TA, SG, NI, TL_<T>>::wideReportRank(Control& control,
											Rank* const ranks) noexcept
{
	HFSM2_ASSERT(ranks);

	*ranks = Single::deepReportRank(control);

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
		Single::deepReportRandomize(control) : Utility{0};

	return *options;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<T>>::wideReportChangeComposite(Control& control) noexcept {
	return Single::deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<T>>::wideReportChangeResumable(Control& control,
													   const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<T>>::wideReportChangeSelectable(Control& control,
														const Short HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
typename TA::UP
CS_<TN, TA, SG, NI, TL_<T>>::wideReportChangeUtilitarian(Control& control) noexcept {
	return Single::deepReportChange(control);
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
		Single::deepReportChange(control).utility : Utility{0};

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

	Single::deepChangeToRequested(control);
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

	Single::deepSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideSaveResumable(const Registry& registry,
											   WriteStream& stream) const noexcept
{
	Single::deepSaveResumable(registry, stream);
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

	Single::deepLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, Short NI, typename T>
HFSM2_CONSTEXPR(14)
void
CS_<TN, TA, SG, NI, TL_<T>>::wideLoadResumable(Registry& registry,
											   ReadStream& stream) const noexcept
{
	Single::deepLoadResumable(registry, stream);
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
	Single::deepGetNames(parent, StructureStateInfo::RegionType::COMPOSITE, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
