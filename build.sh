#!/bin/sh

# Clean build folder -----------------------------------------------------------
rm -rf build/
rm -rf ressources/
rm r-type_client r-type_server

# Install conan dependencies ---------------------------------------------------

conan profile detect --force

conan install . --output-folder=build --build=missing

# Build project ----------------------------------------------------------------

cd build

cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release

cmake --build .

# Copy executable and ressources -----------------------------------------------

mv ./r-type_client ../

mv ./r-type_server ../

cp  -r ../client/ressources ../

cd ..
