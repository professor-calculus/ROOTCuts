//
//  Yield_Adder.cpp
//  ROOTCuts
//
//  Created by Alex Titterton on 16/02/2017.
//
//

#include <stdio.h>
#include "../include/ROOTEffs.hh"

using namespace std;

void Yields_Adder(vector<string> yields, int n_yields)
{
    vector<double> yield_vals;
    double temp;
    string s1, s2, line;
    vector<string> SQ, LSP;
    int k = 0;
    
    fstream fin(yields[0]);
    
    int j=0;
    
    while(getline(fin, line))
    {
        istringstream is(line);
        is >> s1 >> s2 >> temp;
        
        SQ.push_back(s1);
        LSP.push_back(s2);
        
        yield_vals.push_back(temp);
        
        k++;
    }
    
    int no_of_yields = k;
    
    for(int i=1; i<n_yields; i++)
    {
        k=0;
        
        fstream fin(yields[i]);
        
        while(getline(fin, line))
        {
            fstream fin(yields[i]);
        
            istringstream is(line);
            is >> s1 >> s2 >> temp;
            
            yield_vals[k] += temp;
            k++;
        }
        
        if(k != no_of_yields)
        {
            cout << "Problem: Not all files contain same amount of points!\n" << endl;
            return;
        }
    }
    
    ofstream outputfile;
    outputfile.open("yields_total.txt", std::ios_base::trunc);
    
    for(int l=0; l<k; l++)
    {
        outputfile << SQ[l] << "\t" << LSP[l] << "\t" << yield_vals[l] << endl;
    }
    
    return;
}
