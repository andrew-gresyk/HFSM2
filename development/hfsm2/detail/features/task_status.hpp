namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

struct TaskStatus final {
	enum Result {
		NONE,
		SUCCESS,
		FAILURE
	};

	Result result = Result::NONE;
	bool outerTransition = false;

	HFSM2_CONSTEXPR(11)	TaskStatus(const Result result_ = Result::NONE,
								   const bool outerTransition_ = false)			noexcept;

	HFSM2_CONSTEXPR(11)	explicit operator bool()						  const noexcept;

	HFSM2_CONSTEXPR(14)	void clear()											noexcept;
};

//------------------------------------------------------------------------------

HFSM2_CONSTEXPR(14) TaskStatus  operator |  (TaskStatus& l, const TaskStatus r)	noexcept;
HFSM2_CONSTEXPR(14) TaskStatus& operator |= (TaskStatus& l, const TaskStatus r)	noexcept;

////////////////////////////////////////////////////////////////////////////////

}
}

#include "task_status.inl"
