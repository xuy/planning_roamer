#include "feature_extractor.h"
#include "../search_space.h"

// Feature extractor will extract simple and compound features from state varaibles.
FeatureExtractor::FeatureExtractor() {
}

FeatureExtractor::~FeatureExtractor() {
}

void first_state_variable(std::pair<const StateProxy, SearchNodeInfo> iter) {
    const State new_state(iter.first.state_data);
    // const SearchNodeInfo &info = iter.second;
    cout << "First var " << new_state[0] << endl;
    // cout << " h value: " << info.h << endl;
}

void FeatureExtractor::Extract(SearchSpace& space) {
  // TODO(xuy): instead of dumping, actually extract features and learn them.
  space.statistics();
  space.process_nodes(&first_state_variable);
}



