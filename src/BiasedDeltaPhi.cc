//
//  BiasedDeltaPhi.cpp
//  ROOTCuts
//
//  Created by Alexander Titterton on 08/08/2016.
//
//

#include "include/Ulrich_cuts.hh"
#include "include/BiasedDeltaPhi.hh"


double BiasedDeltaPhi(vector<Jet*> jets, int numberofjets)
{
    int i, j, minindex;
    double biaseddeltaphi;
    vector<double> vectorbiaseddeltaphi;
    
    TLorentzVector q, r, MHT, JetRemaining;
    
    for(i=0; i<numberofjets; i++)
    {
        q.Clear();
        r.Clear();
        
        for(j=0; j<numberofjets; j++)
        {
            if(i != j)
            {
                q += jets[i]->P4();
            }
            
            r += jets[i]->P4();
        }
        
        JetRemaining = jets[i]->P4();
        
        MHT = r - q;
        
        vectorbiaseddeltaphi.push_back(abs(MHT.DeltaPhi(JetRemaining)));
    }
    
    minindex = min_element(vectorbiaseddeltaphi.begin(), vectorbiaseddeltaphi.end()) - vectorbiaseddeltaphi.begin();
    
    biaseddeltaphi = vectorbiaseddeltaphi[minindex];
    
    return abs(biaseddeltaphi);
};