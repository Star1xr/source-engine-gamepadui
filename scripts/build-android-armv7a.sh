#!/bin/sh

# git submodule init && git submodule update
# wget https://dl.google.com/android/repository/android-ndk-r10e-linux-x86_64.zip -o /dev/null
# unzip android-ndk-r10e-linux-x86_64.zip
# export ANDROID_NDK_HOME=$PWD/android-ndk-r10e
# export NDK_HOME=$PWD/android-ndk-r10e
# export PATH=$NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH
# ./waf configure -T release --android=armeabi-v7a,host,21 --togles --disable-warns &&
# ./waf build
# cd build
# zip -r ../build.zip .

set -e

git submodule init && git submodule update

NDK_VERSION="r10e"
NDK_UNZIPPED_NAME="android-ndk-${NDK_VERSION}"
NDK_COMPAT_NAME="android-ndk-r10e"

wget https://dl.google.com/android/repository/android-ndk-r10e-linux-x86_64.zip -o /dev/null
unzip android-ndk-r10e-linux-x86_64.zip

export ANDROID_NDK_HOME=$PWD/android-ndk-r10e
export NDK_HOME=$PWD/android-ndk-r10e
export PATH=$NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH

# Fix 'llvm strip is not executable' error by ensuring permissions are set.
chmod -R +x "$NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin"
# ALSO ensure permissions are set for the critical GNU toolchain binaries
chmod -R +x "$NDK_HOME/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin"



./waf configure -T release --android=armeabi-v7a,hard,host,21 --togles --disable-warns &&
./waf build

cd build
zip -r ../build.zip .
