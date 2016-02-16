//
//  Tracking_Main.cc
//  
//
//  Created by Alex Titterton on 14/08/2013.
//
//

#include "include/Ulrich_cuts.hh"


int main(int argc, char *argv[])
{
	gSystem->Load("libTreePlayer");
    gSystem->Load("/home/ast1g15/delphes/libDelphes.so");
    gSystem->Load("libExRootAnalysis.so");
    
    if (argc>1)
    {
    	CutsFunction(argv[1]);
    }

    return 0;
    
}