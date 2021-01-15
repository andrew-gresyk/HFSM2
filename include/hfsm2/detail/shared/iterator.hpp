namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContainer>
class Iterator {
public:
	using Container = TContainer;
	using Item		= typename Container::Item;

	template <typename T, Long NCapacity>
	friend class Array;

private:
	HFSM2_INLINE Iterator(Container& container,
						  const Long cursor)	  noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	HFSM2_INLINE bool operator != (const Iterator<Container>& other) const noexcept;

	HFSM2_INLINE Iterator& operator ++()		  noexcept;

	HFSM2_INLINE	   Item& operator *()		  noexcept	{ return  _container[_cursor];	}
	HFSM2_INLINE const Item& operator *()	const noexcept	{ return  _container[_cursor];	}

	HFSM2_INLINE	   Item* operator->()		  noexcept	{ return &_container[_cursor];	}
	HFSM2_INLINE const Item* operator->()	const noexcept	{ return &_container[_cursor];	}

private:
	Container& _container;

	Long _cursor;
};

//------------------------------------------------------------------------------

template <typename TContainer>
class Iterator<const TContainer> {
public:
	using Container = TContainer;
	using Item = typename Container::Item;

	template <typename T, Long NCapacity>
	friend class Array;

private:
	HFSM2_INLINE Iterator(const Container& container,
						  const Long cursor)	  noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	HFSM2_INLINE bool operator != (const Iterator<const Container>& other) const noexcept;

	HFSM2_INLINE Iterator& operator ++()		  noexcept;

	HFSM2_INLINE const Item& operator *()	const noexcept	{ return _container[_cursor];	}

	HFSM2_INLINE const Item* operator->()	const noexcept	{ return &operator *();			}

private:
	const Container& _container;

	Long _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "iterator.inl"
