To download it just do "git clone https://github.com/professor-calculus/ROOTCuts.git'' in whatever directory you want to install it. Or you can do "git clone https://github.com/professor-calculus/ROOTCuts.git 
/path/to/desired/directory/ROOTCuts’’ .

You'll need cmake, so do 'module load cmake' for Iridis to enable it (doesn't work on other clusters but Iridis does it this way) and you'll also need ROOT and Delphes installed.

Delphes requires ROOT so install ROOT first. ROOT is a pain in the arse to build, set aside hours for this… :/

Instructions are here:  https://github.com/professor-calculus/Cluster-Softwares/blob/master/INSTALL_IRIDIS.md   but the necessary process changes every now and then so I struggle to keep the instructions up to date. Might have to change a few paths is all. You can use ROOT v6 if you want — I usually do, it works fine, but ROOT v5.whatever 
as per instructions should be fine too.

Then to build it go to path/to/ROOTCuts/bin and do "cmake -DDELPHES=/path/to/delphes/install/location .. '' (the .. are important)
And then "cmake --build .''

You should now have an executable called rootcuts in that folder. For ease it’s wise to add a line in ~/.bash_profile containing “alias rootcuts = /path/to/rootcuts/install/bin/rootcuts’’ — I’m hoping to 
make a shell script to do this for the end user but haven’t got round to it yet…

Then to use the code you can do rootcuts /path/to/root/file /path/to/parameter/file

The parameter file contains columns of numbers. First column just has reference numbers for parameters, second column has the values you set them to, 
the third column (must start with # so code knows it’s a comment) has words describing it so you know what’s what. Don’t worry about things like cross-sections as they only matter when running over two root files, one 
signal and one background.

It doesn't matter too much what the parameters are because the output .root file will have histograms of everything without cuts applied. It also has booleans of each cut so you can plot whatever you like with however many 
cuts switched on or off. Opening the output file in ROOT you can do something like:

ROOTCuts->Draw("Uncut.MET","cut_N_bjets")

which will draw the MET distribution for all events that pass the number of b-jets requirement. Use TBrowser to take a look at the histograms and cuts in the ROOTCuts tree.

Take a look at example.rootcuts to see how the parameters are put in. :)
