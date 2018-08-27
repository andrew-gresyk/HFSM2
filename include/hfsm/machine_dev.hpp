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

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>		// @GCC-7: div()

#include <typeindex>
#include <utility>		// @GCC: std::conditional<>, move(), forward()

//------------------------------------------------------------------------------

#include "detail/lib_utility.hpp"
#include "detail/lib_iterator.hpp"
#include "detail/lib_array_view.hpp"
#include "detail/lib_array.hpp"
#include "detail/lib_bit_array.hpp"
#include "detail/lib_list.hpp"
#include "detail/lib_object_pool.hpp"
#include "detail/lib_type_list.hpp"

#include "detail/debug_shared.hpp"
#include "detail/debug_logger_interface.hpp"

//------------------------------------------------------------------------------

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	#define HFSM_IF_LOGGER(...)		__VA_ARGS__
	#define HFSM_LOGGER_OR(y, n)	y
#else
	#define HFSM_IF_LOGGER(...)
	#define HFSM_LOGGER_OR(y, n)	n
#endif

#if defined HFSM_FORCE_DEBUG_LOG
	#define HFSM_LOG_STATE_METHOD(METHOD, ID)									\
		if (auto* const logger = control.logger())								\
			logger->recordMethod(STATE_ID, ID);
	#define HFSM_LOG_PLANNER_METHOD(METHOD, ID)									\
		if (auto* const logger = control.logger())								\
			logger->recordMethod(STATE_ID, ID);
#elif defined HFSM_ENABLE_LOG_INTERFACE
	#define HFSM_LOG_STATE_METHOD(METHOD, ID)									\
		if (auto* const logger = control.logger())								\
			log<decltype(METHOD), ID>(*logger);
	#define HFSM_LOG_PLANNER_METHOD(METHOD, ID)									\
		if (auto* const logger = control.logger())								\
			State::template log<decltype(METHOD), ID>(*logger);
#else
	#define HFSM_LOG_STATE_METHOD(METHOD, ID)
	#define HFSM_LOG_PLANNER_METHOD(METHOD, ID)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	#define HFSM_IF_STRUCTURE(...)	__VA_ARGS__
#else
	#define HFSM_IF_STRUCTURE(...)
#endif

//------------------------------------------------------------------------------

#include "detail/machine_plan.hpp"
#include "detail/machine_registry.hpp"
#include "detail/machine_control.hpp"
#include "detail/debug_structure_report.hpp"
#include "detail/machine_injections.hpp"
#include "detail/machine_forward.hpp"

namespace hfsm {
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

#include "detail/machine_state.hpp"
#include "detail/machine_composite_sub.hpp"
#include "detail/machine_composite.hpp"
#include "detail/machine_sequence.hpp"
#include "detail/machine_orthogonal_sub.hpp"
#include "detail/machine_orthogonal.hpp"
#include "detail/machine_root.hpp"

#undef HFSM_IF_LOGGER
#undef HFSM_LOGGER_OR
#undef HFSM_LOG_STATE_METHOD
#undef HFSM_LOG_PLANNER_METHOD
#undef HFSM_IF_STRUCTURE

#ifdef _MSC_VER
#pragma warning(pop)
#endif
