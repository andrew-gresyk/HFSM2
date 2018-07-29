namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
_CS<TID, TC, TSL, TPL, NI, TI>::_CS(StateRegistry2& stateRegistry,
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
_CS<TID, TC, TSL, TPL, NI, TI>::wideForwardGuard(const ShortIndex HSFM_IF_ASSERT(prong),
												 TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideGuard(const ShortIndex HSFM_IF_ASSERT(prong),
										  TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepGuard(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideEnterInitial(Control& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideEnter(const ShortIndex HSFM_IF_ASSERT(prong),
										  Control& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepEnter(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
bool
_CS<TID, TC, TSL, TPL, NI, TI>::wideUpdate(const ShortIndex HSFM_IF_ASSERT(prong),
										   TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
template <typename TEvent>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideReact(const ShortIndex HSFM_IF_ASSERT(prong),
										  const TEvent& event,
										  TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideExit(const ShortIndex HSFM_IF_ASSERT(prong),
										 Control& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideForwardRequest(const ShortIndex HSFM_IF_ASSERT(prong),
												   const TransitionType transition)
{
	assert(prong == PRONG_INDEX);

	initial.deepForwardRequest(transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideRequestResume(const ShortIndex HSFM_IF_ASSERT(prong)) {
	assert(prong == PRONG_INDEX);

	initial.deepRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideChangeToRequested(const ShortIndex HSFM_IF_ASSERT(prong),
													  Control& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideGetNames(const LongIndex parent,
											 const ShortIndex depth,
											 StructureStateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, ShortIndex NI, typename TI>
void
_CS<TID, TC, TSL, TPL, NI, TI>::wideIsActive(const ShortIndex prong,
											 LongIndex& index,
											 MachineStructure& structure) const
{
	initial.deepIsActive(prong == PRONG_INDEX, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
