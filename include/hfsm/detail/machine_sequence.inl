namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TID, typename TA, typename TH, typename... TS>
void
_Q<TID, TA, TH, TS...>::deepGuard(FullControl& control) {
	Composite::deepGuard(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TA, typename TH, typename... TS>
Status
_Q<TID, TA, TH, TS...>::deepUpdate(FullControl& control) {
	Status status = Composite::deepUpdate(control);
	if (status.success) {
		auto plan = control.plan(HEAD_ID);

		while (const TaskTransition* const step = plan.next())
			if (step->origin)
			{
				control.changeTo(step->destination);
				plan.advance();
			}

		return {false,
				status.failure,
				status.innerTransition,
				status.outerTransition};
	} else
		return status;
}

//------------------------------------------------------------------------------

template <StateID TID, typename TA, typename TH, typename... TS>
template <typename TEvent>
void
_Q<TID, TA, TH, TS...>::deepReact(const TEvent& event,
								  FullControl& control)
{
	Composite::template deepReact<TEvent>(event, control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TA, typename TH, typename... TS>
void
_Q<TID, TA, TH, TS...>::deepExit(PlanControl& control) {
	Composite::deepExit(control);

	auto plan = control.plan(HEAD_ID);
	plan.clear();
}

////////////////////////////////////////////////////////////////////////////////

}
}
