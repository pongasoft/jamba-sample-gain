@Echo OFF
set SRC_DIR=%~dp0
set BUILD_DIR=%CD%\build

Echo BUILD_DIR=%BUILD_DIR%
Echo SRC_DIR=%SRC_DIR%

mkdir %BUILD_DIR%
cd %BUILD_DIR%

Echo =============================================================
Echo ==                                                         ==
Echo ==              Generating Makefiles...                    ==
Echo ==              -----------------------                    ==
Echo =============================================================
cmake -G"Visual Studio 15 2017 Win64" %SRC_DIR%

REM Compiling tests
cmake --build . --config Release --target pongasoft_JambaSampleGain_test

Echo =============================================================
Echo ==                                                         ==
Echo ==              Running Tests..........                    ==
Echo ==              -----------------------                    ==
Echo =============================================================
ctest -C Release

REM Compiling Plugin
cmake --build . --config Release --target pongasoft_JambaSampleGain 

REM Compiling validator
cmake --build . --config Release --target validator

Echo =============================================================
Echo ==                                                         ==
Echo ==              Validating Plugin......                    ==
Echo ==              -----------------------                    ==
Echo =============================================================
.\bin\Release\validator.exe VST3\Release\pongasoft_JambaSampleGain.vst3 

Echo =============================================================
Echo ==                                                         ==
Echo ==              Building archive.......                    ==
Echo ==              -----------------------                    ==
Echo =============================================================
cmake --build . --config Release --target archive
