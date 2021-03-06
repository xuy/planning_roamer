#ifndef XUY_FEATURE_EXTRACTOR_H
#define XUY_FEATURE_EXTRACTOR_H

#include <utility>

class SearchNodeInfo;
class SearchSpace;
class StateProxy;

using namespace std;

class FeatureExtractor {
  public:
    FeatureExtractor();

    virtual ~FeatureExtractor();

    void Extract(SearchSpace& space);

    void first_state_variable(std::pair<const StateProxy, SearchNodeInfo> iter);
  private:
    int test_member;
};

#endif  // XUY_FEATURE_EXTRACTOR_H