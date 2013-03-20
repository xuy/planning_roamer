#include "logistic_learner.h"

#include <algorithm>

#include "../domain_transition_graph.h"
#include "../search_space.h"

// Order each state.
LogisticLearner::LogisticLearner() {
  int offset = 0;
  for (int var = 0; var < g_variable_domain.size(); ++var) {
    int transition_count = g_transition_graphs[var]->get_transition_count();
    cout << "[Logistic Learner] Init the transition count on variable "
         << var << " is : " << transition_count << endl;
    one_hot_offset.push_back(offset);
    offset += transition_count;
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

// probably also cache a map from variable to offset index etc.
double LogisticLearner::predict(SearchNodeInfo* info,
                                map<int, int>* variable_offsets) {
  double prediction = 0.0;
  int var, origin, target, weight_index;
  for (auto& transition : info->creating_operator->get_pre_post()) {
    var = transition.var;
    origin = (transition.pre == -1 ? info->parent_state[var]: transition.pre);
    target = transition.post; 
    weight_index = one_hot_offset[var] + 
      g_transition_graphs[var]->get_transition_index(origin, target);
    (*variable_offsets)[var] = weight_index;
    prediction += weight[weight_index]; // defult x_i is 1 because we use one hot encoding.
  }
  return prediction;
}

// The actual method that extracts the first state variable from the state variable.
void LogisticLearner::learn(SearchNodeInfo* info, int parent_h) {
    int var, origin, target;
    cout << "[Logistic Learner] Delta h is " << info->h - parent_h << endl;
    vector<int> encoding;
    for (auto& transition : info->creating_operator->get_pre_post()) {
      var = transition.var;
      origin = transition.pre;
      // -1 means no fixed precondition, we ground origin to current state.
      if (origin == -1) {
        origin = info->parent_state[var];
      }
      target = transition.post;
      // TODO(xuy): skip the actual encoding, focus on calculating the 
      // gradient for weights directly.
      // target position = offset[var] + transition_index(origin, target) 
      // gradient: 2 * x_i * (delta_h - beta - prediction)
      // prediction using the predict() function provided.
      encoding.push_back(g_transition_graphs[var]->get_transition_index(origin, target));
      //  PrintDebugInfo(var, origin, target);
    }
    cout << "[Logistic Learner] transiton encoding -> ";
    for (auto& code : encoding) {
      cout << code << " ";
    }
    cout << endl;
    // TODO(xuy): stratify the transition space using stratified planning.
}
