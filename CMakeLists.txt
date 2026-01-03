# CMake最低版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称和基本信息
project(Tetris LANGUAGES C)

# 设置C标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# 设置目标名称
set(TARGET_NAME tetris)

# 设置源代码目录
set(SRC_DIR src)
set(INCLUDE_DIR include)

# 列出所有源文件（在src目录下）
set(SOURCES
    ${SRC_DIR}/main.c
    ${SRC_DIR}/logic.c
    ${SRC_DIR}/board.c
    ${SRC_DIR}/block.c
    ${SRC_DIR}/score.c
)

# 列出所有头文件（在include目录下）
set(HEADERS
    ${INCLUDE_DIR}/tetris.h
)

# 添加头文件包含路径
include_directories(${INCLUDE_DIR})

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 创建可执行文件
add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS})

# Windows特定设置
if(WIN32)
    # 链接必要的Windows库
    target_link_libraries(${TARGET_NAME}
        PRIVATE
        kernel32.lib
        user32.lib
        gdi32.lib
        winspool.lib
        shell32.lib
        ole32.lib
        oleaut32.lib
        uuid.lib
        comdlg32.lib
        advapi32.lib
    )
    
    # 设置Windows子系统（控制台程序）
    set_target_properties(${TARGET_NAME} PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
endif()

# 添加调试和发布配置的编译选项
target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CONFIG:Debug>:-O0 -g>
    $<$<CONFIG:Release>:-O2>
)