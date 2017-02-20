//
//  ROOTEffs.hpp
//  ROOTCuts
//
//  Created by Alex Titterton on 15/02/2017.
//
//

#ifndef ROOTEffs_hpp
#define ROOTEffs_hpp

#include <stdio.h>

#endif /* ROOTEffs_hpp */


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


void Plotting(const char*);
void Yields_Adder(vector<string>, int);


typedef struct {double crosssec, eff, HTeff, METeff, MHTeff, Njeff, Nbeff, Mbbeff, BDPeff, doubleeventpass, Msq, Mlsp, HT, MET, MHT, Nj, Nb, Mbb, BDP;} Efficiencies;


//--------The class which will store the data for each TrackID - then we can loop over each to do the tracking.
