#include "logistic_learner.h"

#include <algorithm>

#include "../search_space.h"

// Order each state.
LogisticLearner::LogisticLearner() {
}

LogisticLearner::~LogisticLearner() {
}

// The actual method that extracts the first state variable from the state variable.
void  LogisticLearner::learn(SearchNodeInfo* info, int parent_h) {
    cout << "[Logistic Learner] Delta h is " << info->h - parent_h << endl;
    // TODO(xuy): do something useful here based on state encoding etc.
    // useful data: info->h, info->parent_state and info->creating_operator
}
