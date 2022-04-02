namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::wideEntryGuard(GuardControl& control) noexcept {
	TF		 ::	   entryGuard(				control);
	A_<TR...>::wideEntryGuard(				control);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::wideEnter(PlanControl& control) noexcept {
	TF		 ::	   enter(			  control);
	A_<TR...>::wideEnter(			  control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::wideReenter(PlanControl& control) noexcept {
	TF		 ::	   Reenter(				control);
	A_<TR...>::wideReenter(				control);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::widePreUpdate(FullControl& control) noexcept {
	TF		 ::	   preUpdate(			  control);
	A_<TR...>::widePreUpdate(			  control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::wideUpdate(FullControl& control) noexcept {
	TF		 ::	   update(			   control);
	A_<TR...>::wideUpdate(			   control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::widePostUpdate(FullControl& control) noexcept {
	A_<TR...>::widePostUpdate(			   control);
	TF		 ::	   postUpdate(			   control);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::widePreReact(const TEvent& event,
							FullControl& control) noexcept
{
	TF		 ::	   preReact(			  event, control);
	A_<TR...>::widePreReact(			  event, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF, typename... TR>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::wideReact(const TEvent& event,
						 FullControl& control) noexcept
{
	TF		 ::	   react(			   event, control);
	A_<TR...>::wideReact(			   event, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF, typename... TR>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::widePostReact(const TEvent& event,
							 FullControl& control) noexcept
{
	A_<TR...>::widePostReact(			   event, control);
	TF		 ::	   postReact(			   event, control);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::wideExitGuard(GuardControl& control) noexcept {
	A_<TR...>::wideExitGuard(			   control);
	TF		 ::	   ExitGuard(			   control);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
A_<TF, TR...>::wideExit(PlanControl& control) noexcept {
	A_<TR...>::wideExit(			 control);
	TF		 ::	   exit(			 control);
}

////////////////////////////////////////////////////////////////////////////////

}
}
