#ifndef XUY_LOGISTIC_LEARNER_H
#define XUY_LOGISTIC_LEARNER_H

#include <utility>

#include "../common_types.h"
#include "../search_node_info.h"

using namespace std;

class LogisticLearner {
  public:
    LogisticLearner();

    virtual ~LogisticLearner();

    // The learn function takes in two parameters, a SearchNodeInfo pointer
    // that represents the current state/node during search, and the heuristic
    // function value of its parent.
    void learn(SearchNodeInfo* info, int parent_h);
};

#endif  //  XUY_LOGISTIC_LEARNER_H
