cmake_minimum_required(VERSION 3.1.0) 

project(WikiXMLDumpXerces)

# set c++ standard to c++11
set (CMAKE_CXX_STANDARD 11)

# look for xerces library
find_package(XercesC REQUIRED)
include_directories(${XercesC_INCLUDE_DIR})

add_executable(count_pages src/examples/countPages/main_countPages.cpp)
target_link_libraries(count_pages ${XercesC_LIBRARY})
target_link_libraries(count_pages pthread)

add_executable(print_titles_paralell src/examples/printTitles/main_printTitleParallel.cpp)
target_link_libraries(print_titles_paralell ${XercesC_LIBRARY})
target_link_libraries(print_titles_paralell pthread)

add_executable(print_titles_single_core src/examples/printTitles/main_printTitleSingleCore.cpp)
target_link_libraries(print_titles_single_core ${XercesC_LIBRARY})
target_link_libraries(print_titles_single_core pthread)
