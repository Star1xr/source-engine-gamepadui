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


# --- FIX 2 & 3: Define, Export Path, and Force Compiler Variables ---
# The NDK r10e's most stable toolchain for armeabi-v7a is GCC 4.9.
NDK_GNU_TOOLCHAIN_BIN="$NDK_HOME/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin"

# 2. Add the toolchain bin directory to PATH so utilities like 'strip' are found.
# down there

# 3. Explicitly set CC and CXX to the full path of the GNU compilers.
# This forces Waf to use the specific compiler for its initial check.
export CC="$NDK_GNU_TOOLCHAIN_BIN/arm-linux-androideabi-gcc"
export CXX="$NDK_GNU_TOOLCHAIN_BIN/arm-linux-androideabi-g++"



# Export the PATH, prioritizing the LLVM path first to resolve the 'llvm-strip' issue.
export PATH="$NDK_LLVM_TOOLCHAIN_BIN:$NDK_GNU_TOOLCHAIN_BIN:$PATH"


./waf configure -T release --android=armeabi-v7a-hard,host,21 --togles --disable-warns &&
./waf build

cd build
zip -r ../build.zip .
