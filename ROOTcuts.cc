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
    //      4       0 = h->bb h->tau-tau; 1 = h->bb both cascades; 2 = 3 b-jets
    //      5       b-jet lower bound PT
    //      6       MET lower bound
    //      7       min. taus inv. mass
    //      8       max. taus inv. mass
    //      9       min. Sum taus' PT
    //      10      min. M_bb
    //      11      max. M_bb
    //      12      Jet pair matching algorithm for 2 bb pairs: 0 = Smallest av. Delta-R; 1 = Pairs with closest M_inv(bb)
	//		13		User specify Signal/Background ratio (double precision)

    
    double params[14];
    int param, bkg, signal;
    double value;

	double weight = params[13];
    
    
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
        if(higgsdecay < 0 || higgsdecay > 2)
        {
            cout << "ERROR: Higgs Decay mode must be 0, 1 or 2" << endl;
            return 0;
        }
        
        TH1 *histMbb = new TH1F("mbb", "M_{inv}(b, b) (h->bb in both cascades); M_{inv}(b, b) (GeV)", 20, params[10], params[11]);
        TH1 *histMbbBkg = new TH1F("mbb_bkg", "", 20, params[10], params[11]);
        
        bkg = CutsFunctionBkg(argv[2], params, "Background", histMbbBkg, 0); //Background
		signal = CutsFunctionBkg(argv[1], params, "Signal", histMbb, bkg);   //Signal
        
        TCanvas * cmbb = new TCanvas("cmbb", "cmbb", 600, 600);
        
        THStack hs("hs","test stacked histograms");
        
        hs.Add(histMbb);
        histMbb->SetLineColor(kBlue);
        
        hs.Add(histMbbBkg);
        histMbbBkg->SetLineColor(kRed);
        
        hs.Draw("nostack");
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
            
            int higgsdecay = int(params[4]);
            if(higgsdecay < 0 || higgsdecay > 1)
            {
                cout << "ERROR: Higgs Decay mode must be 0 or 1" << endl;
                return 0;
            }
            
            TH1 *histMbb = new TH1F("mbb", "M_{inv}(b, b) (h->bb in both cascades); M_{inv}(b, b) (GeV)", 20, params[10], params[11]);
            TH1 *histMbbBkg = new TH1F("mbb_bkg", "", 20, params[10], params[11]);

			bkg = CutsFunctionBkg(argv[2], params, "Background", histMbbBkg, 0); //Bkg            
            signal = CutsFunctionBkg(argv[1], params, "Signal", histMbb, bkg);  //Signal
            
            TCanvas * cmbb = new TCanvas("cmbb", "cmbb", 600, 600);
            
            
            histMbb->Draw();
            histMbb->SetLineColor(kBlue);
            cmbb->Update();
            histMbbBkg->SetLineColor(kRed);
            histMbbBkg->Draw("Same");
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
            
            cmbb_bkg->SaveAs("Mbb_Bkg.pdf");
            
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