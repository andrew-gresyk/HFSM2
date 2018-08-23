#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct alignas(2 * sizeof(ShortIndex)) Parent {
	inline Parent() = default;

	inline Parent(const ShortIndex fork_,
				  const ShortIndex prong_)
		: fork(fork_)
		, prong(prong_)
	{}

	inline explicit operator bool() const {
		return fork  != INVALID_SHORT_INDEX &&
			   prong != INVALID_SHORT_INDEX;
	}

	ShortIndex fork  = INVALID_SHORT_INDEX;
	ShortIndex prong = INVALID_SHORT_INDEX;
};

#pragma pack(pop)

using Parents = ArrayView<Parent>;

////////////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG
#pragma pack(push, 1)
#endif

struct HSFM_UNLESS_DEBUG(alignas(4 * sizeof(ShortIndex))) Fork {
	inline Fork(const ShortIndex self_,
				const Parent parent,
				Parents& forkParents,
				const ShortIndex default_
				HSFM_IF_DEBUG(, std::type_index type))
		: self(self_)
		, active(default_)
		, resumable(default_)
		HSFM_IF_DEBUG(, TYPE{type})
	{
		forkParents[self_] = parent;
	}

	ShortIndex self;
	ShortIndex active;
	ShortIndex resumable;
	ShortIndex requested = INVALID_SHORT_INDEX;
	HSFM_IF_DEBUG(const std::type_index TYPE);
};

#ifndef _DEBUG
#pragma pack(pop)
#endif

using ForkPointers = ArrayView<Fork*>;

////////////////////////////////////////////////////////////////////////////////

template <LongIndex NStateCount>
using ParentsT = Array<Parent, NStateCount>;

template <LongIndex NForkCount>
using ForkPointersT = Array<Fork*, NForkCount>;

template <LongIndex NStateCount, LongIndex NForkCount>
struct RegistryT {
	static constexpr LongIndex STATE_COUNT = NStateCount;
	static constexpr LongIndex FORK_COUNT  = NForkCount;

	using StateParents = ParentsT<STATE_COUNT>;
	using ForkParents  = ParentsT<FORK_COUNT>;
	using ForkPointers = ForkPointersT<FORK_COUNT>;

	StateParents stateParents;
	ForkParents  forkParents;
	ForkPointers forkPointers;
};

//------------------------------------------------------------------------------

template <LongIndex NStateCount, LongIndex NForkCount>
bool isActive(const RegistryT<NStateCount, NForkCount>& registry,
			  const StateID stateId);

template <LongIndex NStateCount, LongIndex NForkCount>
bool isResumable(const RegistryT<NStateCount, NForkCount>& registry,
				 const StateID stateId);

template <LongIndex NStateCount, LongIndex NForkCount>
inline
bool isScheduled(const RegistryT<NStateCount, NForkCount>& registry,
				 const StateID stateId)
{
	return isResumable(registry, stateId);
}

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_utility.inl"
