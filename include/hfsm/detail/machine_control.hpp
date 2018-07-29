#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext>
class ControlT {
	template <StateID, typename, typename, typename, typename>
	friend struct _S;

	template <StateID, typename, typename, typename, typename, typename...>
	friend struct _C;

	template <StateID, typename, typename, typename, typename, typename...>
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

	template <typename T>
	static constexpr bool contains() {
		return PayloadList::template contains<T>();
	}

	inline TransitionT() = default;

	inline TransitionT(const Type type_,
					   const StateID stateId_)
		: type(type_)
		, stateId(stateId_)
	{
		assert(type_ < Type::COUNT);
	}

	template <typename T,
			  typename = typename std::enable_if<contains<T>(), T>::type>
	inline TransitionT(const Type type_,
					   const StateID stateId_,
					   T* const payload_)
		: type(type_)
		, stateId(stateId_)
		, payload(payload_)
	{
		assert(type_ < Type::COUNT);
	}

	Type type = RESTART;
	StateID stateId = INVALID_STATE_ID;
	Payload payload;
};

template <typename TPayloadList>
using TransitionQueueT = ArrayView<TransitionT<TPayloadList>>;

//------------------------------------------------------------------------------

template <typename, typename, typename>
struct ControlLockT;

template <typename TContext, typename TStateList, typename TPayloadList>
class TransitionControlT final
	: public ControlT<TContext>
{
	using Context			= TContext;
	using Base				= ControlT<Context>;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using TransitionQueue	= TransitionQueueT<TPayloadList>;

	template <typename, typename, ShortIndex, typename>
	friend class _R;

	template <typename, typename, typename>
	friend struct ControlLockT;

private:
	inline TransitionControlT(Context& context,
							  TransitionQueue& requests,
							  LoggerInterface* const logger)
		: Base(context, logger)
		, _requests(requests)
	{}

	template <typename T>
	static constexpr LongIndex
	stateId()					{ return StateList::template index<T>();	}

public:
	using Lock				= ControlLockT<Context, StateList, PayloadList>;

	inline void changeTo(const StateID stateId);
	inline void resume	(const StateID stateId);
	inline void schedule(const StateID stateId);

	template <typename TState>
	inline void changeTo()						{ changeTo(stateId<TState>());	}

	template <typename TState>
	inline void resume()						{ resume  (stateId<TState>());	}

	template <typename TState>
	inline void schedule()						{ schedule(stateId<TState>());	}

	inline auto requestCount() const			{ return _requests.count();		}

private:
	TransitionQueue& _requests;
	bool _locked = false;
};

template <typename TContext, typename TStateList, typename TPayloadList>
struct ControlLockT {
	using Context			= TContext;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;
	using TransitionControl = TransitionControlT<Context, StateList, PayloadList>;

	inline ControlLockT(TransitionControl& control);
	inline ~ControlLockT();

	TransitionControl* const _control;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_control.inl"
