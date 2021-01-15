namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::isActive(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM2_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong == compoActive[parent.forkId - 1];
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::isResumable(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM2_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong == compoResumable[parent.forkId - 1];
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::isPendingChange(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM2_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return compoRequested[parent.forkId - 1] !=
					   compoActive	 [parent.forkId - 1];
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::isPendingEnter(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM2_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong != compoActive	 [parent.forkId - 1] &&
					   parent.prong == compoRequested[parent.forkId - 1];
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::isPendingExit(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM2_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong == compoActive	 [parent.forkId - 1] &&
					   parent.prong != compoRequested[parent.forkId - 1];
		}

	return true;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
const Parent&
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::forkParent(const ForkID forkId) const noexcept {
	HFSM2_ASSERT(forkId != 0);

	return forkId > 0 ?
		compoParents[ forkId - 1] :
		orthoParents[-forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
typename RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::OrthoBits
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::requestedOrthoFork(const ForkID forkId) noexcept {
	HFSM2_ASSERT(forkId < 0);
	const Units& units = orthoUnits[-forkId - 1];

	return orthoRequested.bits(units);
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
bool
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::requestImmediate(const Transition& request) noexcept {
	if (request.destination == 0)
		return false;
	else if (HFSM2_CHECKED(request.destination < STATE_COUNT)) {
		Parent parent;

		for (parent = stateParents[request.destination];
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
				HFSM2_BREAK();
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
				HFSM2_BREAK();
		}

		for (; parent; parent = forkParent(parent.forkId)) {
			if (parent.forkId > 0)
				compoRemains.set(parent.forkId - 1);
			else if (parent.forkId < 0)
				requestedOrthoFork(parent.forkId).set(parent.prong);
			else
				HFSM2_BREAK();
		}
	}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::requestScheduled(const StateID stateId) noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT)) {
		const Parent parent = stateParents[stateId];

		HFSM2_ASSERT(parent.forkId != 0);
		if (parent.forkId > 0)
			compoResumable[parent.forkId - 1] = parent.prong;
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::clearRequests() noexcept {
	compoRequested.clear();
	orthoRequested.clear();
	compoRemains  .clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, Long NCC, Long NOC, Long NOU, Long NSB, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TTP>
void
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB, NSL HFSM2_IF_PLANS(, NTC), TTP>>::clear() noexcept {
	clearRequests();

	compoActive	  .clear();
	compoResumable.clear();
}

////////////////////////////////////////////////////////////////////////////////

}
}
