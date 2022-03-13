namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
B_<TF, TR...>::widePreEntryGuard(Context& context) noexcept {
	TF		 ::	   preEntryGuard(context);
	B_<TR...>::widePreEntryGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
B_<TF, TR...>::widePreEnter(Context& context) noexcept {
	TF		 ::	   preEnter(context);
	B_<TR...>::widePreEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
B_<TF, TR...>::widePreReenter(Context& context) noexcept {
	TF		 ::	   preReenter(context);
	B_<TR...>::widePreReenter(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
B_<TF, TR...>::widePreUpdate(Context& context) noexcept {
	TF		 ::	   preUpdate(context);
	B_<TR...>::widePreUpdate(context);
}

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
B_<TF, TR...>::widePostUpdate(Context& context) noexcept {
	TF		 ::	   postUpdate(context);
	B_<TR...>::widePostUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
B_<TF, TR...>::widePreReact(const TEvent& event,
							Context& context) noexcept
{
	TF		 ::	   preReact(event, context);
	B_<TR...>::widePreReact(event, context);
}

template <typename TF, typename... TR>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
B_<TF, TR...>::widePostReact(const TEvent& event,
							Context& context) noexcept
{
	TF		 ::	   postReact(event, context);
	B_<TR...>::widePostReact(event, context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
B_<TF, TR...>::widePreExitGuard(Context& context) noexcept {
	TF		 ::	   preExitGuard(context);
	B_<TR...>::widePreExitGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
HFSM2_CONSTEXPR(14)
void
B_<TF, TR...>::widePostExit(Context& context) noexcept {
	TF		 ::	   postExit(context);
	B_<TR...>::widePostExit(context);
}

////////////////////////////////////////////////////////////////////////////////

}
}
