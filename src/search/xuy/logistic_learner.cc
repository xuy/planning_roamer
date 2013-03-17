#include "logistic_learner.h"

#include <algorithm>

#include "../domain_transition_graph.h"
#include "../search_space.h"

// Order each state.
LogisticLearner::LogisticLearner() {
}

LogisticLearner::~LogisticLearner() {
}

void LogisticLearner::PrintDebugInfo(int var, int origin, int target) {
  if (origin != target) {
    cout<< var << "(" << g_variable_domain[var] << ") : "
        << origin << " -> " << target << " (# "
        << g_transition_graphs[var]->get_transition_index(origin, target)
        << ")" << endl;
  }
}

// The actual method that extracts the first state variable from the state variable.
void LogisticLearner::learn(SearchNodeInfo* info, int parent_h) {
    int var, origin, target;
    cout << "[Logistic Learner] Delta h is " << info->h - parent_h << endl;
    for (auto& transition : info->creating_operator->get_pre_post()) {
      var = transition.var;
      origin = transition.pre;
      // -1 means no fixed precondition, we ground origin to current state.
      if (origin == -1) {
        origin = info->parent_state[var];
      }
      target = transition.post;
      PrintDebugInfo(var, origin, target);
      // TODO(xuy): online training for linear regression. See the Simple Note
      //  for the linear regression formulation.
    }
    // TODO(xuy): stratify the transition space using stratified planning.
}
