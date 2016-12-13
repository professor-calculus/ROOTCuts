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
    
    TChain *dick_cheney = new TChain("ROOTEffs");           // TChain named after Dick Cheney. Maybe he used ROOT
                                                            // too, it's making me want to shoot somebody!
    dick_cheney->Add(filename);
    
    dick_cheney->SetBranchAddress("Efficiencies", &efficiencies);
    
    
    TH2F *effs = new TH2F("effs", "effs", 21, 1000., 2050., 18, 0., 900.);
    TH2F *crosssecs = new TH2F("crosssecs", "crosssecs", 21, 1000., 2050., 18, 0., 900.);
    
    Int_t nentries = Int_t(dick_cheney->GetEntriesFast());
    for (Int_t entryInChain=0; entryInChain<nentries; entryInChain++)
    {
        dick_cheney->GetEntry(entryInChain);
        
        cout << efficiencies.eff << endl;
        
        effs->Fill(efficiencies.Msq, efficiencies.Mlsp, efficiencies.eff);    //fill data from a tree to a histogram
        crosssecs->Fill(efficiencies.Msq, efficiencies.Mlsp, efficiencies.crosssec);
    }
    
    TFile *outfile = TFile::Open("Effs_Plots.root","RECREATE");
    
    TCanvas *can_effs = new TCanvas("c", "c", 600, 600);
    effs->Draw("colz");
    effs->Write();
    can_effs->Update();
    can_effs->SaveAs("efficiencies.pdf");
    
    TCanvas *can_crosssecs = new TCanvas("c", "c", 600, 600);
    crosssecs->Draw("colz");
    crosssecs->Write();
    can_crosssecs->Update();
    can_crosssecs->SaveAs("crosssecs.pdf");
    
    outfile->Close();
};
