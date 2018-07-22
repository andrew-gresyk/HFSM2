#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename, typename, typename>
struct _S;

template <typename, typename, typename, typename...>
struct _C;

template <typename, typename, typename, typename...>
struct _O;

template <typename, typename, ShortIndex, typename>
class _R;

//------------------------------------------------------------------------------

#pragma pack(push, 1)

struct Parent {
	HSFM_IF_DEBUG(TypeInfo forkType);
	HSFM_IF_DEBUG(TypeInfo prongType);

	ShortIndex fork  = INVALID_SHORT_INDEX;
	ShortIndex prong = INVALID_SHORT_INDEX;

	inline Parent() = default;

	inline Parent(const ShortIndex fork_,
				  const ShortIndex prong_,
				  const TypeInfo HSFM_IF_DEBUG(forkType_),
				  const TypeInfo HSFM_IF_DEBUG(prongType_))
		: HSFM_IF_DEBUG(forkType(forkType_),)
		  HSFM_IF_DEBUG(prongType(prongType_),)
		  fork(fork_)
		, prong(prong_)
	{
		assert((((uintptr_t) this) & 0x1) == 0);
	}

	inline explicit operator bool() const {
		return fork  != INVALID_SHORT_INDEX &&
			   prong != INVALID_SHORT_INDEX;
	}
};

#pragma pack(pop)

using Parents = ArrayView<Parent>;

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 2)

template <typename TPayloadList>
struct StateInfoT {
	using PayloadList = TPayloadList;
	using Payload = typename PayloadList::Container;

	Payload payload;
	Parent parent;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

template <typename TPayloadList>
struct StateRegistryBaseT {
	using PayloadList = TPayloadList;
	using StateInfo = StateInfoT<PayloadList>;

public:
	virtual void insert(const TypeInfo type) = 0;
	virtual StateInfo& get(const TypeInfo type) = 0;
};

//------------------------------------------------------------------------------

template <typename TPayloadList, LongIndex NCapacity>
class StateRegistryT
	: public StateRegistryBaseT<TPayloadList>
{
	using PayloadList = TPayloadList;
	using StateInfo = StateInfoT<PayloadList>;

	enum : LongIndex { CAPACITY = NCapacity };

	using Table = HashTable<std::type_index, StateInfo, CAPACITY>;

public:
	inline		 StateInfo& operator[] (const TypeInfo type);
	inline const StateInfo& operator[] (const TypeInfo type) const;

	// StateRegistryBase
	virtual void insert(const TypeInfo type) override;
	virtual StateInfo& get(const TypeInfo type) override	{ return operator[](type);	}

private:
	Table _table;
};

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct Fork {
	HSFM_IF_DEBUG (const TypeInfo type);
	HSFM_IF_ASSERT(TypeInfo activeType);
	HSFM_IF_ASSERT(TypeInfo resumableType);
	HSFM_IF_ASSERT(TypeInfo requestedType);

	ShortIndex self		 = INVALID_SHORT_INDEX;
	ShortIndex active	 = INVALID_SHORT_INDEX;
	ShortIndex resumable = INVALID_SHORT_INDEX;
	ShortIndex requested = INVALID_SHORT_INDEX;

	inline Fork(const ShortIndex self_,
				const TypeInfo HSFM_IF_DEBUG(type_))
		: HSFM_IF_DEBUG(type(type_),)
		  self(self_)
	{
		assert((((uintptr_t) this) & 0x3) == 0);
	}
};

#pragma pack(pop)

using ForkPointers = ArrayView<Fork*>;

//------------------------------------------------------------------------------

template <typename T>
struct ForkT
	: Fork
{
	ForkT(const ShortIndex index,
		  const Parent parent,
		  Parents& forkParents)
		: Fork(index, TypeInfo::get<T>())
	{
		forkParents[index] = parent;
	}
};

////////////////////////////////////////////////////////////////////////////////

template <typename TPayloadList>
struct TransitionT {
	using PayloadList = TPayloadList;
	using Payload = typename PayloadList::Container;

	enum Type {
		REMAIN,
		RESTART,
		RESUME,
		SCHEDULE,

		COUNT
	};

	template <typename TCheck>
	struct Contains {
		using Check = TCheck;
		enum : bool {
			VALUE = PayloadList::template Contains<Check>::VALUE
		};
	};

	inline TransitionT() = default;

	inline TransitionT(const Type type_,
					   const TypeInfo stateType_)
		: stateType(stateType_)
		, type(type_)
	{
		assert(type_ < Type::COUNT);
	}

	template <typename T, typename = typename std::enable_if<Contains<T>::VALUE, T>::type>
	inline TransitionT(const Type type_,
					   const TypeInfo stateType_,
					   T* const payload_)
		: stateType(stateType_)
		, payload(payload_)
		, type(type_)
	{
		assert(type_ < Type::COUNT);
	}

	TypeInfo stateType;
	Payload payload;
	Type type = RESTART;
};

template <typename TPayloadList>
using TransitionQueueT = ArrayView<TransitionT<TPayloadList>>;

////////////////////////////////////////////////////////////////////////////////

template <typename TContext>
class ControlT {
	template <typename, typename, typename>
	friend struct _S;

	template <typename, typename, typename, typename...>
	friend struct _C;

	template <typename, typename, typename, typename...>
	friend struct _O;

	template <typename, typename, ShortIndex, typename>
	friend class _R;

protected:
	using Context = TContext;

	inline ControlT(Context& context,
					LoggerInterface* const HFSM_IF_LOGGER(logger))
		: _context(context)
		HFSM_IF_LOGGER(, _logger(logger))
	{}

public:
	inline auto& _()									{ return _context;		}
	inline auto& context()								{ return _context;		}

private:
#ifdef HFSM_ENABLE_LOG_INTERFACE
	inline auto& logger()								{ return _logger;		}
#endif

protected:
	Context& _context;
	HFSM_IF_LOGGER(LoggerInterface* _logger);
};

//------------------------------------------------------------------------------

template <typename TContext, typename TPayloadList>
class TransitionControlT final
	: public ControlT<TContext>
{
	using Context = TContext;
	using Base = ControlT<Context>;
	using PayloadList = TPayloadList;
	using Transition = TransitionT<PayloadList>;
	using TransitionType = enum Transition::Type;
	using TransitionQueue = TransitionQueueT<TPayloadList>;

	template <typename, typename, ShortIndex, typename>
	friend class _R;

	template <typename, typename>
	friend struct ControlLockT;

private:
	inline TransitionControlT(Context& context,
							  TransitionQueue& requests,
							  LoggerInterface* const logger)
		: Base(context, logger)
		, _requests(requests)
	{}

public:
	inline void changeTo(const std::type_index state);
	inline void resume	(const std::type_index state);
	inline void schedule(const std::type_index state);

	template <typename TState>
	inline void changeTo()					{ changeTo(typeid(TState));			}

	template <typename TState>
	inline void resume()					{ resume  (typeid(TState));			}

	template <typename TState>
	inline void schedule()					{ schedule(typeid(TState));			}

	inline auto requestCount() const		{ return _requests.count();			}

private:
	TransitionQueue& _requests;
	bool _locked = false;
};

//------------------------------------------------------------------------------

template <typename TContext, typename TPayloadList>
struct ControlLockT {
	using TransitionControl = TransitionControlT<TContext, TPayloadList>;

	inline ControlLockT(TransitionControl& control);
	inline ~ControlLockT();

	TransitionControl* const _control;
};

////////////////////////////////////////////////////////////////////////////////

template <typename...>
struct WrapState;

template <typename TContext, typename TPayloadList, typename THead>
struct WrapState<TContext, TPayloadList, THead> {
	using Type = _S<TContext, TPayloadList, THead>;
};

template <typename TContext, typename TPayloadList, typename THead, typename... TSubStates>
struct WrapState<TContext, TPayloadList, _C<TContext, TPayloadList, THead, TSubStates...>> {
	using Type = _C<TContext, TPayloadList, THead, TSubStates...>;
};

template <typename TContext, typename TPayloadList, typename THead, typename... TSubStates>
struct WrapState<TContext, TPayloadList, _O<TContext, TPayloadList, THead, TSubStates...>> {
	using Type = _O<TContext, TPayloadList, THead, TSubStates...>;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_utility.inl"
