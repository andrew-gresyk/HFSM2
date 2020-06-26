namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideRegister(Registry& registry,
									 const Parent parent)
{
	state.deepRegister(registry, Parent{parent.forkId, PRONG_INDEX});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
bool
CS_<TN, TA, SG, NI, T>::wideForwardEntryGuard(GuardControl& control,
											  const ShortIndex HFSM2_IF_ASSERT(prong))
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
bool
CS_<TN, TA, SG, NI, T>::wideEntryGuard(GuardControl& control,
									   const ShortIndex HFSM2_IF_ASSERT(prong))
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideConstruct(PlanControl& control,
									  const ShortIndex HFSM2_IF_ASSERT(prong))
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepConstruct(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideEnter(PlanControl& control,
								  const ShortIndex HFSM2_IF_ASSERT(prong))
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideReenter(PlanControl& control,
									const ShortIndex HFSM2_IF_ASSERT(prong))
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
Status
CS_<TN, TA, SG, NI, T>::wideUpdate(FullControl& control,
								   const ShortIndex HFSM2_IF_ASSERT(prong))
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
template <typename TEvent>
Status
CS_<TN, TA, SG, NI, T>::wideReact(FullControl& control,
								  const TEvent& event,
								  const ShortIndex HFSM2_IF_ASSERT(prong))
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepReact(control, event);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
bool
CS_<TN, TA, SG, NI, T>::wideForwardExitGuard(GuardControl& control,
											 const ShortIndex HFSM2_IF_ASSERT(prong))
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
bool
CS_<TN, TA, SG, NI, T>::wideExitGuard(GuardControl& control,
									  const ShortIndex HFSM2_IF_ASSERT(prong))
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideExit(PlanControl& control,
								 const ShortIndex HFSM2_IF_ASSERT(prong))
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideDestruct(PlanControl& control,
									 const ShortIndex HFSM2_IF_ASSERT(prong))
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepDestruct(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideForwardActive(Control& control,
										  const Request::Type request,
										  const ShortIndex HFSM2_IF_ASSERT(prong))
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepForwardActive(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideForwardRequest(Control& control,
										   const Request::Type request,
										   const ShortIndex HFSM2_IF_ASSERT(prong))
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepForwardRequest(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideRequestChangeComposite(Control& control) {
	state.deepRequestChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideRequestChangeResumable(Control& control,
												   const ShortIndex HFSM2_IF_ASSERT(prong))
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepRequestChange(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideRequestRemain(Registry& registry) {
	state.deepRequestRemain(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideRequestRestart(Registry& registry) {
	state.deepRequestRestart(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideRequestResume(Registry& registry,
										  const ShortIndex HFSM2_IF_ASSERT(prong))
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepRequestResume(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
typename TA::UP
CS_<TN, TA, SG, NI, T>::wideReportUtilize(Control& control) {
	return state.deepReportUtilize(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
typename TA::Rank
CS_<TN, TA, SG, NI, T>::wideReportRank(Control& control,
									   Rank* const ranks)
{
	HFSM2_ASSERT(ranks);

	*ranks = state.deepReportRank(control);

	return *ranks;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
typename TA::Utility
CS_<TN, TA, SG, NI, T>::wideReportRandomize(Control& control,
											Utility* const options,
											const Rank* const ranks,
											const Rank top)
{
	HFSM2_ASSERT(options && ranks);

	*options = (*ranks == top) ?
		state.deepReportRandomize(control) : Utility{0.0f};

	return *options;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
typename TA::UP
CS_<TN, TA, SG, NI, T>::wideReportChangeComposite(Control& control) {
	return state.deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
typename TA::UP
CS_<TN, TA, SG, NI, T>::wideReportChangeResumable(Control& control,
												  const ShortIndex HFSM2_IF_ASSERT(prong))
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	return state.deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
typename TA::UP
CS_<TN, TA, SG, NI, T>::wideReportChangeUtilitarian(Control& control) {
	return state.deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
typename TA::Utility
CS_<TN, TA, SG, NI, T>::wideReportChangeRandom(Control& control,
											   Utility* const options,
											   const Rank* const ranks,
											   const Rank top)
{
	HFSM2_ASSERT(options && ranks);

	*options = (*ranks == top) ?
		state.deepReportChange(control).utility : Utility{0.0f};

	return *options;
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideChangeToRequested(PlanControl& control,
											  const ShortIndex HFSM2_IF_ASSERT(prong))
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideSaveActive(const Registry& registry,
									   WriteStream& stream,
									   const ShortIndex HFSM2_IF_ASSERT(prong)) const
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepSaveActive(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideSaveResumable(const Registry& registry,
										  WriteStream& stream) const
{
	state.deepSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideLoadRequested(Registry& registry,
										  ReadStream& stream,
										  const ShortIndex HFSM2_IF_ASSERT(prong)) const
{
	HFSM2_ASSERT(prong == PRONG_INDEX);

	state.deepLoadRequested(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideLoadResumable(Registry& registry,
										  ReadStream& stream) const
{
	state.deepLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, Strategy SG, ShortIndex NI, typename T>
void
CS_<TN, TA, SG, NI, T>::wideGetNames(const LongIndex parent,
									 const RegionType /*region*/,
									 const ShortIndex depth,
									 StructureStateInfos& _stateInfos) const
{
	state.deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
