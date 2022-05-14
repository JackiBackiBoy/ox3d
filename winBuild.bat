if not exist build mkdir build
cd build
cmake -S ../ -B . -G "MinGW Makefiles"
make
make Shaders
make install
cd bin
sandbox.exe
cd ../..
