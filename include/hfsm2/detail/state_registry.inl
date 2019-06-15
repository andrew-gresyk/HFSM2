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
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isActive(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong == compoActive[parent.forkId - 1];
	}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isResumable(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong == resumable.compo[parent.forkId - 1];
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isPendingChange(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return requested.compo[parent.forkId - 1] !=
						   compoActive[parent.forkId - 1];
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isPendingEnter(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong !=	   compoActive[parent.forkId - 1] &&
					   parent.prong == requested.compo[parent.forkId - 1];
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isPendingExit(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong ==	   compoActive[parent.forkId - 1] &&
					   parent.prong != requested.compo[parent.forkId - 1];
		}

	return true;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
const Parent&
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::forkParent(const ForkID forkId) const {
	HFSM_ASSERT(forkId != 0);

	return forkId > 0 ?
		compoParents[ forkId - 1] :
		orthoParents[-forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
typename StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::OrthoBits
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::resumableOrthoFork(const ForkID forkId) {
	HFSM_ASSERT(forkId < 0);
	const Units& units = orthoUnits[-forkId - 1];

	return resumable.ortho.bits(units);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
typename StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::OrthoBits
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestedOrthoFork(const ForkID forkId) {
	HFSM_ASSERT(forkId < 0);
	const Units& units = orthoUnits[-forkId - 1];

	return requested.ortho.bits(units);
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestImmediate(const Request request) {
	if (request.stateId == 0)
		return false;
	else if (HFSM_CHECKED(request.stateId < STATE_COUNT)) {
		Parent parent;

		for (parent = stateParents[request.stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			if (parent.forkId > 0) {
				requested.compo[parent.forkId - 1] = parent.prong;
				parent = forkParent(parent.forkId);

				break;
			} else if (parent.forkId < 0)
				requestedOrthoFork(parent.forkId).set(parent.prong);
			else
				HFSM_BREAK();
		}

		for (; parent; parent = forkParent(parent.forkId)) {
			if (parent.forkId > 0) {
				compoRemains.set(parent.forkId - 1);

				if (	compoActive[parent.forkId - 1] != parent.prong)
					requested.compo[parent.forkId - 1]  = parent.prong;
				else {
					parent = forkParent(parent.forkId);
					break;
				}
			} else if (parent.forkId < 0)
				requestedOrthoFork(parent.forkId).set(parent.prong);
			else
				HFSM_BREAK();
		}

		for (; parent; parent = forkParent(parent.forkId)) {
			if (parent.forkId > 0)
				compoRemains.set(parent.forkId - 1);
			else if (parent.forkId < 0)
				requestedOrthoFork(parent.forkId).set(parent.prong);
			else
				HFSM_BREAK();
		}
	}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestScheduled(const StateID stateId) {
	if (HFSM_CHECKED(stateId < STATE_COUNT)) {
		const Parent parent = stateParents[stateId];

		if (parent.forkId > 0)
			resumable.compo[parent.forkId - 1] = parent.prong;
		else if (parent.forkId < 0)
			resumableOrthoFork(parent.forkId).set(parent.prong);
		else
			HFSM_BREAK();
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::clearRequests() {
	compoRemains.clear();
	requested.clear();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isActive(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT)) {
		if (Parent parent = stateParents[stateId]) {
			HFSM_ASSERT(parent.forkId > 0);

			return parent.prong == compoActive[parent.forkId - 1];
		} else
			return true;
	}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isResumable(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM_ASSERT(parent.forkId > 0);

			return parent.prong == resumable.compo[parent.forkId - 1];
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isPendingChange(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM_ASSERT(parent.forkId > 0);

			return requested.compo[parent.forkId - 1] !=
					   compoActive[parent.forkId - 1];
		}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isPendingEnter(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM_ASSERT(parent.forkId > 0);

			return parent.prong !=	   compoActive[parent.forkId - 1] &&
				   parent.prong == requested.compo[parent.forkId - 1];
		}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isPendingExit(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM_ASSERT(parent.forkId > 0);

			return parent.prong ==	   compoActive[parent.forkId - 1] &&
				   parent.prong != requested.compo[parent.forkId - 1];
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
const Parent&
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::forkParent(const ForkID forkId) const {
	HFSM_ASSERT(forkId > 0);

	return compoParents[forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::requestImmediate(const Request request) {
	if (request.stateId == 0)
		return false;
	else if (HFSM_CHECKED(request.stateId < STATE_COUNT)) {
		Parent parent = stateParents[request.stateId];

		if (HFSM_CHECKED(parent)) {
			HFSM_ASSERT(parent.forkId > 0);

			requested.compo[parent.forkId - 1] = parent.prong;

			for (parent = forkParent(parent.forkId);
				 parent;
				 parent = forkParent(parent.forkId))
			{
				HFSM_ASSERT(parent.forkId > 0);
				compoRemains.set(parent.forkId - 1);

				if (	compoActive[parent.forkId - 1] != parent.prong)
					requested.compo[parent.forkId - 1]  = parent.prong;
				else {
					parent = forkParent(parent.forkId);
					break;
				}
			}

			for (; parent; parent = forkParent(parent.forkId)) {
				HFSM_ASSERT(parent.forkId > 0);
				compoRemains.set(parent.forkId - 1);
			}
		}
	}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
void
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::requestScheduled(const StateID stateId) {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT) {
		const Parent parent = stateParents[stateId];

		if (HFSM_CHECKED(parent.forkId > 0))
			resumable.compo[parent.forkId - 1] = parent.prong;
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
void
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::clearRequests() {
	compoRemains.clear();
	requested.clear();
}

////////////////////////////////////////////////////////////////////////////////

}
}
