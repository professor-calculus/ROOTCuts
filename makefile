#makefile to compile mycode.c.
all:
	g++ -L/home/ast1g15/delphes ROOTcuts.cc src/Ulrich_cuts.cc `root-config --cflags --libs` -lDelphes -lMinuit -Wall -fopenmp -O2 -o ROOTCuts
clean:
	-@rm ROOTCuts 2>/dev/null || true
foo:
	@if g++ -obuild ROOTCuts.cc ; then echo "\033[32;01mWinner Winner Chicken Dinner\033[0m"; else echo "\033[32;01mProblem\033[0m"; fi
#end makefile