#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct alignas(2 * sizeof(ShortIndex)) Parent {
	ShortIndex fork  = INVALID_SHORT_INDEX;
	ShortIndex prong = INVALID_SHORT_INDEX;

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
};

#pragma pack(pop)

using Parents = ArrayView<Parent>;

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct alignas(4 * sizeof(ShortIndex)) Fork {
	ShortIndex self		 = INVALID_SHORT_INDEX;
	ShortIndex active	 = INVALID_SHORT_INDEX;
	ShortIndex resumable = INVALID_SHORT_INDEX;
	ShortIndex requested = INVALID_SHORT_INDEX;

	inline Fork(const ShortIndex self_,
				const Parent parent,
				Parents& forkParents)
		: self(self_)
	{
		forkParents[self_] = parent;
	}
};

#pragma pack(pop)

using ForkPointers = ArrayView<Fork*>;

////////////////////////////////////////////////////////////////////////////////

}
}
