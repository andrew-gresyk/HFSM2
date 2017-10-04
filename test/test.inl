#include <algorithm>	// std::min()

////////////////////////////////////////////////////////////////////////////////

template <unsigned TCapacity>
void
Context::assertHistory(const Status(&reference)[TCapacity]) {
	const unsigned historySize = (unsigned) history.size();
	const unsigned referenceSize = hfsm::detail::count(reference);
	assert(historySize == referenceSize);

	for (unsigned i = 0; i < std::min(historySize, referenceSize); ++i) {
		HSFM_ASSERT_ONLY(const auto h = history[i]);
		HSFM_ASSERT_ONLY(const auto r = reference[i]);
		assert(h == r);
	}

	history.clear();
}

//------------------------------------------------------------------------------
//	helper functions to automagically record transition events

template <typename T, typename TControl>
void
changeTo(TControl& control, Context::History& history) {
	control.template changeTo<T>();
	history.push_back(Status{Event::Restart, std::type_index{typeid(T)}});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, typename TControl>
void
resume(TControl& control, Context::History& history) {
	control.template resume<T>();
	history.push_back(Status{Event::Resume, std::type_index{typeid(T)}});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, typename TControl>
void
schedule(TControl& control, Context::History& history) {
	control.template schedule<T>();
	history.push_back(Status{Event::Schedule, std::type_index{typeid(T)}});
}

////////////////////////////////////////////////////////////////////////////////
