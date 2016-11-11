# Welcome to FAMOUS

Geant4 (https://github.com/Geant4/geant4) detector simulation of the SiPM fluorescence telescope FAMOUS

Further information can be found here: 
 * http://dx.doi.org/10.1016/j.nima.2015.01.067
 * http://dx.doi.org/10.1117/12.925305
 * http://dx.doi.org/10.1117/12.929229
 * https://arxiv.org/abs/1502.00792
 * http://inspirehep.net/record/1483867
 
Requirements
 * Geant4 (4.10 or newer, https://github.com/Geant4/geant4)
 * Boost (1.50.0 or newer, http://www.boost.org/)
 * ROOT (5.34 or newer, https://root.cern.ch)
 
The build process has been tested with Ubuntu 15.04 and Scientific Linux 6.

## Getting started

Clone the code

    git clone https://github.com/ntim/famous.git source
    cd source

Initialize submodule G4SiPM
    
    git submodule init
    git submodule update
    
Initialize the submodules of G4SiPM

    cd externals/g4sipm
    git submodule init
    git submodule update
    cd ../../
    
Configure the build with CMake

    cd ../
    mkdir build
    cd build
    cmake ../source
    
Finally, start the build process

    make
