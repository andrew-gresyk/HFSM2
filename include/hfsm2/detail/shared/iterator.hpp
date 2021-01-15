namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContainer>
class IteratorT {
public:
	using Container = TContainer;
	using Item		= typename Container::Item;

	template <typename T, Long NCapacity>
	friend class ArrayT;

private:
	HFSM2_INLINE IteratorT(Container& container,
						  const Long cursor)	  noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	HFSM2_INLINE bool operator != (const IteratorT<Container>& other) const noexcept;

	HFSM2_INLINE IteratorT& operator ++()		  noexcept;

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
class IteratorT<const TContainer> {
public:
	using Container = TContainer;
	using Item = typename Container::Item;

	template <typename T, Long NCapacity>
	friend class ArrayT;

private:
	HFSM2_INLINE IteratorT(const Container& container,
						  const Long cursor)	  noexcept
		: _container{container}
		, _cursor{cursor}
	{}

public:
	HFSM2_INLINE bool operator != (const IteratorT<const Container>& other) const noexcept;

	HFSM2_INLINE IteratorT& operator ++()		  noexcept;

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
