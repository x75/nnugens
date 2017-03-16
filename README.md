
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

Then 

    cd nnugens
    mkdir build
    cd build
    cmake ..
    make
    cp NNUgens.so ../EvoPlast/EvoPlast.sc ~/.local/share/SuperCollider/Extensions/NNUgens/
