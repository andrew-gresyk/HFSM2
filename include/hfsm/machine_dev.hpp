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

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <typeindex>

#include "detail/lib_utility.hpp"
#include "detail/lib_iterator.hpp"
#include "detail/lib_array_view.hpp"
#include "detail/lib_array.hpp"
#include "detail/lib_wrap.hpp"
#include "detail/lib_hash_table.hpp"
#include "detail/lib_type_info.hpp"
#include "detail/lib_type_list.hpp"

#include "detail/debug_shared.hpp"
#include "detail/debug_logger_interface.hpp"

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_LOG_INTERFACE
	#define HFSM_IF_LOGGER(...)		__VA_ARGS__
	#define HFSM_LOGGER_OR(y, n)	y
#else
	#define HFSM_IF_LOGGER(...)
	#define HFSM_LOGGER_OR(y, n)	n
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	#define HFSM_IF_STRUCTURE(...)	__VA_ARGS__
#else
	#define HFSM_IF_STRUCTURE(...)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_ALIGNMENT_CHEKS
	#define HFSM_IF_ALIGNMENT_CHEKS(...)	__VA_ARGS__
#else
	#define HFSM_IF_ALIGNMENT_CHEKS(...)
#endif

//------------------------------------------------------------------------------

#include "detail/machine_utility.hpp"
#include "detail/debug_structure_report.hpp"
#include "detail/machine_injections.hpp"

//------------------------------------------------------------------------------

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TPayloadList = TypeListT<>,
		  ShortIndex TMaxSubstitutions = 4>
struct _M {
	using Context = TContext;
	using Control = ControlT<Context>;

	enum : ShortIndex {
		MAX_SUBSTITUTIONS = TMaxSubstitutions
	};

	using PayloadList = TPayloadList;
	//using Transition = TransitionT<PayloadList>;

	using TransitionControl = TransitionControlT<Context, PayloadList>;

	using TypeInfo = TypeInfo;

	using Bare = Bare<Context, PayloadList>;
	using Base = Base<Context, PayloadList>;

	template <typename... TInjections>
	using BaseT = _B<TInjections...>;

	//----------------------------------------------------------------------

	template <typename THead, typename... TSubStates>
	using Composite			 = _C<Context, PayloadList, THead, TSubStates...>;

	template <				  typename... TSubStates>
	using CompositePeers	 = _C<Context, PayloadList, Base,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using Orthogonal		 = _O<Context, PayloadList, THead, TSubStates...>;

	template <				  typename... TSubStates>
	using OrthogonalPeers	 = _O<Context, PayloadList, Base,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using Root				 = _R<Context, PayloadList, MAX_SUBSTITUTIONS, Composite< THead, TSubStates...>>;

	template <				  typename... TSubStates>
	using PeerRoot			 = _R<Context, PayloadList, MAX_SUBSTITUTIONS, CompositePeers<	 TSubStates...>>;

	template <typename THead, typename... TSubStates>
	using OrthogonalRoot	 = _R<Context, PayloadList, MAX_SUBSTITUTIONS, Orthogonal<THead, TSubStates...>>;

	template <				  typename... TSubStates>
	using OrthogonalPeerRoot = _R<Context, PayloadList, MAX_SUBSTITUTIONS, OrthogonalPeers<	 TSubStates...>>;

	//----------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////

}

//------------------------------------------------------------------------------

template <ShortIndex TMaxSubstitutions = 4>
struct Config {
	enum : ShortIndex { MAX_SUBSTITUTIONS = TMaxSubstitutions };
};

//------------------------------------------------------------------------------

template <typename... Ts>
using PayloadList = detail::TypeListT<Ts...>;

//------------------------------------------------------------------------------

template <typename...>
struct Machine;

template <typename TContext>
struct Machine<TContext>
	: detail::_M<TContext>
{};

template <typename TContext, typename TConfig>
struct Machine<TContext, TConfig>
	: detail::_M<TContext, PayloadList<>, TConfig::MAX_SUBSTITUTIONS>
{};

template <typename TContext, typename... TPayloads>
struct Machine<TContext, PayloadList<TPayloads...>>
	: detail::_M<TContext, PayloadList<TPayloads...>>
{};

template <typename TContext, typename TConfig, typename... TPayloads>
struct Machine<TContext, PayloadList<TPayloads...>, TConfig>
	: detail::_M<TContext, PayloadList<TPayloads...>, TConfig::MAX_SUBSTITUTIONS>
{};

////////////////////////////////////////////////////////////////////////////////

}

#include "detail/machine_state.hpp"
#include "detail/machine_composite_sub.hpp"
#include "detail/machine_composite.hpp"
#include "detail/machine_orthogonal_sub.hpp"
#include "detail/machine_orthogonal.hpp"
#include "detail/machine_root.hpp"

#undef HFSM_IF_LOGGER
#undef HFSM_LOGGER_OR
#undef HFSM_IF_STRUCTURE
