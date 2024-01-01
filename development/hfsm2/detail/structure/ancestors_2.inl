namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TF_>
HFSM2_CONSTEXPR(14)
void
A_<TF_>::wideEntryGuard(GuardControl& control) noexcept {
	First::	   entryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TF_>
HFSM2_CONSTEXPR(14)
void
A_<TF_>::wideEnter(PlanControl& control) noexcept {
	First::	   enter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF_>
HFSM2_CONSTEXPR(14)
void
A_<TF_>::wideReenter(PlanControl& control) noexcept {
	First::	   reenter(control);
}

//------------------------------------------------------------------------------

template <typename TF_>
HFSM2_CONSTEXPR(14)
void
A_<TF_>::widePreUpdate(FullControl& control) noexcept {
	First::	   preUpdate(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF_>
HFSM2_CONSTEXPR(14)
void
A_<TF_>::wideUpdate(FullControl& control) noexcept {
	First::	   update(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF_>
HFSM2_CONSTEXPR(14)
void
A_<TF_>::widePostUpdate(FullControl& control) noexcept {
	First::	   postUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TF_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF_>::widePreReact(const TEvent& event,
					  EventControl& control) noexcept
{
	First::	   preReact(event, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF_>::wideReact(const TEvent& event,
				   EventControl& control) noexcept
{
	First::	   react(event, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF_>::widePostReact(const TEvent& event,
					   EventControl& control) noexcept
{
	First::	   postReact(event, control);
}

//------------------------------------------------------------------------------

template <typename TF_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF_>::wideQuery(TEvent& event,
				   ConstControl& control) const noexcept
{
	First::	   query(event, control);
}

//------------------------------------------------------------------------------

template <typename TF_>
HFSM2_CONSTEXPR(14)
void
A_<TF_>::wideExitGuard(GuardControl& control) noexcept {
	First::	   exitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TF_>
HFSM2_CONSTEXPR(14)
void
A_<TF_>::wideExit(PlanControl& control) noexcept {
	First::	   exit(control);
}

////////////////////////////////////////////////////////////////////////////////

}
}
