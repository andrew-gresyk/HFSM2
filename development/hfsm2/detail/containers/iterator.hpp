namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <
	typename TContainer,
	typename TItem,
	typename TIndex
>
class IteratorT {
public:
	using Container = TContainer;
	using Item		= TItem;
	using Index		= TIndex;

	template <typename, Long>
	friend class DynamicArrayT;

	template <typename, Long>
	friend class StaticArrayT;

private:
	HFSM2_CONSTEXPR(11)	IteratorT(Container& container,
								  const Index cursor)					noexcept
		: _container{container}
		, _cursor   {cursor   }
	{}

public:
	HFSM2_CONSTEXPR(14)	bool operator != (const IteratorT& other) const noexcept	{
		HFSM2_ASSERT(&_container == &other._container);

		return _cursor != other._cursor;
	}

	HFSM2_CONSTEXPR(14)	IteratorT& operator ++()						noexcept	{
		_cursor = _container.next(_cursor);

		return *this;
	}

	HFSM2_CONSTEXPR(14)		  Item& operator *()						noexcept	{ return  _container[_cursor];	}
	HFSM2_CONSTEXPR(11)	const Item& operator *()				  const noexcept	{ return  _container[_cursor];	}

	HFSM2_CONSTEXPR(14)		  Item* operator->()						noexcept	{ return &_container[_cursor];	}
	HFSM2_CONSTEXPR(11)	const Item* operator->()				  const noexcept	{ return &_container[_cursor];	}

private:
	Container& _container;

	Index _cursor;
};

//------------------------------------------------------------------------------

template <
	typename TContainer,
	typename TItem,
	typename TIndex
>
class IteratorT<const TContainer, TItem, TIndex> {
public:
	using Container = TContainer;
	using Item		= TItem;
	using Index		= TIndex;

	template <typename, Long>
	friend class DynamicArrayT;

	template <typename, Long>
	friend class StaticArrayT;

private:
	HFSM2_CONSTEXPR(11)	IteratorT(const Container& container,
								  const Index cursor)					noexcept
		: _container{container}
		, _cursor   {cursor   }
	{}

public:
	HFSM2_CONSTEXPR(14)	bool operator != (const IteratorT& other) const noexcept	{
		HFSM2_ASSERT(&_container == &other._container);

		return _cursor != other._cursor;
	}

	HFSM2_CONSTEXPR(14)	IteratorT& operator ++()						noexcept	{
		_cursor = _container.next(_cursor);

		return *this;
	}

	HFSM2_CONSTEXPR(11)	const Item& operator *()				  const noexcept	{ return _container[_cursor];	}

	HFSM2_CONSTEXPR(11)	const Item* operator->()				  const noexcept	{ return &operator *();			}

private:
	const Container& _container;

	Index _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}
