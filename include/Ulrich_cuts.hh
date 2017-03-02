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
#include <TObject.h>
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
#include <TVectorD.h>
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

vector<Jet *> JetPairFinder(vector<Jet *>, int);
vector<Jet *> JetDoublePairFinder(vector<Jet *>, int);
vector<Jet *> JetDoubleMbbPairFinder(vector<Jet *>, int);
vector<Jet *> Jet2Plus1Finder(vector<Jet *>, int);
vector<Jet *> Jet2Plus1MbbFinder(vector<Jet *>, int);
double alpha_T(vector<Jet *>, double);
void Christmas();


typedef struct {double M_bb, MET, DeltaR, hardDeltaR, NLSPDeltaR, LSPDeltaR, NLSP_PT1, NLSP_PT2, LSP_PT1, LSP_PT2, b_PT1, b_PT2, b_PT3, b_PT4, biaseddeltaphi, alpha_T, HT, MHT; int n_bjets, n_jets, Msq, Mlsp; bool cut_Mbb, cut_DeltaR, cut_biaseddeltaphi, cut_MET, cut_HT, cut_N_bjets, cut_N_jets, cut_MHT;} UNCUT;

typedef struct {double crosssec, eff, HTeff, METeff, MHTeff, Njeff, Nbeff, Mbbeff, BDPeff, doubleeventpass, Msq, Mlsp, HT, MET, MHT, Nj, Nb, Mbb, BDP;} Efficiencies;

class Yield
{
public:
    double yield;
    int M_LSP;
    int M_Sq;
};

Yield YieldGetter(const char*, double[]);


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
