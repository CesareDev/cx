project(cx)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(cx)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/arg_parsing.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/file_handling.c
)

set(INCLUDE_DIR 
    ${CMAKE_CURRENT_SOURCE_DIR}/dep/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
# set(LIB_DIR ${CMAKE_SOURCE_DIR}/dep/lib)
set(LIB_NAME m)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
# target_link_directories(${PROJECT_NAME} PRIVATE ${LIB_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB_NAME})
