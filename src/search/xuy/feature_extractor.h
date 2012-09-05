#ifndef XUY_FEATURE_EXTRACTOR
#define XUY_FEATURE_EXTRACTOR

class SearchSpace;

using namespace std;

class FeatureExtractor {
  public:
    FeatureExtractor();

    virtual ~FeatureExtractor();

    void Extract(SearchSpace& space);

};

#endif  // XUY_FEATURE_EXTRACTOR
