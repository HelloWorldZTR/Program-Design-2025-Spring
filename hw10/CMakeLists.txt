cmake_minimum_required(VERSION 3.10)

project(Rotate)

set(SOURCES main.cpp BitMap.cpp)
set(HEADERS BitMap.h)

add_executable(rotate ${SOURCES} ${HEADERS})

# Publish the executable to the bin directory
set_target_properties(rotate PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
)

add_custom_target(build_all
    COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR} -B ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
    COMMENT "Building the project..."
)
