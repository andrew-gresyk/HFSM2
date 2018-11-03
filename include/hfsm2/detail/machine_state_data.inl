#pragma once

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <LongIndex NCC, LongIndex NOC, LongIndex NOU>
void
AllForksT<NCC, NOC, NOU>::clear() {
	compo.fill(INVALID_SHORT_INDEX);
	ortho.clear();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
const Parent&
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::forkParent(const ForkID forkId) const {
	HFSM_ASSERT(forkId != 0);

	return forkId > 0 ?
		compoParents[ forkId - 1] :
		orthoParents[-forkId - 1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
ForkID
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::parentCompoFork(const ForkID forkId) const {
	HFSM_ASSERT(forkId != 0);

	for (Parent parent = forkParent(forkId);
		 parent;
		 parent = forkParent(parent.forkId))
	{
		if (parent.forkId > 0)
			return parent.forkId;
	}

	return 0;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isActive(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0) {
				const ShortIndex& active = compoActive[parent.forkId - 1];

				return parent.prong == active;
			}
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isResumable(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);
			if (parent.forkId > 0) {
				const ShortIndex& compoResumable = resumable.compo[parent.forkId - 1];

				return parent.prong == compoResumable;
			}
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestImmediate(const Request request) {
	HFSM_ASSERT(request.stateId < STATE_COUNT);

	if (request.stateId < STATE_COUNT)
		for (Parent parent = stateParents[request.stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0) {
				ShortIndex& compoRequested = requested.compo[parent.forkId - 1];

				compoRequested = parent.prong;
			} else if (parent.forkId < 0) {
				const ForkID forkId = -parent.forkId - 1;

				OrthoFork& orthoRequested = requested.ortho[forkId];

				if (const ForkID compoForkId = parentCompoFork(parent.forkId))
					if (requested.compo[compoForkId - 1] == INVALID_SHORT_INDEX)
						orthoRequested.clear();

				orthoRequested[parent.prong] = true;
			}
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestScheduled(const StateID stateId) {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT) {
		const Parent parent = stateParents[stateId];
		HFSM_ASSERT(parent.forkId != 0);

		if (parent.forkId > 0) {
			ShortIndex& compoResumable = resumable.compo[parent.forkId - 1];

			compoResumable = parent.prong;
		} else if (parent.forkId < 0) {
			OrthoFork& orthoResumable = resumable.ortho[-parent.forkId - 1];

			orthoResumable[parent.prong] = true;
		}
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::clearOrthoRequested() {
	for (ForkID i = 0; i < requested.ortho.count(); ++i) {
		OrthoFork& orthoRequested = requested.ortho[i];

		orthoRequested.clear();
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
const Parent&
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::forkParent(const ForkID forkId) const {
	HFSM_ASSERT(forkId > 0);

	return compoParents[forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isActive(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId > 0);

			if (parent.forkId > 0) {
				const ShortIndex& active = compoActive[parent.forkId - 1];

				return parent.prong == active;
			}
		}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isResumable(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0) {
				const ShortIndex& compoResumable = resumable.compo[parent.forkId - 1];

				return parent.prong == compoResumable;
			}
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
void
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::requestImmediate(const Request request) {
	HFSM_ASSERT(request.stateId < STATE_COUNT);

	if (request.stateId < STATE_COUNT)
		for (Parent parent = stateParents[request.stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId > 0);

			if (parent.forkId > 0) {
				ShortIndex& compoRequested = requested.compo[parent.forkId - 1];

				compoRequested = parent.prong;
			}
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
void
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::requestScheduled(const StateID stateId) {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT) {
		const Parent parent = stateParents[stateId];
		HFSM_ASSERT(parent.forkId > 0);

		if (parent.forkId > 0) {
			ShortIndex& compoResumable = resumable.compo[parent.forkId - 1];

			compoResumable = parent.prong;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

}
}
