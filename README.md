
# neural network ugens

oswald berthold 2017

very preliminary stage

 - basic SO2 network with hardcoded parameter matrix
 - evoplast oscillators
 - x reservoirs
 - x RLS / FORCE / EH readouts

## install

Requires armadillo c++ linear algebra library

    sudo apt-get install libarmadillo-dev

### Mac specifics:
Install armadillo via homebrew:
    brew install homebrew/science/armadillo


Then 

    cd nnugens
    mkdir build
    cd build
    SC_PATH=<path_to_supercollider_sources> cmake ..
    make

    # linux:
    cp NNUgens.so ../EvoPlast/EvoPlast.sc ~/.local/share/SuperCollider/Extensions/NNUgens/

    # mac:
    mkdir -p ls ~/Library/Application\ Support/SuperCollider/Extensions/NNUgens/Classes
    cp NNUgens.scx ~/Library/Application\ Support/SuperCollider/Extensions/NNUgens/
    cp ../EvoPlast/*.sc* ~/Library/Application\ Support/SuperCollider/Extensions/NNUgens/Classes


