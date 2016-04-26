//
//  Ulrich_cuts_bkg.cpp
//  ROOTCuts
//
//  Created by Alexander Titterton on 14/03/2016.
//
//

#include <iostream>
#include "../include/Ulrich_cuts.hh"
#include "../include/TerminalPlot.hh"


using namespace std;


void CutsFunctionBkg(const char* filename, double params[12], string mode, TH1* histmb)
{
    gSystem->Load("libTreePlayer");
    //gSystem->Load("/home/ast1g15/delphes/libDelphes.so");
    //gSystem->Load("libExRootAnalysis.so");
    

    //              Parameters:
    //      0       1st Leading jet PT
    //      1       2nd Leading jet PT
    //      2       3rd Leading jet PT
    //      3       4th Leading jet PT
    //      4       0 = h->bb h->tau-tau; 1 = h->bb both cascades; 2 = 3 b-jets
    //      5       b-jet lower bound PT
    //      6       MET lower bound
    //      7       min. taus inv. mass
    //      8       max. taus inv. mass
    //      9       min. Sum taus' PT
    //      10      min. M_bb
    //      11      max. M_bb
    //      12      Jet pair matching algorithm for 2 bb pairs: 0 = Smallest av. Delta-R; 1 = Pairs with closest M_inv(bb)
    
    
    double jetPT1 = params[0];
    double jetPT2 = params[1];
    double jetPT3 = params[2];
    double jetPT4 = params[3];
    
    int higgsdecay = int(params[4]);
    if(higgsdecay < 0 || higgsdecay > 1)
    {
        cout << "ERROR: Higgs Decay mode must be 0 or 1" << endl;
        return;
    }
    
    double bjetminPT = params[5];
    
    double minMET = params[6];
    
    double minTauinvmass = params[7];
    double maxTauinvmass = params[8];
    
    double minSumTauPT = params[9];
    
    double minMbb = params[10];
    double maxMbb = params[11];
    
    int jetmatchingalgo = params[12];
    
    
    int i, k, l, entries, npass, N_bjets, N_tau, N_PT;
    
    double mtautau, PT_tau, met, efficiency;
    
    double mbb = 0;
    double mbb2 = 0;
    
    double DeltaR, DeltaR2;
    
    int percent, tintin;
    
    string bar;
    
    string filename2,title;
    
    //---------Opening the .root file:
    
    
    TFile *f = TFile::Open(filename,"UPDATE");
    
    TChain chain("Delphes");
    chain.Add(filename);
    ExRootTreeReader *reader = new ExRootTreeReader(&chain);
    
    TClonesArray *branchJet = reader->UseBranch("Jet");
    TClonesArray *branchMET = reader->UseBranch("MissingET");
    
    
    //--------Tell it not to panic if there's no entries - it's better than a segfault!
    if (reader->GetEntries() < 1)
    {
        cout << "Problem! There's no entries!" << endl;
    }
    
    entries = reader->GetEntries();
    
    cout << "Tree copied with " << entries << " entries\n\n" << endl;
    
    // Book histograms
    TH1 *histnbjet = new TH1F("nbjet", "Number of b-jets (h->bb in both cascades); No. b-jets", 10, 0.0, 10.0);
    //TH1 *histMbb = new TH1F("mbb", "M_{inv}(b, b) (h->bb in both cascades); M_{inv}(b, b) (GeV)", 20, minMbb, maxMbb);
    TH1 *histmet = new TH1F ("met", "Missing ET (h->bb in both cascades); MET (GeV)", 20, minMET, 1000.);
    TH1 *histDeltaR = new TH1F("DeltaR", "Delta R between b-jets; Delta R", 20, 0, 6);
    
    
    int pass_jets = 0;
    int pass_N_b_jets = 0;
    //int pass_PT_b_jets = 0;
    int pass_MET = 0;
    int pass_tau = 0;
    int pass_bb_mass = 0;
    int pass_N_jets = 0;
    int pass_tautau_mass = 0;
    
    int eventpass = 0;
    
    vector<Jet *> vectorjet;
    vector<Jet *> vectorbjet;
    vector<Jet *> vectortaujet;
    vector<Jet *> matchingbjets;
    vector<Jet *> matchingtaujets;
    
    
    TLorentzVector p4[4];
    
    
    //The for-loop: Loops over the tree to put the elements of each row into the class
    // until we then put them into the seperate classes depending on Track ID
    //                  -- This is so it only loops over particles with the relevant
    //                      TrackID, not the entire Tree ~1600 times!
    
    
    for(i=0;i<entries;i++)
        
    {
        reader->ReadEntry(i);
        
        vectorjet.clear();
        vectorbjet.clear();
        vectortaujet.clear();
        matchingbjets.clear();
        matchingtaujets.clear();
        
        Jet *jet = NULL;
        
        npass = 0;
        
        met = 0;
        
        N_bjets = 0;
        N_tau = 0;
        PT_tau = 0.0;
        N_PT = 0;
        
        if(branchJet->GetEntries() > 3)
        {
            pass_N_jets++;                  //passes number of hard jets test
            npass++;
            
            for(k=0; k<branchJet->GetEntries(); k++)
            {
                jet = (Jet*) branchJet->At(k);
                
                if(jet->PT > jetPT4)
                {
                    N_PT++;
                }
                
                vectorjet.push_back(jet);
                
                if(jet->BTag && jet->PT > bjetminPT)
                {
                    vectorbjet.push_back(jet);
                    N_bjets++;
                }
                else if(jet->TauTag)
                {
                    vectortaujet.push_back(jet);
                    N_tau++;
                }
            }
            
            if(higgsdecay == 0 && N_bjets > 1)
            {
                pass_N_b_jets++;
                npass++;                   //passes the number of b-jets test
                
                matchingbjets = JetPairFinder(vectorbjet, N_bjets);
                
                p4[0] = matchingbjets[0]->P4();
                p4[1] = matchingbjets[1]->P4();
                
                mbb = ((p4[0]) + (p4[1])).M();
                
                if(mbb > minMbb && mbb < maxMbb)
                {
                    npass++;
                    pass_bb_mass++;               //passes the M_bb inv. mass test
                    
                    DeltaR = p4[0].DeltaR(p4[1]);
                }
            }
            else if(higgsdecay ==1 && N_bjets > 3)
            {
                pass_N_b_jets++;
                npass += 2;                   //passes the number of b-jets test
                
                if(jetmatchingalgo == 0)     //Jet pairs with smallest average Delta-R
                {
                    matchingbjets = JetDoublePairFinder(vectorbjet, N_bjets);
                }
                else if(jetmatchingalgo == 1)                   //Jet pairs with closest M_inv(bb)
                {
                    matchingbjets = JetDoubleMbbPairFinder(vectorbjet, N_bjets);
                }
                
                p4[0] = matchingbjets[0]->P4();
                p4[1] = matchingbjets[1]->P4();
                p4[2] = matchingbjets[2]->P4();
                p4[3] = matchingbjets[3]->P4();
                
                
                mbb = ((p4[0]) + (p4[1])).M();
                mbb2 = ((p4[2]) + (p4[3])).M();
                
                if(mbb > minMbb && mbb < maxMbb && mbb2 > minMbb && mbb2 < maxMbb)
                {
                    npass += 2;
                    pass_bb_mass++;               //passes the M_bb inv. mass test
                    
                    DeltaR = p4[0].DeltaR(p4[1]);
                    DeltaR2 = p4[2].DeltaR(p4[3]);
                }
            }
            
            else if(higgsdecay == 2 && N_bjets > 2)
            {
                pass_N_b_jets++;
                npass += 2;                   //passes the number of b-jets test
                
                if(jetmatchingalgo == 0)     //Jet pairs with smallest average Delta-R
                {
                    matchingbjets = Jet2Plus1Finder(vectorbjet, N_bjets);
                }
                else if(jetmatchingalgo == 1)                   //Jet pairs with closest M_inv(bb)
                {
                    matchingbjets = Jet2Plus1Finder(vectorbjet, N_bjets);
                }
                
                p4[0] = matchingbjets[0]->P4();
                p4[1] = matchingbjets[1]->P4();
                
                mbb = ((p4[0]) + (p4[1])).M();
                mbb2 = matchingbjets[2]->Mass;
                
                
                
                if(mbb > minMbb && mbb < maxMbb && mbb2 > minMbb && mbb2 < maxMbb)
                {
                    npass += 2;
                    pass_bb_mass++;               //passes the M_bb inv. mass test
                    
                    DeltaR = p4[0].DeltaR(p4[1]);
                }
            }
            
            
            if(vectorjet[0]->PT > jetPT1 && vectorjet[1]->PT > jetPT2 && vectorjet[2]->PT > jetPT3 && vectorjet[3]->PT > jetPT4)
            {
                pass_jets++;
                npass++;                    //passes the PT of 4 leading jets test
            }
            
            
            for (int m = 0; m < branchMET->GetEntries(); m++)
            {
                Double_t metv = ((MissingET*) branchMET->At(m))->MET;
                met += metv;
            }
            
            if(met > minMET)
            {
                pass_MET++;
                npass++;                    //passes the MET test
            }
            
            if(higgsdecay == 0 && N_tau > 1)
            {
                matchingtaujets = JetPairFinder(vectortaujet, N_tau);
                
                p4[2] = matchingtaujets[0]->P4();
                p4[3] = matchingtaujets[1]->P4();
                
                mtautau = ((p4[2]) + (p4[3])).M();
                
                if(mtautau > minTauinvmass && mtautau < maxTauinvmass)
                {
                    npass++;                //passes the tautau inv. mass test
                    pass_tautau_mass++;
                }
                
                for(l=0; l<N_tau; l++)
                {
                    PT_tau += double(vectortaujet[l]->PT);
                    //cout << "Tau PT " << PT_tau << endl;
                }
                
                if(double(PT_tau) > minSumTauPT)
                {
                    npass++;                //passes the total tau transverse momentum test
                    pass_tau++;
                    //cout << pass_tau << " events passed the tau test so far" << endl;
                }
            }
            
            //----This bit gives a nice progress bar - unnecessary but looks so nice, like an iPad mini
            
            percent = (i*100)/entries;
            
            for(tintin = 0; tintin < 50; tintin++)
            {
                if( tintin < (percent/2))
                {
                    bar.replace(tintin,1,"=");
                }
                else if( tintin == (percent/2))
                {
                    bar.replace(tintin,1,">");
                }
                else
                {
                    bar.replace(tintin,1," ");
                }
            }
            
            cout << "\033[36m" << "\r" "[" << bar << "] ";
            cout.width( 3 );
            cout << "\033[0m" << percent << "%     " << std::flush;   // lol
        }
        
        if(npass == 7)
        {
            eventpass++;
            
            histmb->Fill(mbb);
            histnbjet->Fill(N_bjets);
            histmet->Fill(met);
            histDeltaR->Fill(DeltaR);
            
            if(higgsdecay == 1)
            {
                histmb->Fill(mbb2);

                histDeltaR->Fill(DeltaR2);
            }
        }
        
        
    }
    
    cout << "\n" << endl;
    
    if(higgsdecay == 0)
    {
        histmb->SetTitle("M_{inv}(b, b) (h->bb and h->tau-tau); M_{inv}(b, b) (GeV)");
        histnbjet->SetTitle("Number of b-jets (h->bb and h->tau-tau); No. b-jets");
        histmet->SetTitle("Missing ET (h->bb and h->tau-tau); MET (GeV)");
    }
    
    
    
    //cout << histMbb->GetBinContent(10) << "\n" << endl;
    
    TCanvas * cbjet = new TCanvas("cbjet", "cbjet", 600, 600);
    
    histnbjet->Draw();
    cbjet->Update();
    
    if(mode == "Signal")
    {
        if(higgsdecay == 0)
        {
            cbjet->SaveAs("n_b_jets_tau.pdf");
        }
        else
        {
            cbjet->SaveAs("n_b_jets.pdf");
        }
    }
    else
    {
        if(higgsdecay == 0)
        {
            cbjet->SaveAs("n_b_jets_tau_bkg.pdf");
        }
        else
        {
            cbjet->SaveAs("n_b_jets_bkg.pdf");
        }
    }
    
    
    TCanvas * cmet = new TCanvas ("cmet", "cmet", 600, 600);
    
    histmet->Draw();
    cmet->Update();
    
    if(mode == "Signal")
    {
        if(higgsdecay == 0)
        {
            cmet->SaveAs("MET_tau.pdf");
        }
        else
        {
            cmet->SaveAs("MET.pdf");
        }
    }
    else
    {
        if(higgsdecay == 0)
        {
            cmet->SaveAs("MET_tau_bkg.pdf");
        }
        else
        {
            cmet->SaveAs("MET_bkg.pdf");
        }
    }
    
    TCanvas * cdelr = new TCanvas ("cdelr", "cdelr", 600, 600);
    
    histDeltaR->Draw();
    cdelr->Update();
    
    if(mode == "Signal")
    {
        if(higgsdecay == 0)
        {
            cdelr->SaveAs("DeltaR_tau.pdf");
        }
        else
        {
            cdelr->SaveAs("DeltaR.pdf");
        }
    }
    else
    {
        if(higgsdecay == 0)
        {
            cdelr->SaveAs("DeltaR_tau_bkg.pdf");
        }
        else
        {
            cdelr->SaveAs("DeltaR_bkg.pdf");
        }
    }
    
    
    
    efficiency = double(eventpass)/double(entries);
    
    cout << "\n" << endl;
    if(higgsdecay == 0)
    {
        cout << "Higgs to bb and Higgs to tau-tau required\n" << endl;
    }
    else
    {
        cout << "Higgs to bb in both cascades required\n" << endl;
    }
    cout << pass_N_jets << " events contained at least 4 jets" << endl;
    cout << pass_jets << " events contained 4 leading jets with PT 400,300,200,100 GeV" << endl;
    cout << pass_N_b_jets << " events contained at least 2 b-jets" << endl;
    cout << pass_bb_mass << " events contained at least 2 b-jets with invariant mass within the bounds" << endl;
    cout << pass_MET << " events had at least 30GeV Missing ET" << endl;
    cout << pass_tau << " events contained at least 2 tau with SUM(PT) > 100GeV" << endl;
    cout << "\n" << eventpass << " events passed all tests" << endl;
    
    cout << "\n\n\n" << endl;
    
    cout << "Cross-section is now reduced by factor of " << efficiency << "\n\n" << endl;
    cout << "\033[32m" << "Winner winner, chicken dinner\n" << "\033[0m" << "\n\n" << endl;
    cout << "Cuts (Energy, masses, PT in GeV):\n" << endl;
    cout << "PT of 1st-4th leading jets = " << jetPT1 << ", " << jetPT2 << ", " << jetPT3 << ", " << jetPT4 << " respectively" << endl;
    cout << "Min. b-jet PT = " << bjetminPT << endl;
    cout << "Min. Missing ET = " << minMET << endl;
    if(higgsdecay == 0)
    {
        cout << "M_tautau from " << minTauinvmass << " to " << maxTauinvmass << endl;
        cout << "Min. Sum of taus' PT = " << minSumTauPT << endl;
    }
    cout << "M_bb from " << minMbb << " to " << maxMbb << "\n\n" << endl;
    
    cout << "Plots:\n" << endl;
    
    if(eventpass != 0)
    {
        //TerminalPlot(histMbb, "M_bb", 40, minMbb, maxMbb);
        TerminalPlot(histnbjet, "No. of b-jets", 40, 0.0, 10.0);
        TerminalPlot(histmet, "Missing ET", 40, minMET, 1000.);
        TerminalPlot(histDeltaR, "b-Jets DeltaR", 40, 0, 6);
    }
    
    delete histDeltaR;
    delete histnbjet;
    delete histmet;

    
    //f->Write();
    f->Close();
    
};