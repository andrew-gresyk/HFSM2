// HFSM (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk
//
// Licensed under the MIT License;
// you may not use this file except in compliance with the License.
//
//
// MIT License
//
// Copyright (c) 2019
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

#include <stdint.h>

#ifdef __GNUC__
#include <stdlib.h>		// @GCC-7: div()
#endif

#include <typeindex>

#if _MSC_VER == 1900
#include <new>			// @VS14: placement new with non-default ctor
#endif

#ifdef __GNUC__
#include <utility>		// @GCC: std::conditional<>, move(), forward()
#endif

#define HFSM_INLINE														  //inline

//------------------------------------------------------------------------------

#include "detail/shared/utility.hpp"
#include "detail/shared/iterator.hpp"
#include "detail/shared/array.hpp"
#include "detail/shared/bit_array.hpp"
#include "detail/shared/list.hpp"
#include "detail/shared/type_list.hpp"

#include "detail/debug/shared.hpp"
#include "detail/debug/logger_interface.hpp"

//------------------------------------------------------------------------------

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_ENABLE_VERBOSE_DEBUG_LOG

	#define HFSM_IF_LOGGER(...)										  __VA_ARGS__

	#define HFSM_LOGGER_OR(Y, N)												Y

	#define HFSM_LOG_TRANSITION(ORIGIN, TRANSITION, DESTINATION)				\
		if (_logger)															\
			_logger->recordTransition(ORIGIN, TRANSITION, DESTINATION);

	#define HFSM_LOG_TASK_STATUS(REGION, ORIGIN, STATUS)						\
		if (_logger)															\
			_logger->recordTaskStatus(REGION, ORIGIN, STATUS);

	#define HFSM_LOG_PLAN_STATUS(REGION, STATUS)								\
		if (_logger)															\
			_logger->recordPlanStatus(REGION, STATUS);

	#define HFSM_LOG_CANCELLED_PENDING(ORIGIN)									\
		if (_logger)															\
			_logger->recordCancelledPending(ORIGIN);

	#define HFSM_LOG_UTILITY_RESOLUTION(HEAD, PRONG, UTILITY)					\
		if (auto* const logger = control.logger())								\
			logger->recordUtilityResolution(HEAD, PRONG, UTILITY);

#else
	#define HFSM_IF_LOGGER(...)
	#define HFSM_LOGGER_OR(Y, N)												N
	#define HFSM_LOG_TRANSITION(ORIGIN, TRANSITION, DESTINATION)
	#define HFSM_LOG_TASK_STATUS(REGION, ORIGIN, STATUS)
	#define HFSM_LOG_PLAN_STATUS(REGION, STATUS)
	#define HFSM_LOG_CANCELLED_PENDING(ORIGIN)
	#define HFSM_LOG_UTILITY_RESOLUTION(HEAD, PRONG, UTILITY)
#endif

#if defined HFSM_ENABLE_VERBOSE_DEBUG_LOG

	#define HFSM_LOG_STATE_METHOD(METHOD, ID)									\
		if (auto* const logger = control.logger())								\
			logger->recordMethod(STATE_ID, ID);

#elif defined HFSM_ENABLE_LOG_INTERFACE

	#define HFSM_LOG_STATE_METHOD(METHOD, ID)									\
		if (auto* const logger = control.logger())								\
			log<decltype(METHOD), ID>(*logger);

#else
	#define HFSM_LOG_STATE_METHOD(METHOD, ID)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	#define HFSM_IF_STRUCTURE(...)									  __VA_ARGS__
#else
	#define HFSM_IF_STRUCTURE(...)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _MSC_VER || defined __clang_major__ && __clang_major__ >= 7
	#define HFSM_EXPLICIT_MEMBER_SPECIALIZATION									1
#else
	#define HFSM_EXPLICIT_MEMBER_SPECIALIZATION									0
#endif

//------------------------------------------------------------------------------

#include "detail/plan_data.hpp"
#include "detail/plan.hpp"
#include "detail/state_registry.hpp"
#include "detail/control.hpp"
#include "detail/debug/structure_report.hpp"
#include "detail/injections.hpp"
#include "detail/structure/forward.hpp"

namespace hfsm2 {

//------------------------------------------------------------------------------

template <typename TC,
		  typename TU,
		  typename TP,
		  LongIndex NS,
		  LongIndex NT>
struct ConfigT {
	using Context = TC;

	using Utility = TU;
	using Logger  = LoggerInterfaceT<Utility>;

	using Payload = TP;

	static constexpr LongIndex SUBSTITUTION_LIMIT = NS;
	static constexpr LongIndex TASK_CAPACITY	  = NT;

	template <typename T>
	using ContextT			 = ConfigT< T, TU, TP, NS, NT>;

	template <typename T>
	using UtilityT			 = ConfigT<TC,  T, TP, NS, NT>;

	template <typename T>
	using PayloadT			 = ConfigT<TC, TU,  T, NS, NT>;

	template <LongIndex N>
	using SubstitutionLimitN = ConfigT<TC, TU, TP,  N, NS>;

	template <LongIndex N>
	using TaskCapacityN		 = ConfigT<TC, TU, TP, NT,  N>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct UP {
		HFSM_INLINE UP(const Utility utility_ = Utility{1.0f},
					   const ShortIndex prong_ = INVALID_SHORT_INDEX)
			: utility{utility_}
			, prong{prong_}
		{}

		Utility utility;
		ShortIndex prong;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

//------------------------------------------------------------------------------

struct EmptyContext {};
struct EmptyPayload {};

using Config = ConfigT<EmptyContext, float, EmptyPayload, INVALID_LONG_INDEX, 4>;

namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig>
struct M_ {
	using Config_ = TConfig;

	//----------------------------------------------------------------------

	template <typename THead, typename... TSubStates>
	using Composite			 = CI_<Strategy::Composite,	 THead,	TSubStates...>;

	template <				  typename... TSubStates>
	using CompositePeers	 = CI_<Strategy::Composite,	 void,	TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using Resumable			  = CI_<Strategy::Resumable,	 THead,	TSubStates...>;

	template <				  typename... TSubStates>
	using ResumablePeers	  = CI_<Strategy::Resumable,	 void,	TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using Utilitarian		  = CI_<Strategy::Utilitarian, THead,	TSubStates...>;

	template <				  typename... TSubStates>
	using UtilitarianPeers	  = CI_<Strategy::Utilitarian, void,	TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using Orthogonal		  = OI_<THead, TSubStates...>;

	template <				  typename... TSubStates>
	using OrthogonalPeers	  = OI_<void,  TSubStates...>;

	//----------------------------------------------------------------------

	template <typename THead, typename... TSubStates>
	using Root				  = RF_<Config_, Composite  <THead, TSubStates...>>;

	template <				  typename... TSubStates>
	using PeerRoot			  = RF_<Config_, CompositePeers  <  TSubStates...>>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using ResumableRoot		  = RF_<Config_, Resumable  <THead, TSubStates...>>;

	template <				  typename... TSubStates>
	using ResumablePeerRoot	  = RF_<Config_, ResumablePeers  <  TSubStates...>>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using UtilitarianRoot	  = RF_<Config_, Utilitarian<THead, TSubStates...>>;

	template <				  typename... TSubStates>
	using UtilitarianPeerRoot = RF_<Config_, UtilitarianPeers<  TSubStates...>>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using OrthogonalRoot	  = RF_<Config_, Orthogonal <THead, TSubStates...>>;

	template <				  typename... TSubStates>
	using OrthogonalPeerRoot  = RF_<Config_, OrthogonalPeers <  TSubStates...>>;

	//----------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////

}

using Machine = detail::M_<Config>;

template <typename TConfig>
using MachineT = detail::M_<TConfig>;

////////////////////////////////////////////////////////////////////////////////

}

#include "detail/structure/state.hpp"
#include "detail/structure/composite_sub.hpp"
#include "detail/structure/composite.hpp"
#include "detail/structure/orthogonal_sub.hpp"
#include "detail/structure/orthogonal.hpp"
#include "detail/structure/root.hpp"

#undef HFSM_INLINE
#undef HFSM_IF_LOGGER
#undef HFSM_LOGGER_OR
#undef HFSM_LOG_STATE_METHOD
#undef HFSM_IF_STRUCTURE

#ifdef _MSC_VER
#pragma warning(pop)
#endif
