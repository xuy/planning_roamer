#ifndef XUY_STATE_ORDER_TAGGER_H
#define XUY_STATE_ORDER_TAGGER_H

#include <utility>

class SearchNodeInfo;
class SearchSpace;
class StateProxy;

using namespace std;

class StateOrderTagger {
  public:
    StateOrderTagger();

    virtual ~StateOrderTagger();

    void tag_state(const StateProxy&, SearchNodeInfo* iter);

    void print_tags(const StateProxy&, SearchNodeInfo* iter);

    void DumpTags(SearchSpace& space);

  private:
    int order;
};

#endif  //  XUY_STATE_ORDER_TAGGER_H
