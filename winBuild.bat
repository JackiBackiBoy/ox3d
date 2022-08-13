if not exist build mkdir build
cd build
cmake -S ../ -B . -G "MinGW Makefiles"
mingw32-make
mingw32-make Shaders
mingw32-make install
cd bin
sandbox.exe
cd ../..
