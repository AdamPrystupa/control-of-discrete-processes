cmake_minimum_required(VERSION 3.10)

# Project name and C++ standard
project(ControlOfDiscreteProcesses)
set(CMAKE_CXX_STANDARD 11)

# Add the executable
add_executable(main main.cpp algorithms.cpp)

# Include directories
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Copy the tasks directory to the build directory if it exists
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tasks)
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tasks DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()