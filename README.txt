##############################
####     ROOTCut v0.1     ####
####  Alexander Titterton ####
####        © 2016        ####
##############################

Simple program for applying cuts to results from
DELPHES, with input in the form of a .root file.

(Old, deprecated) Install: With ROOT configured and ‘source /path/to/thisroot.sh’
	  executed (preferably put it in your .bash_profile) you
	  should be able to just type ‘make’, but might have to set delphes location 	  in the makefile… It’s not the recommended method.
	  To uninstall do ‘make clean’.

NEW (and recommended): CMAKE! This allows easier cross-platform making, plus you can also create an Xcode project with it. ‘cd’ into ‘bin’ and follow instructions in INSTALL.txt

Usage: ./bin/ROOTCuts /path/to/rootfile.root (from this directory, but just adjust to suit)

There we go. This will probably never get beyond a simple, specific
tool for my own analyses, but hey, I can dream of it one day
evolving into a beautiful iPad app for general cut-performing on
the move. Winner winner, chicken dinner.

A.T.