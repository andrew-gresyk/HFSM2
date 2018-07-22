#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 2)

template <typename T>
class ArrayView {
public:
	using Item = T;

	template <typename>
	friend class Iterator;

	enum {
		ITEM_ALIGNMENT	= alignof(Item[2]),
		VIEW_SIZE		= 4,
		OFFSET			= (VIEW_SIZE + ITEM_ALIGNMENT - 1) / ITEM_ALIGNMENT * ITEM_ALIGNMENT,
	};

protected:
	ArrayView(const LongIndex capacity);
	~ArrayView();

public:
	inline void clear()												{ _count = 0;			}

	inline LongIndex resize(const LongIndex count);

	template <typename TValue>
	inline LongIndex operator << (TValue&& value);

	inline		 auto& operator[] (const LongIndex i)				{ return get(i);		}
	inline const auto& operator[] (const LongIndex i) const			{ return get(i);		}

	inline LongIndex count() const									{ return _count;		}
	inline LongIndex capacity() const								{ return _capacity;		}

protected:
	inline LongIndex first() const									{ return 0;				}
	inline LongIndex limit() const									{ return _count;		}

	inline LongIndex prev(const LongIndex i) const					{ return i - 1;			}
	inline LongIndex next(const LongIndex i) const					{ return i + 1;			}

	inline		 Item& get(const LongIndex i);
	inline const Item& get(const LongIndex i) const;

private:
	// hacks
	inline		 auto* data()		{ return reinterpret_cast<		Item*>(((uintptr_t)this) + OFFSET);	}
	inline const auto* data() const	{ return reinterpret_cast<const Item*>(((uintptr_t)this) + OFFSET);	}

protected:
	LongIndex _count = 0;
	const LongIndex _capacity;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}

#include "lib_array_view.inl"
