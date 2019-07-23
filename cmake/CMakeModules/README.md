# CMakeModules

## 使用方法
1. 将 `FindXxx.cmake` 文件放入项目根目录下的 `cmake/Modules` 目录
2. 将以下内容增加到项目根目录下的 `CMakeLists.txt`

        list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")
        FIND_PACKAGE(BOOST REQUIRED)

3. 最后根据需要执行 `CMake` 命令