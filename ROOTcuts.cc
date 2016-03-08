//
//  Tracking_Main.cc
//  
//
//  Created by Alex Titterton on 14/08/2013.
//
//

#include "include/Ulrich_cuts.hh"
#include <fstream>


int main(int argc, char *argv[])
{
	gSystem->Load("libTreePlayer");
    gSystem->Load("/home/ast1g15/delphes/libDelphes.so");
    //gSystem->Load("libExRootAnalysis.so");
    
    //              Parameters:
    //      0       1st Leading jet PT
    //      1       2nd Leading jet PT
    //      2       3rd Leading jet PT
    //      3       4th Leading jet PT
    //      4       b-jet lower bound PT
    //      5       MET lower bound
    //      6       min. taus inv. mass
    //      7       max. taus inv. mass
    //      8       min. Sum taus' PT
    //      9       min. M_bb
    //      10      max. M_bb

    
    double params[11];
    int param;
    double value;
    
    
    if(argc == 3)
    {
        fstream fin(argv[2]);
        string line;
    
        while(getline(fin, line))
        {
            //the following line trims white space from the beginning of the string
            line.erase(line.begin(), find_if(line.begin(), line.end(), not1(ptr_fun<int, int>(isspace))));
        
            if(line[0] == '#') continue;
        
            istringstream is(line);
            is >> param >> value;
        
            params[param] = value;
        }
    
    	CutsFunction(argv[1], params);
    }
    
    else if(argc == 2)
    {
        fstream fin("../default/default_parameters.txt");
        string line;
        
        while(getline(fin, line))
        {
            //the following line trims white space from the beginning of the string
            line.erase(line.begin(), find_if(line.begin(), line.end(), not1(ptr_fun<int, int>(isspace))));
            
            if(line[0] == '#') continue;
            
            istringstream is(line);
            is >> param >> value;
            
            params[param] = value;
        }
        
        CutsFunction(argv[1], params);
    }
    
    else
    {
        std::cout << "Usage: ./ROOTCuts /path/to/root/file.root /path/to/parameters/file.something" << endl;
    }

    return 0;
    
}