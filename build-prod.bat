@Echo OFF
set SRC_DIR=%~dp0
set BUILD_DIR=%CD%\build

Echo BUILD_DIR=%BUILD_DIR%
Echo SRC_DIR=%SRC_DIR%

mkdir %BUILD_DIR%
cd %BUILD_DIR%

cmake -G"Visual Studio 15 2017 Win64" -DCMAKE_CXX_FLAGS=/D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING %SRC_DIR%
cmake --build . --config Release --target JambaSampleGain_test
cmake --build . --config Release --target jamba_test
ctest -C Release

cmake --build . --config Release --target archive
