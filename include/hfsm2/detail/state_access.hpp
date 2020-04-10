namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------
// these should be member S_<> functions
// but neither GCC nor Clang can handle function specializations within class scope

template <typename TState,
		  typename TInternal>
TState&
access(TInternal&);

////////////////////////////////////////////////////////////////////////////////

template <typename T,
		  typename TN,
		  typename TA,
		  Strategy TG,
		  typename TH,
		  typename... TS>
HFSM_INLINE
T&
access(		C_<TN, TA, TG, TH, TS...>& composite) {
	return access<T>(composite._subStates);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T,
		  typename TN,
		  typename TA,
		  Strategy TG,
		  typename TH,
		  typename... TS>
HFSM_INLINE
const T&
access(const C_<TN, TA, TG, TH, TS...>& composite) {
	return access<T>(composite._subStates);
}

//------------------------------------------------------------------------------

template <typename TN,
		  typename TA,
		  Strategy TG,
		  typename TH,
		  typename... TS>
HFSM_INLINE
TH&
access(		 C_<TN, TA, TG, TH, TS...>& composite) {
	return composite._headState._head;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN,
		  typename TA,
		  Strategy TG,
		  typename TH,
		  typename... TS>
HFSM_INLINE
const TH&
access(const C_<TN, TA, TG, TH, TS...>& composite) {
	return composite._headState._head;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T,
		  typename TN,
		  typename TA,
		  Strategy TG,
		  ShortIndex NI,
		  typename... TS>
T&
access(		 CS_<TN, TA, TG, NI, TS...>& compositeSub) {
	return CS_<TN, TA, TG, NI, TS...>::LHalf::StateList::template contains<T>() ?
		access<T>(compositeSub.lHalf) :
		access<T>(compositeSub.rHalf);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T,
		  typename TN,
		  typename TA,
		  Strategy TG,
		  ShortIndex NI,
		  typename... TS>
const T&
access(const CS_<TN, TA, TG, NI, TS...>& compositeSub) {
	return CS_<TN, TA, TG, NI, TS...>::LHalf::StateList::template contains<T>() ?
		access<T>(compositeSub.lHalf) :
		access<T>(compositeSub.rHalf);
}

//------------------------------------------------------------------------------

template <typename T,
		  typename TN,
		  typename TA,
		  Strategy TG,
		  ShortIndex NI,
		  typename TS>
T&
access(		 CS_<TN, TA, TG, NI, TS>& compositeSub) {
	return access<T>(compositeSub.state);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T,
		  typename TN,
		  typename TA,
		  Strategy TG,
		  ShortIndex NI,
		  typename TS>
const T&
access(const CS_<TN, TA, TG, NI, TS>& compositeSub) {
	return access<T>(compositeSub.state);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T,
		  typename TN,
		  typename TA,
		  typename TH,
		  typename... TS>
HFSM_INLINE
T&
access(		O_<TN, TA, TH, TS...>& orthogonal) {
	return access<T>(orthogonal._subStates);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T,
		  typename TN,
		  typename TA,
		  typename TH,
		  typename... TS>
HFSM_INLINE
const T&
access(const O_<TN, TA, TH, TS...>& orthogonal) {
	return access<T>(orthogonal._subStates);
}

//------------------------------------------------------------------------------

template <typename TN,
		  typename TA,
		  typename TH,
		  typename... TS>
HFSM_INLINE
TH&
access(		 O_<TN, TA, TH, TS...>& orthogonal) {
	return orthogonal._headState._head;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN,
		  typename TA,
		  typename TH,
		  typename... TS>
HFSM_INLINE
const TH&
access(const O_<TN, TA, TH, TS...>& orthogonal) {
	return orthogonal._headState._head;
}

////////////////////////////////////////////////////////////////////////////////

#ifndef __clang__

template <typename T,
		  typename TN,
		  typename TA,
		  ShortIndex NI,
		  typename TI,
		  typename... TR>
HFSM_INLINE
T&
access(		OS_<TN, TA, NI, TI, TR...>& orthogonalSub) {
	return OS_<TN, TA, NI, TI, TR...>::InitialStates::template contains<T>() ?
		access<T>(orthogonalSub.initial  ) :
		access<T>(orthogonalSub.remaining);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T,
		  typename TN,
		  typename TA,
		  ShortIndex NI,
		  typename TI,
		  typename... TR>
HFSM_INLINE
const T&
access(const OS_<TN, TA, NI, TI, TR...>& orthogonalSub) {
	return OS_<TN, TA, NI, TI, TR...>::InitialStates::template contains<T>() ?
		access<T>(orthogonalSub.initial  ) :
		access<T>(orthogonalSub.remaining);
}

#else

template <typename T,
		  typename TN,
		  typename TA,
		  ShortIndex NI,
		  typename TI,
		  typename TR, // Clang can't figure out the correct specialization without a hint
		  typename... TRs>
HFSM_INLINE
T&
access(		OS_<TN, TA, NI, TI, TR, TRs...>& orthogonalSub) {
	return OS_<TN, TA, NI, TI, TR, TRs...>::InitialStates::template contains<T>() ?
		access<T>(orthogonalSub.initial  ) :
		access<T>(orthogonalSub.remaining);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T,
		  typename TN,
		  typename TA,
		  ShortIndex NI,
		  typename TI,
		  typename TR, // Clang can't figure out the correct specialization without a hint
		  typename... TRs>
HFSM_INLINE
const T&
access(const OS_<TN, TA, NI, TI, TR, TRs...>& orthogonalSub) {
	return OS_<TN, TA, NI, TI, TR, TRs...>::InitialStates::template contains<T>() ?
		access<T>(orthogonalSub.initial  ) :
		access<T>(orthogonalSub.remaining);
}

#endif

//------------------------------------------------------------------------------

template <typename T,
		  typename TN,
		  typename TA,
		  ShortIndex NI,
		  typename TI>
HFSM_INLINE
T&
access(		OS_<TN, TA, NI, TI>& orthogonalSub) {
	return access<T>(orthogonalSub.initial);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T,
		  typename TN,
		  typename TA,
		  ShortIndex NI,
		  typename TI>
HFSM_INLINE
const T&
access(const OS_<TN, TA, NI, TI>& orthogonalSub) {
	return access<T>(orthogonalSub.initial);
}

////////////////////////////////////////////////////////////////////////////////

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wnull-dereference"
#endif

template <typename T,
		  typename TI,
		  typename TA,
		  typename TH>
HFSM_INLINE
T&
access(		 S_<TI, TA, TH>&	  ) {
	return *reinterpret_cast<T*>(0);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T,
		  typename TI,
		  typename TA,
		  typename TH>
HFSM_INLINE
T&
access(const S_<TI, TA, TH>&	  ) {
	return *reinterpret_cast<T*>(0);
}

#ifdef __clang__
	#pragma clang diagnostic pop
#endif

//------------------------------------------------------------------------------

template <typename TI,
		  typename TA,
		  typename TH>
HFSM_INLINE
TH&
access(		 S_<TI, TA, TH>& state) {
	return state._head;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TI,
		  typename TA,
		  typename TH>
HFSM_INLINE
const TH&
access(const S_<TI, TA, TH>& state)	{
	return state._head;
}

////////////////////////////////////////////////////////////////////////////////

}
}
