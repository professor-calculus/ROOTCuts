echo 'Using ROOT installed in:'
echo $ROOTSYS
echo 'Installing latest version of Delphes'
cd ..
git clone https://github.com/delphes/delphes.git
cd delphes
sed -i '/save/d; /loadFromFile/d' display/Delphes3DGeometry.h
make
cd ../bin
echo 'Delphes installed, now installing ROOTCuts :)'
cmake -DDELPHES=$PWD/../delphes ..
cmake --build .
source env.sh
echo 'All done!'
