//
//  ROOTCuts.cc
//  
//
//  Created by Alex Titterton on or around 01/03/2016.
//
//

#include "include/Ulrich_cuts.hh"
#include <stdio.h>
#include <iostream>
#include <fstream>


int main(int argc, char *argv[])
{

    cout << "\n";
    
    gSystem->Load("libTreePlayer");
    //gSystem->Load("/home/ast1g15/delphes/libDelphes.so");
    //gSystem->Load("libExRootAnalysis.so");
    

    //              Parameters:
    //      0       1st Leading jet min. PT
    //      1       2nd Leading jet min. PT
    //      2       3rd Leading jet min. PT
    //      3       4th Leading jet min. PT
    //      4       0 = h->bb h->tau-tau; 1 = 4 b-tags; 2 = 3 b-jets, 3: EXACTLY 2 b-jets
    //      5       b-jet lower bound PT
    //      6       MET lower bound
    //      7       min. taus inv. mass     }
    //      8       max. taus inv. mass     }   Only for h->tau tau in one of the cascades
    //      9       min. Sum taus' PT       }
    //      10      min. M_bb
    //      11      max. M_bb
    //      12      Jet pair matching algorithm for 2 bb pairs: 0 = Smallest av. Delta-R; 1 = Pairs with closest M_inv(bb)
    //      13      Pythia Version "6" OR "8" -- for getting the cross-section (Only matters in FOLDER mode!)
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
    //      24      Max. MHT (-1 means infinity)
    //      25      min. alpha_T

    
    double params[26];
    
    
    int param;
    double value;
    params[21] = 0;
    params[22] = 0;
    params[23] = 0;
    params[24] = -1;
    params[25] = 0.;

    
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
        string rootpath;
        if(params[13] == 8)
        {
            rootpath = inpstring + "/Events/run_01/Py8/delphes_py8.root";
        }
        else
        {
            rootpath = inpstring + "/Events/run_01/tag_1_delphes_events.root";
        }
        string crosssectionpath = inpstring + "/Events/run_01/tag_1_pythia.log";
        
        ifstream delphesfile;
        delphesfile.open(rootpath);
        
        double crosssectionvalue = 0;
        
        if(!delphesfile)
        {
            cout << "What folder? I don't know anything about it..." << endl;
            cout << "\n There's likely a typo in the folder path somewhere, or .root file doesn't exist!" << endl;
            return 1;
        }
        delphesfile.close();
        
        if(params[13] == 8)
        {
            TFile delphesinput(rootpath.c_str());
            TVectorD *v = (TVectorD*)delphesinput.Get("crosssection");
            
            crosssectionvalue = v->Sum();
        }
        else
        {
            ifstream inputFile;
            inputFile.open(crosssectionpath);
            
            if(!inputFile)
            {
                cout << "What folder? I don't know anything about it..." << endl;
                cout << "\n Pythia6 log file doesn't exist!" << endl;
                return 1;
            }
            
            string crosssection;
            
            while(!inputFile.eof())
            {
                inputFile >> crosssection;      // Yolo-swagged to redefine crosssection as each line until it gets to the last line
                                                                                // of tag_1_pythia.log, which contains the cross-section value after jet matching
            }
            
            size_t pos = crosssection.find(":");
            crosssection.erase(0,pos + 1);
            
            crosssectionvalue = std::stod(crosssection);        // converts string to double so we can use the cross-sec
        }
        
        params[20] = crosssectionvalue;
        
        CutsFunction(rootpath.c_str(), params);
        
    }
    
    
    if(argc == 6 && string(argv[5]) == "YIELD")         // Here we input a MG output folder and a param card, with the
                                                        // options M_sq, M_LSP and "FOLDER". (no sig vs. bkg etc)
    {
        cout <<  "\n YIELD mode!\n\n" << endl;
        
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
        string rootpath;
        if(params[13] == 8)
        {
            rootpath = inpstring + "/Events/run_01/Py8/delphes_py8.root";
        }
        else
        {
            rootpath = inpstring + "/Events/run_01/tag_1_delphes_events.root";
        }
        string crosssectionpath = inpstring + "/Events/run_01/tag_1_pythia.log";
        
        ifstream delphesfile;
        delphesfile.open(rootpath);
        
        double crosssectionvalue = 0;
        
        if(!delphesfile)
        {
            cout << "What folder? I don't know anything about it..." << endl;
            cout << "\n There's likely a typo in the folder path somewhere, or .root file doesn't exist!" << endl;
            return 1;
        }
        delphesfile.close();
        
        if(params[13] == 8)
        {
            TFile delphesinput(rootpath.c_str());
            TVectorD *v = (TVectorD*)delphesinput.Get("crosssection");
            
            crosssectionvalue = v->Sum();
        }
        else
        {
            ifstream inputFile;
            inputFile.open(crosssectionpath);
            
            if(!inputFile)
            {
                cout << "What folder? I don't know anything about it..." << endl;
                cout << "\n Pythia6 log file doesn't exist!" << endl;
                return 1;
            }
            
            string crosssection;
            
            while(!inputFile.eof())
            {
                inputFile >> crosssection;      // Yolo-swagged to redefine crosssection as each line until it gets to the last line
                // of tag_1_pythia.log, which contains the cross-section value after jet matching
            }
            
            size_t pos = crosssection.find(":");
            crosssection.erase(0,pos + 1);
            
            crosssectionvalue = std::stod(crosssection);        // converts string to double so we can use the cross-sec
            
            inputFile.close();
        }
        
        params[20] = crosssectionvalue;
        
        
        Yield yields = YieldGetter(rootpath.c_str(), params);
        
        if(yields.yield < 0)
        {
            cout << "There's an error somewhere!" << endl;
            return 1;
        }
        
        
        ofstream outputfile;
        
        string outputfilename = "yields.txt";
        
        outputfile.open(outputfilename, std::ios_base::app);
        
        outputfile << yields.M_Sq << "\t" << yields.M_LSP << "\t" << yields.yield << endl;
        
        outputfile.close();
        
    }
    
    else if(argc == 4 && string(argv[3]) == "FOLDER")             // Here we input a MG output folder and a param card, with the
                                                                  // option "folder", the code calculates the masses.
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
        string rootpath;
        if(params[13] == 8)
        {
            rootpath = inpstring + "/Events/run_01/Py8/delphes_py8.root";
        }
        else
        {
            rootpath = inpstring + "/Events/run_01/tag_1_delphes_events.root";
        }
        string crosssectionpath = inpstring + "/Events/run_01/tag_1_pythia.log";
        
        ifstream delphesfile;
        delphesfile.open(rootpath);
        
        double crosssectionvalue = 0;
        
        if(!delphesfile)
        {
            cout << "What folder? I don't know anything about it..." << endl;
            cout << "\n There's likely a typo in the folder path somewhere, or .root file doesn't exist!" << endl;
            return 1;
        }
        delphesfile.close();
        
        if(params[13] == 8)
        {
            TFile delphesinput(rootpath.c_str());
            TVectorD *v = (TVectorD*)delphesinput.Get("crosssection");
            
            crosssectionvalue = v->Sum();
        }
        else
        {
            ifstream inputFile;
            inputFile.open(crosssectionpath);
            
            if(!inputFile)
            {
                cout << "What folder? I don't know anything about it..." << endl;
                cout << "\n Pythia6 log file doesn't exist!" << endl;
                return 1;
            }
            
            string crosssection;
            
            while(!inputFile.eof())
            {
                inputFile >> crosssection;      // Yolo-swagged to redefine crosssection as each line until it gets to the last line
                // of tag_1_pythia.log, which contains the cross-section value after jet matching
            }
            
            size_t pos = crosssection.find(":");
            crosssection.erase(0,pos + 1);
            
            crosssectionvalue = std::stod(crosssection);        // converts string to double so we can use the cross-sec
            
            inputFile.close();
        }
        
        params[20] = crosssectionvalue;
        
        CutsFunction(rootpath.c_str(), params);
        
    }
    
            
    else if(argc == 3)                  // Here just one .root file and a parameter file. Masses ignored since they don't matter.
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
    
    else
    {
        std::cout << "Usage:" << endl;
        std::cout << "\"ROOTCuts /path/to/rootfile /path/to/params.rootcuts <Sqmass> <LSPmass> FOLDER\" for running over MG output folder." << endl;
        std::cout << "\"ROOTCuts /path/to/rootfile /path/to/params.rootcuts <Sqmass> <LSPmass> YIELD\" for running over MG output folder quicker, no plots, just gives yield." << endl;
        std::cout << "In either case not specifying the masses will just let the program calculate them, but this is slower.\n" << endl;
        std::cout << "Standard usage for a simple .root file (just to test efficiency, not yield etc) is more like:" << endl;
        std::cout << "\"ROOTCuts /path/to/rootfile /path/to/params.rootcuts\" for one .root file. Main use." << endl;
        
        return 1;
    }

    return 0;
    
}
