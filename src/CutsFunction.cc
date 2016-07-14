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


using namespace std;


void CutsFunction(const char* filename, map<string, double> parameters)
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
    
    int higgsdecay = int(parameters["higgsdecay"]);
    if(higgsdecay < 0 || higgsdecay > 2)
    {
        cout << "ERROR: Higgs Decay mode must be 0, 1 or 2" << endl;
        return;
    }
    
    double bjetminPT, taujetminPT;
    
    double minMET = parameters["minMET"];
    
    double minTauinvmass = parameters["minTauinvmass"];
    double maxTauinvmass = parameters["maxTauinvmass"];
    
    double minSumTauPT = parameters["minSumTauPT"];
    
    double minMbb = parameters["minMbb"];
    double maxMbb = parameters["maxMbb"];
    
    int jetmatchingalgo = parameters["jetmatchingalgo"];
    
	double sigbkgratio = parameters["sigbkgratio"];

	int i, k, l, entries, npass, N_bjets, N_taujets, N_PT;

    double mtautau, PT_tau, met, efficiency;
    
    double mbb = 0;
    double mbb2 = 0;
    
    int jetshardcut;
    
    double DeltaR, DeltaR2;
    
    int no_jets, percent, tintin;

    string bar;
    
	string filename2, title, jetpt_index;
    
    
	
    //---------Opening the .root file:
    
    
    TFile *f = TFile::Open(filename,"UPDATE");

    TChain chain("Delphes");
    chain.Add(filename);
    ExRootTreeReader *reader = new ExRootTreeReader(&chain);

    
    //--------- Load the Delphes branches ready for the cuts
    TClonesArray *branchJet = reader->UseBranch("Jet");
    TClonesArray *branchMET = reader->UseBranch("MissingET");
    TClonesArray *branchParticle = reader->UseBranch("Particle");
    TClonesArray *branchTrack = reader->UseBranch("Track");
    TClonesArray *branchTower = reader->UseBranch("Tower");
    TClonesArray *branchEFlowTrack = reader->UseBranch("EFlowTrack");
    TClonesArray *branchEFlowPhoton = reader->UseBranch("EFlowPhoton");
    TClonesArray *branchEFlowNeutralHadron = reader->UseBranch("EFlowNeutralHadron");
    TClonesArray *branchGenJet = reader->UseBranch("GenJet");
    TClonesArray *branchElectron = reader->UseBranch("Electron");
    TClonesArray *branchPhoton = reader->UseBranch("Photon");
    TClonesArray *branchMuon = reader->UseBranch("Muon");
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
    TH1 *histnbjet = new TH1F("nbjet", "Number of b-jets (h->bb in both cascades); No. b-jets", 10, 0.0, 10.0);
    TH1 *histMbb = new TH1F("mbb", "M_{inv}(b, b) (h->bb in both cascades); M_{inv}(b, b) (GeV)", 20, minMbb, maxMbb);
    TH1 *histmet = new TH1F ("met", "Missing ET (h->bb in both cascades); MET (GeV)", 50, 0.0, 1000.);
    TH1 *histDeltaR = new TH1F("DeltaR", "Delta R between b-jets; Delta R", 60, 0, 6);



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
    
    if(parameters.count("no_jets") == 1)
    {
        no_jets = parameters["no_jets"];
    }
    else
    {
        for(int jetno=1; jetno<5; jetno++)
        {
            jetpt_index = "jet_PT" + to_string(5 - jetno);
            if(parameters.count(jetpt_index) == 1)
            {
                no_jets = jetno;
            }
            else
            {
                no_jets = 0;
            }
        }
    }
    
    vector<double> jetPTcut;
    
    int njetcuts = 0;
    
    for(int jetptno=1; jetptno<5; jetptno++)
    {
        jetpt_index = "jet_PT" + to_string(jetptno);
        if(parameters.count(jetpt_index) == 1)
        {
            jetPTcut.push_back(parameters[jetpt_index]);
            njetcuts += 1;
        }
    }

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
        
        jetshardcut = 0;
        
        N_bjets = 0;
        N_taujets = 0;
        PT_tau = 0.0;
        N_PT = 0;
        
        //------------- # of jets cut
        if(branchJet->GetEntries() > no_jets)
        {
            pass_N_jets++;                  //passes number of hard jets test
            npass++;
        }
        
        
        //------------- Loop over jets, find b-jets, taus, whatever else.
        for(k=0; k<branchJet->GetEntries(); k++)
        {
            jet = (Jet*) branchJet->At(k);

            vectorjet.push_back(jet);
            
            if(parameters.count("n_bjets") == 1)
            {
                if(parameters.count("bjetminPT") == 1)
                {
                    bjetminPT = parameters["bjetminPT"];
                }
                else
                {
                    bjetminPT = 0;
                }
                
                
                if(jet->BTag && jet->PT > bjetminPT)
                {
                    vectorbjet.push_back(jet);
                    N_bjets++;
                }
            }
            else if(parameters.count("n_taujets") == 1)
            {
                if(parameters.count("taujetminPT") == 1)
                {
                    taujetminPT = parameters["taujetminPT"];
                }
                else
                {
                    taujetminPT = 0;
                }
                
                
                if(jet->TauTag && jet->PT > taujetminPT)
                {
                    vectortaujet.push_back(jet);
                    N_taujets++;
                }
            }
        }

        if(parameters["higgsdecay"] == 0 && N_bjets > 1)
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
        else if(parameters["higgsdecay"] == 1 && N_bjets > 3)
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
        
        else if(parameters["higgsdecay"] == 2 && N_bjets > 2)
        {
            pass_N_b_jets++;
            npass += 2;                   //passes the number of b-jets test
            
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
                
                DeltaR = p4[0].DeltaR(p4[1]);
            }
        }
        
        for (vector<double>::iterator it = jetPTcut.begin() ; it != jetPTcut.end(); ++it)
        {
            if(distance(jetPTcut.begin(), it) < no_jets + 1 && vectorjet[distance(jetPTcut.begin(), it)]->PT < *it)
            {
                jetshardcut += 1;
            }
        }
        
        if(jetshardcut == 0)
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

        if(higgsdecay == 0 && N_taujets > 1)
        {
            matchingtaujets = JetPairFinder(vectortaujet, N_taujets);

            p4[2] = matchingtaujets[0]->P4();
            p4[3] = matchingtaujets[1]->P4();

            mtautau = ((p4[2]) + (p4[3])).M();

            if(mtautau > minTauinvmass && mtautau < maxTauinvmass)
            {
                npass++;                //passes the tautau inv. mass test
                pass_tautau_mass++;
            }

            for(l=0; l<N_taujets; l++)
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

        histMbb->Fill(mbb);
        histnbjet->Fill(N_bjets);
        histmet->Fill(met);
        histDeltaR->Fill(DeltaR);
        if(higgsdecay == 1)
        {
            histMbb->Fill(mbb2);
            
            histDeltaR->Fill(DeltaR2);
        }
    }

    
    cout << "\n" << endl;
    
    if(parameters["higgsdecay"] == 0)
    {
        histMbb->SetTitle("M_{inv}(b, b) (h->bb and h->tau-tau); M_{inv}(b, b) (GeV); Events / 5 GeV");
        histnbjet->SetTitle("Number of b-jets (h->bb and h->tau-tau); No. b-jets");
        histmet->SetTitle("Missing ET (h->bb and h->tau-tau); MET (GeV); Events / 20 GeV");
    }

    TCanvas * cmbb = new TCanvas("cmbb", "cmbb", 600, 600);
    
    histMbb->Draw();
    cmbb->Update();
    
    if(parameters["higgsdecay"] == 0)
    {
        cmbb->SaveAs("Mbb_tau.pdf");
    }
    else
    {
        cmbb->SaveAs("Mbb.pdf");
    }

    //cout << histMbb->GetBinContent(10) << "\n" << endl;

    TCanvas * cbjet = new TCanvas("cbjet", "cbjet", 600, 600);

    histnbjet->Draw();
    cbjet->Update();

    if(parameters["higgsdecay"] == 0)
    {
        cbjet->SaveAs("n_b_jets_tau.pdf");
    }
    else
    {
        cbjet->SaveAs("n_b_jets.pdf");
    }
    
    
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
    
    
    TCanvas * cdelr = new TCanvas ("cdelr", "cdelr", 600, 600);
    
    histDeltaR->Draw();
    cdelr->Update();
    

    if(parameters["higgsdecay"] == 0)
    {
        cdelr->SaveAs("DeltaR_tau.pdf");
    }
    else
    {
        cdelr->SaveAs("DeltaR.pdf");
    }
    


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
    cout << "PT of 1st-4th leading jets = ";
    for(int n=0; n<njetcuts; n++)
    {
        cout << jetPTcut[n] << ", ";
    }
    cout << "respectively" << endl;
    cout << "Min. b-jet PT = " << bjetminPT << endl;
    cout << "Min. Missing ET = " << minMET << endl;
    
    outputfile << pass_N_jets << " events contained at least 4 jets" << endl;
    outputfile << pass_jets << " events contained 4 leading jets with PT 400,300,200,100 GeV" << endl;
    outputfile << pass_N_b_jets << " events contained at least 2 b-jets" << endl;
    outputfile << pass_bb_mass << " events contained at least 2 b-jets with invariant mass within the bounds" << endl;
    outputfile << pass_MET << " events had at least 30GeV Missing ET" << endl;
    outputfile << pass_tau << " events contained at least 2 tau with SUM(PT) > 100GeV" << endl;
    outputfile << "\n" << eventpass << " events passed all tests" << endl;
    
    outputfile << "\n\n\n" << endl;
    
    outputfile << "Cross-section is now reduced by factor of " << efficiency << "\n\n" << endl;
    outputfile << "\033[32m" << "Winner winner, chicken dinner\n" << "\033[0m" << "\n\n" << endl;
    outputfile << "Cuts (Energy, masses, PT in GeV):\n" << endl;
    outputfile << "PT of 1st-4th leading jets = ";
    for(int n=0; n<njetcuts; n++)
    {
        outputfile << jetPTcut[n] << ", ";
    }
    outputfile << "respectively" << endl;
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
    }

//f->Write();
f->Close();
    outputfile.close();

};

