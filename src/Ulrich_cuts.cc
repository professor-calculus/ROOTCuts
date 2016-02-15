//------------------------------------------------------------
//--------------©Alexander Titterton 11/02/2016---------------
//---------------U. Bristol/U. Southampton/RAL----------------
//------------------------------------------------------------
//
//
//

#include <iostream>
#include "../include/Ulrich_cuts.hh"
#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#else
class ExRootTreeReader;
class ExRootResult;
#endif

using namespace std;


void CutsFunction(const char* filename)
{
	gSystem->Load("libTreePlayer");
    gSystem->Load("/home/ast1g15/delphes/libDelphes");

	int i, j, k, l, entries, npass, N_bjets, N_tau, N_PT;

    double mbb, PT_tau, met;
    
    int percent, tintin;

    string bar;
    
	string filename2,title;
	
    //---------Opening the .root file:
    
    /*
    TFile *f = TFile::Open(filename.c_str());

    //---------Aiming the pointers with the relevant TBranch and TLeaves
    //---------This is not particularly well-documented online,
    //---------Google is not your friend!
    TTree *EM = (TTree*)f->Get("Delphes");

    TBranch *branchJet = (TBranch*)EM->GetBranch("Jet");
    TBranch *Event = (TBranch*)EM->GetBranch("Event");
    TBranch *MissingET = (TBranch*)EM->GetBranch("MissingET");
    TBranch *Particle = (TBranch*)EM->GetBranch("Particle");
    TBranch *Track = (TBranch*)EM->GetBranch("Track");
    
    TLeaf *Btag = branchJet->GetLeaf("Jet.BTag");
    TLeaf *Tautag = branchJet->GetLeaf("Jet.TauTag");
    TLeaf *JetMass = branchJet->GetLeaf("Jet.Mass");
    TLeaf *JetEta = branchJet->GetLeaf("Jet.Eta");
    TLeaf *JetPhi = branchJet->GetLeaf("Jet.Phi");
    TLeaf *NJets = Event->GetLeaf("Jet_size");
    
    */

    TChain chain("Delphes");
    chain.Add(filename);
    ExRootTreeReader *reader = new ExRootTreeReader(&chain);

    TClonesArray *branchJet = reader->UseBranch("Jet");
    TClonesArray *branchEvent = reader->UseBranch("Event");
    TClonesArray *branchMET = reader->UseBranch("MissingET");
    TClonesArray *branchParticle = reader->UseBranch("Particle");
    
    //--------Tell it not to panic if there's no entries - it's better than a segfault!
    if (reader->GetEntries() < 1)
    {
        cout << "Problem" << endl;
    }
        
    entries = reader->GetEntries();

    cout << "Tree copied with " << entries << "entries\n\n\n" << endl;

    // Book histograms
    TH1 *histnbjet = new TH1F("nbjet", "Number of b-jets", 5, 0.0, 5.0);
    TH1 *histnptjet = new TH1F("nbjet", "Number of b-jets", 5, 0.0, 5.0);
    TH1 *histMbb = new TH1F("mbb", "M_{inv}(b, b)", 75, 50.0, 200.0);


    int pass_jets = 0;
    int pass_N_b_jets = 0;
    int pass_PT_b_jets = 0;
    int pass_MET = 0;
    int pass_tau = 0;
    int pass_bb_mass = 0;
    int pass_N_jets = 0;

    
    vector<Jet *> vectorjet;
    vector<Jet *> vectorbjet;
    vector<Jet *> vectortaujet;


    TLorentzVector p4[2];
    

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

                if(jet->PT > 100)
                {
                    N_PT++;
                }

                vectorjet.push_back(jet);

                if(jet->BTag && jet->PT > 40)
                {
                    vectorbjet.push_back(jet);
                    N_bjets++;
                }
                else if(jet->TauTag && jet->Mass > 20 && jet->Mass < 160)
                {
                    vectortaujet.push_back(jet);
                    N_tau++;
                }
            }

            if(N_bjets > 2)
            {
                pass_N_b_jets++;
                npass++;                   //passes the number of b-jets test

                p4[0] = vectorbjet[0]->P4();
                p4[1] = vectorbjet[1]->P4();

                mbb = ((p4[0]) + (p4[1])).M();

                if(mbb > 60 && mbb < 160)
                {
                    npass++;                //passes the M_bb inv. mass test
                }
            }

            if(vectorjet[0]->PT > 400 && vectorjet[1]->PT > 300 && vectorjet[2]->PT > 200 && vectorjet[3]->PT > 100)
            {
                pass_jets++;
                npass++;                    //passes the PT of 4 leading jets test
            }


            for (int m = 0; m < branchMET->GetEntries(); m++)
            {
                Double_t metv = ((MissingET*) branchMET->At(m))->MET;
                met += metv;
            }

            if(met > 30)
            {
                pass_MET++;
                npass++;                    //passes the MET test
            }

            if(N_tau > 1)
            {
                for(l=0; l<N_tau; l++)
                {
                    PT_tau += double(vectortaujet[l]->PT);
                }

                if(PT_tau > 100)
                {
                    npass++;                //passes the total tau transverse momentum test

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

        }

        if(npass == 6)
        {
            histMbb->Fill(mbb);
            histnbjet->Fill(N_bjets);
            histnptjet->Fill(N_PT);
        }



    }
 
//f->Close();   

};

