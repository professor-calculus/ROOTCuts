//
//  Tracking.h
//  
//
//  Created by Alex Titterton on 09/08/2013.
//
//


#include <TROOT.h>
#include <TFile.h>
#include <TNtuple.h>
#include <TH2.h>
#include <TSystem.h>
#include <TInterpreter.h>
#include <cmath>
#include <TTree.h>
#include <TCanvas.h>
#include <TFrame.h>
#include <TChain.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <TString.h>
#include <TH1D.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <TApplication.h>
#include <TClonesArray.h>

using namespace std;

class TProfile;
class TH1D;
class TFile;
class TObjArray;
class TTree;

void CutsFunction(const char*);


//--------The class which will store the data for each TrackID - then we can loop over each to do the tracking.
class Constituents
{
public:
    
    int ID;
    
    vector<double> Jet_PT;
    
    vector<vector<double> > y;

    
private:
    TTree*					 tree;
    TTree*					 tracker;
    //string                 filename;
    int                      maxtrack;
    //int                      entries;
    //TFile*                   f;
};


class LinearTracks
{
public:
    double x[4];
    double y[4];
    int EventID;
    int MuonNumber;
};
