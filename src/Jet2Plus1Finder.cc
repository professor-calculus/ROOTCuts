//
//  JetDoublePairFinder.cc
//  ROOTCuts
//
//  Created by Alexander Titterton on 01/03/2016.
//
//

#include <iostream>
#include "../include/Ulrich_cuts.hh"

vector<Jet*> Jet2Plus1Finder(vector<Jet*> inputjets, int numberofjets)
{
    TLorentzVector p[4];
    
    DoubleJets jets;
    int minindex, minindex2;
    double DeltaR, DeltaR2, DeltaR3;
    vector<double> DeltaRtot;
    
    vector<Jet*> matchingjets;
    
    for(int j=0; j<numberofjets-2; j++)
    {
        for(int a=j+1; a<numberofjets-2; a++)
        {
            for(int b=j+2; b<numberofjets; b++)
            {

                    DeltaRtot.clear();
                    
                    p[0] = inputjets[j]->P4();
                    p[1] = inputjets[a]->P4();
                    p[2] = inputjets[b]->P4();

                    
                    DeltaR = p[0].DeltaR(p[1]);
                    DeltaRtot.push_back(DeltaR);
                    
                    DeltaR2 = p[0].DeltaR(p[2]);
                    DeltaRtot.push_back(DeltaR2);
                    
                    DeltaR3 = p[0].DeltaR(p[3]);
                    DeltaRtot.push_back(DeltaR3);
                    
                    minindex2 = min_element(DeltaRtot.begin(), DeltaRtot.end()) - DeltaRtot.begin();
                    
                    switch(minindex2)
                    {
                        case 0:
                            jets.DelR.push_back(DeltaR);
                            jets.DelRtot.push_back(DeltaR);
                            jets.j1.push_back(inputjets[j]);
                            jets.j2.push_back(inputjets[a]);
                            jets.j3.push_back(inputjets[b]);
                            break;
                            
                        case 1:
                            jets.DelR.push_back(DeltaR2);
                            jets.DelRtot.push_back(DeltaR2);
                            jets.j1.push_back(inputjets[j]);
                            jets.j2.push_back(inputjets[b]);
                            jets.j3.push_back(inputjets[a]);
                            break;
                            
                        case 2:
                            jets.DelR.push_back(DeltaR3);
                            jets.DelRtot.push_back(DeltaR3);
                            jets.j1.push_back(inputjets[j]);
                            jets.j2.push_back(inputjets[a]);
                            jets.j3.push_back(inputjets[b]);
                            break;
                    }
                }
            }
        }
    
    minindex = min_element(jets.DelRtot.begin(), jets.DelRtot.end()) - jets.DelRtot.begin();
    
    
    matchingjets.push_back(jets.j1[minindex]);
    matchingjets.push_back(jets.j2[minindex]);
    matchingjets.push_back(jets.j3[minindex]);
    
    
    return matchingjets;
}