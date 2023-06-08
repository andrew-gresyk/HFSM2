// HFSM2 (hierarchical state machine for games and interactive applications)
// 2.3.0 (2023-06-08)
//
// Created by Andrew Gresyk
//
// Licensed under the MIT License;
// you may not use this file except in compliance with the License.
//
//
// MIT License
//
// Copyright (c) 2023
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

#define HFSM2_VERSION_MAJOR 2
#define HFSM2_VERSION_MINOR 3
#define HFSM2_VERSION_PATCH 0

#define HFSM2_VERSION (10000 * HFSM2_VERSION_MAJOR + 100 * HFSM2_VERSION_MINOR + HFSM2_VERSION_PATCH)

#include <stdint.h>			// uint32_t, uint64_t
#include <string.h>			// memcpy_s()

#include <new>
#ifndef HFSM2_DISABLE_TYPEINDEX
	#include <typeindex>
#endif
#include <type_traits>		// std::aligned_storage<>

#if defined _DEBUG && _MSC_VER
	#include <intrin.h>		// __debugbreak()
#endif

//------------------------------------------------------------------------------

#include "detail/shared/macros_on.hpp"

#include "detail/shared/utility.hpp"
#include "detail/shared/iterator.hpp"
#include "detail/shared/bit_stream.hpp"
#include "detail/shared/random.hpp"
#include "detail/shared/type_list.hpp"

#include "detail/features/common.hpp"
#include "detail/features/logger_interface.hpp"
#include "detail/features/structure_report.hpp"

#include "detail/containers/array.hpp"
#include "detail/containers/bit_array.hpp"
#include "detail/containers/task_list.hpp"

#include "detail/root/registry.hpp"
#include "detail/root/plan_data.hpp"
#include "detail/root/plan.hpp"
#include "detail/root/core.hpp"
#include "detail/root/control.hpp"

#include "detail/structure/ancestors.hpp"
#include "detail/structure/state.hpp"
#include "detail/structure/forward.hpp"
#include "detail/structure/composite_sub.hpp"
#include "detail/structure/composite.hpp"
#include "detail/structure/orthogonal_sub.hpp"
#include "detail/structure/orthogonal.hpp"

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct G_ final {
	static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

	using Context			 = TContext;
	using Activation		 = TActivation;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank				 = TRank;
	using Utility			 = TUtility;
	using RNG				 = TRNG;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using LoggerInterface	 = LoggerInterfaceT<FEATURE_TAG, Context HFSM2_IF_UTILITY_THEORY(, Utility)>;
#endif

	static constexpr Long SUBSTITUTION_LIMIT = NSubstitutionLimit;

#if HFSM2_PLANS_AVAILABLE()
	static constexpr Long TASK_CAPACITY		 = NTaskCapacity;
#endif

	using Payload			 = TPayload;
	using Transition		 = TransitionT<Payload>;

#if HFSM2_PLANS_AVAILABLE()
	using Task				 = TaskT<Payload>;
#endif

	/// @brief Set Context type
	/// @tparam T Context type for data shared between states and/or data interface between FSM and external code
	template <typename T>
	using ContextT			 = G_<FEATURE_TAG, T	  , Activation HFSM2_IF_UTILITY_THEORY(, Rank, Utility, RNG), SUBSTITUTION_LIMIT HFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

	/// @brief Select manual activation strategy
	using ManualActivation	 = G_<FEATURE_TAG, Context, Manual	   HFSM2_IF_UTILITY_THEORY(, Rank, Utility, RNG), SUBSTITUTION_LIMIT HFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Set Rank type
	/// @tparam T Rank type for 'TRank State::rank() const' method
	template <typename T>
	using RankT				 = G_<FEATURE_TAG, Context, Activation						   , T   , Utility, RNG , SUBSTITUTION_LIMIT HFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

	/// @brief Set Utility type
	/// @tparam T Utility type for 'TUtility State::utility() const' method
	template <typename T>
	using UtilityT			 = G_<FEATURE_TAG, Context, Activation						   , Rank, T      , RNG , SUBSTITUTION_LIMIT HFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

	/// @brief Set RNG type
	/// @tparam T RNG type used in 'Random' regions
	template <typename T>
	using RandomT			 = G_<FEATURE_TAG, Context, Activation						   , Rank, Utility, T   , SUBSTITUTION_LIMIT HFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

#endif

	/// @brief Set Substitution limit
	/// @tparam N Maximum number times 'guard()' methods can substitute their states for others
	template <Long N>
	using SubstitutionLimitN = G_<FEATURE_TAG, Context, Activation HFSM2_IF_UTILITY_THEORY(, Rank, Utility, RNG), N                  HFSM2_IF_PLANS(, TASK_CAPACITY), Payload>;

#if HFSM2_PLANS_AVAILABLE()

	/// @brief Set Task capacity
	/// @tparam N Maximum number of tasks across all plans
	template <Long N>
	using TaskCapacityN		 = G_<FEATURE_TAG, Context, Activation HFSM2_IF_UTILITY_THEORY(, Rank, Utility, RNG), SUBSTITUTION_LIMIT                , N             , Payload>;

#endif

	/// @brief Set Transition Payload type
	/// @tparam T Utility type for 'TUtility State::utility() const' method
	template <typename T>
	using PayloadT			 = G_<FEATURE_TAG, Context, Activation HFSM2_IF_UTILITY_THEORY(, Rank, Utility, RNG), SUBSTITUTION_LIMIT HFSM2_IF_PLANS(, TASK_CAPACITY), T      >;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	struct UP final {
		HFSM2_CONSTEXPR(14)	UP(const Utility utility_ = Utility{1},
							   const Short prong_ = INVALID_SHORT)		noexcept
			: utility{utility_}
			, prong{prong_}
		{}

		Utility utility;
		Short prong;
	};

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig>
struct M_;

template <FeatureTag NFeatureTag
		, typename TContext
		, typename TActivation

	#if HFSM2_UTILITY_THEORY_AVAILABLE()
		, typename TRank
		, typename TUtility
		, typename TRNG
	#endif

		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct M_	   <G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>> final {
	using Cfg = G_<NFeatureTag, TContext, TActivation HFSM2_IF_UTILITY_THEORY(, TRank, TUtility, TRNG), NSubstitutionLimit HFSM2_IF_PLANS(, NTaskCapacity), TPayload>;

	static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

	using Context			= TContext;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Utility			= TUtility;
#endif

	using Payload			= TPayload;
	using Transition		= TransitionT<Payload>;

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using LoggerInterface	= typename Cfg::LoggerInterface;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Composite region ('changeTo<>()' into the region acts as 'restart<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Composite			 = CI_<Strategy::Composite,	   THead, TSubStates...>;

	/// @brief Headless composite region ('changeTo<>()' into the region acts as 'restart<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using CompositePeers	 = CI_<Strategy::Composite,    void,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Resumable region ('changeTo<>()' into the region acts as 'resume<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Resumable			  = CI_<Strategy::Resumable,   THead, TSubStates...>;

	/// @brief Headless resumable region ('changeTo<>()' into the region acts as 'resume<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using ResumablePeers	  = CI_<Strategy::Resumable,   void,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Selectable region ('changeTo<>()' into the region acts as 'select<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Selectable		  = CI_<Strategy::Selectable,  THead, TSubStates...>;

	/// @brief Headless selectable region ('changeTo<>()' into the region acts as 'select<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using SelectablePeers	  = CI_<Strategy::Selectable,  void,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Utilitarian region ('changeTo<>()' into the region acts as 'utilize<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Utilitarian		  = CI_<Strategy::Utilitarian, THead, TSubStates...>;

	/// @brief Headless utilitarian region ('changeTo<>()' into the region acts as 'utilize<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using UtilitarianPeers	  = CI_<Strategy::Utilitarian, void,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Random region ('changeTo<>()' into the region acts as 'randomize<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Random			  = CI_<Strategy::RandomUtil,  THead, TSubStates...>;

	/// @brief Headless random region ('changeTo<>()' into the region acts as 'randomize<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using RandomPeers		  = CI_<Strategy::RandomUtil,  void,  TSubStates...>;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Orthogonal region (when activated, activates all sub-states)
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Orthogonal		  = OI_<THead, TSubStates...>;

	/// @brief Headless orthogonal region (when activated, activates all sub-states)
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using OrthogonalPeers	  = OI_<void,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Root ('changeTo<>()' into the root region acts as 'restart<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using Root				  = RF_<Cfg, Composite  <THead, TSubStates...>>;

	/// @brief Headless root ('changeTo<>()' into the root region acts as 'restart<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using PeerRoot			  = RF_<Cfg, CompositePeers  <  TSubStates...>>;

	// COMMON
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Resumable root ('changeTo<>()' into the root region acts as 'resume<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using ResumableRoot		  = RF_<Cfg, Resumable  <THead, TSubStates...>>;

	/// @brief Headless resumable root ('changeTo<>()' into the root region acts as 'resume<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using ResumablePeerRoot	  = RF_<Cfg, ResumablePeers  <  TSubStates...>>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Selectable root ('changeTo<>()' into the region acts as 'select<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using SelectableRoot	  = RF_<Cfg, Selectable <THead, TSubStates...>>;

	/// @brief Headless selectable root ('changeTo<>()' into the region acts as 'select<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using SelectablePeerRoot  = RF_<Cfg, SelectablePeers <  TSubStates...>>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

	/// @brief Utilitarian root ('changeTo<>()' into the root region acts as 'utilize<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using UtilitarianRoot	  = RF_<Cfg, Utilitarian<THead, TSubStates...>>;

	/// @brief Headless utilitarian root ('changeTo<>()' into the root region acts as 'utilize<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using UtilitarianPeerRoot = RF_<Cfg, UtilitarianPeers<  TSubStates...>>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Random root ('changeTo<>()' into the root region acts as 'randomize<>()')
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using RandomRoot		  = RF_<Cfg, Random		<THead, TSubStates...>>;

	/// @brief Headless random root ('changeTo<>()' into the root region acts as 'randomize<>()')
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using RandomPeerRoot	  = RF_<Cfg, RandomPeers	 <  TSubStates...>>;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Orthogonal root (all sub-states are active at all times)
	/// @tparam THead Head state
	/// @tparam TSubStates Sub-states
	template <typename THead, typename... TSubStates>
	using OrthogonalRoot	  = RF_<Cfg, Orthogonal <THead, TSubStates...>>;

	/// @brief Headless orthogonal root (all sub-states are active at all times)
	/// @tparam TSubStates Sub-states
	template <				  typename... TSubStates>
	using OrthogonalPeerRoot  = RF_<Cfg, OrthogonalPeers <  TSubStates...>>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

}

/// @brief Type configuration for MachineT<>
using Config = detail::G_<HFSM2_FEATURE_TAG, EmptyContext, Automatic HFSM2_IF_UTILITY_THEORY(, int8_t, float, RNGT<float>), 4 HFSM2_IF_PLANS(, INVALID_LONG), void>;

/// @brief 'Template namespace' for FSM classes
/// @tparam TConfig 'ConfigT<>' type configuration for MachineT<>
/// @see ConfigT<>
template <typename TConfig = Config>
using MachineT = detail::M_<TConfig>;

/// @brief 'Template namespace' for FSM classes parametrized with default types
using Machine = MachineT<>;

////////////////////////////////////////////////////////////////////////////////

}

#include "detail/root.hpp"

#include "detail/shared/macros_off.hpp"
