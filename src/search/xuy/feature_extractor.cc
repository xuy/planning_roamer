#include "feature_extractor.h"
#include "../search_space.h"

#include <algorithm>

// Feature extractor will extract simple and compound features from state varaibles.
FeatureExtractor::FeatureExtractor() {
    test_member = 1;
}

FeatureExtractor::~FeatureExtractor() {
}

void FeatureExtractor::first_state_variable(std::pair<const StateProxy, SearchNodeInfo> iter) {
    const State new_state(iter.first.state_data);
    cout << "First var " << new_state[0] << " test member " << test_member << endl;
}

void FeatureExtractor::Extract(SearchSpace& space) {
    InfoNodeCallback* closure = new NodeMethodClosure<FeatureExtractor>(this, &FeatureExtractor::first_state_variable);
    space.process_nodes(closure);
}
