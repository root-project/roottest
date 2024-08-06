# Recipe to debug test and generate new references files
Reference JSON's are used to generate new SVG's. It is important to debug always both test and in case of a need of new references files to generate new ones for both
## 1. Setting up
Before building, create a myTesting directory and clone root and roottest. After that create a build directory, change into it, enable the 'testing' and 'roottest' option:

    mkdir myTesting
    cd myTesting
    git clone https://github.com/root-project/root.git
    git clone https://github.com/root-project/roottest.git
    mkdir build
    cd build
    cmake -Dtesting=ON -Droottest=ON ../root
    cmake --build . -j8

## Run all test you want to update or debug

    ctest -R roottest-graphics-macroname_testtype (replace macroname and testtype)

## Update the references files

    # Navigate to the source folder
    cd /path/to/build/roottest/graphics/pro_folder
    # Rename and move files
    for file in *_pro*; do 
        mv "$file" "/path/to/roottest/graphics/ref_folder/${file/_pro/}"; 
    done
    # Navigate to the destination folder
    cd /path/to/roottest/graphics/ref_folder
    # Compress all new ref files
    ls *.fileformat | awk '{print "zip -m " $1 ".zip " $1}' | sh (replace fileformat)




    