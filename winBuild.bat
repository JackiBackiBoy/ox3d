if not exist build mkdir build
cd build
cmake -S ../ -B . -G "MinGW Makefiles"
make && make Shaders && ox3d.exe
cd ..
pause
