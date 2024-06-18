project(cxlib)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(cxlib)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cxapi.c
)

set(INCLUDE_DIR 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dep/lib)
# set(LIB_NAME )

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/cxapi.h ${CMAKE_CURRENT_BINARY_DIR}/include/cxapi.h COPYONLY
)

add_library(${PROJECT_NAME}_obj OBJECT ${SOURCES})

target_include_directories(${PROJECT_NAME}_obj PRIVATE ${INCLUDE_DIR})
# target_link_directories(${PROJECT_NAME}_obj PRIVATE ${LIB_DIR})
# target_link_libraries(${PROJECT_NAME}_obj PRIVATE ${LIB_NAME})

add_library(${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}_obj>)
add_library(${PROJECT_NAME}_shared SHARED $<TARGET_OBJECTS:${PROJECT_NAME}_obj>)

set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}_shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
