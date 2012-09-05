cd search
make debug
cd ..
./plan ../benchmarks/airport-adl/domain.pddl ../benchmarks/airport-adl/p07-airport2-p2.pddl --heuristic "hff=ff()" --heuristic "lm=cea()" --search "lazy_greedy([hff,lm], preferred=[hff,lm])"
