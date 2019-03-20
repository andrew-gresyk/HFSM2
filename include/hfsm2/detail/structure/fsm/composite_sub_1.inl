namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideRegister(StateRegistry& stateRegistry,
												 const ForkID forkId)
{
	initial  .deepRegister(stateRegistry, Parent{forkId, PRONG_INDEX});
	remaining.wideRegister(stateRegistry, forkId);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardEntryGuard(const ShortIndex prong,
														  GuardControl& control)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong == PRONG_INDEX)
		return initial  .deepForwardEntryGuard(		  control);
	else
		return remaining.wideForwardEntryGuard(prong, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideEntryGuard(const ShortIndex prong,
												   GuardControl& control)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong == PRONG_INDEX)
		return initial  .deepEntryGuard(	   control);
	else
		return remaining.wideEntryGuard(prong, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideEnterInitial(Control& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideEnter(const ShortIndex prong,
											  Control& control)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong == PRONG_INDEX)
		initial  .deepEnter(	   control);
	else
		remaining.wideEnter(prong, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
Status
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideUpdate(const ShortIndex prong,
											   FullControl& control)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	return prong == PRONG_INDEX ?
		initial  .deepUpdate(		control) :
		remaining.wideUpdate(prong, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
template <typename TEvent>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideReact(const ShortIndex prong,
											  const TEvent& event,
											  FullControl& control)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong == PRONG_INDEX)
		initial  .deepReact(	   event, control);
	else
		remaining.wideReact(prong, event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardExitGuard(const ShortIndex prong,
														 GuardControl& control)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong == PRONG_INDEX)
		return initial  .deepForwardExitGuard(		 control);
	else
		return remaining.wideForwardExitGuard(prong, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideExitGuard(const ShortIndex prong,
												  GuardControl& control)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong == PRONG_INDEX)
		return initial  .deepExitGuard(		  control);
	else
		return remaining.wideExitGuard(prong, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideExit(const ShortIndex prong,
											 Control& control)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong == PRONG_INDEX)
		initial  .deepExit(		  control);
	else
		remaining.wideExit(prong, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardActive(StateRegistry& stateRegistry,
													  const ShortIndex prong,
													  const RequestType transition)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong == PRONG_INDEX)
		initial	 .deepForwardActive(stateRegistry,		  transition);
	else
		remaining.wideForwardActive(stateRegistry, prong, transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardRequest(StateRegistry& stateRegistry,
													   const ShortIndex prong,
													   const RequestType transition)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong == PRONG_INDEX)
		initial	 .deepForwardRequest(stateRegistry, 	   transition);
	else
		remaining.wideForwardRequest(stateRegistry, prong, transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestRemain(StateRegistry& stateRegistry) {
	initial.deepRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestRestart(StateRegistry& stateRegistry) {
	initial.deepRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestResume(StateRegistry& stateRegistry,
													  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong == PRONG_INDEX)
		initial	 .deepRequestResume(stateRegistry);
	else
		remaining.wideRequestResume(stateRegistry, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideChangeToRequested(StateRegistry& stateRegistry,
														  const ShortIndex prong,
														  Control& control)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong == PRONG_INDEX)
		initial	 .deepChangeToRequested(stateRegistry,		  control);
	else
		remaining.wideChangeToRequested(stateRegistry, prong, control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideGetNames(const LongIndex parent,
												 const ShortIndex depth,
												 StructureStateInfos& _stateInfos) const
{
	initial	 .deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
	remaining.wideGetNames(parent,								  depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
