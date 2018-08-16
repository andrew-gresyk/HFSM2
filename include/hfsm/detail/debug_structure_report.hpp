#pragma once

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

namespace hfsm {

//------------------------------------------------------------------------------

struct StructureEntry {
	bool isActive;
	const wchar_t* prefix;
	const char* name;
};
using MachineStructure = detail::ArrayView<StructureEntry>;
using MachineActivity  = detail::ArrayView<char>;

//------------------------------------------------------------------------------

namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct StructureStateInfo {
	enum RegionType : ShortIndex {
		COMPOSITE,
		ORTHOGONAL,
	};

	StructureStateInfo() = default;

	inline StructureStateInfo(const LongIndex parent_,
							  const RegionType region_,
							  const ShortIndex depth_,
							  const char* const name_)
		: name(name_)
		, parent(parent_)
		, region(region_)
		, depth(depth_)
	{
		HFSM_IF_ALIGNMENT_CHEKS(assert((((uintptr_t) this) & (sizeof(void*) - 1)) == 0));
	}

	alignas(alignof(char*)) const char* name;
	LongIndex parent;
	RegionType region;
	ShortIndex depth;
};

#pragma pack(pop)

using StructureStateInfos = ArrayView<StructureStateInfo>;

//------------------------------------------------------------------------------

template <typename TPayloadList>
::hfsm::Transition
inline get(const typename TransitionT<TPayloadList>::Type type) {
	using Transition = TransitionT<TPayloadList>;

	switch (type) {
		case Transition::RESTART:
			return ::hfsm::Transition::RESTART;
		case Transition::RESUME:
			return ::hfsm::Transition::RESUME;
		case Transition::SCHEDULE:
			return ::hfsm::Transition::SCHEDULE;
		default:
			assert(false);
			return ::hfsm::Transition::RESTART;
	}
}

#pragma pack(push, 2)

template <typename TPayloadList>
struct TransitionInfoT {
	using TransitionPayloads = TPayloadList;
	using Transition = TransitionT<TPayloadList>;

	inline TransitionInfoT() = default;

	inline TransitionInfoT(const Transition transition_,
						   const ::hfsm::Method method_)
		: stateId{transition_.stateId}
		, method(method_)
		, transition(get<TransitionPayloads>(transition_.type))
	{
		HFSM_IF_ALIGNMENT_CHEKS(assert((((uintptr_t) this) & (sizeof(void*) - 1)) == 0));
		assert(method_ < ::hfsm::Method::COUNT);
	}

	StateID stateId = INVALID_STATE_ID;
	::hfsm::Method method;
	::hfsm::Transition transition;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
