//
//  FileExists.cpp
//  ROOTCuts
//
//  Created by Alexander Titterton on 01/12/2016.
//
//

#include <stdio.h>
#include "../include/Ulrich_cuts.hh"

bool fexists(const char *filename)
{
    ifstream ifile(filename);
    return !!ifile;
};
