// HFSM (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk
//
// Licensed under the MIT License;
// you may not use this file except in compliance with the License.
//
//
// MIT License
//
// Copyright (c) 2017
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
#ifdef __GNUC__
#include <utility>		// @GCC: std::conditional<>, move(), forward()
#endif

#define HFSM_INLINE														  inline

//------------------------------------------------------------------------------

#include "detail/shared/utility.hpp"
#include "detail/shared/iterator.hpp"
#include "detail/shared/array_view.hpp"
#include "detail/shared/array.hpp"
#include "detail/shared/bit_array.hpp"
#include "detail/shared/list.hpp"
#include "detail/shared/object_pool.hpp"
#include "detail/shared/type_list.hpp"

#include "detail/debug/shared.hpp"
#include "detail/debug/logger_interface.hpp"

//------------------------------------------------------------------------------

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	#define HFSM_IF_LOGGER(...)										  __VA_ARGS__
	#define HFSM_LOGGER_OR(Y, N)												Y
	#define HFSM_LOG_TASK_STATUS(REGION, ORIGIN, STATUS)						\
		if (_logger)															\
			_logger->recordTaskStatus(REGION, ORIGIN, STATUS);
	#define HFSM_LOG_PLAN_STATUS(REGION, STATUS)								\
		if (_logger)															\
			_logger->recordPlanStatus(REGION, STATUS);
	#define HFSM_LOG_CANCELLED_PENDING(ORIGIN)									\
		if (_logger)															\
			_logger->recordCancelledPending(ORIGIN);
#else
	#define HFSM_IF_LOGGER(...)
	#define HFSM_LOGGER_OR(Y, N)												N
	#define HFSM_LOG_TASK_STATUS(REGION, ORIGIN, STATUS)
	#define HFSM_LOG_PLAN_STATUS(REGION, STATUS)
	#define HFSM_LOG_CANCELLED_PENDING(ORIGIN)
#endif

#if defined HFSM_VERBOSE_DEBUG_LOG
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

//------------------------------------------------------------------------------

#include "detail/plan_data.hpp"
#include "detail/plan.hpp"
#include "detail/state_registry.hpp"
#include "detail/control.hpp"
#include "detail/debug/structure_report.hpp"
#include "detail/injections.hpp"
#include "detail/structure/forward.hpp"

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TConfig = Config<>,
		  typename TPayloadList = _TL<>>
struct _M {
	using Context = TContext;
	using Config  = TConfig;

	using PayloadList = TPayloadList;

	//----------------------------------------------------------------------

	template <typename THead, typename... TSubStates>
	using Composite			 = _CF<THead, TSubStates...>;

	template <				  typename... TSubStates>
	using CompositePeers	 = _CF<void,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using Orthogonal		 = _OF<THead, TSubStates...>;

	template <				  typename... TSubStates>
	using OrthogonalPeers	 = _OF<void,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using Root				 = _RF<Context, Config, PayloadList, Composite <THead, TSubStates...>>;

	template <				  typename... TSubStates>
	using PeerRoot			 = _RF<Context, Config, PayloadList, CompositePeers	<  TSubStates...>>;

	template <typename THead, typename... TSubStates>
	using OrthogonalRoot	 = _RF<Context, Config, PayloadList, Orthogonal<THead, TSubStates...>>;

	template <				  typename... TSubStates>
	using OrthogonalPeerRoot = _RF<Context, Config, PayloadList, OrthogonalPeers<  TSubStates...>>;

	//----------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////

}

template <typename... Ts>
using TransitionPayloads = detail::_TL<Ts...>;

//------------------------------------------------------------------------------

template <typename...>
struct Machine;

template <typename TContext>
struct Machine<TContext>
	: detail::_M<TContext>
{};

template <typename TContext, LongIndex... NConstants>
struct Machine<TContext, Config<NConstants...>>
	: detail::_M<TContext, Config<NConstants...>, TransitionPayloads<>>
{};

template <typename TContext, typename... TPayloads>
struct Machine<TContext, TransitionPayloads<TPayloads...>>
	: detail::_M<TContext, Config<>, TransitionPayloads<TPayloads...>>
{};

template <typename TContext, LongIndex... NConstants, typename... TPayloads>
struct Machine<TContext, Config<NConstants...>, TransitionPayloads<TPayloads...>>
	: detail::_M<TContext, Config<NConstants...>, TransitionPayloads<TPayloads...>>
{};

////////////////////////////////////////////////////////////////////////////////

}

#include "detail/structure/state.hpp"
#include "detail/structure/fsm/composite_sub.hpp"
#include "detail/structure/fsm/composite.hpp"
#include "detail/structure/fsm/orthogonal_sub.hpp"
#include "detail/structure/fsm/orthogonal.hpp"
#include "detail/structure/root.hpp"

#undef HFSM_INLINE
#undef HFSM_IF_LOGGER
#undef HFSM_LOGGER_OR
#undef HFSM_LOG_STATE_METHOD
#undef HFSM_IF_STRUCTURE

#ifdef _MSC_VER
#pragma warning(pop)
#endif
