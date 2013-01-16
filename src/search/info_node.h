#ifndef INFO_NODE_H
#define INFO_NODE_H

#include "search_node_info.h"
#include "state_proxy.h"

class StateProxy;

// StateProxy is defined in state_proxy.h. It is a pointer that maps to an
// underlying storage area to represent a state. SearchNodeInfo SearchNodeInfo
// is defined in search_node_info.h. It provides extra fields to support graph
// search.  For instance, the NEW/OPEN/CLOSE info and the g and h values are
// all stored in SearchNodeInfo.  A SearchSpace maintains a hash table that
// maps states to their SearchNodeInfo. InfoNode is used as a handy type alias
// to represent those entries in the hash table.
typedef std::pair<const StateProxy, SearchNodeInfo> InfoNode;

// Ideally, all callback functions that inspects the search space at search time
// should take InfoNode as a parameter. However, C++ pairs are always copy-constructed,
// meaning any changes made to InfoNode in callback does not actually change the
// entry stored in StateSpace's hashtable. So, all callback functiosn are taking
// in two parameters, StateProxy and SearchNodeInfo*, instead of InfoNode.

class NodeCallback : public std::binary_function<const StateProxy&, SearchNodeInfo*, void> {
  public:
    virtual void operator() (
        const StateProxy& /*unused_arg*/, SearchNodeInfo* /*unused_arg*/) const = 0;
    virtual ~NodeCallback() {}
};

// Note to users: always use InfoNodeCallback pointers to denote
// callbacks, but use NodeMethodClosure to instanciate objects.
// e.g.
//  InfoNodeCallback* closure =
//      NodeMethodClosure<FeatureExtractor>(
//          SomeClassObject, &SomeClass::member_method);
// Later we might also implement a class-less closure. e.g.
//  InfoNodeCallback* closure =
//      NodeFunction(some_method);
// It is less useful, however, because it does not maintain inner states.
template <typename Class>
class NodeMethodClosure : public NodeCallback {
 public:
  typedef void (Class::*MethodType)(const StateProxy&, SearchNodeInfo*);

  NodeMethodClosure(Class* object, MethodType method)
    : object_(object), method_(method) {}
  virtual ~NodeMethodClosure() {}

  virtual void operator() (const StateProxy& arg1, SearchNodeInfo* arg2) const { 
    (object_->*method_)(arg1, arg2);
  }

 private:
  Class* object_;
  MethodType method_;
};

#endif  // INFO_NODE_H
