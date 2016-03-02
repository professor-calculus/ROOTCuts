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


void CutsFunction(const char* filename)
{
    gSystem->Load("libTreePlayer");
    //gSystem->Load("/home/ast1g15/delphes/libDelphes.so");
    //gSystem->Load("libExRootAnalysis.so");

	int i, k, l, entries, npass, N_bjets, N_tau, N_PT;

    double mtautau, PT_tau, met, efficiency;
    
    double mbb = 0;
    
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
    TH1 *histnbjet = new TH1F("nbjet", "Number of b-jets", 10, 0.0, 10.0);
    //TH1 *histnptjet = new TH1F("nbjet", "Number of b-jets", 5, 0.0, 5.0);
    TH1 *histMbb = new TH1F("mbb", "M_{inv}(b, b)", 20, 60.0, 160.0);



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
                else if(jet->TauTag) // && jet->Mass > 20 && jet->Mass < 160)
                {
                    vectortaujet.push_back(jet);
                    N_tau++;
                }
            }

            if(N_bjets > 1)
            {
                pass_N_b_jets++;
                npass++;                   //passes the number of b-jets test
                
                matchingbjets = JetPairFinder(vectorbjet, N_bjets);
                
                p4[0] = matchingbjets[0]->P4();
                p4[1] = matchingbjets[1]->P4();

                mbb = ((p4[0]) + (p4[1])).M();

                if(mbb > 60 && mbb < 160)
                {
                    npass++;
                    pass_bb_mass++;               //passes the M_bb inv. mass test
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
                matchingtaujets = JetPairFinder(vectortaujet, N_tau);

                p4[2] = matchingtaujets[0]->P4();
                p4[3] = matchingtaujets[1]->P4();

                mtautau = ((p4[2]) + (p4[3])).M();

                if(mtautau > 20 && mtautau < 160)
                {
                    npass++;                //passes the tautau inv. mass test
                    pass_tautau_mass++;
                }

                for(l=0; l<N_tau; l++)
                {
                    PT_tau += double(vectortaujet[l]->PT);
                    //cout << "Tau PT " << PT_tau << endl;
                }

                if(double(PT_tau) > 100.)
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
        }


    }
    
    cout << "\n" << endl;

    TCanvas * cmbb = new TCanvas("cmbb", "cmbb", 600, 600);

    histMbb->Draw();
    cmbb->Update();

    cmbb->SaveAs("Mbb.pdf");

    //cout << histMbb->GetBinContent(10) << "\n" << endl;

    TCanvas * cbjet = new TCanvas("cbjet", "cbjet", 600, 600);

    histnbjet->Draw();
    cbjet->Update();

    cbjet->SaveAs("n_b_jets.pdf");


    efficiency = double(eventpass)/double(entries);

    cout << "\n" << endl;
    cout << pass_N_jets << " events contained at least 4 jets" << endl;
    cout << pass_jets << " events contained 4 leading jets with PT 400,300,200,100 GeV" << endl;
    cout << pass_N_b_jets << " events contained at least 2 b-jets" << endl;
    cout << pass_bb_mass << " events contained at least 2 b-jets with invariant mass within the bounds" << endl;
    cout << pass_MET << " events had at least 30GeV Missing ET" << endl;
    cout << pass_tau << " events contained at least 2 tau with SUM(PT) > 100GeV" << endl;
    cout << "\n" << eventpass << " events passed all tests" << endl;
    cout << "\n\n\n" << endl;
    cout << "Cross-section is now reduced by factor of " << efficiency << "\n\n" << endl;
    cout << "\033[32m" << "Winner winner, chicken dinner\n" << "\033[0m" << "\n\n\n" << endl;
    
    TerminalPlot(histMbb, "M_bb", 40, 0.0, 160.0);
    TerminalPlot(histnbjet, "No. of b-jets", 40, 0.0, 10.0);

//f->Write();
f->Close();

};

