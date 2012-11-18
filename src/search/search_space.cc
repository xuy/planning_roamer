get#include "search_space.h"
#include "state.h"
#include "operator.h"

#include <cassert>
#include <ext/hash_map>
#include "state_proxy.h"
#include "search_node_info.h"

using namespace std;
using namespace __gnu_cxx;




SearchNode::SearchNode(state_var_t *state_buffer_, SearchNodeInfo &info_, OperatorCost cost_type_)
    : state_buffer(state_buffer_), info(info_), cost_type(cost_type_) {
}

State SearchNode::get_state() const {
    return State(state_buffer);
}

bool SearchNode::is_open() const {
    return info.status == SearchNodeInfo::OPEN;
}

bool SearchNode::is_closed() const {
    return info.status == SearchNodeInfo::CLOSED;
}

bool SearchNode::is_dead_end() const {
    return info.status == SearchNodeInfo::DEAD_END;
}

bool SearchNode::is_new() const {
    return info.status == SearchNodeInfo::NEW;
}

int SearchNode::get_g() const {
    return info.g;
}

int SearchNode::get_real_g() const {
    return info.real_g;
}

int SearchNode::get_h() const {
    return info.h;
}

bool SearchNode::is_h_dirty() const {
    return info.h_is_dirty;
}

void SearchNode::set_h_dirty() {
    info.h_is_dirty = true;
}

void SearchNode::clear_h_dirty() {
    info.h_is_dirty = false;
}

const state_var_t *SearchNode::get_parent_buffer() const {
    return info.parent_state;
}

void SearchNode::open_initial(int h) {
    assert(info.status == SearchNodeInfo::NEW);
    info.status = SearchNodeInfo::OPEN;
    info.g = 0;
    info.real_g = 0;
    info.h = h;
    info.parent_state = 0;
    info.creating_operator = 0;
}

void SearchNode::open(int h, const SearchNode &parent_node,
                      const Operator *parent_op) {
    assert(info.status == SearchNodeInfo::NEW);
    info.status = SearchNodeInfo::OPEN;
    info.g = parent_node.info.g + get_adjusted_action_cost(*parent_op, cost_type);
    info.real_g = parent_node.info.real_g + parent_op->get_cost();
    info.h = h;
    info.parent_state = parent_node.state_buffer;
    info.creating_operator = parent_op;
}

void SearchNode::reopen(const SearchNode &parent_node,
                        const Operator *parent_op) {
    assert(info.status == SearchNodeInfo::OPEN ||
           info.status == SearchNodeInfo::CLOSED);

    // The latter possibility is for inconsistent heuristics, which
    // may require reopening closed nodes.
    info.status = SearchNodeInfo::OPEN;
    info.g = parent_node.info.g + get_adjusted_action_cost(*parent_op, cost_type);
    info.real_g = parent_node.info.real_g + parent_op->get_cost();
    info.parent_state = parent_node.state_buffer;
    info.creating_operator = parent_op;
}

// like reopen, except doesn't change status
void SearchNode::update_parent(const SearchNode &parent_node,
                               const Operator *parent_op) {
    assert(info.status == SearchNodeInfo::OPEN ||
           info.status == SearchNodeInfo::CLOSED);
    // The latter possibility is for inconsistent heuristics, which
    // may require reopening closed nodes.
    info.g = parent_node.info.g + get_adjusted_action_cost(*parent_op, cost_type);
    info.real_g = parent_node.info.real_g + parent_op->get_cost();
    info.parent_state = parent_node.state_buffer;
    info.creating_operator = parent_op;
}

void SearchNode::increase_h(int h) {
    assert(h >= info.h);
    info.h = h;
}

void SearchNode::close() {
    assert(info.status == SearchNodeInfo::OPEN);
    info.status = SearchNodeInfo::CLOSED;
}

void SearchNode::mark_as_dead_end() {
    info.status = SearchNodeInfo::DEAD_END;
}

void SearchNode::dump() {
    cout << state_buffer << ": ";
    State(state_buffer).dump();
    cout << " created by " << info.creating_operator->get_name()
         << " from " << info.parent_state << endl;
}

class SearchSpace::HashTable
    : public __gnu_cxx::hash_map<StateProxy, SearchNodeInfo> {
    // This is more like a typedef really, but we need a proper class
    // so that we can hide the information in the header file by using
    // a forward declaration. This is also the reason why the hash
    // table is allocated dynamically in the constructor.
};


SearchSpace::SearchSpace(OperatorCost cost_type_)
    : cost_type(cost_type_) {
    nodes = new HashTable;
}

SearchSpace::~SearchSpace() {
    delete nodes;
}

int SearchSpace::size() const {
    return nodes->size();
}

SearchSpace::add_new_node_callback(Callback* callback) {

}

// The key function for state space search. It is called to transform a state to a node in
// state space. This is the place to add callback functions.
SearchNode SearchSpace::get_node(const State &state) {
    static SearchNodeInfo default_info;
    InfoNode info_node = make_pair(StateProxy(&state), default_info);
    pair<HashTable::iterator, bool> result = nodes->insert(info_node);
    if (result.second) {
        // This is a new entry: Must give the state permanent lifetime.
        result.first->first.make_permanent();
        // Invoke callback functions that is applied to only new nodes.
        invoke_callbacks(info_node, new_node_callbacks);
    }
    // Invoke callback functions that is applied to all get_node calls.
    invoke_callbacks(info_node, get_node_callbacks);
    HashTable::iterator iter = result.first;
    return SearchNode(iter->first.state_data, iter->second, cost_type);
}

void SearchSpace::trace_path(const State &goal_state,
                             vector<const Operator *> &path) const {
    StateProxy current_state(&goal_state);
    assert(path.empty());
    for (;;) {
        HashTable::const_iterator iter = nodes->find(current_state);
        assert(iter != nodes->end());
        const SearchNodeInfo &info = iter->second;
        const Operator *op = info.creating_operator;
        if (op == 0)
            break;
        path.push_back(op);
        current_state = StateProxy(const_cast<state_var_t *>(info.parent_state));
    }
    reverse(path.begin(), path.end());
}

void SearchSpace::dump_node(const InfoNode& node) {
   cout << "#" << " (" << node.first.state_data << "): ";
   State(node.first.state_data).dump();
   const SearchNodeInfo &info = node.second;
   cout << " h value: " << info.h << endl;
   if (node.second.creating_operator &&
       node.second.parent_state) {
       cout << " created by " << node.second.creating_operator->get_name()
            << " from " << node.second.parent_state << endl;
   } else {
       cout << "has no parent" << endl;
   }
}

void SearchSpace::dump() {
    for (auto& iter : *nodes) {
      dump_node(iter);
    }
}

void SearchSpace::statistics() const {
    cout << "Search space hash size: " << nodes->size() << endl;
    cout << "Search space hash bucket count: " << nodes->bucket_count() << endl;
}

/* Methods used by feature extraction.  */
void SearchSpace::process_nodes(const InfoNodeCallback* callback) {
    for (auto& iter : *nodes) {
      callback->operator()(iter);
    }
}

void SearchSpace::invoke_callbacks(const InfoNode& node,
    const vector<InfoNodeCallback*>& callbacks) {
  for (auto node_callback : callbacks) {
    node_callback->operator()(node);
  }
}

void SearchSpace::add_new_node_callback(InfoNodeCallback* callback) {
  new_node_callbacks.push_back(callback);
}

void SearchSpace::add_node_callback(InfoNodeCallback* callback) {
  get_node_callbacks.push_back(callback);
}

