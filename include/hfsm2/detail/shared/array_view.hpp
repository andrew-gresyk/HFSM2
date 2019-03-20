#pragma once

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 4)

template <typename T>
class ArrayView {
public:
	using Item = T;

	template <typename>
	friend class Iterator;

	static constexpr auto ITEM_ALIGNMENT = alignof(Item[2]);
	static constexpr auto VIEW_SIZE		 = 4;
	static constexpr auto OFFSET		 = (VIEW_SIZE + ITEM_ALIGNMENT - 1) / ITEM_ALIGNMENT * ITEM_ALIGNMENT;

protected:
	ArrayView(const LongIndex capacity);
	~ArrayView();

public:
	HFSM_INLINE void clear()										{ _count = 0;		}

	HFSM_INLINE LongIndex resize(const LongIndex count);

	template <typename TValue>
	HFSM_INLINE LongIndex operator << (TValue&& value);

	HFSM_INLINE		  Item& operator[] (const LongIndex i)			{ return get(i);	}
	HFSM_INLINE const Item& operator[] (const LongIndex i) const	{ return get(i);	}

	HFSM_INLINE LongIndex count() const								{ return _count;	}
	HFSM_INLINE LongIndex capacity() const							{ return _capacity;	}

protected:
	HFSM_INLINE LongIndex first() const								{ return 0;			}
	HFSM_INLINE LongIndex limit() const								{ return _count;	}

	HFSM_INLINE LongIndex prev(const LongIndex i) const				{ return i - 1;		}
	HFSM_INLINE LongIndex next(const LongIndex i) const				{ return i + 1;		}

	HFSM_INLINE		  Item& get(const LongIndex i);
	HFSM_INLINE const Item& get(const LongIndex i) const;

protected:
	HFSM_INLINE		  Item* data()		 { return reinterpret_cast<		 Item*>(((uintptr_t)this) + OFFSET); }
	HFSM_INLINE const Item* data() const { return reinterpret_cast<const Item*>(((uintptr_t)this) + OFFSET); }

protected:
	LongIndex _count = 0;
	const LongIndex _capacity;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}

#include "array_view.inl"
