namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TF_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::wideEntryGuard(GuardControl& control) noexcept {
	First::	   entryGuard(control);
	Rest ::wideEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TF_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::wideEnter(PlanControl& control) noexcept {
	First::	   enter(control);
	Rest ::wideEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::wideReenter(PlanControl& control) noexcept {
	First::	   reenter(control);
	Rest ::wideReenter(control);
}

//------------------------------------------------------------------------------

template <typename TF_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::widePreUpdate(FullControl& control) noexcept {
	First::	   preUpdate(control);
	Rest ::widePreUpdate(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::wideUpdate(FullControl& control) noexcept {
	First::	   update(control);
	Rest ::wideUpdate(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::widePostUpdate(FullControl& control) noexcept {
	Rest ::widePostUpdate(control);
	First::	   postUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TF_, typename... TR_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::widePreReact(const TEvent& event,
							  FullControl& control) noexcept
{
	First::	   preReact(event, control);
	Rest ::widePreReact(event, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF_, typename... TR_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::wideReact(const TEvent& event,
						   FullControl& control) noexcept
{
	First::	   react(event, control);
	Rest ::wideReact(event, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF_, typename... TR_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::widePostReact(const TEvent& event,
							   FullControl& control) noexcept
{
	Rest ::widePostReact(event, control);
	First::	   postReact(event, control);
}

//------------------------------------------------------------------------------

template <typename TF_, typename... TR_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::wideQuery(TEvent& event,
						   ConstControl& control) const noexcept
{
	First::	   query(event, control);
	Rest ::wideQuery(event, control);
}

//------------------------------------------------------------------------------

template <typename TF_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::wideExitGuard(GuardControl& control) noexcept {
	Rest ::wideExitGuard(control);
	First::	   exitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TF_, typename... TR_>
HFSM2_CONSTEXPR(14)
void
A_<TF_, TR_...>::wideExit(PlanControl& control) noexcept {
	Rest ::wideExit(control);
	First::	   exit(control);
}

////////////////////////////////////////////////////////////////////////////////

}
}
