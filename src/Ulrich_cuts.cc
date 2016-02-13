//------------------------------------------------------------
//--------------©Alexander Titterton 11/02/2016---------------
//---------------U. Bristol/U. Southampton/RAL----------------
//------------------------------------------------------------
//
//
//

#include <iostream>
#include "../include/Ulrich_cuts.hh"



using namespace std;


void CutsFunction(const string& filename)
{
	gSystem->Load("libTreePlayer");

	int i, j, m, n, p, q, k[4], l[4], entries;
    
    double eps = 0.05;
    
    int percent;
    
    string bar;
    
        
    
	string filename2,title;
	
    //---------Opening the .root file:
    
    
    TFile *f = TFile::Open(filename.c_str());

    //---------Aiming the pointers with the relevant TBranch and TLeaves
    //---------This is not particularly well-documented online,
    //---------Google is not your friend!
    TTree *EM = (TTree*)f->Get("Delphes");

    TBranch *Jet = (TBranch*)EM->GetBranch("Jet");
    TBranch *Event = (TBranch*)EM->GetBranch("Event");
    TBranch *MissingET = (TBranch*)EM->GetBranch("MissingET");
    TBranch *Particle = (TBranch*)EM->GetBranch("Particle");
    TBranch *Track = (TBranch*)EM->GetBranch("Track");
    
    TLeaf *Btag = Jet->GetLeaf("Jet.BTag");
    TLeaf *Tautag = Jet->GetLeaf("Jet.TauTag");
    TLeaf *JetMass = Jet->GetLeaf("Jet.Mass");
    TLeaf *JetEta = Jet->GetLeaf("Jet.Eta");
    TLeaf *JetPhi = Jet->GetLeaf("Jet.Phi");
    TLeaf *Evt = Branch->GetLeaf("Event");
    TLeaf *Tr = Branch->GetLeaf("TrackID");
    TLeaf *Na = Branch->GetLeaf("Name");

    
    
    
    //--------Tell it not to panic if there's no entries - it's better than a segfault!
    if (EM->GetEntries() < 1)
    {
        cout << "Problem" << endl;
    }
        
    entries = EM->GetEntries();

    cout << "Tree copied with " << entries << "entries\n\n\n" << endl;

    /*

    //--------Map in which to store all the particles by TrackID - more efficient than a vector
    //--------of objects since it doesn't require repeated looping over the whole shaboozle.
    
    map<int, Tracking*> map2;
    

    //The for-loop: Loops over the tree to put the elements of each row into the class
    // until we then put them into the seperate classes depending on Track ID
    //                  -- This is so it only loops over particles with the relevant
    //                      TrackID, not the entire Tree ~1600 times!
    
    for(i=0;i<entries;i++)
        
	{
		//----I hate Root for making bit this as illogical as possible. Also the only way
        //----I managed to do it was by following a method suggested by Rene Brun on a Root
        //----forum.
        //----      Rene Brun wrote Root.     Rene Brun is taunting me.
        
        Evt->GetBranch()->GetEntry(i);
        
        X->GetBranch()->GetEntry(i);
        Y->GetBranch()->GetEntry(i);
        Pl->GetBranch()->GetEntry(i);
        Plane = Pl->GetValue();
        //Na->GetBranch()->GetEntry(i);
        //cout << Na->GetValue() << endl;
        
        if((X->GetValue() < -(5+(2*Plane)) || X->GetValue() > (5+(2*Plane)) || Y->GetValue() < -(5+(2*Plane)) || Y->GetValue() > (5+(2*Plane)) ))
        {
        
            int current_id = Evt->GetValue();
        
            if(map2[current_id]==NULL)
            {
                map2[current_id] = new Tracking();
            }
                
            map2[current_id]->ID = current_id;
        
            map2[current_id]->x.resize (6);
            map2[current_id]->y.resize (6);
        
            
            map2[current_id]->Plane = Plane;
        
            map2[current_id]->x[Plane].push_back (X->GetValue());
            map2[current_id]->y[Plane].push_back (Y->GetValue());
		}

        
    }
 
    
    //--------The loop to analyse the tracks to see which fit a straight line
    //--------Straight line ---> MUON! (Probably)

    for(map<int,Tracking*>::iterator it = map2.begin(); it!=map2.end(); ++it)
    {
        
        for(int a=0;a<4;a++)
        {
            k[a] = it->second->x[a].size();
            //cout << "Tracker " << a << " tracked " << k[a] << " particles from incoming electron #" << it->first << endl; // lol
        }
        
        //----This bit gives a nice progress bar - unnecessary but looks so nice, like an iPad mini
        
        percent = ((it->first)*100)/tracks;
        
        for(int tintin = 0; tintin < 50; tintin++){
            if( tintin < (percent/2)){
                bar.replace(tintin,1,"=");
            }else if( tintin == (percent/2)){
                bar.replace(tintin,1,">");
            }else{
                bar.replace(tintin,1," ");
            }
        }
        
        cout<< "\r" "[" << bar << "] ";
        cout.width( 3 );
        cout<< percent << "%     " << std::flush;   // lol
        
        //--------The Loop:
        
        for (m=0;m<k[0];m++)
            {
                for (n=0;n<k[1];n++)
                {
                    for(p=0;p<k[2];p++)
                    {
                        for(q=0;q<k[3];q++)
                        {
                            dx0 = -(it->second->x[2][p]);
                            dx1 = (it->second->x[1][n])-(it->second->x[2][p]);
                            
                            dx2 = (it->second->x[0][m])-(it->second->x[1][n]);
                            
                            ratio1 = dx1/dx2;
                            ratio0 = dx0/dx2;
                            
                            if(ratio1 < 1+eps && ratio1 > 1-eps && ratio0 < 3+(3*eps) && ratio0 > 3-(3*eps))
                            {
                                dx3 = (it->second->x[2][p])-(it->second->x[3][q]);
                                ratio2 = dx2/dx3;
                                
                                if(ratio2 < 1+eps && ratio2 > 1-eps)
                                {
                                    
                                    dy1 = (it->second->x[1][n])-(it->second->x[2][p]);
                                    dy2 = (it->second->x[0][m])-(it->second->x[1][n]);
                                
                                    ratio3 = dy1/dy2;
                                
                                    if(ratio3 < 1+eps && ratio3 > 1-eps)
                                    {
                                        dy3 = (it->second->x[2][p])-(it->second->x[3][q]);
                                        ratio4 = dy2/dy3;
                                    
                                        if(ratio4 < 1+eps && ratio4 > 1-eps)
                                        {
                                            Linear.x[0] = it->second->x[0][m];
                                            Linear.x[1] = it->second->x[1][n];
                                            Linear.x[2] = it->second->x[2][p];
                                            Linear.x[3] = it->second->x[3][q];
                                     
                                            Linear.y[0] = it->second->y[0][m];
                                            Linear.y[1] = it->second->y[1][n];
                                            Linear.y[2] = it->second->y[2][p];
                                            Linear.y[3] = it->second->y[3][q];
                                     
                                            Linear.EventID = it->first;
                                            Linear.MuonNumber = LinearCount;
                                    
                                            LinearMuons.push_back (Linear);
                                    
                                            MuCount++;
                                            
                                        }
                                    }
                                }
                                
                            }
                        
                        //else cout << "No!" << endl;
                        }
                    }
                }
            }
        //--------If it finds one and only one straight line for a track, it's probably not a
        //--------muon, since where is its antimuon?! They have to come in pairs via symmetry
        //-------- and conservation of momentum!
        if(MuCount > 1)
        {
            LinearCount++;
            //cout << "Muon Pair!" << endl;
            MuTestCount += MuCount;
        }
        MuCount = 0;
    }

    cout << endl;
    
    cout << LinearCount << " muon pairs detected" << endl;
    cout << MuTestCount << " muons possibly detected..." << endl;
    
*/

f->Close();   

};

