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

    cout << "\n";
    
    gSystem->Load("libTreePlayer");
    //gSystem->Load("/home/ast1g15/delphes/libDelphes.so");
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
    //      17      minimum Missing HT
    //      18      Lumi mode: 0 = off. 1 = on, return numbers of events at lumi before/after cuts. 2 = on, also scale histos.
    //      19      Luminosity
    //      20      Cross-section: Will be overridden if in FOLDER mode!
    //      21      Scan Mode (automatically set, does not matter)
    //      22      Mass of Squark (doesn't matter for non-scan mode)
    //      23      Mass of LSP     (doesn't matter for non-scan mode)

    
    double params[25];
    
    
    int param, bkg, signal;
    double value;
    params[21] = 0;
    params[22] = 0;
    params[23] = 0;
		params[24] = -1;


    
    if(argc == 6 && string(argv[5]) == "FOLDER") // Here we input a MG output folder and a param card, with the
                                                // options M_sq, M_LSP and "FOLDER". (no sig vs. bkg etc)
    {
        cout <<  "\n FOLDER mode!\n\n" << endl;
        
        fstream fin(argv[2]);
        string line, line2;
        
        while(getline(fin, line))
        {
            //the following line trims white space from the beginning of the string
            line.erase(line.begin(), find_if(line.begin(), line.end(), not1(ptr_fun<int, int>(isspace))));
            
            cout << line << endl;
            
            if(line[0] == '#') continue;
            
            istringstream is(line);
            is >> param >> value;
            
            params[param] = value;
        }
        
        params[21] = 1;
        
        params[22] = std::stod(argv[3]);
        params[23] = std::stod(argv[4]);
        
        string inpstring(argv[1]);
        string rootpath = inpstring + "/Events/run_01/tag_1_delphes_events.root";
        string crosssectionpath = inpstring + "/Events/run_01/tag_1_pythia.log";
        
        ifstream inputFile;
        inputFile.open(crosssectionpath);
        
        if(!inputFile)
        {
            cout << "What folder? I don't know anything about it..." << endl;
            cout << "\n There's likely a typo in the folder path somewhere!" << endl;
            return 1;
        }
        
        string crosssection;
        double crosssectionvalue = 0;
        
        while(!inputFile.eof())
        {
            inputFile >> crosssection;      // Yolo-swagged to redefine crosssection as each line until it gets to the last line
            																// of tag_1_pythia.log, which contains the cross-section value after jet matching
        }
        
        size_t pos = crosssection.find(":");
        crosssection.erase(0,pos + 1);
        
        crosssectionvalue = std::stod(crosssection);        // converts string to double so we can use the cross-sec
        
        params[20] = crosssectionvalue;
        
        CutsFunction(rootpath.c_str(), params);
        
    }
    
    else if(argc == 4 && string(argv[3]) == "FOLDER") // Here we input a MG output folder and a param card, with the
                                                                  // option "folder". (no sig vs. bkg etc)
    {
        cout <<  "FOLDER mode!" << endl;
        
        fstream fin(argv[2]);
        string line, line2;
        
        while(getline(fin, line))
        {
            //the following line trims white space from the beginning of the string
            line.erase(line.begin(), find_if(line.begin(), line.end(), not1(ptr_fun<int, int>(isspace))));
            
            cout << line << endl;
            
            if(line[0] == '#') continue;
            
            istringstream is(line);
            is >> param >> value;
            
            params[param] = value;
        }
        
        params[21] = 1;
        params[22] = -1;
        params[23] = -1;
        
        string inpstring(argv[1]);
        string rootpath = inpstring + "/Events/run_01/tag_1_delphes_events.root";
        string crosssectionpath = inpstring + "/Events/run_01/tag_1_pythia.log";
        
        ifstream inputFile;
        inputFile.open(crosssectionpath);
        
        string crosssection;
        double crosssectionvalue = 0;
        
        //fstream fin2(crosssectionpath);
        while(!inputFile.eof())
        {
            inputFile >> crosssection;      // Yolo-swagged to redefine crosssection as each line until it gets to the last line
                                            // of tag_1_pythia.log, which contains the cross-section value after jet matching
        }
        
        size_t pos = crosssection.find(":");
        crosssection.erase(0,pos + 1);
        
        crosssectionvalue = std::stod(crosssection);        // converts string to double so we can use the cross-sec
        
        params[20] = crosssectionvalue;
        
        CutsFunction(rootpath.c_str(), params);
        
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
    
    else if(argc == 3 && (string(argv[2]) == "EFF" || string(argv[2]) == "PLOTTING"))     // Plotting tool for efficiencies
    {
        cout << "\n Plotting Mode\n" << endl;
        Plotting(argv[1]);
        
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
        
        //Christmas();
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
        std::cout << "Usage:" << endl;
        std::cout << "\"ROOTCuts /path/to/rootfile /path/to/params.rootcuts <Sqmass> <LSPmass> FOLDER\" for running over MG output folder." << endl;
        std::cout << "\"ROOTCuts /path/to/rootfile EFF (or PLOTTING)\" for plotting the output from a scan" << endl;
        std::cout << "\"ROOTCuts /path/to/rootfile /path/to/params.rootcuts\" for one .root file. Main use." << endl;
    }

    return 0;
    
}
