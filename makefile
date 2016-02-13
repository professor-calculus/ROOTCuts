#makefile to compile mycode.c.
all:
	g++ ROOTcuts.cc src/Ulrich_cuts.cc `root-config --cflags --libs` -lMinuit -Wall -fopenmp -O2 -o ROOTCuts
clean:
	-@rm ROOTCuts 2>/dev/null || true
#end makefile
