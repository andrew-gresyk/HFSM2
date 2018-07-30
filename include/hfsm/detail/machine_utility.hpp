#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct Parent {
	ShortIndex fork  = INVALID_SHORT_INDEX;
	ShortIndex prong = INVALID_SHORT_INDEX;

	inline Parent() = default;

	inline Parent(const ShortIndex fork_,
				  const ShortIndex prong_)
		: fork(fork_)
		, prong(prong_)
	{
		HFSM_IF_ALIGNMENT_CHEKS(assert((((uintptr_t) this) & 0x1) == 0));
	}

	inline explicit operator bool() const {
		return fork  != INVALID_SHORT_INDEX &&
			   prong != INVALID_SHORT_INDEX;
	}
};

#pragma pack(pop)

using Parents = ArrayView<Parent>;

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct Fork {
	ShortIndex self		 = INVALID_SHORT_INDEX;
	ShortIndex active	 = INVALID_SHORT_INDEX;
	ShortIndex resumable = INVALID_SHORT_INDEX;
	ShortIndex requested = INVALID_SHORT_INDEX;

	inline Fork(const ShortIndex self_,
				const Parent parent,
				Parents& forkParents)
		: self(self_)
	{
		HFSM_IF_ALIGNMENT_CHEKS(assert((((uintptr_t) this) & 0x3) == 0));

		forkParents[self_] = parent;
	}
};

#pragma pack(pop)

using ForkPointers = ArrayView<Fork*>;

////////////////////////////////////////////////////////////////////////////////

}
}
