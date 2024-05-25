namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class EventControlT final
	: public FullControlT<TArgs>
{
	template <typename, typename>
	friend class R_;

	template <typename, typename>
	friend struct PreReactWrapperT;

	template <typename, typename>
	friend struct ReactWrapperT;

	template <typename, typename>
	friend struct PostReactWrapperT;

	using FullControl	= FullControlT<TArgs>;

	using FullControl::FullControl;

public:
	using FullControl::context;

	/// @brief Stops processing of the current event down the hierarchy
	/// @see `Config::BottomUpReactions`
	HFSM2_CONSTEXPR(14)	void consumeEvent()								noexcept	{ _consumed = true;	}

private:
	bool _consumed = false;
};

////////////////////////////////////////////////////////////////////////////////

}
}
