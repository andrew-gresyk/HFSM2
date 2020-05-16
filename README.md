[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Build status](https://ci.appveyor.com/api/projects/status/egs56khk70ud35un?svg=true)](https://ci.appveyor.com/project/andrew-gresyk/hfsm2)
[![Build Status](https://travis-ci.org/andrew-gresyk/HFSM2.svg?branch=master)](https://travis-ci.org/andrew-gresyk/HFSM2)
[![Gitter](https://badges.gitter.im/andrew-gresyk/HFSM2.svg)](https://gitter.im/andrew-gresyk/HFSM2)

# HFSM2: High-Performance Hierarchical Finite State Machine Framework

Header-only heriarchical FSM framework in C++14, with fully statically-defined structure (no dynamic allocations), built with variadic templates.

## Compiler Support

- Visual Studio 14, 15, 16
- GCC 5, 6, 7, 8, 9
- Clang 3.7, 3.8, 3.9, 4, 5, 6, 7, 8, 9, 10

---

## Documentation

[Old Wiki](https://github.com/andrew-gresyk/HFSM2/wiki/Tutorial) docs are being upgraded and moved to the [New GitBook](https://doc.hfsm.dev/).

In-line comment-based docs is an on-going effort.

---

## Feature Highlights

- Permissive [MIT License](https://github.com/andrew-gresyk/HFSM2/blob/master/LICENSE)
- Written in widely-supported modern(ish) C++11
- Header-only
- Convenient, minimal boilerplate
- Fully static, no dynamic allocations
- Uses inline-friendly compile-time pylymorphism, no virtual methods were harmed
- Type-safe transitions: `FSM.changeTo<TargetState>()`
- 100% NoUML-compliant
- [Hierarchical](https://github.com/andrew-gresyk/HFSM2/wiki/Transitions-within-Hierarchy), with a selection of composite (sub-machine) and orthogonal regions
- Gamedev-friendly, supports explicit `State::update()`
- Also supports traditional event-based workflow with `State::react()`
- AI-friendly with [Dynamic planning](https://github.com/andrew-gresyk/HFSM2/wiki/Plans) support
- [Utility theory](https://github.com/andrew-gresyk/HFSM2/wiki/Utility-Theory) support (both max score and ranked weighted random)
- Scaleable, supports robust state re-use via state injections
- [Debug-assisted](https://gresyk.dev/features/2018/01/15/hfsm-magic.html), includes automatic structure and activity visualization API with `#define HFSM_ENABLE_STRUCTURE_REPORT`
- [Serializable](https://doc.hfsm.dev/user-guide/debugging-and-tools/serialization), with activity and transition history support
- Built-in logging support

---

## 3rd Party Libraries

- [Catch2](https://github.com/catchorg/Catch2) unit testing framework
- [XoShiRo](http://xoshiro.di.unimi.it/) pseuto-random number generators

---

## Get Updates

- [Blog](https://andrew-gresyk.github.io/)
- [Twitter](https://www.twitter.com/andrew_gresyk)

---

## Get In Touch

Please share your comments and suggestions on:

- [Gitter](https://gitter.im/andrew-gresyk/HFSM2)
- [Twitter](https://www.twitter.com/andrew_gresyk)

---

## Special Thanks

- [Kevin Greene](https://github.com/kgreenek)
- [Kjeld Mathias Petersen](https://github.com/DonMathi)
- Mehdi Houshmand
- [Phil Nash](https://github.com/philsquared)
- [Romain Cheminade](https://github.com/romaincheminade)
- [Tristan Brindle](https://github.com/tcbrindle)

- [C++::London](https://www.meetup.com/CppLondon/) meetup
- programming community at [Splash Damage](http://www.splashdamage.com/)
