namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename, typename>
struct PreReactWrapperT;

//------------------------------------------------------------------------------

template <typename TRegion>
struct PreReactWrapperT<TRegion, TopDown> {
	using Region		= TRegion;
	using HeadState		= typename Region::HeadState;
	using SubStates		= typename Region::SubStates;
	using EventControl	= typename Region::EventControl;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(Region& region,
			EventControl& control,
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

	template <typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(Region& region,
			EventControl& control,
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

template <typename TRegion>
struct PreReactWrapperT<TRegion, BottomUp> {
	using Region		= TRegion;
	using HeadState		= typename Region::HeadState;
	using SubStates		= typename Region::SubStates;
	using EventControl	= typename Region::EventControl;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(Region& region,
			EventControl& control,
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

	template <typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(Region& region,
			EventControl& control,
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

template <typename, typename>
struct ReactWrapperT;

//------------------------------------------------------------------------------

template <typename TRegion>
struct ReactWrapperT<TRegion, TopDown> {
	using Region		= TRegion;
	using HeadState		= typename Region::HeadState;
	using SubStates		= typename Region::SubStates;
	using EventControl	= typename Region::EventControl;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(Region& region,
			EventControl& control,
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

	template <typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(Region& region,
			EventControl& control,
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

template <typename TRegion>
struct ReactWrapperT<TRegion, BottomUp> {
	using Region		= TRegion;
	using HeadState		= typename Region::HeadState;
	using SubStates		= typename Region::SubStates;
	using EventControl	= typename Region::EventControl;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(Region& region,
			EventControl& control,
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

	template <typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(Region& region,
			EventControl& control,
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

template <typename, typename>
struct PostReactWrapperT;

//------------------------------------------------------------------------------

template <typename TRegion>
struct PostReactWrapperT<TRegion, TopDown> {
	using Region		= TRegion;
	using HeadState		= typename Region::HeadState;
	using SubStates		= typename Region::SubStates;
	using EventControl	= typename Region::EventControl;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(Region& region,
			EventControl& control,
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

	template <typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(Region& region,
			EventControl& control,
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

template <typename TRegion>
struct PostReactWrapperT<TRegion, BottomUp> {
	using Region		= TRegion;
	using HeadState		= typename Region::HeadState;
	using SubStates		= typename Region::SubStates;
	using EventControl	= typename Region::EventControl;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(Region& region,
			EventControl& control,
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

	template <typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	TaskStatus
	execute(Region& region,
			EventControl& control,
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

template <typename, typename>
struct QueryWrapperT;

//------------------------------------------------------------------------------

template <typename TRegion>
struct QueryWrapperT<TRegion, TopDown> {
	using Region		= TRegion;
	using HeadState		= typename Region::HeadState;
	using SubStates		= typename Region::SubStates;
	using ConstControl	= typename Region::ConstControl;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	void
	execute(const Region& region,
			ConstControl& control,
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

	template <typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	void
	execute(const Region& region,
			ConstControl& control,
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

template <typename TRegion>
struct QueryWrapperT<TRegion, BottomUp> {
	using Region		= TRegion;
	using HeadState		= typename Region::HeadState;
	using SubStates		= typename Region::SubStates;
	using ConstControl	= typename Region::ConstControl;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	void
	execute(const Region& region,
			ConstControl& control,
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

	template <typename TEvent>
	static
	HFSM2_CONSTEXPR(14)
	void
	execute(const Region& region,
			ConstControl& control,
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
