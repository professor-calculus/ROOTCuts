//
//  FileExists.cpp
//  ROOTCuts
//
//  Created by Alexander Titterton on 01/12/2016.
//
//

#include <stdio.h>
#include <iomanip>
#include "../include/ROOTEffs.hh"

void Plotting(const char *filename)
{
    static Efficiencies efficiencies;
    
    double dbeventpass;
    
    
    TChain *dick_cheney = new TChain("ROOTEffs");           // TChain named after Dick Cheney. Maybe he used ROOT
                                                            // too, it's making me want to shoot somebody!
    dick_cheney->Add(filename);
    
    dick_cheney->SetBranchAddress("Efficiencies", &efficiencies);
    
    
    TH2F *effs = new TH2F("effs", "effs", 21, 1000., 2050., 18, 0., 900.);
    TH2F *crosssecs = new TH2F("crosssecs", "crosssecs", 21, 1000., 2050., 18, 0., 900.);
    TH1F *dcrosssecs = new TH1F("1Dcrosssecs", "1Dcrosssecs", 21, 1000, 2050.);
    
    Int_t nentries = Int_t(dick_cheney->GetEntries());
    
    ofstream outputfile;
    ofstream cutflowfile;
    
    string outputfilename = "yields.txt";
    string cutflowfilename = "cutflow.txt";
    
    outputfile.open(outputfilename, std::ios_base::trunc);
    cutflowfile.open(cutflowfilename, std::ios_base::trunc);
    
    //cout << nentries << endl;
    
    
    for (Int_t entryInChain=0; entryInChain<nentries; entryInChain++)
    {
        dick_cheney->GetEntry(entryInChain);
        
        dbeventpass = round(efficiencies.doubleeventpass * 10000.)/10000.;
        
        outputfile << efficiencies.Msq << "\t" << efficiencies.Mlsp << "\t" << dbeventpass << endl;
        
        cutflowfile << efficiencies.Msq << "\t" << efficiencies.Mlsp << "\t" << efficiencies.crosssec << "\t" << efficiencies.eff << "\t" << efficiencies.HT << "\t" << efficiencies.HTeff << "\t" << efficiencies.MET << "\t" << efficiencies.METeff << "\t" << efficiencies.MHT << "\t" << efficiencies.MHTeff << "\t" << efficiencies.Nj << "\t" << efficiencies.Njeff << "\t" << efficiencies.Nb << "\t" << efficiencies.Nbeff << "\t" << efficiencies.Mbb << "\t" << efficiencies.Mbbeff << "\t" << efficiencies.BDP << "\t" << efficiencies.BDPeff << "\t" << endl;
        
        effs->Fill(efficiencies.Msq, efficiencies.Mlsp, efficiencies.eff);    //fill data from a tree to a histogram
        crosssecs->Fill(efficiencies.Msq, efficiencies.Mlsp, efficiencies.crosssec);
        dcrosssecs->Fill(efficiencies.Msq, efficiencies.crosssec);
    }
    
    outputfile.close();
    cutflowfile.close();
    
    TFile *outfile = TFile::Open("Effs_Plots.root","RECREATE");
    
    TCanvas *can_effs = new TCanvas("c", "c", 600, 600);
    effs->Draw("colz");
    effs->Write();
    can_effs->Update();
    can_effs->SaveAs("efficiencies.pdf");
    
    TCanvas *can_crosssecs = new TCanvas("c2", "c2", 600, 600);
    crosssecs->Draw("logzcolz");
    crosssecs->Write();
    can_crosssecs->Update();
    can_crosssecs->SaveAs("crosssecs.pdf");
    
    TCanvas *can_1Dcrosssecs = new TCanvas("c3", "c3", 600, 600);
    dcrosssecs->Draw("logzcolz");
    dcrosssecs->Write();
    can_1Dcrosssecs->Update();
    can_1Dcrosssecs->SaveAs("1D_crosssecs.pdf");
    
    outfile->Close();
};
