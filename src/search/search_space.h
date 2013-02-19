#ifndef SEARCH_SPACE_H
#define SEARCH_SPACE_H

#include "state.h" // for state_var_t
#include <vector>
#include <ext/hash_map>
#include "search_callback.h"
#include "state.h"
#include "state_proxy.h"
#include "search_node_info.h"
#include "operator_cost.h"

#include <vector>

class Operator;
class State;

// A handy type used for SearchSpace callbacks.
typedef std::pair<const StateProxy, SearchNodeInfo> InfoNode;

// TODO(xuy): Split the callback functions into two types:
//      1. SearchSpace callback on the creation of new nodes (get_node). [done]
//      2. SearchNode callback on the opening of the node (open).
//
// Stored in SearchSpace, contains everything related to search. SearchSpace is a
// collection of SearchNodes, indexed by states.
class SearchNode {
    state_var_t *state_buffer;
    SearchNodeInfo &info;
    OperatorCost cost_type;
    SearchNodeOpenCallback* open_callback_;
    
public:
    SearchNode(state_var_t *state_buffer_, SearchNodeInfo &info_, 
        OperatorCost cost_type_, SearchNodeOpenCallback* open_callback_);

    state_var_t *get_state_buffer() {
        return state_buffer;
    }
    State get_state() const;

    bool is_new() const;
    bool is_open() const;
    bool is_closed() const;
    bool is_dead_end() const;

    bool is_h_dirty() const;
    void set_h_dirty();
    void clear_h_dirty();
    int get_g() const;
    int get_real_g() const;
    int get_h() const;
    const state_var_t *get_parent_buffer() const;

    void open_initial(int h);

    void open(int h, const SearchNode &parent_node,
              const Operator *parent_op);
    void reopen(const SearchNode &parent_node,
                const Operator *parent_op);
    void update_parent(const SearchNode &parent_node,
                       const Operator *parent_op);
    void increase_h(int h);
    void close();
    void mark_as_dead_end();

    void dump() const;
};

// A SearchSpace maintains a hash table that maps states to their
// SearchNodeInfo, which contains h, g and other values related to a state.
class SearchSpace {
    class HashTable;
    HashTable *nodes;
    OperatorCost cost_type;
    // A list of callback functions that will be invoked whenever "get_node"
    // is called.
    vector<SearchSpaceCallback*> get_node_callbacks;

    // A list of callback functions that will be invoked whenever "get_node"
    // is called AND a new node is generated (never seen it before).
    vector<SearchSpaceCallback*> new_node_callbacks;
    
    // A callback functions that will be invoked whenever "SearchNode::open"
    // is called on a (node, op) pair.
    SearchNodeOpenCallback* open_node_callback_;

public:
    SearchSpace(OperatorCost cost_type_);
    ~SearchSpace();
    int size() const;
    SearchNode get_node(const State &state);
    void trace_path(const State &goal_state,
                    std::vector<const Operator *> &path) const;

    void dump();
    void statistics() const;
    static void dump_node(const InfoNode& node);

    // Methods related to feature extraction. Takes in a callback function
    // and iterate it through all nodes in the search space.
    void process_nodes(const SearchSpaceCallback* callback);
    void add_get_node_callback(SearchSpaceCallback* callback);
    void add_new_node_callback(SearchSpaceCallback* callback);
    void set_open_node_callback(SearchNodeOpenCallback* callback);
};

#endif
