#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <LongIndex NStateCount, LongIndex NForkCount>
bool
isActive(const RegistryT<NStateCount, NForkCount>& registry,
		 const StateID stateId)
{
	constexpr LongIndex STATE_COUNT = NStateCount;
	assert(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (auto parent = registry.stateParents[stateId];
			 parent;
			 parent = registry.forkParents[parent.fork])
		{
			const auto& fork = *registry.forkPointers[parent.fork];

			if (fork.active != STATE_COUNT)
				return parent.prong == fork.active;
		}

	return false;
}

//------------------------------------------------------------------------------

template <LongIndex NStateCount, LongIndex NForkCount>
bool
isResumable(const RegistryT<NStateCount, NForkCount>& registry,
			const StateID stateId)
{
	constexpr LongIndex STATE_COUNT = NStateCount;
	assert(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (auto parent = registry.stateParents[stateId];
			 parent;
			 parent = registry.forkParents[parent.fork])
		{
			const auto& fork = *registry.forkPointers[parent.fork];

			if (fork.active != STATE_COUNT)
				return parent.prong == fork.resumable;
		}

	return false;
}

////////////////////////////////////////////////////////////////////////////////

}
}
