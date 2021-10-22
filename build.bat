@echo off
if not exist build mkdir build
pushd build
g++ -o ox3d.exe ../src/*.cpp ../src/resources.res
popd
echo Build successful!