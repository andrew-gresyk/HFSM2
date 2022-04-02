namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TF>
HFSM2_CONSTEXPR(14)
void
A_<TF>::wideEntryGuard(GuardControl& control) noexcept {
	TF::	entryGuard(				 control);
}

//------------------------------------------------------------------------------

template <typename TF>
HFSM2_CONSTEXPR(14)
void
A_<TF>::wideEnter(PlanControl& control) noexcept {
	TF::	enter(			   control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF>
HFSM2_CONSTEXPR(14)
void
A_<TF>::wideReenter(PlanControl& control) noexcept {
	TF::	reenter(			 control);
}

//------------------------------------------------------------------------------

template <typename TF>
HFSM2_CONSTEXPR(14)
void
A_<TF>::widePreUpdate(FullControl& control) noexcept {
	TF::	preUpdate(			   control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF>
HFSM2_CONSTEXPR(14)
void
A_<TF>::wideUpdate(FullControl& control) noexcept {
	TF::	update(				control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF>
HFSM2_CONSTEXPR(14)
void
A_<TF>::widePostUpdate(FullControl& control) noexcept {
	TF::	postUpdate(				control);
}

//------------------------------------------------------------------------------

template <typename TF>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF>::widePreReact(const TEvent& event,
					 FullControl& control) noexcept
{
	TF::	preReact(			   event, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF>::wideReact(const TEvent& event,
				  FullControl& control) noexcept
{
	TF::	react(				event, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF>::widePostReact(const TEvent& event,
					  FullControl& control) noexcept
{
	TF::	postReact(				event, control);
}

//------------------------------------------------------------------------------

template <typename TF>
HFSM2_CONSTEXPR(14)
void
A_<TF>::wideExitGuard(GuardControl& control) noexcept {
	TF::	exitGuard(				control);
}

//------------------------------------------------------------------------------

template <typename TF>
HFSM2_CONSTEXPR(14)
void
A_<TF>::wideExit(PlanControl& control) noexcept {
	TF::	exit(			  control);
}

////////////////////////////////////////////////////////////////////////////////

}
}
