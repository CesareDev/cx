cmake_minimum_required(VERSION 3.29.2)

# export compile_commmands.json for neovim lsp
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(cx)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.c
)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/include)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/dep/lib)
# set(LIB_NAME )

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PRIVATE ${LIB_DIR})
# target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB_NAME})
