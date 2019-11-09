cmake_minimum_required(VERSION 3.12)
project(Mewtwo VERSION 1.0.0)

set (CMAKE_CXX_STANDARD 11)

add_library(
    TestLibrary SHARED
    TestLibrary.cpp
)

set(Loaded_LIBRARY TestLibrary)
set(Poco_LIBRARY PocoFoundation)

include_directories(
    /usr/local/include
)

link_directories(
    /usr/local/lib/
)


add_executable(LibraryLoaderTest LibraryLoaderTest.cpp)

target_link_libraries(LibraryLoaderTest ${Loaded_LIBRARY} ${Poco_LIBRARY} -lpthread)
