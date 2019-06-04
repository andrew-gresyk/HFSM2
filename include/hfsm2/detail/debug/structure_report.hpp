#pragma once

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

namespace hfsm2 {

//------------------------------------------------------------------------------

struct StructureEntry {
	bool isActive;
	const wchar_t* prefix;
	const char* name;
};

//------------------------------------------------------------------------------

namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct alignas(alignof(void*)) StructureStateInfo {
	enum RegionType : ShortIndex {
		COMPOSITE,
		ORTHOGONAL,
	};

	StructureStateInfo() = default;

	HFSM_INLINE StructureStateInfo(const LongIndex parent_,
								   const RegionType region_,
								   const ShortIndex depth_,
								   const char* const name_)
		: name{name_}
		, parent{parent_}
		, region{region_}
		, depth{depth_}
	{}

	const char* name;
	LongIndex parent;
	RegionType region;
	ShortIndex depth;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

template <typename TPayload>
Transition
HFSM_INLINE get(const typename RequestT<TPayload>::Type type) {
	using Request = RequestT<TPayload>;

	switch (type) {
		case Request::CHANGE:
			return Transition::CHANGE;

		case Request::RESTART:
			return Transition::RESTART;

		case Request::RESUME:
			return Transition::RESUME;

		case Request::UTILIZE:
			return Transition::UTILIZE;

		case Request::SCHEDULE:
			return Transition::SCHEDULE;

		default:
			HFSM_BREAK();
			return Transition::CHANGE;
	}
}

#pragma pack(push, 1)

template <typename TPayload>
struct alignas(4) TransitionInfoT {
	using Payload = TPayload;
	using Request = RequestT<TPayload>;

	HFSM_INLINE TransitionInfoT() = default;

	HFSM_INLINE TransitionInfoT(const Request transition_,
								const Method method_)
		: stateId{transition_.stateId}
		, method{method_}
		, transition{get<Payload>(transition_.type)}
	{
		HFSM_ASSERT(method_ < Method::COUNT);
	}

	StateID stateId = INVALID_STATE_ID;
	Method method;
	Transition transition;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
