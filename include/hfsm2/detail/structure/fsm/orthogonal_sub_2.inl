namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideRegister(StateRegistry& stateRegistry,
										  const ForkID forkId)
{
	initial.deepRegister(stateRegistry, Parent{forkId, PRONG_INDEX});
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
bool
_OS<NS, NC, NO, TA, NI, TI>::wideForwardEntryGuard(const OrthoFork& prongs,
												   GuardControl& control)
{
	return prongs[PRONG_INDEX] ?
		initial.deepForwardEntryGuard(control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
bool
_OS<NS, NC, NO, TA, NI, TI>::wideForwardEntryGuard(GuardControl& control) {
	return initial.deepForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
bool
_OS<NS, NC, NO, TA, NI, TI>::wideEntryGuard(GuardControl& control) {
	return initial.deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideEnterInitial(Control& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideEnter(Control& control) {
	initial.deepEnter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
Status
_OS<NS, NC, NO, TA, NI, TI>::wideUpdate(FullControl& control) {
	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
template <typename TEvent>
void
_OS<NS, NC, NO, TA, NI, TI>::wideReact(const TEvent& event,
									   FullControl& control)
{
	initial.deepReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
bool
_OS<NS, NC, NO, TA, NI, TI>::wideForwardExitGuard(const OrthoFork& prongs,
												  GuardControl& control)
{
	return prongs[PRONG_INDEX] ?
		initial.deepForwardExitGuard(control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
bool
_OS<NS, NC, NO, TA, NI, TI>::wideForwardExitGuard(GuardControl& control) {
	return initial.deepForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
bool
_OS<NS, NC, NO, TA, NI, TI>::wideExitGuard(GuardControl& control) {
	return initial.deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideExit(Control& control) {
	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideForwardActive(StateRegistry& stateRegistry,
											   const OrthoFork& prongs,
											   const RequestType request)
{
	initial.deepForwardActive(stateRegistry, prongs[PRONG_INDEX] ? request : Request::REMAIN);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideForwardRequest(StateRegistry& stateRegistry,
												const OrthoFork& prongs,
												const RequestType request)
{
	initial.deepForwardRequest(stateRegistry, prongs[PRONG_INDEX] ? request : Request::REMAIN);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideRequestRemain(StateRegistry& stateRegistry) {
	initial.deepRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideRequestRestart(StateRegistry& stateRegistry) {
	initial.deepRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideRequestResume(StateRegistry& stateRegistry) {
	initial.deepRequestResume(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideChangeToRequested(StateRegistry& stateRegistry,
												   Control& control)
{
	initial.deepChangeToRequested(stateRegistry, control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_OS<NS, NC, NO, TA, NI, TI>::wideGetNames(const LongIndex parent,
										  const ShortIndex depth,
										  StructureStateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StructureStateInfo::ORTHOGONAL, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
