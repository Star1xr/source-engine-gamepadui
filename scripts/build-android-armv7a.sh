#!/bin/sh

git submodule init && git submodule update
wget https://dl.google.com/android/repository/android-ndk-r10e-linux-x86_64.zip -o /dev/null
unzip android-ndk-r10e-linux-x86_64.zip
export ANDROID_NDK_HOME=$PWD/android-ndk-r10e
export NDK_HOME=$PWD/android-ndk-r10e
export PATH=$NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH
sudo apt-get update
sudo apt-get install libatomic1 libgcc-s1 libstdc++6 gcc-multilib g++-multilib ninja-build libfontconfig-dev
./waf configure -T release --android=armeabi-v7a,host,21 --togles --disable-warns &&
./waf build
cd build
zip -r ../build.zip .

