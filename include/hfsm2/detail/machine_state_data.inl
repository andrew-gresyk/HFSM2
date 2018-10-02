#pragma once

namespace hfsm2 {
namespace detail {

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
const CompoFork*
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::compoParent(const ForkID forkId) const {
	HFSM_ASSERT(forkId != 0);

	for (Parent parent = forkParent(forkId);
		 parent;
		 parent = forkParent(parent.forkId))
	{
		if (parent.forkId > 0)
			return &compoForks[parent.forkId - 1];
	}

	return nullptr;
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
				const CompoFork& fork = compoForks[parent.forkId - 1];

				return parent.prong == fork.active;
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
				const CompoFork& fork = compoForks[parent.forkId - 1];

				return parent.prong == fork.resumable;
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
				CompoFork& fork = compoForks[parent.forkId - 1];

				fork.requested = parent.prong;
			} else if (parent.forkId < 0) {
				const ForkID forkId = -parent.forkId - 1;

				OrthoFork& requested = orthoRequested[forkId];

				if (const CompoFork* const compoFork = compoParent(parent.forkId))
					if (compoFork->requested == INVALID_SHORT_INDEX)
						requested.prongs.clear();

				requested.prongs[parent.prong] = true;
			}
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestScheduled(const Request request) {
	HFSM_ASSERT(request.stateId < STATE_COUNT);

	if (request.stateId < STATE_COUNT) {
		const Parent parent = stateParents[request.stateId];
		HFSM_ASSERT(parent.forkId != 0);

		if (parent.forkId > 0) {
			CompoFork& fork = compoForks[parent.forkId - 1];

			fork.resumable = parent.prong;
		} else if (parent.forkId < 0) {
			OrthoFork& resumable = orthoResumable[-parent.forkId - 1];

			resumable.prongs[parent.prong] = true;
		}
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::clearOrthoRequested() {
	for (ForkID i = 0; i < orthoRequested.count(); ++i) {
		OrthoFork& requested = orthoRequested[i];
		requested.prongs.clear();
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
				const CompoFork& fork = compoForks[parent.forkId - 1];

				return parent.prong == fork.active;
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
				const CompoFork& fork = compoForks[parent.forkId - 1];

				return parent.prong == fork.resumable;
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
				CompoFork& fork = compoForks[parent.forkId - 1];

				fork.requested = parent.prong;
			}
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
void
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::requestScheduled(const Request request) {
	HFSM_ASSERT(request.stateId < STATE_COUNT);

	if (request.stateId < STATE_COUNT) {
		const Parent parent = stateParents[request.stateId];
		HFSM_ASSERT(parent.forkId > 0);

		if (parent.forkId > 0) {
			CompoFork& fork = compoForks[parent.forkId - 1];

			fork.resumable = parent.prong;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
const Parent&
StateDataT<ArgsT<TC, TG, TSL, TRL, 0, NOC, NOU, TPL, NTC>>::forkParent(const ForkID forkId) const {
	HFSM_ASSERT(forkId < 0);

	return orthoParents[-forkId - 1];
}

////////////////////////////////////////////////////////////////////////////////

}
}
