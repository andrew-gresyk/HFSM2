// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_PLANS
#include <hfsm2/machine.hpp>

#include <catch2/catch.hpp>
#undef assert
#define assert(x) REQUIRE(x)

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Wiki.Plans.Traffic Light", "[Wiki]") {
    using M = hfsm2::Machine;          // stateID

    using FSM = M::Root<struct Apex,   //      0
                    struct Red,        //      1
                    struct Yellow,     //      2
                    struct Green       //      3
                >;

    //----------------------------------------------------------------------

    struct Apex
        : FSM::State
    {
        void enter(PlanControl& control) {
            // create an empty plan
            auto plan = control.plan();

            // 'Red' will be implicitly activated as an initial substate
            // of the composite root region
            //
            // loop yellow -> green -> yellow -> red
            plan.change<Red, Yellow>();
            plan.change<Yellow, Green>();
            plan.change<Green, Yellow>();
            plan.change<Yellow, Red>();
        }
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    struct Red    : FSM::State {
        void update(FullControl& control) { control.succeed(); }
    };

    struct Yellow : FSM::State {
        void update(FullControl& control) { control.succeed(); }
    };

    struct Green  : FSM::State {
        void update(FullControl& control) { control.succeed(); }
    };

    //----------------------------------------------------------------------

    FSM::Instance fsm;

    assert(fsm.isActive<Red>());

    fsm.update();
    assert(fsm.isActive<Yellow>());

    fsm.update();
    assert(fsm.isActive<Green>());

    fsm.update();
    assert(fsm.isActive<Yellow>());

    fsm.update();
    assert(fsm.isActive<Red>());
}

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Wiki.Plans.Detailed Demo", "[Wiki]") {
    using M = hfsm2::Machine;                                 // stateID

    using FSM = M::Root<struct Apex,                          //      0
                    M::Composite<struct PlanOwner,            //      1
                        struct StateTask,                     //      2
                        M::Composite<struct CompositeTask,    //      3
                            struct CT_Initial,                //      4
                            struct CT_Following               //      5
                        >,
                        M::Orthogonal<struct OrthogonalTask,  //      6
                            struct OT_1,                      //      7
                            struct OT_2                       //      8
                        >,
                        struct ReplanTask,                    //      9
                        M::Composite<struct SubPlanOwner,     //     10
                            struct SubTask_1,                 //     11
                            struct SubTask_2                  //     12
                        >,
                        struct End                            //     13
                    >
                >;

    //----------------------------------------------------------------------

    struct Apex : FSM::State {};

    struct PlanOwner
        : FSM::State
    {
        void enter(PlanControl& control) {
            auto plan = control.plan();

            // build the plan by sequencing transitions
            // StateTask -> CompositeTask -> OrthogonalTask -> SubPlanOwner
            //
            // sequence links can be ordred arbitrarily
            // here - ordered linearly for readability
            plan.restart<StateTask, CompositeTask>();
            plan.change<CompositeTask, OrthogonalTask>();
            plan.change<OrthogonalTask, ReplanTask>();
            // skipping SubPlanOwner on purpose, see ReplanTask
            plan.change<ReplanTask, End>();
        }

        // optional plan execution result reactions
        // can be used to alter the execution flow
        //void planSucceeded(FullControl& control) {}

        // respond to plan failure below
        void planFailed(FullControl& control) {
            control.changeTo<End>();
        }
    };

    struct StateTask
        : FSM::State
    {
        void update(FullControl& control) {
            control.succeed();
        }
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    struct CompositeTask : FSM::State {};

    struct CT_Initial
        : FSM::State
    {
        void update(FullControl& control) {
            // mark the task as successful
            control.changeTo<CT_Following>();
        }
    };

    struct CT_Following
        : FSM::State
    {
        void update(FullControl& control) {
            // even though CompositeTask has no plan attached to it,
            // the sub-state can 'succeed' the entire region
            control.succeed();
        }
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    struct OrthogonalTask    : FSM::State {};

    struct OT_1              : FSM::State {};

    struct OT_2
        : FSM::State
    {
        void update(FullControl& control) {
            // the task can also be marked successful
            // from its sub-state one level down
            control.succeed();
        }
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    struct ReplanTask
        : FSM::State
    {
        void update(FullControl& control) {
            // parametrized version of PlanControl::plan() allows access to plans
            // hosted by any region
            auto plan = control.plan<PlanOwner>();
            assert(plan);

            // inspect the plan
            auto taskIterator = plan.first();
            assert(taskIterator);
            assert(taskIterator->origin == control.stateId<ReplanTask>());
            assert(taskIterator->destination == control.stateId<End>());

            // loop over plan task sequence
            for (auto it = plan.first(); it; ++it) {
                if (it->destination == control.stateId<End>())
                    // and remove task links
                    it.remove();
            }

            // when the plan is empty it is reported as 'invalid'
            assert(!plan);

            // plan can be explicitly cleared too
            plan.clear();

            // and re-populated
            plan.change<ReplanTask, SubPlanOwner>();

            control.succeed();
        }
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    struct SubPlanOwner
        : FSM::State
    {
        // guard gets executed before the first sub-state activates implicitly ..
        void entryGuard(GuardControl& control) {
            // .. so the plan can start from the second sub-state
            control.changeTo<SubTask_2>();

            // and then continue in the reverse order
            control.plan().change<SubTask_2, SubTask_1>();
        }

        // without plan execution reactions, plan results
        // are propagated to the outer plan
        //
        //void planSucceeded(FullControl& control) {}
        //void planFailed(FullControl& control) {}
    };

    // these appear in the plan in reverse order
    struct SubTask_1
        : FSM::State
    {
        void update(FullControl& control) {
            // owner region's planFailed() gets called in response to plan failure
            control.fail();
        }
    };

    struct SubTask_2
        : FSM::State
    {
        void update(FullControl& control) {
            // continue in reverse order, as defined by the plan in SubPlanOwner
            control.succeed();
        }
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    struct End
        : FSM::State
    {};

    //----------------------------------------------------------------------

    FSM::Instance fsm;

    assert(fsm.isActive<PlanOwner>());
    assert(fsm.isActive<StateTask>());

    fsm.update();
    assert(fsm.isActive<CompositeTask>());
    assert(fsm.isActive<CT_Initial>());

    fsm.update();
    assert(fsm.isActive<CompositeTask>());
    assert(fsm.isActive<CT_Following>());

    fsm.update();
    assert(fsm.isActive<OrthogonalTask>());
    assert(fsm.isActive<OT_1>());
    assert(fsm.isActive<OT_2>());

    fsm.update();
    assert(fsm.isActive<ReplanTask>());

    fsm.update();
    assert(fsm.isActive<SubPlanOwner>());
    assert(fsm.isActive<SubTask_2>());

    fsm.update();
    assert(fsm.isActive<SubPlanOwner>());
    assert(fsm.isActive<SubTask_1>());

    fsm.update();
    assert(fsm.isActive<End>());
}

////////////////////////////////////////////////////////////////////////////////
