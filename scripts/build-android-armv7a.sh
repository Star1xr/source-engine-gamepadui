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

NDK_VERSION="r17c"
NDK_UNZIPPED_NAME="android-ndk-${NDK_VERSION}"
NDK_COMPAT_NAME="android-ndk-r10e"

wget https://dl.google.com/android/repository/android-ndk-${NDK_VERSION}-linux-x86_64.zip -O android-ndk-${NDK_VERSION}-linux-x86_64.zip
unzip -q android-ndk-${NDK_VERSION}-linux-x86_64.zip

# Patch NDK version to 10.3.0 to bypass waf's 'Unknown NDK revision: 17' error.
sed -i 's/Pkg.Revision=.*/Pkg.Revision=10.3.0/g' "$PWD/${NDK_UNZIPPED_NAME}/source.properties"

# Fix 'llvm strip is not executable' error by ensuring permissions are set.
chmod -R +x "$PWD/${NDK_UNZIPPED_NAME}/toolchains/llvm/prebuilt/linux-x86_64/bin"

# Rename the NDK directory to r10e for path compatibility.
mv $PWD/${NDK_UNZIPPED_NAME} $PWD/${NDK_COMPAT_NAME}

export ANDROID_NDK_HOME=$PWD/$NDK_COMPAT_NAME
export NDK_HOME=$PWD/$NDK_COMPAT_NAME
export NDK="$NDK_HOME"
export PATH="$NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH"

./waf configure -T release --android=armeabi-v7a,host,21 --togles --disable-warns &&
./waf build

cd build
zip -r ../build.zip .
