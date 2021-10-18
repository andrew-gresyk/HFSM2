#if HFSM2_PLANS_AVAILABLE()

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct TaskBase {
	HFSM2_CONSTEXPR(11)	TaskBase()								  noexcept {}

	HFSM2_CONSTEXPR(11)	TaskBase(const StateID origin_,
								 const StateID destination_,
								 const TransitionType type_)	  noexcept
		: origin{origin_}
		, destination{destination_}
		, type{type_}
	{}

	static_assert(sizeof(Long) == sizeof(StateID), "");

	union {
		StateID origin		= INVALID_STATE_ID;
		Long prev;
	};

	union {
		StateID destination	= INVALID_STATE_ID;
		Long next;
	};

	TransitionType type = TransitionType::COUNT;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

HFSM2_CONSTEXPR(11)
bool
operator == (const TaskBase& lhs,
			 const TaskBase& rhs)								  noexcept
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
	using Storage = typename std::aligned_storage<sizeof(Payload), 2>::type;

	using TaskBase::TaskBase;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	TaskT()									  noexcept {
		new (&storage) Payload{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	TaskT(const StateID origin,
							  const StateID destination,
							  const TransitionType type,
							  const Payload& payload)			  noexcept
		: TaskBase{origin, destination, type}
		, payloadSet{true}
	{
		new (&storage) Payload{payload};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	TaskT(const StateID origin,
							  const StateID destination,
							  const TransitionType type,
							  Payload&& payload)				  noexcept
		: TaskBase{origin, destination, type}
		, payloadSet{true}
	{
		new (&storage) Payload{move(payload)};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	Storage storage;
	bool payloadSet = false;
};

//------------------------------------------------------------------------------

template <>
struct TaskT<void> final
	: TaskBase
{
	using TaskBase::TaskBase;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

template <typename TPayload, Long NCapacity>
class TaskListT {
public:
	using Index		= Long;

	static constexpr Index CAPACITY	= NCapacity;
	static constexpr Index INVALID	= Index (-1);

private:
	using Payload	= TPayload;
	using Item		= TaskT<Payload>;

public:
	template <typename... TArgs>
	HFSM2_CONSTEXPR(14)	Index emplace(TArgs&&... args)							  noexcept;

	HFSM2_CONSTEXPR(14)	void remove(const Index i)								  noexcept;

	HFSM2_CONSTEXPR(14)		  Item& operator[] (const Index i)					  noexcept;
	HFSM2_CONSTEXPR(11)	const Item& operator[] (const Index i)				const noexcept;

	HFSM2_CONSTEXPR(11)	Index count()										const noexcept	{ return _count;	}

private:
	HFSM2_IF_ASSERT(void verifyStructure(const Index occupied = INVALID)	const noexcept);

private:
	Index _vacantHead = 0;
	Index _vacantTail = 0;
	Index _last  = 0;
	Index _count = 0;
	Item _items[CAPACITY];
};

//------------------------------------------------------------------------------

template <typename TItem>
class TaskListT<TItem, 0> {};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "task_list.inl"

#endif
