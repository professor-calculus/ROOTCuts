echo 'Using ROOT installed in:'
echo $ROOTSYS
echo 'Using user-specified Delphes location'
args=("$@")
DELPHES_LOC=${args[0]}
cmake -DDELPHES=$DELPHES_LOC ..
cmake --build .
source env.sh
