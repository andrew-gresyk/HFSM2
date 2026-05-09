#if HFSM2_PLANS_AVAILABLE()

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

struct TaskBase {
	static_assert(sizeof(Long) == sizeof(StateID), "");

	HFSM2_CONSTEXPR(11)	TaskBase()										noexcept	{}

	HFSM2_CONSTEXPR(11)	TaskBase(const StateID origin_,
								 const StateID destination_,
								 const TransitionType type_)			noexcept
		: origin{origin_}
		, destination{destination_}
		, type{type_}
	{}

	HFSM2_CONSTEXPR(11) bool cyclic()							  const noexcept	{ return origin == destination;	}

	StateID origin		= INVALID_STATE_ID;
	StateID destination	= INVALID_STATE_ID;

	TransitionType type = TransitionType::COUNT;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

HFSM2_CONSTEXPR(11)
bool
operator == (const TaskBase& lhs,
			 const TaskBase& rhs)										noexcept
{
	return lhs.origin	   == rhs.origin &&
		   lhs.destination == rhs.destination &&
		   lhs.type		   == rhs.type;
}

//------------------------------------------------------------------------------

template <typename TPayload>
struct TaskT final
	: TaskBase
{
	using Payload = TPayload;
	using Storage = uint8_t[sizeof(Payload)];
	using This	  = TaskT<Payload>;

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

	HFSM2_CONSTEXPR(11)	TaskT()											noexcept = default;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(11)	TaskT(const StateID origin_,
							  const StateID destination_,
							  const TransitionType type_)				noexcept
		: TaskBase{origin_, destination_, type_}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	TaskT(const This& other)				  HFSM2_NOEXCEPT_17(noexcept(Payload{              other.payloadStorage() }))
		: TaskBase{other.origin, other.destination, other.type}
		, payloadSet{other.payloadSet}
	{
		if (other.payloadSet)
			new (storage) Payload{other.payloadStorage()};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	TaskT(This&& other)						  HFSM2_NOEXCEPT_17(noexcept(Payload{::hfsm2::move(other.payloadStorage())}))
		: TaskBase{other.origin, other.destination, other.type}
		, payloadSet{other.payloadSet}
	{
		if (other.payloadSet)
			new (storage) Payload{::hfsm2::move(other.payloadStorage())};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	TaskT(const StateID origin_,
							  const StateID destination_,
							  const TransitionType type_,
							  const Payload& payload)			  HFSM2_NOEXCEPT_17(noexcept(Payload{payload}))
		: TaskBase{origin_, destination_, type_}
		, payloadSet{true}
	{
		new (storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(20)	~TaskT()										noexcept	{
		clearPayload();
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	This& operator = (const This& other)	  HFSM2_NOEXCEPT_17(noexcept(payloadStorage() =               other.payloadStorage() ) && noexcept(Payload{              other.payloadStorage() })) {
		if (this == &other)
			return *this;

		origin		= other.origin;
		destination = other.destination;
		type		= other.type;

		assignPayload(other);

		return *this;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	This& operator = (This&& other)			  HFSM2_NOEXCEPT_17(noexcept(payloadStorage() = ::hfsm2::move(other.payloadStorage())) && noexcept(Payload{::hfsm2::move(other.payloadStorage())})) {
		if (this == &other)
			return *this;

		origin		= other.origin;
		destination = other.destination;
		type		= other.type;

		assignPayload(::hfsm2::move(other));

		return *this;
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
				payloadStorage() =    ::hfsm2::move(other.payloadStorage()) ;
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
struct TaskT<void> final
	: TaskBase
{
	using TaskBase::TaskBase;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
