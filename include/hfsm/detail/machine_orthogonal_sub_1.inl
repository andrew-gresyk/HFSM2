namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
_OS<TC, TPL, NI, TI, TR...>::_OS(StateRegistryBase& stateRegistry,
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
	, remaining(stateRegistry,
				fork,
				parentInfo,
				forkParents,
				forkPointers)
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideRegister(StateRegistryBase& stateRegistry,
										  const ShortIndex fork,
										  const TypeInfo parentInfo)
{
	initial	 .deepRegister(stateRegistry,
						   Parent(fork,
								  PRONG_INDEX,
								  parentInfo,
								  TypeInfo::get<typename Initial::Head>()));
	remaining.wideRegister(stateRegistry,
						   fork,
						   parentInfo);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideForwardGuard(const ShortIndex prong,
											  TransitionControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepForwardGuard(control);
	else
		remaining.wideForwardGuard(prong, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideForwardGuard(TransitionControl& control) {
	initial	 .deepForwardGuard(control);
	remaining.wideForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideGuard(TransitionControl& control) {
	initial	 .deepGuard(control);
	remaining.wideGuard(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideEnterInitial(Control& control) {
	initial  .deepEnterInitial(control);
	remaining.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideEnter(Control& control) {
	initial  .deepEnter(control);
	remaining.wideEnter(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
bool
_OS<TC, TPL, NI, TI, TR...>::wideUpdate(TransitionControl& control) {
	const bool r = initial.deepUpdate(control);

	return		 remaining.wideUpdate(control) || r;
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
template <typename TEvent>
void
_OS<TC, TPL, NI, TI, TR...>::wideReact(const TEvent& event,
									   TransitionControl& control)
{
	initial  .deepReact(event, control);
	remaining.wideReact(event, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideExit(Control& control) {
	initial	 .deepExit(control);
	remaining.wideExit(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideForwardRequest(const ShortIndex prong,
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

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideRequestRemain() {
	initial	 .deepRequestRemain();
	remaining.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideRequestRestart() {
	initial	 .deepRequestRestart();
	remaining.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideRequestResume() {
	initial	 .deepRequestResume();
	remaining.wideRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideChangeToRequested(Control& control) {
	initial	 .deepChangeToRequested(control);
	remaining.wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideGetNames(const LongIndex parent,
										  const ShortIndex depth,
										  StructureStateInfos& _stateInfos) const
{
	initial	 .deepGetNames(parent, StructureStateInfo::ORTHOGONAL, depth, _stateInfos);
	remaining.wideGetNames(parent,								   depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_OS<TC, TPL, NI, TI, TR...>::wideIsActive(const bool isActive,
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
