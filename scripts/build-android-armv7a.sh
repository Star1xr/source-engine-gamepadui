#!/bin/sh

git submodule init && git submodule update
wget https://dl.google.com/android/repository/android-ndk-r10e-linux-x86_64.zip -o /dev/null
unzip android-ndk-r10e-linux-x86_64.zip
export ANDROID_NDK_HOME=$PWD/android-ndk-r10e/
export NDK_HOME=$PWD/android-ndk-r10e/
chmod +x android-ndk-r10e/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-strip
./waf configure -T release --android=armeabi-v7a-hard,host,21 --togles --disable-warns &&
./waf build
cd build
zip -r ../build.zip .

