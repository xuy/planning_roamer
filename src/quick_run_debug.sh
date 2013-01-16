cd search
make debug
cd ..
./plan-debug ../benchmarks/airport-adl/domain.pddl ../benchmarks/airport-adl/p19-airport3-p6.pddl --heuristic "hff=ff()" --heuristic "lm=cea()" --search "lazy_greedy([hff,lm], preferred=[hff,lm])"
