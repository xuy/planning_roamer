#include "feature_extractor.h"
#include "../search_space.h"

// Feature extractor will extract simple and compound features from state varaibles.
FeatureExtractor::FeatureExtractor() {
}

FeatureExtractor::~FeatureExtractor() {
}

void FeatureExtractor::Extract(SearchSpace& space) {
  // TODO(xuy): instead of dumping, actually extract features and learn them.
  space.statistics();
  space.dump();
}



