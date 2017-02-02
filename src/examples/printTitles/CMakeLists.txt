cmake_minimum_required(VERSION 3.1)
project(WikiXmlDumpXerces CXX)

find_package(XercesC REQUIRED)

add_executable(printTitleSingleCore main_printTitleSingleCore.cpp)
target_link_libraries(printTitleSingleCore xerces-c)

# enforce c++11
target_compile_features(printTitleSingleCore PRIVATE cxx_range_for)
