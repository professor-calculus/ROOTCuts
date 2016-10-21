#!/bin/bash
echo 'Installing latest version of Delphes'
cd ..
git clone https://github.com/delphes/delphes.git
cd delphes
make
cd ../bin
echo 'Delphes installed, now installing ROOTCuts :)'
cmake -DDELPHES=../delphes ..
cmake --build .
