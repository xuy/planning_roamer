#include "logistic_learner.h"
#include "../search_space.h"

#include <algorithm>

// Order each state.
LogisticLearner::LogisticLearner() {
}

LogisticLearner::~LogisticLearner() {
}

// The actual method that extracts the first state variable from the state variable.
void LogisticLearner::learn(const int delta_h, const Operator* op) {
  if (op!= NULL) {
    cout << "delta h is " << delta_h << endl;
  }
}
