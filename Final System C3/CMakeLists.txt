cmake_minimum_required(VERSION 3.10)  # Update to at least CMake 3.10
project(my_systemc_project CXX)

# Set the path to your SystemC installation
set(SystemC_ROOT "/opt/systemc")

# Find the SystemC package
find_package(SystemCLanguage CONFIG REQUIRED)

# Set the C++ standard explicitly if SystemC_CXX_STANDARD is not set
if(NOT SystemC_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)  # Default to C++17 if SystemC_CXX_STANDARD is not found
else()
    set(CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Ensure the C++ standard is required

# Add your executable
#add_executable(my_systemc_project main.cpp)
file(GLOB SOURCES "*.cpp")

add_executable(my_systemc_project ${SOURCES})


# Link against the SystemC library
target_link_libraries(my_systemc_project SystemC::systemc)