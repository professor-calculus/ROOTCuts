//------------------------------------------------------------
//--------------©Alexander Titterton 11/02/2016---------------
//---------------U. Bristol/U. Southampton/RAL----------------
//------------------------------------------------------------
//
//
//

#include <iostream>
#include "../include/Ulrich_cuts.hh"
#include "../include/TerminalPlot.hh"
#include "../include/BiasedDeltaPhi.hh"


using namespace std;


void CutsFunction(const char* filename, double params[16])
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
    //      13      Sig/Bkg ratio
    //      14      min. HT
    //      15      min. no. of jets

    
    double jetPT1 = params[0];
    double jetPT2 = params[1];
    double jetPT3 = params[2];
    double jetPT4 = params[3];
    
    int higgsdecay = int(params[4]);
    if(higgsdecay < 0 || higgsdecay > 2)
    {
        cout << "ERROR: Higgs Decay mode must be 0, 1 or 2" << endl;
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
    
	double sigbkgratio = params[13];
    
    double minHT = params[14];
    
    int minN_jets = params[15];

	int i, k, l, entries, npass, N_bjets, N_tau, N_PT, N_jets;

    double mtautau, PT_tau, met, efficiency;
    
    double mbb = 0;
    double mbb2 = 0;
    
    double DeltaR, DeltaR2, biaseddeltaphi, HT;
    
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
    TClonesArray *branchScalarHT = reader->UseBranch("ScalarHT");
    
    
    //--------Tell it not to panic if there's no entries - it's better than a segfault!
    if (reader->GetEntries() < 1)
    {
        cout << "Problem! There's no entries!" << endl;
    }
        
    entries = reader->GetEntries();

	double weight = double(entries)*sigbkgratio;

    cout << "Tree copied with " << entries << " entries\n\n" << endl;
    
    
    
    //----- Output File
    ofstream outputfile;
    outputfile.open("output.txt");
    outputfile << "\n\n\n SIGNAL:" << endl;
    outputfile << "Tree copied with " << entries << " entries\n\n" << endl;

    
    
    // Book histograms
    TH1 *histnbjet = new TH1F("nbjet", "Number of b-jets; No. b-jets", 10, 0.0, 10.0);
    TH1 *histnjet = new TH1F("njet", "Number of Jets; No. Jets", 15, 0.0, 15.0);
    TH1 *histMbb = new TH1F("mbb", "M_{inv}(b, b); M_{inv}(b, b) (GeV)", 40, minMbb, maxMbb);
    TH1 *histmet = new TH1F ("met", "Missing ET; MET (GeV)", 50, 0.0, 1000.);
    TH1 *histDeltaR = new TH1F("DeltaR", "Delta R between b-jets; Delta R", 60, 0, 6);
    TH1 *histMHT = new TH1F("MHT", "Missing HT; Missing HT (GeV)", 100, 0., 8000.);
    TH1 *histHT = new TH1F("HT", "Scalar HT; Scalar HT (GeV)", 100, 0., 8000.);
    TH1 *histBiasedDeltaPhi = new TH1F("biaseddeltaphi", "Biased Delta Phi; Biased Delta Phi", 50, 0., 5.);
    
    TH1 *histnbjet_precut = new TH1F("nbjet_precut", "Number of b-jets Before Cut; No. b-jets", 10, 0.0, 10.0);
    TH1 *histnjet_precut = new TH1F("njet_precut", "Number of Jets; No. Jets", 15, 0.0, 15.0);
    TH1 *histMbb_precut = new TH1F("mbb_precut", "M_{inv}(b, b) Before Cut; M_{inv}(b, b) (GeV)", 40, minMbb, maxMbb);
    TH1 *histmet_precut = new TH1F ("met_precut", "Missing ET Before Cut; MET (GeV)", 50, 0.0, 1000.);
    TH1 *histDeltaR_precut = new TH1F("DeltaR_precut", "Delta R between b-jets Before Cut; Delta R", 60, 0, 6);
    //TH1 *histMHT_precut = new TH1F("MHT_precut", "Missing HT Before Cut; Missing HT (GeV)", 50, 0., 1000.);
    TH1 *histHT_precut = new TH1F("HT_precut", "Scalar HT Before Cut; Scalar HT (GeV)", 100, 0., 8000.);
    TH1 *histBiasedDeltaPhi_precut = new TH1F("biaseddeltaphi_precut", "Biased Delta Phi Before Cut; Biased Delta Phi", 50, 0., 5.);



    int pass_jets = 0;
    int pass_biaseddeltaphi = 0;
    int pass_N_b_jets = 0;
    //int pass_PT_b_jets = 0;
    int pass_MET = 0;
    int pass_tau = 0;
    int pass_bb_mass = 0;
    int pass_N_jets = 0;
    int pass_tautau_mass = 0;
    int pass_HT = 0;

    int eventpass = 0;
    
    vector<Jet *> vectorjet;
    vector<Jet *> vectorbjet;
    vector<Jet *> vectortaujet;
    vector<Jet *> matchingbjets;
    vector<Jet *> matchingtaujets;
    
    bool cut_Mbb = false;
    bool cut_DeltaR = false;
    bool cut_biaseddeltaphi = false;
    bool cut_MET = false;
    bool cut_HT = false;
    bool cut_N_bjets = false;
    bool cut_N_jets = false;


    TLorentzVector p4[4];
    TLorentzVector MissingHT;
    double ScalarMissingHT;
    

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
        
        MissingHT.Clear();

        Jet *jet = NULL;

        npass = 0;

        met = 0;
        HT = 0;

        N_bjets = 0;
        N_tau = 0;
        PT_tau = 0.0;
        N_PT = 0;
        
        cut_Mbb = false;
        cut_DeltaR = false;
        cut_biaseddeltaphi = false;
        cut_MET = false;
        cut_HT = false;
        cut_N_bjets = false;
        cut_N_jets = false;
        
        N_jets = branchJet->GetEntries();
        
        
        if(N_jets > 3)
        {
            
            if(N_jets > minN_jets)
            {
                pass_N_jets++;                  //passes number of hard jets test
                npass++;
                
                cut_N_jets = true;
            }
            

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
                
                MissingHT += jet->P4();
            }
            
            ScalarMissingHT = MissingHT.Pt();
            
            
            
            biaseddeltaphi = BiasedDeltaPhi(vectorjet, N_jets);
            
            if(biaseddeltaphi > 0.5)
            {
                pass_biaseddeltaphi++;
                npass++;
                
                cut_biaseddeltaphi = true;
            }
            

            if(higgsdecay == 0 && N_bjets > 1)
            {
                pass_N_b_jets++;
                npass++;                   //passes the number of b-jets test
                cut_N_bjets = true;
                
                matchingbjets = JetPairFinder(vectorbjet, N_bjets);
                
                p4[0] = matchingbjets[0]->P4();
                p4[1] = matchingbjets[1]->P4();

                mbb = ((p4[0]) + (p4[1])).M();

                if(mbb > minMbb && mbb < maxMbb)
                {
                    npass++;
                    pass_bb_mass++;               //passes the M_bb inv. mass test
                    cut_Mbb = true;
                    cut_DeltaR = true;
                    
                    DeltaR = p4[0].DeltaR(p4[1]);
                }
            }
            else if(higgsdecay == 1 && N_bjets > 3)
            {
                pass_N_b_jets++;
                npass += 2;                   //passes the number of b-jets test
                cut_N_bjets = true;
                
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
                    
                    cut_Mbb = true;
                    cut_DeltaR = true;
                    
                    DeltaR = p4[0].DeltaR(p4[1]);
                    DeltaR2 = p4[2].DeltaR(p4[3]);
                }
            }
            
            else if(higgsdecay == 2 && N_bjets > 2)
            {
                pass_N_b_jets++;
                npass += 2;                   //passes the number of b-jets test
                cut_N_bjets = true;
                
                if(jetmatchingalgo == 0)     //Jet pairs with smallest average Delta-R
                {
                    matchingbjets = Jet2Plus1Finder(vectorbjet, N_bjets);
                }
                else if(jetmatchingalgo == 1)                   //Jet pairs with closest M_inv(bb)
                {
                    matchingbjets = Jet2Plus1MbbFinder(vectorbjet, N_bjets);
                }
                
                p4[0] = matchingbjets[0]->P4();
                p4[1] = matchingbjets[1]->P4();
                
                mbb = ((p4[0]) + (p4[1])).M();
                mbb2 = matchingbjets[2]->Mass;
                
                
                
                if(mbb > minMbb && mbb < maxMbb)
                {
                    npass += 2;
                    pass_bb_mass++;               //passes the M_bb inv. mass test
                    cut_Mbb = true;
                    cut_DeltaR = true;
                    
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
                cut_MET = true;
            }
            
            for (int v = 0; v < branchScalarHT->GetEntries(); v++)
            {
                Double_t HTv = ((ScalarHT*) branchScalarHT->At(v))->HT;
                HT += HTv;
            }
            
            if(HT > minHT)
            {
                pass_HT++;
                npass++;                    //passes the HT test
                cut_HT = true;
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
        

        if(npass == 9)
        {
            eventpass++;

            histMbb->Fill(mbb);
            histnbjet->Fill(N_bjets);
            histmet->Fill(met);
            histDeltaR->Fill(DeltaR);
            histBiasedDeltaPhi->Fill(biaseddeltaphi);
            histHT->Fill(HT);
            histnjet->Fill(N_jets);
            
            if(higgsdecay == 1)
            {
                histMbb->Fill(mbb2);
                
                histDeltaR->Fill(DeltaR2);
            }
            
            histMbb_precut->Fill(mbb);
            histnbjet_precut->Fill(N_bjets);
            histmet_precut->Fill(met);
            histDeltaR_precut->Fill(DeltaR);
            histBiasedDeltaPhi_precut->Fill(biaseddeltaphi);
            histHT_precut->Fill(HT);
            histnjet_precut->Fill(N_jets);
            
            if(higgsdecay == 1)
            {
                histMbb->Fill(mbb2);
                
                histDeltaR->Fill(DeltaR2);
            }
            
            histMHT->Fill(ScalarMissingHT);
            
        }
        else if(npass == 8)
        {
            if(!cut_Mbb) histMbb_precut->Fill(mbb);
            else if(!cut_N_bjets) histnbjet_precut->Fill(N_bjets);
            else if(!cut_MET) histmet_precut->Fill(met);
            else if(!cut_DeltaR) histDeltaR_precut->Fill(DeltaR);
            else if(!cut_biaseddeltaphi) histBiasedDeltaPhi_precut->Fill(biaseddeltaphi);
            else if(!cut_HT) histHT_precut->Fill(HT);
            else if(!cut_N_jets) histnjet_precut->Fill(N_jets);
            
            if(higgsdecay == 1)
            {
                if(!cut_Mbb) histMbb_precut->Fill(mbb2);
                
                if(!cut_DeltaR) histDeltaR_precut->Fill(DeltaR2);
            }
        }


    }
    
    cout << "\n" << endl;
    
    if(higgsdecay == 0)
    {
        histMbb->SetTitle("M_{inv}(b, b) (h->bb and h->tau-tau); M_{inv}(b, b) (GeV); Events / 5 GeV");
        histnbjet->SetTitle("Number of b-jets (h->bb and h->tau-tau); No. b-jets");
        histmet->SetTitle("Missing ET (h->bb and h->tau-tau); MET (GeV); Events / 20 GeV");
    }
    
    
    //--------- # of jets plots (with and without the cut)
    TCanvas * cnjet = new TCanvas("cnjet", "cnjet", 600, 600);
    
    histnjet->Draw();
    cnjet->Update();
    cnjet->SaveAs("N_jets.pdf");

    TCanvas * cnjet_precut = new TCanvas("cnjet_precut", "cnjet_precut", 600, 600);
    
    histnjet_precut->Draw();
    cnjet_precut->Update();
    cnjet_precut->SaveAs("N_jets_precut.pdf");

    
    
    //----------- M_inv. b-bbar plots (with/without cuts)
    TCanvas * cmbb = new TCanvas("cmbb", "cmbb", 600, 600);
    
    histMbb->Draw();
    cmbb->Update();
    
    if(higgsdecay == 0)
    {
        cmbb->SaveAs("Mbb_tau.pdf");
    }
    else
    {
        cmbb->SaveAs("Mbb.pdf");
    }
    
    TCanvas * cmbb_precut = new TCanvas("cmbb_precut", "cmbb_precut", 600, 600);
    
    histMbb_precut->Draw();
    cmbb_precut->Update();
    
    if(higgsdecay == 0)
    {
        cmbb_precut->SaveAs("Mbb_tau_precut.pdf");
    }
    else
    {
        cmbb_precut->SaveAs("Mbb_precut.pdf");
    }

    
    
    //------------ # of b-jets plots (with/without the cut)

    TCanvas * cbjet = new TCanvas("cbjet", "cbjet", 600, 600);

    histnbjet->Draw();
    cbjet->Update();

    if(higgsdecay == 0)
    {
        cbjet->SaveAs("n_b_jets_tau.pdf");
    }
    else
    {
        cbjet->SaveAs("n_b_jets.pdf");
    }
    
    TCanvas * cbjet_precut = new TCanvas("cbjet_precut", "cbjet_precut", 600, 600);
    
    histnbjet_precut->Draw();
    cbjet_precut->Update();
    
    if(higgsdecay == 0)
    {
        cbjet_precut->SaveAs("n_b_jets_tau_precut.pdf");
    }
    else
    {
        cbjet_precut->SaveAs("n_b_jets_precut.pdf");
    }
    
    
    //-------------- MET Plots (with/without cut)
    
    TCanvas * cmet = new TCanvas ("cmet", "cmet", 600, 600);
    
    histmet->Draw();
    cmet->Update();
    
    if(higgsdecay == 0)
    {
        cmet->SaveAs("MET_tau.pdf");
    }
    else
    {
        cmet->SaveAs("MET.pdf");
    }
    
    TCanvas * cmet_precut = new TCanvas ("cmet_precut", "cmet_precut", 600, 600);
    
    histmet_precut->Draw();
    cmet_precut->Update();
    
    if(higgsdecay == 0)
    {
        cmet_precut->SaveAs("MET_tau_precut.pdf");
    }
    else
    {
        cmet_precut->SaveAs("MET_precut.pdf");
    }
    
    
    
    //---------- Delta-R between b-jets (With/Without the cut)
    
    TCanvas * cdelr = new TCanvas ("cdelr", "cdelr", 600, 600);
    
    histDeltaR->Draw();
    cdelr->Update();
    

    if(higgsdecay == 0)
    {
        cdelr->SaveAs("DeltaR_tau.pdf");
    }
    else
    {
        cdelr->SaveAs("DeltaR.pdf");
    }
    
    TCanvas * cdelr_precut = new TCanvas ("cdelr_precut", "cdelr_precut", 600, 600);
    
    histDeltaR_precut->Draw();
    cdelr_precut->Update();
    
    
    if(higgsdecay == 0)
    {
        cdelr_precut->SaveAs("DeltaR_tau_precut.pdf");
    }
    else
    {
        cdelr_precut->SaveAs("DeltaR_precut.pdf");
    }
    
    
    
    //---------- Missing HT (There's currently no cut on this)
    TCanvas * cmht = new TCanvas("cmht", "cmht", 600, 600);
    
    histMHT->Draw();
    cmht->Update();
    
    if(higgsdecay == 0)
    {
        cmht->SaveAs("MissingHT_tau.pdf");
    }
    else
    {
        cmht->SaveAs("MissingHT.pdf");
    }

    
    //---------- Biased Delta-Phi (With/Without the Cut)
    TCanvas * cbdp = new TCanvas("cbdp", "cbdp", 600, 600);
    
    histBiasedDeltaPhi->Draw();
    cbdp->Update();
    
    if(higgsdecay == 0)
    {
        cbdp->SaveAs("BiasedDeltaPhi_tau.pdf");
    }
    else
    {
        cbdp->SaveAs("BiasedDeltaPhi.pdf");
    }
    
    TCanvas * cbdp_precut = new TCanvas("cbdp_precut", "cbdp_precut", 600, 600);
    
    histBiasedDeltaPhi_precut->Draw();
    cbdp_precut->Update();
    
    if(higgsdecay == 0)
    {
        cbdp_precut->SaveAs("BiasedDeltaPhi_tau_precut.pdf");
    }
    else
    {
        cbdp_precut->SaveAs("BiasedDeltaPhi_precut.pdf");
    }
    
    
    
    //------------ Outputting the results...

    efficiency = double(eventpass)/double(entries);

    cout << "\n" << endl;
    if(higgsdecay == 0)
    {
        cout << "Higgs to bb and Higgs to tau-tau required\n" << endl;
        
        outputfile << "Higgs to bb and Higgs to tau-tau required\n" << endl;
    }
    else
    {
        cout << "Higgs to bb in both cascades required\n" << endl;
        
        outputfile << "Higgs to bb in both cascades required\n" << endl;
    }
    cout << pass_jets << " or " << 100.*double(pass_jets)/double(entries) << "percent of events contained 4 leading jets with PT 400,300,200,100 GeV" << endl;
    cout << pass_N_b_jets << " or " << 100.*double(pass_N_b_jets)/double(entries) << "percent of events contained at least 2 b-jets" << endl;
    cout << pass_bb_mass << " or " << 100.*double(pass_bb_mass)/double(entries) << "percent of events contained at least 2 b-jets with invariant mass within the bounds" << endl;
    cout << pass_MET << " or " << 100.*double(pass_MET)/double(entries) << "percent of events had at least 30GeV Missing ET" << endl;
    cout << pass_tau << " or " << 100.*double(pass_tau)/double(entries) << "percent of events contained at least 2 tau with SUM(PT) > 100GeV" << endl;
    cout << pass_HT << " or " << 100.*double(pass_HT)/double(entries) << "percent of events contained at least " << minHT << "GeV HT" << endl;
    cout << pass_N_jets << " or " << 100.*double(pass_N_jets)/double(entries) << "percent of events contained at least " << minN_jets << " jets" << endl;
    cout << pass_biaseddeltaphi << " or " << 100.*double(pass_biaseddeltaphi)/double(entries) << "percent of events had biased delta-phi > 0.5" << endl;
    cout << "\n" << 100.*eventpass << "percent efficiency" << endl;
    
    cout << "\n\n\n" << endl;
    
    cout << "Cross-section is now reduced by factor of " << efficiency << "\n\n" << endl;
    cout << "\033[32m" << "Winner winner, chicken dinner\n" << "\033[0m" << "\n\n" << endl;
    cout << "Cuts (Energy, masses, PT in GeV):\n" << endl;
    cout << "PT of 1st-4th leading jets = " << jetPT1 << ", " << jetPT2 << ", " << jetPT3 << ", " << jetPT4 << " respectively" << endl;
    cout << "Min. b-jet PT = " << bjetminPT << endl;
    cout << "Min. Missing ET = " << minMET << endl;
    
    
    //--------- And outputting them to a file too...
    outputfile << pass_jets << " or " << 100.*double(pass_jets)/double(entries) << "percent of events contained 4 leading jets with PT 400,300,200,100 GeV" << endl;
    outputfile << pass_N_b_jets << " or " << 100.*double(pass_N_b_jets)/double(entries) << "percent of events contained at least 2 b-jets" << endl;
    outputfile << pass_bb_mass << " or " << 100.*double(pass_bb_mass)/double(entries) << "percent of events contained at least 2 b-jets with invariant mass within the bounds" << endl;
    outputfile << pass_MET << " or " << 100.*double(pass_MET)/double(entries) << "percent of events had at least 30GeV Missing ET" << endl;
    outputfile << pass_tau << " or " << 100.*double(pass_tau)/double(entries) << "percent of events contained at least 2 tau with SUM(PT) > 100GeV" << endl;
    outputfile << pass_HT << " or " << 100.*double(pass_HT)/double(entries) << "percent of events contained at least " << minHT << "GeV HT" << endl;
    outputfile << pass_N_jets << " or " << 100.*double(pass_N_jets)/double(entries) << "percent of events contained at least " << minN_jets << " jets" << endl;
    outputfile << pass_biaseddeltaphi << " or " << 100.*double(pass_biaseddeltaphi)/double(entries) << "percent of events had biased delta-phi > 0.5" << endl;
    outputfile << "\n" << 100.*eventpass << "percent efficiency" << endl;
    
    outputfile << "\n\n\n" << endl;
    
    outputfile << "Cross-section is now reduced by factor of " << efficiency << "\n\n" << endl;
    outputfile << "\033[32m" << "Winner winner, chicken dinner\n" << "\033[0m" << "\n\n" << endl;
    outputfile << "Cuts (Energy, masses, PT in GeV):\n" << endl;
    outputfile << "PT of 1st-4th leading jets = " << jetPT1 << ", " << jetPT2 << ", " << jetPT3 << ", " << jetPT4 << " respectively" << endl;
    outputfile << "Min. b-jet PT = " << bjetminPT << endl;
    outputfile << "Min. Missing ET = " << minMET << endl;
    
    if(higgsdecay == 0)
    {
        cout << "M_tautau from " << minTauinvmass << " to " << maxTauinvmass << endl;
        cout << "Min. Sum of taus' PT = " << minSumTauPT << endl;
        
        outputfile << "M_tautau from " << minTauinvmass << " to " << maxTauinvmass << endl;
        outputfile << "Min. Sum of taus' PT = " << minSumTauPT << endl;
    }
    cout << "M_bb from " << minMbb << " to " << maxMbb << "\n\n" << endl;
    outputfile << "M_bb from " << minMbb << " to " << maxMbb << "\n\n" << endl;

    cout << "Plots:\n" << endl;
    
    if(eventpass != 0)
    {
        TerminalPlot(histMbb, "M_bb", 40, minMbb, maxMbb);
        TerminalPlot(histnbjet, "No. of b-jets", 40, 0.0, 10.0);
        TerminalPlot(histmet, "Missing ET", 40, minMET, 1000.);
        TerminalPlot(histDeltaR, "b-Jets DeltaR", 40, 0, 6);
        TerminalPlot(histMHT, "Missing HT", 40, 0., 1000.);
    }

    f->Close();
    outputfile.close();
    
    
    //----------- Writing all the histos into a .root file
    TTimeStamp time;
    string rootfile = "ROOTCuts_" + to_string(*filename) + to_string(time.GetDate()) + "_" + to_string(time.GetTime()) + ".root";
    const char * rootfilename = rootfile.c_str();
    TFile *g = TFile::Open(rootfilename,"NEW");
    
    histMHT->Write();
    
    //histMbb->Write();
    histMbb_precut->Write();
    
    histBiasedDeltaPhi->Write();
    histBiasedDeltaPhi_precut->Write();
    
    histmet->Write();
    histmet_precut->Write();
    
    histDeltaR->Write();
    histDeltaR_precut->Write();
    
    histnbjet->Write();
    histnbjet_precut->Write();
    
    histnjet->Write();
    histnjet_precut->Write();
    
    
    g->Close();
    
//    delete histBiasedDeltaPhi_precut;
//    delete histBiasedDeltaPhi;
//    delete histMHT;
//    delete histnjet_precut;
//    delete histHT;
//    delete histMbb;
//    delete histmet;
//    delete histnjet;
//    delete histMbb_precut;
//    delete histmet_precut;

};

