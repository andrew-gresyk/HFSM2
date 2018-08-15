namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TID, typename TA, ShortIndex NI, typename TI>
_OS<TID, TA, NI, TI>::_OS(StateRegistry& stateRegistry,
						  const ShortIndex fork,
						  Parents& forkParents,
						  ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork, PRONG_INDEX),
			  forkParents,
			  forkPointers)
{}

//------------------------------------------------------------------------------

template <StateID TID, typename TA, ShortIndex NI, typename TI>
void
_OS<TID, TA, NI, TI>::wideForwardGuard(const ShortIndex HSFM_IF_ASSERT(prong),
									   FullControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TA, ShortIndex NI, typename TI>
void
_OS<TID, TA, NI, TI>::wideForwardGuard(FullControl& control) {
	initial.deepForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TA, ShortIndex NI, typename TI>
void
_OS<TID, TA, NI, TI>::wideGuard(FullControl& control) {
	initial.deepGuard(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TA, ShortIndex NI, typename TI>
void
_OS<TID, TA, NI, TI>::wideEnterInitial(PlanControl& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TA, ShortIndex NI, typename TI>
void
_OS<TID, TA, NI, TI>::wideEnter(PlanControl& control) {
	initial.deepEnter(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TA, ShortIndex NI, typename TI>
Status
_OS<TID, TA, NI, TI>::wideUpdate(FullControl& control) {
	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TA, ShortIndex NI, typename TI>
template <typename TEvent>
void
_OS<TID, TA, NI, TI>::wideReact(const TEvent& event,
								FullControl& control)
{
	initial.deepReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TA, ShortIndex NI, typename TI>
void
_OS<TID, TA, NI, TI>::wideExit(PlanControl& control) {
	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TA, ShortIndex NI, typename TI>
void
_OS<TID, TA, NI, TI>::wideForwardRequest(const ShortIndex prong,
										 const TransitionType transition)
{
	assert(prong <= PRONG_INDEX);

	if (prong == PRONG_INDEX)
		initial.deepForwardRequest(transition);
	else
		initial.deepForwardRequest(Transition::REMAIN);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TA, ShortIndex NI, typename TI>
void
_OS<TID, TA, NI, TI>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TA, ShortIndex NI, typename TI>
void
_OS<TID, TA, NI, TI>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TA, ShortIndex NI, typename TI>
void
_OS<TID, TA, NI, TI>::wideRequestResume() {
	initial.deepRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TA, ShortIndex NI, typename TI>
void
_OS<TID, TA, NI, TI>::wideChangeToRequested(PlanControl& control) {
	initial.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID TID, typename TA, ShortIndex NI, typename TI>
void
_OS<TID, TA, NI, TI>::wideGetNames(const LongIndex parent,
								   const ShortIndex depth,
								   StructureStateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StructureStateInfo::ORTHOGONAL, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
