namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
_CS<TC, TPL, NI, TI, TR...>::_CS(StateRegistryBase& stateRegistry,
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
_CS<TC, TPL, NI, TI, TR...>::wideRegister(StateRegistryBase& stateRegistry,
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
_CS<TC, TPL, NI, TI, TR...>::wideForwardGuard(const ShortIndex prong,
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
_CS<TC, TPL, NI, TI, TR...>::wideGuard(const ShortIndex prong,
									   TransitionControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepGuard(control);
	else
		remaining.wideGuard(prong, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideEnterInitial(Control& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideEnter(const ShortIndex prong,
									   Control& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepEnter(control);
	else
		remaining.wideEnter(prong, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
bool
_CS<TC, TPL, NI, TI, TR...>::wideUpdate(const ShortIndex prong,
										TransitionControl& control)
{
	return prong == PRONG_INDEX ?
		initial  .deepUpdate(control) :
		remaining.wideUpdate(prong, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
template <typename TEvent>
void
_CS<TC, TPL, NI, TI, TR...>::wideReact(const ShortIndex prong,
									   const TEvent& event,
									   TransitionControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepReact(	   event, control);
	else
		remaining.wideReact(prong, event, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideExit(const ShortIndex prong,
									  Control& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepExit(control);
	else
		remaining.wideExit(prong, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideForwardRequest(const ShortIndex prong,
												const TransitionType transition)
{
	if (prong == PRONG_INDEX)
		initial	 .deepForwardRequest(		transition);
	else
		remaining.wideForwardRequest(prong, transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideRequestResume(const ShortIndex prong) {
	if (prong == PRONG_INDEX)
		initial	 .deepRequestResume();
	else
		remaining.wideRequestResume(prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideChangeToRequested(const ShortIndex prong,
												   Control& control)
{
	if (prong == PRONG_INDEX)
		initial	 .deepChangeToRequested(control);
	else
		remaining.wideChangeToRequested(prong, control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideGetNames(const LongIndex parent,
										  const ShortIndex depth,
										  StructureStateInfos& _stateInfos) const
{
	initial	 .deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
	remaining.wideGetNames(parent,								  depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI, typename... TR>
void
_CS<TC, TPL, NI, TI, TR...>::wideIsActive(const ShortIndex prong,
										  LongIndex& index,
										  MachineStructure& structure) const
{
	initial	 .deepIsActive(prong == PRONG_INDEX, index, structure);
	remaining.wideIsActive(prong,				 index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
