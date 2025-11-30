// HFSM2 (hierarchical state machine for games and interactive applications)
// 2.10.0 (2025-11-30)
//
// Created by Andrew Gresyk
//
// Licensed under the MIT License;
// you may not use this file except in compliance with the License.
//
//
// MIT License
//
// Copyright (c) 2025
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
#define HFSM2_VERSION_MINOR 10
#define HFSM2_VERSION_PATCH 0

#define HFSM2_VERSION (10000 * HFSM2_VERSION_MAJOR + 100 * HFSM2_VERSION_MINOR + HFSM2_VERSION_PATCH)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#include <stdint.h>			// uint32_t, uint64_t
#include <string.h>			// memcpy_s()

#include <new>
#ifndef HFSM2_DISABLE_TYPEINDEX
	#include <typeindex>
#endif

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

#include "detail/containers/array.hpp"
#include "detail/containers/bit_array.hpp"

#include "detail/features/transition.hpp"
#include "detail/features/logger_interface.hpp"
#include "detail/features/structure_report.hpp"
#include "detail/features/task.hpp"
#include "detail/features/task_list.hpp"

#include "detail/root/registry_1.hpp"
#include "detail/root/registry_2.hpp"
#include "detail/root/plan_data.hpp"
#include "detail/root/plan_0.hpp"
#include "detail/root/plan_1.hpp"
#include "detail/root/plan_2.hpp"
#include "detail/root/core.hpp"
#include "detail/root/control_0.hpp"
#include "detail/root/control_1.hpp"
#include "detail/root/control_2.hpp"
#include "detail/root/control_3.hpp"
#include "detail/root/control_4.hpp"
#include "detail/root/control_5.hpp"

#include "detail/structure/base.hpp"
#include "detail/structure/ancestors_1.hpp"
#include "detail/structure/ancestors_2.hpp"
#include "detail/structure/state_1.hpp"
#include "detail/structure/state_2.hpp"
#include "detail/structure/forward.hpp"
#include "detail/structure/reactions.inl"
#include "detail/structure/composite_sub_1.hpp"
#include "detail/structure/composite_sub_2.hpp"
#include "detail/structure/composite.hpp"
#include "detail/structure/orthogonal_sub_1.hpp"
#include "detail/structure/orthogonal_sub_2.hpp"
#include "detail/structure/orthogonal.hpp"

#include "detail/config.hpp"
#include "detail/root_0.hpp"
#include "detail/root_1.hpp"
#include "detail/root_2.hpp"
#include "detail/root_3.hpp"
#include "detail/root_4.hpp"

#include "detail/shared/macros_off.hpp"
