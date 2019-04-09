namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepRegister(StateRegistry& stateRegistry,
											 const Parent parent)
{
	lHalf.deepRegister(stateRegistry, Parent{parent.forkId, L_PRONG});
	rHalf.deepRegister(stateRegistry, Parent{parent.forkId, R_PRONG});
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
bool
_CS<NS, NC, NO, TA, NI, TS...>::deepForwardEntryGuard(GuardControl& control,
													  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.deepForwardEntryGuard(control, prong);
	else
		return rHalf.deepForwardEntryGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
bool
_CS<NS, NC, NO, TA, NI, TS...>::deepEntryGuard(GuardControl& control,
											   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.deepEntryGuard(control, prong);
	else
		return rHalf.deepEntryGuard(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepEnterInitial(Control& control) {
	lHalf.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepEnter(Control& control,
										  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepEnter(control, prong);
	else
		rHalf.deepEnter(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
Status
_CS<NS, NC, NO, TA, NI, TS...>::deepUpdate(FullControl& control,
										   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	return prong < R_PRONG ?
		lHalf.deepUpdate(control, prong) :
		rHalf.deepUpdate(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
template <typename TEvent>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepReact(FullControl& control,
										  const TEvent& event,
										  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepReact(control, event, prong);
	else
		rHalf.deepReact(control, event, prong);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
bool
_CS<NS, NC, NO, TA, NI, TS...>::deepForwardExitGuard(GuardControl& control,
													 const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.deepForwardExitGuard(control, prong);
	else
		return rHalf.deepForwardExitGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
bool
_CS<NS, NC, NO, TA, NI, TS...>::deepExitGuard(GuardControl& control,
											  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.deepExitGuard(control, prong);
	else
		return rHalf.deepExitGuard(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepExit(Control& control,
										 const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepExit(control, prong);
	else
		rHalf.deepExit(control, prong);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepForwardActive(StateRegistry& stateRegistry,
												  const RequestType request,
												  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepForwardActive(stateRegistry, request, prong);
	else
		rHalf.deepForwardActive(stateRegistry, request, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepForwardRequest(StateRegistry& stateRegistry,
												   const RequestType request,
												   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepForwardRequest(stateRegistry, request, prong);
	else
		rHalf.deepForwardRequest(stateRegistry, request, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepRequestRemain(StateRegistry& stateRegistry) {
	lHalf.deepRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepRequestRestart(StateRegistry& stateRegistry) {
	lHalf.deepRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepRequestResume(StateRegistry& stateRegistry,
												  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepRequestResume(stateRegistry, prong);
	else
		rHalf.deepRequestResume(stateRegistry, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepChangeToRequested(StateRegistry& stateRegistry,
													  Control& control,
													  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.deepChangeToRequested(stateRegistry, control, prong);
	else
		rHalf.deepChangeToRequested(stateRegistry, control, prong);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename... TS>
void
_CS<NS, NC, NO, TA, NI, TS...>::deepGetNames(const LongIndex parent,
											 const RegionType /*region*/,
											 const ShortIndex depth,
											 StructureStateInfos& _stateInfos) const
{
	lHalf.deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
	rHalf.deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
