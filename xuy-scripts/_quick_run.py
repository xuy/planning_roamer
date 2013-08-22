#!/usr/bin/python
import sys
import os

from Problems import problems as benchmarks

# This handy script allows users to use unique prefix to denote domains,
# instead of typing out the fullname of the domain. Also, it uses
# domain/problem definitions in Problems.py to locate the correct pddl files
# for the specified domain and problem.

# Given a domain name prefix, maps it to an unique domain.
def FindDomain(name):
  possible_domain = []
  for domain_name in benchmarks:
    if domain_name.startswith(sys.argv[1]):
      possible_domain.append(domain_name)
  if len(possible_domain) == 1:
    return possible_domain[0]
  else:
    print "Possible domains are ",  possible_domain
    sys.exit(-1)

# A fancy way to issue command line calls such as
# ../src/plan-debug \
# ../benchmarks/airport-adl/domain.pddl \
# ../benchmarks/airport-adl/p19-airport3-p6.pddl \
# --heuristic "hff=ff()"
# --heuristic "lm=cea()"
# --search "lazy_greedy([hff,lm], preferred=[hff,lm])"

def RunRoamer(directory, domain_file, problem_file):
  print "Call roamer to solve ", directory, domain_file, problem_file
  template = '../src/plan-terse ../benchmarks/%s/%s ../benchmarks/%s/%s ' +\
            '--heuristic "hff=ff()" --heuristic "lm=cea()" ' +\
            '--search "lazy_greedy([hff,lm], preferred=[hff,lm], ' +\
            ' open_cb=[logistic], new_cb=[order_tagger])"'
            # callback functions, subject to change
  command = template % (directory, domain_file, directory, problem_file)
  os.system(command)

if __name__ == '__main__':
  domain_name = FindDomain(sys.argv[1])
  index = int(sys.argv[2])-1
  directory = benchmarks[domain_name]['directory']
  domain_files = benchmarks[domain_name]['domain']
  if isinstance(domain_files, str):
    # one domain file per domain
    domain_file = benchmarks[domain_name]['domain']
  else: # one domain file per problem, saved as a list
    domain_file = benchmarks[domain_name]['domain'][index]
  problem_file = benchmarks[domain_name]['problems'][index]
  RunRoamer(directory, domain_file, problem_file)
