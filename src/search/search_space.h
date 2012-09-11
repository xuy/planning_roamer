#ifndef SEARCH_SPACE_H
#define SEARCH_SPACE_H

#include "state.h" // for state_var_t
#include <vector>
#include <ext/hash_map>
#include "callback.h"
#include "state.h"
#include "state_proxy.h"
#include "search_node_info.h"
#include "operator_cost.h"

#include <vector>

class Operator;
class State;
class StateProxy;

// TODO(xuy): transform the following to use callback.{h/cc}.

typedef std::pair<const StateProxy, SearchNodeInfo> SearchSpaceArg;

class NodeCallback : public std::unary_function<SearchSpaceArg, void> {
  public:
    virtual void operator() (SearchSpaceArg /*unused_arg*/) const = 0;
};

template <typename Class>
class NodeMethodClosure : public NodeCallback {
 public:
  typedef void (Class::*MethodType)(SearchSpaceArg);

  NodeMethodClosure(Class* object, MethodType method)
    : object_(object), method_(method) {}
  ~NodeMethodClosure() {}

  virtual void operator() (SearchSpaceArg arg) const { 
    (object_->*method_)(arg);
  }

 private:
  Class* object_;
  MethodType method_;
};

class SearchNode {
    state_var_t *state_buffer;
    SearchNodeInfo &info;
    OperatorCost cost_type;
public:
    SearchNode(state_var_t *state_buffer_, SearchNodeInfo &info_, OperatorCost cost_type_);

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

    void dump();
};

class SearchSpace {
    class HashTable;
    HashTable *nodes;
    OperatorCost cost_type;

public:
    SearchSpace(OperatorCost cost_type_);
    ~SearchSpace();
    int size() const;
    SearchNode get_node(const State &state);
    void trace_path(const State &goal_state,
                    std::vector<const Operator *> &path) const;

    void dump();
    void statistics() const;

    void process_nodes(const NodeCallback* callback);

    static void dump_node(const pair<StateProxy, SearchNodeInfo>& iter);
};

#endif
