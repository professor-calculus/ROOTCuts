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


void CutsFunction(const char* filename, double params[24])
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
    //      16      minimum Biased-Delta-Phi: 0.5 for SUSY CMS Searches usually
    //      17      minimum Missing HT
    //      18      Lumi mode: 0 = off. 1 = on, return numbers of events at lumi before/after cuts. 2 = on, also scale histos.
    //      19      Luminosity
    //      20      Cross-section: Will be overridden if in FOLDER mode!
    //      21      Scan Mode (automatically set, does not matter)

    
    double jetPT1 = params[0];
    double jetPT2 = params[1];
    double jetPT3 = params[2];
    double jetPT4 = params[3];
    
    int higgsdecay = int(params[4]);
    if(higgsdecay < 0 || higgsdecay > 3)
    {
        cout << "ERROR: Higgs Decay mode must be 0, 1, 2 or 3" << endl;
        return;
    }
    
    double bjetminPT = params[5];
    
    double minMET = 0;
    minMET = params[6];
    
    double minTauinvmass = params[7];
    double maxTauinvmass = params[8];
    
    double minSumTauPT = params[9];
    
    double minMbb = 0;
    minMbb = params[10];
    double maxMbb = 16000;
    maxMbb = params[11];
    
    int jetmatchingalgo = params[12];
    
	double sigbkgratio = params[13];
    
    double minHT = 0;
    minHT = params[14];
    
    int minN_jets = 0;
    minN_jets = params[15];
    
    double BDP = 0;
    BDP = params[16];
    
    double min_MHT = params[17];
    
    int lumimode = 0;
    lumimode = params[18];              //
                                        //
    double lumi = 1000.0*params[19];    //
                                        //      These will be used further down to set histo, #events scales.
    double crosssec = params[20];       //
                                        //
    double scale, histoscale;           //
    
    int scan_mode = params[21];
    

	int i, k, l, entries, npass, N_bjets, N_tau, N_PT, N_jets;

    double mtautau, PT_tau, met, efficiency;
    
    double mbb = 0;
    double mbb2 = 0;
    
    double DeltaR, DeltaR2, biaseddeltaphi, HT;
    
    int percent, tintin;
    string n_b;

    string bar;
    
	string filename2,title, outputcountfile;
    
    if(scan_mode == 1)
    {
        if(higgsdecay == 0)
        {
            outputcountfile = "../../efficiencies_2b_2tau.root";
            n_b = "ge2";
        }
        else if(higgsdecay == 1)
        {
            outputcountfile = "../../efficiencies_4b.root";
            n_b = "ge4";
        }
        else if(higgsdecay == 2)
        {
            outputcountfile = "../../efficiencies_ge3b.root";
            n_b = "ge3";
        }
        else
        {
            outputcountfile = "../../efficiencies_2b.root";
            n_b = "=2";
        }
    }
    else
    {
        if(higgsdecay == 0)
        {
            outputcountfile = "efficiencies_2b_2tau.root";
            n_b = "ge2";
        }
        else if(higgsdecay == 1)
        {
            outputcountfile = "efficiencies_4b.root";
            n_b = "ge4";
        }
        else if(higgsdecay == 2)
        {
            outputcountfile = "efficiencies_ge3b.root";
            n_b = "ge3";
        }
        else
        {
            outputcountfile = "efficiencies_2b.root";
            n_b = "=2";
        }
    }
    
	
    //---------Opening the .root file:
    
    
    //TFile *f = TFile::Open(filename,"UPDATE");
    

    TChain chain("Delphes");
    chain.Add(filename);
    ExRootTreeReader *reader = new ExRootTreeReader(&chain);

    TClonesArray *branchJet = reader->UseBranch("Jet");
    TClonesArray *branchMET = reader->UseBranch("MissingET");
    TClonesArray *branchScalarHT = reader->UseBranch("ScalarHT");
    TClonesArray *branchParticle;
    
    if(params[22] == -1)
    {
        branchParticle = reader->UseBranch("Particle");
    }
    
    //--------Tell it not to panic if there's no entries - it's better than a segfault!
    if (reader->GetEntries() < 1)
    {
        cout << "Problem! There's no entries!" << endl;
        return;
    }
        
    entries = reader->GetEntries();

	double weight = double(entries)*sigbkgratio;

    cout << "Tree copied with " << entries << " entries\n\n" << endl;

    
    
    // Book histograms
    TH1 *histnbjet = new TH1F("nbjet", "Number of b-jets; No. b-jets", 10, 0.0, 10.0);
    TH1 *histnjet = new TH1F("njet", "Number of Jets; No. Jets", 15, 0.0, 15.0);
    TH1 *histMbb = new TH1F("mbb", "M_{inv}(b, b); M_{inv}(b, b) (GeV)", 40, 0., 200.);
    TH1 *histmet = new TH1F ("met", "Missing ET; MET (GeV)", 50, 0.0, 1000.);
    TH1 *histDeltaR = new TH1F("DeltaR", "Delta R between b-jets; Delta R", 60, 0, 6);
    TH1 *histMHT = new TH1F("MHT", "Missing HT; Missing HT (GeV)", 100, 0., 8000.);
    TH1 *histHT = new TH1F("HT", "Scalar HT; Scalar HT (GeV)", 100, 0., 8000.);
    TH1 *histBiasedDeltaPhi = new TH1F("biaseddeltaphi", "Biased Delta Phi; Biased Delta Phi", 50, 0., 5.);
    
    TH1 *histnbjet_precut = new TH1F("nbjet_n-1cut", "Number of b-jets Before Cut; No. b-jets", 10, 0.0, 10.0);
    TH1 *histnjet_precut = new TH1F("njet_n-1cut", "Number of Jets; No. Jets", 15, 0.0, 15.0);
    TH1 *histMbb_precut = new TH1F("mbb_n-1cut", "M_{inv}(b, b) Before Cut; M_{inv}(b, b) (GeV)", 40, 0., 200.);
    TH1 *histmet_precut = new TH1F ("met_n-1cut", "Missing ET Before Cut; MET (GeV)", 50, 0.0, 1000.);
    TH1 *histDeltaR_precut = new TH1F("DeltaR_n-1cut", "Delta R between b-jets Before Cut; Delta R", 60, 0, 6);
    TH1 *histMHT_precut = new TH1F("MHT_n-1cut", "Missing HT Before Cut; Missing HT (GeV)", 100, 0., 8000.);
    TH1 *histHT_precut = new TH1F("HT_n-1cut", "Scalar HT Before Cut; Scalar HT (GeV)", 100, 0., 8000.);
    TH1 *histBiasedDeltaPhi_precut = new TH1F("biaseddeltaphi_n-1cut", "Biased Delta Phi Before Cut; Biased Delta Phi", 50, 0., 5.);

    
    double Msq, Mlsp;
    int roundedMsq, roundedMlsp;
    
    TH1 *histMsq = new TH1F("histMsq", "histMsq", 3000, 0., 3000.);
//    chain.Draw("Particle.Mass>>histMsq","Particle.PID == 1000002","");
//    Msq = histMsq->GetMean();
//    roundedMsq = 50*round(Msq/50.);
    
    TH1 *histMlsp = new TH1F("histMlsp", "histMlsp", 3000, 0., 3000.);
//    chain.Draw("Particle.Mass>>histMlsp","Particle.PID == 1000022","");
//    Mlsp = histMlsp->GetMean();
//    roundedMlsp = Mlsp;
    
    
    
    static UNCUT uncut;
    static Efficiencies efficiencies;
    

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
    int pass_MHT = 0;

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
    bool cut_MHT = false;
    
    int cumul_Mbb = 0;
    int cumul_DeltaR = 0;
    int cumul_biaseddeltaphi = 0;
    int cumul_MET = 0;
    int cumul_HT = 0;
    int cumul_N_bjets = 0;
    int cumul_N_jets = 0;
    int cumul_MHT = 0;


    TLorentzVector p4[4];
    TLorentzVector MissingHT;
    TVector2 MissingHT2Vector;
    double ScalarMissingHT;
    
    double HT_x, HT_y;
    
    //double cut_efficiencies;
    
    
    TTree *outputtree = new TTree("ROOTCuts","ROOTCuts output TTree");
    
    int32_t *histnbjet_nocuts;
    int32_t *histnjet_nocuts;
//    double_t *histMbb_nocuts;
//    double_t *histmet_nocuts;
//    double_t *histDeltaR_nocuts;
//    double_t *histMHT_nocuts;
//    double_t *histHT_nocuts;
//    double_t *histBiasedDeltaPhi_nocuts;
    
    histnbjet_nocuts = &N_bjets;
    histnjet_nocuts = &N_jets;
    
    
    outputtree->Branch("Uncut", &uncut, "M_bb/D:MET:DeltaR:biaseddeltaphi:HT:MHT:n_bjets/I:n_jets:cut_Mbb/O:cut_DeltaR:cut_biaseddeltaphi:cut_MET:cut_HT:cut_N_bjets:cut_N_jets:cut_MHT");
    
    
//    outputtree->Branch("n_bjet",&histnbjet_nocuts,"I",320000);
//    outputtree->Branch("n_jet",&histnjet_nocuts,"I",320000);
//    outputtree->Branch("M_bb",&histMbb_nocuts,"D",320000);
//    outputtree->Branch("M_ET",&histmet_nocuts,"D",320000);
//    outputtree->Branch("Delta_R",&histDeltaR_nocuts,"D",320000);
//    outputtree->Branch("M_HT",&histMHT_nocuts,"D",320000);
//    outputtree->Branch("Total_HT",&histHT_nocuts,"D",320000);
//    outputtree->Branch("biased_deltaphi",&histBiasedDeltaPhi_nocuts,"D",320000);
//    
//    outputtree->Branch("cut_n_jets",cut_N_jets,"O",320000);
//    outputtree->Branch("cut_n_b_jets",cut_N_bjets,"O",320000);
//    outputtree->Branch("cut_M_bb",cut_Mbb,"O",320000);
//    outputtree->Branch("cut_MET",cut_MET,"O",320000);
//    outputtree->Branch("cut_HT",cut_HT,"O",320000);
//    outputtree->Branch("cut_Delta_R",cut_DeltaR,"O",320000);
//    outputtree->Branch("cut_biaseddeltaphi",cut_biaseddeltaphi,"O",320000);
    
    TTimeStamp time;
    string rootfile = "ROOTCuts_" + to_string(*filename) + to_string(time.GetDate()) + "_" + to_string(time.GetTime()) + ".root";
    const char * rootfilename = rootfile.c_str();
    TFile *g = TFile::Open(rootfilename,"NEW");
    
    
    
    // #events & Histo scaling! Here we allow for the # of events pre/post-cuts to be scaled
    // according to a desired luminosity and cross-section set by the user or read from the pythia log.
    
    if(lumimode == 0)
    {
        scale = 1;
        histoscale = 1;
    }
    else if(lumimode == 1)
    {
        scale = lumi*crosssec/double(entries);
        histoscale = 1;
    }
    else if(lumimode == 2)
    {
        scale = lumi*crosssec/double(entries);
        histoscale = lumi*crosssec/double(entries);
    }
    else
    {
        scale = 1;
        histoscale = 1;
        cout << "No/Wrong Lumi mode given, switching it off (no scaling)." << endl;
    }
    
    int scaledentries = entries*scale;
    
    

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
        Jet *jet2 = NULL;

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
        cut_MHT = false;
        
        HT_x = 0;
        HT_y = 0;
        
        if(params[22] == -1 && i<100)
        {
            for(int l = 0; l<branchParticle->GetEntries(); l++)
            {
                if(((GenParticle*) branchParticle->At(l))->PID == 1000002)
                {
                    Msq = ((GenParticle*) branchParticle->At(l))->Mass;
                    
                    histMsq->Fill(Msq);
                }
                else if(((GenParticle*) branchParticle->At(l))->PID == 1000022)
                {
                    Mlsp = ((GenParticle*) branchParticle->At(l))->Mass;
                    
                    histMlsp->Fill(Mlsp);
                }
            }
        }
        
        N_jets = 0;
        
        for(int lol=0; lol<branchJet->GetEntries(); lol++)
        {
            jet2 = (Jet*) branchJet->At(lol);
            
            if(jet2->PT > 40.)
            {
                N_jets += 1;
            }
        }
        
        
        if(N_jets > 1)
        {
            
            if(N_jets >= minN_jets)
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
                
                if(jet->PT > 40.)
                {
                    HT_x += jet->P4().Px();
                    HT_y += jet->P4().Py();
                }
                
            }
            
            
            
            MissingHT2Vector.Set(MissingHT.Px(), MissingHT.Py());
            
            ScalarMissingHT = TMath::Sqrt((HT_x*HT_x) + (HT_y*HT_y));
            
            if(ScalarMissingHT > min_MHT)
            {
                cut_MHT = true;
                npass++;
                pass_MHT++;
            }
            
            
            
            biaseddeltaphi = BiasedDeltaPhi(vectorjet, N_jets);
            
            if(biaseddeltaphi > BDP)
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
            else if(higgsdecay == 3 && N_bjets == 2)
            {
                pass_N_b_jets++;
                npass+=2;                   //passes the number of b-jets test
                cut_N_bjets = true;
                
                //matchingbjets = JetPairFinder(vectorbjet, N_bjets);
                
                p4[0] = vectorbjet[0]->P4();
                p4[1] = vectorbjet[1]->P4();
                
                mbb = ((p4[0]) + (p4[1])).M();
                
                if(mbb > minMbb && mbb < maxMbb)
                {
                    npass+=2;
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
        
        
        //------------- Uncut variables for .root file
        
        uncut.M_bb = mbb;
        uncut.n_bjets = N_bjets;
        uncut.MET = met;
        uncut.DeltaR = DeltaR;
        uncut.biaseddeltaphi = biaseddeltaphi;
        uncut.HT = HT;
        uncut.n_jets = N_jets;
        
//        if(higgsdecay == 1)
//        {
//            histMbb_nocuts = &mbb2;
//            
//            histDeltaR_nocuts = &DeltaR2;
//        }
        
        uncut.MHT = ScalarMissingHT;
        
        uncut.cut_HT = cut_HT;
        uncut.cut_Mbb = cut_Mbb;
        uncut.cut_MET = cut_MET;
        uncut.cut_DeltaR = cut_DeltaR;
        uncut.cut_N_jets = cut_N_jets;
        uncut.cut_N_bjets = cut_N_bjets;
        uncut.cut_biaseddeltaphi = cut_biaseddeltaphi;
        uncut.cut_MHT = cut_MHT;
        
        
        outputtree->Fill();
        
        
        
        
        //------------- Post-cuts stuff
        
        if(cut_HT)      // I know, it's terrible coding here...
        {
            cumul_HT++;
        }
        if(cut_HT && cut_MET)
        {
            cumul_MET++;
        }
        if(cut_HT && cut_MET && cut_MHT)
        {
            cumul_MHT++;
        }
        if(cut_HT && cut_MET && cut_MHT && cut_N_jets)
        {
            cumul_N_jets++;
        }
        if(cut_HT && cut_MET && cut_MHT && cut_N_jets && cut_N_bjets)
        {
            cumul_N_bjets++;
        }
        if(cut_HT && cut_MET && cut_MHT && cut_N_jets && cut_N_bjets && cut_Mbb)
        {
            cumul_Mbb++;
        }
        if(cut_HT && cut_MET && cut_MHT && cut_N_jets && cut_N_bjets && cut_Mbb && cut_biaseddeltaphi)
        {
            cumul_biaseddeltaphi++;
        }


        if(npass == 10)
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
            histMHT_precut->Fill(ScalarMissingHT);
            
        }
        else if(npass == 9)
        {
            if(!cut_Mbb) histMbb_precut->Fill(mbb);
            else if(!cut_N_bjets) histnbjet_precut->Fill(N_bjets);
            else if(!cut_MET) histmet_precut->Fill(met);
            else if(!cut_DeltaR) histDeltaR_precut->Fill(DeltaR);
            else if(!cut_biaseddeltaphi) histBiasedDeltaPhi_precut->Fill(biaseddeltaphi);
            else if(!cut_HT) histHT_precut->Fill(HT);
            else if(!cut_N_jets) histnjet_precut->Fill(N_jets);
            else if(!cut_MHT) histMHT_precut->Fill(ScalarMissingHT);
            
            if(higgsdecay == 1)
            {
                if(!cut_Mbb) histMbb_precut->Fill(mbb2);
                
                if(!cut_DeltaR) histDeltaR_precut->Fill(DeltaR2);
            }
        }
        else if(npass == 8)
        {
            if(!cut_Mbb) histMbb_precut->Fill(mbb);
        }
        else if(npass == 6)
        {
            if(!cut_N_bjets && !cut_Mbb) histnbjet_precut->Fill(N_bjets);
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
    
    histnjet->Scale(histoscale);
    histnjet->Draw();
    cnjet->Update();
    cnjet->SaveAs("N_jets.pdf");

    TCanvas * cnjet_precut = new TCanvas("cnjet_precut", "cnjet_precut", 600, 600);
    
    histnjet_precut->Scale(histoscale);
    histnjet_precut->Draw();
    cnjet_precut->Update();
    cnjet_precut->SaveAs("N_jets_precut.pdf");

    
    
    //----------- M_inv. b-bbar plots (with/without cuts)
    TCanvas * cmbb = new TCanvas("cmbb", "cmbb", 600, 600);
    
    histMbb->Scale(histoscale);
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
    
    histMbb_precut->Scale(histoscale);
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

    histnbjet->Scale(histoscale);
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
    
    histnbjet_precut->Scale(histoscale);
    histnbjet_precut->Draw();
    cbjet_precut->Update();
    
    if(higgsdecay == 0)
    {
        cbjet_precut->SaveAs("n_b_jets_tau_n-1cut.pdf");
    }
    else
    {
        cbjet_precut->SaveAs("n_b_jets_n-1cut.pdf");
    }
    
    
    //-------------- MET Plots (with/without cut)
    
    TCanvas * cmet = new TCanvas ("cmet", "cmet", 600, 600);
    
    histmet->Scale(histoscale);
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
    
    histmet_precut->Scale(histoscale);
    histmet_precut->Draw();
    cmet_precut->Update();
    
    if(higgsdecay == 0)
    {
        cmet_precut->SaveAs("MET_tau_n-1cut.pdf");
    }
    else
    {
        cmet_precut->SaveAs("MET_n-1cut.pdf");
    }
    
    
    
    //---------- Delta-R between b-jets (With/Without the cut)
    
    TCanvas * cdelr = new TCanvas ("cdelr", "cdelr", 600, 600);
    
    histDeltaR->Scale(histoscale);
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
    
    histDeltaR_precut->Scale(histoscale);
    histDeltaR_precut->Draw();
    cdelr_precut->Update();
    
    
    if(higgsdecay == 0)
    {
        cdelr_precut->SaveAs("DeltaR_tau_n-1cut.pdf");
    }
    else
    {
        cdelr_precut->SaveAs("DeltaR_n-1cut.pdf");
    }
    
    
    
    //---------- Missing HT (There's currently no cut on this)
    TCanvas * cmht = new TCanvas("cmht", "cmht", 600, 600);
    
    histMHT->Scale(histoscale);
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
    
    histBiasedDeltaPhi->Scale(histoscale);
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
    
    histBiasedDeltaPhi_precut->Scale(histoscale);
    histBiasedDeltaPhi_precut->Draw();
    cbdp_precut->Update();
    
    if(higgsdecay == 0)
    {
        cbdp_precut->SaveAs("BiasedDeltaPhi_tau_n-1cut.pdf");
    }
    else
    {
        cbdp_precut->SaveAs("BiasedDeltaPhi_n-1cut.pdf");
    }
    

    
//    histMbb_nocuts->Draw();
//    histnbjet_nocuts->Draw();
//    histmet_nocuts->Draw();
//    histDeltaR_nocuts->Draw();
//    histBiasedDeltaPhi_nocuts->Draw();
//    histHT_nocuts->Draw();
//    histnjet_nocuts->Draw();
//    histMHT_nocuts->Draw();
//    
//    histMbb_nocuts->Write();
//    histnbjet_nocuts->Write();
//    histmet_nocuts->Write();
//    histDeltaR_nocuts->Write();
//    histBiasedDeltaPhi_nocuts->Write();
//    histHT_nocuts->Write();
//    histnjet_nocuts->Write();
//    histMHT_nocuts->Write();
    
    
    //----------- Writing all the histos into a .root file
    
    //------ Pre-cuts

    
    //outputtree->Print();
    outputtree->Write();

    
    //------ Post-cuts
    
    histMHT->Write();
    histMHT_precut->Write();
    
    histMbb->Write("Mbb_hist");
    histMbb_precut->Write("Mbb_hist_precut");
    
    histBiasedDeltaPhi->Write();
    histBiasedDeltaPhi_precut->Write();
    
    histmet->Write();
    histmet_precut->Write();
    
    histDeltaR->Write();
    histDeltaR_precut->Write();
    
    histnbjet->Write("n_b_jet_hist");
    histnbjet_precut->Write("n_b_jet_hist_precuts");
    
    histnjet->Write();
    histnjet_precut->Write();
    
    g->Close();
    
    
    
    //------------ Outputting the results to .txt ...
    
    
    //----- Output File
    
    
    //scaling the event pass #s
    pass_HT = pass_HT*scale;
    pass_MET = pass_MET*scale;
    pass_MHT = pass_MHT*scale;
    pass_tau = pass_tau*scale;
    pass_jets = pass_jets*scale;
    pass_N_jets = pass_N_jets*scale;
    pass_bb_mass = pass_bb_mass*scale;
    pass_N_b_jets = pass_N_b_jets*scale;
    pass_tautau_mass = pass_tautau_mass*scale;
    pass_biaseddeltaphi = pass_biaseddeltaphi*scale;
    
    //string outputfilename = "In_Numbers_" + to_string(time.GetDate()) + ".txt";
    string outputfilename = "output.txt";
    
    ofstream outputfile;
    outputfile.open(outputfilename);
    outputfile << "\n\n\n SIGNAL:" << endl;
    outputfile << "Tree copied with " << entries << " entries\n\n" << endl;
    outputfile << "Scaled to " << scaledentries << " entries\n\n" << endl;
    
    eventpass = eventpass*scale;
    efficiency = double(eventpass)/double(scaledentries);
    
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
    cout << pass_jets << " or \t" << 100.*double(pass_jets)/double(scaledentries) << " % of events contained 4 leading jets with PT 400,300,200,100 GeV" << endl;
    cout << pass_N_b_jets << " or \t" << 100.*double(pass_N_b_jets)/double(scaledentries) << " % of events contained at least 2 b-jets" << endl;
    cout << pass_bb_mass << " or \t" << 100.*double(pass_bb_mass)/double(scaledentries) << " % of events contained at least 2 b-jets with invariant mass within the bounds" << endl;
    cout << pass_MET << " or \t" << 100.*double(pass_MET)/double(scaledentries) << " % of events had at least 30GeV Missing ET" << endl;
    cout << pass_tau << " or \t" << 100.*double(pass_tau)/double(scaledentries) << " % of events contained at least 2 tau with SUM(PT) > 100GeV" << endl;
    cout << pass_HT << " or \t" << 100.*double(pass_HT)/double(scaledentries) << " % of events contained at least " << minHT << "GeV HT" << endl;
    cout << pass_N_jets << " or \t" << 100.*double(pass_N_jets)/double(scaledentries) << " % of events contained at least " << minN_jets << " jets" << endl;
    cout << pass_biaseddeltaphi << " or \t" << 100.*double(pass_biaseddeltaphi)/double(scaledentries) << " % of events had biased delta-phi > " << BDP << endl;
    cout << pass_MHT << " or \t" << 100.*double(pass_MHT)/double(scaledentries) << " % of events had Missing HT > " << min_MHT << endl;
    cout << "\n" << 100.*double(eventpass)/double(scaledentries) << " % efficiency" << endl;
    
    cout << "\n\n\n" << endl;
    
    cout << "Cross-section is now reduced by factor of " << efficiency << "\n\n" << endl;
    cout << "\033[32m" << "Winner winner, chicken dinner\n" << "\033[0m" << "\n\n" << endl;
    cout << "Cuts (Energy, masses, PT in GeV):\n" << endl;
    cout << "PT of 1st-4th leading jets = " << jetPT1 << ", " << jetPT2 << ", " << jetPT3 << ", " << jetPT4 << " respectively" << endl;
    cout << "Min. b-jet PT = " << bjetminPT << endl;
    cout << "Min. Missing ET = " << minMET << endl;
    
    
    //--------- And outputting them to a file too...
    outputfile << pass_jets << " or " << 100.*double(pass_jets)/double(scaledentries) << " % of events contained 4 leading jets with PT 400,300,200,100 GeV" << endl;
    outputfile << pass_N_b_jets << " or " << 100.*double(pass_N_b_jets)/double(scaledentries) << " % of events contained at least 2 b-jets" << endl;
    outputfile << pass_bb_mass << " or " << 100.*double(pass_bb_mass)/double(scaledentries) << " % of events contained at least 2 b-jets with invariant mass within the bounds" << endl;
    outputfile << pass_MET << " or " << 100.*double(pass_MET)/double(scaledentries) << " % of events had at least 30GeV Missing ET" << endl;
    outputfile << pass_tau << " or " << 100.*double(pass_tau)/double(scaledentries) << " % of events contained at least 2 tau with SUM(PT) > 100GeV" << endl;
    outputfile << pass_HT << " or " << 100.*double(pass_HT)/double(scaledentries) << " % of events contained at least " << minHT << "GeV HT" << endl;
    outputfile << pass_N_jets << " or " << 100.*double(pass_N_jets)/double(scaledentries) << " % of events contained at least " << minN_jets << " jets" << endl;
    outputfile << pass_biaseddeltaphi << " or " << 100.*double(pass_biaseddeltaphi)/double(scaledentries) << " % of events had biased delta-phi > " << BDP << endl;
    outputfile << pass_MHT << " or \t" << 100.*double(pass_MHT)/double(scaledentries) << " % of events had Missing HT > " << min_MHT << endl;

    outputfile << "\n" << 100.*double(eventpass)/double(scaledentries) << " % efficiency" << endl;
    
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
    
    outputfile << "Count: " << eventpass << endl;
    
    outputfile.close();
    
    
    //----- Cumulative cuts file
    
    //ofstream outputcount;
    
    TTree *effstree = new TTree("ROOTCuts","ROOTCuts efficiencies TTree");
    
    effstree->Branch("Efficiencies", &efficiencies, "Msq/I:Mlsp:HT:MET:MHT:Nj:Nb:Mbb:BDP:crosssec/D:eff:HTeff:METeff:MHTeff:Njeff:Nbeff:Mbbeff:BDPeff");
    
    double meanMsq, meanMlsp;
    
    if(params[22] == -1)
    {
        meanMsq = histMsq->GetMean();
        meanMlsp = histMlsp->GetMean();
        
        roundedMsq = 50*round(meanMsq/50.);     // Want to round this as the jets in Pythia etc change the mass slightly.
        roundedMlsp = meanMlsp;                 // No need to round this as it won't change (no jets from neutralino)
    }
    else
    {
        roundedMsq = params[22];
        roundedMlsp = params[23];
    }
    
    cout << "M_sq = " << roundedMsq << endl;
    cout << "M_LSP = " << roundedMlsp << endl;
    
    efficiencies.Msq = roundedMsq;
    efficiencies.Mlsp = roundedMlsp;
    efficiencies.HT = cumul_HT*scale;
    efficiencies.MET = cumul_MET*scale;
    efficiencies.MHT = cumul_MHT*scale;
    efficiencies.Nj = cumul_N_jets*scale;
    efficiencies.Nb = cumul_N_bjets*scale;
    efficiencies.Mbb = cumul_Mbb*scale;
    efficiencies.BDP = cumul_biaseddeltaphi*scale;
    
    efficiencies.HTeff = double(cumul_HT)/double(entries);
    cout << double(cumul_HT)/double(entries) << endl;
    efficiencies.METeff = double(cumul_MET)/double(entries);
    efficiencies.MHTeff = double(cumul_MHT)/double(entries);
    efficiencies.Njeff = double(cumul_N_jets)/double(entries);
    efficiencies.Nbeff = double(cumul_N_bjets)/double(entries);
    efficiencies.Mbbeff = double(cumul_Mbb)/double(entries);
    efficiencies.BDPeff = double(cumul_biaseddeltaphi)/double(entries);
    cout << double(cumul_biaseddeltaphi)/double(entries) << endl;
    
    effstree->Fill();
    

    TFile *effsfile = TFile::Open(outputcountfile.c_str(),"UPDATE");
    
    cout << "test" << endl;
    
    effstree->Write();
    
    effsfile->Close();
    

};

