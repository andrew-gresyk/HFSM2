namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

HFSM2_CONSTEXPR(11)
TaskStatus::TaskStatus(const Result result_,
					   const bool outerTransition_) noexcept
	: result         {result_         }
	, outerTransition{outerTransition_}
{}

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(11)
TaskStatus::operator bool() const noexcept	{
	return result != Result::NONE || outerTransition;
}

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(14)
void
TaskStatus::clear() noexcept {
	result = Result::NONE;
	outerTransition = false;
}

////////////////////////////////////////////////////////////////////////////////

HFSM2_CONSTEXPR(14)
TaskStatus
operator | (TaskStatus& lhs,
			const TaskStatus rhs) noexcept
{
	const TaskStatus::Result result = lhs.result > rhs.result ?
		lhs.result : rhs.result;

	return TaskStatus{result, lhs.outerTransition || rhs.outerTransition};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

HFSM2_CONSTEXPR(14)
TaskStatus&
operator |= (TaskStatus& lhs,
			 const TaskStatus rhs) noexcept
{
	const TaskStatus::Result result = lhs.result > rhs.result ?
										  lhs.result : rhs.result;

	lhs = TaskStatus{result, lhs.outerTransition || rhs.outerTransition};

	return lhs;
}

////////////////////////////////////////////////////////////////////////////////

}
}
