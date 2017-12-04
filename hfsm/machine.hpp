#pragma once

#include "detail/array.hpp"
#include "detail/hash_table.hpp"
#include "detail/type_info.hpp"

#include <limits>
#include <type_traits>

namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext, unsigned TMaxSubstitutions = 4>
class M {

	using TypeInfo = detail::TypeInfo;

	template <typename T, unsigned TCapacity>
	using Array = detail::Array<T, TCapacity>;

	template <typename T>
	using ArrayView = detail::ArrayView<T>;

	template <typename TKey, typename TValue, unsigned TCapacity, typename THasher = std::hash<TKey>>
	using HashTable = detail::HashTable<TKey, TValue, TCapacity, THasher>;

	//----------------------------------------------------------------------

#pragma region Utility

public:
	using Context = TContext;
	class Control;

private:
	using Index = unsigned char;
	enum : Index { INVALID_INDEX = std::numeric_limits<Index>::max() };

	enum { MaxSubstitutions = TMaxSubstitutions };

	//----------------------------------------------------------------------

	struct Parent {
		#pragma pack(push, 1)
			Index fork  = INVALID_INDEX;
			Index prong = INVALID_INDEX;
		#pragma pack(pop)

		HSFM_DEBUG_ONLY(TypeInfo forkType);
		HSFM_DEBUG_ONLY(TypeInfo prongType);

		inline Parent() = default;

		inline Parent(const Index fork_,
					  const Index prong_,
					  const TypeInfo HSFM_DEBUG_ONLY(forkType_),
					  const TypeInfo HSFM_DEBUG_ONLY(prongType_))
			: fork(fork_)
			, prong(prong_)
		#ifdef _DEBUG
			, forkType(forkType_)
			, prongType(prongType_)
		#endif
		{}

		inline explicit operator bool() const { return fork != INVALID_INDEX && prong != INVALID_INDEX; }
	};

	using Parents = ArrayView<Parent>;

	//----------------------------------------------------------------------

	struct StateRegistry {
	public:
		virtual unsigned add(const TypeInfo stateType) = 0;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <unsigned TCapacity>
	class StateRegistryT
		: public StateRegistry
	{
		enum { Capacity = TCapacity };

		using TypeToIndex = HashTable<TypeInfo::Native, unsigned, Capacity>;

	public:
		inline unsigned operator[] (const TypeInfo stateType) const { return *_typeToIndex.find(*stateType); }

		virtual unsigned add(const TypeInfo stateType) override;

	private:
		TypeToIndex _typeToIndex;
	};

	//----------------------------------------------------------------------

	struct Fork {
		#pragma pack(push, 1)
			Index self		= INVALID_INDEX;
			Index active	= INVALID_INDEX;
			Index resumable = INVALID_INDEX;
			Index requested = INVALID_INDEX;
		#pragma pack(pop)

		HSFM_DEBUG_ONLY(const TypeInfo type);
		HSFM_ASSERT_ONLY(TypeInfo activeType);
		HSFM_ASSERT_ONLY(TypeInfo resumableType);
		HSFM_ASSERT_ONLY(TypeInfo requestedType);

		Fork(const Index index, const TypeInfo type_);
	};
	using ForkPointers = ArrayView<Fork*>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename T>
	struct ForkT
		: Fork
	{
		ForkT(const Index index,
			  const Parent parent,
			  Parents& forkParents)
			: Fork(index, TypeInfo::get<T>())
		{
			forkParents[index] = parent;
		}
	};

	//----------------------------------------------------------------------

	struct Transition {
		enum Type {
			Remain,
			Restart,
			Resume,
			Schedule,

			COUNT
		};

		Type type = Restart;
		TypeInfo stateType;

		inline Transition() = default;

		inline Transition(const Type type_, const TypeInfo stateType_)
			: type(type_)
			, stateType(stateType_)
		{
			assert(type_ < Type::COUNT);
		}
	};
	using TransitionQueue = ArrayView<Transition>;

	//----------------------------------------------------------------------
	// shortened class names
	template <typename>
	struct _S;

	template <typename, typename...>
	struct _C;

	template <typename, typename...>
	struct _O;

	template <typename>
	class _R;

	//----------------------------------------------------------------------

	template <typename... TS>
	struct WrapState;

	template <typename T>
	struct WrapState<T> {
		using Type = _S<T>;
	};

	template <typename T>
	struct WrapState<_S<T>> {
		using Type = _S<T>;
	};

	template <typename T, typename... TS>
	struct WrapState<_C<T, TS...>> {
		using Type = _C<T, TS...>;
	};

	template <typename T, typename... TS>
	struct WrapState<_O<T, TS...>> {
		using Type = _O<T, TS...>;
	};

#pragma endregion

	//----------------------------------------------------------------------

#pragma region Injections

public:

	class Bare {
		template <typename...>
		friend struct _B;

	protected:
		using Control	 = typename M::Control;
		using Context	 = typename M::Context;
		using Transition = typename M::Transition;

	public:
		inline void preSubstitute(Context&) const		{}
		inline void preEnter(Context&)					{}
		inline void preUpdate(Context&)					{}
		inline void preTransition(Context&) const		{}
		template <typename TEvent>
		inline void preReact(const TEvent&, Context&)	{}
		inline void postLeave(Context&)					{}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
private:

	template <typename...>
	struct _B;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TInjection, typename... TRest>
	struct _B<TInjection, TRest...>
		: public TInjection
		, public _B<TRest...>
	{
		inline void widePreSubstitute(Context& context) const;
		inline void widePreEnter(Context& context);
		inline void widePreUpdate(Context& context);
		inline void widePreTransition(Context& context) const;
		template <typename TEvent>
		inline void widePreReact(const TEvent& event, Context& context);
		inline void widePostLeave(Context& context);
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TInjection>
	struct _B<TInjection>
		: public TInjection
	{
		inline void substitute(Control&, Context) const			{}
		inline void enter(Context)								{}
		inline void update(Context)								{}
		inline void transition(Control&, Context) const			{}
		template <typename TEvent>
		inline void react(const TEvent&, Control&, Context&)	{}
		inline void leave(Context)								{}

		inline void widePreSubstitute(Context& context) const;
		inline void widePreEnter(Context& context);
		inline void widePreUpdate(Context& context);
		inline void widePreTransition(Context& context) const;
		template <typename TEvent>
		inline void widePreReact(const TEvent& event, Context& context);
		inline void widePostLeave(Context& context);
	};

#pragma endregion

	//----------------------------------------------------------------------

#pragma region State

	template <typename T>
	struct _S {
		using Client = T;

		enum {
			ReverseDepth = 1,
			DeepWidth	 = 0,
			StateCount	 = 1,
			ForkCount	 = 0,
			ProngCount	 = 0,
			Width		 = 1,
		};

		_S(StateRegistry& stateRegistry,
		   const Parent parent,
		   Parents& stateParents,
		   Parents& forkParents,
		   ForkPointers& forkPointers);

		inline void deepEnterInitial(Context& context);

		inline void deepForwardRequest(const enum Transition::Type)				{}
		inline void deepRequestRemain()											{}
		inline void deepRequestRestart()										{}
		inline void deepRequestResume()											{}

		inline void deepForwardSubstitute(Control&, Context&) const				{}
		inline bool deepSubstitute(Control& control, Context& context) const;

		inline void deepChangeToRequested(Context&)	{}
		inline void deepEnter(Context& context);
		inline void deepLeave(Context& context);

		inline bool deepUpdateAndTransition(Control& control, Context& context);
		inline void deepUpdate(Context& context);
		
		template <typename TEvent>
		inline void deepReact(const TEvent& event, Control& control, Context& context);

		Client _client;

		HSFM_DEBUG_ONLY(const TypeInfo _type = TypeInfo::get<Client>());
	};

#pragma endregion

	//----------------------------------------------------------------------

#pragma region Composite

	template <typename T, typename... TS>
	struct _C final
		: public ForkT<T>
	{
		using State = _S<T>;
		using Client = typename State::Client;

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#pragma region Substates

		template <unsigned TN, typename...>
		struct Sub;

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		template <unsigned TN, typename TI, typename... TR>
		struct Sub<TN, TI, TR...> {
			using Initial = typename WrapState<TI>::Type;
			using Remaining = Sub<TN + 1, TR...>;

			enum {
				ProngIndex	 = TN,
				ReverseDepth = hfsm::detail::Max<Initial::ReverseDepth, Remaining::ReverseDepth>::Value,
				DeepWidth	 = hfsm::detail::Max<Initial::DeepWidth, Remaining::DeepWidth>::Value,
				StateCount	 = Initial::StateCount + Remaining::StateCount,
				ForkCount	 = Initial::ForkCount  + Remaining::ForkCount,
				ProngCount	 = Initial::ProngCount + Remaining::ProngCount,
			};

			Sub(StateRegistry& stateRegistry,
				const Index fork,
				Parents& stateParents,
				Parents& forkParents,
				ForkPointers& forkPointers);

			inline void wideEnterInitial(Context& context);

			inline void wideForwardRequest(const unsigned prong, const enum Transition::Type transition);
			inline void wideRequestRemain();
			inline void wideRequestRestart();
			inline void wideRequestResume(const unsigned prong);

			inline void wideForwardSubstitute(const unsigned prong, Control& control, Context& context) const;
			inline void wideSubstitute(const unsigned prong, Control& control, Context& context) const;

			inline void wideChangeTo(const unsigned prong, Context& context);
			inline void wideEnter(const unsigned prong, Context& context);
			inline void wideLeave(const unsigned prong, Context& context);

			inline bool wideUpdateAndTransition(const unsigned prong, Control& control, Context& context);
			inline void wideUpdate(const unsigned prong, Context& context);

			template <typename TEvent>
			inline void wideReact(const unsigned prong, const TEvent& event, Control& control, Context& context);

			Initial initial;
			Remaining remaining;
		};

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		template <unsigned TN, typename TI>
		struct Sub<TN, TI> {
			using Initial = typename WrapState<TI>::Type;

			enum {
				ProngIndex	 = TN,
				ReverseDepth = Initial::ReverseDepth,
				DeepWidth	 = hfsm::detail::Max<1, Initial::DeepWidth>::Value,
				StateCount	 = Initial::StateCount,
				ForkCount	 = Initial::ForkCount,
				ProngCount	 = Initial::ProngCount,
			};

			Sub(StateRegistry& stateRegistry,
				const Index fork,
				Parents& stateParents,
				Parents& forkParents,
				ForkPointers& forkPointers);

			inline void wideEnterInitial(Context& context);

			inline void wideForwardRequest(const unsigned prong, const enum Transition::Type transition);
			inline void wideRequestRemain();
			inline void wideRequestRestart();
			inline void wideRequestResume(const unsigned prong);

			inline void wideForwardSubstitute(const unsigned prong, Control& control, Context& context) const;
			inline void wideSubstitute(const unsigned prong, Control& control, Context& context) const;

			inline void wideChangeTo(const unsigned prong, Context& context);
			inline void wideEnter(const unsigned prong, Context& context);
			inline void wideLeave(const unsigned prong, Context& context);

			inline bool wideUpdateAndTransition(const unsigned prong, Control& control, Context& context);
			inline void wideUpdate(const unsigned prong, Context& context);

			template <typename TEvent>
			inline void wideReact(const unsigned prong, const TEvent& event, Control& control, Context& context);

			Initial initial;
		};

		using SubStates = Sub<0, TS...>;

#pragma endregion

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		enum {
			ReverseDepth = SubStates::ReverseDepth + 1,
			DeepWidth	 = SubStates::DeepWidth,
			StateCount	 = State::StateCount + SubStates::StateCount,
			ForkCount	 = SubStates::ForkCount + 1,
			ProngCount	 = SubStates::ProngCount + sizeof...(TS),
			Width		 = sizeof...(TS),
		};

		_C(StateRegistry& stateRegistry,
		   const Parent parent,
		   Parents& stateParents,
		   Parents& forkParents,
		   ForkPointers& forkPointers);

		inline void deepEnterInitial(Context& context);

		inline void deepForwardRequest(const enum Transition::Type transition);
		inline void deepRequestRemain();
		inline void deepRequestRestart();
		inline void deepRequestResume();

		inline void deepForwardSubstitute(Control& control, Context& context) const;
		inline void deepSubstitute(Control& control, Context& context) const;

		inline void deepChangeToRequested(Context& context);
		inline void deepEnter(Context& context);
		inline void deepLeave(Context& context);

		inline bool deepUpdateAndTransition(Control& control, Context& context);
		inline void deepUpdate(Context& context);

		template <typename TEvent>
		inline void deepReact(const TEvent& event, Control& control, Context& context);

		State _state;
		SubStates _subStates;

		HSFM_DEBUG_ONLY(const TypeInfo _type = TypeInfo::get<Client>());
	};

#pragma endregion

	//----------------------------------------------------------------------

#pragma region Orthogonal

	template <typename T, typename... TS>
	struct _O final
		: public ForkT<T>
	{
		using State = _S<T>;
		using Client = typename State::Client;

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#pragma region Substates

		template <unsigned TN, typename...>
		struct Sub;

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		template <unsigned TN, typename TI, typename... TR>
		struct Sub<TN, TI, TR...> {
			using Initial = typename WrapState<TI>::Type;
			using Remaining = Sub<TN + 1, TR...>;

			enum {
				ProngIndex	 = TN,
				ReverseDepth = hfsm::detail::Max<Initial::ReverseDepth, Remaining::ReverseDepth>::Value,
				DeepWidth	 = Initial::DeepWidth  + Remaining::DeepWidth,
				StateCount	 = Initial::StateCount + Remaining::StateCount,
				ForkCount	 = Initial::ForkCount  + Remaining::ForkCount,
				ProngCount	 = Initial::ProngCount + Remaining::ProngCount,
			};

			Sub(StateRegistry& stateRegistry,
				const Index fork,
				Parents& stateParents,
				Parents& forkParents,
				ForkPointers& forkPointers);

			inline void wideEnterInitial(Context& context);

			inline void wideForwardRequest(const unsigned prong, const enum Transition::Type transition);
			inline void wideRequestRemain();
			inline void wideRequestRestart();
			inline void wideRequestResume();

			inline void wideForwardSubstitute(const unsigned prong, Control& control, Context& context) const;
			inline void wideForwardSubstitute(Control& control, Context& context) const;
			inline void wideSubstitute(Control& control, Context& context) const;

			inline void wideChangeToRequested(Context& context);
			inline void wideEnter(Context& context);
			inline void wideLeave(Context& context);

			inline bool wideUpdateAndTransition(Control& control, Context& context);
			inline void wideUpdate(Context& context);

			template <typename TEvent>
			inline void wideReact(const TEvent& event, Control& control, Context& context);

			Initial initial;
			Remaining remaining;
		};

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		template <unsigned TN, typename TI>
		struct Sub<TN, TI> {
			using Initial = typename WrapState<TI>::Type;

			enum {
				ProngIndex	 = TN,
				ReverseDepth = Initial::ReverseDepth,
				DeepWidth	 = Initial::DeepWidth,
				StateCount	 = Initial::StateCount,
				ForkCount	 = Initial::ForkCount,
				ProngCount	 = Initial::ProngCount,
			};

			Sub(StateRegistry& stateRegistry,
				const Index fork,
				Parents& stateParents,
				Parents& forkParents,
				ForkPointers& forkPointers);

			inline void wideEnterInitial(Context& context);

			inline void wideForwardRequest(const unsigned prong, const enum Transition::Type transition);
			inline void wideRequestRemain();
			inline void wideRequestRestart();
			inline void wideRequestResume();

			inline void wideForwardSubstitute(const unsigned prong, Control& control, Context& context) const;
			inline void wideForwardSubstitute(Control& control, Context& context) const;
			inline void wideSubstitute(Control& control, Context& context) const;

			inline void wideChangeToRequested(Context& context);
			inline void wideEnter(Context& context);
			inline void wideLeave(Context& context);

			inline bool wideUpdateAndTransition(Control& control, Context& context);
			inline void wideUpdate(Context& context);

			template <typename TEvent>
			inline void wideReact(const TEvent& event, Control& control, Context& context);

			Initial initial;
		};

		using SubStates = Sub<0, TS...>;

#pragma endregion

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		enum {
			ReverseDepth = SubStates::ReverseDepth + 1,
			DeepWidth	 = SubStates::DeepWidth,
			StateCount	 = State::StateCount + SubStates::StateCount,
			ForkCount	 = SubStates::ForkCount + 1,
			ProngCount	 = SubStates::ProngCount,
			Width		 = sizeof...(TS),
		};

		_O(StateRegistry& stateRegistry,
		   const Parent parent,
		   Parents& stateParents,
		   Parents& forkParents,
		   ForkPointers& forkPointers);

		inline void deepEnterInitial(Context& context);

		inline void deepForwardRequest(const enum Transition::Type transition);
		inline void deepRequestRemain();
		inline void deepRequestRestart();
		inline void deepRequestResume();

		inline void deepForwardSubstitute(Control& control, Context& context) const;
		inline void deepSubstitute(Control& control, Context& context) const;

		inline void deepChangeToRequested(Context& context);
		inline void deepEnter(Context& context);
		inline void deepLeave(Context& context);

		inline bool deepUpdateAndTransition(Control& control, Context& context);
		inline void deepUpdate(Context& context);

		template <typename TEvent>
		inline void deepReact(const TEvent& event, Control& control, Context& context);

		State _state;
		SubStates _subStates;

		HSFM_DEBUG_ONLY(const TypeInfo _type = TypeInfo::get<Client>());
	};

#pragma endregion

	//----------------------------------------------------------------------

#pragma region Root

	template <typename TApex>
	class _R final {
		using Apex = typename WrapState<TApex>::Type;

		enum : unsigned {
			StateCapacity = (unsigned) 1.3 * Apex::StateCount,
			ForkCapacity  = Apex::ForkCount,
		};

		using StateRegistryImpl		 = StateRegistryT<StateCapacity>;
		using StateParentStorage	 = Array<Parent, Apex::StateCount>;
		using ForkParentStorage		 = Array<Parent, Apex::ForkCount>;
		using ForkPointerStorage	 = Array<Fork*, ForkCapacity>;
		using TransitionQueueStorage = Array<Transition, Apex::ForkCount>;

	public:
		enum {
			DeepWidth	= Apex::DeepWidth,
			StateCount	= Apex::StateCount,
			ForkCount	= Apex::ForkCount,
			ProngCount	= Apex::ProngCount,
			Width		= Apex::Width,
		};
		static_assert(StateCount < std::numeric_limits<Index>::max(), "Too many states in the hierarchy. Change 'Index' type.");

	public:
		_R(Context& context);
		~_R();

		void update();

		template <typename TEvent>
		inline void react(const TEvent& event);

		template <typename T>
		inline void changeTo()	{ _requests << Transition(Transition::Type::Restart,  TypeInfo::get<T>());	}

		template <typename T>
		inline void schedule()	{ _requests << Transition(Transition::Type::Schedule, TypeInfo::get<T>());	}

		template <typename T>
		inline void resume()	{ _requests << Transition(Transition::Type::Resume,   TypeInfo::get<T>());	}

		template <typename T>
		inline bool isActive();

		template <typename T>
		inline bool isResumable();

	protected:
		void processRequests();
		void requestImmediate(const Transition request);
		void requestScheduled(const Transition request);

		inline unsigned id(const Transition request) const	{ return _stateRegistry[*request.stateType];	}

	private:
		Context& _context;

		StateRegistryImpl _stateRegistry;

		StateParentStorage _stateParents;
		ForkParentStorage  _forkParents;
		ForkPointerStorage _forkPointers;

		TransitionQueueStorage _requests;

		Apex _apex;
	};

	//----------------------------------------------------------------------

public:

	class Control final {
		template <typename>
		friend class _R;

	private:
		Control(TransitionQueue& requests)
			: _requests(requests)
		{}

	public:
		template <typename T>
		inline void changeTo()	{ _requests << Transition(Transition::Type::Restart,  TypeInfo::get<T>());	}

		template <typename T>
		inline void schedule()	{ _requests << Transition(Transition::Type::Schedule, TypeInfo::get<T>());	}

		template <typename T>
		inline void resume()	{ _requests << Transition(Transition::Type::Resume,	  TypeInfo::get<T>());	}

		inline unsigned requestCount() const									{ return _requests.count();	}

	private:
		TransitionQueue& _requests;
	};

#pragma endregion

	//----------------------------------------------------------------------

#pragma region Public Typedefs

	using Base = _B<Bare>;

	template <typename... TInjections>
	using BaseT = _B<TInjections...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TState, typename... TSubStates>
	using Composite	= _C<TState, TSubStates...>;

	template <typename... TSubStates>
	using CompositePeers = _C<Base, TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TState, typename... TSubStates>
	using Orthogonal = _O<TState, TSubStates...>;

	template <typename... TSubStates>
	using OrthogonalPeers = _O<Base, TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TState, typename... TSubStates>
	using Root = _R<Composite<TState, TSubStates...>>;

	template <typename... TSubStates>
	using PeerRoot = _R<CompositePeers<TSubStates...>>;

	template <typename TState, typename... TSubStates>
	using OrthogonalRoot = _R<Orthogonal<TState, TSubStates...>>;

	template <typename... TSubStates>
	using OrthogonalPeerRoot = _R<OrthogonalPeers<TSubStates...>>;

	//----------------------------------------------------------------------

#pragma endregion
};

template <typename TContext, unsigned TMaxSubstitutions = 4>
using Machine = M<TContext, TMaxSubstitutions>;

////////////////////////////////////////////////////////////////////////////////

}

#include "machine.inl"
