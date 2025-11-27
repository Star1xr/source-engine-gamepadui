#!/bin/sh

git submodule init && git submodule update
wget https://dl.google.com/android/repository/android-ndk-r20b-linux-x86_64.zip -o /dev/null
unzip android-ndk-r20b-linux-x86_64.zip
export ANDROID_NDK_HOME=$PWD/android-ndk-r20b
export NDK_HOME=$PWD/android-ndk-r20b
export PATH=$NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH
./waf configure -T release --android=armeabi-v7a,host,21 --togles --disable-warns &&
./waf build
cd build
zip -r ../build.zip .
