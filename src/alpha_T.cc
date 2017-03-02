//
//  alpha_T.cpp
//  ROOTCuts
//
//  Created by Alex Titterton on 01/03/2017.
//
//

#include "../include/Ulrich_cuts.hh"
#include <bitset>

double alpha_T(vector<Jet *> jets, double MHT)
{
    std::vector<double> delta_ET, ET1, ET2;
    double ET1_temp, ET2_temp, alpha_t, M_T;
    int minindex;
    string binary;
    TLorentzVector p4;
    int N_jets = jets.size();
    
    for(long i=0; i<pow(2,N_jets-1); i++)
    {
        ET1_temp = 0.;
        ET2_temp = 0.;
        
        p4.Clear();
        
        binary = std::bitset<50>(i).to_string();
        binary.erase(binary.begin(),binary.end() - N_jets);
        
        for(int j=0; j<binary.size(); j++)
        {
            if(strcmp(&binary[j], "0"))
            {
                p4 = jets[j]->P4();
                ET1_temp += p4.Et();
            }
            else
            {
                p4 = jets[j]->P4();
                ET2_temp += p4.Et();
            }
            
        }
        
        delta_ET.push_back(abs(ET1_temp - ET2_temp));
        ET1.push_back(ET1_temp);
        ET2.push_back(ET2_temp);
    }
    
    minindex = min_element(delta_ET.begin(), delta_ET.end()) - delta_ET.begin();
    
    M_T = sqrt( (ET1[minindex] + ET2[minindex])*(ET1[minindex] + ET2[minindex]) - MHT*MHT );
    
    alpha_t = 0.5*(ET1[minindex] + ET2[minindex] - delta_ET[minindex])/M_T;
    
    
    return alpha_t;
}
