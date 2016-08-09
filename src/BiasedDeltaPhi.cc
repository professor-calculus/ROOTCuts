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
    double biaseddeltaphi, HTphi, Jetphi;
    vector<double> vectorbiaseddeltaphi;
    
    vector<TLorentzVector> p;
    TLorentzVector q;
    
    for(i=0; i<numberofjets; i++)
    {
        q.Clear();
        
        for(j=0; j<numberofjets; j++)
        {
            if(i != j)
            {
                q += jets[i]->P4();
            }
        }
        
        p.push_back(q);
        
        HTphi = p[i].Phi();
        Jetphi = jets[i]->Phi;
        
        vectorbiaseddeltaphi.push_back(HTphi + Jetphi);
    }
    
    minindex = min_element(vectorbiaseddeltaphi.begin(), vectorbiaseddeltaphi.end()) - vectorbiaseddeltaphi.begin();
    
    biaseddeltaphi = vectorbiaseddeltaphi[minindex];
    
    return biaseddeltaphi;
};