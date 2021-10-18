namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContainer>
class IteratorT {
public:
	using Container = TContainer;
	using Item		= typename Container::Item;
	using Index		= typename Container::Index;

	template <typename, Long>
	friend class ArrayT;

private:
	HFSM2_CONSTEXPR(11)	IteratorT(Container& container,
								  const Index cursor)									  noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	HFSM2_CONSTEXPR(14)	bool operator != (const IteratorT& HFSM2_IF_ASSERT(other))	const noexcept	{
		HFSM2_ASSERT(&_container == &other._container);

		return _cursor != _container.limit();
	}

	HFSM2_CONSTEXPR(14)	IteratorT& operator ++()										  noexcept	{
		_cursor = _container.next(_cursor);

		return *this;
	}

	HFSM2_CONSTEXPR(14)		  Item& operator *()		  noexcept	{ return  _container[_cursor];	}
	HFSM2_CONSTEXPR(11)	const Item& operator *()	const noexcept	{ return  _container[_cursor];	}

	HFSM2_CONSTEXPR(14)		  Item* operator->()		  noexcept	{ return &_container[_cursor];	}
	HFSM2_CONSTEXPR(11)	const Item* operator->()	const noexcept	{ return &_container[_cursor];	}

private:
	Container& _container;

	Index _cursor;
};

//------------------------------------------------------------------------------

template <typename TContainer>
class IteratorT<const TContainer> {
public:
	using Container = TContainer;
	using Item		= typename Container::Item;
	using Index		= typename Container::Index;

	template <typename, Long>
	friend class ArrayT;

private:
	HFSM2_CONSTEXPR(11)	IteratorT(const Container& container,
								  const Index cursor)									  noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	HFSM2_CONSTEXPR(14)	bool operator != (const IteratorT& HFSM2_IF_ASSERT(other))	const noexcept	{
		HFSM2_ASSERT(&_container == &other._container);

		return _cursor != _container.limit();
	}

	HFSM2_CONSTEXPR(14)	IteratorT& operator ++()										  noexcept	{
		_cursor = _container.next(_cursor);

		return *this;
	}

	HFSM2_CONSTEXPR(11)	const Item& operator *()	const noexcept	{ return _container[_cursor];	}

	HFSM2_CONSTEXPR(11)	const Item* operator->()	const noexcept	{ return &operator *();			}

private:
	const Container& _container;

	Index _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}
