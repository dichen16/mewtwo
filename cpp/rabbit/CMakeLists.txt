cmake_minimum_required(VERSION 3.12)
project(Mewtwo VERSION 1.0.0)

set (CMAKE_CXX_STANDARD 11)

set(Loaded_LIBRARY TestLibrary)
set(Poco_LIBRARY 
    PocoFoundation
    PocoUtil
    PocoNet 
)
set(AMQP_LIBRARY amqpcpp)
set(EV_LIBRARY ev)

set(SAC_LIB_SRCS
    include/Publish.h
    src/Publish.cpp

    include/Receive.h
    src/Receive.cpp
)

include_directories(
    /usr/local/include
    /usr/include
    include/
)

link_directories(
    /usr/local/lib/
    /usr/lib64/
)



add_executable(rabbit main.cpp ${SAC_LIB_SRCS})

target_link_libraries(rabbit ${Poco_LIBRARY} ${AMQP_LIBRARY} ${EV_LIBRARY} -lpthread)
