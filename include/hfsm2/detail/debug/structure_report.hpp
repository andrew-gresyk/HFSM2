namespace hfsm2 {

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

struct StructureEntry {
	bool isActive;
	const wchar_t* prefix;
	const char* name;
};

#endif

namespace detail {

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY

TransitionType
HFSM2_INLINE
convert(const Request::Type type) {
	switch (type) {
		case Request::CHANGE:
			return TransitionType::CHANGE;

		case Request::RESTART:
			return TransitionType::RESTART;

		case Request::RESUME:
			return TransitionType::RESUME;

	#ifdef HFSM2_ENABLE_UTILITY_THEORY

		case Request::UTILIZE:
			return TransitionType::UTILIZE;

		case Request::RANDOMIZE:
			return TransitionType::RANDOMIZE;

	#endif

		case Request::SCHEDULE:
			return TransitionType::SCHEDULE;

		default:
			HFSM2_BREAK();
			return TransitionType::CHANGE;
	}
}

//------------------------------------------------------------------------------

Request::Type
HFSM2_INLINE
convert(const TransitionType type) {
	switch (type) {
	case TransitionType::CHANGE:
		return Request::CHANGE;

	case TransitionType::RESTART:
		return Request::RESTART;

	case TransitionType::RESUME:
		return Request::RESUME;

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	case TransitionType::UTILIZE:
		return Request::UTILIZE;

	case TransitionType::RANDOMIZE:
		return Request::RANDOMIZE;

#endif

	case TransitionType::SCHEDULE:
		return Request::SCHEDULE;

	default:
		HFSM2_BREAK();
		return Request::CHANGE;
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

#pragma pack(push, 1)

struct alignas(alignof(void*)) StructureStateInfo {
	enum RegionType : ShortIndex {
		COMPOSITE,
		ORTHOGONAL,
	};

	StructureStateInfo() = default;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE StructureStateInfo(const LongIndex parent_,
									const RegionType region_,
									const ShortIndex depth_,
									const char* const name_)
		: name{name_}
		, parent{parent_}
		, region{region_}
		, depth{depth_}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	const char* name;
	LongIndex parent;
	RegionType region;
	ShortIndex depth;
};

#pragma pack(pop)

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

#endif

}

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_TRANSITION_HISTORY

#pragma pack(push, 1)

struct alignas(4) Transition {
	HFSM2_INLINE Transition() = default;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE Transition(const detail::Request request,
							const Method method_)
		: stateId{request.stateId}
		, method{method_}
		, transitionType{detail::convert(request.type)}
	{
		HFSM2_ASSERT(method_ < Method::COUNT);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE Transition(const StateID stateId_,
							const Method method_,
							const TransitionType transitionType_)
		: stateId{stateId_}
		, method{method_}
		, transitionType{transitionType_}
	{
		HFSM2_ASSERT(method_ < Method::COUNT);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	detail::Request request() const		{ return detail::Request{detail::convert(transitionType), stateId};	}

	StateID stateId = INVALID_STATE_ID;
	Method method;
	TransitionType transitionType;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

bool operator == (const Transition& l, const Transition& r) {
	return l.stateId		== r.stateId
		&& l.method			== r.method
		&& l.transitionType	== r.transitionType;
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
