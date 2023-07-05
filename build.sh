#!/bin/bash

declare -r DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
declare silence=$1

#####################################################################
#                                                                   #
# Script to build the Caps Library.                                 #
#                                                                   #
#   Running instructions:                                           #
#       chmod +x build.sh                                           #
#       ./build.sh                                                  #
#                                                                   #
#   Option: Avoid getting the full output of the internal commands: #
#       ./build.sh quiet                                            #
#                                                                   # 
#   Once ran, you can run the executables by:                       #
#       bin/cannon                                                  #
#####################################################################

quiet(){
    if [ "$silence" == "quiet" ]; then
        "$@" > /dev/null
    else
        "$@"
    fi
}


echo "Building the files..."
quiet rm -R build 2> /dev/null
quiet mkdir build
quiet cd build 

echo "Running CMake..."
quiet cmake .. -DCMAKE_INSTALL_PREFIX=.. -DCMAKE_BUILD_TYPE=Debug


echo "Running Make..."
quiet make
quiet make install

echo "Finished!"
