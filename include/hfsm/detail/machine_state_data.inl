#pragma once

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
const Parent&
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::forkParent(const ForkID forkId) const {
	assert(forkId != 0);

	return forkId > 0 ?
		compoParents[ forkId - 1] :
		orthoParents[-forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isActive(const StateID stateId) const {
	assert(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (auto parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			assert(parent.forkId != 0);

			if (parent.forkId > 0) {
				const CompoFork& forkId = compoForks[ parent.forkId - 1];

				return parent.prong == forkId.active;
			}
		}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isResumable(const StateID stateId) const {
	assert(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (auto parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			assert(parent.forkId != 0);
			if (parent.forkId > 0) {
				const CompoFork& forkId = compoForks[ parent.forkId - 1];

				return parent.prong == forkId.resumable;
			}
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestImmediate(const Request request) {
	assert(request.stateId < STATE_COUNT);

	if (request.stateId < STATE_COUNT)
		for (auto parent = stateParents[request.stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			assert(parent.forkId != 0);

			if (parent.forkId > 0) {
				CompoFork& forkId = compoForks[ parent.forkId - 1];

				forkId.requested = parent.prong;
			} else if (parent.forkId < 0) {
				OrthoFork& requested = orthoRequested[-parent.forkId - 1];

				requested.prongs[parent.prong] = true;
			}
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestScheduled(const Request request) {
	assert(request.stateId < STATE_COUNT);

	if (request.stateId < STATE_COUNT) {
		const auto parent = stateParents[request.stateId];
		assert(parent.forkId != 0);

		if (parent.forkId > 0) {
			CompoFork& forkId = compoForks[ parent.forkId - 1];

			forkId.resumable = parent.prong;
		} else if (parent.forkId < 0) {
			OrthoFork& resumable = orthoResumable[-parent.forkId - 1];

			resumable.prongs[parent.prong] = true;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
const Parent&
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::forkParent(const ForkID forkId) const {
	assert(forkId > 0);

	return compoParents[forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isActive(const StateID stateId) const {
	assert(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (auto parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			assert(parent.forkId > 0);

			if (parent.forkId > 0) {
				const CompoFork& forkId = compoForks[ parent.forkId - 1];

				return parent.prong == forkId.active;
			}
		}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isResumable(const StateID stateId) const {
	assert(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (auto parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			assert(parent.forkId != 0);

			if (parent.forkId > 0) {
				const CompoFork& forkId = compoForks[ parent.forkId - 1];

				return parent.prong == forkId.resumable;
			}
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
void
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::requestImmediate(const Request request) {
	assert(request.stateId < STATE_COUNT);

	if (request.stateId < STATE_COUNT)
		for (auto parent = stateParents[request.stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			assert(parent.forkId > 0);

			if (parent.forkId > 0) {
				CompoFork& fork = compoForks[ parent.forkId - 1];

				fork.requested = parent.prong;
			}
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
void
StateDataT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::requestScheduled(const Request request) {
	assert(request.stateId < STATE_COUNT);

	if (request.stateId < STATE_COUNT) {
		const auto parent = stateParents[request.stateId];
		assert(parent.forkId > 0);

		if (parent.forkId > 0) {
			CompoFork& fork = compoForks[ parent.forkId - 1];

			fork.resumable = parent.prong;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
const Parent&
StateDataT<ArgsT<TC, TG, TSL, TRL, 0, NOC, NOU, TPL, NTC>>::forkParent(const ForkID forkId) const {
	assert(forkId < 0);

	return orthoParents[-forkId - 1];
}

////////////////////////////////////////////////////////////////////////////////

}
}
