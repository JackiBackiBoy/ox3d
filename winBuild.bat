if not exist build mkdir build
cd build
cmake -S ../ -B . -G "MinGW Makefiles" -D GLFW_BUILD_DOCS=OFF
mingw32-make
