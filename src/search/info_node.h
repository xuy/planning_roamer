#ifndef INFO_NODE_H
#define INFO_NODE_H

#include "search_node_info.h"
#include "state_proxy.h"

class StateProxy;

// StateProxy is defined in state_proxy.h. It is a pointer that maps
// to the actualy FD State class. SearchNodeInfo is defined in
// search_node_info.h. It is the grahy search version of a state node,
// which contains NEW/OPEN/CLOSE info, g and h values, parent info etc.
// A SearchSpace maintains a collection of InfoNodes.
typedef std::pair<const StateProxy, SearchNodeInfo> InfoNode;

class InfoNodeCallback : public std::unary_function<InfoNode, void> {
  public:
    virtual void operator() (InfoNode /*unused_arg*/) const = 0;
    virtual ~InfoNodeCallback() {}
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
class NodeMethodClosure : public InfoNodeCallback {
 public:
  typedef void (Class::*MethodType)(InfoNode);

  NodeMethodClosure(Class* object, MethodType method)
    : object_(object), method_(method) {}
  virtual ~NodeMethodClosure() {}

  virtual void operator() (InfoNode arg) const { 
    (object_->*method_)(arg);
  }

 private:
  Class* object_;
  MethodType method_;
};

#endif  // INFO_NODE_H
