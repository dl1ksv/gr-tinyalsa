gr-tinyalsa provides a small oot audio sink using tinyalsa instead of alsa-lib.
See 
https://github.com/tinyalsa

It runs on linux and android tested on a nexus 7 with android 5.
At the moment only an audio-sink is available

Mainly to run gnuradio python scripts on android.

1. Installation on Linux

get the latest code from https://github.com/dl1ksv/gr-tinyalsa

The build process is cmake based. So change to code directory.
There

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=<where gnuradio is installed> ../
make
su
make install


If you are unsure about the CMAKE_INSTALL_PREFIX run

gnuradio-config-info --prefix

2. Building for Android
Have a look at the gnuradio branch of my python_for_android fork 

3. To be done

Implementing audio source
