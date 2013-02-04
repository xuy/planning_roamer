#include "state_order_tagger.h"
#include "../search_space.h"

#include <algorithm>

// Order each state.
StateOrderTagger::StateOrderTagger() {
    order = 0;
    //#data.open("data.txt", fstream::out);
}

StateOrderTagger::~StateOrderTagger() {
    //data.close();
}

// The actual method that extracts the first state variable from the state variable.
void StateOrderTagger::tag_state(const StateProxy&, SearchNodeInfo* info) {
    order++;
    info->order = order;
}

void StateOrderTagger::print_tags(const StateProxy&, SearchNodeInfo* info) {
    cout << info->order << "," << info->h << endl;
}

// [Post search] This method will extract features from search space.
void StateOrderTagger::DumpTags(SearchSpace& space) {
    SearchSpaceCallback* closure = new SearchSpaceClosure<StateOrderTagger>(
	this, &StateOrderTagger::print_tags);
    cout << ">>>>>>>" << endl;
    space.process_nodes(closure);
    cout << "<<<<<<<" << endl;
    delete closure;
}
