#ifndef XUY_LOGISTIC_LEARNER_H
#define XUY_LOGISTIC_LEARNER_H

#include <utility>

class Operator;

using namespace std;

class LogisticLearner {
  public:
    LogisticLearner();

    virtual ~LogisticLearner();

    void learn(const int delta_h, const Operator* op);
};

#endif  //  XUY_LOGISTIC_LEARNER_H
