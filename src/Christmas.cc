//
//  Christmas.cpp
//  ROOTCuts
//
//  Created by Alexander Titterton on 08/12/2016.
//
//

#include "include/Ulrich_cuts.hh"
#include <sys/ioctl.h>
#include <unistd.h>

void Christmas()
{
    int cols = 80;
    int lines = 48;
    
#ifdef TIOCGSIZE
    struct ttysize ts;
    ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
    cols = ts.ts_cols;
#elif defined(TIOCGWINSZ)
    struct winsize ts;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
    cols = ts.ws_col;
    //lines = ts.ws_row;
#endif /* TIOCGSIZE */
    
    string bar;
    string temp;
    string temp2[18];
    
    string snow[2];
    snow[0] = " ";
    snow[1] = "*";
    int random;
    
    cout << "\n\n \e[1;31m *****  MERRY CHRISTMAS! ***** \e[0m \n\n" << endl;
    
    bar = "*   * *  *   * *   *   *      *   * *   * ";
    cout << bar << "\n" << endl;
    
    for(int haddock = 1; haddock < lines-18; haddock++)
    {
        temp.clear();
        
        for(int b=0; b<cols/2; b++)
        {
            random = rand() % 2;
            temp += snow[random];
            temp += " ";
        }
        bar = temp;
        if(haddock<19)
        {
            temp.erase(1,20);
            temp2[haddock-1] = temp;
        }
        usleep(250000);
        
        cout << bar << "\n" << endl;
    }
    
    usleep(250000);
    
    cout << "        _" << temp2[0] << endl;
    cout << "       {_}" << temp2[1] << endl;
    cout << "\e[91m       / \\" << temp2[2] << endl;
    cout << "      /   \\" << temp2[3] << endl;
    cout << "     /_____\\\e[0m" << temp2[4] << endl;
    cout << "   {`_______`}" << temp2[5] << endl;
    cout << "    // . . \\\\" << temp2[6] << endl;
    cout << "   (/(__7__)\\)" << temp2[7] << endl;
    cout << "   |'-' = `-'|" << temp2[8] << endl;
    cout << "   |         |" << temp2[9] << endl;
    cout << "\e[91m   /\e[0m\\       /\e[91m\\" << temp2[10] << endl;
    cout << "  /  \e[0m'.   .'\e[91m  \\" << temp2[11] << endl;
    cout << " /_/   \e[0m`\"`\e[91m   \\_\\" << temp2[12] << endl;
    cout << "\e[0m{__}###[_]###{__}" << temp2[13] << endl;
    cout << "(_/\e[91m\\_________/\e[0m\\_)" << temp2[14] << endl;
    cout << "\e[91m    |___|___|" << temp2[15] << endl;
    cout << "     |--|--|" << temp2[16] << endl;
    cout << "\e[0m    (__)`(__)" << temp2[17] << endl;
    
};
