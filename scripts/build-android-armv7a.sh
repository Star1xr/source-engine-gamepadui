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

# 1. LLVM Path (Likely 'llvm-3.6' in r10e)
# If Waf is looking for 'llvm-strip', it needs this path.
NDK_LLVM_TOOLCHAIN_BIN="$NDK_HOME/toolchains/llvm-3.6/prebuilt/linux-x86_64/bin"

# 2. GNU Path (Legacy GCC 4.9 and 4.8)
# Used for prefixed strip/objcopy binaries (e.g., 'arm-linux-androideabi-strip').
NDK_GNU_TOOLCHAIN_49="$NDK_HOME/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin"
NDK_GNU_TOOLCHAIN_48="$NDK_HOME/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin"

# Export the PATH, prioritizing the LLVM path first to resolve the 'llvm-strip' issue.
export PATH="$NDK_LLVM_TOOLCHAIN_BIN:$NDK_GNU_TOOLCHAIN_49:$NDK_GNU_TOOLCHAIN_48:$PATH"


./waf configure -T release --android=armeabi-v7a,hard,host,21 --togles --disable-warns &&
./waf build

cd build
zip -r ../build.zip .
