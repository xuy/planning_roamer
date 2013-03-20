#ifndef XUY_LOGISTIC_LEARNER_H
#define XUY_LOGISTIC_LEARNER_H

#include <map>
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

  private:

    vector<double> weight;

    vector<int> one_hot_offset;

    void PrintDebugInfo(int var, int origin, int target);

    void stochastic_gradient_descent(int var, int origin, int target);

    double predict(SearchNodeInfo* info, map<int, int>* variable_offsets);
};

#endif  //  XUY_LOGISTIC_LEARNER_H
