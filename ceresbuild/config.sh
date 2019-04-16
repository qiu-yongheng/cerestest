/Users/qiuyongheng/Library/Android/sdk/cmake/3.10.2.4988404/bin/cmake \
-DCMAKE_TOOLCHAIN_FILE=/Users/qiuyongheng/Library/Android/sdk/ndk-bundle/build/cmake/android.toolchain.cmake \
-DEIGEN_INCLUDE_DIR=/Users/qiuyongheng/Downloads/eigen-3.3.7 \
-DANDROID_ABI=armeabi-v7a \
-DANDROID_STL=c++_shared \
-DANDROID_NATIVE_API_LEVEL=android-21 \
-DBUILD_SHARED_LIBS=ON \
-DMINIGLOG=ON \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_C_FLAGS="-s" \
-DCMAKE_C_FLAGS=-std=c99 -Os -fvisibility=hidden \
/Users/qiuyongheng/Downloads/ceres-solver-1.14.0

make clean
make
make install
