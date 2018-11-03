namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideRegister(StateData& stateData,
												 const ForkID forkId)
{
	initial  .deepRegister(stateData, Parent{forkId, PRONG_INDEX});
	remaining.wideRegister(stateData, forkId);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardGuard(const ShortIndex prong,
													 GuardControl& control)
{
	if (prong == PRONG_INDEX)
		return initial  .deepForwardGuard(		 control);
	else
		return remaining.wideForwardGuard(prong, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideGuard(const ShortIndex prong,
											  GuardControl& control)
{
	if (prong == PRONG_INDEX)
		return initial  .deepGuard(		  control);
	else
		return remaining.wideGuard(prong, control);
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
	if (prong == PRONG_INDEX)
		initial  .deepReact(	   event, control);
	else
		remaining.wideReact(prong, event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideExit(const ShortIndex prong,
											 Control& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepExit(		  control);
	else
		remaining.wideExit(prong, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardRequest(StateData& stateData,
													   const ShortIndex prong,
													   const RequestType transition)
{
	if (prong == PRONG_INDEX)
		initial	 .deepForwardRequest(stateData, 		  transition);
	else
		remaining.wideForwardRequest(stateData, prong, transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestRemain(StateData& stateData) {
	initial.deepRequestRemain(stateData);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestRestart(StateData& stateData) {
	initial.deepRequestRestart(stateData);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestResume(StateData& stateData,
													  const ShortIndex prong)
{
	if (prong == PRONG_INDEX)
		initial	 .deepRequestResume(stateData);
	else
		remaining.wideRequestResume(stateData, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideChangeToRequested(StateData& stateData,
														  const ShortIndex prong,
														  Control& control)
{
	if (prong == PRONG_INDEX)
		initial	 .deepChangeToRequested(stateData,		 control);
	else
		remaining.wideChangeToRequested(stateData, prong, control);
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
