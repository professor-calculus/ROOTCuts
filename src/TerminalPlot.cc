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
    int binscale;
    double xvalue;
    
    bins = hist->GetNbinsX();
    
    int scaledhist[bins];
    
    int maxbin = hist->GetMaximum();
    
    for(int i=0; i<bins; i++)
    {
        tempbin = hist->GetBinContent(i+1);
        if(maxbin != 0)
        {
            scaledhist[i] = tempbin*height/maxbin;
        }
        else return;
    }
    
    if(bins>0 && bins<25)
    {
        binscale = 4;
    }
    else if(bins>24 && bins<33)
    {
        binscale = 3;
    }
    else if(bins>32 && bins<49)
    {
        binscale = 2;
    }
    else
    {
        binscale = 1;
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
        
        cout << setw(1) << left << "\033[36m" << "|" << "\033[0m";
        
        
        for(int k=0; k<bins; k++)
        {
            if(scaledhist[k] > height-j+1)
            {
                if(binscale == 4)
                {
                    cout << setw(4) << left << "\033[31m" << "****" << "\033[0m";
                }
                else if(binscale == 3)
                {
                    cout << setw(3) << left << "\033[31m" << "***" << "\033[0m";
                }
                else if(binscale == 2)
                {
                    cout << setw(2) << left << "\033[31m" << "**" << "\033[0m";
                }
                else
                {
                    cout << setw(1) << left << "\033[31m" << "*" << "\033[0m";
                }
            }
            else
            {
                if(binscale == 4)
                {
                    cout << setw(4) << left << "\033[31m" << "    " << "\033[0m";
                }
                else if(binscale == 3)
                {
                    cout << setw(3) << left << "\033[31m" << "   " << "\033[0m";
                }
                else if(binscale == 2)
                {
                    cout << setw(2) << left << "\033[31m" << "  " << "\033[0m";
                }
                else
                {
                    cout << setw(1) << left << "\033[31m" << " " << "\033[0m";
                }
            }
        }
        
        
        if(j<height-1)
        {
            cout << "\n";
        }
        
    }
    
    
    cout << "\n" << setw(2) << left << 0;
    cout << setw(1) << "\033[36m" << "|" << "\033[0m";
    for(int l=0; l<bins; l++)
    {
        if(binscale == 4)
        {
            cout << "\033[36m" << setw(4) << "----" << "\033[0m";
        }
        else if(binscale == 3)
        {
            cout << "\033[36m" << setw(3) << "---" << "\033[0m";
        }
        else if(binscale == 2)
        {
            cout << "\033[36m" << setw(2) << "--" << "\033[0m";
        }
        else
        {
            cout << "\033[36m" << setw(1) << "-" << "\033[0m";
        }
    }
    
    cout << "\n" << setw(3) << left << "   ";
    
    for(int m=0; m<bins*binscale/8; m++)
    {
        xvalue = xmin + (xmax - xmin)*8.0*double(m)/(double(bins)*double(binscale));
        cout << setw(8) << left << xvalue;
    }
    
    
    cout << setw(8) << left << xmax << endl;
    
    for(int n=0; n<bins*binscale/8 - 1; n++)
    {
        cout << setw(4) << left << "    ";
    }
    
    cout << "\t" << title << "\n\n" << endl;
    
};