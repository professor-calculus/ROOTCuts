//
//  FileExists.cpp
//  ROOTCuts
//
//  Created by Alexander Titterton on 01/12/2016.
//
//

#include <stdio.h>
#include "../include/Ulrich_cuts.hh"

void Plotting(const char *filename)
{
    static Efficiencies efficiencies;
    
    TChain *dick_cheney = new TChain("ROOTCuts");           // TChain named after Dick Cheney. Maybe he used ROOT
                                                            // too, it's making me want to shoot somebody!
    dick_cheney->Add(filename);
    
    dick_cheney->SetBranchAddress("Efficiencies", &efficiencies);
    
    
    TH2F *h1 = new TH2F("h1", "h1", 21, 1000., 2050., 18, 0., 900.);
    
    Int_t nentries = Int_t(dick_cheney->GetEntriesFast());
    for (Int_t entryInChain=0; entryInChain<nentries; entryInChain++)
    {
        dick_cheney->GetEntry(entryInChain);
        
        h1->Fill(efficiencies.Msq, efficiencies.Mlsp, efficiencies.eff);    //fill data from a tree to a histogram
    }
    
    TCanvas *can = new TCanvas("c", "c", 600, 600);
    h1->Draw("colz");
    can->Update();
    can->SaveAs("test.pdf");
};
