#ifndef ENFORCED_HILL_CLIMBING_SEARCH_H
#define ENFORCED_HILL_CLIMBING_SEARCH_H

#include "search_engine.h"
#include "globals.h"
#include "search_space.h"
#include "search_node_info.h"
#include "operator.h"
#include "state.h"
#include "open_lists/open_list.h"
#include "g_evaluator.h"
#include "search_progress.h"
#include <vector>
#include <map>

using namespace std;

typedef pair<state_var_t*, pair<int, const Operator * > > OpenListEntryEHC;

enum PreferredUsage {rank_preferred_first = 1, prune_by_preferred = 2};

class EnforcedHillClimbingSearch : public SearchEngine {
protected:
    OpenList<OpenListEntryEHC> *open_list;
    GEvaluator *g_evaluator;

    Heuristic *heuristic;
    bool preferred_contains_eval;
    vector<Heuristic *> preferred_heuristics;
    bool use_preferred;
    PreferredUsage preferred_usage;

    bool use_cost_for_bfs;

    State current_state;
    int current_h;
    int current_g;

    // statistics
    SearchProgress search_progress;
    map<int, pair<int, int> > d_counts;
    int num_ehc_phases;
    int last_expanded;

    virtual void initialize();
    virtual int step();
    int ehc();
    void get_successors(const State &state, vector<const Operator *> &ops);
    void evaluate(const State &parent, const Operator * op, const State &state);
public:
    EnforcedHillClimbingSearch(Heuristic *heuristic_, 
        PreferredUsage preferred_usage_, bool use_cost_for_bfs_);
    virtual ~EnforcedHillClimbingSearch();
    void set_pref_operator_heuristics(std::vector<Heuristic *> &heur);

    virtual void statistics() const;

    static SearchEngine* create_engine(const vector<string> &config, 
                                       int start, int &end);
};

#endif
