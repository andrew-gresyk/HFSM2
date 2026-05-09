#if HFSM2_PLANS_AVAILABLE()

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
CPlanT<TArgs>::Iterator::Iterator(const CPlanT& plan) noexcept
	: _plan{plan}
	, _curr{plan._planData.tasks.bounds(plan._regionId).first}
	, _next{next()}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
CPlanT<TArgs>::Iterator::operator bool() const noexcept {
	HFSM2_ASSERT(_curr  < CPlanT::TASK_CAPACITY ||
				 _curr == INVALID_LONG);

	return _curr < CPlanT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
CPlanT<TArgs>::Iterator::operator ++() noexcept {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
Long
CPlanT<TArgs>::Iterator::next() const noexcept {
	return _curr < CPlanT::TASK_CAPACITY ?
		_plan._planData.tasks.next(_curr) :
		INVALID_LONG;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
CPlanT<TArgs>::operator bool() const noexcept {
	const Bounds& bounds = _planData.tasks.bounds(_regionId);

	HFSM2_ASSERT(bounds.first < TASK_CAPACITY &&
				 bounds.last  < TASK_CAPACITY ||
				 bounds.last == INVALID_LONG);

	return bounds.first < TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
const typename CPlanT<TArgs>::Task&
CPlanT<TArgs>::first() const noexcept {
	const Bounds& bounds = _planData.tasks.bounds(_regionId);

	HFSM2_ASSERT(bounds.first < TASK_CAPACITY);

	return _planData.tasks[bounds.first];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
HFSM2_CONSTEXPR(14)
const typename CPlanT<TArgs>::Task&
CPlanT<TArgs>::last() const noexcept {
	const Bounds& bounds = _planData.tasks.bounds(_regionId);

	HFSM2_ASSERT(bounds.last < TASK_CAPACITY);

	return _planData.tasks[bounds.last];
}

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
