project(cxlib)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(cxlib)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cxapi.c
)

set(INCLUDE_DIR 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
# set(LIB_DIR ${CMAKE_SOURCE_DIR}/dep/lib)
# set(LIB_NAME m)

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
# target_link_directories(${PROJECT_NAME} PRIVATE ${LIB_DIR})
# target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB_NAME})
