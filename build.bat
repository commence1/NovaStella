@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
"C:\Program Files\CMake\bin\cmake.exe" -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="C:\vcpkg\scripts\buildsystems\vcpkg.cmake" -DVCPKG_MANIFEST_INSTALL=ON -S . -B build
"C:\Program Files\CMake\bin\cmake.exe" --build build
