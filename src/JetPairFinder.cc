//
//  JetPairFinder.cc
//  ROOTCuts
//
//  Created by Alexander Titterton on 01/03/2016.
//
//

#include <iostream>
#include "../include/Ulrich_cuts.hh"

vector<Jet*> JetPairFinder(vector<Jet*> inputjets, int numberofjets, double DeltaR)
{
    TLorentzVector p[2];
    
    Jets jets;
    int minindex;
    
    vector<Jet*> matchingjets;
    
    for(int j=0; j<numberofjets; j++)
    {
        for(int a=j+1; a<numberofjets; a++)
        {
            p[0] = inputjets[j]->P4();
            p[1] = inputjets[a]->P4();
            
            DeltaR = p[0].DeltaR(p[1]);
            
            jets.DelR.push_back(DeltaR);
            jets.j1.push_back(inputjets[j]);
            jets.j2.push_back(inputjets[a]);
        }
    }
    
    minindex = min_element(jets.DelR.begin(), jets.DelR.end()) - jets.DelR.begin();
    
    matchingjets.push_back(jets.j1[minindex]);
    matchingjets.push_back(jets.j2[minindex]);
    
    return matchingjets;
}