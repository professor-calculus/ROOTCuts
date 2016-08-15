//
//  Tracking.h
//  
//
//  Created by Alex Titterton on 09/08/2013.
//
//


#include <TROOT.h>
#include <TTimeStamp.h>
#include <TLegend.h>
#include <TFile.h>
#include <TMath.h>
#include <TNtuple.h>
#include <TH2.h>
#include <TSystem.h>
#include <TInterpreter.h>
#include <cmath>
#include <TTree.h>
#include <TCanvas.h>
#include <TFrame.h>
#include <TChain.h>
#include <THStack.h>
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
#include "classes/DelphesClasses.h"
#include "ExRootAnalysis/ExRootTreeReader.h"
#include "ExRootAnalysis/ExRootTreeBranch.h"
#include "ExRootAnalysis/ExRootResult.h"
#include "ExRootAnalysis/ExRootUtilities.h"

using namespace std;

class TProfile;
class TH1D;
class TFile;
class TObjArray;
class TTree;


void CutsFunction(const char*, double []);
int CutsFunctionBkg(const char*, double [], string, TH1*, int);
vector<Jet *> JetPairFinder(vector<Jet *>, int);
vector<Jet *> JetDoublePairFinder(vector<Jet *>, int);
vector<Jet *> JetDoubleMbbPairFinder(vector<Jet *>, int);
vector<Jet *> Jet2Plus1Finder(vector<Jet *>, int);
vector<Jet *> Jet2Plus1MbbFinder(vector<Jet *>, int);



//--------The class which will store the data for each TrackID - then we can loop over each to do the tracking.
class Constituents
{
public:
    
    int ID;
    
    vector<double> Jet_PT;
    
    vector<vector<double> > y;

    
private:
    //int                      entries;
    //TFile*                   f;
};


class Jets
{
public:
    vector<double> DelR;
    vector<Jet*> j1;
    vector<Jet*> j2;
};

class DoubleJets
{
public:
    vector<double> DelR;
    vector<double> DelR2;
    vector<double> DelRtot;
    vector<Jet*> j1;
    vector<Jet*> j2;
    vector<Jet*> j3;
    vector<Jet*> j4;
};

class DoubleMbbJets
{
public:
    vector<double> DelMbb;
    vector<Jet*> j1;
    vector<Jet*> j2;
    vector<Jet*> j3;
    vector<Jet*> j4;
};
