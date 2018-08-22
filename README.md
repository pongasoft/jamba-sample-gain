Jamba Sample - Simple Gain plugin
=================================
The goal of this project is to serve as documentation for the [Jamba](https://github.com/pongasoft/jamba) framework. It shows some of the most important features of the framework. The code is heavily documented to explain the various concepts and features. The end result for this project is a plugin similar to the `again` example plugin provided with the VST SDK.

If you just want to build the project then skip to [Configuration](#configuration).

Concepts
--------
Jamba is a set of helpers (classes, concepts, build files, etc...) built on top of the VST SDK to provide a lightweight framework to build a VST2/3 plugin.

### Parameters
VST parameters are what the host DAW deal with (for example a knob is usually tied to a VST parameter). Both the GUI and the RT (Real Time) are "sharing" those parameters. As a result Jamba makes them front and center: you define them in one place and use them in the GUI and RT. Check the file [JSGainPlugin.h](src/cpp/JSGainPlugin.h)


Note: the Jamba framework introduces a new kind of parameters that go beyond VST parameters (which are limited to a double value in the range \[0.0, 1.0]).

### State
Jamba helps you deal with the state. From the parameters that you declare, you then define a `RTState` subclass (for the Real Time / processing code) and a `GUIState` subclass (for the GUI). Jamba then deals with the details of reading/writing the state in a thread safe fashion. Check the file [JSGainPlugin.h](src/cpp/JSGainPlugin.h)

### RT Processor
The Real Time (RT) processing code is where the main logic of the plugin resides. The DAW repeatedly calls the `process` method (actually `processInputs32Bits` or `processInputs64Bits` in Jamba) to process a batch of samples. This is usually called a "frame". The processor uses the `RTState` class. You simply need to inherit from `RTProcessor`. Check the file [JSGainProcessor.h](src/cpp/RT/JSGainProcessor.h)

### GUI Controller
The entry point of the GUI is the GUI controller. Jamba takes care of most of the details of the implementation for you: you simply need to inherit from `GUIController`. The controller uses the `GUIState` class (and makes it available to all the views). Check the file [JSGainController.h](src/cpp/GUI/JSGainController.h).

### Views
Each item in the UI is represented by a view (ex: a label, a knob, a slider, etc...). Jamba makes it very easy to create custom views which can implement complex behavior since they get access to the state. Check the 3 views provided as examples: [JSGainStatsView.h](src/cpp/GUI/JSGainStatsView.h), [JSGainSendMessageView.h](src/cpp/GUI/JSGainSendMessageView.h) and [LinkedSliderView.h](src/cpp/GUI/LinkedSliderView.h).

### Testing
Jamba also helps in providing an out of the box solution for (unit) testing using google test. Check [test-JSGain.cpp](test/cpp/test-JSGain.cpp) (and [CMakeLists.txt](CMakeLists.txt)).

Configuration
-------------
This project is known to work on macOS High Siera 10.13.3 with Xcode 9.2 installed. It also has been tested on Windows 10 64 bits and Visual Studio Build tools (2017). It requires `cmake` version 3.9 at the minimum. Because it uses `cmake` it should work on other platforms but it has not been tested.

### Downloading the SDK
You need to download the VST3 SDK version 3.6.9 from [steinberg](https://download.steinberg.net/sdk_downloads/vstsdk369_01_03_2018_build_132.zip) (shasum 256 => `7c6c2a5f0bcbf8a7a0d6a42b782f0d3c00ec8eafa4226bbf2f5554e8cd764964`). Note that 3.6.10 was released in June 2018 but at this time, this project uses 3.6.9.

### Installing the SDK
Unpack the SDK to the following location (note how I renamed it with the version number)

* `/Users/Shared/Steinberg/VST_SDK.369` for macOS
* `C:\Users\Public\Documents\Steinberg\VST_SDK.369` for windows.

You can also store in a different location and use the `VST3_SDK_ROOT` variable when using cmake to define its location.

### Configuring the SDK
In order to build both VST2 and VST3 at the same time, you need to run the following commands

    # for macOS
    cd /Users/Shared/Steinberg/VST_SDK.369
    ./copy_vst2_to_vst3_sdk.sh

    # for Windows
    cd C:\Users\Public\Documents\Steinberg\VST_SDK.369
    copy_vst2_to_vst3_sdk.bat

Build this project (debug)
--------------------------

The following steps describes (for each platform) how to build the plugin for development (debug mode with logging, etc...)

### macOS

- Create a folder for the build and `cd` to it (for simplicity I am creating it at the root of the source tree, but can obviously be *outside* the source tree entirely):

        mkdir -p build/Debug
        cd build/Debug

- Generate the Makefile(s): provide the path to the *source* of this project (which contains `CMakeLists.txt`):

        cmake -DCMAKE_BUILD_TYPE=Debug ../..

- In order to build the plugin run:

        ./build.sh

- In order to test the plugin (unit tests) run:

        ./test.sh

- In order to validate the plugin (uses validator) run:

        ./validate.sh

- In order to edit the plugin UI (uses editor) run:

        ./edit.sh

- In order to install the plugin locally run (~/Library/Audio/Plug-Ins/VST for VST2 and ~/Library/Audio/Plug-Ins/VST3 for VST3):

        ./install.sh

Because this project uses `cmake` you can also generate an Xcode project by using the proper generator (`-G Xcode`). You can also load the project directly in CLion.

### Windows
- Create a folder for the build and `cd` to it (for simplicity I am creating it at the root of the source tree, but can obviously be *outside* the source tree entirely):

      mkdir build
      cd build

- Generate the Makefile(s): provide the path to the *source* of this project (which contains `CMakeLists.txt`):

      cmake -G"Visual Studio 15 2017 Win64" -DCMAKE_CXX_FLAGS=/D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING ../

- In order to build the plugin run:

        For Debug => ./build.bat
        For Release => ./build.bat Release

- In order to test the plugin (unit tests) run:

        For Debug => ./test.bat
        For Release => ./test.bat Release

- In order to validate the plugin (uses validator) run:

        For Debug => ./validate.bat
        For Release => ./validate.bat Release

- In order to edit the plugin UI (uses editor) run:

        ./edit.sh


- In order to install the plugin:

  For VST2, copy VST3/JambaSampleGain.vst3 and RENAME into JambaSampleGain.dll under
  - C:\ProgramFiles\VstPlugins
  - or any DAW specific path (64bits)
  - MAKE SURE TO RENAME the file otherwise it will not work

  For VST3, copy JambaSampleGain.vst3 under
  - C:\Program Files\Common Files\VST3 (may require admin access)
  - or any DAW specific path (64bits)



Creating archive (release)
--------------------------

A convenient script (`build-prod.sh` for macOS and `build-prod.bat` for Windows) will invoke the proper commands to build and zip the entire project for production release. This can be run in any directory and will create a `build` folder.


Misc
----
- This project uses [loguru](https://github.com/emilk/loguru) for logging (included under `src/cpp/logging`)

Licensing
---------
GPL version 3
