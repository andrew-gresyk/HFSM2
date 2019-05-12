[![Standard](https://img.shields.io/badge/c%2B%2B-14/17/20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Build status](https://ci.appveyor.com/api/projects/status/egs56khk70ud35un?svg=true)](https://ci.appveyor.com/project/andrew-gresyk/hfsm2)
[![Build Status](https://travis-ci.org/andrew-gresyk/HFSM2.svg?branch=master)](https://travis-ci.org/andrew-gresyk/HFSM2)
[![codecov](https://codecov.io/gh/andrew-gresyk/HFSM2/branch/master/graph/badge.svg)](https://codecov.io/gh/andrew-gresyk/HFSM2)
[![Coverage Status](https://coveralls.io/repos/github/andrew-gresyk/HFSM2/badge.svg?branch=master)](https://coveralls.io/github/andrew-gresyk/HFSM2?branch=master)

# HFSM2: Hierarchical Finite State Machine Framework with **Planning Support**

Header-only heriarchical FSM framework in C++14, completely static (no dynamic allocations), built with variadic templates.

## Compiler Support

- Visual Studio 14, 15, 16
- GCC 5, 6, 7, 8
- Clang 3.7, 3.8, 3.9, 4, 5, 6, 7

---

## Tutorial

Check [Wiki](../../wiki/Tutorial) for basic usage and more info.

---

## Feature Highlights

- Permissive [MIT License](LICENSE.md)
- Written in widely-supported modern(ish) C++11
- Header-only
- Fully static, no dynamic allocations
- Uses inline-friendly compile-time pylymorphism, no virtual methods were harmed
- Type-safe transitions: `FSM.changeTo<TargetState>()`
- 100% NoUML-compliant
- Hierarchical, with composite (sub-machine) and orthogonal regions
- Gamedev-friendly, supports explicit `State::update()`
- Also supports traditional event-based workflow with `State::react()`
- Planning support.
- Utility theory support.
- Scaleable, supports state re-use via state injections
- Debug-assisted, includes automatic structure and activity visualization API with `#define HFSM_ENABLE_STRUCTURE_REPORT`
- Convenient, minimal boilerplate

---

## Get Updates

- [Blog](https://andrew-gresyk.github.io/)
- [Twitter](https://www.twitter.com/andrew_gresyk)

---

## Special Thanks

- [Phil Nash](https://github.com/philsquared)
- [Romain Cheminade](https://github.com/romaincheminade)
- [Tristan Brindle](https://github.com/tcbrindle)
- [Kevin Greene](https://github.com/kgreenek)
- everybody at [C++::London](https://www.meetup.com/CppLondon/) meetup
- programming community at [Splash Damage](http://www.splashdamage.com/)
