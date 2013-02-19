#ifndef SEARCH_CALLBACK_H
#define SEARCH_CALLBACK_H

#include "operator.h"
#include "search_node_info.h"
#include "state_proxy.h"

class StateProxy;

// StateProxy is defined in state_proxy.h. It is a pointer that maps to an
// underlying storage area to represent a state. SearchNodeInfo SearchNodeInfo
// is defined in search_node_info.h. It provides extra fields to support graph
// search.  For instance, the NEW/OPEN/CLOSE info and the g and h values are
// all stored in SearchNodeInfo.  A SearchSpace maintains a hash table that
// maps states to their SearchNodeInfo. InfoNode is used as a handy type alias
// to represent those entries in the hash table in search_space.cc.


// Ideally, all callback functions that inspects the search space at search time
// should take InfoNode as a parameter. However, C++ pairs are always copy-constructed,
// meaning any changes made to InfoNode in callback does not actually change the
// entry stored in StateSpace's hashtable. So, all callback functions are taking
// in two parameters, StateProxy and SearchNodeInfo*, instead of a simple pair.
class SearchSpaceCallback : public std::binary_function<const StateProxy&, SearchNodeInfo*, void> {
  public:
    virtual void operator() (
        const StateProxy& /*unused_arg*/, SearchNodeInfo* /*unused_arg*/) const = 0;
    virtual ~SearchSpaceCallback() {}
};

// SearchSpaceClosure is a SearchSpaceCallback that wraps the member 
// function of an object as the callback function.
// Usage:
//      SearchSpaceCallback* closure =
//          SearchSpaceClosure<FeatureExtractor>(
//              object, &Class::member_method);
template <typename Class>
class SearchSpaceClosure : public SearchSpaceCallback {
 public:
  typedef void (Class::*MethodType)(const StateProxy&, SearchNodeInfo*);

  SearchSpaceClosure(Class* object, MethodType method)
    : object_(object), method_(method) {}
  virtual ~SearchSpaceClosure() {}

  virtual void operator() (const StateProxy& arg1, SearchNodeInfo* arg2) const { 
    (object_->*method_)(arg1, arg2);
  }

 private:
  Class* object_;
  MethodType method_;
};

// Callback that happens when you open a new node.
class SearchNodeOpenCallback : public std::binary_function
    <const int, const Operator*, void> {
  public:
    virtual void operator() (
        const int delta /*unused_arg*/,
        const Operator* parent_op /*unused_arg*/) const = 0;
    
    virtual ~SearchNodeOpenCallback() {}
};

template <typename Class>
class SearchNodeOpenClosure : public SearchNodeOpenCallback {
 public:
  typedef void (Class::*MethodType)(const int delta, const Operator* parent_op);

  SearchNodeOpenClosure(Class* object, MethodType method)
    : object_(object), method_(method) {}

  virtual ~SearchNodeOpenClosure() {}

  virtual void operator() (const int arg1, const Operator* arg2) const { 
    (object_->*method_)(arg1, arg2);
  }

 private:
  Class* object_;
  MethodType method_;
};

#endif  // SEARCH_CALLBACK_H
