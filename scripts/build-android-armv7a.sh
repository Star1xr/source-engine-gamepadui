#!/bin/sh

git submodule init && git submodule update
wget https://dl.google.com/android/repository/android-ndk-r17c-linux-x86_64.zip -o /dev/null
unzip android-ndk-r17c-linux-x86_64.zip
mv $PWD/android-ndk-r17c $PWD/android-ndk-r10e
export ANDROID_NDK_HOME=$PWD/android-ndk-r10e

# This command changes the revision number inside the NDK's source.properties file
# from its original value (e.g., 17.2.4988785) to '10.3.0', which your waf script should recognize.
sed -i 's/Pkg.Revision=.*/Pkg.Revision=10.3.0/g' "$PWD/android-ndk-r10e/source.properties"

export NDK_HOME=$PWD/android-ndk-r10e
export PATH=$NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH
./waf configure -T release --android=armeabi-v7a,host,21 --togles --disable-warns &&
./waf build
cd build
zip -r ../build.zip .
