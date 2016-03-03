//
//  TerminalPlot.cpp
//  ROOTCuts
//
//  Created by Alexander Titterton on 02/03/2016.
//
//

#include "../include/Ulrich_cuts.hh"
#include "../include/TerminalPlot.hh"

using namespace std;

void TerminalPlot(TH1* hist, string title, int height, double xmin, double xmax)
{
    int tempbin, bins, yvalue;
    double xvalue;
    
    bins = hist->GetNbinsX();
    
    int scaledhist[bins];
    
    int maxbin = hist->GetMaximum();
    
    for(int i=0; i<bins; i++)
    {
        tempbin = hist->GetBinContent(i);
        scaledhist[i] = tempbin*height/maxbin;
    }
    
    
    for(int j = 0; j< height; j++)
    {
        yvalue = maxbin*(height - j)/height;
        if(int(yvalue) == double(maxbin)*(double(height) - double(j))/double(height))
        {
            cout << setw(2) << left << yvalue;
        }
        else
        {
            cout << setw(2) << left << "  ";
        }
        
        cout << left << "\033[36m" << "| " << "\033[0m";
        
        
        for(int k=0; k<bins; k++)
        {
            if(scaledhist[k] > height-j+1)
            {
                cout << setw(4) << left << "\033[31m" << "****" "\033[0m";
            }
            else
            {
                cout << setw(4) << left << "    ";
            }
        }
        
        if(j<height-1)
        {
            cout << "\n";
        }
    }
    
    cout << "\n" << setw(2) << left << 0 << "\033[36m" << "|-" << "\033[0m";
    for(int l=0; l<bins; l++)
    {
        cout << "\033[36m" << setw(4) << "----" << "\033[0m";
    }
    
    cout << "\n" << setw(2) << left << "  ";
    
    for(int m=0; m<bins/2; m++)
    {
        xvalue = xmin + (xmax - xmin)*2.0*double(m)/double(bins);
        cout << setw(8) << left << xvalue;
    }
    
    
    cout << setw(8) << left << xmax << endl;
    
    for(int n=0; n<bins/2; n++)
    {
        cout << setw(4) << left << "    ";
    }
    
    cout << "\t" << title << "\n\n" << endl;
};