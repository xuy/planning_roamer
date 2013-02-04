#include "feature_extractor.h"
#include "../search_space.h"

#include <algorithm>

// Feature extractor will extract simple and compound features from state varaibles.
FeatureExtractor::FeatureExtractor() {
    test_member = 1;
}

FeatureExtractor::~FeatureExtractor() {
}

// The actual method that extracts the first state variable from the state variable.
void FeatureExtractor::first_state_variable(StateProxy& proxy, SearchNodeInfo* iter) {
    const State new_state(proxy.state_data);
    cout << "First var " << new_state[0] << " test member " << test_member << endl;
}

// [Post search] This method will extract features from search space.
void FeatureExtractor::Extract(SearchSpace& space) {
    SearchSpaceCallback* closure = new SearchSpaceClosure<FeatureExtractor>(this, &FeatureExtractor::first_state_variable);
    space.process_nodes(closure);
    delete closure;
}
