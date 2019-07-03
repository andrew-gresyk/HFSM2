namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideRegister(StateRegistry& stateRegistry,
									 const Parent parent)
{
	state.deepRegister(stateRegistry, Parent{parent.forkId, PRONG_INDEX});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
bool
CS_<TN, TA, TG, NI, T>::wideForwardEntryGuard(GuardControl& control,
											  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
bool
CS_<TN, TA, TG, NI, T>::wideEntryGuard(GuardControl& control,
									   const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideEnter(PlanControl& control,
								  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideReenter(PlanControl& control,
									const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
Status
CS_<TN, TA, TG, NI, T>::wideUpdate(FullControl& control,
								   const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
template <typename TEvent>
Status
CS_<TN, TA, TG, NI, T>::wideReact(FullControl& control,
								  const TEvent& event,
								  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepReact(control, event);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
bool
CS_<TN, TA, TG, NI, T>::wideForwardExitGuard(GuardControl& control,
											 const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
bool
CS_<TN, TA, TG, NI, T>::wideExitGuard(GuardControl& control,
									  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideExit(PlanControl& control,
								 const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideForwardActive(Control& control,
										  const RequestType request,
										  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepForwardActive(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideForwardRequest(Control& control,
										   const RequestType request,
										   const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepForwardRequest(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideRequestChangeComposite(Control& control) {
	state.deepRequestChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideRequestChangeResumable(Control& control,
												   const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepRequestChange(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideRequestRemain(StateRegistry& stateRegistry) {
	state.deepRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideRequestRestart(StateRegistry& stateRegistry) {
	state.deepRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideRequestResume(StateRegistry& stateRegistry,
										  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepRequestResume(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
typename TA::UP
CS_<TN, TA, TG, NI, T>::wideReportUtilize(Control& control) {
	return state.deepReportUtilize(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
typename TA::Rank
CS_<TN, TA, TG, NI, T>::wideReportRank(Control& control,
									   Rank* const ranks)
{
	HFSM_ASSERT(ranks);

	*ranks = state.deepReportRank(control);

	return *ranks;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
typename TA::Utility
CS_<TN, TA, TG, NI, T>::wideReportRandomize(Control& control,
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

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
typename TA::UP
CS_<TN, TA, TG, NI, T>::wideReportChangeComposite(Control& control) {
	return state.deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
typename TA::UP
CS_<TN, TA, TG, NI, T>::wideReportChangeResumable(Control& control,
												  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
typename TA::UP
CS_<TN, TA, TG, NI, T>::wideReportChangeUtilitarian(Control& control) {
	return state.deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
typename TA::Utility
CS_<TN, TA, TG, NI, T>::wideReportChangeRandom(Control& control,
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

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideEnterRequested(PlanControl& control,
										   const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepEnterRequested(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideChangeToRequested(PlanControl& control,
											  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideGetNames(const LongIndex parent,
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
