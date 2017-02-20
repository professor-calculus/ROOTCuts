//
//  ROOTEffs.cpp
//  ROOTCuts
//
//  Created by Alex Titterton on 15/02/2017.
//
//

#include "include/ROOTEffs.hh"

int main(int argc, char *argv[])
{
    
    if(argc == 2)
    {
        cout << "\n Plotting Mode\n" << endl;
        Plotting(argv[1]);
        
        return 0;
    }
    
    else if(argc > 2)
    {
        vector<string> infiles;
        
        for(int i=1; i<argc; i++)
        {
            infiles.push_back(argv[i]);
            
            Yields_Adder(infiles, argc - 1);
        }
        return 0;
    }
    
    else
    {
        std::cout << "Error: Please put the path(s) of the yields .txt files as the argument(s)." << endl;
    }
}
