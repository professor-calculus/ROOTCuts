//
//  YieldGetter.cpp
//  ROOTCuts
//
//  Created by Alex Titterton on 20/02/2017.
//
//

//------------------------------------------------------------
//--------------©Alexander Titterton 11/02/2016---------------
//---------------U. Bristol/U. Southampton/RAL----------------
//------------------------------------------------------------
//
//			Adapted for parallel running in bash
//

#include <iostream>
#include "../include/Ulrich_cuts.hh"
#include "../include/TerminalPlot.hh"
#include "../include/BiasedDeltaPhi.hh"


using namespace std;


Yield YieldGetter(const char* filename, double params[25])
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
    //      21      Scan Mode (automatically set)
    //      22      Mass of Squark (doesn't matter for non-scan mode)
    //      23      Mass of LSP     (doesn't matter for non-scan mode)
    //			24			Max. MHT
    
    
    Yield erroryield;
    erroryield.yield = -1.;
    erroryield.M_Sq = -1;
    erroryield.M_LSP = -1;
    
    double jetPT1 = params[0];
    double jetPT2 = params[1];
    double jetPT3 = params[2];
    double jetPT4 = params[3];
    
    int higgsdecay = int(params[4]);
    if(higgsdecay < 0 || higgsdecay > 3)
    {
        cout << "ERROR: Higgs Decay mode must be 0, 1, 2 or 3" << endl;
        return erroryield;
    }
    
    double bjetminPT = params[5];
    
    double minMET = 0;
    minMET = params[6];
    
    double minTauinvmass = params[7];
    double maxTauinvmass = params[8];
    
    double minSumTauPT = params[9];
    
    double minMbb = -1;
    minMbb = params[10];
    double maxMbb = -1;
    maxMbb = params[11];
    
    int jetmatchingalgo = params[12];
    
    //double sigbkgratio = params[13];
    
    double minHT = 0;
    minHT = params[14];
    
    int minN_jets = 0;
    minN_jets = params[15];
    
    double BDP = 0;
    BDP = params[16];
    
    double min_MHT = params[17];
    double max_MHT = params[24];
    
    int lumimode = 0;
    lumimode = params[18];              //
    //
    double lumi = 1000.0*params[19];    //
    //      These will be used further down to set histo, #events scales.
    double crosssec = params[20];       //
    //
    double scale;           //
    
    int scan_mode = params[21];
    
    
    int i, k, l, entries, npass, N_bjets, N_tau, N_PT, N_jets, N_NLSP, N_LSP;
    
    double mtautau, PT_tau, met;
    
    double mbb = 0;
    double mbb2 = 0;
    
    double DeltaR, DeltaR2, biaseddeltaphi, HT;
    double b_PT[4];
    
    string n_b;
    
    string bar;
    
    string filename2,title, outputcountfile;
    
    
    //---------Opening the .root file:
    
    
    //TFile *f = TFile::Open(filename,"UPDATE");
    
    
    TChain chain("Delphes");
    chain.Add(filename);
    ExRootTreeReader *reader = new ExRootTreeReader(&chain);
    
    TClonesArray *branchJet = reader->UseBranch("Jet");
    TClonesArray *branchMET = reader->UseBranch("MissingET");
    TClonesArray *branchScalarHT = reader->UseBranch("ScalarHT");
    //TClonesArray *branchParticle = reader->UseBranch("Particle");
    
    //--------Tell it not to panic if there's no entries - it's better than a segfault!
    if (reader->GetEntries() < 1)
    {
        cout << "Problem! There's no entries!" << endl;
        return erroryield;
    }
    
    entries = reader->GetEntries();
    
    
    cout << "Tree copied with " << entries << " entries\n\n" << endl;
    
    
    
    // No histograms
    
    
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
    
    vector<GenParticle *> vectorNLSP;
    vector<GenParticle *> vectorLSP;
    
    bool cut_Mbb = false;
    bool cut_DeltaR = false;
    bool cut_biaseddeltaphi = false;
    bool cut_MET = false;
    bool cut_HT = false;
    bool cut_N_bjets = false;
    bool cut_N_jets = false;
    bool cut_MHT = false;
    bool hard_jets = false;
    
    
    TLorentzVector p4[4];
    TLorentzVector MissingHT;
    TVector2 MissingHT2Vector;
    double ScalarMissingHT;
    
    double HT_x, HT_y;

    
    
    
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
        N_NLSP = 0;
        N_LSP = 0;
        
        cut_Mbb = false;
        cut_DeltaR = false;
        cut_biaseddeltaphi = false;
        cut_MET = false;
        cut_HT = false;
        cut_N_bjets = false;
        cut_N_jets = false;
        cut_MHT = false;
        hard_jets = false;
        
        HT_x = 0;
        HT_y = 0;
        
        
        
        
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
                
                if(jet->PT > 40.)					// Should we make this (currently) 40GeV user-input? I think so!
                {
                    HT_x += jet->P4().Px();
                    HT_y += jet->P4().Py();
                }
                
            }
            
            

            
            MissingHT2Vector.Set(MissingHT.Px(), MissingHT.Py());
            
            ScalarMissingHT = TMath::Sqrt((HT_x*HT_x) + (HT_y*HT_y));
            
            if(ScalarMissingHT > min_MHT && (ScalarMissingHT < max_MHT || max_MHT < 0.) )
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
                
                b_PT[0] = matchingbjets[0]->PT;
                b_PT[1] = matchingbjets[1]->PT;
                b_PT[2] = 0.;
                b_PT[3] = 0.;
                
                mbb = ((p4[0]) + (p4[1])).M();
                
                if( (mbb > minMbb && mbb < maxMbb) || (minMbb < 0 && maxMbb < 0) )
                {
                    npass++;
                    pass_bb_mass++;               //passes the M_bb inv. mass test
                    cut_Mbb = true;
                    cut_DeltaR = true;
                    
                    DeltaR = p4[0].DeltaR(p4[1]);
                }
            }
            else if((higgsdecay == 1 || higgsdecay == 2) && N_bjets > 3)
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
                
                b_PT[0] = matchingbjets[0]->PT;
                b_PT[1] = matchingbjets[1]->PT;
                b_PT[2] = matchingbjets[2]->PT;
                b_PT[3] = matchingbjets[3]->PT;
                
                
                mbb = ((p4[0]) + (p4[1])).M();
                mbb2 = ((p4[2]) + (p4[3])).M();
                
                
                
                if( (mbb > minMbb && mbb < maxMbb && mbb2 > minMbb && mbb2 < maxMbb) || (maxMbb < 0 && minMbb < 0) )
                {
                    npass += 2;
                    pass_bb_mass++;               //passes the M_bb inv. mass test
                    
                    cut_Mbb = true;
                    cut_DeltaR = true;
                    
                    DeltaR = p4[0].DeltaR(p4[1]);
                    DeltaR2 = p4[2].DeltaR(p4[3]);
                }
            }
            
            else if(higgsdecay == 2 && N_bjets == 3)
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
                
                b_PT[0] = matchingbjets[0]->PT;
                b_PT[1] = matchingbjets[1]->PT;
                b_PT[2] = matchingbjets[2]->PT;
                b_PT[3] = 0.;
                
                
                
                if( (mbb > minMbb && mbb < maxMbb) || (minMbb < 0 && maxMbb < 0) )
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
                
                b_PT[0] = vectorbjet[0]->PT;
                b_PT[1] = vectorbjet[1]->PT;
                b_PT[2] = 0.;
                b_PT[3] = 0.;
                
                mbb = ((p4[0]) + (p4[1])).M();
                
                if( (mbb > minMbb && mbb < maxMbb) || (minMbb < 0 && maxMbb < 0) )
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
                hard_jets = true;
                npass++;                    //passes the PT of 4 leading jets test
            }
            
            
            for (int m = 0; m < branchMET->GetEntries(); m++)
            {
                Double_t metv = ((MissingET*) branchMET->At(m))->MET;
                met += metv;
            }
            
            if(met > minMET || minMET < 0)
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
            
            if(HT > minHT || minHT < 0)
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
            
        }
        
        
        
        //------------- Post-cuts stuff
        
        if(npass == 10)
        {
            eventpass++;
        }
        
    }
    
    
    
    // #events & Histo scaling! Here we allow for the # of events pre/post-cuts to be scaled
    // according to a desired luminosity and cross-section set by the user or read from the pythia log.
    
    if(lumimode == 0)
    {
        scale = 1;
    }
    else if(lumimode == 1)
    {
        scale = lumi*crosssec/double(entries);
    }
    else if(lumimode == 2)
    {
        scale = lumi*crosssec/double(entries);
    }
    else
    {
        scale = 1;
        cout << "No/Wrong Lumi mode given, switching it off (no scaling)." << endl;
    }
    
    
    
    
    
    cout << "\n" << endl;
    
    
    
    //------------ Outputting the results to .txt ...
    
    
    //----- Output File
    
    
    //scaling the event pass #s
    
    double doubleeventpass = double(eventpass)*double(scale);
    
    int roundedMsq = params[22];
    int roundedMlsp = params[23];
    
    
    Yield yields;
    
    yields.M_LSP = roundedMlsp;
    yields.M_Sq = roundedMsq;
    yields.yield = doubleeventpass;
    
    return yields;
    
};
