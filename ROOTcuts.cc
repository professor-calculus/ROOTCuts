//
//  Tracking_Main.cc
//  
//
//  Created by Alex Titterton on 14/08/2013.
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
    //      4       0 = h->bb h->tau-tau; 1 = h->bb both cascades
    //      5       b-jet lower bound PT
    //      6       MET lower bound
    //      7       min. taus inv. mass
    //      8       max. taus inv. mass
    //      9       min. Sum taus' PT
    //      10      min. M_bb
    //      11      max. M_bb

    
    double params[12];
    int param;
    double value;
    
    
    if(argc == 4)
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
        if(higgsdecay < 0 || higgsdecay > 1)
        {
            cout << "ERROR: Higgs Decay mode must be 0 or 1" << endl;
            return 0;
        }
        
        //TH1 *histMbb;
        //TH1 *histMbbBkg;
        
        vector<TH1*> histos;
        
        histos.push_back(CutsFunctionBkg(argv[1], params, "Signal"));
        histos.push_back(CutsFunctionBkg(argv[2], params, "Background"));
        
        TCanvas * cmbb = new TCanvas("cmbb", "cmbb", 600, 600);
        
        
        histos[0]->Draw();
        histos[0]->SetLineColor(kBlue);
        cmbb->Update();
        histos[1]->SetLineColor(kRed);
        histos[1]->Draw("Same");
        cmbb->Update();
        
        TLegend *legend = new TLegend(0.1, 0.7, 0.48, 0.9);
        legend->AddEntry(histos[0],"Signal","l");
        legend->AddEntry(histos[1],"Background","l");
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
        
        
    }
    
    else if(argc == 3)
    {
        int option;
        
        cout << "Select input file types:" << endl;
        cout << "rootfile.root parameters.txt ------ Type 1\n" << "rootfile.root rootfile.root ------ Type 2" << endl;
        
        cin >> option;
        
        if(option == 1)
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
        
        else
        {
            fstream fin("../default/default_parameters.txt");
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
            
            CutsFunctionBkg(argv[1], params, "Signal");
            CutsFunctionBkg(argv[2], params, "Background");
        }
    }
    
    else if(argc == 2)
    {
        fstream fin("../default/default_parameters.txt");
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
        std::cout << "Usage: ./ROOTCuts /path/to/root/file.root /path/to/parameters/file.something" << endl;
    }

    return 0;
    
}