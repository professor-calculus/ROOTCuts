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
	string inputfile;
    if (argc>1)
    {
      inputfile = argv[1];
    }
    CutsFunction(inputfile);
    
    return 0;
    
}