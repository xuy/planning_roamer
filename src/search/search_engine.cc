#include <cassert>
#include <iostream>
#include <limits>
using namespace std;

#include "globals.h"
#include "search_engine.h"
#include "timer.h"
#include "option_parser.h"
//#include "xuy/feature_extractor.h"
#include "xuy/logistic_learner.h"
#include "xuy/state_order_tagger.h"

SearchEngine::SearchEngine(const Options &opts)
    : search_space(OperatorCost(opts.get_enum("cost_type"))),
      cost_type(OperatorCost(opts.get_enum("cost_type"))) {
    solved = false;
    if (opts.get<int>("bound") < 0) {
        cerr << "error: negative cost bound " << opts.get<int>("bound") << endl;
        exit(2);
    }
    bound = opts.get<int>("bound");

    // Please note: allocate callback functions on stack will cause it to be
    // destructed when it is out of scope.
    // Allocate them on the heap (use new) instead.

    // Inject various callback functions according to options.
    for (auto& callback : opts.get_list<string>("new_cb")) {
      if (callback == "order_tagger") {
        cout << "[Injection] Activate State Order Tagger." << endl;
        StateOrderTagger* tagger = new StateOrderTagger();
        SearchSpaceCallback* tagger_function =
            new SearchSpaceClosure<StateOrderTagger>(
                tagger, &StateOrderTagger::tag_state);
        search_space.add_new_node_callback(tagger_function);
      }
    }

    // TODO(xuy): see how to fit learner into the heuristic framework.
    for (auto& callback : opts.get_list<string>("open_cb")) {
      if (callback == "logistic") {
        cout << "[Injection] Activate Logistic Learner." << endl;
        LogisticLearner* learner = new LogisticLearner();
        SearchNodeOpenCallback* learner_function =
            new SearchNodeOpenClosure<LogisticLearner>(
                learner, &LogisticLearner::learn);
        search_space.set_open_node_callback(learner_function);
      }
    }
}

SearchEngine::~SearchEngine() {
}

void SearchEngine::statistics() const {
}

bool SearchEngine::found_solution() const {
    return solved;
}

const SearchEngine::Plan &SearchEngine::get_plan() const {
    assert(solved);
    return plan;
}

void SearchEngine::set_plan(const Plan &p) {
    solved = true;
    plan = p;
}

void SearchEngine::search() {
    initialize();
    Timer timer;
    while (step() == IN_PROGRESS)
        ;
    cout << "Actual search time: " << timer
         << " [t=" << g_timer << "]" << endl;
}

bool SearchEngine::check_goal_and_set_plan(const State &state) {
    if (test_goal(state)) {
        cout << "Solution found!" << endl;
        Plan plan;
        search_space.trace_path(state, plan);
        set_plan(plan);
        StateOrderTagger tagger;

        // Post search extraction if needed.
        // commented out by Eric to avoiding printing them all the time.
        // tagger.DumpTags(search_space);
        return true;
    }
    return false;
}

void SearchEngine::save_plan_if_necessary() const {
    if (found_solution())
        save_plan(get_plan(), 0);
}

int SearchEngine::get_adjusted_cost(const Operator &op) const {
    return get_adjusted_action_cost(op, cost_type);
}

void SearchEngine::add_options_to_parser(OptionParser &parser) {
    vector<string> cost_types;
    cost_types.push_back("NORMAL");
    cost_types.push_back("ONE");
    cost_types.push_back("PLUSONE");
    parser.add_enum_option("cost_type",
                           cost_types,
                           "NORMAL",
                           "operator cost adjustment type");
    parser.add_option<int>("bound",
                           numeric_limits<int>::max(),
                           "bound on plan cost");
    vector<string> default_value;
    parser.add_list_option<string>(
        "new_cb", default_value, "callbacks when new node is created");
    parser.add_list_option<string>(
        "open_cb", default_value, "callbacks when node is opened");
}
