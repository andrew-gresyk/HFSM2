namespace hfsm2 {

////////////////////////////////////////////////////////////////////////////////

enum class Method : uint8_t {
	NONE,

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	RANK,
	UTILITY,
#endif

	ENTRY_GUARD,
	CONSTRUCT,
	ENTER,
	REENTER,
	UPDATE,
	REACT,
	EXIT_GUARD,
	EXIT,
	DESTRUCT,

#ifdef HFSM2_ENABLE_PLANS
	PLAN_SUCCEEDED,
	PLAN_FAILED,
#endif

	COUNT
};

enum class TransitionType : uint8_t {
	CHANGE,
	RESTART,
	RESUME,

//#ifdef HFSM2_ENABLE_UTILITY_THEORY
	UTILIZE,
	RANDOMIZE,
//#endif

	SCHEDULE,

	COUNT
};

#ifdef HFSM2_ENABLE_PLANS

enum class StatusEvent : uint8_t {
	SUCCEEDED,
	FAILED,

	COUNT
};

#endif

//------------------------------------------------------------------------------

static inline
const char*
stateName(const std::type_index stateType) noexcept {
	const char* const raw = stateType.name();

	#if defined(_MSC_VER)

		auto first =
			raw[0] == 's' ? 7 : // Struct
			raw[0] == 'c' ? 6 : // Class
							0;
		return raw + first;

	#elif defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)

		return raw;

	#else

		return raw;

	#endif
}

//------------------------------------------------------------------------------

static inline
const char*
methodName(const Method method) noexcept {
	switch (method) {

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	case Method::RANK:			 return "rank";
	case Method::UTILITY:		 return "utility";
#endif

	case Method::ENTRY_GUARD:	 return "entryGuard";
	case Method::ENTER:			 return "enter";
	case Method::CONSTRUCT:		 return "construct";
	case Method::REENTER:		 return "reenter";
	case Method::UPDATE:		 return "update";
	case Method::REACT:			 return "react";
	case Method::EXIT_GUARD:	 return "exitGuard";
	case Method::EXIT:			 return "exit";
	case Method::DESTRUCT:		 return "destruct";

#ifdef HFSM2_ENABLE_PLANS
	case Method::PLAN_SUCCEEDED: return "planSucceeded";
	case Method::PLAN_FAILED:	 return "planFailed";
#endif

	default:
		HFSM2_BREAK();
		return nullptr;
	}
}

//------------------------------------------------------------------------------

static inline
const char*
transitionName(const TransitionType type) noexcept {
	switch (type) {
	case TransitionType::CHANGE:	return "changeTo";
	case TransitionType::RESTART:	return "restart";
	case TransitionType::RESUME:	return "resume";

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	case TransitionType::UTILIZE:	return "utilize";
	case TransitionType::RANDOMIZE:	return "randomize";
#endif

	case TransitionType::SCHEDULE:	return "schedule";

	default:
		HFSM2_BREAK();
		return nullptr;
	}
}

namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

struct alignas(4) TransitionBase {
	constexpr TransitionBase() noexcept = default;

	//----------------------------------------------------------------------

	HFSM2_INLINE TransitionBase(const StateID destination_,
								const TransitionType type_) noexcept
		: destination{destination_}
		, type{type_}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE TransitionBase(const StateID origin_,
								const StateID destination_,
								const TransitionType type_) noexcept
		: origin	 {origin_}
		, destination{destination_}
		, type		 {type_}
	{}

	//----------------------------------------------------------------------

	HFSM2_INLINE bool operator == (const TransitionBase& other) const noexcept {
		return origin	   == other.origin &&
			   destination == other.destination &&
			   method	   == other.method &&
			   type		   == other.type;
	}

	//----------------------------------------------------------------------

	StateID	origin		= INVALID_STATE_ID;
	StateID	destination	= INVALID_STATE_ID;
	Method	method		= Method::NONE;
	TransitionType type	= TransitionType::COUNT;
};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TPayload>
struct alignas(4) TransitionT
	: TransitionBase
{
	using Payload = TPayload;
	using Storage = typename std::aligned_storage<sizeof(Payload), 4>::type;

	using TransitionBase::TransitionBase;

	//----------------------------------------------------------------------

	HFSM2_INLINE TransitionT() noexcept {
		new (&storage) Payload{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE TransitionT(const StateID destination,
							 const TransitionType type,
							 const Payload& payload) noexcept
		: TransitionBase{destination, type}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE TransitionT(const StateID destination,
							 const TransitionType type,
							 Payload&& payload) noexcept
		: TransitionBase{destination, type}
		, payloadSet{true}
	{
		new (&storage) Payload{std::move(payload)};
	}

	//----------------------------------------------------------------------

	HFSM2_INLINE TransitionT(const StateID origin,
							 const StateID destination,
							 const TransitionType type,
							 const Payload& payload) noexcept
		: TransitionBase{origin, destination, type}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE TransitionT(const StateID origin,
							 const StateID destination,
							 const TransitionType type,
							 Payload&& payload) noexcept
		: TransitionBase{origin, destination, type}
		, payloadSet{true}
	{
		new (&storage) Payload{std::move(payload)};
	}

	//----------------------------------------------------------------------

	HFSM2_INLINE bool operator == (const TransitionT& other) const noexcept {
		return TransitionBase::operator == (other) &&
			   (payloadSet ==  other.payloadSet);
		//	  (!payloadSet && !other.payloadSet || payload ==  other.payload);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	bool payloadSet = false;
	Storage storage;
};

//------------------------------------------------------------------------------

template <>
struct alignas(4) TransitionT<void>
	: TransitionBase
{
	using TransitionBase::TransitionBase;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}

struct Request {
	TransitionType type;
	Short index;
};

////////////////////////////////////////////////////////////////////////////////

}
