### 步骤

1. 下载ceres, eigen库
2. 修改CMakeLists.txt编译配置
3. 编写编译脚本
4. Android studio 集成动态库

### 下载库文件
[ceres官网下载](http://www.ceres-solver.org/installation.html)

[eigen官网下载](http://eigen.tuxfamily.org/index.php?title=Main_Page)

### 编译准备
创建一个文件夹, 用于存放ceres, eigen解压包, 再创建一个build文件夹, 用于存放编译结果, 目录如下
```
----source
    |
    +---ceres-solver-1.14.0
    |
    +---eigen-3.3.7
    |
    +---build
```

### 编写交叉编译脚本
进入build文件夹, 创建config.sh脚本, 参考[官方文档](https://github.com/ceres-solver/ceres-solver/blob/master/docs/source/installation.rst#android)编写一下脚本
1. 第一行填入自己电脑的cmake路径
2. DCMAKE_TOOLCHAIN_FILE: 使用NDK自带工具链编译, 填入工具链地址
3. DEIGEN_INCLUDE_DIR: 填入eigen项目地址
4. 在配置最后填入ceres项目地址

```
/Users/qiuyongheng/Library/Android/sdk/cmake/3.10.2.4988404/bin/cmake \
-DCMAKE_TOOLCHAIN_FILE=/Users/qiuyongheng/Library/Android/sdk/ndk-bundle/build/cmake/android.toolchain.cmake \
-DEIGEN_INCLUDE_DIR=/Users/qiuyongheng/Downloads/eigen-3.3.7 \
-DANDROID_ABI=armeabi-v7a \
-DANDROID_STL=c++_shared \
-DANDROID_NATIVE_API_LEVEL=android-24 \
-DBUILD_SHARED_LIBS=ON \
-DMINIGLOG=ON \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_C_FLAGS="-s" \
-DCMAKE_C_FLAGS=-std=c99 -Os -fvisibility=hidden \
/Users/qiuyongheng/Downloads/ceres-solver-1.14.0

make clean
make
make install
```

### 开始编译
进入build文件夹, 运行以下命令, 执行编译脚本
```
sh config.sh
```
中途会报以下错误
```
error: 
miniglog/glog/logging.h:201: error: undefined reference to '__android_log_wr
解决方案: 
find_libary也没有解决, 最后直接就注释报错的代码, 不适用log就行了...

error: 
java.lang.UnsatisfiedLinkError: dlopen failed: library "libc++_shared.so" no found
解决方案:
去ndk-bundle目录复制需要平台的libc++_shared.so到项目中即可.
我的目录为/Users/qiuyongheng/Library/Android/sdk/ndk-bundle/sources/cxx-stl/llvm-libc++/libs
```

### 集成到项目中
1. 添加动态库和头文件到项目中
```
生成的动态库和头文件的路径
/usr/local/lib/libceres.so
/usr/local/include/ceres
```
2. 修改CMakeLists.txt
```
cmake_minimum_required(VERSION 3.4.1)

set(jniLibs_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs)
message(STATUS "jniLibs_DIR: ${jniLibs_DIR}")

set(lib_build_DIR $ENV{HOME}/tmp)
message(STATUS "tmp_dir: ${lib_build_DIR}")

add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp)

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# 添加ceres库
add_library(ceres SHARED IMPORTED)
set_target_properties(ceres PROPERTIES IMPORTED_LOCATION
        ${jniLibs_DIR}/${ANDROID_ABI}/libceres.so)


include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/eigen3
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ceres/internal/miniglog
)

# 
target_link_libraries( # Specifies the target library.
        native-lib
        ceres
        ${log-lib})
```
3. 测试
```
extern "C"
JNIEXPORT jstring JNICALL
Java_com_qyh_cerestest_MainActivity_testCeres(JNIEnv *env, jobject instance) {
    std::string hello = "Hello Ceres";

    // 寻优参数x的初始值，为5
    double initial_x = 5.0;
    double x = initial_x;

    // 第二部分：构建寻优问题
    Problem problem;
    CostFunction* cost_function =
            new AutoDiffCostFunction<CostFunctor, 1, 1>(new CostFunctor); //使用自动求导，将之前的代价函数结构体传入，第一个1是输出维度，即残差的维度，第二个1是输入维度，即待寻优参数x的维度。
    problem.AddResidualBlock(cost_function, NULL, &x); //向问题中添加误差项，本问题比较简单，添加一个就行。

    //第三部分： 配置并运行求解器
    Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR; //配置增量方程的解法
    options.minimizer_progress_to_stdout = true;//输出到cout
    Solver::Summary summary;//优化信息
    ceres::Solve(options, &problem, &summary);//求解!!!

    std::cout << summary.BriefReport() << "\n";//输出优化的简要信息
    //最终结果
    std::cout << "x : " << initial_x
              << " -> " << x << "\n";

    return env->NewStringUTF(summary.BriefReport().c_str());
}
```
