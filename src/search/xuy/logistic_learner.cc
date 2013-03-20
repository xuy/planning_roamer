#include "logistic_learner.h"

#include <algorithm>
#include <random>

#include "../domain_transition_graph.h"
#include "../search_space.h"

// TODO(xu): a general logging framework instead of cout.
// Sets the offsets for each DTG, and assigns the initial weights
// for each one hot assignment.
LogisticLearner::LogisticLearner() {
  int offset = 0;
  for (int var = 0; var < g_variable_domain.size(); ++var) {
    int transition_count = g_transition_graphs[var]->get_transition_count();
    cout << "[Logistic Learner] Init the transition count on variable "
         << var << " is : " << transition_count << endl;
    one_hot_offset_.push_back(offset);
    offset += transition_count;
  }
  // Assign random weight (-1,1).
  assert(offset > 0);
  weight_.resize(offset);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(-1, 1);
  for (int index = 0; index < weight_.size(); ++index) {
    weight_[index] = dis(gen);
  }
  cout << "Done with Logistic Learning Initialization" << endl;
}

LogisticLearner::~LogisticLearner() {
}

void LogisticLearner::PrintDebugInfo(int var, int origin, int target) {
  cout<< var << "(" << g_variable_domain[var] << ") : "
      << origin << " -> " << target << " (# "
      << g_transition_graphs[var]->get_transition_index(origin, target)
      << ")" << endl;
}

// TODO(xuy): online training for linear regression. See the Simple Note
//  for the linear regression formulation.
void LogisticLearner::stochastic_gradient_descent(int var, int origin, int target) {
  cout << var << origin << target;
  cout << "Stochastic Gradient";
}

// Get the variable offset for each transition appeared in the operator
// that leads to the search node. This function is used by predict
// and gradient descent.
void LogisticLearner::get_variable_offsets(
    const SearchNodeInfo* info , map<int, int>* variable_offsets) {
  int var, origin;
  for (auto& transition : info->creating_operator->get_pre_post()) {
    var = transition.var;
    origin = (transition.pre == -1 ? info->parent_state[var]: transition.pre);
    (*variable_offsets)[var] = one_hot_offset_[var] +
        g_transition_graphs[var]->get_transition_index(origin, transition.post);
  }
}

// probably also cache a map from variable to offset index etc.
double LogisticLearner::predict(const map<int, int>& weight_indices) {
  double prediction = 0.0;
  for (auto& var_index_pair : weight_indices) {
    prediction += weight_.at(var_index_pair.second);
    // It is actually prediction += weight_i * x_i. Here x_i = 1.
  }
  return prediction;
}

// The actual method that extracts the first state variable from the state variable.
void LogisticLearner::learn(SearchNodeInfo* info, int parent_h) {
    cout << "[Logistic Learner] Delta h is " << info->h - parent_h << endl;
    map<int, int> one_hot_indices;
    get_variable_offsets(info, &one_hot_indices);
    double prediction = predict(one_hot_indices);
    cout << "Prediction is " << prediction << endl;
    // TODO(xuy):
    // gradient: 2 * x_i * (delta_h - beta - prediction)
    //  PrintDebugInfo(var, origin, target);
    /*
    cout << "[Logistic Learner] transiton encoding -> ";
    for (auto& code : encoding) {
      cout << code << " ";
    }
    cout << endl;
    */
    // TODO(xuy): stratify the transition space using stratified planning.
}
