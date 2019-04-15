cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_VERBOSE_MAKEFILE on)

# 源码路径
set(lib_src_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# 编译临时文件路径
set(lib_build_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../tmp)
file(MAKE_DIRECTORY ${lib_build_DIR})
