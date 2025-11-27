#!/bin/sh

git submodule init && git submodule update
wget https://dl.google.com/android/repository/android-ndk-r29-linux.zip -o /dev/null
unzip android-ndk-r29-linux.zip
export ANDROID_NDK_HOME=$PWD/android-ndk-r29
export NDK_HOME=$PWD/android-ndk-r29
export PATH=$NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH
./waf configure -T release --android=armeabi-v7a-hard,host,21 --togles --disable-warns &&
./waf build
cd build
zip -r ../build.zip .
