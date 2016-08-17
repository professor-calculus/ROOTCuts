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
    double biaseddeltaphi, phi1, phi2;
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
            
            //r += jets[i]->P4();
        }
        
        JetRemaining = jets[i]->P4();
        
        MHT = -1.*q;
        
        phi1 = atan2(-MHT.Py() - JetRemaining.Py(), MHT.Px() + JetRemaining.Px());
        phi2 = atan2(-JetRemaining.Py(), JetRemaining.Px());
        
        if(phi1 - phi2 > TMath::Pi())
        {
            vectorbiaseddeltaphi.push_back(abs(phi1 - phi2 - 2.*TMath::Pi()));
        }
        else if(phi1 - phi2 < -TMath::Pi())
        {
            vectorbiaseddeltaphi.push_back(abs(phi1 - phi2 + 2.*TMath::Pi()));
        }
        else
        {
            vectorbiaseddeltaphi.push_back(abs(phi1 - phi2));
        }
        
    }
    
    minindex = min_element(vectorbiaseddeltaphi.begin(), vectorbiaseddeltaphi.end()) - vectorbiaseddeltaphi.begin();
    
    biaseddeltaphi = vectorbiaseddeltaphi[minindex];
    
    return biaseddeltaphi;
};