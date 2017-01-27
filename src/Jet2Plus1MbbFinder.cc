//
//  JetDoubleMbbPairFinder.cpp
//  ROOTCuts
//
//  Created by Alexander Titterton on 14/03/2016.
//
//

#include <stdio.h>
#include <iostream>
#include "../include/Ulrich_cuts.hh"

vector<Jet*> Jet2Plus1MbbFinder(vector<Jet*> inputjets, int numberofjets)
{
    TLorentzVector p[4];
    
    DoubleMbbJets jets;
    int minindex, minindex2;
    double Mbb[2], Mbb2[2], Mbb3[2];
    vector<double> DeltaMbb;
    
    vector<Jet*> matchingjets;
    
    for(int j=0; j<numberofjets-2; j++)
    {
        for(int a=j+1; a<numberofjets-1; a++)
        {
            for(int b=j+2; b<numberofjets; b++)
            {
                
                    DeltaMbb.clear();
                    
                    p[0] = inputjets[j]->P4();
                    p[1] = inputjets[a]->P4();
                    p[2] = inputjets[b]->P4();
                    
                    Mbb[0] = ((p[0]) + (p[1])).M();
                    Mbb[1] = inputjets[b]->Mass;
                    DeltaMbb.push_back(abs(Mbb[0] - Mbb[1]));
                    
                    Mbb2[0] = ((p[0]) + (p[2])).M();
                    Mbb2[1] = inputjets[a]->Mass;
                    DeltaMbb.push_back(abs(Mbb2[0] - Mbb2[1]));
                    
                    Mbb3[0] = ((p[1]) + (p[2])).M();
                    Mbb3[1] = inputjets[j]->Mass;
                    DeltaMbb.push_back(abs(Mbb3[0] - Mbb3[1]));
                    
                    minindex2 = min_element(DeltaMbb.begin(), DeltaMbb.end()) - DeltaMbb.begin();
                    
                    switch(minindex2)
                    {
                        case 0:
                            jets.DelMbb.push_back(DeltaMbb[0]);
                            jets.j1.push_back(inputjets[j]);
                            jets.j2.push_back(inputjets[a]);
                            jets.j3.push_back(inputjets[b]);
                            break;
                            
                        case 1:
                            jets.DelMbb.push_back(DeltaMbb[1]);
                            jets.j1.push_back(inputjets[j]);
                            jets.j2.push_back(inputjets[b]);
                            jets.j3.push_back(inputjets[a]);
                            break;
                            
                        case 2:
                            jets.DelMbb.push_back(DeltaMbb[2]);
                            jets.j1.push_back(inputjets[a]);
                            jets.j2.push_back(inputjets[b]);
                            jets.j3.push_back(inputjets[j]);
                            break;
                    }
                }
            }
        }
    
    minindex = min_element(jets.DelMbb.begin(), jets.DelMbb.end()) - jets.DelMbb.begin();
    
    
    matchingjets.push_back(jets.j1[minindex]);
    matchingjets.push_back(jets.j2[minindex]);
    matchingjets.push_back(jets.j3[minindex]);
    
    
    return matchingjets;
}