cmake_minimum_required(VERSION 3.16)
project(algorithms)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
file(GLOB SOURCES "src/*.cpp")

add_library(algorithms STATIC ${SOURCES})

# Include directories
target_include_directories(algorithms PUBLIC include)

foreach(F ${SOURCES})
    get_filename_component(EXE_NAME ${F} NAME_WE)
    add_executable(${EXE_NAME} ${F})
    target_link_libraries(${EXE_NAME} algorithms)
endforeach()


# Add dependencies
# target_link_libraries(${EXE_NAME} algorithms)