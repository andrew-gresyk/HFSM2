namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::_OS(StateRegistry2& stateRegistry,
										   const ShortIndex fork,
										   Parents& forkParents,
										   ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork, PRONG_INDEX),
			  forkParents,
			  forkPointers)
	, remaining(stateRegistry,
				fork,
				forkParents,
				forkPointers)
{}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideForwardGuard(const ShortIndex prong,
														TransitionControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepForwardGuard(control);
	else
		remaining.wideForwardGuard(prong, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideForwardGuard(TransitionControl& control) {
	initial	 .deepForwardGuard(control);
	remaining.wideForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideGuard(TransitionControl& control) {
	initial	 .deepGuard(control);
	remaining.wideGuard(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideEnterInitial(Control& control) {
	initial  .deepEnterInitial(control);
	remaining.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideEnter(Control& control) {
	initial  .deepEnter(control);
	remaining.wideEnter(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
bool
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideUpdate(TransitionControl& control) {
	const bool r = initial.deepUpdate(control);

	return		 remaining.wideUpdate(control) || r;
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
template <typename TEvent>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideReact(const TEvent& event,
												 TransitionControl& control)
{
	initial  .deepReact(event, control);
	remaining.wideReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideExit(Control& control) {
	initial	 .deepExit(control);
	remaining.wideExit(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideForwardRequest(const ShortIndex prong,
														  const TransitionType transition)
{
	if (prong == PRONG_INDEX) {
		initial	 .deepForwardRequest(		transition);
		remaining.wideForwardRequest(prong, Transition::REMAIN);
	} else {
		initial	 .deepForwardRequest(		Transition::REMAIN);
		remaining.wideForwardRequest(prong, transition);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideRequestRemain() {
	initial	 .deepRequestRemain();
	remaining.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideRequestRestart() {
	initial	 .deepRequestRestart();
	remaining.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideRequestResume() {
	initial	 .deepRequestResume();
	remaining.wideRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideChangeToRequested(Control& control) {
	initial	 .deepChangeToRequested(control);
	remaining.wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideGetNames(const LongIndex parent,
													const ShortIndex depth,
													StructureStateInfos& _stateInfos) const
{
	initial	 .deepGetNames(parent, StructureStateInfo::ORTHOGONAL, depth, _stateInfos);
	remaining.wideGetNames(parent,								   depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TID, TC, TSL, TPL, NI, TI, TR...>::wideIsActive(const bool isActive,
													LongIndex& index,
													MachineStructure& structure) const
{
	initial	 .deepIsActive(isActive, index, structure);
	remaining.wideIsActive(isActive, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
