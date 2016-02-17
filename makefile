#makefile to compile mycode.c.
all:
	g++ -L/home/ast1g15/delphes ROOTcuts.cc src/Ulrich_cuts.cc `root-config --cflags --libs` -lDelphes -lMinuit -Wall -fopenmp -O2 -o ROOTCuts
clean:
	-@rm ROOTCuts 2>/dev/null || true
#end makefile
