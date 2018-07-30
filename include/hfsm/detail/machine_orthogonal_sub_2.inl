namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
_OS<TID, TC, TSL, TPL, NI, TI>::_OS(StateRegistry& stateRegistry,
									const ShortIndex fork,
									Parents& forkParents,
									ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork, PRONG_INDEX),
			  forkParents,
			  forkPointers)
{}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideForwardGuard(const ShortIndex HSFM_IF_ASSERT(prong),
												 TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideForwardGuard(TransitionControl& control) {
	initial.deepForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideGuard(TransitionControl& control) {
	initial.deepGuard(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideEnterInitial(Control& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideEnter(Control& control) {
	initial.deepEnter(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
bool
_OS<TID, TC, TSL, TPL, NI, TI>::wideUpdate(TransitionControl& control) {
	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
template <typename TEvent>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideReact(const TEvent& event,
										  TransitionControl& control)
{
	initial.deepReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideExit(Control& control) {
	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideForwardRequest(const ShortIndex prong,
												   const TransitionType transition)
{
	assert(prong <= PRONG_INDEX);

	if (prong == PRONG_INDEX)
		initial.deepForwardRequest(transition);
	else
		initial.deepForwardRequest(Transition::REMAIN);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideRequestResume() {
	initial.deepRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideChangeToRequested(Control& control) {
	initial.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_OS<TID, TC, TSL, TPL, NI, TI>::wideGetNames(const LongIndex parent,
											 const ShortIndex depth,
											 StructureStateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StructureStateInfo::ORTHOGONAL, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
