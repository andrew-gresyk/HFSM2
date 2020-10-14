[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Build status](https://ci.appveyor.com/api/projects/status/egs56khk70ud35un?svg=true)](https://ci.appveyor.com/project/andrew-gresyk/HFSM2)
[![Build Status](https://travis-ci.org/andrew-gresyk/HFSM2.svg?branch=master)](https://travis-ci.org/andrew-gresyk/HFSM2)
[![Gitter](https://badges.gitter.im/andrew-gresyk/HFSM2.svg)](https://gitter.im/andrew-gresyk/HFSM2)

---

![HFSM2](assets/logos/hfsm2-logo-large.png)

# [HFSM2](https://hfsm.dev): High-Performance Hierarchical Finite State Machine Framework

Header-only heriarchical FSM framework in C++11, with fully statically-defined structure (no dynamic allocations), built with variadic templates.

## Compiler Support

- Visual Studio 14, 15, 16
- GCC 5, 6, 7, 8, 9
- Clang 3.7, 3.8, 3.9, 4, 5, 6, 7, 8, 9, 10

---

## See Also

- **[FFSM2](https://flat.hfsm.dev)**: High-Performance **Flat** Finite State Machine Framework

---

## Documentation

- GitBook: **[doc.hfsm.dev](https://doc.hfsm.dev/)**
- Wiki: **[Tutorial](https://github.com/andrew-gresyk/HFSM2/wiki/Tutorial)** (Obsolete)
- In-line comments

---

## Feature Highlights

- Permissive **[MIT License](https://github.com/andrew-gresyk/FFSM2/blob/master/LICENSE)**
- Written in widely-supported modern(ish) C++11
- Header-only
- Convenient, minimal boilerplate
- Fully static, no dynamic allocations
- Uses inline-friendly compile-time pylymorphism, no virtual methods are used
- Type-safe transitions: `FSM.changeTo<TargetState>()` with optional payloads
- Scaleable, supports robust state re-use via state injections
- **[Hierarchical](https://github.com/andrew-gresyk/HFSM2/wiki/Transitions-within-Hierarchy)**, with a selection of composite (sub-machine) and orthogonal regions
- Gamedev-friendly, supports explicit `State::update()`
- Also supports traditional event-based workflow with `State::react()`
- Inspect anything: previous and current transitions, state activation status, and more!
- Game AI-friendly with **[dynamic planning](https://github.com/andrew-gresyk/HFSM2/wiki/Plans)** support
- **[Utility theory](https://github.com/andrew-gresyk/HFSM2/wiki/Utility-Theory)** support (max score and ranked weighted random)
- **[Serializable](https://doc.hfsm.dev/user-guide/debugging-and-tools/serialization)**, with activity and transition history support
- **[Debug-assisted](https://gresyk.dev/features/2018/01/15/hfsm-magic.html)**, includes automatic structure and activity visualization API with `#define HFSM_ENABLE_STRUCTURE_REPORT`
- Built-in logging support

---

## 3rd Party Libraries

- **[Catch2](https://github.com/catchorg/Catch2)** unit testing framework
- **[XoShiRo](http://xoshiro.di.unimi.it/)** pseuto-random number generators

---

## Receive Updates

- Blog: [gresyk.dev](https://gresyk.dev)
- Twitter: [@andrew_gresyk](https://www.twitter.com/andrew_gresyk)

---

## Get In Touch

- Discord: **[HFSM.dev](https://discord.gg/v4t3tzh)**
- Gitter: **[andrew-gresyk/HFSM2](https://gitter.im/andrew-gresyk/HFSM2)**

---

## Special Thanks

- [DJuego](https://github.com/DJuego)
- [Kevin Greene](https://github.com/kgreenek)
- [Kjeld Mathias Petersen](https://github.com/DonMathi)
- Mehdi Houshmand
- [Phil Nash](https://github.com/philsquared)
- [Romain Cheminade](https://github.com/romaincheminade)
- [Tristan Brindle](https://github.com/tcbrindle)

- [C++::London](https://www.meetup.com/CppLondon/) meetup
- programming community at [Splash Damage](http://www.splashdamage.com/)
