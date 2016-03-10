//
//  JetDoublePairFinder.cc
//  ROOTCuts
//
//  Created by Alexander Titterton on 01/03/2016.
//
//

#include <iostream>
#include "../include/Ulrich_cuts.hh"

vector<Jet*> JetDoublePairFinder(vector<Jet*> inputjets, int numberofjets)
{
    TLorentzVector p[4];
    
    DoubleJets jets;
    int minindex, minindex2;
    double DeltaR[2], DeltaR2[2], DeltaR3[2];
    vector<double> DeltaRtot;
    
    vector<Jet*> matchingjets;
    
    for(int j=0; j<numberofjets-3; j++)
    {
        for(int a=j+1; a<numberofjets-2; a++)
        {
            for(int b=j+2; b<numberofjets-1; b++)
            {
                for(int c=j+3; c<numberofjets; c++)
                {
                    p[0] = inputjets[j]->P4();
                    p[1] = inputjets[a]->P4();
                    p[2] = inputjets[b]->P4();
                    p[3] = inputjets[c]->P4();
                    
                    DeltaR[0] = p[0].DeltaR(p[1]);
                    DeltaR[1] = p[2].DeltaR(p[3]);
                    DeltaRtot.push_back(DeltaR[0] + DeltaR[1]);
                    
                    DeltaR2[0] = p[0].DeltaR(p[2]);
                    DeltaR2[1] = p[1].DeltaR(p[3]);
                    DeltaRtot.push_back(DeltaR2[0] + DeltaR2[1]);
                    
                    DeltaR3[0] = p[0].DeltaR(p[3]);
                    DeltaR3[1] = p[1].DeltaR(p[2]);
                    DeltaRtot.push_back(DeltaR3[0] + DeltaR3[1]);
                    
                    minindex2 = min_element(DeltaRtot.begin(), DeltaRtot.end()) - DeltaRtot.begin();
                    
                    switch(minindex2)
                    {
                        case 0:
                            jets.DelR.push_back(DeltaRtot[0]);
                            jets.j1.push_back(inputjets[j]);
                            jets.j2.push_back(inputjets[a]);
                            jets.j3.push_back(inputjets[b]);
                            jets.j4.push_back(inputjets[c]);
                            break;
                    
                        case 1:
                            jets.DelR.push_back(DeltaRtot[1]);
                            jets.j1.push_back(inputjets[j]);
                            jets.j2.push_back(inputjets[b]);
                            jets.j3.push_back(inputjets[a]);
                            jets.j4.push_back(inputjets[c]);
                            break;
                    
                        case 2:
                            jets.DelR.push_back(DeltaRtot[2]);
                            jets.j1.push_back(inputjets[j]);
                            jets.j2.push_back(inputjets[c]);
                            jets.j3.push_back(inputjets[a]);
                            jets.j4.push_back(inputjets[b]);
                            break;
                    }
                }
            }
        }
    }
    
    minindex = min_element(jets.DelR.begin(), jets.DelR.end()) - jets.DelR.begin();
    

    matchingjets.push_back(jets.j1[minindex]);
    matchingjets.push_back(jets.j2[minindex]);
    matchingjets.push_back(jets.j3[minindex]);
    matchingjets.push_back(jets.j4[minindex]);
    
    
    return matchingjets;
}