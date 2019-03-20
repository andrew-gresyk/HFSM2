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
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong == activeCompoProng(parent.forkId);
	}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isResumable(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong == resumableCompoProng(parent.forkId);
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isPendingChange(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return requestedCompoProng(parent.forkId) != activeCompoProng(parent.forkId);
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isPendingEnter(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong !=	  activeCompoProng(parent.forkId) &&
					   parent.prong == requestedCompoProng(parent.forkId);
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isPendingExit(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong ==	  activeCompoProng(parent.forkId) &&
					   parent.prong != requestedCompoProng(parent.forkId);
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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
ForkID
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::parentCompoForkId(const ForkID forkId) const {
	HFSM_ASSERT(forkId != 0);

	for (Parent parent = forkParent(forkId);
		 parent;
		 parent = forkParent(parent.forkId))
	{
		if (parent.forkId > 0)
			return parent.forkId;
	}

	HFSM_BREAK();
	return 0;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
ShortIndex
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::activeCompoProng(const ForkID forkId) const {
	HFSM_ASSERT(forkId > 0);

	return compoActive[forkId - 1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
ShortIndex
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::resumableCompoProng(const ForkID forkId) const {
	HFSM_ASSERT(forkId > 0);

	return resumable.compo[forkId - 1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
ShortIndex
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestedCompoProng(const ForkID forkId) const {
	HFSM_ASSERT(forkId > 0);

	return requested.compo[forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
ShortIndex&
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::resumableCompoFork(const ForkID forkId) {
	HFSM_ASSERT(forkId > 0);

	return resumable.compo[forkId - 1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
OrthoFork&
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::resumableOrthoFork(const ForkID forkId) {
	HFSM_ASSERT(forkId < 0);

	return resumable.ortho[-forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
ShortIndex&
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestedCompoFork(const ForkID forkId) {
	HFSM_ASSERT(forkId > 0);

	return requested.compo[forkId - 1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
OrthoFork&
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestedOrthoFork(const ForkID forkId) {
	HFSM_ASSERT(forkId < 0);

	return requested.ortho[-forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestImmediate(const Request request) {
	if (HFSM_CHECKED(request.stateId < STATE_COUNT)) {
		Parent parent;

		for (parent = stateParents[request.stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			if (parent.forkId > 0) {
				requestedCompoFork(parent.forkId) = parent.prong;

				break;
			} else if (parent.forkId < 0)
				requestedOrthoFork(parent.forkId)[parent.prong] = true;
			else
				HFSM_BREAK();
		}

		for (parent = forkParent(parent.forkId);
			 parent;
			 parent = forkParent(parent.forkId))
		{
			if (parent.forkId > 0) {
				if (   activeCompoProng(parent.forkId) != parent.prong)
					requestedCompoFork (parent.forkId)  = parent.prong;
				else {
					parent = forkParent(parent.forkId);
					break;
				}
			} else if (parent.forkId < 0)
				requestedOrthoFork(parent.forkId)[parent.prong] = true;
			else
				HFSM_BREAK();
		}

		for (; parent; parent = forkParent(parent.forkId)) {
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId < 0)
				requestedOrthoFork(parent.forkId)[parent.prong] = true;
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestScheduled(const StateID stateId) {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT) {
		const Parent parent = stateParents[stateId];

		if (parent.forkId > 0)
			resumableCompoFork(parent.forkId) = parent.prong;
		else if (parent.forkId < 0)
			resumableOrthoFork(parent.forkId)[parent.prong] = true;
		else
			HFSM_BREAK();
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::clearOrthoRequested() {
	for (ForkID i = 0; i < requested.ortho.count(); ++i) {
		OrthoFork& orthoRequested = requested.ortho[i];

		orthoRequested.clear();
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isActive(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId > 0);

			if (parent.forkId > 0)
				return parent.prong == activeCompoProng(parent.forkId);
	}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isResumable(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong == resumableCompoProng(parent.forkId);
	}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isPendingChange(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return requestedCompoProng(parent.forkId) != activeCompoProng(parent.forkId);
	}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isPendingEnter(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong !=    activeCompoProng(parent.forkId) &&
					   parent.prong == requestedCompoProng(parent.forkId);
		}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isPendingExit(const StateID stateId) const {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong ==    activeCompoProng(parent.forkId) &&
					   parent.prong != requestedCompoProng(parent.forkId);
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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
ShortIndex
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::activeCompoProng(const ForkID forkId) const {
	HFSM_ASSERT(forkId > 0);

	return compoActive[forkId - 1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
ShortIndex
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::resumableCompoProng(const ForkID forkId) const {
	HFSM_ASSERT(forkId > 0);

	return resumable.compo[forkId - 1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
ShortIndex
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::requestedCompoProng(const ForkID forkId) const {
	HFSM_ASSERT(forkId > 0);

	return requested.compo[forkId - 1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
ShortIndex&
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::resumableCompoFork(const ForkID forkId) {
	HFSM_ASSERT(forkId > 0);

	return resumable.compo[forkId - 1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
ShortIndex&
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::requestedCompoFork(const ForkID forkId) {
	HFSM_ASSERT(forkId > 0);

	return requested.compo[forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
void
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::requestImmediate(const Request request) {
	if (HFSM_CHECKED(request.stateId < STATE_COUNT)) {
		if (Parent parent = stateParents[request.stateId]) {
			HFSM_ASSERT(parent.forkId > 0);

			requestedCompoFork(parent.forkId) = parent.prong;

			for (parent = forkParent(parent.forkId);
				 parent;
				 parent = forkParent(parent.forkId))
			{
				HFSM_ASSERT(parent.forkId > 0);

				if (   activeCompoProng(parent.forkId) != parent.prong)
					requestedCompoFork (parent.forkId)  = parent.prong;
				else
					break;
			}
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
void
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::requestScheduled(const StateID stateId) {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT) {
		const Parent parent = stateParents[stateId];

		if (HFSM_CHECKED(parent.forkId > 0))
			resumableCompoFork(parent.forkId) = parent.prong;
	}
}

////////////////////////////////////////////////////////////////////////////////

}
}
