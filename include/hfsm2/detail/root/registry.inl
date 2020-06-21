namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NTC>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NTC>>::isActive(const StateID stateId) const {
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

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NTC>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NTC>>::isResumable(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong == compoResumable[parent.forkId - 1];
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NTC>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NTC>>::isPendingChange(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return compoRequested[parent.forkId - 1] !=
					   compoActive	 [parent.forkId - 1];
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NTC>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NTC>>::isPendingEnter(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong != compoActive	 [parent.forkId - 1] &&
					   parent.prong == compoRequested[parent.forkId - 1];
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NTC>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NTC>>::isPendingExit(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong == compoActive	 [parent.forkId - 1] &&
					   parent.prong != compoRequested[parent.forkId - 1];
		}

	return true;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NTC>
const Parent&
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NTC>>::forkParent(const ForkID forkId) const {
	HFSM_ASSERT(forkId != 0);

	return forkId > 0 ?
		compoParents[ forkId - 1] :
		orthoParents[-forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NTC>
typename RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NTC>>::OrthoBits
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NTC>>::requestedOrthoFork(const ForkID forkId) {
	HFSM_ASSERT(forkId < 0);
	const Units& units = orthoUnits[-forkId - 1];

	return orthoRequested.bits(units);
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NTC>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NTC>>::requestImmediate(const Request request) {
	if (request.stateId == 0)
		return false;
	else if (HFSM_CHECKED(request.stateId < STATE_COUNT)) {
		Parent parent;

		for (parent = stateParents[request.stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			if (parent.forkId > 0) {
				compoRequested[parent.forkId - 1] = parent.prong;
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

				if (compoActive	  [parent.forkId - 1] != parent.prong)
					compoRequested[parent.forkId - 1]  = parent.prong;
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

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NTC>
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NTC>>::requestScheduled(const StateID stateId) {
	if (HFSM_CHECKED(stateId < STATE_COUNT)) {
		const Parent parent = stateParents[stateId];

		HFSM_ASSERT(parent.forkId != 0);
		if (parent.forkId > 0)
			compoResumable[parent.forkId - 1] = parent.prong;
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NTC>
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NTC>>::clearRequests() {
	compoRequested.clear();
	orthoRequested.clear();
	compoRemains  .clear();
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, LongIndex NSB, LongIndex NTC>
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NTC>>::reset() {
	compoRequested.clear();
	orthoRequested.clear();
	compoActive	  .clear();
	compoResumable.clear();
	compoRemains  .clear();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NSB, LongIndex NTC>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB, NTC>>::isActive(const StateID stateId) const {
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

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NSB, LongIndex NTC>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB, NTC>>::isResumable(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM_ASSERT(parent.forkId > 0);

			return parent.prong == compoResumable[parent.forkId - 1];
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NSB, LongIndex NTC>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB, NTC>>::isPendingChange(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM_ASSERT(parent.forkId > 0);

			return compoRequested[parent.forkId - 1] !=
				   compoActive	 [parent.forkId - 1];
		}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NSB, LongIndex NTC>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB, NTC>>::isPendingEnter(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM_ASSERT(parent.forkId > 0);

			return parent.prong != compoActive	 [parent.forkId - 1] &&
				   parent.prong == compoRequested[parent.forkId - 1];
		}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NSB, LongIndex NTC>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB, NTC>>::isPendingExit(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM_ASSERT(parent.forkId > 0);

			return parent.prong == compoActive	 [parent.forkId - 1] &&
				   parent.prong != compoRequested[parent.forkId - 1];
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NSB, LongIndex NTC>
const Parent&
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB, NTC>>::forkParent(const ForkID forkId) const {
	HFSM_ASSERT(forkId > 0);

	return compoParents[forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NSB, LongIndex NTC>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB, NTC>>::requestImmediate(const Request request) {
	if (request.stateId == 0)
		return false;
	else if (HFSM_CHECKED(request.stateId < STATE_COUNT)) {
		Parent parent = stateParents[request.stateId];

		if (HFSM_CHECKED(parent)) {
			HFSM_ASSERT(parent.forkId > 0);

			compoRequested[parent.forkId - 1] = parent.prong;

			for (parent = forkParent(parent.forkId);
				 parent;
				 parent = forkParent(parent.forkId))
			{
				HFSM_ASSERT(parent.forkId > 0);
				compoRemains.set(parent.forkId - 1);

				if (compoActive	  [parent.forkId - 1] != parent.prong)
					compoRequested[parent.forkId - 1]  = parent.prong;
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

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NSB, LongIndex NTC>
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB, NTC>>::requestScheduled(const StateID stateId) {
	if (HFSM_CHECKED(stateId < STATE_COUNT)) {
		const Parent parent = stateParents[stateId];

		HFSM_ASSERT(parent.forkId > 0);
		compoResumable[parent.forkId - 1] = parent.prong;
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NSB, LongIndex NTC>
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB, NTC>>::clearRequests() {
	compoRequested.clear();
	orthoRequested.clear();
	compoRemains  .clear();
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NSB, LongIndex NTC>
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB, NTC>>::reset() {
	compoRequested.clear();
	orthoRequested.clear();
	compoActive	  .clear();
	compoResumable.clear();
	compoRemains  .clear();
}

////////////////////////////////////////////////////////////////////////////////

}
}
