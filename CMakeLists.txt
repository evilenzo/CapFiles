cmake_minimum_required(VERSION 3.12)

###
### PROJECT
### name and version
###
project(CapFiles VERSION 1.0 LANGUAGES CXX)

###
### CONFIGURATION
###
set(CMAKE_CXX_STANDARD              20)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)

###
### SOURCES
###
add_subdirectory(src)
