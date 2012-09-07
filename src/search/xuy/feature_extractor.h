#ifndef XUY_FEATURE_EXTRACTOR
#define XUY_FEATURE_EXTRACTOR

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

};

#endif  // XUY_FEATURE_EXTRACTOR
