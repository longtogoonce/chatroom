cmake_minimum_required(VERSION 3.5)

# 设置项目名称
project(mychatroom)

# 设置编译标准为C++11
set(CMAKE_CXX_STANDARD 11)

# 添加hiredis库头文件搜索路径
include_directories(/usr/include/hiredis)

# 查找hiredis库
find_library(HIREDIS_LIBRARY hiredis)

# 列出所有需要包含的源文件
file(GLOB_RECURSE CLIENT_SOURCES Client/*.cpp)
file(GLOB_RECURSE SERVER_SOURCES Server/*.cpp)
file(GLOB_RECURSE COMMON_HEADERS Common/*.hpp)

# 添加可执行文件的构建目标
add_executable(client ${CLIENT_SOURCES})
add_executable(server ${SERVER_SOURCES})

# 链接hiredis库
target_link_libraries(server ${HIREDIS_LIBRARY})