//
//  ROOTCuts.cc
//  
//
//  Created by Alex Titterton on or around 01/03/2016.
//
//

#include "include/Ulrich_cuts.hh"
#include <fstream>



int main(int argc, char *argv[])
{
	gSystem->Load("libTreePlayer");
    gSystem->Load("/home/ast1g15/delphes/libDelphes.so");
    //gSystem->Load("libExRootAnalysis.so");
    

    //              Parameters:
    //      0       1st Leading jet PT
    //      1       2nd Leading jet PT
    //      2       3rd Leading jet PT
    //      3       4th Leading jet PT
    //      4       0 = h->bb h->tau-tau; 1 = h->bb both cascades; 2 = 3 b-jets, 3: EXACTLY 2 b-jets
    //      5       b-jet lower bound PT
    //      6       MET lower bound
    //      7       min. taus inv. mass
    //      8       max. taus inv. mass
    //      9       min. Sum taus' PT
    //      10      min. M_bb
    //      11      max. M_bb
    //      12      Jet pair matching algorithm for 2 bb pairs: 0 = Smallest av. Delta-R; 1 = Pairs with closest M_inv(bb)
	//		13		User specify Signal/Background ratio (double precision)
    //      14      min. HT
    //      15      min. no. of jets
    //      16      minimum Biased-Delta-Phi: 0.5 for SUSY CMS Searches usually
    

    
    double params[17];
    
    for(int i=0; i<17; i++)
    {
        params[i] = 0;
    }
    
    int param, bkg, signal;
    double value;

	//double weight = params[13];
    
    
    if(argc == 4 && string(argv[3]) == "FOLDER") // Here we input a MG output folder and a param card, with the
                                                                  // option "folder". (no sig vs. bkg etc)
    {
        cout <<  "FOLDER mode!" << endl;
        
        fstream fin(argv[2]);
        string line, line2;
        
        while(getline(fin, line))
        {
            //the following line trims white space from the beginning of the string
            line.erase(line.begin(), find_if(line.begin(), line.end(), not1(ptr_fun<int, int>(isspace))));
            
            if(line[0] == '#') continue;
            
            istringstream is(line);
            is >> param >> value;
            
            params[param] = value;
        }
        
        string inpstring(argv[1]);
        string rootpath = inpstring + "/Events/run_01/tag_1_delphes_events.root";
        string crosssectionpath = inpstring + "/Events/run_01/tag_1_pythia.log";
        
        CutsFunction(rootpath.c_str(), params);
        
        
        string crosssection;
        double crosssectionvalue = 0;
        
        fstream fin2(crosssectionpath);
        while(getline(fin2, line2))
        {
            //the following line trims white space from the beginning of the string
            //line2.erase(line2.begin(), find_if(line2.begin(), line2.end(), not1(ptr_fun<int, int>(isspace))));
            
            //if(line2.find("Cross section"))
            //{
            
            crosssection = string(line2);
            
            cout << crosssection << endl;
                
                //size_t pos = line2.find(":");
                //line2.erase(0,pos + 4);
                
                //istringstream is2(line2);
                //is2 >> crosssectionvalue;
                
                //break;
            //}
        
        size_t pos = line2.find(":");
        line2.erase(0,pos + 4);
        
            
        }
        
        crosssectionvalue = stod(crosssection);
        
        TTimeStamp time;
        
        string outputfilename = "In_Numbers_" + to_string(time.GetDate()) + ".txt";
        
        ofstream outputfile;
        outputfile.open(outputfilename);
        
        outputfile << "Cross-section from MG5/Pythia =\t" << crosssectionvalue << endl;
        cout << "Cross-section from MG5/Pythia =\t" << crosssectionvalue << endl;
        
    }
    
    
    else if(argc == 4)               // Here we would be feeding the program with 2 .root files (sig, bkg) and a parameter file.
    {
        fstream fin(argv[3]);
        string line;
        
        while(getline(fin, line))
        {
            //the following line trims white space from the beginning of the string
            line.erase(line.begin(), find_if(line.begin(), line.end(), not1(ptr_fun<int, int>(isspace))));
            
            if(line[0] == '#') continue;
            
            istringstream is(line);
            is >> param >> value;
            
            params[param] = value;
        }
        
        int higgsdecay = int(params[4]);
        if(higgsdecay < 0 || higgsdecay > 3)
        {
            cout << "ERROR: Higgs Decay mode must be 0, 1, 2 or 3" << endl;
            return 0;
        }
        
        
        //---- Output File
        ofstream outputfile;
        outputfile.open("output.txt", std::ios_base::app | std::ios_base::out);
        outputfile << "\n\n ######## NEW RUN #######\n\n";
        outputfile.close();
        
        
        TH1 *histMbb = new TH1F("mbb", "M_{inv}(b, b) (h->bb in both cascades); M_{inv}(b, b) (GeV)", 20, params[10], params[11]);
        TH1 *histMbbBkg = new TH1F("mbb_bkg", "", 20, params[10], params[11]);
        
        bkg = CutsFunctionBkg(argv[2], params, "Background", histMbbBkg, 0); //Background
		signal = CutsFunctionBkg(argv[1], params, "Signal", histMbb, bkg);   //Signal
        
        TCanvas * cmbb = new TCanvas("cmbb", "cmbb", 600, 600);
        
        THStack *hs = new THStack("hs","M_{bb}^{inv.} Signal vs Background; M_{bb} / GeV");
        
        hs->Add(histMbb);
        histMbb->SetLineColor(kBlue);
        
        hs->Add(histMbbBkg);
        histMbbBkg->SetLineColor(kRed);
        
        hs->Draw("nostack");

        cmbb->Update();
        
        TLegend *legend = new TLegend(0.1, 0.7, 0.48, 0.9);
        legend->AddEntry(histMbb,"Signal","l");
        legend->AddEntry(histMbbBkg,"Background","l");
        legend->Draw();
        cmbb->Update();
        
        if(higgsdecay == 0)
        {
            cmbb->SaveAs("Mbb_SigBkg_tau.pdf");
        }
        else
        {
            cmbb->SaveAs("Mbb_SigBkg.pdf");
        }
        
        TCanvas * cmbb_bkg = new TCanvas("cmbb_bkg", "cmbb_bkg", 600, 600);
        histMbbBkg->Draw();
        cmbb_bkg->Update();
        histMbb->Draw("Same");
        cmbb_bkg->Update();
        
        cmbb_bkg->SaveAs("Mbb_Bkg.pdf");
        
    }
    
    else if(argc == 3)                  // Here just one .root file and a parameter file. (i.e. no sig vs. bkg etc)
    {
        fstream fin(argv[2]);
        string line;
    
        while(getline(fin, line))
        {
            //the following line trims white space from the beginning of the string
            line.erase(line.begin(), find_if(line.begin(), line.end(), not1(ptr_fun<int, int>(isspace))));
        
            if(line[0] == '#') continue;
        
            istringstream is(line);
            is >> param >> value;
        
            params[param] = value;
        }
    
        CutsFunction(argv[1], params);
    
    }
    
    
    else if(argc == 2)                  // Uses default parameter file, for test purposes
    {
        fstream fin("$ROOTCUTS_DIR/example.rootcuts");
        string line;
        
        while(getline(fin, line))
        {
            //the following line trims white space from the beginning of the string
            line.erase(line.begin(), find_if(line.begin(), line.end(), not1(ptr_fun<int, int>(isspace))));
            
            if(line[0] == '#') continue;
            
            istringstream is(line);
            is >> param >> value;
            
            params[param] = value;
        }
        
        CutsFunction(argv[1], params);
    }
    
    else
    {
        std::cout << "Usage: ./ROOTCuts /path/to/root/file.root /path/to/parameters/file.something or see README for other modes." << endl;
    }

    return 0;
    
}
