namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::_CS(StateRegistry& stateRegistry,
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
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideForwardGuard(const ShortIndex prong,
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
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideGuard(const ShortIndex prong,
												 TransitionControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepGuard(control);
	else
		remaining.wideGuard(prong, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideEnterInitial(Control& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideEnter(const ShortIndex prong,
												 Control& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepEnter(control);
	else
		remaining.wideEnter(prong, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
bool
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideUpdate(const ShortIndex prong,
												  TransitionControl& control)
{
	return prong == PRONG_INDEX ?
		initial  .deepUpdate(control) :
		remaining.wideUpdate(prong, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
template <typename TEvent>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideReact(const ShortIndex prong,
												 const TEvent& event,
												 TransitionControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepReact(	   event, control);
	else
		remaining.wideReact(prong, event, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideExit(const ShortIndex prong,
												Control& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepExit(control);
	else
		remaining.wideExit(prong, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideForwardRequest(const ShortIndex prong,
														  const TransitionType transition)
{
	if (prong == PRONG_INDEX)
		initial	 .deepForwardRequest(		transition);
	else
		remaining.wideForwardRequest(prong, transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideRequestResume(const ShortIndex prong) {
	if (prong == PRONG_INDEX)
		initial	 .deepRequestResume();
	else
		remaining.wideRequestResume(prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideChangeToRequested(const ShortIndex prong,
															 Control& control)
{
	if (prong == PRONG_INDEX)
		initial	 .deepChangeToRequested(control);
	else
		remaining.wideChangeToRequested(prong, control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TID, TC, TSL, TPL, NI, TI, TR...>::wideGetNames(const LongIndex parent,
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
