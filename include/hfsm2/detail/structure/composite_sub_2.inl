namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
void
CS_<TN_, TA_, TG_, NI_, T>::wideRegister(Registry& registry,
										 const Parent parent)
{
	state.deepRegister(registry, Parent{parent.forkId, PRONG_INDEX});
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
bool
CS_<TN_, TA_, TG_, NI_, T>::wideForwardEntryGuard(GuardControl& control,
												  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
bool
CS_<TN_, TA_, TG_, NI_, T>::wideEntryGuard(GuardControl& control,
										   const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
void
CS_<TN_, TA_, TG_, NI_, T>::wideConstruct(PlanControl& control,
										  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepConstruct(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
void
CS_<TN_, TA_, TG_, NI_, T>::wideEnter(PlanControl& control,
									  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
void
CS_<TN_, TA_, TG_, NI_, T>::wideReenter(PlanControl& control,
										const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
Status
CS_<TN_, TA_, TG_, NI_, T>::wideUpdate(FullControl& control,
									   const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
template <typename TEvent>
Status
CS_<TN_, TA_, TG_, NI_, T>::wideReact(FullControl& control,
									  const TEvent& event,
									  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepReact(control, event);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
bool
CS_<TN_, TA_, TG_, NI_, T>::wideForwardExitGuard(GuardControl& control,
												 const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
bool
CS_<TN_, TA_, TG_, NI_, T>::wideExitGuard(GuardControl& control,
										  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
void
CS_<TN_, TA_, TG_, NI_, T>::wideExit(PlanControl& control,
									 const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
void
CS_<TN_, TA_, TG_, NI_, T>::wideDestruct(PlanControl& control,
										 const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepDestruct(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
void
CS_<TN_, TA_, TG_, NI_, T>::wideForwardActive(Control& control,
											  const Request::Type request,
											  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepForwardActive(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
void
CS_<TN_, TA_, TG_, NI_, T>::wideForwardRequest(Control& control,
											   const Request::Type request,
											   const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepForwardRequest(control, request);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
void
CS_<TN_, TA_, TG_, NI_, T>::wideRequestChangeComposite(Control& control) {
	state.deepRequestChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
void
CS_<TN_, TA_, TG_, NI_, T>::wideRequestChangeResumable(Control& control,
													   const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepRequestChange(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
void
CS_<TN_, TA_, TG_, NI_, T>::wideRequestRemain(Registry& registry) {
	state.deepRequestRemain(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
void
CS_<TN_, TA_, TG_, NI_, T>::wideRequestRestart(Registry& registry) {
	state.deepRequestRestart(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
void
CS_<TN_, TA_, TG_, NI_, T>::wideRequestResume(Registry& registry,
											  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepRequestResume(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
typename TA_::UP
CS_<TN_, TA_, TG_, NI_, T>::wideReportUtilize(Control& control) {
	return state.deepReportUtilize(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
typename TA_::Rank
CS_<TN_, TA_, TG_, NI_, T>::wideReportRank(Control& control,
										   Rank* const ranks)
{
	HFSM_ASSERT(ranks);

	*ranks = state.deepReportRank(control);

	return *ranks;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
typename TA_::Utility
CS_<TN_, TA_, TG_, NI_, T>::wideReportRandomize(Control& control,
												Utility* const options,
												const Rank* const ranks,
												const Rank top)
{
	HFSM_ASSERT(options && ranks);

	*options = (*ranks == top) ?
		state.deepReportRandomize(control) : Utility{0.0f};

	return *options;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
typename TA_::UP
CS_<TN_, TA_, TG_, NI_, T>::wideReportChangeComposite(Control& control) {
	return state.deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
typename TA_::UP
CS_<TN_, TA_, TG_, NI_, T>::wideReportChangeResumable(Control& control,
													  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
typename TA_::UP
CS_<TN_, TA_, TG_, NI_, T>::wideReportChangeUtilitarian(Control& control) {
	return state.deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
typename TA_::Utility
CS_<TN_, TA_, TG_, NI_, T>::wideReportChangeRandom(Control& control,
												   Utility* const options,
												   const Rank* const ranks,
												   const Rank top)
{
	HFSM_ASSERT(options && ranks);

	*options = (*ranks == top) ?
		state.deepReportChange(control).utility : Utility{0.0f};

	return *options;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
void
CS_<TN_, TA_, TG_, NI_, T>::wideChangeToRequested(PlanControl& control,
												  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
void
CS_<TN_, TA_, TG_, NI_, T>::wideGetNames(const LongIndex parent,
										 const RegionType /*region*/,
										 const ShortIndex depth,
										 StructureStateInfos& _stateInfos) const
{
	state.deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_SERIALIZATION

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
void
CS_<TN_, TA_, TG_, NI_, T>::wideSaveActive(const Registry& registry,
										   WriteStream& stream,
										   const ShortIndex HFSM_IF_ASSERT(prong)) const
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
void
CS_<TN_, TA_, TG_, NI_, T>::wideSaveResumable(const Registry& registry,
											  WriteStream& stream) const
{
	state.deepSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
void
CS_<TN_, TA_, TG_, NI_, T>::wideLoadRequested(Registry& registry,
											  ReadStream& stream,
											  const ShortIndex HFSM_IF_ASSERT(prong)) const
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, ShortIndex NI_, typename T>
void
CS_<TN_, TA_, TG_, NI_, T>::wideLoadResumable(Registry& registry,
											  ReadStream& stream) const
{
	state.deepLoadResumable(registry, stream);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
