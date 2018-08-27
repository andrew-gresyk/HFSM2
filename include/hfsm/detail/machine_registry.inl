#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NPC>
const Parent&
RegistryT<ArgsT<TC, TG, TSL, NCC, NOC, NOU, TPL, NPC>>::forkParent(const ForkID fork) const {
	assert(fork != 0);

	return fork > 0 ?
		compoParents[ fork - 1] :
		orthoParents[-fork - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NPC>
bool
RegistryT<ArgsT<TC, TG, TSL, NCC, NOC, NOU, TPL, NPC>>::isActive(const StateID stateId) const {
	assert(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (auto parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.fork))
		{
			assert(parent.fork != 0);

			if (parent.fork > 0) {
				const CompoFork& fork = compoForks[ parent.fork - 1];

				return parent.prong == fork.active;
			}
		}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NPC>
bool
RegistryT<ArgsT<TC, TG, TSL, NCC, NOC, NOU, TPL, NPC>>::isResumable(const StateID stateId) const {
	assert(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (auto parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.fork))
		{
			assert(parent.fork != 0);
			if (parent.fork > 0) {
				const CompoFork& fork = compoForks[ parent.fork - 1];

				return parent.prong == fork.resumable;
			}
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NPC>
void
RegistryT<ArgsT<TC, TG, TSL, NCC, NOC, NOU, TPL, NPC>>::requestImmediate(const Request request) {
	assert(request.stateId < STATE_COUNT);

	if (request.stateId < STATE_COUNT)
		for (auto parent = stateParents[request.stateId];
			 parent;
			 parent = forkParent(parent.fork))
		{
			assert(parent.fork != 0);

			if (parent.fork > 0) {
				CompoFork& fork = compoForks[ parent.fork - 1];

				fork.requested = parent.prong;
			} else if (parent.fork < 0) {
				OrthoFork& requested = orthoRequested[-parent.fork - 1];

				requested.prongs[parent.prong] = true;
			}
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NPC>
void
RegistryT<ArgsT<TC, TG, TSL, NCC, NOC, NOU, TPL, NPC>>::requestScheduled(const Request request) {
	assert(request.stateId < STATE_COUNT);

	if (request.stateId < STATE_COUNT) {
		const auto parent = stateParents[request.stateId];
		assert(parent.fork != 0);

		if (parent.fork > 0) {
			CompoFork& fork = compoForks[ parent.fork - 1];

			fork.resumable = parent.prong;
		} else if (parent.fork < 0) {
			OrthoFork& resumable = orthoResumable[-parent.fork - 1];

			resumable.prongs[parent.prong] = true;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, LongIndex NCC, typename TPL, LongIndex NPC>
const Parent&
RegistryT<ArgsT<TC, TG, TSL, NCC, 0, 0, TPL, NPC>>::forkParent(const ForkID fork) const {
	assert(fork > 0);

	return compoParents[fork - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, LongIndex NCC, typename TPL, LongIndex NPC>
bool
RegistryT<ArgsT<TC, TG, TSL, NCC, 0, 0, TPL, NPC>>::isActive(const StateID stateId) const {
	assert(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (auto parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.fork))
		{
			assert(parent.fork > 0);

			if (parent.fork > 0) {
				const CompoFork& fork = compoForks[ parent.fork - 1];

				return parent.prong == fork.active;
			}
		}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, LongIndex NCC, typename TPL, LongIndex NPC>
bool
RegistryT<ArgsT<TC, TG, TSL, NCC, 0, 0, TPL, NPC>>::isResumable(const StateID stateId) const {
	assert(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (auto parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.fork))
		{
			assert(parent.fork != 0);

			if (parent.fork > 0) {
				const CompoFork& fork = compoForks[ parent.fork - 1];

				return parent.prong == fork.resumable;
			}
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, LongIndex NCC, typename TPL, LongIndex NPC>
void
RegistryT<ArgsT<TC, TG, TSL, NCC, 0, 0, TPL, NPC>>::requestImmediate(const Request request) {
	assert(request.stateId < STATE_COUNT);

	if (request.stateId < STATE_COUNT)
		for (auto parent = stateParents[request.stateId];
			 parent;
			 parent = forkParent(parent.fork))
		{
			assert(parent.fork > 0);

			if (parent.fork > 0) {
				CompoFork& fork = compoForks[ parent.fork - 1];

				fork.requested = parent.prong;
			}
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, LongIndex NCC, typename TPL, LongIndex NPC>
void
RegistryT<ArgsT<TC, TG, TSL, NCC, 0, 0, TPL, NPC>>::requestScheduled(const Request request) {
	assert(request.stateId < STATE_COUNT);

	if (request.stateId < STATE_COUNT) {
		const auto parent = stateParents[request.stateId];
		assert(parent.fork > 0);

		if (parent.fork > 0) {
			CompoFork& fork = compoForks[ parent.fork - 1];

			fork.resumable = parent.prong;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NPC>
const Parent&
RegistryT<ArgsT<TC, TG, TSL, 0, NOC, NOU, TPL, NPC>>::forkParent(const ForkID fork) const {
	assert(fork < 0);

	return orthoParents[-fork - 1];
}

////////////////////////////////////////////////////////////////////////////////

}
}
