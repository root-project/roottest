# Recipe for Generating New Reference Files
This guide will help you update and generate new reference JSON, SVG and PDF files required for testing and debugging. Follow the steps below to ensure you are working with the most recent reference files.

IMPORTANT: Reference JSON's are used to generate new SVG's (headless chrome browser). It is important to debug always both test and in case of a need of new references files to generate new ones for both. Therefore it is necessary to first update the json files and then the new svg files.

## 1. Setting up
### 1. Create and Prepare Directories
Start by creating a directory for testing and cloning the necessary repositories.

    mkdir myTesting
    cd myTesting
    git clone https://github.com/root-project/root.git
    git clone https://github.com/root-project/roottest.git

### 2. Build Configuration
Create a build directory, configure the build to include testing and roottest options, and build the project.

    mkdir build
    cd build
    cmake -Dtesting=ON -Droottest=ON ../root
    cmake --build . -j8

## 2. Running Tests
To update or debug tests, run the specific tests you are interested in. Replace macroname and testtype with the actual names used in your tests.

    ctest -R roottest-graphics-macroname_testtype

## 3. Updating Reference Files
### 1. Navigate to the Generated Files Directory
Change to the directory where the new reference files are located. Replace folder_pro with the actual folder name.

    cd /path/to/build/roottest/graphics/folder_pro

### 2. Move and Rename Files
Move and rename the files from the _pro suffix to the appropriate reference folder. Replace folder_ref with the actual folder name.

    for file in *_pro*; do 
        mv "$file" "/path/to/roottest/graphics/folder_ref/${file/_pro/}"; 
    done

### 3. Zip the Updated Reference Files
Change to the reference folder and compress the updated files into zip archives.

    cd /path/to/roottest/graphics/ref_folder
    ls *.fileformat | awk '{print "zip -m " $1 ".zip " $1}' | sh (replace fileformat)

Replace fileformat with the actual file extension of your reference files (e.g., svg, pdf, json).