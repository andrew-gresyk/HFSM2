namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TPL, ShortIndex NI, typename TI>
_CS<TC, TPL, NI, TI>::_CS(StateRegistryBase& stateRegistry,
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
_CS<TC, TPL, NI, TI>::wideRegister(StateRegistryBase& stateRegistry,
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
_CS<TC, TPL, NI, TI>::wideForwardGuard(const ShortIndex HSFM_IF_ASSERT(prong),
									   TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideGuard(const ShortIndex HSFM_IF_ASSERT(prong),
								TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepGuard(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideEnterInitial(Control& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideEnter(const ShortIndex HSFM_IF_ASSERT(prong),
								Control& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepEnter(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
bool
_CS<TC, TPL, NI, TI>::wideUpdate(const ShortIndex HSFM_IF_ASSERT(prong),
								 TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
template <typename TEvent>
void
_CS<TC, TPL, NI, TI>::wideReact(const ShortIndex HSFM_IF_ASSERT(prong),
								const TEvent& event,
								TransitionControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepReact(event, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideExit(const ShortIndex HSFM_IF_ASSERT(prong),
							   Control& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideForwardRequest(const ShortIndex HSFM_IF_ASSERT(prong),
										 const TransitionType transition)
{
	assert(prong == PRONG_INDEX);

	initial.deepForwardRequest(transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideRequestResume(const ShortIndex HSFM_IF_ASSERT(prong)) {
	assert(prong == PRONG_INDEX);

	initial.deepRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideChangeToRequested(const ShortIndex HSFM_IF_ASSERT(prong),
											Control& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideGetNames(const LongIndex parent,
								   const ShortIndex depth,
								   StructureStateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex NI, typename TI>
void
_CS<TC, TPL, NI, TI>::wideIsActive(const ShortIndex prong,
								   LongIndex& index,
								   MachineStructure& structure) const
{
	initial.deepIsActive(prong == PRONG_INDEX, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
