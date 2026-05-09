namespace hfsm2 {

////////////////////////////////////////////////////////////////////////////////

enum class Method : uint8_t {
	NONE,

	SELECT,

//#if HFSM2_UTILITY_THEORY_AVAILABLE()
	RANK,
	UTILITY,
//#endif

	ENTRY_GUARD,
	ENTER,
	REENTER,
	PRE_UPDATE,
	UPDATE,
	POST_UPDATE,
	PRE_REACT,
	REACT,
	QUERY,
	POST_REACT,
	EXIT_GUARD,
	EXIT,

//#if HFSM2_PLANS_AVAILABLE()
	PLAN_SUCCEEDED,
	PLAN_FAILED,
//#endif

	COUNT
};

//------------------------------------------------------------------------------

enum class TransitionType : uint8_t {
	CHANGE,
	RESTART,
	RESUME,
	SELECT,

//#if HFSM2_UTILITY_THEORY_AVAILABLE()
	UTILIZE,
	RANDOMIZE,
//#endif

	SCHEDULE,

	COUNT
};

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

enum class StatusEvent : uint8_t {
	SUCCEEDED,
	FAILED,

	COUNT
};

#endif

//------------------------------------------------------------------------------

static
HFSM2_CONSTEXPR(14)
const char*
methodName(const Method method)											noexcept	{
	switch (method) {

	case Method::SELECT:		 return "select";

//#if HFSM2_UTILITY_THEORY_AVAILABLE()
	case Method::RANK:			 return "rank";
	case Method::UTILITY:		 return "utility";
//#endif

	case Method::ENTRY_GUARD:	 return "entryGuard";
	case Method::ENTER:			 return "enter";
	case Method::REENTER:		 return "reenter";
	case Method::PRE_UPDATE:	 return "preUpdate";
	case Method::UPDATE:		 return "update";
	case Method::POST_UPDATE:	 return "postUpdate";
	case Method::PRE_REACT:		 return "preReact";
	case Method::REACT:			 return "react";
	case Method::POST_REACT:	 return "postReact";
	case Method::QUERY:			 return "query";
	case Method::EXIT_GUARD:	 return "exitGuard";
	case Method::EXIT:			 return "exit";

//#if HFSM2_PLANS_AVAILABLE()
	case Method::PLAN_SUCCEEDED: return "planSucceeded";
	case Method::PLAN_FAILED:	 return "planFailed";
//#endif

	default:
		HFSM2_BREAK();
		return nullptr;
	}
}

//------------------------------------------------------------------------------

static
HFSM2_CONSTEXPR(14)
const char*
transitionName(const TransitionType type)								noexcept	{
	switch (type) {
	case TransitionType::CHANGE:	return "changeTo";
	case TransitionType::RESTART:	return "restart";
	case TransitionType::RESUME:	return "resume";
	case TransitionType::SELECT:	return "select";

//#if HFSM2_UTILITY_THEORY_AVAILABLE()
	case TransitionType::UTILIZE:	return "utilize";
	case TransitionType::RANDOMIZE:	return "randomize";
//#endif

	case TransitionType::SCHEDULE:	return "schedule";

	default:
		HFSM2_BREAK();
		return nullptr;
	}
}

//------------------------------------------------------------------------------

namespace detail {

////////////////////////////////////////////////////////////////////////////////

enum Strategy {
	Composite,
	Resumable,
	Selectable,

//#if HFSM2_UTILITY_THEORY_AVAILABLE()
	Utilitarian,
	RandomUtil,
//#endif
};

//------------------------------------------------------------------------------

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

struct TransitionBase {
	HFSM2_CONSTEXPR(11)
	TransitionBase()													noexcept = default;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(11)
	TransitionBase(const StateID destination_,
				   const TransitionType type_)							noexcept
		: destination{destination_}
		, type		 {type_}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(11)
	TransitionBase(const StateID origin_,
				   const StateID destination_,
				   const TransitionType type_)							noexcept
		: origin	 {origin_}
		, destination{destination_}
		, type		 {type_}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(11)
	bool
	operator == (const TransitionBase& other)					  const noexcept	{
		return origin	   == other.origin		&&
			   destination == other.destination &&
			   method	   == other.method		&&
			   type		   == other.type;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(11)
	bool
	operator != (const TransitionBase& other)					  const noexcept	{
		return origin	   != other.origin		||
			   destination != other.destination ||
			   method	   != other.method		||
			   type		   != other.type;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
struct TransitionT final
	: TransitionBase
{
	using Payload = TPayload;
	using Storage = uint8_t[sizeof(Payload)];
	using This	  = TransitionT<Payload>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)
		  Payload&
	payloadStorage()													noexcept	{
		return *::hfsm2::reinterpret_launder<Payload>(storage);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(11)
	const Payload&
	payloadStorage()											  const noexcept	{
		return *::hfsm2::reinterpret_launder<Payload>(storage);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(11)
	TransitionT()														noexcept = default;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)
	TransitionT(const StateID destination_,
				const TransitionType type_)								noexcept
		: TransitionBase{destination_, type_}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)
	TransitionT(const StateID origin_,
				const StateID destination_,
				const TransitionType type_)								noexcept
		: TransitionBase{origin_, destination_, type_}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)
	TransitionT(const This& other)								  HFSM2_NOEXCEPT_17(noexcept(Payload{              other.payloadStorage() }))
		: TransitionBase{other.origin, other.destination, other.type}
	{
		method = other.method;
		payloadSet = other.payloadSet;

		if (other.payloadSet)
			new (storage) Payload{other.payloadStorage()};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)
	TransitionT(This&& other)									  HFSM2_NOEXCEPT_17(noexcept(Payload{::hfsm2::move(other.payloadStorage())}))
		: TransitionBase{other.origin, other.destination, other.type}
	{
		method = other.method;
		payloadSet = other.payloadSet;

		if (other.payloadSet)
			new (storage) Payload{::hfsm2::move(other.payloadStorage())};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)
	TransitionT(const StateID destination_,
				const TransitionType type_,
				const Payload& payload)							  HFSM2_NOEXCEPT_17(noexcept(Payload{payload}))
		: TransitionBase{destination_, type_}
		, payloadSet{true}
	{
		new (storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)
	TransitionT(const StateID origin_,
				const StateID destination_,
				const TransitionType type_,
				const Payload& payload)							  HFSM2_NOEXCEPT_17(noexcept(Payload{payload}))
		: TransitionBase{origin_, destination_, type_}
		, payloadSet{true}
	{
		new (storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(20)
	~TransitionT()														noexcept	{
		clearPayload();
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)
	This&
	operator = (const This& other)								  HFSM2_NOEXCEPT_17(noexcept(payloadStorage() =               other.payloadStorage() ) && noexcept(Payload{              other.payloadStorage() }))	{
		if (this == &other)
			return *this;

		origin		= other.origin;
		destination = other.destination;
		method		= other.method;
		type		= other.type;

		assignPayload(other);

		return *this;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)
	This&
	operator = (This&& other)									  HFSM2_NOEXCEPT_17(noexcept(payloadStorage() = ::hfsm2::move(other.payloadStorage())) && noexcept(Payload{::hfsm2::move(other.payloadStorage())}))	{
		if (this == &other)
			return *this;

		origin		= other.origin;
		destination = other.destination;
		method		= other.method;
		type		= other.type;

		assignPayload(::hfsm2::move(other));

		return *this;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(11)
	bool
	operator == (const TransitionT& other)						  const noexcept	{
		return TransitionBase::operator == (other) &&
			   (payloadSet ==  other.payloadSet);
		//	  (!payloadSet && !other.payloadSet || payload ==  other.payload);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(11)
	bool
	operator != (const TransitionT& other)						  const noexcept	{
		return TransitionBase::operator != (other) ||
			   (payloadSet != other.payloadSet);
		//	   (payloadSet |= other.payloadSet || payload != other.payload);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(11)
	const Payload*
	payload()													  const noexcept	{
		return payloadSet ?
			&payloadStorage() : nullptr;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)
	void
	clearPayload()														noexcept	{
		if (payloadSet) {
			::hfsm2::destroy(payloadStorage());
			payloadSet = false;
		}
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)
	void
	assignPayload(const This& other)							  HFSM2_NOEXCEPT_17(noexcept(payloadStorage() =               other.payloadStorage() ) && noexcept(Payload{              other.payloadStorage() }))	{
		if (other.payloadSet) {
			if (payloadSet)
				payloadStorage() = other.payloadStorage();
			else {
				new (storage) Payload{other.payloadStorage()};
				payloadSet = true;
			}
		}
		else
			clearPayload();
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)
	void
	assignPayload(This&& other)									  HFSM2_NOEXCEPT_17(noexcept(payloadStorage() = ::hfsm2::move(other.payloadStorage())) && noexcept(Payload{::hfsm2::move(other.payloadStorage())}))	{
		if (other.payloadSet) {
			if (payloadSet)
				payloadStorage() = ::hfsm2::move(other.payloadStorage());
			else {
				new (storage) Payload{::hfsm2::move(other.payloadStorage())};
				payloadSet = true;
			}
		}
		else
			clearPayload();
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

	alignas(Payload) Storage storage {};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

	bool payloadSet = false;
};

//------------------------------------------------------------------------------

template <>
struct TransitionT<void> final
	: TransitionBase
{
	using TransitionBase::TransitionBase;
};

////////////////////////////////////////////////////////////////////////////////

}

struct Request final {
	TransitionType type;
	Short index;
};

////////////////////////////////////////////////////////////////////////////////

}
