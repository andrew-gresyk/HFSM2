namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideRegister(Registry& registry,
											   const Parent parent) noexcept
{
	Single::deepRegister(registry, Parent{parent.forkId, PRONG_INDEX});
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
bool
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideForwardEntryGuard(GuardControl& control,
														const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
bool
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideEntryGuard(GuardControl& control,
												 const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideEnter(PlanControl& control,
											const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideReenter(PlanControl& control,
											  const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
TaskStatus
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::widePreUpdate(FullControl& control,
												const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPreUpdate(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
TaskStatus
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideUpdate(FullControl& control,
											 const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepUpdate(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
TaskStatus
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::widePostUpdate(FullControl& control,
												 const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPostUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::widePreReact(FullControl& control,
											   const TEvent& event,
											   const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPreReact(control, event);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideReact(FullControl& control,
											const TEvent& event,
											const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepReact(control, event);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
TaskStatus
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::widePostReact(FullControl& control,
												const TEvent& event,
												const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepPostReact(control, event);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideQuery(ConstControl& control,
											TEvent& event,
											const Prong HFSM2_IF_ASSERT(prong)) const noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepQuery(control, event);
}

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
TaskStatus
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideUpdatePlans(FullControl& control,
												  const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepUpdatePlans(control);
}

#endif

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
bool
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideForwardExitGuard(GuardControl& control,
													   const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// COMMON

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
bool
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideExitGuard(GuardControl& control,
												const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideExit(PlanControl& control,
										   const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideForwardActive(Control& control,
													const Request request,
													const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepForwardActive(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideForwardRequest(Control& control,
													 const Request request,
													 const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepForwardRequest(control, request);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideRequestChangeComposite(Control& control,
															 const Request request) noexcept
{
	Single::deepRequestChange(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideRequestChangeResumable(Control& control,
															 const Request request,
															 const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepRequestChange(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideRequestChangeSelectable(Control& control,
															  const Request request,
															  const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepRequestChange(control, request);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideRequestRestart(Control& control,
													 const Request request) noexcept
{
	Single::deepRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideRequestResume(Control& control,
													const Request request,
													const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepRequestResume(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideRequestSelect(Control& control,
													const Request request,
													const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepRequestSelect(control, request);
}

//------------------------------------------------------------------------------

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
typename TA_::UP
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideReportUtilize(Control& control) noexcept {
	return Single::deepReportUtilize(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
typename TA_::Rank
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideReportRank(Control& control,
												 Rank* const ranks) noexcept
{
	HFSM2_ASSERT(ranks);

	*ranks = Single::deepReportRank(control);

	return *ranks;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
typename TA_::Utility
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideReportRandomize(Control& control,
													  Utility* const utilities,
													  const Rank* const ranks,
													  const Rank top) noexcept
{
	HFSM2_ASSERT(utilities && ranks);

	*utilities = (*ranks == top) ?
		Single::deepReportRandomize(control) : Utility{0};

	return *utilities;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
typename TA_::UP
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideReportChangeComposite(Control& control) noexcept {
	return Single::deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
typename TA_::UP
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideReportChangeResumable(Control& control,
															const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
typename TA_::UP
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideReportChangeSelectable(Control& control,
															 const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return Single::deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
typename TA_::UP
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideReportChangeUtilitarian(Control& control) noexcept {
	return Single::deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
typename TA_::Utility
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideReportChangeRandom(Control& control,
														 Utility* const utilities,
														 const Rank* const ranks,
														 const Rank top) noexcept
{
	HFSM2_ASSERT(utilities && ranks);

	*utilities = (*ranks == top) ?
		Single::deepReportChange(control).utility : Utility{0};

	return *utilities;
}

#endif

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideChangeToRequested(PlanControl& control,
														const Prong HFSM2_IF_ASSERT(prong)) noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideSaveActive(const Registry& registry,
												 WriteStream& stream,
												 const Prong HFSM2_IF_ASSERT(prong)) const noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideSaveResumable(const Registry& registry,
													WriteStream& stream) const noexcept
{
	Single::deepSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideLoadRequested(Registry& registry,
													ReadStream& stream,
													const Prong HFSM2_IF_ASSERT(prong)) const noexcept
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	Single::deepLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideLoadResumable(Registry& registry,
													ReadStream& stream) const noexcept
{
	Single::deepLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TN_, typename TA_, Strategy SG_, Prong NP_, typename T_>
HFSM2_CONSTEXPR(14)
void
CS_<TN_, TA_, SG_, NP_, TL_<T_>>::wideGetNames(const Long parent,
											   const RegionType /*region*/,
											   const Short depth,
											   StructureStateInfos& stateInfos) const noexcept
{
	Single::deepGetNames(parent, RegionType::COMPOSITE, depth, stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
