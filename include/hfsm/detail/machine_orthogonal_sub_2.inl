namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TPL, ShortIndex NI, typename TI>
_OS<TC, TPL, NI, TI>::_OS(StateRegistryBase& stateRegistry,
						  const ShortIndex fork,
						  const TypeInfo parentInfo,
						  Parents& forkParents,
						  ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork,
					 PRONG_INDEX,
					 parentInfo,
					 TypeInfo::get<typename Initial::Head>()),
			  forkParents,
			  forkPointers)
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideRegister(StateRegistryBase& stateRegistry,
								   const ShortIndex fork,
								   const TypeInfo parentInfo)
{
	initial.deepRegister(stateRegistry,
						 Parent(fork,
								PRONG_INDEX,
								parentInfo,
								TypeInfo::get<typename Initial::Head>()));
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideForwardGuard(const ShortIndex HSFM_IF_ASSERT(prong),
									   TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideForwardGuard(TransitionControl& control) {
	initial.deepForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideGuard(TransitionControl& control) {
	initial.deepGuard(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideEnterInitial(Control& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideEnter(Control& control) {
	initial.deepEnter(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
bool
_OS<TC, TPL, NI, TI>::wideUpdate(TransitionControl& control) {
	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
template <typename TEvent>
void
_OS<TC, TPL, NI, TI>::wideReact(const TEvent& event,
								TransitionControl& control)
{
	initial.deepReact(event, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideExit(Control& control) {
	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideForwardRequest(const ShortIndex prong,
										 const TransitionType transition)
{
	assert(prong <= PRONG_INDEX);

	if (prong == PRONG_INDEX)
		initial.deepForwardRequest(transition);
	else
		initial.deepForwardRequest(Transition::REMAIN);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideRequestResume() {
	initial.deepRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideChangeToRequested(Control& control) {
	initial.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideGetNames(const LongIndex parent,
								   const ShortIndex depth,
								   StructureStateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StructureStateInfo::ORTHOGONAL, depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_OS<TC, TPL, NI, TI>::wideIsActive(const bool isActive,
								   LongIndex& index,
								   MachineStructure& structure) const
{
	initial.deepIsActive(isActive, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
