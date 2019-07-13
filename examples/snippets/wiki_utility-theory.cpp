// HFSM (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM_ENABLE_LOG_INTERFACE
#define HFSM_ENABLE_VERBOSE_DEBUG_LOG
#include <hfsm2/machine.hpp>
#include <catch2/catch.hpp>

//#include <vector> // already included in catch.hpp

////////////////////////////////////////////////////////////////////////////////

struct Event {
    enum Enum {
        RANK,
        UTILITY,
        UTILITY_RESOLUTION,
        RANDOM_RESOLUTION,

        COUNT
    };

    Event(const hfsm2::StateID state_,
          const Enum type_,
          const hfsm2::StateID prong_ = hfsm2::INVALID_STATE_ID)
        : state{state_}
        , type{type_}
        , prong{prong_}
    {}

    hfsm2::StateID state;
    Enum type;
    hfsm2::StateID prong;
};
using Events = std::vector<Event>;

//------------------------------------------------------------------------------

struct Logger
    : hfsm2::LoggerInterface    // requires HFSM_ENABLE_LOG_INTERFACE defined
{
    void recordMethod(const StateID state,
                      const Method method) override
    {
        if (method == Method::RANK)
            history.emplace_back(state, Event::RANK);
        else if (method == Method::UTILITY)
            history.emplace_back(state, Event::UTILITY);
    }

    void recordUtilityResolution(const StateID head,
                                 const StateID prong,
                                 const Utilty /*utilty*/) override
    {
        history.emplace_back(head, Event::UTILITY_RESOLUTION,  prong);
    }

    void recordRandomResolution(const StateID head,
                                const StateID prong,
                                const Utilty /*utilty*/) override
    {
        history.emplace_back(head, Event::RANDOM_RESOLUTION, prong);
    }

    void assertSequence(const Events& reference)
    {
        const auto count = std::max(history.size(), reference.size());

        for (unsigned i = 0; i < count; ++i) {
            REQUIRE(i < history.size()); //-V521
            REQUIRE(i < reference.size()); //-V521

            if (i < history.size() &&
                i < reference.size())
            {
                REQUIRE(history[i].state == reference[i].state); //-V521
                REQUIRE(history[i].type  == reference[i].type); //-V521
                REQUIRE(history[i].prong == reference[i].prong); //-V521
            }
        }

        history.clear();
    }

    Events history;
};

//------------------------------------------------------------------------------

using M = hfsm2::Machine;

using FSM = M::PeerRoot<
                struct Origin,
                M::Utilitarian<struct Destination,
                    struct S,
                    M::Composite<struct C,
                        struct C_Initial,
                        struct C_1
                    >,
                    M::Resumable<struct R,
                        struct R_0,
                        struct R_Activated
                    >,
                    M::Utilitarian<struct U,
                        struct U_033,
                        struct U_067
                    >,
                    M::Random<struct D,
                        struct D_Filtered,
                        struct D_010,
                        struct D_090
                    >,
                    M::Orthogonal<struct O,
                        struct O_0,
                        struct O_1
                    >
                >
            >;

//------------------------------------------------------------------------------
// with HFSM_ENABLE_VERBOSE_DEBUG_LOG inherited state methods will also be logged

struct Origin       : FSM::State {};

struct Destination  : FSM::State {};
struct S            : FSM::State {};

struct C            : FSM::State {};  // for 'Composite' region,
struct C_Initial    : FSM::State {};  // only consider the initial sub-state
struct C_1          : FSM::State {};

struct R            : FSM::State {};  // for 'Resumable' region,
struct R_0          : FSM::State {};
struct R_Activated  : FSM::State {};  // only consider previously activated sub-state

struct U            : FSM::State {};  // for 'Utilitarian' region,
                                      // find the highest-scoring sub-state
struct U_033        : FSM::State {
    Utility utility(const Control&) { return 0.33f; }
};

struct U_067        : FSM::State {
    Utility utility(const Control&) { return 0.67f; }
};

struct D            : FSM::State {};  // for 'Random' region,
                                      // 1. filter out low-ranking sub-states
                                      // 2. from the remaining sub-states,
                                      // randomly select one based on their score
struct D_Filtered   : FSM::State {
    Rank rank(const Control&) {return -1; }
};

struct D_010        : FSM::State {
    Rank rank(const Control&) {return 0; }
    Utility utility(const Control&) { return 0.10f; }
};

struct D_090        : FSM::State {
    // inherit FSM::State::rank(), which returns '0'
    Utility utility(const Control&) { return 0.90f; }
};

struct O            : FSM::State {};
struct O_0          : FSM::State {};
struct O_1          : FSM::State {};

//------------------------------------------------------------------------------

TEST_CASE("Utility Theory", "[Wiki]") {
    Logger logger;
    FSM::Instance fsm{&logger};
    REQUIRE(fsm.isActive<Origin>());        // Initial activation

    fsm.changeTo<R_Activated>();
    fsm.update();
    REQUIRE(fsm.isActive<R_Activated>());   // Prepare resumable state

    fsm.changeTo<Origin>();
    fsm.update();
    REQUIRE(fsm.isActive<Origin>());        // Ready for the main test

    fsm.changeTo<Destination>();
    fsm.update();
    REQUIRE(fsm.isActive<Destination>());
    REQUIRE(fsm.isActive<S>());

    const Events reference = {
        { FSM::stateId<S>(),            Event::UTILITY },

        { FSM::stateId<C>(),            Event::UTILITY },
        { FSM::stateId<C_Initial>(),    Event::UTILITY },

        { FSM::stateId<R>(),            Event::UTILITY },
        { FSM::stateId<R_Activated>(),  Event::UTILITY },

        { FSM::stateId<U>(),            Event::UTILITY },
        { FSM::stateId<U_033>(),        Event::UTILITY },
        { FSM::stateId<U_067>(),        Event::UTILITY },
        { FSM::stateId<U>(),            Event::UTILITY_RESOLUTION, 1 }, // 'U_067' selected

        { FSM::stateId<D>(),            Event::UTILITY },
        { FSM::stateId<D_Filtered>(),   Event::RANK }, // will be filtered out, score not checked
        { FSM::stateId<D_010>(),        Event::RANK },
        { FSM::stateId<D_090>(),        Event::RANK },

        { FSM::stateId<D_010>(),        Event::UTILITY },
        { FSM::stateId<D_090>(),        Event::UTILITY },
        { FSM::stateId<D>(),            Event::RANDOM_RESOLUTION, 2 }, // 'D_090' selected

        { FSM::stateId<O>(),            Event::UTILITY },
        { FSM::stateId<O_0>(),          Event::UTILITY },
        { FSM::stateId<O_1>(),          Event::UTILITY },
        { FSM::stateId<O>(),            Event::UTILITY_RESOLUTION },

        { FSM::stateId<Destination>(),  Event::UTILITY_RESOLUTION, 0 }, // 'S' selected
    };
    logger.assertSequence(reference);
}

////////////////////////////////////////////////////////////////////////////////
