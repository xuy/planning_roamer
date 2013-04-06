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
    // The learning rate of this learniner. We pick learning_rate = 0.9
    double learning_rate_;
    
    // The constant part of the linear regression.
    /// Prediction = \sum x_i w_i  + beta. We pick \beta = 0.5.
    double beta_;

    vector<double> weight_;

    vector<int> one_hot_offset_;

    void PrintDebugInfo(int var, int origin, int target);

    void get_variable_offsets(const SearchNodeInfo* info , map<int, int>* variable_offsets);

    void gradient_descent(const map<int, int>& weight_indices, double error);

    void stochastic_gradient_descent(int var, int origin, int target);

    double predict(const map<int, int>& variable_offsets);

};

#endif  //  XUY_LOGISTIC_LEARNER_H
