namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename, typename, typename >
struct PreReactWrapperT;

//------------------------------------------------------------------------------

template <typename THeadState, typename TSubStates>
struct PreReactWrapperT<THeadState, TSubStates, TopDown> {
	using HeadState = THeadState;
	using SubStates = TSubStates;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TRegion, typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(TRegion& region,
			typename TRegion::EventControl& control,
			const TEvent& event,
			const Prong active) noexcept
	{
		if (!control._consumed) {
			HeadState& headState = static_cast<HeadState&>(region);
			SubStates& subStates = static_cast<SubStates&>(region);

			const TaskStatus h =
				headState.deepPreReact(control, event);
			HFSM2_IF_PLANS(region.headStatus(control) |= h);

			if (!control._consumed)
				HFSM2_IF_PLANS(region.subStatus(control) |=)
					subStates.widePreReact(control, event, active);

			return h;
		} else
			return TaskStatus{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TRegion, typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(TRegion& region,
			typename TRegion::EventControl& control,
			const TEvent& event) noexcept
	{
		if (!control._consumed) {
			HeadState& headState = static_cast<HeadState&>(region);
			SubStates& subStates = static_cast<SubStates&>(region);

			const TaskStatus h =
				headState.deepPreReact(control, event);
			HFSM2_IF_PLANS(region.headStatus(control) |= h);

			if (!control._consumed)
				HFSM2_IF_PLANS(region.subStatus(control) |=)
					subStates.widePreReact(control, event);

			return h;
		} else
			return TaskStatus{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

//------------------------------------------------------------------------------

template <typename THeadState, typename TSubStates>
struct PreReactWrapperT<THeadState, TSubStates, BottomUp> {
	using HeadState = THeadState;
	using SubStates = TSubStates;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TRegion, typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(TRegion& region,
			typename TRegion::EventControl& control,
			const TEvent& event,
			const Prong active) noexcept
	{
		if (!control._consumed) {
			HeadState& headState = static_cast<HeadState&>(region);
			SubStates& subStates = static_cast<SubStates&>(region);

			const TaskStatus h =
				subStates.widePreReact(control, event, active);
			HFSM2_IF_PLANS(region. subStatus(control) |= h);

			if (!control._consumed)
				HFSM2_IF_PLANS(region.headStatus(control) |=)
					headState.deepPreReact(control, event);

			return h;
		} else
			return TaskStatus{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TRegion, typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(TRegion& region,
			typename TRegion::EventControl& control,
			const TEvent& event) noexcept
	{
		if (!control._consumed) {
			HeadState& headState = static_cast<HeadState&>(region);
			SubStates& subStates = static_cast<SubStates&>(region);

			const TaskStatus h =
				subStates.widePreReact(control, event);
			HFSM2_IF_PLANS(region. subStatus(control) |= h);

			if (!control._consumed)
				HFSM2_IF_PLANS(region.headStatus(control) |=)
					headState.deepPreReact(control, event);

			return h;
		} else
			return TaskStatus{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

template <typename, typename, typename >
struct ReactWrapperT;

//------------------------------------------------------------------------------

template <typename THeadState, typename TSubStates>
struct ReactWrapperT<THeadState, TSubStates, TopDown> {
	using HeadState = THeadState;
	using SubStates = TSubStates;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TRegion, typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(TRegion& region,
			typename TRegion::EventControl& control,
			const TEvent& event,
			const Prong active) noexcept
	{
		if (!control._consumed) {
			HeadState& headState = static_cast<HeadState&>(region);
			SubStates& subStates = static_cast<SubStates&>(region);

			const TaskStatus h =
				headState.deepReact(control, event);
			HFSM2_IF_PLANS(region.headStatus(control) |= h);

			if (!control._consumed)
				HFSM2_IF_PLANS(region. subStatus(control) |=)
					subStates.wideReact(control, event, active);

			return h;
		} else
			return TaskStatus{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TRegion, typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(TRegion& region,
			typename TRegion::EventControl& control,
			const TEvent& event) noexcept
	{
		if (!control._consumed) {
			HeadState& headState = static_cast<HeadState&>(region);
			SubStates& subStates = static_cast<SubStates&>(region);

			const TaskStatus h =
				headState.deepReact(control, event);
			HFSM2_IF_PLANS(region.headStatus(control) |= h);

			if (!control._consumed)
				HFSM2_IF_PLANS(region. subStatus(control) |=)
					subStates.wideReact(control, event);

			return h;
		} else
			return TaskStatus{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

//------------------------------------------------------------------------------

template <typename THeadState, typename TSubStates>
struct ReactWrapperT<THeadState, TSubStates, BottomUp> {
	using HeadState = THeadState;
	using SubStates = TSubStates;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TRegion, typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(TRegion& region,
			typename TRegion::EventControl& control,
			const TEvent& event,
			const Prong active) noexcept
	{
		if (!control._consumed) {
			HeadState& headState = static_cast<HeadState&>(region);
			SubStates& subStates = static_cast<SubStates&>(region);

			const TaskStatus h =
				subStates.wideReact(control, event, active);
			HFSM2_IF_PLANS(region. subStatus(control) |= h);

			if (!control._consumed)
				HFSM2_IF_PLANS(region.headStatus(control) |=)
					headState.deepReact(control, event);

			return h;
		} else
			return TaskStatus{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TRegion, typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(TRegion& region,
			typename TRegion::EventControl& control,
			const TEvent& event) noexcept
	{
		if (!control._consumed) {
			HeadState& headState = static_cast<HeadState&>(region);
			SubStates& subStates = static_cast<SubStates&>(region);

			const TaskStatus h =
				subStates.wideReact(control, event);
			HFSM2_IF_PLANS(region. subStatus(control) |= h);

			if (!control._consumed)
				HFSM2_IF_PLANS(region.headStatus(control) |=)
					headState.deepReact(control, event);

			return h;
		} else
			return TaskStatus{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

template <typename, typename, typename >
struct PostReactWrapperT;

//------------------------------------------------------------------------------

template <typename THeadState, typename TSubStates>
struct PostReactWrapperT<THeadState, TSubStates, TopDown> {
	using HeadState = THeadState;
	using SubStates = TSubStates;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TRegion, typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(TRegion& region,
			typename TRegion::EventControl& control,
			const TEvent& event,
			const Prong active) noexcept
	{
		if (!control._consumed) {
			HeadState& headState = static_cast<HeadState&>(region);
			SubStates& subStates = static_cast<SubStates&>(region);

			HFSM2_IF_PLANS(region. subStatus(control) |=)
				subStates.widePostReact(control, event, active);

			if (control._consumed == false) {
				const TaskStatus h =
					headState.deepPostReact(control, event);
				HFSM2_IF_PLANS(region.headStatus(control) |= h);

				return h;
			}
		}

		return TaskStatus{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TRegion, typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(TRegion& region,
			typename TRegion::EventControl& control,
			const TEvent& event) noexcept
	{
		if (!control._consumed) {
			HeadState& headState = static_cast<HeadState&>(region);
			SubStates& subStates = static_cast<SubStates&>(region);

			HFSM2_IF_PLANS(region. subStatus(control) |=)
				subStates.widePostReact(control, event);

			if (control._consumed == false) {
				const TaskStatus h =
					headState.deepPostReact(control, event);
				HFSM2_IF_PLANS(region.headStatus(control) |= h);

				return h;
			}
		}

		return TaskStatus{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

//------------------------------------------------------------------------------

template <typename THeadState, typename TSubStates>
struct PostReactWrapperT<THeadState, TSubStates, BottomUp> {
	using HeadState = THeadState;
	using SubStates = TSubStates;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TRegion, typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(TRegion& region,
			typename TRegion::EventControl& control,
			const TEvent& event,
			const Prong active) noexcept
	{
		if (!control._consumed) {
			HeadState& headState = static_cast<HeadState&>(region);
			SubStates& subStates = static_cast<SubStates&>(region);

			HFSM2_IF_PLANS(region.headStatus(control) |=)
				headState.deepPostReact(control, event);

			if (control._consumed == false) {
				const TaskStatus h =
					subStates.widePostReact(control, event, active);
				HFSM2_IF_PLANS(region. subStatus(control) |= h);

				return h;
			}
		}

		return TaskStatus{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TRegion, typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(TRegion& region,
			typename TRegion::EventControl& control,
			const TEvent& event) noexcept
	{
		if (!control._consumed) {
			HeadState& headState = static_cast<HeadState&>(region);
			SubStates& subStates = static_cast<SubStates&>(region);

			HFSM2_IF_PLANS(region.headStatus(control) |=)
				headState.deepPostReact(control, event);

			if (control._consumed == false) {
				const TaskStatus h =
					subStates.widePostReact(control, event);
				HFSM2_IF_PLANS(region. subStatus(control) |= h);

				return h;
			}
		}

		return TaskStatus{};
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

template <typename, typename, typename>
struct QueryWrapperT;

//------------------------------------------------------------------------------

template <typename THeadState, typename TSubStates>
struct QueryWrapperT<THeadState, TSubStates, TopDown> {
	using HeadState = THeadState;
	using SubStates = TSubStates;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TRegion, typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	void
	execute(const TRegion& region,
			typename TRegion::ConstControl& control,
			TEvent& event,
			const Prong active) noexcept
	{
		const HeadState& headState = static_cast<const HeadState&>(region);
		const SubStates& subStates = static_cast<const SubStates&>(region);

		if (!control._consumed)
			headState.deepQuery(control, event);

		if (!control._consumed)
			subStates.wideQuery(control, event, active);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TRegion, typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	void
	execute(const TRegion& region,
			typename TRegion::ConstControl& control,
			TEvent& event) noexcept
	{
		const HeadState& headState = static_cast<const HeadState&>(region);
		const SubStates& subStates = static_cast<const SubStates&>(region);

		if (!control._consumed)
			headState.deepQuery(control, event);

		if (!control._consumed)
			subStates.wideQuery(control, event);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

//------------------------------------------------------------------------------

template <typename THeadState, typename TSubStates>
struct QueryWrapperT<THeadState, TSubStates, BottomUp> {
	using HeadState = THeadState;
	using SubStates = TSubStates;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TRegion, typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	void
	execute(const TRegion& region,
			typename TRegion::ConstControl& control,
			TEvent& event,
			const Prong active) noexcept
	{
		const HeadState& headState = static_cast<const HeadState&>(region);
		const SubStates& subStates = static_cast<const SubStates&>(region);

		if (!control._consumed)
			subStates.wideQuery(control, event, active);

		if (!control._consumed)
			headState.deepQuery(control, event);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TRegion, typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	void
	execute(const TRegion& region,
			typename TRegion::ConstControl& control,
			TEvent& event) noexcept
	{
		const HeadState& headState = static_cast<const HeadState&>(region);
		const SubStates& subStates = static_cast<const SubStates&>(region);

		if (!control._consumed)
			subStates.wideQuery(control, event);

		if (!control._consumed)
			headState.deepQuery(control, event);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

}
}
